// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//



#ifndef _DEBUG_H_
#define _DEBUG_H_


//------------------------------------------------------------------------------
//
//                   Macro definitions and constants
//
//------------------------------------------------------------------------------

enum debug_modes_t
{
	NONE=0,
	ERR,
	WARN,
	INFO,
	DEBUG,
	LIGHTOPC_DEBUG,
	NUM_DEBUG_MODES,
	DEF_MODE = ERR,
	MAX_MODE=DEBUG
};

#define DP     debug_print
#define DP_WIN debug_print_win_error
#define DP_BUF debug_dump_buffer


//------------------------------------------------------------------------------
//
//                         Function prototypes
//
//------------------------------------------------------------------------------

void debug_set_level(int level);
int  debug_get_level(void);
void debug_print(int level, const char* format, ... );
void debug_init(const char* filename, int level);
void debug_dump_buffer(int level, unsigned char* buf, int size);
void debug_print_win_error(int level, const char* message);
#endif
