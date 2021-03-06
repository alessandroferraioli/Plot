#include "NNGInterface.h"



using namespace json11;

int NNG_Interface::count_interfaces{};



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Point NNG_Interface::readMsg(){

    char *buf;
    size_t sz;
    char empty_msg[4096];

    Point new_point;

    if ((rv = nng_recv(sock, &buf, &sz, NNG_FLAG_ALLOC)) != 0) {
            fatal("nng_recv", rv);
    }

    std::string err;

    memcpy(empty_msg,buf,sz );

    empty_msg[sz] = '\0';
    Json json = Json::parse(empty_msg, err);

    if (json.is_object())
    {
            Json::object itemMap = json.object_items();
            new_point.x = itemMap["north_m"].number_value();
            new_point.y = itemMap["east_m"].number_value();
            new_point.z = (-1)*itemMap["down_m"].number_value();
    }


	   printf("Got  the new point x=%f | y=%f | z=%f \n",new_point.x,new_point.y,new_point.z);

    nng_free(buf, sz);

    return new_point;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void NNG_Interface::thrdFunctionGetTrajectory(SmartPtrTrajectories *trajectory, std::mutex* mtx){
	Point new_point;
	int counter{0};

	while((new_point = readMsg()).z != -100000){
	   if(isFirstPoint && ConnectionIsSet){
		   startPoint = new_point;
		 //  printf("Starting point of interface %d is x=%f | y=%f | z=%f \n",index,new_point.x,new_point.y,new_point.z);
		   isFirstPoint = false;
	   }
	   new_point.x -= startPoint.x;
	   new_point.y -= startPoint.y;
	   new_point.z -= startPoint.z;

	   //Try to get the lock on the shared resource
	   if(mtx->try_lock()){
		  std::lock_guard<std::mutex> guard(*mtx, std::adopt_lock);
		  if(Debug)
			  printf("Thread %d Got the lock \n",this->index);
		  trajectory->at(this->index)->points.push_back(new_point);

		  //If I ve smth backuped, Imma add them to the trajectory
		  if(backupNotEmpty){
			  for(unsigned int i = 0 ; i< backup_trajectory->points.size();i++){
				 trajectory->at(this->index)->points.push_back(backup_trajectory->points.at(i));
				  	 printf("Thread %d is emptying his backup trajectory",this->index);
			  }
			  backup_trajectory->points.clear();
			  backupNotEmpty = false;
		  }
	   }
	   else{
		   //Save the new point when I cannot take the lock
		   backup_trajectory->points.push_back(new_point);
		   //printf("Thread %d, size backup : %d \n",this->index, this->backup_trajectory->points.size());
		   backupNotEmpty = true;
	   }
	   if(Debug){
		   printf("Added the new point x=%f | y=%f | z=%f  on the trajectory %d\n",
				   new_point.x+ startPoint.x,new_point.y+ startPoint.y,new_point.z+ startPoint.z,this->index);
		   printf("-------------------------------------------------------------------------\n");
	   }

	   counter++;
	   //sleep(0.8);
	}

	printf("Got %d MESSAGGES THREAD %d\n",counter,this->index);
	if(Debug)
		printf("Finished the receiving of the data, z=-100000 occurred. \n");
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void NNG_Interface::GetTrajectory(SmartPtrTrajectories* trajectory, std::mutex* mtx){
	std::shared_ptr<Trajectory> memory_allocation = std::make_shared<Trajectory>();
	trajectory->push_back(memory_allocation);
	thrd_read_msg = std::thread(&NNG_Interface::thrdFunctionGetTrajectory,this,trajectory,mtx);

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
        	if(Debug)
        		printf("Waiting for the connection\n");
                //fatal("nng_dial", rv);
        }
      printf("Connection is correctly set\n");
      ConnectionIsSet = true;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
NNG_Interface::NNG_Interface(const char * url) {
	this->url = url;
	index = count_interfaces;
	printf("Started the interface %d\n",index);
	count_interfaces++;
	backup_trajectory = new Trajectory;
	thrd_wait_connection = std::thread(&NNG_Interface::waitConnection,this);


}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
NNG_Interface::~NNG_Interface() {
	thrd_read_msg.join();
	thrd_wait_connection.join();
	printf("NNG Interface Object %d is dead\n",this->index);
	count_interfaces--;
}

