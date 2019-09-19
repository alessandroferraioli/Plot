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
    GLfloat x, y, z;
    GLfloat r, g, b, a;
}Vertex;

typedef struct{
        int width;
        int height;
}Window_params;


class Plot{

public:
    Plot(Window_params window,std::string path);

    void drawPlot(std::vector<Vertex>,GLfloat);
    std::vector<Vertex> readCSV(std::string);

    Vertex vertex;
    Window_params window;



private:
    void drawPoint(Vertex ,GLfloat );
};


#endif
