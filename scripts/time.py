import subprocess
import re
import smtplib
import sys
import pickle

command = sys.argv[1:]
max_nw = int(command[-2])
sp = [None]
rep = 1

for nw in range(1, max_nw + 1):
        command[-2] = str(nw)
        time = 0.
        for i in range(rep):
            out = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            sout, serr = out.communicate()
            time += float(re.search("Total time computed in ([0-9.]*)", str(sout)).group(1))
            time_avg = time / rep
        sp.append(float(time_avg)) # my laptop: 14893764 10000 ch: 30338755 1000 ch: 2590559

with open('data.pkl', 'wb+') as write_file:
        pickle.dump(sp, write_file)
print(sp)