#!/usr/bin/python2

import urllib2
from tioj_url import tioj_url, tioj_key

try:
  respond = urllib2.urlopen(tioj_url+"/fetch/submission?key="+tioj_key)
  print respond.read()

except urllib2.URLError, e:
  print -2, e.reason

"""
   print data['id']
   print data['problem_id']
   print data['problem_type']
   print data['user_id']
   print data['compiler']

"""
