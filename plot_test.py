import py_wrappers as wr
import matplotlib.pyplot as plt
import matplotlib.widgets as mwidgets
import numpy as np

# Some convenience functions
def speedToPaceString(speed):
    if speed <= 0:
        return '0.0'

    pace = 1/(speed*0.06)
    min = np.floor(pace)
    sec = (pace - min)*0.6
    sec = round(sec,2)

    return str(round(min + sec,2))

def lapSpeedAndPace(lap_times, lap_distances):
    lap_speeds = []
    lap_pace_string = []
    for i in range(len(lap_times)):
        lap_speeds.append( lap_distances[i]/lap_times[i] )
        lap_pace_string.append(speedToPaceString(lap_speeds[i]))
    
    return lap_speeds, lap_pace_string


class RunPlotter:

    def __init__(self, run, axs):
        self.run = run
        self.axs = axs

    def plotSpeed(self, pos = 0):
        times = [ round(t/60,2) for t in self.run.getTimes() ] # time in minutes
        speed = self.run.getSpeeds()
        self.axs[pos].set_xlabel('Time (min)')
        self.axs[pos].set_ylabel('Pace (min/km)')
        self.axs[pos].set_ylim(0,max(speed)+0.5)
        color = 'xkcd:sky blue'
        self.axs[pos].plot(times, speed, color = color)
        self.axs[pos].tick_params(axis='y', labelcolor = color) #speed values get same color as speed graph
        self.axs[pos].fill_between(times,speed, alpha = 0.4)

        # To get min/km instead of m/s we format the values on the y-axis:
        formatter = plt.FuncFormatter(lambda speed, pos : speedToPaceString(speed))
        self.axs[pos].yaxis.set_major_formatter(formatter)

    def plotTable(self, pos = 0, loc = 'top'):
        rows = ['Total Time', 'Total Distance', 'Avg. pace', 'Avg. HR']
        time = self.run.getTotalTime()
        distance = self.run.getTotalDistance()
        values = [str(round(np.floor(time/60) + (time/60 - np.floor(time/60))*0.6,2)) + " min",
                  str(round(distance/1000, 2)) + " km", 
                  speedToPaceString(distance/time) + " min/km", 
                  np.round(np.average(self.run.getHR()),2) ]
        table = self.axs[pos].table(cellText = [values], colLabels = rows, loc = loc)
        table.auto_set_font_size(False)
        table.set_fontsize(10)
    
    def plotAltitude(self, pos = 0):
        times = [round(t/60,2) for t in self.run.getTimes() ]
        alt = self.run.getAltitudes()
        ax2 = self.axs[pos].twinx()
        ax2.set_ylabel('Altitude (meters)')
        color = 'tab:orange'
        ax2.tick_params(axis='y', labelcolor = color)
        ax2.plot(times, alt, color = color)

    def plotLaps(self, laplength = 1000, pos = 0):
        lap_times, lap_distances = self.run.getLaps(laplength)

        # want speed (m/s) for plot and pace (min/km) for labels
        lap_speeds, lap_pace_string = lapSpeedAndPace(lap_times, lap_distances) 

        # plot the line piece y = lap_speed[k] between x_min (meters past at lap begining) 
        # and x_max (meters past at lap end). We also draw vertical lines at x_min and x_max
        x_min = 0
        x_max = 0
        for k in range(len(lap_speeds)):
            x_max += lap_distances[k]
            cur_speed = lap_speeds[k]

            self.axs[pos].hlines(y = cur_speed, xmin = x_min, xmax = x_max)
            self.axs[pos].vlines(x_min, ymin = 0, ymax = cur_speed)
            self.axs[pos].vlines(x_max, ymin = 0, ymax = cur_speed)

            # lap pace on top of lap line (but short laps don't need label):
            if (x_max - x_min) > laplength/2:
                self.axs[pos].text(x = x_min+200, y = cur_speed + 0.02, s=lap_pace_string[k])

            self.axs[pos].fill_between([x_min, x_max], cur_speed, hatch = '/', facecolor = 'g', alpha = 0.4 ) # aesthetics

            x_min = x_max

        # set the axis labels and limits
        self.axs[pos].set_xlabel('Distance (meters)')
        self.axs[pos].set_ylabel('Lap pace (min)')
        self.axs[pos].set_ylim(2.5,max(lap_speeds)+0.2)
        formatter = plt.FuncFormatter(lambda speed, pos : speedToPaceString(speed))
        self.axs[pos].yaxis.set_major_formatter(formatter) # Want min/km instead of m/s here as well (see above)

