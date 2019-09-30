

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

	//printf("Got the message x: %f, y: %f, z: %f\n",new_vertex.point.x,new_vertex.point.y,new_vertex.point.z);

    nng_free(buf, sz);

    return new_point;

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void NNG_Interface::GetTrajectory(Trajectory **trajectory, std::mutex* mtx){

	while(1){
		Point new_point = readMsg();
	    std::lock_guard<std::mutex> lock(*mtx);
	   (*trajectory)->points.push_back(new_point);
	    printf("Got the new point x=%f | y=%f | z=%f \n ",new_point.x,new_point.y,new_point.z);
	}

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
NNG_Interface::NNG_Interface(const char * url,Trajectory **trajectory,std::mutex* mtx) {
	this->url = url;
	waitConnection();
	//std::thread thrd_read_msg(&NNG_Interface::GetTrajectory,this,trajectory,mtx);
	GetTrajectory(trajectory,mtx);
	//thrd_read_msg.join();

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
NNG_Interface::~NNG_Interface() {
}

