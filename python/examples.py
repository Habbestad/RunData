import matplotlib.pyplot as plt
import matplotlib.widgets as mwidgets

from pyRunData import PyRun
from RunPlotter import RunPlotter

# ---- Examples ------
#Load a run:
run = PyRun(b"/Users/erikhabbestad/Documents/prosjekter/RunData/tcx_files/activity_11991473329.tcx")
print(run)

# --- Simple figure with 2 Axes ---
fig, ax = plt.subplots(2,1)
plotter = RunPlotter(run, ax)
plotter.plotSpeed()
plotter.plotTable()
plotter.plotLaps(pos = 1)
plotter.plotAltitude()
plt.show()

# ---- An interactive figure, using the getSection() method ----
fig, ax = plt.subplots(2,2)
plotter = RunPlotter(run, ax)

def plotSpan(t1, t2):
    if t1 == t2:
        return
    ax[0,1].clear()
    ax[1,1].clear()
    section = run.getSectionByTime(t1*60, t2*60, b"test")  #Time must be in seconds here
    sec = RunPlotter(section, ax)
    sec.plotSpeed(pos = (0,1))
    sec.plotLaps(pos = (1,1))
    sec.plotTable(pos = (0,1))

span = mwidgets.SpanSelector(ax[0,0], plotSpan, 'horizontal', interactive = True,
                             props=dict(facecolor='blue', alpha=0.5))
        
plotter.plotSpeed(pos = (0,0))
plotter.plotTable(pos = (0,0))
plotter.plotLaps(pos = (1,0))

plt.show()