import matplotlib.pyplot as plt
import numpy as np
from datetime import datetime
import pandas as pd

import tkinter as tk
from tkinter import filedialog

root = tk.Tk()
root.withdraw()

fname = filedialog.askopenfilename()

sampleType = np.dtype([('timestamp', np.uint32), ('temperature', 'f4')])

log = np.fromfile(fname, dtype=sampleType)

df = pd.DataFrame.from_records(log)
df['timestamp'] = pd.to_datetime(df['timestamp'],unit='s')

df.plot()
plt.ylim(0,40)
plt.show()
