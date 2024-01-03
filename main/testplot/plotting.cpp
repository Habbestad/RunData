#include "../include/runclasses.h"
#include "matplot/matplot.h"

int main()
{
    Run run("../../../tcx_files/activity_11871637028.tcx");

    Run section = run.getSectionByDistance(3000, 10200, " Intervals");
    auto x = section.getDistances();
    auto y = section.getSpeeds();
    
    using namespace matplot;
    plot(x, y);
    title(section.getId());
    xlabel("Distance (m)");
    ylabel("Speed (m/s)");
    hold(on);
    show();

    return 0;
}