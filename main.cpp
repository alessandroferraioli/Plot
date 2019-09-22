#include <stdlib.h>
#include <stdio.h>
#include "Plot.h"

//test 2  commit


int main(void){

	Window_params params{1280,720};
    std::vector<Vertex> trajectory;
    std::string path = "CartesianTrajectory.csv";


    GLfloat size= 5;

	Plot plot(params,path);
   // trajectory = plot.readCSV(path);
    //plot.drawPlot(trajectory,size);

    exit(EXIT_SUCCESS);


}
