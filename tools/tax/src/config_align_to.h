/*===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
*/

#ifndef CONFIG_ALIGN_TO_H_INCLUDED
#define CONFIG_ALIGN_TO_H_INCLUDED

#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <stdexcept>
#include "log.h"

struct Config
{
	std::string reference, db, dbs, dbss, dbss_tax_list, contig_file, spot_filter_file;
	typedef std::list<std::string> Strings;
	Strings contig_files;
    bool unaligned_only;
    bool hide_counts;

	Config(int argc, char const *argv[])
        : hide_counts(false)
        , unaligned_only(false)
	{
        std::list<std::string> args;
        for (int i = 1; i < argc; ++i) {
            args.emplace_back(argv[i]);
        }

        while (!args.empty()) {
            auto arg = pop_arg(args);
            if (arg == "-db") {
                db = pop_arg(args);
            } else if (arg == "-dbs") {
                dbs = pop_arg(args);
            } else if (arg == "-dbss") {
                dbss = pop_arg(args);
            } else if (arg == "-tax_list") {
                dbss_tax_list = pop_arg(args);
            } else if (arg == "-hide_counts") {
                hide_counts = true;
            } else if (arg == "-unaligned_only") {
                unaligned_only = true;
            } else if (arg == "-list") {
                contig_files = load_list(pop_arg(args));
            } else if (arg == "-spot_filter") {
                spot_filter_file = pop_arg(args);
            } else if (arg.empty() || arg[0] == '-' || !contig_file.empty()) {
                std::string reason = "unexpected argument: " + arg;
                fail(reason.c_str());
            } else {
                contig_file = arg;
            }
        }

        // exactly one should exist
        if (contig_file.empty() == contig_files.empty()) { 
            fail("please provide either contig file or list");
        }

        int db_count = int(!db.empty()) + int(!dbs.empty()) + int(!dbss.empty());
        if (db_count != 1) {
            fail("please provide exactly one db argument");
        }

        // tax list makes sense if and only if dbss specified
        if (dbss.empty() != dbss_tax_list.empty()) {
            fail("-tax_list should be used with -dbss");
        }
        
	}

	static void fail(const char* reason = "invalid arguments")
	{
		print_usage();
        LOG(reason);
        exit(1);
	}

	static void print_usage()
	{
        LOG("need <database> [-spot_filter <spot or read file>] [-hide_counts] [-unaligned_only] <contig fasta or accession>" << std::endl 
            << "where <database> is one of:" << std::endl
            << "-db <database>" << std::endl
            << "-dbs <database +tax>" << std::endl
            << "-dbss <sorted database +tax> -tax_list <tax_list file>")
	}

private:
	static Strings load_list(const std::string &filename)
	{
		Strings lines;
		std::ifstream f(filename);

		while (!f.eof())
		{
			std::string line;
			std::getline(f, line);
			if (!line.empty())
				lines.push_back(line);
		}

		return lines;
	}

    std::string pop_arg(std::list<std::string>& args) const
	{
        if (args.empty()) {
            fail("need more args");
        }
        std::string arg = args.front();
        args.pop_front();
        return arg;
	}
};

#endif
