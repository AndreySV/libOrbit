// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//  
// Redistribution of this file is permitted under the terms of the GNU 
// Public License (GPL)
//


#ifndef _COMPORT_WIN_H_
#define _COMPORT_WIN_H_


#include <windows.h>
#include <iostream>
#include <memory>
#include "comport_itf.h"





class TComTimeouts
{
private:
	HANDLE&         pComPortHandle;
	unsigned long   ReadIntervalTimeout;
	unsigned long   ReadTotalTimeoutMultiplier;
	unsigned long   ReadTotalTimeoutConstant;
	unsigned long   WriteTotalTimeoutMultiplier;
	unsigned long   WriteTotalTimeoutConstant;
public:
	TComTimeouts(HANDLE& ApComPortHandle);
	bool SetTimeouts(void);
	bool GetTimeouts(void);
};




class TComPort: public TComPortItf
{
	std::unique_ptr<TComTimeouts>   Timeouts;
	HANDLE          Handle;

public:
	TComPort(const std::string& APortName        = COM_DEFAULT_PORT,
		 const TBaudRate ABaudRate      = COM_DEFAULT_BAUD_RATE,
		 const TByteSize AByteSize      = COM_DEFAULT_BYTE_SIZE,
		 const TParity AParity          = COM_DEFAULT_PARITY,
		 const TStopBits AStopBits      = COM_DEFAULT_STOP_BITS,
		 const TRtsControl ARtsControl  = COM_DEFAULT_RTS_CONTROL);


	~TComPort();

	virtual bool IsOpen(void) const;

	virtual	bool ApplySettings(void);
	virtual bool ReadByte(unsigned char& pValue);
	virtual bool WriteByte(unsigned char value);
	virtual bool Write(std::vector<unsigned char>& pBuf, unsigned long NumberOfToWrite);
	virtual void Flush(void);
	virtual void Close(void);
	virtual bool Open(void);

};
//---------------------------------------------------------------------------




#endif	
	
