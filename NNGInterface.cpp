#include "NNGInterface.h"

using namespace json11;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Point NNG_Interface::readMsg(){

    char *buf;
    size_t sz;

    Point new_point;


    if ((rv = nng_recv(sock, &buf, &sz, NNG_FLAG_ALLOC)) != 0) {
            fatal("nng_recv", rv);
    }

    std::string err;
    Json json = Json::parse(buf, err);

    if (json.is_object())
    {
            Json::object itemMap = json.object_items();
            new_point.x = itemMap["x"].number_value();
            new_point.y = itemMap["y"].number_value();
            new_point.z = itemMap["z"].number_value();
    }

    nng_free(buf, sz);

    return new_point;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void NNG_Interface::GetTrajectory(Trajectory *trajectory, std::mutex* mtx){
	Point new_point;
	while((new_point = readMsg()).z != -100000){
	   std::lock_guard<std::mutex> lock(*mtx);
	   if(isFirstPoint){
		   startPoint = new_point;
	   }
	   new_point.x -= startPoint.x;
	   new_point.y -= startPoint.y;
	   new_point.z -= startPoint.z;
	   trajectory->points.push_back(new_point);
	   isFirstPoint = false;
	   if(Debug)
		   printf("Added the new point x=%f | y=%f | z=%f \n to the trajectory ",new_point.x,new_point.y,new_point.z);
	}

	if(Debug)
		printf("Finished the receiving of the data, z=-100000 occurred. \n");

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void NNG_Interface::waitConnection(){

		if ((rv = nng_sub0_open(&sock)) != 0) {
        fprintf(stderr, "nng_sub0_open: %s\n",  nng_strerror(rv));
		}

        // subscribe to everything (empty means all topics)
        while ((rv = nng_setopt(sock, NNG_OPT_SUB_SUBSCRIBE, "", 0)) != 0) {
                fatal("nng_setopt", rv);
        }

        while ((rv = nng_dial(sock, url, NULL, 0)) != 0) {
        		printf("Waiting for the connection\n");
                //fatal("nng_dial", rv);
        }
      printf("Connection is correctly set\n");
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
NNG_Interface::NNG_Interface(const char * url,Trajectory *trajectory,std::mutex* mtx) {
	this->url = url;
	waitConnection();
	thrd_read_msg = std::thread(&NNG_Interface::GetTrajectory,this,trajectory,mtx);

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
NNG_Interface::~NNG_Interface() {
	thrd_read_msg.join();

}

