#include "Plot.h"
#include <thread>

#include <mutex>

using namespace json11;

//freeze the animation
bool freeze = false;

//rotation angles and zoom factor for 3D rendering
GLfloat alpha=210.0f, beta=-70.0f, zoom=10.0f;
GLboolean locked = GL_FALSE;

int cursorX = 0;
int cursorY = 0;



std::mutex mtx;
//Trajectory trajectory;




void
fatal(const char *func, int rv)
{
        fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Plot::drawPoint(Vertex v, GLfloat size){

    glPointSize(size);
    glBegin(GL_POINTS);
    glColor4f(v.color.r, v.color.g, v.color.b, v.color.a);
    glVertex3f(v.point.x, v.point.y, v.point.z);
    //printf("x: %f , y :%f , z:%f\n",v.x,v.y,v.z);
    glEnd();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Plot::drawLine(Vertex start,Vertex end,float lineWidth){

	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glColor4f(start.color.r, start.color.g,start.color.b, start.color.a);
	glVertex3f(start.point.x, start.point.y, start.point.z);
	glColor4f(end.color.r, end.color.g,end.color.b, end.color.a);
	glVertex3f(end.point.x, end.point.y, end.point.z);
	glEnd();

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Just used in the tests
/*
void drawPoints(  GLfloat size){

    float box_size = 20.0f;
    float step = 1.0f;

    for(float x=-box_size;x<box_size;x+=step){
        for(float y=-box_size;y<box_size;y+=step){
            for(float z=-box_size;z<box_size;z+=step){
            Vertex v1 = {x, y, z, 1.0f, 1.0f, 1.0f, 1.0f};
            glPointSize(size);
            glBegin(GL_POINTS);
            glColor4f(v1.color.r, v1.g, v1.b, v1.a);
            glVertex3f(v1.x, v1.y, v1.z);
            glEnd();
            }
        }
    }
}*/


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
    zoom += (float) y / 2.0f;
    if (zoom < 0.0f)
        zoom = 0.0f;
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
std::vector<Vertex> Plot::readCSV(std::string PathFile,float *max_value){

	std::vector<Vertex> trajectory;
	Color start_color{1.0f,1.0f,1.0f,1.0f};
    Vertex v;
    v.point.x = 0;
    v.point.y = 0;
    v.point.z = 0;
    v.color = start_color;
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
                v.point.x =(GLfloat)std::stof(data) - start_x;
                if(v.point.x>max_x)
                    max_x = v.point.x;
                if(v.point.x<min_x)
                    min_x = v.point.x;
            }
            if(counter==2){
                if(first_measure)
                		start_y = std::stof(data);
                v.point.y = (GLfloat)std::stof(data)-start_y;
                if(v.point.y>max_y)
                    max_y = v.point.y;
                if(v.point.y<min_y)
                    min_y = v.point.y;
            }
            if(counter==3){
                if(first_measure)
                		start_z = std::stof(data);
                v.point.z = (GLfloat)std::stof(data)-start_z;
                if(v.point.z>max_z)
                    max_z = v.point.z;
                if(v.point.z<min_z)
                    min_z = v.point.z;
            }
        counter++;
      }
     printf("x: %f | y :%f | z:%f\n",v.point.x,v.point.y,v.point.z);
      trajectory.push_back(v);
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

	for(GLfloat x =-max_value ; x<max_value ; x++){
			Vertex start{x,-max_value,0.0f,1.0f,1.0f,1.0f,1.0f};
			Vertex end{x,max_value,0.0f,1.0f,1.0f,1.0f,1.0f};
			drawLine(start,end,lineWidth);
	}

	for(GLfloat y =-max_value ; y<max_value ; y++){
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

void Plot::waitConnection(const char* url){


        // subscribe to everything (empty means all topics)
        if ((rv = nng_setopt(sock, NNG_OPT_SUB_SUBSCRIBE, "", 0)) != 0) {
                fatal("nng_setopt", rv);
        }

        while ((rv = nng_dial(sock, url, NULL, 0)) != 0) {
        		printf("Waiting for the connection\n");
                //fatal("nng_dial", rv);
        }
      printf("Connection is correctly set\n");
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Vertex Plot::readMsg(){

    char *buf;
    size_t sz;

    Vertex new_vertex;


    if ((rv = nng_recv(sock, &buf, &sz, NNG_FLAG_ALLOC)) != 0) {
            fatal("nng_recv", rv);
    }

    std::string err;
    Json json = Json::parse(buf, err);

    if (json.is_object())
    {
            Json::object itemMap = json.object_items();
            new_vertex.point.x = itemMap["x"].number_value();
            new_vertex.point.y = itemMap["y"].number_value();
            new_vertex.point.z = itemMap["z"].number_value();
    }

    Color new_vertexColor{0.0f,1.0f,0.0f,1.0f};
    new_vertex.color = new_vertexColor;
	//printf("Got the message x: %f, y: %f, z: %f\n",new_vertex.point.x,new_vertex.point.y,new_vertex.point.z);

    nng_free(buf, sz);

    return new_vertex;

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Plot::getMessage(Trajectory* trajectory){
	printf("Started thread GetMessage \n");
	for(;;){
		Vertex new_vertex = readMsg();
		mtx.lock();
		trajectory->push_back(new_vertex);
		//printf("Added new vertex x: %f, y: %f, z: %f\n",new_vertex.point.x,new_vertex.point.y,new_vertex.point.z);
		mtx.unlock();
		//usleep(100);
	}

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Plot::nngPlot(Trajectory* trajectory,GLfloat axisWidth,GLfloat plotWidth,Color background){
	   //initialize the call backs for event handling

		printf("Started thread nngPlot \n");

		//Trajectory copy_trajectory;


	    //frame buffer size callback - i.e., window resizing
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
			drawFloor(30.0f);

			drawOrigin(30.0f,axisWidth);


		//	if(mtx.try_lock()==-1){
			//	printf("Got the lock on the trajectory, Updating the trajectory plotted\n");
			//	copy_trajectory = trajectory;
				//mtx.unlock();

			//}

	        //Plotting the dataset
			Trajectory copied_trajectory = *trajectory;
			printf("Trajectory's size : %d\n",copied_trajectory.size());
				for (unsigned i=0; i<copied_trajectory.size(); i++){
					//trajectory.at(i).color = getColor(i,trajectory.size());
				//	printf("x: %f , y :%f , z:%f\n",trajectory.at(i).point.x,trajectory.at(i).point.y,trajectory.at(i).point.z);
					copied_trajectory.at(i).color.r = 0.0f;
					copied_trajectory.at(i).color.g = 1.0f;
					copied_trajectory.at(i).color.b = 0.0f;
					copied_trajectory.at(i).color.a = 1.0f;

					drawPoint(copied_trajectory.at(i),plotWidth);
				}


	        glfwSwapBuffers(window);
	        glfwPollEvents();
	    }
	    glfwDestroyWindow(window);
	    glfwTerminate();

}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Plot::drawPlotNNG(const char* url,GLfloat axisWidth,GLfloat plotWidth,Color backgroundColor){
	Trajectory trajectory;
	waitConnection(url);
	std::thread thrd_getMsg(&Plot::getMessage,this,&trajectory);
	std::thread thrd_Plot (&Plot::nngPlot,this,&trajectory,axisWidth,plotWidth, backgroundColor);
	thrd_Plot.join();
	thrd_getMsg.join();

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Plot::drawPlot(std::vector<Trajectory> trajectories,GLfloat axisWidth,GLfloat plotWidth,float max_value,
			Color background, std::vector<Color> colors){


    //initialize the call backs for event handling



    //frame buffer size callback - i.e., window resizing
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
			for (unsigned i=0; i<trajectory.size(); i++){
				//trajectory.at(i).color = getColor(i,trajectory.size());
				printf("x: %f , y :%f , z:%f\n",trajectory.at(i).point.x,trajectory.at(i).point.y,trajectory.at(i).point.z);
				trajectory.at(i).color.r = trajColor.r;
				trajectory.at(i).color.g = trajColor.g;
				trajectory.at(i).color.b = trajColor.b;

				drawPoint(trajectory.at(i),plotWidth);
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

    if ((rv = nng_sub0_open(&sock)) != 0) {
        fprintf(stderr, "nng_sub0_open: %s\n",  nng_strerror(rv));
    }

}
