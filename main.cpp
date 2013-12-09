//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include "orbit_network.h"
#include "orbit_module.h"
#include "orbit_dp.h"
#include "debug.h"


using std::exception;
using std::cerr;
using std::cout;
using std::endl;


#ifdef __WIN32
void sleep(int seconds)
{
	Sleep( seconds*1000);
}
#endif





void  workThread(void)
{
#ifdef __WIN32
	const string port  = "COM6";
#else
	const string port  = "/dev/ttyUSB0";	
#endif

	// const string moduleID ="115C822P01";
	const string moduleID ="110B642P01";
	const int    pause_s = 1;

	OrbitNetwork  network( port );
	// OrbitModule   dp10( network, moduleID );
	OrbitDP   dp10( network, moduleID );	
	
	debug_set_level( DEBUG );

	if (network.Open() == false )
	{
		cerr<<"Can not open comport " << network.GetPortName()<<endl;
		throw(int(-1));
	}

	cout<<"Port "<<network.GetPortName()<<" is opened"<<endl;

	dp10.ResetNetwork();
	sleep(pause_s);
	
	dp10.SetAddress(5);
	cout<< "Set address to the orbit device"<<endl;

	//-----------------------------------------
	cout<< "Read device values:"<<endl;		
	while(1) {
		double value;
		
		try{
			sleep(pause_s);			
			value = dp10.ReadValue();
			cout<<"Current value: "<<value<<endl;			
		}
		catch(OrbitException& e)
		{
			cerr<<e.what()<<endl;
		}
		catch(OrbitTimeout& e)
		{
			cerr<<e.what()<<endl;
		}


	}

	
}




int main(void)
{
	try {
		workThread();
	}
	catch(exception& e) {
		cerr<<e.what()<<endl;
	}
	catch(int err) {
		switch(err) {
			case -1:
				cerr<<"Can open port"<<endl;
				break;
			default:
				cerr<<"Unknown exception:"<<endl;
				break;
		}
	}
	return 0;


	
}




