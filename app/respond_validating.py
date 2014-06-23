#!/usr/bin/python2

import sys
import urllib2

submission_id = sys.argv[1]

respond = urllib2.urlopen("http://0.0.0.0:3000/fetch/validating?key=31415926&sid=" + submission_id)
