// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//


#include <string>
#include <memory>

#include <termios.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <fcntl.h>
#include <unistd.h>

#include "comport_linux.h"
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
	fd = 0;
	Init( APortName, ABaudRate, AByteSize, AParity, AStopBits, ARtsControl );
};



TComPort::~TComPort()
{
	Close();
}



bool TComPort::Open(void)
{
	if (IsOpen())
		return true;

	fd = open( GetPortName().c_str() , O_RDWR | O_NOCTTY | O_NONBLOCK | O_SYNC );
	if ( fd == -1) {
		DP( DEBUG, "Can't open ComPort %s: Error invalid handle\n", GetPortName().c_str() );
		fd = 0;
		return false;
	}


	if (ApplySettings()==false)
	{
		Close();
		DP( DEBUG, "Can't apply settings ComPort\n");
		return false;
	}
	Flush();
	return true;
}

void TComPort::Close(void)
{
	close(fd);
	fd = 0;
}


bool TComPort::ReadByte(unsigned char& Value)
{
	if (!IsOpen())
		return false;

	fcntl(fd, F_SETFL, O_NONBLOCK);

	int NumberOfReaded = 0;
	NumberOfReaded = read(fd, &Value, 1);
	if (NumberOfReaded!=1) {
		// if (NumberOfReaded<1)
		// DP( DEBUG, "ComPort: Rx error!\n");
		return false;
	}

	return ( NumberOfReaded==sizeof(Value) );
}

bool TComPort::WriteByte(unsigned char value)
{
	vector<unsigned char> data(1,value);
	return Write( data, 1);
}

bool TComPort::Write(vector<unsigned char>& pBuf, unsigned long NumberOfToWrite)
{
	if(!IsOpen())
		return false;

	unsigned long NumberOfWritten = 0;

	NumberOfWritten = write( fd, pBuf.data() , NumberOfToWrite );

	if (NumberOfWritten!=NumberOfToWrite) {
		if (NumberOfWritten<0)
			DP( DEBUG, "ComPort: Tx error!\n");
		DP( DEBUG, "ComPort: not all bytes were written!\n");
	}
	return ((NumberOfWritten==NumberOfToWrite));
}


void TComPort::Flush(void)
{
	if(! IsOpen())
		return;

	// purge input and output buffers
	tcflush(fd, TCIOFLUSH);
}


int TComPort::RateToConstant(TBaudRate baudrate) const {
#define B(x) case x: return B##x
	switch(baudrate) {
		B(50);     B(75);     B(110);    B(134);    B(150);
		B(200);    B(300);    B(600);    B(1200);   B(1800);
		B(2400);   B(4800);   B(9600);   B(19200);  B(38400);
		B(57600);  B(115200); B(230400); B(460800); B(500000);
		B(576000); B(921600); B(1000000);B(1152000);B(1500000);
		default: return 0;
	}
#undef B
}


bool TComPort::ApplyBaudrate(void)
{
	if( !IsOpen() )
		return false;

	struct termios options;
	struct serial_struct serinfo;
	int speed = 0;

	speed = RateToConstant( GetBaudRate() );

	if (speed == 0) {
		/* Custom divisor */
		serinfo.reserved_char[0] = 0;
		if (ioctl(fd, TIOCGSERIAL, &serinfo) < 0)
			return -1;
		serinfo.flags &= (int)(~ASYNC_SPD_MASK);
		serinfo.flags |= (int)ASYNC_SPD_CUST;
		serinfo.custom_divisor = (serinfo.baud_base + (GetBaudRate() / 2)) / GetBaudRate();
		if (serinfo.custom_divisor < 1)
			serinfo.custom_divisor = 1;
		if (ioctl(fd, TIOCSSERIAL, &serinfo) < 0)
			return -1;
		if (ioctl(fd, TIOCGSERIAL, &serinfo) < 0)
			return -1;
		if (serinfo.custom_divisor * GetBaudRate() != serinfo.baud_base) {
			DP( DEBUG, "actual baudrate is %d / %d = %f",
			    serinfo.baud_base, serinfo.custom_divisor,
			    ((float)serinfo.baud_base) / serinfo.custom_divisor );
		}
	}

	fcntl(fd, F_SETFL, 0);
	tcgetattr(fd, &options);
	cfsetispeed(&options, (speed_t)(speed ? speed : B38400));
	cfsetospeed(&options, (speed_t)(speed ? speed : B38400));
	cfmakeraw(&options);
	options.c_cflag |= (CLOCAL | CREAD);

	if  (tcsetattr(fd, TCSANOW, &options) == -1) {
		DP( ERR, "%s: failed to write settings\n", __FUNCTION__ );
		return false;
	}
	return true;
}

