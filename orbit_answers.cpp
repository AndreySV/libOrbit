// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//

#include <iostream>
#include "orbit_answers.h"

using std::string;
using std::ostream;
using std::endl;


//-----------------------------------------------------------------------------

void OrbitAnswerBase::Print(ostream& out)
{
	for(auto byte: *this)
		out<<byte<<" ";
	out<<endl;
}


void OrbitAnswerBase::PrintHex(ostream& out)
{
	out<<std::hex;
	for(int byte: *this)
		out<<std::hex<<byte<<" ";
	out<<std::dec<<endl;
}



string OrbitAnswerBase::String(int start, int len)
{
	string ret("");
	for(int i=0; i<len; ++i)
		ret.push_back( (*this)[ start + i ] );
	return ret;
}

int    OrbitAnswerBase::Short(int start)
{
	return IntDifferentSize<int16_t>(start);
}

int    OrbitAnswerBase::Int(int start)
{
	return IntDifferentSize<int32_t>(start);
}

long   OrbitAnswerBase::Long(int start)
{
	return IntDifferentSize<int64_t>(start);
}


int    OrbitAnswerBase::ErrCode(void)
{
	return ( (*this)[0]=='!' ? (*this)[1] : 0 );
}


ostream& operator<<(ostream& out, OrbitAnswerBase& data)
{
	data.Print(out);
	return out;
}


//-----------------------------------------------------------------------------

void OrbitAnswerSetaddr::Print(ostream& out)
{
	out<<"Command:       "<<Command()<<endl;
	out<<"Prev. address: "<<PrevAddress()<<endl;	
}


//-----------------------------------------------------------------------------

string  OrbitAnswerIdentify::Id(void) 
{
	return String(1, 10);
}

string   OrbitAnswerIdentify::DeviceType(void)
{
	return String( 11, 12 );
}

string   OrbitAnswerIdentify::Version(void)
{
	return String( 23, 5 );
}

int           OrbitAnswerIdentify::Stroke(void)
{
	return Short(28);
}


void OrbitAnswerIdentify::Print(ostream& out)
{
	out<<"Command:     "<<Command()<<endl;
	out<<"Id:          "<<Id()<<endl;
	out<<"Device type: "<<DeviceType()<<endl;
	out<<"Version:     "<<Version()<<endl;
	out<<"Stroke:      "<<Stroke()<<endl;	
}

//-----------------------------------------------------------------------------


std::string   OrbitAnswerGetinfo::ModuleType(void)
{
	return String( startModuleType, 4);
}


int           OrbitAnswerGetinfo::HardwareType(void)
{
	return Short( startHardwareType );
}


int           OrbitAnswerGetinfo::Resolution(void)
{
	return Short( startResolution );
}

char OrbitAnswerGetinfo::Compatibility(void)
{
	return (*this)[ startModInfo ];
}


std::string   OrbitAnswerGetinfo::UOM(void)
{
	return String( startUOM, 6 );
}

std::string   OrbitAnswerGetinfo::CPUOM(void)
{
	return String( startCPUOM, 8);
}

std::string   OrbitAnswerGetinfo::OS(void)
{
	return String( startOS, 6);
	
}

char OrbitAnswerGetinfo::NCBDP(void)
{
	return (*this)[startNCBDP];
}

char OrbitAnswerGetinfo::RTS(void)
{
	return (*this)[startRTS];
	
}

	
std::string   OrbitAnswerGetinfo::Mult(void)
{
	return String(startMult, 2);
}



