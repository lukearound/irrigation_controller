/*
  IrrEvent.cpp - Library for irrigation events.
  Created by lukearound, 12.02.2017
  Released into the public domain.
*/

#include "Arduino.h"
#include "IrrEvent.h"
#include <timeLib.h>
#include <SD.h>

#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINTLN(x)  Serial.println (x)
  #define DEBUG_PRINT(x)  Serial.print (x)
#else
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT(x)
#endif


IrrEvent::IrrEvent()
{
  relay_number = 0;
    
  for(int n=0; n<7; n++)  start_days[n] = 0;  
  
  parallel_permission      = 0;
  start_time_hour          = 0;
  start_time_min           = 0;
  duration                 = 0;
  interval_len             = 0;
  interval_pause           = 0;
  event_scheduled          = 0;
  event_paused             = 0;
  event_finished           = 0;
  event_running            = 0;
  interval_active          = 0;
  duration_countdown       = 0;
  interval_len_countdown   = 0;
  interval_pause_countdown = 0;
}



void IrrEvent::setRelay(int relay)        // set relay number
{
  relay_number = relay;
}



void IrrEvent::setStartTime(int h, int m)         // set new start time
{
  start_time_hour = h;
  start_time_min = m;
}



void IrrEvent::setDuration(int m)                 // [mins ]set how long this valve has to release water
{
  duration = m;
}



void IrrEvent::setStartDays(boolean *d)             // [1001010] set on which week days this event should be active
{
  for(int n=0; n<7; n++)
  {
    start_days[n] = d[n];  
  }
}



int IrrEvent::setScheduled(boolean scheduled)    // 0: exclude from schedule, 1: execute this event when due
{
  event_scheduled = scheduled;

  // cancel running processes if event was removed from schedule
  if(!event_scheduled)
  {
    event_paused             = 0;
    event_finished           = 0;
    event_running            = 0;
    duration_countdown       = 0;
    interval_len_countdown   = 0;
    interval_pause_countdown = 0;

    DEBUG_PRINTLN("removed event from schedule");
    return EVENT_REMOVED_FROM_SCHEDULE;
  }
  else 
  {
    DEBUG_PRINTLN("put event on schedule");
    return EVENT_REGISTERED_INTO_SCHEDULE;
  }
}



void IrrEvent::setPaused(boolean paused)          // 0: process normal, 1: stop processing and wait for unpause
{
  
}



void IrrEvent::setIntervalLen(int m)              // [min] set how lon this valve opens at a time
{
  
}



void IrrEvent::setIntervalPause(int m)            // [min] set pause duration in case interval length is exceeded (let water soak)
{
  
}



void IrrEvent::setIntervalActive(boolean active)  // 0: intverval length is ignored, 1: interval length is considered
{
  
}



void IrrEvent::setNextDay()                       // the next day has started -> reset finish event flag
{
  
}



void IrrEvent::resetEvent()                       // set all states and variables to default
{
  
}



int IrrEvent::process(IrrValve* valveControl)      // check if it's this events turn and execute
{
  
  // check for aboard conditions ***************************************************************
  if(!event_scheduled) 
  { 
    DEBUG_PRINTLN(String("event ") + String(relay_number) + String(" not scheduled"));
    return EVENT_NOT_SCHEDULED; 
  }
  if(event_finished) 
  {   
    DEBUG_PRINTLN(String("event ") + String(relay_number) + String(" has finished already"));
    return EVENT_FINISHED; 
  }
  if(event_paused) 
  {     
    DEBUG_PRINTLN(String("event ") + String(relay_number) + String(" is paused"));
    return EVENT_PAUSED; 
  }

  // check if event is due ********************************************************************
  float calcTimeEvent = start_time_hour + (float)start_time_min/60;
  float calcTimeNow   = (float)hour() + (float)minute()/60;
  
  DEBUG_PRINTLN(String(calcTimeNow) + String(" == ") + String(calcTimeEvent));
  
  if(calcTimeNow >= calcTimeEvent && !event_running)
  {
    duration_countdown = duration;
    event_running = 1;
    DEBUG_PRINTLN("event is due. set duration countdown = " + String(duration));
  }

  
  // check if event is running already ********************************************************


  

  
  
  
  
  
  return 0;
}



