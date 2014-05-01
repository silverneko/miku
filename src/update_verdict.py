#!/usr/bin/python2

import MySQLdb
import sys

submission_id = sys.argv[1]
verdict = sys.argv[2]

if verdict == 'WA':
   verdict = 'Wrong Answer'
elif verdict == 'AC':
   verdict = 'Accepted'

db = MySQLdb.connect("localhost", "root", "iloveinfor", "tioj_dev");
cursor = db.cursor();
command = "UPDATE submissions SET `result` = '" + verdict + "' "
command += "WHERE `id` = " + str(submission_id)
cursor.execute(command)
db.commit()

db.close()

