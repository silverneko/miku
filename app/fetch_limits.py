#!/usr/bin/python2

import sys
import urllib2

problem_id = sys.argv[1];

respond = urllib2.urlopen("http://0.0.0.0:3000/fetch/testdata_limit?key=31415926&pid=" + problem_id)
print respond.read()
quit()

##################################################################

import sys
import MySQLdb

problem_id = sys.argv[1];

db = MySQLdb.connect("localhost", "root", "iloveinfor", "tioj_dev");
cursor = db.cursor();
command = "SELECT `time`, `memory` FROM limits ";
command += "WHERE `problem_id` = " + str(problem_id);
command += " LIMIT 1";

cursor.execute(command);
data = cursor.fetchone();

print data[0], data[1]

db.close();

