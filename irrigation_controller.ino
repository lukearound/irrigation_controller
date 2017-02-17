#include <TimeLib.h>         // http://www.arduino.cc/playground/Code/Time
#include <Wire.h>            // http://arduino.cc/en/Reference/Wire
#include <SD.h>
#include <DS3232RTC.h>       // http://github.com/JChristensen/DS3232RTC
#include "IrrValve.h";
#include "IrrEvent.h";


#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINTLN(x)  Serial.println (x)
  #define DEBUG_PRINT(x)  Serial.print (x)
#else
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT(x)
#endif


int relayPinN = 3;             // number of attached relays
int relayPin[] = {5, 6, 7};    // digitalPin numbers linked to the attached relays

IrrValve* valveControl;   // handles all valve operations, triggered by IrrEvents
IrrEvent  event[12];       // 4 events per 3 relays



void setup() {
  pinMode(53, OUTPUT);  // for SD card use
  Serial.begin(9600);
  if(!SD.begin(51))
  {
    DEBUG_PRINTLN("SD card initialization failed!");
    DEBUG_PRINTLN("restart system if SD access is needed");
    return false;
  }
  DEBUG_PRINTLN("SD card initialized.");

  
  for(int n=0; n<relayPinN; n++)
  {
    pinMode(relayPin[n], OUTPUT);      // sets the digital pin as output      
    digitalWrite(relayPin[n], HIGH);   // sets the LED on
  }

  /* initialize time (only at first start)
     setTime(hr,min,sec,day,month,yr)         */
  /* setTime(17, 14, 01, 12, 2, 2017);   //set the system time
  RTC.set(now());                     //set the RTC from the system time */

  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  setSyncInterval(36000);     // sync every 10 hours
  if(timeStatus() != timeSet) 
      DEBUG_PRINTLN("Unable to sync with the RTC");
  else
      DEBUG_PRINTLN("RTC has set the system time");  

  /* initialize all events with relay number and name
     four events are reserved per valve/relay             */
  for(int n=0; n<3; n++)
  {
    for(int m=0; m<4; m++)
    {
      event[n*4+m].setRelay(n);
    }
  }


  /*boolean days[7] = {1,1,1,1,1,1,1};
 
  event[0].setStartTime(21, 59);
  event[0].setStartDays(days);
  event[0].setScheduled(1);
  event[0].setDuration(10);
*/

  char fname[] = "name0.txt";
 
  //event[0].saveEventToSD(fname);
   event[0].readEventFromSD(fname);

  

          
}









void loop() {
  // TOGGLE: digitalWrite(relayPin1, !digitalRead(relayPin1));
  
  //Serial.println(digitalRead(relayPin[0]));
  /*Serial.print(weekday()); Serial.print(" ");
  Serial.print(hour()); ; Serial.print("h ");
  Serial.print(minute()); ; Serial.println("min ");*/
  
  event[0].process(valveControl);
  
  delay(1000);

}



































//

