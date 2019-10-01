#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>
#include <json11.hpp>
#include "Plot.h"

#ifndef NNGINTERFACE_H_
#define NNGINTERFACE_H_


class NNG_Interface {
	public:

		NNG_Interface(const char * url);
		~NNG_Interface();
		void GetTrajectory(SmartPtr<Trajectory> *trajectory, std::mutex* mtx);
		const char* url;



	private:

		void waitConnection();
		void GetMessage(SmartPtr<Trajectory> trajectory);
		Point readMsg();
		void thrdFunctionGetTrajectory(SmartPtr<Trajectory> *trajectory, std::mutex* mtx);


		nng_socket sock;
		int rv{};



		Point startPoint{};
		bool isFirstPoint = true;
		bool ConnectionIsSet = false;


		bool Debug = false;

		std::thread thrd_read_msg;
		std::thread thrd_wait_connection;



};

#endif /* NNGINTERFACE_H_ */
