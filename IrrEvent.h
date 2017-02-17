/*
  IrrEvent.h - Library for irrigation events.
  Created by lukearound, 12.02.2017
  Released into the public domain.
*/

#ifndef IrrEvent_h
#define IrrEvent_h

#include "Arduino.h"
#include "IrrValve.h"
#include <SD.h>


#define EVENT_REMOVED_FROM_SCHEDULE 0
#define EVENT_ON_SCHEDULE           1
#define EVENT_RUNNING               2
#define EVENT_PAUSED                3
#define EVENT_FINISHED              4


//
//#define EVENT_REGISTERED_INTO_SCHEDULE 4

#define CYCLE_INACTIVE       0
#define CYCLE_INTERVAL_RUN   1
#define CYCLE_INTERVAL_PAUSE 2






class IrrEvent
{
  public:
    IrrEvent();
	void setID(int uid);                     // each event needs a unique ID
	void setRelay(int relay);                // set relay number
    void setStartTime(int h, int m);         // set new start time
    void setDuration(int m);                 // [mins ]set how long this valve has to release water
    void setStartDays(boolean *d);           // [1001010] set on which week days this event should be active
    int  setScheduled(boolean scheduled);    // 0: exclude from schedule, 1: execute this event when due
    void setPaused(boolean paused);          // 0: process normal, 1: stop processing and wait for unpause
    void setIntervalLen(int m);              // [min] set how lon this valve opens at a time
    void setIntervalPause(int m);            // [min] set pause duration in case interval length is exceeded (let water soak)
    void setIntervalActive(boolean active);  // 0: intverval length is ignored, 1: interval length is considered
    void setNextDay();                       // the next day has started -> reset finish event flag
    void resetEvent();                       // set all states and variables to default
    int  process(IrrValve* valveControl);    // check if it's this events turn and execute
	void cycleFinished();                    // to be called by valve control if cycle time has passed
    boolean saveEventToSD(char* fname);
    boolean readEventFromSD(char* fname);
    
  private:

    void extractSD(String *txtLine);
  
	int unique_event_id;
    int relay_number;
    int parallel_permission;  // for future use
    
    boolean start_days[7];    // [1,0,0,1,0,0,0]  / [su, mo, tu, we, th, fr, sa]
    int start_time_hour;      // [hr]
    int start_time_min;       // [min]
    int duration;             // [sec]
    int interval_len;         // [sec]
    int interval_pause;       // [sec]
	int inverval_active;

	int event_status;
	int cycle_status;

    int duration_countdown;                       // [sec]
	unsigned long interval_pause_restart_time;    // [msec]

	File myFile;
};


















#endif
