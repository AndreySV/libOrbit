// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//


#ifndef _COMPORT_ITF_H_
#define _COMPORT_ITF_H_

#include <string>
#include <vector>
#include <stdexcept>


typedef long TBaudRate;

typedef enum {bs5=5,
	      bs6,
	      bs7,
	      bs8} TByteSize;

typedef enum {ptNONE=0,
	      ptODD,
	      ptEVEN,
	      ptMARK,
	      ptSPACE} TParity;

typedef enum {sb1BITS=0, sb1HALFBITS, sb2BITS} TStopBits;

typedef enum {rtsControlDisable = 0,
	      rtsControlEnable,
	      rtsControlHandShake,
	      rtsControlToggle } TRtsControl;


// default settings
const	std::string COM_DEFAULT_PORT        = "";
const	TBaudRate   COM_DEFAULT_BAUD_RATE   = static_cast<TBaudRate>(9600UL);
const	TByteSize   COM_DEFAULT_BYTE_SIZE   = bs8;
const	TParity     COM_DEFAULT_PARITY      = ptNONE;
const	TStopBits   COM_DEFAULT_STOP_BITS   = sb1BITS;
const	TRtsControl COM_DEFAULT_RTS_CONTROL = rtsControlDisable;


// interface virtual class
// for serial communication
class TComPortItf
{
	std::string     PortName;
	TBaudRate       BaudRate;
	TByteSize       ByteSize;
	TParity         Parity;
	TStopBits       StopBits;
	TRtsControl     RtsControl;

protected:


public:
	~TComPortItf() {};


	// accessors
	void SetPortName(const std::string& APort);
	void SetBaudRate(const TBaudRate ABaudRate);
	void SetByteSize(const TByteSize AByteSize);
	void SetParity(const TParity AParity);
	void SetStopBits(const TStopBits AStopBits);
	void SetRtsControl(const TRtsControl ARtsControl);

	std::string GetPortName(void) const;
	TBaudRate   GetBaudRate(void) const;
	TByteSize   GetByteSize(void) const;
	TParity     GetParity(void) const;
	TStopBits   GetStopBits(void) const;
	TRtsControl GetRtsControl(void) const;


	bool Init(const std::string& APort,
		  const TBaudRate ABaudRate,
		  const TByteSize AByteSize,
		  const TParity AParity,
		  const TStopBits AStopBits,
		  const TRtsControl ARtsControl);



	unsigned char GetTotalByteLength(void) const;

	// platform-depend functions
	virtual bool IsOpen(void) const = 0;
	virtual bool ApplySettings(void) = 0;

	virtual bool ReadByte(unsigned char& pValue) = 0;
	virtual bool WriteByte(unsigned char value) = 0;
	virtual bool Write(std::vector<unsigned char>& pBuf, unsigned long NumberOfToWrite) = 0;

	virtual void Flush(void) = 0;
	virtual bool Open(void) = 0;
	virtual void Close(void) = 0;
};
//---------------------------------------------------------------------------
#endif
