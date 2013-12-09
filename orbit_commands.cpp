//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//

#include "orbit_commands.h"

void OrbitCommandBase::AnswerData(shared_ptr<OrbitAnswerBase> _Answer)
{
	Answer = _Answer;
	if (Answer != nullptr )
		Answer->resize(AnswerLength);
}


OrbitCommandRst::OrbitCommandRst(void):
	OrbitCommandBase( nullptr )
{
	Request.resize(2);
	Request[0] = OrbitCommands::OrbitRst;
	Request[1] = 0x0;
	AnswerLength = 0;
	
	if (Answer!=nullptr) 
		Answer->resize(AnswerLength);
}



OrbitCommandSetaddr::OrbitCommandSetaddr( const string& id, const unsigned char address)
{
	int i=0;
	Request.resize( 13 );
	Request[i++] = OrbitCommands::OrbitSetaddr;
	Request[i++] = address;
	
	for(; i<12; ++i)
		Request[i] = id[i-2];
	
	Request[i++] = 0;
	AnswerLength = 2;
}





OrbitCommandIdentify::OrbitCommandIdentify( const unsigned char address  )
{
	int i = 0;
	Request.resize( 2 );
	Request[i++] = OrbitCommands::OrbitIdentify;
	Request[i++] = address;
	
	AnswerLength = 0x1E;
}


OrbitCommandGetinfo::OrbitCommandGetinfo( const unsigned char address  )
{
	int i = 0;
	Request.resize( 2 );
	Request[i++] = OrbitCommands::OrbitGetInfo;
	Request[i++] = address;
	
	AnswerLength = 0x29;
}

OrbitCommandGetstatus::OrbitCommandGetstatus( const unsigned char address  )
{
	int i = 0;
	Request.resize( 2 );
	Request[i++] = OrbitCommands::OrbitGetstatus;
	Request[i++] = address;
	
	AnswerLength = 4;
}

OrbitCommandRead1::OrbitCommandRead1( const unsigned char address  )
{
	int i = 0;
	Request.resize( 2 );
	Request[i++] = OrbitCommands::OrbitRead1;
	Request[i++] = address;
	
	AnswerLength = 3;
}

OrbitCommandRead2::OrbitCommandRead2( const unsigned char address  )
{
	int i = 0;
	Request.resize( 2 );
	Request[i++] = OrbitCommands::OrbitRead2;
	Request[i++] = address;
	
	AnswerLength = 5;
}


OrbitCommandReadbuffer1::OrbitCommandReadbuffer1( const unsigned char address  )
{
	int i = 0;
	Request.resize( 2 );
	Request[i++] = OrbitCommands::OrbitReadbuffer1;
	Request[i++] = address;

	// answer length is variable (from 2  to 67 bytes )
	AnswerLength = 67; 
}


OrbitCommandClr::OrbitCommandClr( const unsigned char address  )
{
	int i = 0;
	Request.resize( 2 );
	Request[i++] = OrbitCommands::OrbitClr;
	Request[i++] = address;

	AnswerLength = 2;
}
