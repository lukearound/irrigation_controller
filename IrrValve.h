/*
  IrrValve.h - Library for irrigation valve control.
  Created by lukearound, 12.02.2017
  Released into the public domain.
*/

#ifndef IrrValve_h
#define IrrValve_h

#include "Arduino.h"
#include "IrrEvent.h"


class IrrValve
{
  public:
    IrrValve();
	boolean registerEvent(int relay_number, int duration, int event_id, IrrEvent* event_pointer);
    
    
  private:
};


















#endif
