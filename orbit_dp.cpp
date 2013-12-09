//
// Copyright (C) 2013 by Andrey Skvortsov
//
// Redistribution of this file is permitted under the terms of the GNU
// Public License (GPL)
//
#include "stod.h"
#include "orbit_dp.h"


double OrbitDP::ReadValue(void)
{

	if (inited) {
		try {
			int units;
			if (mult==1) {
				std::shared_ptr<OrbitAnswerRead1> value;
				value =  std::shared_ptr<OrbitAnswerRead1>( new OrbitAnswerRead1 );
				Read16(value);
				units = value->Value();
			}
			else {
				std::shared_ptr<OrbitAnswerRead2> value;
				value =  std::shared_ptr<OrbitAnswerRead2>( new OrbitAnswerRead2 );
				Read32(value);
				units = value->Value();				
			}
			
			return UnitsToValue( units );
		}
		catch(OrbitException& ex)
		{
			
			if ( (*ex.GetAnswer())[1] == ErrUnderRange ) return 0.0;
			if ( (*ex.GetAnswer())[1] == ErrOverRange ) return UnitsToValue( GetMaxUnits() );
		}


	}
	return 0.0;
}



int    OrbitDP::GetMaxUnits(void)
{
	int max;
	switch(mult) {
		default:
		case 1:
			max = (1<<14); // 14 bits
			break;
		case 4:
			max = (1<<16); // 16 bits
			break;
		case 16:
			max = (1<<18); // 18 bits
	}
	return max;
}

double OrbitDP::UnitsToValue(int units)
{
	double value = 0;
	if (inited) {
		value = units/(countPerUnitsOfMeasure*mult) + offset;
	}
	return value;
}

void   OrbitDP::SetAddress(unsigned char address)
{
	OrbitModule::SetAddress(address);
	
	typedef std::shared_ptr<OrbitAnswerGetinfo> OrbitGetinfoPtr;
	OrbitGetinfoPtr info = OrbitGetinfoPtr( new OrbitAnswerGetinfo );
	
	Getinfo( info );


	if ( info->ModuleType() == "DP  " ) {
		
		offset = std::stod( info->OS() );
		countPerUnitsOfMeasure = std::stod( info->CPUOM() );
		mult  = std::stol( info->Mult() );
		unitsOfMeasure = info->UOM();
		
		inited = true;		
	}
}
