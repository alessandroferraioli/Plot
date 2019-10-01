#include <stdlib.h>
#include <stdio.h>
#include "Plot.h"
#include "NNGInterface.h"



int main(void){

	const char* url = "tcp://127.0.0.1:10002";
	const char* url2 = "tcp://127.0.0.2:10004";

	Color grey{0.5f,0.5f,0.5f,1.0f};
	Color black{0.0f,0.0f,0.0f,1.0};
	Color green{0.0,1.0f,0.0f,1.0f};

   SmartPtr<Trajectory> shared_trajectory(new Trajectory);
   SmartPtr<Trajectory> shared_trajectory2(new Trajectory);

	std::mutex mtx;
	std::mutex mtx2;


	GLfloat plotWidth = 1.0f;
	GLfloat axisWidth = 2.0f;
	float max_value = 0.0f; // Used to set the max size of the plot


    Plot plot(1280,720);

    NNG_Interface nng_interface(url);
    NNG_Interface nng_interface_2(url2);




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



    nng_interface.GetTrajectory(&shared_trajectory, &mtx);
    nng_interface_2.GetTrajectory(&shared_trajectory2, &mtx2);

    plot.drawPlotNNG(&shared_trajectory,axisWidth,black,plotWidth,grey,&mtx);
    plot.drawPlotNNG(&shared_trajectory2,axisWidth,black,plotWidth,grey,&mtx2);


}
