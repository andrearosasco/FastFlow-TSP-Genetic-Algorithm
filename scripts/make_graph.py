import pickle
import matplotlib.pyplot as plt
import numpy as np


with open('.\\data\\speedup3.pkl', 'rb') as read_file:
    sp = pickle.load(read_file)
sp = [None] + [22410470 / x for x in sp[1:]]
s1 = sp[1] 
sp = [None] + [s1 / x for x in sp[1:]]
sp[9] = s1 / 11614629
sp[10] = s1 / 11422377
sp = sp[:11]
print(sp)

with open('.\\data\\data3.pkl', 'wb') as read_file:
    pickle.dump(sp, read_file)

print(np.max(sp))
print(np.argmax(sp))
plt.plot(sp)
plt.plot([None] + range(1, len(sp)))

plt.savefig('speedup.png')