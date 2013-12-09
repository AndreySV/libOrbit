// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//  
// Redistribution of this file is permitted under the terms of the GNU 
// Public License (GPL)
//

#include <windows.h>
#include <memory>
#include <stdio.h>
#include "comport_win.h"
#include "debug.h"

using std::string;
using std::vector;


TComPort::TComPort(const string& APortName,
		   const TBaudRate ABaudRate,
		   const TByteSize AByteSize,
		   const TParity AParity,
		   const TStopBits AStopBits,
		   const TRtsControl ARtsControl)
{
	Init( APortName, ABaudRate, AByteSize, AParity, AStopBits, ARtsControl );
	Handle = INVALID_HANDLE_VALUE;
	Timeouts = std::unique_ptr<TComTimeouts>( new TComTimeouts(Handle) );
};



TComPort::~TComPort()
{
	Close();
}



bool TComPort::Open(void)
{
	if (IsOpen())
	{
		return true;
	}

	Handle = CreateFile( GetPortName().c_str(),       // Имя порта.
			     GENERIC_READ | GENERIC_WRITE,  // Ресурс для чтения и записи.
			     0,                             // устройство связи должно быть открыто для монопольного доступа.
			     NULL,                          // Нет атрибутов защиты.
			     OPEN_EXISTING,                 // Вернуть ошибку если ресурс не существует.
			     0,                             // Нет перекрытый с I/O.
			     NULL );                        // Для коммуникационных устройств параметр hTemplate должен быть NULL.

	if (ApplySettings()==false)
	{
		Handle = INVALID_HANDLE_VALUE;
		DP( DEBUG, "Can't open ComPort: Error invalid handle\n");
		return false;
	}
	Flush();
	return true;
}

void TComPort::Close(void)
{
	CloseHandle(Handle);
	Handle = INVALID_HANDLE_VALUE;
}


bool TComPort::ReadByte(unsigned char& Value)
{
	if (!IsOpen())
	{
		return false;
	}

	unsigned long NumberOfReaded = 0;
	BOOL res;
	res = ReadFile(                    // BOOL WriteFile(
		Handle,            // HANDLE hFile,
		&Value,            // PCVOID lpBuffer,
		sizeof(Value),   // DWORD nNumberOfBytesToWrite,
		&NumberOfReaded,   // PDWORD lpNumberOfBytesWritten,
		NULL );            // POVERLAPPED lpOverlapped);
	if (res==FALSE)
	{
		unsigned long  dwErrors;
		COMSTAT cstStatus;
		ClearCommError(Handle,&dwErrors,&cstStatus);
		DP( DEBUG, "ComPort: Rx error!\n");
		return false;
	}
	return ((res==TRUE)&&(NumberOfReaded==sizeof(Value)));
}

bool TComPort::WriteByte(unsigned char value)
{
	if (!IsOpen())
	{
		return (false);
	}
	unsigned long NumberOfWritten = 0;
	BOOL res;
	res = WriteFile(                   // BOOL WriteFile(
		Handle,            // HANDLE hFile,
		&value,            // PCVOID lpBuffer,
		sizeof(value),     // DWORD nNumberOfBytesToWrite,
		&NumberOfWritten,  // PDWORD lpNumberOfBytesWritten,
		NULL );            // POVERLAPPED lpOverlapped);
	if (res==FALSE)
	{
		unsigned long  dwErrors;
		COMSTAT cstStatus;
		ClearCommError(Handle,&dwErrors,&cstStatus);
		DP( DEBUG, "ComPort: Tx error!\n");
		return false;
	}
	return ((res==TRUE)&&(NumberOfWritten==sizeof(value)));
}

bool TComPort::Write(vector<unsigned char>& pBuf, unsigned long NumberOfToWrite)
{
	if(!IsOpen())
	{
		return  false;
	}
	unsigned long NumberOfWritten = 0;
	BOOL res;
	res = WriteFile(                   // BOOL WriteFile(
		Handle,            // HANDLE hFile,
		pBuf.data(),              // PCVOID lpBuffer,
		NumberOfToWrite,   // DWORD nNumberOfBytesToWrite,
		&NumberOfWritten,  // PDWORD lpNumberOfBytesWritten,
		NULL );            // POVERLAPPED lpOverlapped);
	if (res==FALSE)
	{
		unsigned long  dwErrors;
		COMSTAT cstStatus;
		ClearCommError(Handle,&dwErrors,&cstStatus);
		DP( DEBUG, "ComPort: Tx error!\n");
	}
	if (NumberOfWritten!=NumberOfToWrite)
	{
		DP( DEBUG, "ComPort: not all bytes were written!\n");
		return false;
	}
	return ((res==TRUE)&&(NumberOfWritten==NumberOfToWrite));
}


