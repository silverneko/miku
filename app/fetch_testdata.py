#!/usr/bin/python2

import sys
import urllib2
from tioj_url import tioj_url

problem_id = sys.argv[2].zfill(4)
testdata_no = sys.argv[3].zfill(3)

infile = open("./testdata/"+problem_id+"/input"+testdata_no, "w");
outfile = open("./testdata/"+problem_id+"/output"+testdata_no, "w");

respond = urllib2.urlopen(tioj_url+"fetch/testdata?key=31415926&input=&tid=" + sys.argv[1]);
infile.write(respond.read());
respond = urllib2.urlopen(tioj_url+"fetch/testdata?key=31415926&tid=" + sys.argv[1]);
outfile.write(respond.read());
