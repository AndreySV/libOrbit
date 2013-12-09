// -*- mode: c++ -*-
//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//


#ifndef _ORBIT_DP_H_
#define _ORBIT_DP_H_

#include "orbit_module.h"

class OrbitDP: public OrbitModule
{
	double countPerUnitsOfMeasure;
	double offset;				
	bool   inited;
	int    mult;
	string unitsOfMeasure;
	double lastValue;

	double UnitsToValue(int units);
	int    GetMaxUnits(void);	
public:	
	OrbitDP(OrbitNetwork& network, const string& id = "" ):
		OrbitModule( network, id ),
		inited(false),
		lastValue(0) {};
	

	void   SetAddress(unsigned char address);
	double ReadValue(void);
};


#endif

