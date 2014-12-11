/********
 * |)|2\/\//-\|<`/
 **
 * Arduino - Pomodoro Totem Creature.
 **
 * @author     Joaquin Lopez <drwaky@gmail.com> 
 * @twitter    @drwaky
 * @bitbucket  drwaky
 * @github     drwaky
 * @copyright  Copyright (c) 2014, DrWaky
 * @version    0.3
 * @date       2014/12/11
 * @license    GPLv3 - https://www.gnu.org/licenses/gpl-3.0.html
 * @category   Arduino
 **
 * Arduino Based Pomodoro Totem Creature to the SBA Team at MediaNet Software
 **/
 
 const int restCycle = 4;
 const int defaultLed = 13;
 const int workLed = 2;
 const int restLed = 3;
 int defaultLedState = LOW;
 int workLedState = HIGH;
 int restLedState = LOW;
 
 char* mode = "working";
 
 unsigned int pomodorosCount = 0;
 
 unsigned long defaultLedPreviousMillis = 0;
 unsigned long workRestLedPreviousMillis = 0;
 
 const long defaultLedInterval = 500;
 const long shortRestInterval = 500;
 const long longRestInterval = 1500;
 const long workInterval = 3000;
 long restInterval = 0;
 

/**
 * Returns the opposite state of a LED.
 * @param      {int} val - The current value of the led to toggle.
 * @returns    {int} The opposite state: LOW => HIGH, HIGH => LOW.
 */
 int toggleLedState(int val){
   int res;
   if(val == HIGH){
     res = LOW;
   } else {
     res = HIGH;
   }
   return res;
 }
 
 
 
 void setup(){
   pinMode(defaultLed, OUTPUT);
   pinMode(workLed, OUTPUT);
   pinMode(restLed, OUTPUT);
   digitalWrite(workLed, workLedState);
 }


 
 void loop(){
   unsigned long currentMillis = millis();
   
   if(currentMillis - defaultLedPreviousMillis >= defaultLedInterval){
     defaultLedPreviousMillis = currentMillis;
     defaultLedState = toggleLedState(defaultLedState);
     digitalWrite(defaultLed, defaultLedState);
   }
   
   if(mode == "working"){
     if(currentMillis - workRestLedPreviousMillis >= workInterval){
       pomodorosCount++;
       workRestLedPreviousMillis = currentMillis;
       workLedState = toggleLedState(workLedState);
       restLedState = toggleLedState(restLedState);
       digitalWrite(workLed, workLedState);
       digitalWrite(restLed, restLedState);
       mode = "resting";
     }
   } else if(mode == "resting"){
     if(pomodorosCount % restCycle == 0){
       restInterval = longRestInterval;
     } else {
       restInterval = shortRestInterval;
     }
     
     if(currentMillis - workRestLedPreviousMillis >= restInterval){
       workRestLedPreviousMillis = currentMillis;
       workLedState = toggleLedState(workLedState);
       restLedState = toggleLedState(restLedState);
       digitalWrite(workLed, workLedState);
       digitalWrite(restLed, restLedState);
       mode = "working";
     }
   }
 }
