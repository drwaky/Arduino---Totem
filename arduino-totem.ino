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
 * @version    0.4
 * @date       2014/12/11
 * @license    GPLv3 - https://www.gnu.org/licenses/gpl-3.0.html
 * @category   Arduino
 **
 * Arduino Based Pomodoro Totem Creature to the SBA Team at MediaNet Software
 **/
 
 const int restCycle = 4;        // Long rest cycle size
 const int systemLed = 13;       // Testing Led pin
 const int restLed = 2;          // Rest Led Arduino pin
 const int firstWorkLed = 3;     // First Wirk led Arduino pin
 const int workLedQuantity = 3;  // Quantity of Work Leds
 
 boolean working;  // Working mode On or Off
 
 int currentWorkLedOn = workLedQuantity;  // Quantity of Work Leds are on

 int systemLedState = LOW;    // Testing Led state.
 int restLedState = LOW;      // Rest Led state.
 boolean cycleSetSw = false;  // Switch variable to check or not the rest type.
 
 unsigned int pomodorosCount = 0;  // Pomodoros counter
 
 unsigned long systemLedPreviousMillis = 0;    // Testing Led time moment
 unsigned long workRestLedPreviousMillis = 0;  // Working or Resting Leds time moment
 
 const long systemLedInterval = 500;  // Testing Led time interval
 const long shortRestInterval = 500;  // Short rest time interval value in ms
 const long longRestInterval = 1500;  // Long rest time interval value in ms
 const long workInterval = 3000;      // Work time interval value in ms
 long restInterval = 0;               // Corrent Rest time interval in ms (can be long or short)
 

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


/**
 * Turn on all the work LEDs.
 * @param      {int} init - The first pin for a work led.
 * @param      {int} number - The numbers of work led.
 */
 void allWorkLedOn(int init, int number){
   int pin;
   int lastLed = firstWorkLed + number;
   for (int currentLed = firstWorkLed; currentLed < lastLed; currentLed++){
     digitalWrite(currentLed, HIGH);
   }
 }
 
 
 /**
 * Setup the environment.
 */
 void setup(){
   setupPins();
   pinMode(restLed, OUTPUT);
   pinMode(systemLed, OUTPUT);
   allWorkLedOn(firstWorkLed, workLedQuantity);
   working = true;
 }
 
 /**
  * Init all pines 
  */
void setupPins(){
   int pin;
   for (int i = 0; i < workLedQuantity; i++){
     pin = firstWorkLed + i;
     pinMode(pin, OUTPUT);
   }
}

 /**
 * Main infinite loop.
 */
 void loop(){
   unsigned long currentMillis = millis();
   
   
   /* LED 13th blinking - Only for dev env */
   if(currentMillis - systemLedPreviousMillis >= systemLedInterval){
     systemLedPreviousMillis = currentMillis;
     systemLedState = toggleLedState(systemLedState);
     digitalWrite(systemLed, systemLedState);
   }
   /* --- */
   
   
   if(working){
     if((currentMillis - workRestLedPreviousMillis >= workInterval / workLedQuantity) && (currentWorkLedOn > 0)){
       currentWorkLedOn--;
       workRestLedPreviousMillis = currentMillis;
       digitalWrite(firstWorkLed + currentWorkLedOn, LOW);
     } else if(currentWorkLedOn == 0){
       pomodorosCount++;
       workRestLedPreviousMillis = currentMillis;
       restLedState = toggleLedState(restLedState);
       digitalWrite(restLed, restLedState);
       working = false;
       cycleSetSw = false;
     }
   } else {
     if(!cycleSetSw){
       if(pomodorosCount % restCycle == 0){
         restInterval = longRestInterval;
         cycleSetSw = true;
       } else {
         restInterval = shortRestInterval;
         cycleSetSw = true;
       }
     }
     
     if(currentMillis - workRestLedPreviousMillis >= restInterval){
       workRestLedPreviousMillis = currentMillis;
       allWorkLedOn(firstWorkLed, workLedQuantity);
       currentWorkLedOn = workLedQuantity;
       restLedState = toggleLedState(restLedState);
       digitalWrite(restLed, restLedState);
       working = true;
     }
   }
 }
