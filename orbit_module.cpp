// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//

#include "orbit_module.h"
#include "orbit_commands.h"
#include <unistd.h>
using std::shared_ptr;




void OrbitModule::SetAddress(unsigned char address)
{
	Address = address;
	OrbitCommandSetaddr command( Id, Address );
        command.AnswerData( OrbitAnswerPtr(new OrbitAnswerSetaddr)  );
        Network.SendCommand( command );
}

void OrbitModule::ResetNetwork(void)
{
	OrbitCommandRst command;
	Network.SendCommand( command );
}

void OrbitModule::Identify(OrbitAnswerPtr identify)
{
	RunCommand<OrbitCommandIdentify>(identify);
}


void OrbitModule::Getinfo(OrbitAnswerPtr info)
{
	RunCommand<OrbitCommandGetinfo>(info);
}

void OrbitModule::Getstatus(OrbitAnswerPtr status)
{
	RunCommand<OrbitCommandGetstatus>(status);	
}


void OrbitModule::Read16(OrbitAnswerPtr value)
{
	RunCommand<OrbitCommandRead1>(value);	
}

void OrbitModule::Read32(OrbitAnswerPtr value)
{
	RunCommand<OrbitCommandRead2>(value);	
}

void OrbitModule::ResetModule(OrbitAnswerPtr value)
{
	RunCommand<OrbitCommandClr>(value);	
}


// void OrbitModule::ReadBuffer16(OrbitAnswerPtr value)
// {
// 	OrbitCommandSetaddr command( Id, Address );
//         AnswerData( OrbitAnswerPtr(new OrbitAnswerSetaddr)  );

// 	Network.GenerateBreak();
//         Network.Write( command.Request() );
// }


