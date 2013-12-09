// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//


#ifndef _ORBIT_COMMANDS_H_
#define _ORBIT_COMMANDS_H_

#include <vector>
#include <memory>
#include "orbit_answers.h"

using std::shared_ptr;
using std::vector;
using std::string;


enum OrbitCommands {
	OrbitRst           = 0x52, // 'R'
	OrbitSetaddr       = 0x53, // 'S'
	OrbitNotify        = 0x4E, // 'N'
	OrbitIdentify      = 0x49, // 'I'
	OrbitGetInfo       = 0x42, // 'B'
	OrbitGetstatus     = 0x47, // 'G'
	OrbitSetmode       = 0x56, // 'V'
	OrbitControl       = 0x57, // 'W'
	OrbitRead1         = 0x31, // '1'
	OrbitReadbuffer1   = 0x32, // '2'
	OrbitRead2         = 0x4C, // 'L'
	OrbitClr           = 0x43, // 'C'
	OrbitDifference    = 0x46, // 'F'
	OrbitStartdiff     = 0x4F, // 'O'
	OrbitStopdiff      = 0x48, // 'H'
	OrbitReaddiff1     = 0x44, // 'D'
	OrbitReaddiff2     = 0x58, // 'X'
	OrbitPreset        = 0x50, // 'P'
	OrbitRefmark       = 0x4B, // 'K'
	OrbitDirection     = 0x55, // 'U'
	OrbitConclr        = 0x72, // 'r'
	OrbitAcquire       = 0x41, // 'A'
	OrbitTrigger       = 0x54, // 'T'
	OrbitReadia        = 0x45, // 'E'
};





class OrbitCommandBase
{
protected:
	std::vector<unsigned char> Request;
	unsigned int AnswerLength;
	shared_ptr<OrbitAnswerBase> Answer;
public:
	OrbitCommandBase(shared_ptr<OrbitAnswerBase> _Answer = nullptr):
		Request(0),
		AnswerLength(0),
		Answer( _Answer )
	{};

	// accessors 
	void AnswerLen(unsigned int Len) { AnswerLength = Len; };
	unsigned int AnswerLen(void)  { return AnswerLength; };
	
	vector<unsigned char>& RequestData(void)  { return Request; };
	
	void AnswerData(shared_ptr<OrbitAnswerBase> _Answer);
	shared_ptr<OrbitAnswerBase> AnswerData(void)  { return Answer; };
};


class OrbitCommandRst: public OrbitCommandBase
{
public: 
	OrbitCommandRst(void);
};


class OrbitCommandSetaddr: public OrbitCommandBase
{
public: 
	OrbitCommandSetaddr( const string& id, const unsigned char address);
};


class OrbitCommandIdentify: public OrbitCommandBase
{
public: 
	OrbitCommandIdentify(const unsigned char address);
};


class OrbitCommandGetinfo: public OrbitCommandBase
{
public: 
	OrbitCommandGetinfo(const unsigned char address);
};


class OrbitCommandGetstatus: public OrbitCommandBase
{
public: 
	OrbitCommandGetstatus(const unsigned char address);
};



class OrbitCommandRead1: public OrbitCommandBase
{
public: 
	OrbitCommandRead1(const unsigned char address);
};

class OrbitCommandRead2: public OrbitCommandBase
{
public: 
	OrbitCommandRead2(const unsigned char address);
};


class OrbitCommandReadbuffer1: public OrbitCommandBase
{
public: 
	OrbitCommandReadbuffer1(const unsigned char address);
};


class OrbitCommandClr: public OrbitCommandBase
{
public: 
	OrbitCommandClr(const unsigned char address);
};




#endif
