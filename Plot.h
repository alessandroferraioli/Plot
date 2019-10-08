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
#include <memory>



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
	Color color{1.0f,1.0f,1.0f,1.0f};
}Vertex;

//typedef std::vector<Vertex> Trajectory;

typedef std::vector<Point> Points;

typedef struct{
	Points points;
	Color color;
	GLfloat width;
}Trajectory;



typedef std::vector<std::shared_ptr<Trajectory>> SmartPtrTrajectories;



typedef struct{
        int width;
        int height;
}Window_params;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


class Plot{

public:

    Plot(int width, int height);
    ~Plot();

    void drawPlotNNG(	SmartPtrTrajectories *trajectory,
    					GLfloat axisWidth,
						std::vector<Color>  colorPlot,
						GLfloat plotWidth,Color backgroundColor,
						std::string mode);



    void drawTrajectories(std::vector<Trajectory> trajectory,GLfloat axisWidth,GLfloat plotWidth,
    				float max_value,Color backgroundColor,std::vector<Color>);
    Points readCSV(std::string,float* max_value);


    GLFWwindow* window;
    Window_params window_params;
    std::string mode{};


private:

    void drawPoint(Point vp,Color color, GLfloat size);
    void drawFloor(float max_value);
    void drawOrigin(float max_value,GLfloat axisThickness);
    void drawPlot(SmartPtrTrajectories *trajectory, std::vector<Color> colorPlot,GLfloat plotWidth );



    void drawLine(Vertex start,Vertex end,float lineWidth);
    void drawLine(Point start,Point end,float lineWidth , Color color);
    Color checkColor(std::vector<Color> colorPlot,unsigned int index );


    void nngPlot(SmartPtrTrajectories *trajectory,GLfloat axisWidth,std::vector<Color> colorPlot,GLfloat plotWidth,Color backgroundColor);

    void InitializeWindowSettings();


    static void scroll_callback(GLFWwindow* window, double x, double y);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double x, double y);


    bool Debug = false;
    nng_socket sock;
    int rv{};


    GLfloat alpha{210.0f};
    GLfloat beta{-70.0f};
    GLfloat zoom{10.0f};
    GLboolean locked = GL_FALSE;
    int cursorX = 0;
    int cursorY = 0;
    std::thread thrd_Plot;

};



#endif




