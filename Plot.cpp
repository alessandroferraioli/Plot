#include "Plot.h"


#include <GLFW/glfw3.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

//freeze the animation
bool freeze = false;

//rotation angles and zoom factor for 3D rendering
GLfloat alpha=210.0f, beta=-70.0f, zoom=2.0f;
GLboolean locked = GL_FALSE;

int cursorX = 0;
int cursorY = 0;


//Support' functions


void Plot::drawPoint(Vertex v, GLfloat size){

    glPointSize(size);
    glBegin(GL_POINTS);
    glColor4f(v.r, v.g, v.b, v.a);
    glVertex3f(v.x, v.y, v.z);
    //printf("x: %f , y :%f , z:%f\n",v.x,v.y,v.z);
    glEnd();

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void drawPoints(  GLfloat size){

    float box_size = 20.0f;
    float step = 1.0f;

    for(float x=-box_size;x<box_size;x+=step){
        for(float y=-box_size;y<box_size;y+=step){
            for(float z=-box_size;z<box_size;z+=step){
            Vertex v1 = {x, y, z, 1.0f, 1.0f, 1.0f, 1.0f};

            glPointSize(size);
            glBegin(GL_POINTS);
            glColor4f(v1.r, v1.g, v1.b, v1.a);
            glVertex3f(v1.x, v1.y, v1.z);
            glEnd();


            }
        }
    }
}



//========================================================================
// Callback function for mouse button events
//========================================================================
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    if (action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        locked = GL_TRUE;
    }
    else
    {
        locked = GL_FALSE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

//========================================================================
// Callback function for cursor motion events
//========================================================================
void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    //if the mouse button is pressed
    if (locked)
    {
        alpha += (GLfloat) (x - cursorX) / 10.0f;
        beta += (GLfloat) (y - cursorY) / 10.0f;
    }
    //update the cursor position
    cursorX = (int) x;
    cursorY = (int) y;
}

//========================================================================
// Callback function for scroll events
//========================================================================
void scroll_callback(GLFWwindow* window, double x, double y)
{
    zoom += (float) y / 4.0f;
    if (zoom < 0.0f)
        zoom = 0.0f;
}

//========================================================================
// Callback function for framebuffer resize events
//========================================================================
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    const double DEG2RAD = 3.14159265 / 180;
    const float fovY = 45.0f;
    const float front = 0.1f;
    const float back = 128.0f;
    float ratio = 1.0f;
    if (height > 0)
        ratio = (float) width / (float) height;

    // Setup viewport
    glViewport(0, 0, width, height);

    // Change to the projection matrix and set our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double tangent = tan(fovY/2 * DEG2RAD);   // tangent of half fovY
    double height_f = front * tangent;          // half height of near plane
    double width_f = height_f * ratio;      // half width of near plane

    //Create the projection matrix based on the front
    //clipping plane and the location of the corners
    glFrustum(-width_f, width_f, -height_f, height_f, front, back);

    //Alternative: gluPerspective will provide the same solution as above
    //gluPerspective(fovY, ratio, front, back);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
void drawOrigin(int width, int height){
    float transparency = 1.0f;
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    //draw a red line for the x-axis
    glColor4f(1.0f, 0.0f, 0.0f, transparency);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor4f(1.0f, 0.0f, 0.0f, transparency);
    glVertex3f(width, 0.0f, 0.0f);

    //draw a green line for the y-axis
    glColor4f(0.0f, 1.0f, 0.0f, transparency);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor4f(0.0f, 1.0f, 0.0f, transparency);
    glVertex3f(0.0f, 0.0f, height);

    //draw a blue line for the z-axis
    glColor4f(0.0f, 0.0f, 1.0f, transparency);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor4f(0.0f, 0.0f, 1.0f, transparency);
    glVertex3f(0.0f, 10, 0.0f);
    glEnd();
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

float rescale(float value,float range, float max, float min){
    float new_value;
    float new_max = range;
    float new_min = -range;

	new_value =  ((new_max-new_min)/(max-min)) * (value - max ) + new_max + range;

return new_value;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

std::vector<Vertex> rescaleTrajectory(	std::vector<Vertex> trajectory,
										float max_x,float min_x,float max_y,
										float min_y,float max_z,float min_z){
	float range = 1.0f;
	std::vector<Vertex> rescaled_trajectory = trajectory;
    for (unsigned i=0; i<trajectory.size(); i++){
    	rescaled_trajectory.at(i).x = rescale(trajectory.at(i).x,range,max_x,min_x);
    	rescaled_trajectory.at(i).y = rescale(trajectory.at(i).y,range,max_y,min_z);
    	rescaled_trajectory.at(i).z = rescale(trajectory.at(i).z,range,max_z,min_y);
    }
	return rescaled_trajectory;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


std::vector<Vertex> Plot::readCSV(std::string PathFile){

	std::vector<Vertex> trajectory;
    Vertex v {0,0,0,1.0f,1.0f,1.0f,1.0f};
    float max_x = 0;
    float max_y = 0;
    float max_z = 0;
    
    float min_x = 0;
    float min_y = 0;
    float min_z = 0;


    std::ifstream file(PathFile);
    std::string line;
    int index = 0;

    while (getline( file, line ))                   // read a whole line of the file
    {
    std::stringstream ss( line );                     // put it in a stringstream (internal stream)
      
     std::string data;
     int counter = 0;// 0 time, 1 x, 2 y, 3 z
      while ( getline( ss, data, ',' ) )           // read (string) items up to a comma
      {

            if(counter==1){
                v.x =(GLfloat)std::stof(data);
                if(v.x>max_x)
                    max_x = v.x;
                if(v.x<min_x)
                    min_x = v.x;
            }
            if(counter==2){
                v.y = (GLfloat)std::stof(data);
                if(v.y>max_y)
                    max_y = v.y;
                if(v.y<min_y)
                    min_y = v.y;
            }
            if(counter==3){
                v.z = (GLfloat)std::stof(data);
                if(v.z>max_z)
                    max_z = v.z;
                if(v.z<min_z)
                    min_z = v.z;
            }
        counter++;
     // }
      }
      //printf("x: %f | y :%f | z:%f\n",v.x,v.y,v.z);
      trajectory.push_back(v);
      index ++;

   }

   return trajectory;
	//return rescaleTrajectory(trajectory,max_x,min_x,max_y,min_y,max_z,min_z);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Plot::drawPlot(std::vector<Vertex> trajectory,GLfloat size){
	Vertex v;
    for (unsigned i=0; i<trajectory.size(); i++){
    	printf("x: %f , y :%f , z:%f\n",trajectory.at(i).x,trajectory.at(i).y,trajectory.at(i).z);
    	v = trajectory.at(i);
        glPointSize(size);
        glBegin(GL_POINTS);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glVertex3f(v.x, v.y, v.z);
        //printf("x: %f , y :%f , z:%f\n",v.x,v.y,v.z);
        glEnd();


    	//drawPoint(trajectory.at(i),size);
    }

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Plot::Plot (Window_params params,std::string path){

	GLFWwindow* window;
	// Window size
	const int WINDOWS_WIDTH = params.width;
	const int WINDOWS_HEIGHT = params.height;

	std::vector<Vertex> trajectory = readCSV(path);
	GLfloat size = 5.0f;




    if (!glfwInit()){
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT,"Plot", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    //framebuffer size callback - i.e., window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //mouse button callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    //mouse movement callback
    glfwSetCursorPosCallback(window, cursor_position_callback);
    //mouse scroll callback
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //get the frame buffer (window) size
    //int width, height;
    glfwGetFramebufferSize(window, &params.width, &params.height);


    //initial call to the framebuffer callback, and initialize the OpenGL
    //camera and other properties there
    framebuffer_size_callback(window, params.width, params.height);


    //enable anti-aliasing
    glEnable(GL_BLEND);
    //smooth the points
    glEnable(GL_LINE_SMOOTH);
    //smooth the lines
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    //needed for alpha blendin
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST) ;


    //initialize all parameters
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //draw the scene


        //switch to modelview so the transformation applies to entire model
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //move the object back and forth based on the zoom level
        glTranslatef(0.0, 0.0, -zoom);
        // rotate beta degrees around the x-axis
        glRotatef(beta, 1.0, 0.0, 0.0);
        // rotate alpha degrees around the z-axis
        glRotatef(alpha, 0.0, 0.0, 1.0);

        //draw the origin with the x,y,z axes for visualization

        drawPlot(trajectory,size);
        //drawPoints(size);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);

    glfwTerminate();




}
