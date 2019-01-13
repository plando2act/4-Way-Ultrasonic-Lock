// Code from Ultrasonic 4 way lock device used in some CTF events for fun and agility
// seach for the gimmicscreen() module to set the flag/reward text
// also check the unlock variables to set the unlocking distances
// By Bob - releasing into the public domain for others to build and experience fun
// v1.0 released in January 2019

#include <LiquidCrystal.h>

// Rangefinder stuff to hook up the 4 ultrasonic sensors
//******************************************************
const int echo = 6              ;     // pin names are not difficult
const int trigger = 7           ;
const int echo2 = 0             ;
const int trigger2 = 1          ;
const int echo3 = 8             ;
const int trigger3 = 9          ;
const int echo4 = 10            ;
const int trigger4 = 13         ;

const int unlock = 33           ;      //  the unlock distance in centimeters
const int unlock2 = 88          ;      //  88 cm plus and minus 3 cm ..
const int unlock3 = 95          ;      //  feel free to change
const int unlock4 = 73          ;      //  for all 4 locks
const int margin = 3            ;      //  window in cm to approve the unlock distance. This works in + and - direction so the margin = 6 cm 
boolean lock  = false           ;
boolean lock2 = false           ;
boolean lock3 = false           ;
boolean lock4 = false           ;

long Distance                   ;      // Global Var for distance
long Distance2                  ;      // Global Var for distance
long Distance3                  ;      // Global Var for distance
long Distance4                  ;      // Global Var for distance
int offset = 0                  ;      // 0 cm 
long lastrangefindertime = 0    ;      // the last time the range was checked
long refresh_rangefinder = 4000 ;      // at least 1000 ms before next rangefinder call or you will break it
boolean newDist = false         ;
byte nulstand                   ;

//LCD stuff
//******************************************************
/* LiquidCrystal Library - display() and noDisplay() The LiquidCrystal library works with all LCD displays that are compatible with the
   Hitachi HD44780 driver. There are many of them out there, and you  can usually tell them by the 16-pin interface.
 * LCD RS pin to digital pin 12  * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5   * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3   * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground         * 10K resistor: ends to +5V and ground and wiper to LCD VO pin   */
// set up the LCD's number of columns and rows:
#define LCD_WIDTH 16
#define LCD_HEIGHT 2
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


//SETUP  Run Once
//******************************************************
void setup() {
//DEBUG only
// initialize serial communication at 9600 bits per second:
//Serial.begin(9600);
  
//LCD SETUP
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  welcomescreen()                 ;
  
//RANGEFINDER SETUP
  pinMode (trigger, OUTPUT);  //Define output pin - was important addition to make it work
  pinMode (echo, INPUT)    ;  //Define input pin - was important addition to make it work
  pinMode (trigger2, OUTPUT);  //Define output pin - was important addition to make it work
  pinMode (echo2, INPUT)    ;  //Define input pin - was important addition to make it work
  pinMode (trigger3, OUTPUT);  //Define output pin - was important addition to make it work
  pinMode (echo3, INPUT)    ;  //Define input pin - was important addition to make it work
  pinMode (trigger4, OUTPUT);  //Define output pin - was important addition to make it work
  pinMode (echo4, INPUT)    ;  //Define input pin - was important addition to make it work

  lastrangefindertime = millis();

//INIT offset in distance
   nulstand = 0;   // read installation standard hight from programm source

//CLOSURE OF SETUP
  mainscreen();    //prepare mainscreen
}
//END SETUP


//MAIN LOOP
//******************************************************
void loop() {
  //Next block is executed every refresh_rangefinder time to prevent breaking the sensors.. yes they do break below 1000 ms ..
  if ((millis() - lastrangefindertime) > refresh_rangefinder) {
    Distance = range();          //call rangefinder
    Distance2 = range2();        //call rangefinder
    Distance3 = range3();        //call rangefinder
    Distance4 = range4();        //call rangefinder
    newDist = true ;
    lastrangefindertime = millis();  //update last rangefindertime
  }
    
  if(newDist == true) {   
    PrintRange();                //Print the found range on the LCD 
    PrintRange2();               //Print the found range2 on the LCD
    PrintRange3();               //Print the found range2 on the LCD 
    PrintRange4();               //Print the found range2 on the LCD  
    newDist = false; 
    if ( (lock == true) and (lock2 == true) and (lock3 == true) and (lock4 == true) ) { 
    gimmicscreen();
    }    
  }                  
}
//END MAIN LOOP*****************************************


