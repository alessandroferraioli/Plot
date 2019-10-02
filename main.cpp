#include <stdlib.h>
#include <stdio.h>
#include "Plot.h"
#include "NNGInterface.h"



int main(void){

	const char* url1 = "tcp://127.0.0.1:10002";
	const char* url2 = "tcp://127.0.0.2:10002";
	const char* url3 = "tcp://127.0.0.3:10002";

	Color grey{0.5f,0.5f,0.5f,1.0f};
	Color black{0.0f,0.0f,0.0f,1.0};
	Color green{0.0,1.0f,0.0f,1.0f};
	Color red{1.0,0.0f,0.0f,1.0f};

	SmartPtrTrajectories trajectories;
	std::mutex mtx;



	//Vector of the colors used in the plot - Default one is black
	std::vector<Color>trajectories_color;
	trajectories_color.push_back(black);
	trajectories_color.push_back(green);
	trajectories_color.push_back(red);



	GLfloat plotWidth = 1.0f;
	GLfloat axisWidth = 2.0f;
	float max_value = 0.0f; // Used to set the max size of the plot


    Plot plot(1280,720);

    NNG_Interface nng_interface_1(url1);
   // NNG_Interface nng_interface_2(url2);
  //  NNG_Interface nng_interface_3(url3);


 /*   std::vector<Trajectory> trajectories;
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



    Trajectory trajectory0;
    trajectory0.points = plot.readCSV(path_0,&max_value);

    Trajectory trajectory1;
    trajectory1.points = plot.readCSV(path_1,&max_value);

    trajectories.push_back(trajectory0);
    trajectories.push_back(trajectory1);

    plot.drawTrajectories(trajectories,axisWidth,plotWidth,max_value,background,colors);

     */


	nng_interface_1.GetTrajectory(&trajectories, &mtx);
	//nng_interface_2.GetTrajectory(&trajectories, &mtx);
	//nng_interface_3.GetTrajectory(&trajectories, &mtx);

    plot.drawPlotNNG(&trajectories,axisWidth,trajectories_color,plotWidth,grey,&mtx);




}
