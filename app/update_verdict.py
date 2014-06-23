#!/usr/bin/python2

import sys
import urllib2

submission_id = sys.argv[1]
verdict = sys.argv[2]
status = sys.argv[3]

respond = urllib2.urlopen("http://0.0.0.0:3000/fetch/write_result?key=31415926&sid="+submission_id+"&result="+verdict+"&status="+status)
