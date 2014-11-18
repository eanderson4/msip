#!./python273/bin/python

import os
import platform
import sys
import time
import shutil

print >> sys.stderr, __doc__
print "Version :", platform.python_version()
print "Program :", sys.executable
print 'Script  :', os.path.abspath(__file__)
print

print "I Have Arrived"

shutil.copy2('additions.cap','add.cap')

f = open('ARRIVE','w')
print >>f,'just','starting'
sys.exit(0)
