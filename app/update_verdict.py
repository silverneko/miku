#!/usr/bin/python2

import sys
import urllib2

submission_id = sys.argv[1]
verdict = sys.argv[2]

respond = urllib2.urlopen("http://0.0.0.0:3000/fetch/write_result?key=31415926&sid="+submission_id+"&result="+verdict)
quit()

#########################################

import MySQLdb
import sys

submission_id = sys.argv[1]
verdict = sys.argv[2]

db = MySQLdb.connect("localhost", "root", "iloveinfor", "tioj_dev");
cursor = db.cursor();
command = "UPDATE submissions SET `result` = '" + verdict + "' "
command += "WHERE `id` = " + str(submission_id)
cursor.execute(command)
db.commit()

db.close()

