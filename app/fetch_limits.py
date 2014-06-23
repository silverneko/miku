#!/usr/bin/python2

import sys
import urllib2

problem_id = sys.argv[1];

respond = urllib2.urlopen("http://0.0.0.0:3000/fetch/testdata_limit?key=31415926&pid=" + problem_id)
print respond.read()
