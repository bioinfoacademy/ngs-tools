#!/opt/python-2.7env/bin/python
import sys
#import argparse

def median_of(v):
	v = list(v)
	v.sort()
	return v[len(v)/2]

def is_break_point(line, pos, level):
	LOW_CONNECTIVITY = 70
	BEGINNING = 70
	END = 140
	if pos < BEGINNING:
		if line[pos] < LOW_CONNECTIVITY/3:
			return True
		else:
			return False

	distance_to_end = len(line) - pos
	if distance_to_end < END:
		if line[pos] < distance_to_end - LOW_CONNECTIVITY and line[pos] < level / 2:
			return True
		else:
			return False

	if line[pos] < LOW_CONNECTIVITY and line[pos] < level / 2:
		return True

	return False

def main():
	if len(sys.argv) < 2:
		print >> sys.stderr, "need <connectivity file>"
		return

	f = open(sys.argv[1])
	level = None		
	for line in f:
		line = line.split()
		line = [int(x) for x in line]
		if level == None:
			level = median_of(line)
			print >> sys.stderr, "level is ", level

		break_points = []
		for pos in xrange(len(line)):
			if is_break_point(line, pos, level):#pos > 70 and line[pos] < 70 and line[pos] < level / 2 and pos < len(line) - 140: # level: # todo: probably -level*2/3
				break_points.append((pos, line[pos]))

		print len(break_points),
		for pos, conn in break_points:
			print pos, conn,

		print ""
			
if __name__ == "__main__":
	main()
