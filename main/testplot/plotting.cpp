#include "../include/runclasses.h"
#include "matplot/matplot.h"

int main()
{
    Run run("../../../tcx_files/activity_11871637028.tcx");

    auto x = run.getTimes();
    auto y = run.getSpeeds();

    using namespace matplot;
    plot(x,y);
    hold(on);
    show();

    return 0;
}