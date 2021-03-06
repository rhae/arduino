/**
 * \file GP2Y0A02F.h
 * \author Rüdiger Härtel
 *
 *
 */

#ifndef _GPY20A02F_H_
#define _GPY20A02F_H_

#include <stdint.h>

#define GP2Y0A02F_VERSION (_F("1.0.0"))

#if linux
#else
# if defined(ARDUINO) && ARDUINO >= 100
#  include "Arduino.h"
# else
#  include "WProgram.h"
#  include <pins_arduino.h>
# endif
#endif


/* constant definitions
---------------------------------------------------------------------------*/

/* global defined data types
---------------------------------------------------------------------------*/

/* list of global defined functions
---------------------------------------------------------------------------*/

class GP2Y0A02F
{
  
public:
           GP2Y0A02F( int Pin );
  
  void     begin();
  
  uint16_t getDistanceCm();
  uint16_t getDistanceCm(uint16_t);
  uint16_t getDistanceRaw();

  
private:
  int _pin;
};


#endif
