#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>
#include "Plot.h"
#include <time.h>

#include <cstdlib>
#include <iostream>
#include<cstring>

#ifndef NNGINTERFACE_H_
#define NNGINTERFACE_H_


class NNG_Interface {


	public:

		NNG_Interface(const char * url);
		~NNG_Interface();
		void GetTrajectory(SmartPtrTrajectories *trajectories, std::mutex* mtx);
		const char* url;
		unsigned int index;



	private:

		void waitConnection();
		//void GetMessage(std::shared_ptr<Trajectory> trajectory);
		Point readMsg();
		void thrdFunctionGetTrajectory(SmartPtrTrajectories *trajectories, std::mutex* mtx);

		Trajectory *backup_trajectory;
		bool backupNotEmpty{false};

		nng_socket sock;
		int rv{};



		Point startPoint{};
		bool isFirstPoint = true;
		bool ConnectionIsSet = false;

		static int count_interfaces;


		bool Debug = false;

		std::thread thrd_read_msg;
		std::thread thrd_wait_connection;



};

#endif /* NNGINTERFACE_H_ */