bool TComPort::ApplyRtsControl(void)
{
	if( !IsOpen() )
		return false;


	struct termios options;

	tcgetattr(fd, &options);

	switch( GetRtsControl() ) {
		case rtsControlHandShake:
			options.c_cflag |= CRTSCTS;
			break;
		case rtsControlDisable:
			options.c_cflag &= ~CRTSCTS;
			break;
		default:
			DP( ERR, "Unknown rts control (%d)\n", GetRtsControl() );
			return false;
	}

	if  (tcsetattr(fd, TCSANOW, &options) == -1) {
		DP( ERR, "%s: failed to write settings\n", __FUNCTION__ );
		return false;
	}
	return true;
}


bool TComPort::ApplyByteSize(void)
{
	if( !IsOpen() )
		return false;

	struct termios options;

	tcgetattr(fd, &options);
	options.c_cflag &= ~CSIZE;

	switch( GetByteSize() ) {
		case bs5:
			options.c_cflag |= CS5;
			break;
		case bs6:
			options.c_cflag |= CS6;
			break;
		case bs7:
			options.c_cflag |= CS7;
			break;
		case bs8:
			options.c_cflag |= CS8;
			break;
		default:
			DP( ERR, "Unknown byte size (%d)\n", GetByteSize() );
			return false;
	}

	return tcsetattr(fd, TCSANOW, &options) == 0;
}

bool TComPort::ApplyParity(void)
{
	if( !IsOpen() )
		return false;

	struct termios options;

	tcgetattr(fd, &options);

	switch( GetParity() ) {
		case ptMARK:
			options.c_cflag |= CMSPAR;
			options.c_cflag |= PARODD;
			options.c_cflag |= PARENB;
			break;
		case ptSPACE:
			options.c_cflag |= CMSPAR;
			options.c_cflag &=~PARODD;
			options.c_cflag |= PARENB;
			break;
		case ptODD:
			options.c_cflag &=~CMSPAR;
			options.c_cflag |= PARENB;
			options.c_cflag |= PARODD;
			break;
		case ptEVEN:
			options.c_cflag &=~CMSPAR;
			options.c_cflag |= PARENB;
			options.c_cflag &=~PARODD;
			break;
		case ptNONE:
			options.c_cflag &= ~PARENB;
			break;
		default:
			DP( ERR, "Unknown parity (%d)\n", GetParity() );
			return false;
	}

	return tcsetattr(fd, TCSANOW, &options) == 0;
}


bool TComPort::ApplyStopBits(void)
{
	if( !IsOpen() )
		return false;

	struct termios options;
	tcgetattr(fd, &options);
	switch( GetStopBits() ) {
		case sb1BITS:
			options.c_cflag &=~CSTOPB;
			break;
		case sb2BITS:
			options.c_cflag |=CSTOPB;
			break;
		default:
			DP( ERR, "Unknown stop bits (%d)\n", GetStopBits() );
			return false;
	};

	return tcsetattr(fd, TCSANOW, &options) == 0;
}


bool TComPort::ResetSettings(void)
{
	if( !IsOpen() )
		return false;

	struct termios options;
	tcgetattr(fd, &options);
	cfmakeraw(&options);

	return tcsetattr(fd, TCSANOW, &options) ==0;
}



bool TComPort::ApplySettings(void)
{
	if( !IsOpen())
		return false;

	if (! ResetSettings() ) return false;
	if (! ApplyBaudrate() ) return false;
	if (! ApplyParity() ) return false;
	if (! ApplyByteSize() ) return false;
	if (! ApplyRtsControl() ) return false;

	Flush();

	return true;
}


bool TComPort::IsOpen(void) const
{
	return (fd!=0);
};
