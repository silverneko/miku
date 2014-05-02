#!/usr/bin/python2

import MySQLdb
import sys

db = MySQLdb.connect("localhost", "root", "iloveinfor", "tioj_dev");
cursor = db.cursor();
command = "SELECT `input`, `answer` FROM testdata WHERE `id` = "
command += str(sys.argv[1]) + " LIMIT 1"
cursor.execute(command);
data = cursor.fetchone();

problem_id = sys.argv[2].zfill(4)
testdata_no = sys.argv[3].zfill(3)

infile = open("./testdata/"+problem_id+"/input"+testdata_no, "w");
infile.write(data[0]);
outfile = open("./testdata/"+problem_id+"/output"+testdata_no, "w");
outfile.write(data[1]);

db.close()

