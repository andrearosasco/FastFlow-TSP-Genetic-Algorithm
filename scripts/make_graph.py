import pickle
import matplotlib.pyplot as plt
import numpy as np

with open('.\\data\\data8.pkl', 'rb') as read_file:
    sp = pickle.load(read_file)

with open('.\\data\\data1.pkl', 'rb') as read_file:
    sp1 = pickle.load(read_file) # normal_form
    
s1 = 25525289
s2 = 25525289

# sp = [None] + [(x * (i + 1))  for i, x in enumerate(sp[1:])]
# sp1 = [None] + [s2 / (x * (i + 1))  for i, x in enumerate(sp1[1:])]
sp = [None] + [s1 / x for x in sp[1:]]
sp1 = [None] + [s2 / x for x in sp1[1:]]

print(np.max(sp))
print(np.argmax(sp))
plt.plot(sp, label='std C++')
plt.plot(sp1, linestyle='dashdot', label="Fastflow")
plt.plot([None] + [x for x in range(1, len(sp))], label="reference", linestyle='--')

plt.xlabel('Number of workers')
plt.ylabel('Speedup')
plt.legend()

plt.savefig('speedup.png')