//SCREENS
//******************************************************
void welcomescreen() {
      lcd.setCursor(0, 0);
      lcd.print("<< Ultrasonic >>");
      lcd.setCursor(0, 1);
      lcd.print("<< 4 way lock >>");
      delay(3000);
      lcd.setCursor(0, 0);
      lcd.print("<< Prep date  >>");
      lcd.setCursor(0, 1);
      lcd.print("< 15 June 2018 >");
      delay(2000);
      lcd.setCursor(0, 0);
      lcd.print("+ means to far..");
      lcd.setCursor(0, 1);
      lcd.print("- is to close...");
      delay(6000);     
      lcd.setCursor(0, 0);
      lcd.print("<< 4 x 'Ok' is>>");
      lcd.setCursor(0, 1);
      lcd.print("<< the goal.  >>");
      delay(6000);
      lcd.setCursor(0, 0);
      lcd.print("<< Be a team  >>");
      lcd.setCursor(0, 1);
      lcd.print("< and have fun >");
      delay(3000);
}

void gimmicscreen() {
      lcd.setCursor(0, 0);
      lcd.print("<<!25 Seconds!>>");
      lcd.setCursor(0, 1);
      lcd.print(" FLAG  MADR4242 ");
      delay(25000);
      mainscreen();
      lastrangefindertime = millis();  //reset last rangefindertime
}

void mainscreen() {
     lcd.setCursor(0, 0)             ;
     lcd.print("?     Cm ?    Cm")   ;
     lcd.setCursor(0, 1)             ;
     lcd.print("?     Cm ?    Cm")   ;
}

void PrintRange() {
      lcd.setCursor(2, 0);       // position 1 line number 0
      lcd.print("   ");
      lcd.setCursor(2, 0);       // position 1 line number 0
      lcd.print(Distance);

      lcd.setCursor(0, 0);       // position 3 line number 0
      lcd.print("  ");
      lcd.setCursor(0, 0);       // position 3 line number 0
      
      if (Distance > (unlock + margin)) 
      { lcd.print("+");
      lock  = false  ;
      }
      else if (Distance < (unlock - margin))
      {lcd.print("-");
      lock  = false  ;
      }
      else if ( (Distance >= (unlock-margin)) and (Distance <= (unlock+margin)) )
      {lcd.print("Ok");
      lock  = true   ;
      }   
}

void PrintRange2() {
      lcd.setCursor(11, 0);       // position 12 line number 0
      lcd.print("   ");
      lcd.setCursor(11, 0);       // position 12 line number 0
      lcd.print(Distance2);

      lcd.setCursor(9, 0);       // position 10 line number 0
      lcd.print("  ");
      lcd.setCursor(9, 0);       // position 10 line number 0
      
      if (Distance2 > (unlock2 + margin)) 
      { lcd.print("+");
      lock2  = false  ;
      }
      else if (Distance2 < (unlock2 - margin))
      {lcd.print("-");
      lock2  = false  ;
      }
      else if ( (Distance2 >= (unlock2-margin)) and (Distance2 <= (unlock2+margin)) )
      {lcd.print("Ok");
      lock2  = true   ;
      }         
}

void PrintRange3() {
      lcd.setCursor(2, 1);       // position 1 line number 1
      lcd.print("   ");
      lcd.setCursor(2, 1);       // position 1 line number 1
      lcd.print(Distance3);

      lcd.setCursor(0, 1);       // position 3 line number 1
      lcd.print("  ");
      lcd.setCursor(0, 1);       // position 3 line number 1
      
      if (Distance3 > (unlock3 + margin)) 
      { lcd.print("+");
      lock3  = false  ;
      }
      else if (Distance3 < (unlock3 - margin))
      {lcd.print("-");
      lock3  = false  ;
      }
      else if ( (Distance3 >= (unlock3-margin)) and (Distance3 <= (unlock3+margin)) )
      {lcd.print("Ok");
      lock3  = true   ;
      }         
}

void PrintRange4() {
      lcd.setCursor(11, 1);       // position 12 line number 1
      lcd.print("   ");
      lcd.setCursor(11, 1);       // position 12 line number 1
      lcd.print(Distance4);

      lcd.setCursor(9, 1);       // position 10 line number 1
      lcd.print("  ");
      lcd.setCursor(9, 1);       // position 10 line number 1
      
      if (Distance4 > (unlock4 + margin)) 
      { lcd.print("+");
      lock4  = false  ;
      }
      else if (Distance4 < (unlock4 - margin))
      {lcd.print("-");
      lock4  = false  ;
      }
      else if ( (Distance4 >= (unlock4-margin)) and (Distance4 <= (unlock4+margin)) )
      {lcd.print("Ok");
      lock4  = true   ;
      }         
}

