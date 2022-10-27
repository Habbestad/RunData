import py_wrappers as wr
import matplotlib.pyplot as plt
import numpy as np

run = wr.PyStandardRun(b"/users/erikhabbestad/cpp_prosjekter/RunData/ctx_reader/files_by_date/2021-10-05T05:41:33.000Z.tcx")


# speedToPace() transforms a number representing speed (m/s)
# into a string with corresponding min/km
def speedToPace(speed):
    if speed <= 0:
        return '0.0'

    pace = 1/(speed*0.06)
    min = np.floor(pace)
    sec = (pace - min)*0.6
    sec = round(sec,2)

    return str(min + sec)


def lapSpeedAndPace(lap_times, lap_distances):
    lap_speeds = []
    lap_pace = []
    for i in range(len(lap_times)):
        lap_speeds.append( lap_distances[i]/lap_times[i] )
        lap_pace.append(speedToPace(lap_speeds[i]))
    
    return lap_speeds, lap_pace



def plotRun(a_run, laplength = 1000):

    fig, ax1 = plt.subplots(2,1) 
    fig.subplots_adjust(bottom = 0.3) # larger table boxes

    # The title is simply the date of the run
    title = str(a_run.getDate())
    title = title[10:12] + "-" + title[7:9] + "-" + title[2:6] # Swap from YYYY-MM-DD to DD-MM-YYYY
    fig.suptitle(title)

    # Create a table with summary time, distance and pace:
    # load data:
    total_dist = a_run.getTotalDistance()
    total_time = a_run.getTotalTime()
    avg_pace = speedToPace(total_dist/total_time)

    # create table:
    table = [ str(round(total_time/60,2)) + " min"
            , str(round(total_dist/1000, 2)) + " km",
              avg_pace + " min/km"]
    labels = ("Total time", "Total distance:", "Avg pace")
    ax1[0].table(cellText = [table], colLabels = labels,loc = 'top', cellLoc = 'center')


    # Plot pace and altitude against time in the same window:
    # load data:
    times = [ round(t/60,2) for t in a_run.getTimes() ]
    speed = a_run.getSpeeds()
    alt = a_run.getAltitudes()

    # create the speed plot:
    ax1[0].set_xlabel('Time (min)')
    ax1[0].set_ylabel('Pace (min/km)')
    ax1[0].set_ylim(0,max(speed)+0.5)
    color = 'xkcd:sky blue'
    ax1[0].plot(times, speed, color = color)
    ax1[0].tick_params(axis='y', labelcolor = color) #speed values get same color as speed graph
    ax1[0].fill_between(times,speed, alpha = 0.4)

    # To get min/km instead of m/s we format the values on the y-axis:
    formatter = plt.FuncFormatter(lambda speed, pos : speedToPace(speed))
    ax1[0].yaxis.set_major_formatter(formatter)

    # Altitude in same window but with different y-axis
    ax2 = ax1[0].twinx()
    ax2.set_ylabel('Altitude (meters)')
    color = 'tab:orange'
    ax2.plot(times,alt, color = color)
    ax2.tick_params(axis='y', labelcolor = color)
    

    # Second subplot consists of the laps (determined by the parameter 'laplength')
    # load data:

    lap_times = a_run.getLapTimes(laplength)

    # Need this for accuracy (laps are not exactly laplength)
    lap_distances = a_run.getLapDistances(laplength)  

    # want speed (m/s) for plot and pace (min/km) for labels
    lap_speeds, lap_pace = lapSpeedAndPace(lap_times, lap_distances) 

    # plot the line piece y = lap_speed[k] between x_min (meters past at lap begining) 
    # and x_max (meters past at lap end). We also draw vertical lines at x_min and x_max
    x_min = 0
    x_max = 0
    for k in range(len(lap_speeds)):
        x_max += lap_distances[k]
        cur_speed = lap_speeds[k]

        ax1[1].hlines(y = cur_speed, xmin = x_min, xmax = x_max)
        ax1[1].vlines(x_min, ymin = 0, ymax = cur_speed)
        ax1[1].vlines(x_max, ymin = 0, ymax = cur_speed)

        # lap pace on top of lap line (but short laps don't need label):
        if (x_max - x_min) > laplength/2:
            ax1[1].text(x = x_min+200, y = cur_speed + 0.02, s=lap_pace[k])

        ax1[1].fill_between([x_min, x_max], cur_speed, hatch = '/', facecolor = 'g', alpha = 0.4 ) # aesthetics

        x_min = x_max

    # set the axis labels and limits
    ax1[1].set_xlabel('Distance (meters)')
    ax1[1].set_ylabel('Lap pace (min)')
    ax1[1].set_ylim(2.5,max(lap_speeds)+0.2)
    ax1[1].yaxis.set_major_formatter(formatter) # Want min/km instead of m/s here as well (see above)


    fig.tight_layout()
    plt.show()

plotRun(run)



    
