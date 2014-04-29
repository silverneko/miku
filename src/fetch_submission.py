#!/usr/bin/python

import MySQLdb
import MySQLdb.cursors

db = MySQLdb.connect("localhost", "root", "iloveinfor", "tioj_dev");
cursor = db.cursor(MySQLdb.cursors.DictCursor);
cursor.execute("SELECT `id`, `code`, `compiler`, `problem_id`," \
               "`user_id` FROM submissions WHERE `result` = ''" \
               "ORDER BY `id` ASC LIMIT 1");
data = cursor.fetchone();
if data == None :
   print -1
else :
   print data['id']
   print data['problem_id']
   #print data['user_id']
   print data['compiler']
   print data['code']

db.close()