//RANGEFINDER
//******************************************************
int range() {
  long start;
  long watchdog;
  long elapsed;
  float speedofsound;
  float dist ;
  // trigger the rangefinder
  digitalWrite(trigger, HIGH);
  watchdog = 0;
  dist = 998;
  delay(2);
  digitalWrite(trigger, LOW);

  // wait for the echo to pulse back to 0
  while (!digitalRead(echo) and watchdog <= 200 ) {           
          watchdog++;
  } 
  if (watchdog > 200 ) {goto bailout ;} // this protects from hanging sensors.. they have that nasty habbit sometimes

  dist = 997 ;
  watchdog = 0;
  
  start = micros();           // start timer to measure output length on echo pin - this is not critical and does work
//  while (digitalRead(echo));  // wait for echo to return
  while (digitalRead(echo) and watchdog <= 6500 ) {           
          watchdog++;
  } 
  if (watchdog >6500) {
      goto bailout ;                     // this protects from hanging sensors.. they have that nasty habbit sometimes
  }
 
  elapsed = micros() - start; // stop timer

  speedofsound = 331.3 + (0.606 * 21); 
  dist = (elapsed * speedofsound /20000)+ offset ;

  if ( dist > 400) { 
       dist = 999;
  }
bailout:
  return dist;
}

int range2() {
  long start;
  long watchdog;
  long elapsed;
  float speedofsound;
  float dist ;
  // trigger the rangefinder
  digitalWrite(trigger2, HIGH);
  watchdog = 0;
  dist = 998;
  delay(2);
  digitalWrite(trigger2, LOW);

  // wait for the echo to pulse back to 0
  while (!digitalRead(echo2) and watchdog <= 200 ) {           
          watchdog++;
  } 
  if (watchdog > 200 ) {goto bailout ;}     // this protects from hanging sensors.. they have that nasty habbit sometimes

  dist = 997 ;
  watchdog = 0;
  
  start = micros();           // start timer to measure output length on echo pin - this is not critical and does work
//  while (digitalRead(echo2));  // wait for echo to return
  while (digitalRead(echo2) and watchdog <= 6500 ) {           
          watchdog++;
  } 
  if (watchdog >6500) {
      goto bailout ;                        // this protects from hanging sensors.. they have that nasty habbit sometimes
  }
 
  elapsed = micros() - start; // stop timer

  speedofsound = 331.3 + (0.606 * 21); 
  dist = (elapsed * speedofsound /20000)+ offset ;

  if ( dist > 400) { 
       dist = 999;
  }
bailout:
  return dist;
}

int range3() {
  long start;
  long watchdog;
  long elapsed;
  float speedofsound;
  float dist ;
  // trigger the rangefinder
  digitalWrite(trigger3, HIGH);
  watchdog = 0;
  dist = 998;
  delay(2);
  digitalWrite(trigger3, LOW);

  // wait for the echo to pulse back to 0
  while (!digitalRead(echo3) and watchdog <= 200 ) {           
          watchdog++;
  } 
  if (watchdog > 200 ) {goto bailout ;}          // this protects from hanging sensors.. they have that nasty habbit sometimes

  dist = 997 ;
  watchdog = 0;
  
  start = micros();           // start timer to measure output length on echo pin - this is not critical and does work
//  while (digitalRead(echo3));  // wait for echo to return
  while (digitalRead(echo3) and watchdog <= 6500 ) {           
          watchdog++;
  } 
  if (watchdog >6500) {
      goto bailout ;                            // this protects from hanging sensors.. they have that nasty habbit sometimes
  }
 
  elapsed = micros() - start; // stop timer

  speedofsound = 331.3 + (0.606 * 21); 
  dist = (elapsed * speedofsound /20000)+ offset ;

  if ( dist > 400) { 
       dist = 999;
  }
bailout:
  return dist;
}
int range4() {
  long start;
  long watchdog;
  long elapsed;
  float speedofsound;
  float dist ;
  // trigger the rangefinder
  digitalWrite(trigger4, HIGH);
  watchdog = 0;
  dist = 998;
  delay(2);
  digitalWrite(trigger4, LOW);

  // wait for the echo to pulse back to 0
  while (!digitalRead(echo4) and watchdog <= 200 ) {           
          watchdog++;
  } 
  if (watchdog > 200 ) {goto bailout ;}    // this protects from hanging sensors.. they have that nasty habbit sometimes

  dist = 997 ;
  watchdog = 0;
  
  start = micros();           // start timer to measure output length on echo pin - this is not critical and does work
//  while (digitalRead(echo4));  // wait for echo to return
  while (digitalRead(echo4) and watchdog <= 6500 ) {           
          watchdog++;
  } 
  if (watchdog >6500) {
      goto bailout ;                     // this protects from hanging sensors.. they have that nasty habbit sometimes
  }
 
  elapsed = micros() - start; // stop timer

  speedofsound = 331.3 + (0.606 * 21); 
  dist = (elapsed * speedofsound /20000)+ offset ;

  if ( dist > 400) { 
       dist = 999;
  }
bailout:
  return dist;
}
