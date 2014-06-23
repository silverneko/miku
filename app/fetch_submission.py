#!/usr/bin/python2

import urllib2

try:
  respond = urllib2.urlopen("http://0.0.0.0:3000/fetch/submission?key=31415926")
  print respond.read()

except urllib2.URLError, e:
  print -2, e.reason

"""
   print data['id']
   print data['problem_id']
   print data['user_id']
   print data['compiler']
   print data['code']

"""
