#include "Plot.h"

using namespace json11;



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void fatal(const char *func, int rv)
{
        fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Plot::drawPoint(Point p,Color color, GLfloat size){

    glPointSize(size);
    glBegin(GL_POINTS);
    glColor4f(color.r, color.g, color.b, color.a);
    glVertex3f(p.x,p.y, p.z);
    //printf("x: %f , y :%f , z:%f\n",v.x,v.y,v.z);
    glEnd();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Plot::drawLine(Point start,Point end,float lineWidth , Color color){

	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glColor4f(color.r, color.g,color.b, color.a);
	glVertex3f(start.x, start.y, start.z);
	glVertex3f(end.x, end.y, end.z);
	glEnd();

}



void Plot::drawLine(Vertex start,Vertex end,float lineWidth){

	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glColor4f(start.color.r, start.color.g,start.color.b, start.color.a);
	glVertex3f(start.point.x, start.point.y, start.point.z);
	glVertex3f(end.point.x, end.point.y, end.point.z);
	glEnd();

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//========================================================================
// Callback function for mouse button events
//========================================================================
void Plot::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{


	Plot *instance =(Plot*)glfwGetWindowUserPointer(window);

    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    if (action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        instance->locked = GL_TRUE;
    }
    else
    {
    	instance->locked = GL_FALSE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

//========================================================================
// Callback function for cursor motion events
//========================================================================
void Plot::cursor_position_callback(GLFWwindow* window, double x, double y)
{
    //if the mouse button is pressed

	Plot *instance =(Plot*)glfwGetWindowUserPointer(window);

    if (instance->locked)
    {
    	instance->alpha += (GLfloat) (x - instance->cursorX) / 10.0f;
    	instance->beta += (GLfloat) (y - instance->cursorY) / 10.0f;
    }
    //update the cursor position
    instance->cursorX = (int) x;
    instance->cursorY = (int) y;
}

//========================================================================
// Callback function for scroll events
//========================================================================
void Plot::scroll_callback(GLFWwindow* window, double x, double y)
{

	Plot *instance =(Plot*)glfwGetWindowUserPointer(window);

	instance->zoom += (float) y / 2.0f;
    if (instance->zoom < 0.0f)
    	instance->zoom = 0.0f;
}

//========================================================================
// Callback function for framebuffer resize events
//========================================================================
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    const double DEG2RAD = 3.14159265 / 180;
    const float fovY = 100.0f;
    const float front = 1.0f;
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


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//The rescale' functions are not used at moment

float rescale(float value,float range, float max, float min){
    float new_value;
    float new_max = range;
    float new_min = -range;

	new_value =  ((new_max-new_min)/(max-min)) * (value - max ) + new_max + range;

return new_value;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

std::vector<Vertex> rescaleTrajectory(	std::vector<Vertex> trajectory,
										float max_x,float min_x,float max_y,
										float min_y,float max_z,float min_z){
	float range = 2.0f;
	float max_rescale = std::max(std::max(max_x,max_y),max_z);
	float min_rescale = std::min(std::min(min_x,min_y),min_z);

	std::vector<Vertex> rescaled_trajectory = trajectory;
    for (unsigned i=0; i<trajectory.size(); i++){
    	rescaled_trajectory.at(i).point.x = rescale(trajectory.at(i).point.x,range,max_rescale,min_rescale);
    	rescaled_trajectory.at(i).point.y = rescale(trajectory.at(i).point.y,range,max_rescale,min_rescale);
    	rescaled_trajectory.at(i).point.z = rescale(trajectory.at(i).point.z,range,max_rescale,min_rescale);
    }
	return rescaled_trajectory;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Points Plot::readCSV(std::string PathFile,float *max_value){

	Points trajectory;
    Point p;
    p.x = 0;
    p.y = 0;
    p.z = 0;

    float max_x = 0;
    float max_y = 0;
    float max_z = 0;
    
    float min_x = 0;
    float min_y = 0;
    float min_z = 0;

    float start_x,start_y,start_z;

    std::ifstream file(PathFile);
    std::string line;
    bool first_measure = true;

    while (getline( file, line ))                   // read a whole line of the file
    {
    std::stringstream ss( line );                     // put it in a stringstream (internal stream)
      
     std::string data;
     int counter = 0;// 0 time, 1 x, 2 y, 3 z
      while ( getline( ss, data, ',' ) )           // read (string) items up to a comma
      {
    	  if(counter==1){
                if(first_measure)
                		start_x = std::stof(data);
                p.x =(GLfloat)std::stof(data) - start_x;
                if(p.x>max_x)
                    max_x = p.x;
                if(p.x<min_x)
                    min_x = p.x;
            }
            if(counter==2){
                if(first_measure)
                		start_y = std::stof(data);
               p.y = (GLfloat)std::stof(data)-start_y;
                if(p.y>max_y)
                    max_y = p.y;
                if(p.y<min_y)
                    min_y = p.y;
            }
            if(counter==3){
                if(first_measure)
                		start_z = std::stof(data);
                p.z = (GLfloat)std::stof(data)-start_z;
                if(p.z>max_z)
                    max_z = p.z;
                if(p.z<min_z)
                    min_z = p.z;
            }
        counter++;
      }
     printf("x: %f | y :%f | z:%f\n",p.x,p.y,p.z);
      trajectory.push_back(p);
      first_measure = false;
   }
    	if(*max_value < std::max(std::max(max_x,max_y),max_z))
    		*max_value = std::max(std::max(max_x,max_y),max_z);

    return trajectory;
	//return rescaleTrajectory(trajectory,max_x,min_x,max_y,min_y,max_z,min_z);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//The color goes from red to green as the time goes to the end
Color getColor(int index, int size){
	Color color{1.0f,0.0f,0.0f,1.0f};
	color.r = (1.0f - (float)index/size);
	color.g = (float)index/size;

	return color;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//The floor behaves on the x-z plane
void Plot::drawFloor(float max_value){
	float lineWidth = 1.0f;

	for(GLfloat x =-max_value ; x<=max_value ; x++){
			Vertex start{x,-max_value,0.0f,1.0f,1.0f,1.0f,1.0f};
			Vertex end{x,max_value,0.0f,1.0f,1.0f,1.0f,1.0f};
			drawLine(start,end,lineWidth);
	}

	for(GLfloat y =-max_value ; y<=max_value ; y++){
			Vertex start{-max_value,y,0.0f,1.0f,1.0f,1.0f,1.0f};
			Vertex end{max_value,y,0.0f,1.0f,1.0f,1.0f,1.0f};
			drawLine(start,end,lineWidth);
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Plot::drawOrigin(float max_value,GLfloat axisWidth){
	float transparency = 0.5f;
	int factor = 1.3;
	//float lineWidth = 8.0f;
	max_value = factor * max_value;

	glLineWidth(axisWidth);
	glBegin(GL_LINES);
	//draw a red line for the x-axis
	glColor4f(1.0f, 0.0f, 0.0f, transparency);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 0.0f, 0.0f, transparency);
	glVertex3f(max_value, 0.0f, 0.0f);

	//draw a green line for the y-axis
	glColor4f(0.0f, 1.0f, 0.0f, transparency);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor4f(0.0f, 1.0f, 0.0f, transparency);
	glVertex3f(0.0f, 0.0f, max_value);

	//draw a blue line for the z-axis
	glColor4f(0.0f, 0.0f, 1.0f, transparency);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor4f(0.0f, 0.0f, 1.0f, transparency);
	glVertex3f(0.0f, max_value, 0.0f);
	glEnd();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Plot::InitializeWindowSettings(){

			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

			//mouse button callback
			glfwSetMouseButtonCallback(window, &Plot::mouse_button_callback);

			//mouse movement callback
			glfwSetCursorPosCallback(window, &Plot::cursor_position_callback);

			//mouse scroll callback
			glfwSetScrollCallback(window,&Plot::scroll_callback);

			glfwMakeContextCurrent(window);
			glfwSwapInterval(1);

			//get the frame buffer (window) size
			glfwGetFramebufferSize(window, &window_params.width, &window_params.height);

			//initial call to the frame buffer callback, and initialize the OpenGL
			//camera and other properties there
			framebuffer_size_callback(window, window_params.width, window_params.height);

			//enabling a lot of stuff
			glEnable(GL_BLEND);

			//smooth the points
			glEnable(GL_LINE_SMOOTH);

			//smooth the lines
			glEnable(GL_POINT_SMOOTH);

			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

			//needed for alpha blending
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_ALPHA_TEST) ;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Color Plot::checkColor(std::vector<Color> colorPlot,unsigned int index ){
	Color current_color;

	//set default color
	if(index<colorPlot.size())
		current_color = colorPlot.at(index);
	else
		current_color = {0.0f,0.0f,0.0f,1.0f};

	return current_color;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Plot::drawPlot(SmartPtrTrajectories *trajectory, std::vector<Color> colorPlot,GLfloat plotWidth) {
	Color current_color;
	for(unsigned int j=0; j<trajectory->size();j++){
		Trajectory copied_trajectory;
		current_color =checkColor(colorPlot, j);
		copied_trajectory = *(trajectory->at(j));
		//printf("PLOT : Trajectory %d, size %d\n",j,copied_trajectory.points.size());
			for (unsigned i=0; i<copied_trajectory.points.size()-1; i++){
				copied_trajectory.color = current_color;
				copied_trajectory.width = plotWidth;
				if(Debug)
					printf("TRAJECTORY %d x: %f , y :%f , z:%f\n",j,copied_trajectory.points.at(i).x,copied_trajectory.points.at(i).y,copied_trajectory.points.at(i).z);
				if(this->mode == "line")
					drawLine(	copied_trajectory.points.at(i),
								copied_trajectory.points.at(i+1),
								copied_trajectory.width,
								copied_trajectory.color);

				if(this->mode == "point")
					drawPoint(	copied_trajectory.points.at(i),
								copied_trajectory.color,
								copied_trajectory.width);
			}
	}
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Plot::nngPlot(	SmartPtrTrajectories *trajectory,GLfloat axisWidth,
					std::vector<Color> colorPlot,GLfloat plotWidth,Color background){
	   //initialize the call backs for event handling

		GLfloat max_value = 35.0f;

		InitializeWindowSettings();
		printf("Started the Plot thread \n");

		while (!glfwWindowShouldClose(window))
	    {
	        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	        glClearColor(background.r, background.g, background.b, background.a);

	        //draw the scene

	        //switch to model view so the transformation applies to entire model
	        glMatrixMode(GL_MODELVIEW);
	        glLoadIdentity();
	        //move the object back and forth based on the zoom level
	        glTranslatef(0.0, 0.0, -zoom);
	        // rotate beta degrees around the x-axis
	        glRotatef(beta, 1.0, 0.0, 0.0);
	        // rotate alpha degrees around the z-axis
	        glRotatef(alpha, 0.0, 0.0, 1.0);

			drawFloor(max_value);
			drawOrigin(max_value,axisWidth);
			drawPlot(trajectory, colorPlot, plotWidth);

	        glfwSwapBuffers(window);
	        glfwPollEvents();
	    }


}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Plot::drawPlotNNG(	SmartPtrTrajectories *trajectory,GLfloat axisWidth,
						std::vector<Color> colorPlot,GLfloat plotWidth,Color backgroundColor,std::string mode){
	this->mode = mode;
	thrd_Plot = std::thread(&Plot::nngPlot,this,trajectory,axisWidth,colorPlot,plotWidth, backgroundColor);

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Plot::drawTrajectories(std::vector<Trajectory> trajectories,GLfloat axisWidth,GLfloat plotWidth,float max_value,
			Color background, std::vector<Color> colors){

	InitializeWindowSettings();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(background.r, background.g, background.b, background.a);
        //draw the scene
        //switch to model view so the transformation applies to entire model
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //move the object back and forth based on the zoom level
        glTranslatef(0.0, 0.0, -zoom);
        // rotate beta degrees around the x-axis
        glRotatef(beta, 1.0, 0.0, 0.0);
        // rotate alpha degrees around the z-axis
        glRotatef(alpha, 0.0, 0.0, 1.0);
		drawFloor(max_value);

		drawOrigin(max_value,axisWidth);

        //Plotting the dataset
		for (unsigned j = 0; j< trajectories.size(); j++){
			Trajectory trajectory = trajectories.at(j);
			Color trajColor = colors.at(j);
			for (unsigned i=0; i<trajectory.points.size(); i++){
				//trajectory.at(i).color = getColor(i,trajectory.size());
				printf("x: %f , y :%f , z:%f\n",trajectory.points.at(i).x,trajectory.points.at(i).y,trajectory.points.at(i).z);
				trajectory.color = trajColor;
				drawPoint(trajectory.points.at(i),trajectory.color,plotWidth);
			}
		}

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Plot::Plot (int width,int height){

	Window_params window_params{width,height};

	// Window size
	const int WINDOWS_WIDTH = window_params.width;
	const int WINDOWS_HEIGHT = window_params.height;

	//Initialize the window
    if (!glfwInit()){
    	printf("Error on the glfwInit()\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "Plot", NULL, NULL);
    if (!window){
        glfwTerminate();
    	printf("Cannot initialize the window\n");
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(window, this);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Plot::~Plot(){

	thrd_Plot.join();
	glfwDestroyWindow(window);
	glfwTerminate();




};



