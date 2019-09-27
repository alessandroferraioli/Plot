

#include "NNGInterface.h"

using namespace json11;

void fatal(const char *func, int rv)
{
        fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
}

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

	//printf("Got the message x: %f, y: %f, z: %f\n",new_vertex.point.x,new_vertex.point.y,new_vertex.point.z);

    nng_free(buf, sz);

    return new_point;

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void NNG_Interface::GetMessage(Trajectory** trajectory){
	Point newPoint;
	while((newPoint = readMsg()).z != -100){




	}



}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void NNG_Interface::GetTrajectory(Trajectory** trajectory){

	std::thread thrd_getMsg(&NNG_Interface::GetMessage,this,trajectory);

}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void NNG_Interface::waitConnection(){


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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

NNG_Interface::NNG_Interface(const char * url,SharedPoint* buffer) {
	this->url = url;
	this->buffer = buffer;
	waitConnection();



}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
NNG_Interface::~NNG_Interface() {
}

