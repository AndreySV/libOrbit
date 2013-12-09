// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//

#ifndef _ORBIT_ANSWERS_H_
#define _ORBIT_ANSWERS_H_

#include <vector>
#include <exception>
#include <string>
#include <memory>
#include <cstdint>




class OrbitAnswerBase: public std::vector<unsigned char>
{
	template<class T> T  IntDifferentSize(int start)
	{
		T ret = 0;
		for(int i=0;i<sizeof(T);++i)
			ret +=(*this)[start+i]<<(8*i);
		return ret;
	};
	
public:
	OrbitAnswerBase(int i=0):
		std::vector<unsigned char>(i,0) {};

	std::string  String(int start, int len);
	int          Short(int start);
	int          Int(int start);
	long         Long(int start);
	void         PrintHex(std::ostream& out);		
	virtual void Print(std::ostream& out);
	virtual int  ErrCode(void);
};

typedef std::shared_ptr<OrbitAnswerBase>  OrbitAnswerPtr;

std::ostream& operator<<(std::ostream& out, OrbitAnswerBase& data);


	


class OrbitAnswerSetaddr: public OrbitAnswerBase
{
public:
	OrbitAnswerSetaddr():
		OrbitAnswerBase(2) {};

	unsigned char Command(void) { return (*this)[0]; }
	unsigned char PrevAddress(void) { return (*this)[1]; }
	
	virtual void Print(std::ostream& out);
};

class OrbitAnswerIdentify: public OrbitAnswerBase
{
public:
	OrbitAnswerIdentify():
		OrbitAnswerBase(0x1E) {};

	unsigned char Command(void) { return (*this)[0]; }
	std::string   Id(void);
	std::string   DeviceType(void);
	std::string   Version(void);
	int           Stroke(void);
	
	virtual void Print(std::ostream& out);
};


class OrbitAnswerGetinfo: public OrbitAnswerBase
{
	enum {
		startModuleType = 1,
		startHardwareType = startModuleType + 4,
		startResolution = startHardwareType + 2,
		startModInfo = startResolution + 2,
		startUOM = startModInfo + 1,
		startCPUOM = startUOM + 6,
		startOS = startCPUOM + 8,
		startNCBDP = startOS + 6,
		startRTS  = startNCBDP + 1,
		reserved  = startRTS + 1,
		startMult = reserved + 7
	};

public:
	OrbitAnswerGetinfo():
		OrbitAnswerBase(0x29) {};

	unsigned char Command(void) { return (*this)[0]; }
	std::string   ModuleType(void);
	int           HardwareType(void);
	int           Resolution(void);
	char          Compatibility(void);
	std::string   UOM(void);
	std::string   CPUOM(void);
	std::string   OS(void);
	char          NCBDP(void);
        char          RTS(void);
	std::string   Mult(void);
	

	
	virtual void Print(std::ostream& out);
};


class OrbitAnswerGetstatus: public OrbitAnswerBase
{
public:
	OrbitAnswerGetstatus():
		OrbitAnswerBase(4) {};

	unsigned char Command(void) { return (*this)[0]; }
	int       ErrorCode(void){ return (*this)[1]; }
	int       Status(void) { return Short(2); }	

	virtual void PrintStatus(std::ostream& out) {};
	virtual void Print(std::ostream& out);
};

class OrbitAnswerGetstatusDP: public OrbitAnswerGetstatus
{
	std::string Mode(unsigned int mode);
public:
	OrbitAnswerGetstatusDP() {};

	virtual void PrintStatus(std::ostream& out);
};


class OrbitAnswerRead1: public OrbitAnswerBase
{
public:
	OrbitAnswerRead1():
		OrbitAnswerBase(3) {};

	unsigned char Command(void) { return (*this)[0]; }
	int           Value(void)   { return Short(1);   }

	virtual void Print(std::ostream& out);	
};


class OrbitAnswerRead2: public OrbitAnswerBase
{
public:
	OrbitAnswerRead2():
		OrbitAnswerBase(5) {};

	unsigned char Command(void) { return (*this)[0]; }
	int           Value(void)   { return Int(1);   }

	virtual void Print(std::ostream& out);	
};



class OrbitAnswerReadbuffer1: public OrbitAnswerBase
{
	
public:
	OrbitAnswerReadbuffer1():
		OrbitAnswerBase(67) {};

	unsigned char          Command(void) { return (*this)[0]; };
	int                    Length(void)  { return (*this)[1]; };
	int                    Checksum(void) ;
	int                    CalcChecksum(void);
	std::vector<int16_t>   Values(void);
	
	virtual void Print(std::ostream& out);	
};



typedef OrbitAnswerSetaddr OrbitAnswerClr;

#endif
