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


#define EVENT_NOT_SCHEDULED 0
#define EVENT_FINISHED 1
#define EVENT_PAUSED 2
#define EVENT_REMOVED_FROM_SCHEDULE 3
#define EVENT_REGISTERED_INTO_SCHEDULE 4




class IrrEvent
{
  public:
    IrrEvent();
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
    int  process(IrrValve* valveControl);     // check if it's this events turn and execute
    boolean saveEventToSD(char* fname);
    boolean readEventFromSD(char* fname);
    
  private:

    void extractSD(String *txtLine);
  
    int relay_number;
    int parallel_permission;
    
    boolean start_days[7];
    int start_time_hour;
    int start_time_min;
    int duration;
    int interval_len;
    int interval_pause;

    boolean event_scheduled;
    boolean event_paused;
    boolean event_finished;
    boolean event_running;
    boolean interval_active;

    int duration_countdown;
    int interval_len_countdown;
    int interval_pause_countdown;

    File myFile;
};


















#endif
