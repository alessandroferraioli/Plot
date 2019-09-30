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


void fatal(const char *func, int rv);

typedef struct{
    GLfloat r, g, b, a;
}Color;

typedef struct{
	GLfloat  x, y, z;
}Point;


typedef struct{
	Point point;
	Color color{1.0f,1.0f,1.0f,1.0f}; //TODO: Associa il colore alla traiettoria anziché al punto
}Vertex;

//typedef std::vector<Vertex> Trajectory;

typedef std::vector<Point> Points;

typedef struct{
	Points points;
	Color color;
	GLfloat width;
}Trajectory;



typedef struct{
        int width;
        int height;
}Window_params;


class Plot{

public:
    Plot(int width, int height);
    ~Plot();

    void drawTrajectories(std::vector<Trajectory> trajectory,GLfloat axisWidth,GLfloat plotWidth,
    				float max_value,Color backgroundColor,std::vector<Color>);

    void drawPlotNNG(Trajectory* trajectory,GLfloat axisWidth,Color colorPlot, GLfloat plotWidth,Color backgroundColor, std::mutex* mtx);

    Points readCSV(std::string,float* max_value);


    GLFWwindow* window;
    Window_params window_params;



private:

    void drawPoint(Point vp,Color color, GLfloat size);
    void drawFloor(float max_value);
    void drawOrigin(float max_value,GLfloat axisThickness);
    void drawLine(Vertex start,Vertex end,float lineWidth);


    void nngPlot(Trajectory* trajectory,GLfloat axisWidth,Color colorPlot,GLfloat plotWidth,Color backgroundColor,std::mutex *mtx);

    void InitializeWindowSettings();


    static void scroll_callback(GLFWwindow* window, double x, double y);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double x, double y);


    bool Debug = true;
    nng_socket sock;
    int rv{};
    std::thread thrd_Plot;

/*

    GLfloat alpha{};
    GLfloat beta{};
    GLfloat zoom{};

    GLboolean locked = GL_FALSE;


    int cursorX = 0;
    int cursorY = 0;

    std::mutex mtx; */

};



#endif
