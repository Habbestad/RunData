from pyRunData import PyRun

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

# Extension of Matplotlib.Axes, to plot data from a run.
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




    

