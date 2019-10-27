#include <stdlib.h>
#include <stdio.h>
#include "Plot.h"
#include "NNGInterface.h"



int main(void){

	const char* url1 = "tcp://10.20.102.41:10002";
	const char* url2 = "tcp://127.0.0.2:10002";
	const char* url3 = "tcp://127.0.0.3:10002";

	Color Grey{0.5f,0.5f,0.5f,1.0f};
	Color Black{0.0f,0.0f,0.0f,1.0};
	Color Green{0.0,1.0f,0.0f,1.0f};
	Color Red{1.0,0.0f,0.0f,1.0f};
	Color Blue{0.0,0.0f,1.0f,1.0f};


	SmartPtrTrajectories trajectories;
	std::mutex mtx;


	//Vector of the colors used in the plot - Default one is black
	std::vector<Color>trajectories_color;
//	trajectories_color.push_back(Black);
//	trajectories_color.push_back(Green);
	trajectories_color.push_back(Red);




	GLfloat plotWidth = 5.0f;
	GLfloat axisWidth = 2.0f;


    Plot plot(1280,720);
	//test visual code git control

    NNG_Interface nng_interface_1(url1);
  //  NNG_Interface nng_interface_2(url2);
    //NNG_Interface nng_interface_3(url3);


	nng_interface_1.GetTrajectory(&trajectories, &mtx);
	//nng_interface_2.GetTrajectory(&trajectories, &mtx);
	//nng_interface_3.GetTrajectory(&trajectories, &mtx);



    plot.drawPlotNNG(&trajectories,axisWidth,trajectories_color,plotWidth,Grey,"line");




}
