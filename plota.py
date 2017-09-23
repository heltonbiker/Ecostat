import matplotlib.pyplot as plt
import numpy as np
from datetime import datetime

import tkinter as tk
from tkinter import filedialog

root = tk.Tk()
root.withdraw()

fname = filedialog.askopenfilename()

sampleType = np.dtype([('timestamp', np.uint32), ('temperature', 'f4')])

log = np.fromfile(fname, dtype=sampleType)

tss = log['timestamp']
times = [datetime.fromtimestamp(stamp) for stamp in tss]
temps = log['temperature']

plt.plot(times, temps)
plt.ylim(0,40)
plt.show()