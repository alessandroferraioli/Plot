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

#define _USE_MATH_DEFINES // M_PI constant

typedef struct{
    GLfloat r, g, b, a;
}Color;


typedef struct{
    GLfloat x, y, z;
    Color color;
}Vertex;

typedef struct{
        int width;
        int height;
}Window_params;


class Plot{

public:
    Plot(int width, int height);

    void drawPlot(std::vector<Vertex> trajectory,GLfloat point_size,float max_value);
    std::vector<Vertex> readCSV(std::string,float* max_value);

    GLFWwindow* window;
    Window_params window_params;



private:

    void drawPoint(Vertex ,GLfloat );

};


#endif
