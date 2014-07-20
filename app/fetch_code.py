#!/usr/bin/python2

import sys
import urllib2
from tioj_url import tioj_url, tioj_key

submission_id = sys.argv[1]

respond = urllib2.urlopen(tioj_url+"fetch/code?key="+tioj_key+"&sid=" + submission_id)
print respond.read()

