import subprocess
import re
import smtplib
import sys
import random

run = 100
expected = int(sys.argv[4])
count = 0

for nw in range(run):
        sys.argv[-1] = str(int(random.random() * 10000))
        out = subprocess.Popen(sys.argv[1:], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        sout, serr = out.communicate()
        res = int(re.search("Best path found: ([0-9.]*)", str(sout)).group(1))
        
        if res == expected:
            count += 1

print(str(count) + ' / ' + str(run))

