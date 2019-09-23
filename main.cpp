#include <stdlib.h>
#include <stdio.h>
#include "Plot.h"




int main(void){



    std::vector<Vertex> trajectory;
    std::string path = "/home/alessandro/workspace/Plot/CartesianTrajectory.csv";
    GLfloat size = 5.0f;
    float max_value;


	Plot plot(1280,720);
	trajectory = plot.readCSV(path,&max_value);
	plot.drawPlot(trajectory,size,max_value);

    exit(EXIT_SUCCESS);


}
