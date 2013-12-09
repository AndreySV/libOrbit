// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//


#ifndef _ORBIT_NETWORK_H_
#define _ORBIT_NETWORK_H_

#include <exception>
#include "comport.h"
#include "orbit_commands.h"


const TBaudRate OrbitBaudrateDefault = 187500;
const TBaudRate OrbitBaudrateLow     = 9600;

class OrbitNetwork: public TComPort {
	
	TBaudRate BreakBaudrate(void);
public:
	OrbitNetwork(const string & port);
	void GenerateBreak(void);
	void SendCommand( OrbitCommandBase& cmd );


};





enum OrbitErrorCodes{
	// General errors
	ErrParity = 0x1,
	ErrCoilRange,
	ErrNoCommand,
	ErrBroadcastNotAllowed,
	ErrBroadcastExpected,
	ErrAddress,
	ErrMode,
	ErrCalibrationTable,
	ErrADC,
	ErrReading,
	ErrFraming,

	// Algorithm
	ErrSlope = 0x11,
	ErrUnderRange,
	ErrOverRange,
	ErrMultOverflow,

	// Buffered mode
	ErrNotEmpty = 0x1A,
	ErrEmpty,

	// Difference mode
	ErrFlagNotSet = 0x21,
	ErrWaitForTrig,
	ErrAquireSet,
	ErrDiffOverflow,
	ErrSumOverflow,
	ErrDiffRun,
	ErrHighRes,

	// Acquire mode
	ErrAcquireNotSet = 0x31,

	// Set mode
	ErrInvalidMode = 0x40,
	ErrDelay,
	ErrSynch,
	ErrRunning,
	ErrCaptureSample = 0x52,
	ErrAverage = 0x60,
	ErrAverageChange,
	ErrResolution,
	ErrResolutionChange,
	ErrHighResolution,
	ErrLowResolution,
	ErrNormalMode,
	ErrAddrRange,

	// Orbit 5V Power
	Err5VLowWarn = 0x90,
	Err5VLowErr,
	Err5VHighErr,
	ErrADS1252,
	Err5VHighWarn,

};



class OrbitException: public std::exception
{
	vector<unsigned char> Request;
	shared_ptr<OrbitAnswerBase> Answer;
public:
	OrbitException(
		vector<unsigned char>& _Request	 ):
		Request(_Request)
		{};
	
	OrbitException(
		vector<unsigned char>& _Request,
		shared_ptr<OrbitAnswerBase> _Answer ):
		Request(_Request),
		Answer(_Answer) {};

	virtual ~OrbitException() throw() {};
	
	vector<unsigned char>& GetRequest(void) { return Request; };
	shared_ptr<OrbitAnswerBase>& GetAnswer(void) { return Answer; };


	const char* what() const throw();
};



class OrbitTimeout: public std::exception
{
public:
	OrbitTimeout() {};
	virtual ~OrbitTimeout() throw() {};

	const char* what() const throw()
	{
		return string("Network timeout").c_str();
	}
};

#endif
