#!/usr/bin/python2

import sys
import urllib2
from tioj_url import tioj_url, tioj_key

submission_id = sys.argv[1]
verdict = sys.argv[2]
status = sys.argv[3]

respond = urllib2.urlopen(tioj_url+"fetch/write_result?key="+tioj_key+"&sid="+submission_id+"&result="+verdict+"&status="+status)
