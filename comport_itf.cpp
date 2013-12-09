// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//

#include "comport_itf.h"
#include "debug.h"

using std::string;
using std::vector;

bool TComPortItf::Init(const string& APortName,
		       const TBaudRate ABaudRate,
		       const TByteSize AByteSize,
		       const TParity AParity,
		       const TStopBits AStopBits,
		       const TRtsControl ARtsControl)
{
	try
	{
		bool Active = IsOpen();
		if (Active)
			Close();

		SetPortName(APortName);
		SetBaudRate(ABaudRate);
		SetByteSize(AByteSize);
		SetParity(AParity);
		SetStopBits(AStopBits);
		SetRtsControl(ARtsControl);

		if (Active)
			Open();

	}
	catch (std::runtime_error& e)
	{
		DP( DEBUG, "Init failed() %s\n", e.what() );
		return false;
	}
	catch (...)
	{
		return false;
	}
	return true;
}



unsigned char TComPortItf::GetTotalByteLength(void) const
{
        // Start bit + stop bit;
	unsigned char len = 2;
	len += (unsigned char)ByteSize;

	if (Parity!=ptNONE)
		len++;

	if (StopBits!=sb1BITS)
		len++;

	return(len);
}

void TComPortItf::SetPortName(const string& APortName)
{
	PortName = APortName;
}

void TComPortItf::SetBaudRate(const TBaudRate ABaudRate)
{
	BaudRate = ABaudRate;
	return;
}

void TComPortItf::SetByteSize(const TByteSize AByteSize)
{
	ByteSize = AByteSize;
}

void TComPortItf::SetRtsControl(const TRtsControl ARtsControl)
{
	RtsControl = ARtsControl;
}

void TComPortItf::SetParity(const TParity AParity)
{
	Parity = AParity;
}


void TComPortItf::SetStopBits(const TStopBits AStopBits)
{
	StopBits = AStopBits;
}



string TComPortItf::GetPortName(void) const
{
	return PortName;
};

TBaudRate TComPortItf::GetBaudRate(void) const
{
	return BaudRate;
};


TByteSize TComPortItf::GetByteSize(void) const
{
	return ByteSize;
};


TParity TComPortItf::GetParity(void) const
{
	return Parity;
};


TStopBits TComPortItf::GetStopBits(void) const
{
	return StopBits;
};


TRtsControl TComPortItf::GetRtsControl(void) const
{
	return RtsControl;
};
