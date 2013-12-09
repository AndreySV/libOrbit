// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//
#include <ctime>
#include <sstream>
#include "orbit_network.h"

using std::ostream;
using std::endl;


OrbitNetwork::OrbitNetwork(const string& port) {
	SetPortName( port );
	SetBaudRate( OrbitBaudrateDefault );
	Open();
};

TBaudRate OrbitNetwork::BreakBaudrate(void)
{
	const TBaudRate baudrate = GetBaudRate();		
	return (baudrate<OrbitBaudrateDefault) ? 2400 : 57600;
}

void OrbitNetwork::GenerateBreak(void)
{
	const string port = GetPortName();
	const TBaudRate baudrate = GetBaudRate();	
	Flush();

	Init( port,
	      BreakBaudrate(),
	      COM_DEFAULT_BYTE_SIZE,
	      COM_DEFAULT_PARITY,
	      COM_DEFAULT_STOP_BITS,
	      COM_DEFAULT_RTS_CONTROL);

	// length of 8 bits on the speed 57900 is a little bit bigger
	// than neccessary <BREAK> symbol for OrbitNetwork according
	// documentation (9ms at speed 187.5k)
	WriteByte(0);
	Flush();

	Init( port,
	      baudrate,
	      COM_DEFAULT_BYTE_SIZE,
	      ptODD,
	      COM_DEFAULT_STOP_BITS,
	      COM_DEFAULT_RTS_CONTROL);
}


inline double msecondsSince(clock_t startTime)
{
	return(( clock() - startTime )*1000.0)/(CLOCKS_PER_SEC);
}


void OrbitNetwork::SendCommand( OrbitCommandBase& cmd )
{
	unsigned char byte;
	vector<unsigned char> Request = cmd.RequestData();
	shared_ptr<OrbitAnswerBase> Answer = cmd.AnswerData();
	int AnswerLen = cmd.AnswerLen();

	Flush();
	
	// send request
	GenerateBreak();
	for(auto it=Request.begin();it!= Request.end(); ++it)
		WriteByte( *it );

	// read answer
	OrbitException exceptionData( Request, Answer );
	bool exception = false;
	
	const   double msTimeout = 300;
	clock_t startTime = clock();
	
	for( int i=0; i<AnswerLen; ) {
		if (ReadByte( byte )) {
			startTime = clock();
			
			if ((i==0) && (byte == '!'))
				exception = true;
			
			if ( Answer!=NULL )
				(*Answer)[i] = byte;
			i++;
		}

		if (msecondsSince( startTime )>msTimeout)
			throw OrbitTimeout(); 

	}

	if (exception)
		throw exceptionData;

	
	Flush();

}


	

const char* OrbitException::what(void) const throw()
{
	std::ostringstream out;

	out<<"Request: "<<endl;
	for(auto it=Request.begin();it!= Request.end(); ++it)	
		out<<std::hex<<*it<<" ";
	out<<endl;

	if (Answer!=NULL) {
		out<<"Answer: "<<endl;
		for(auto it=Answer->begin();it!= Answer->end(); ++it)		
			out<<std::hex<<*it<<" ";
		out<<endl;
	}

	out<<std::dec<<endl;	

	return out.str().c_str();
}
