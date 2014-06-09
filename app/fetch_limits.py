#!/usr/bin/python2

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

