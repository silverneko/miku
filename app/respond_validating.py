#!/usr/bin/python2

import sys
import urllib2
from tioj_url import tioj_url

submission_id = sys.argv[1]

respond = urllib2.urlopen(tioj_url+"fetch/validating?key=31415926&sid=" + submission_id)
