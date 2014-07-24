#!/usr/bin/python2

import sys
import urllib2
from tioj_url import tioj_url, tioj_key

problem_id = sys.argv[1]

respond = urllib2.urlopen(tioj_url+"/fetch/sjcode?key="+tioj_key+"&pid=" + problem_id)
print respond.read()

