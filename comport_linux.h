// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//

#ifndef _COMPORT_LINUX_H_
#define _COMPORT_LINUX_H_



#include <iostream>
#include <memory>
#include "comport_itf.h"




class TComPort: public TComPortItf
{
	int fd;

	int  RateToConstant(TBaudRate baudrate) const;
	bool ResetSettings(void);
	bool ApplyRtsControl(void);
	bool ApplyBaudrate(void);
	bool ApplyParity(void);
	bool ApplyStopBits(void);
	bool ApplyByteSize(void);
public:
	TComPort(const std::string& APortName  = COM_DEFAULT_PORT,
		const TBaudRate ABaudRate      = COM_DEFAULT_BAUD_RATE,
		const TByteSize AByteSize      = COM_DEFAULT_BYTE_SIZE,
		const TParity AParity          = COM_DEFAULT_PARITY,
		const TStopBits AStopBits      = COM_DEFAULT_STOP_BITS,
		const TRtsControl ARtsControl  = COM_DEFAULT_RTS_CONTROL);


	~TComPort();

	virtual bool IsOpen(void) const;

	virtual bool ApplySettings(void);
	virtual bool ReadByte(unsigned char& pValue);
	virtual bool WriteByte(unsigned char value);
	virtual bool Write(std::vector<unsigned char>& pBuf, unsigned long NumberOfToWrite);
	virtual void Flush(void);
	virtual void Close(void);
	virtual bool Open(void);

};


#endif
