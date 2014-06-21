#!/usr/bin/python2

import sys
import urllib2

problem_id = sys.argv[2].zfill(4)
testdata_no = sys.argv[3].zfill(3)

infile = open("./testdata/"+problem_id+"/input"+testdata_no, "w");
outfile = open("./testdata/"+problem_id+"/output"+testdata_no, "w");

respond = urllib2.urlopen("http://0.0.0.0:3000/fetch/testdata?key=31415926&input=&tid=" + sys.argv[1]);
infile.write(respond.read());
respond = urllib2.urlopen("http://0.0.0.0:3000/fetch/testdata?key=31415926&tid=" + sys.argv[1]);
outfile.write(respond.read());

quit()

#######################################################

import MySQLdb
import sys
import urllib2

db = MySQLdb.connect("localhost", "root", "iloveinfor", "tioj_dev");
cursor = db.cursor();
command = "SELECT `test_input`, `test_output` FROM testdata WHERE `id` = "
command += str(sys.argv[1]) + " LIMIT 1"
cursor.execute(command);
data = cursor.fetchone();

problem_id = sys.argv[2].zfill(4)
testdata_no = sys.argv[3].zfill(3)

infile = open("./testdata/"+problem_id+"/input"+testdata_no, "w");
outfile = open("./testdata/"+problem_id+"/output"+testdata_no, "w");

respond = urllib2.urlopen("http://0.0.0.0:3000/uploads/td/"+sys.argv[2]+"/"+data[0]);
infile.write(respond.read());
respond = urllib2.urlopen("http://0.0.0.0:3000/uploads/td/"+sys.argv[2]+"/"+data[1]);
outfile.write(respond.read());

db.close()