// save variables to SD card, use fname as filename
boolean IrrEvent::saveEventToSD(char* fname)
{
  char entry[50];
    
  SD.remove(fname);
  myFile = SD.open(fname, FILE_WRITE);

  sprintf(entry, "[relay_number=%i]",relay_number); myFile.println(entry);
  sprintf(entry, "[parallel_permission=%i]",parallel_permission); myFile.println(entry);
  sprintf(entry, "[start_days1=%i]",start_days[0]); myFile.println(entry);
  sprintf(entry, "[start_days2=%i]",start_days[1]); myFile.println(entry);
  sprintf(entry, "[start_days3=%i]",start_days[2]); myFile.println(entry);
  sprintf(entry, "[start_days4=%i]",start_days[3]); myFile.println(entry);
  sprintf(entry, "[start_days5=%i]",start_days[4]); myFile.println(entry);
  sprintf(entry, "[start_days6=%i]",start_days[5]); myFile.println(entry);
  sprintf(entry, "[start_days7=%i]",start_days[6]); myFile.println(entry);
  sprintf(entry, "[start_time_hour=%i]",start_time_hour); myFile.println(entry);
  sprintf(entry, "[start_time_min=%i]",start_time_min); myFile.println(entry);
  sprintf(entry, "[duration=%i]",duration); myFile.println(entry);  
  sprintf(entry, "[interval_len=%i]",interval_len); myFile.println(entry);
  sprintf(entry, "[interval_pause=%i]",interval_pause); myFile.println(entry);
  sprintf(entry, "[event_scheduled=%i]",event_scheduled); myFile.println(entry);
  sprintf(entry, "[interval_active=%i]",interval_active); myFile.println(entry);
    
  myFile.close();

  DEBUG_PRINTLN("event data written to SD");

}


// read parameter values from SD card and copy to event variables
boolean IrrEvent::readEventFromSD(char* fname)
{
  char   character;
  String txtLine;
  
  txtLine.reserve(50);
  txtLine = "";
  myFile = SD.open(fname, FILE_READ);

  // check if file was found
  if(!myFile)
  {
    DEBUG_PRINTLN("file not found!");
    return false;
  }

  // loop through lines in file
  while(myFile.available())
  {
    character = myFile.read();
    //DEBUG_PRINT(character);

    // EOL not reached
    if(character != 0x0D){   // 0x0D = CR character
      txtLine += character;
    }

    // EOL reached
    else
    {
      txtLine.concat(character);
      extractSD(&txtLine);
      txtLine = "";
    }
  }
  DEBUG_PRINTLN("SD card settings imported");
}




void IrrEvent::extractSD(String *txtLine)
{
  int eq;
  
  // remove everything beginning with "]"
  txtLine->remove(txtLine->indexOf("]"));

  // remove everything from the start to "["
  txtLine->remove(0,txtLine->indexOf("[")+1);

  // find "=" sign
  eq = txtLine->indexOf("=");

  // extract variable name and value to check for errors
  String var = txtLine->substring(0,eq);
  String val = txtLine->substring(eq+1);

  // todo: check if values are legal before assignment!

  // assign values to variable
  if(var.equalsIgnoreCase("duration")) duration = val.toInt();
  if(var.equalsIgnoreCase("parallel_permission")) parallel_permission = val.toInt();
  if(var.equalsIgnoreCase("start_days1")) start_days[0] = val.toInt();
  if(var.equalsIgnoreCase("start_days2")) start_days[1] = val.toInt();
  if(var.equalsIgnoreCase("start_days3")) start_days[2] = val.toInt();
  if(var.equalsIgnoreCase("start_days4")) start_days[3] = val.toInt();
  if(var.equalsIgnoreCase("start_days5")) start_days[4] = val.toInt();
  if(var.equalsIgnoreCase("start_days6")) start_days[5] = val.toInt();
  if(var.equalsIgnoreCase("start_days7")) start_days[6] = val.toInt();
  if(var.equalsIgnoreCase("start_time_hour")) start_time_hour = val.toInt();
  if(var.equalsIgnoreCase("start_time_min")) start_time_min   = val.toInt();
  if(var.equalsIgnoreCase("relay_number")) relay_number = val.toInt();
  if(var.equalsIgnoreCase("interval_len")) interval_len = val.toInt();
  if(var.equalsIgnoreCase("interval_pause")) interval_pause = val.toInt();
  if(var.equalsIgnoreCase("event_scheduled")) event_scheduled = val.toInt();
  if(var.equalsIgnoreCase("interval_active")) interval_active = val.toInt();
}






















//