void TComPort::Flush(void)
{
	if(IsOpen())
	{
		PurgeComm(Handle, PURGE_TXABORT + PURGE_RXABORT + PURGE_TXCLEAR + PURGE_RXCLEAR);
		FlushFileBuffers(Handle);
	}
}



bool TComPort::ApplySettings(void)
{
	BOOL res = FALSE;
	if(IsOpen())
	{
		DCB ComDCB;
		ZeroMemory(&ComDCB, sizeof(ComDCB));
		ComDCB.DCBlength = sizeof(ComDCB);
		ComDCB.fBinary = true;
		ComDCB.BaudRate = GetBaudRate();
		ComDCB.ByteSize = GetByteSize();
		ComDCB.Parity   = GetParity();
		ComDCB.StopBits = GetStopBits();
		ComDCB.fRtsControl = GetRtsControl();

		res = SetCommState(Handle, &ComDCB);    // Apply port settings.
		if (res==FALSE)
		{
			DP_WIN( DEBUG, "SetCommState failed()");
			return (res==TRUE);
		}
		res = SetupComm(Handle, 512, 512);      // Set buffers size.
		if (res==FALSE)
		{
			DP_WIN( DEBUG, "SetupCom failed()");
			return (res==TRUE);
		}
		Flush();                                   // Clear buffers.
		res = Timeouts->SetTimeouts();          // Set ComTimeouts.
		if (res==FALSE)
		{
			return (res==TRUE);
		}
	}
	return (res==TRUE);
}


bool TComPort::IsOpen(void) const
{
	return (Handle!=INVALID_HANDLE_VALUE);
};


//---------------------------------------------------------------------------

TComTimeouts::TComTimeouts(HANDLE& ApComPortHandle):
	pComPortHandle(ApComPortHandle)
{
	ReadIntervalTimeout          = MAXDWORD;
	ReadTotalTimeoutMultiplier   = 0;
	ReadTotalTimeoutConstant     = 0;
	WriteTotalTimeoutMultiplier  = 0;
	WriteTotalTimeoutConstant    = 0;
}

bool TComTimeouts::SetTimeouts(void)
{
	COMMTIMEOUTS CTO;
	ZeroMemory(&CTO, sizeof(CTO));
	try
	{
		CTO.ReadIntervalTimeout         = ReadIntervalTimeout;
		CTO.ReadTotalTimeoutMultiplier  = ReadTotalTimeoutMultiplier;
		CTO.ReadTotalTimeoutConstant    = ReadTotalTimeoutConstant;
		CTO.WriteTotalTimeoutMultiplier = WriteTotalTimeoutMultiplier;
		CTO.WriteTotalTimeoutConstant   = WriteTotalTimeoutConstant;
	}
	catch( ... )
	{
		CTO.ReadIntervalTimeout         = MAXDWORD;
		CTO.ReadTotalTimeoutMultiplier  = 0;
		CTO.ReadTotalTimeoutConstant    = 0;
		CTO.WriteTotalTimeoutMultiplier = 0;
		CTO.WriteTotalTimeoutConstant   = 0;
	}
	return (TRUE==SetCommTimeouts(pComPortHandle, &CTO));
}

bool TComTimeouts::GetTimeouts(void)
{
	COMMTIMEOUTS CTO;
	ZeroMemory(&CTO, sizeof(CTO));
	if (!GetCommTimeouts(pComPortHandle, &CTO))
	{
		return (false);
	}
	ReadIntervalTimeout           = CTO.ReadIntervalTimeout;
	ReadTotalTimeoutMultiplier    = CTO.ReadTotalTimeoutMultiplier;
	ReadTotalTimeoutConstant      = CTO.ReadTotalTimeoutConstant;
	WriteTotalTimeoutMultiplier   = CTO.WriteTotalTimeoutMultiplier;
	WriteTotalTimeoutConstant     = CTO.WriteTotalTimeoutConstant;
	return (true);
}

