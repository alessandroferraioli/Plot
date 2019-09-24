#include <stdlib.h>
#include <stdio.h>
#include "Plot.h"




int main(void){

	const char* url = "tcp://127.0.0.1:10002";

	std::vector<Trajectory> trajectories;
    Trajectory trajectory_1;

    std::string path_0 = "/home/alessandro/workspace/Plot/CartesianTrajectory_0.csv";
    std::string path_1 = "/home/alessandro/workspace/Plot/CartesianTrajectory_1.csv";
    std::string path_2 = "/home/alessandro/workspace/Plot/CartesianTrajectory_2.csv";


    Color trajectoryCol_0{1.0f,1.0f,1.0f,1.0f};
    Color trajectoryCol_1{0.0f,0.0f,0.0f,1.0f};
    Color trajectoryCol_2{0.0f,1.0f,0.0f,1.0f};


    std::vector<Color> colors;
    colors.push_back(trajectoryCol_0);
    colors.push_back(trajectoryCol_1);
    colors.push_back(trajectoryCol_2);



    GLfloat plotWidth = 20.0f;
    GLfloat axisWidth = 2.0f;
    float max_value = 0.0f; // Used to set the max size of the plot

    Color background{0.5f,0.5f,0.5f,1.0f};


	Plot plot(1280,720);
	/*trajectories.push_back( plot.readCSV(path_0,&max_value));
	trajectories.push_back( plot.readCSV(path_1,&max_value));
	trajectories.push_back( plot.readCSV(path_2,&max_value));

	plot.drawPlot(trajectories,axisWidth,plotWidth,max_value,background,colors);*/

	plot.drawPlotNNG(url,axisWidth,plotWidth,background);



    exit(EXIT_SUCCESS);


}
