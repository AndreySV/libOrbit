// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//

#ifndef _ORBIT_MODULE_H_
#define _ORBIT_MODULE_H_

#include "orbit_network.h"
#include "orbit_answers.h"

class OrbitModule
{
	string Id;
	unsigned char Address;
	OrbitNetwork& Network;


	template<class T> void RunCommand(OrbitAnswerPtr result) {
		T command( Address );
		command.AnswerData( result );
		Network.SendCommand( command );		
	}
public:
	
	OrbitModule(OrbitNetwork& network,const string& id = ""):
		Id(id),
		Address( 0 ),
	        Network(network)
	{};


	void SetAddress(unsigned char address);
	void ResetNetwork(void);
	void Identify(OrbitAnswerPtr identify);
	void Getinfo(OrbitAnswerPtr info);
	void Getstatus(OrbitAnswerPtr status);
	void Read16(OrbitAnswerPtr value);
	void Read32(OrbitAnswerPtr value);
	void ResetModule(OrbitAnswerPtr value);		

};

#endif