# ---- Examples ------
#Load a run:
run = wr.PyRun(b"/Users/erikhabbestad/Documents/prosjekter/RunData/tcx_files/activity_12087970552.tcx")

# --- Simple figure with 2 Axes ---
fig, ax = plt.subplots(2,1)
plotter = RunPlotter(run, ax)
plotter.plotSpeed()
plotter.plotTable()
plotter.plotLaps(pos = 1)
plt.show()

# ---- An interactive figure ----
# To try: add some buttons to turn on and off the various plots.
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


    





"""
class RunFigure:
    
    This class just "hides" matplotlib by holding the Figure object and the Axes object
    that is needed to make a figure from the RunPlotter. 
    

    def __init__(self, run, nrows = 2, ncols = 1):
        self.run = run
        self.fig, self.ax = plt.subplots(nrows, ncols)
        self.run_plots = RunPlotter(run, self.ax)

    def getMatplotlibFig(self):
        return self.fig, self.ax

    def plotSpeed(self, pos = 0):
        self.run_plots.plotSpeed(pos = pos)

    def plotTable(self, pos = 0):
        self.run_plots.plotTable(pos = pos)

    def plotAltitude(self, pos = 0):
        self.run_plots.plotAltitude(pos = pos)

    def plotLaps(self, laplength = 1000, pos = 1):
        self.run_plots.plotLaps(laplength = laplength, pos = pos)
    
    def show(self):
        plt.show()


class SectionPlotter(RunFigure):

    This small class shows an example of how one can make an interactive figure.
    At the moment it is not customizable, but is nice to encapsulate the SpanSelector and 
    the callback function (plotSpan())
    

    def __init__(self, run):
        super().__init__(run, 2, 2)
        self.fig, self.ax = self.getMatplotlibFig()

        self.span = mwidgets.SpanSelector(self.ax[0,0], self.plotSpan, 'horizontal', interactive = True,
                             props=dict(facecolor='blue', alpha=0.5))
        
    def plotSpan(self, t1, t2):
        if t1 == t2:
            return
        self.ax[0,1].clear()
        self.ax[1,1].clear()
        section = self.run.getSectionByTime(t1*60, t2*60, b"test")  #Time must be in seconds here
        sec = RunPlotter(section, self.ax)
        sec.plotSpeed(pos = (0,1))
        sec.plotLaps(pos = (1,1))
        sec.plotTable(pos = (0,1))
    
    def plotRun(self):
        super().plotSpeed(pos = (0,0))
        super().plotTable(pos = (0,0))
        super().plotLaps(pos = (1,0))
"""


"""
        self.button_labels = ['Altitude']
        self.buttons = mwidgets.CheckButtons(self.ax[0].inset_axes([0.0, 0.0, 0.12, 0.2]),
                                    labels = self.button_labels,
                                    actives = [False])

        #self.altitude_button = Button(self.ax[1].inset_axes([0.0, 0.0, 0.0, 0.0]), label = 'Altitude')
        self.altitude_ax = self.altitude()
        self.altitude_ax.set_visible(False)

        self.cid = self.buttons.on_clicked(self.callback)

    def callback(self, label):
        if label == 'Altitude':
            self.altitude_ax.set_visible(not self.altitude_ax.get_visible())
            self.altitude_ax.draw_idle()


"""