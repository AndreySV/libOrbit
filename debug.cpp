// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "debug.h"
#include "debug_names.h"

FILE* debug_output_file = NULL;
int   debug_current_level = DEF_MODE;

void debug_set_level(int level)
{
	if (level>MAX_MODE) level = MAX_MODE;
	if (level<0) level = 0;

	debug_current_level = level;
	debug_print( INFO, "Debug level set to %d\n", debug_current_level );
}

int debug_get_level(void)
{
	return debug_current_level;
}

#ifdef _WIN32
void debug_print_level(FILE* fd, int level)
{
	SYSTEMTIME local_time;
	if (level<NUM_DEBUG_MODES)
	{
		GetLocalTime( &local_time );
		fprintf(fd, "%02d.%02d.%04d %02d:%02d:%02d:%03d ",
			local_time.wDay,
			local_time.wMonth,
			local_time.wYear,
			local_time.wHour ,
			local_time.wMinute,
			local_time.wSecond,
			local_time.wMilliseconds );

		fprintf(fd, "%s:", level_names[ level ] );
	}
}
#else
void debug_print_level(FILE* fd, int level)
{
	if (level<NUM_DEBUG_MODES)
	{
		time_t t = time(NULL);
		tm local_time;
		localtime_r( &t, &local_time);
		fprintf(fd, "%02d.%02d.%04d %02d:%02d:%02d ",
			local_time.tm_mday,
			local_time.tm_mon,
			local_time.tm_year,
			local_time.tm_hour,
			local_time.tm_min,
			local_time.tm_sec );


		fprintf(fd, "%s:", level_names[ level ] );
	}
}
#endif


void debug_print( int level, const char* format, ... )
{

	FILE* fd;
	fd = debug_output_file ? debug_output_file : stdout;


	if (level<=debug_current_level)
	{
		va_list args;

		// Initialise the va_list variable with the ... after fmt
		va_start(args, format);

		// Forward the '...' to vprintf
		debug_print_level( fd, level );
		vfprintf( fd, format, args );

		// Clean up the va_list
		va_end(args);
		fflush( fd );
	}
}


void debug_init(const char* filename, int level)
{

	if (!debug_output_file)
	{
		if (filename)
		{
			debug_output_file = fopen( filename, "at");
			if (!debug_output_file)
			{
				debug_print( ERR, "can not create debug log file %s\n",
					     filename);
			}
		}
		else  debug_print( WARN, "debug system is inited already\n" );
		debug_set_level ( level );
	}

}

void debug_dump_buffer(int level, unsigned char* buf, int size)
{
	FILE* fd;
	fd = debug_output_file ? debug_output_file : stdout;


	if (level<=debug_current_level)
	{
		debug_print_level( fd, level );
		for( int i=0; i<size; i++)
			fprintf( fd, "0x%02X ", buf[i] );
		fprintf( fd, "\n");
		fflush( fd );
	}
}

#ifdef _WIN32

void debug_print_win_error(int level, const char *msg ) {
	DWORD errCode = GetLastError();
	char *err;

	if (!FormatMessage(
		    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		    NULL,
		    errCode,
//	      MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		    0,
		    (LPTSTR) &err,
		    0,
		    NULL))
	{
		DP( level, "%s: format message of errCode 0x%lx failed with 0x%lx\n", msg, errCode, GetLastError());
		return;
	}

	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "%s: %s", msg, err);
	DP( level, "%s", buffer);
	LocalFree(err);
}

#endif