void OrbitAnswerGetinfo::Print(ostream& out)
{
	OrbitAnswerBase::Print(out);
	OrbitAnswerBase::PrintHex(out);
	
	out<<endl;
	out<<endl;
	out<<"Command:        "<<Command()<<endl;
	out<<"Module type:    "<<ModuleType()<<endl;
	out<<"Hardware type:  "<<HardwareType()<<endl;
	out<<"Resolution:     "<<Resolution()<<endl;
	out<<endl;
	
	out<<"Module info                               "<<endl;
	out<<"Compatibility:                            "<<Compatibility()<<endl;
	out<<"Unit of measure (UOM):                    "<<UOM()<<endl;
	out<<"Counts per unit of measure (CPUOM):       "<<CPUOM()<<endl;
	out<<"Offset (OS):                              "<<OS()<<endl;
	out<<"No of chars before decimal point (NCBDP): "<<NCBDP()<<endl;
	out<<"Read types supported (RTS):               "<<RTS()<<endl;
	out<<"Multiplier (MULT):                        "<<Mult()<<endl;

}

//-----------------------------------------------------------------------------

void OrbitAnswerGetstatus::Print(ostream& out)
{
	out<<"Command:        "<<Command()<<endl;
	out<<"Error code:     "<<ErrorCode()<<endl;
	out<<endl;
	out<<"Status:         "<<Status()<<endl;	
	PrintStatus(out);
	
	out<<endl;
}



string OrbitAnswerGetstatusDP::Mode(unsigned int mode)
{
	vector<string> modes = { "Normal mode",
			   "OrbitDifference mode",
			   "OrbitAcquire mode",
			   "Sync mode",
			   "Sample mode (Buffered)",
			   "Capture mode (Buffered)"
	};
	
	if (mode < modes.size() )
		return modes[mode];
	return "Reserved";

}


void OrbitAnswerGetstatusDP::PrintStatus(ostream& out)
{
	union {
		struct 
		{
			unsigned int  ReadingsTaken:8;   // number of readings
			unsigned int  Mode:3;            // working mode
			unsigned int  NewReading:1;      // new reading available
			unsigned int  NU:2;              // not used
			unsigned int  Stopped:1;         // module stopped
			unsigned int  Triggered:1;       // module triggered
		} DP;
		int status;
	} statusDP;
	
	statusDP.status = Status();

	out<<"DP status"<<endl;
	out<<"Reading taken:            "<<statusDP.DP.ReadingsTaken<<endl;
	out<<"Working mode:             "<<statusDP.DP.Mode<<"  ("<< Mode( statusDP.DP.Mode )<<" )"<<endl;
	out<<"New reading is available: "<< statusDP.DP.NewReading<<endl;
	out<<"Module stopped:           "<<statusDP.DP.Stopped<<endl;
	out<<"Module triggered:         "<<statusDP.DP.Triggered<<endl;
}


//-----------------------------------------------------------------------------


void OrbitAnswerRead1::Print(ostream& out)
{
	out<<"Command: "<<Command()<<endl;
	out<<"Value:   "<<Value()<<endl;
	out<<endl;
}


void OrbitAnswerRead2::Print(ostream& out)
{
	out<<"Command: "<<Command()<<endl;
	out<<"Value:   "<<Value()<<endl;
	out<<endl;
}

//-----------------------------------------------------------------------------

int OrbitAnswerReadbuffer1::Checksum(void)
{
	return Length()>0 ? (*this)[ Length() + 1 ]: 0;
}

int OrbitAnswerReadbuffer1::CalcChecksum(void)
{
	unsigned char checksum = 0;
	for(int i=0; i<(Length()-1); ++i)
		checksum +=(*this)[2 + i ];
	return checksum;
}


std::vector<int16_t> OrbitAnswerReadbuffer1::Values(void)
{
	std::vector<int16_t> values;
	int words = (Length()-1)/2;

	values.resize(words);
	
	int i = 2;
	for(auto& word: values){
		word = Short(i);
		i+=2;
	}
	return values;
}

void OrbitAnswerReadbuffer1::Print(ostream& out)
{
	out<<"Command: "<<Command()<<endl;
	out<<"Length:   "<<Length()<<endl;
	out<<"Checksum: "<<Checksum()<<endl;
	out<<"Calculated checksum: "<<CalcChecksum()<<endl;
	out<<endl;
	out<<"Values:"<<endl;
	for(auto& word: Values())
		out<<word<<" "<<endl;
	out<<endl;
	out<<endl;
}
