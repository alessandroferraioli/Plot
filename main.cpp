#include <stdlib.h>
#include <stdio.h>
#include "Plot.h"
#include "NNGInterface.h"


void launch_interface(const char * url,Trajectory **shared_trajectory,std::mutex* mtx){
	printf("Started the NNG interface's thread\n");
    NNG_Interface nng_interface(url,shared_trajectory,mtx);
}

void launch_plot(Plot plot, Trajectory* trajectory,GLfloat axisWidth,Color colorPlot,GLfloat plotWidth, Color background, std::mutex *mtx ){
	printf("Started the Plot's thread\n");

    plot.drawPlotNNG(trajectory, axisWidth,colorPlot, plotWidth, background,mtx);

}


int main(void){

	const char* url = "tcp://127.0.0.1:10002";

	Color background{0.5f,0.5f,0.5f,1.0f};
	Color colorPlotBlack{0.0f,0.0f,0.0f,1.0};

	Trajectory * shared_trajectory = new Trajectory;

	std::mutex mtx;

	GLfloat plotWidth = 5.0f;
	GLfloat axisWidth = 2.0f;
	float max_value = 0.0f; // Used to set the max size of the plot




    Plot plot(1280,720);

 /* std::vector<Trajectory> trajectories;
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

    trajectories.push_back( trajectory0);
    trajectories.push_back(trajectory1); */

    std::thread thrd_nng_interface(launch_interface,url,&shared_trajectory,&mtx);
    std::thread thrd_plot_nng(launch_plot,plot,shared_trajectory, axisWidth, colorPlotBlack,plotWidth, background,&mtx);

    thrd_nng_interface.join();
    thrd_plot_nng.join();

    //NNG_Interface nng_interface(url,&shared_trajectory,&mtx);


    //plot.drawPlot(trajectories,axisWidth,plotWidth,max_value,background,colors);

    //plot.drawPlotNNG(url,axisWidth,plotWidth,background);

    exit(EXIT_SUCCESS);
}
