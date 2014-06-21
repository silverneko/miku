#!/usr/bin/python2

import sys
import urllib2

problem_id = sys.argv[1];

respond = urllib2.urlopen("http://0.0.0.0:3000/fetch/testdata_meta?key=31415926&pid=" + problem_id)
print respond.read()
quit()

##################################################################

import sys
import MySQLdb

problem_id = sys.argv[1];

db = MySQLdb.connect("localhost", "root", "iloveinfor", "tioj_dev");
cursor = db.cursor();
command = "SELECT `id`,UNIX_TIMESTAMP(`updated_at`) FROM testdata ";
command += "WHERE `problem_id` = " + str(problem_id);
command += " ORDER BY `id` ASC";

cursor.execute(command);
data = cursor.fetchall();

print len(data);
for it in data:
   print it[0], it[1];

db.close();

