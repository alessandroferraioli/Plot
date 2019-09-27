#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>
#include <json11.hpp>
#include "Plot.h"

#ifndef NNGINTERFACE_H_
#define NNGINTERFACE_H_




class NNG_Interface {
	public:

		NNG_Interface(const char * url,Trajectory **trajectory,std::mutex* mtx);
		~NNG_Interface();
		const char* url;


	private:

		void waitConnection();
		void GetMessage(Trajectory **trajectory);
		Point readMsg();
		void GetTrajectory(Trajectory **trajectory, std::mutex* mtx);

		nng_socket sock;
		int rv{};


};

#endif /* NNGINTERFACE_H_ */
