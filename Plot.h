#ifndef PLOT_H
#define PLOT_H


#include <GLFW/glfw3.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <pthread.h>

#include <thread>
#include <mutex>
#include <unistd.h>



#include <json11.hpp>

#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>

#define _USE_MATH_DEFINES // M_PI constant

typedef struct{
    GLfloat r, g, b, a;
}Color;

typedef struct{
	GLfloat  x, y, z;
}Point;


typedef struct{
	Point point;
	Color color{1.0f,1.0f,1.0f,1.0f};
}Vertex;

typedef std::vector<Vertex> Trajectory;


typedef struct{
        int width;
        int height;
}Window_params;


class Plot{

public:
    Plot(int width, int height);

    void drawPlot(std::vector<Trajectory> trajectory,GLfloat axisWidth,GLfloat plotWidth,
    				float max_value,Color backgroundColor,std::vector<Color>);

    void drawPlotNNG(const char* url,GLfloat axisWidth,GLfloat plotWidth,Color backgroundColor);
    std::vector<Vertex> readCSV(std::string,float* max_value);

    GLFWwindow* window;
    Window_params window_params;

   // nng_socket sock;
    //int rv;



private:

    void drawPoint(Vertex ,GLfloat );
    void drawFloor(float max_value);
    void drawOrigin(float max_value,GLfloat axisThickness);
    void drawLine(Vertex start,Vertex end,float lineWidth);


    void waitConnection(const char* url);
    void nngPlot(Trajectory* trajectory,GLfloat axisWidth,GLfloat plotWidth,Color backgroundColor);

    Vertex readMsg();
    void getMessage(Trajectory* trajectory);

    nng_socket sock;
    int rv;






};


#endif
