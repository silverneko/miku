#!/usr/bin/python2

import sys
import urllib2
from tioj_url import tioj_url

problem_id = sys.argv[1];

respond = urllib2.urlopen(tioj_url+"fetch/testdata_meta?key=31415926&pid=" + problem_id)
print respond.read()
