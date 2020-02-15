#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
const int rs = 9, en = 8, d4 = 5, d5 = 4, d6 = 3, d7 = 2; int h=0,m=0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int val;
int tempPin = 2;

int pushButton = 6;
int count=1;

int sensorValue;
int digitalValue;
int state=0;
long int counter=0,cg=0;

    int UpperThreshold = 518;
    int LowerThreshold = 490;
    int reading = 0;
    float BPM = 0.0;
    bool IgnoreReading = false;
    bool FirstPulseDetected = false;
    unsigned long FirstPulseTime = 0;
    unsigned long SecondPulseTime = 0;
    unsigned long PulseInterval = 0;
    ///
int soundDetectedPin = 12; // Use Pin 10 as our Input
int soundDetectedVal = HIGH; // This is where we record our Sound Measurement
boolean bAlarm = false;

unsigned long lastSoundDetectTime; // Record the time that we measured a sound


int soundAlarmTime = 400; // Number of milli seconds to keep the sound alarm high



/* Create object named bt of the class SoftwareSerial */
SoftwareSerial GPS_SoftSerial(11, 10);/* (Rx, Tx) */
/* Create an object named gps of the class TinyGPSPlus */
TinyGPSPlus gps;      

volatile float minutes, seconds;
volatile int degree, secs, mins;

void setup() {
  Serial.begin(9600); /* Define baud rate for serial communication */
  GPS_SoftSerial.begin(9600); /* Define baud rate for software serial communication */
  pinMode(13, OUTPUT);
  pinMode( 3, INPUT);
  pinMode (soundDetectedPin, INPUT) ; // input from the Sound Detection Module
    lcd.begin(16, 2);
    lcd.print("Hello!");
      pinMode(pushButton, INPUT);
  }
int soundThreshold =150;
void loop() {

int buttonState = digitalRead(pushButton);
  if (buttonState==1){
           state=1;   };


  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
  
  val = analogRead(tempPin);
float mv = ( val/1024.0)*5000; 
float cel = mv/10 - 5 ;
float farh = (cel*9)/5 + 32;


  counter++;cg++;
  
  soundDetectedVal = digitalRead (soundDetectedPin) ; // read the sound alarm time
  
  if (soundDetectedVal == LOW) // If we hear a sound
  {
  
    lastSoundDetectTime = millis(); // record the time of the sound alarm
    // The following is so you don't scroll on the output screen
    if (!bAlarm){
      Serial.println("GUN SHOT FIRED!! <GPS REPORT>"); state=1;lcd.clear();
      lcd.print("Danger!GunShot");delay(150);
      
      bAlarm = true;
    }
  }
  else
  {
    if( (millis()-lastSoundDetectTime) > soundAlarmTime  &&  bAlarm){
      //Serial.println("quiet");
      bAlarm = false;
    }
  }
  
      reading = analogRead(A1); 

      // Heart beat leading edge detected.
      if(reading > UpperThreshold && IgnoreReading == false){
        if(FirstPulseDetected == false){
          FirstPulseTime = millis();
          FirstPulseDetected = true;
        }
        else{
          SecondPulseTime = millis();
          PulseInterval = SecondPulseTime - FirstPulseTime;
          FirstPulseTime = SecondPulseTime;
        }
        IgnoreReading = true;
      }

      // Heart beat trailing edge detected.
      if(reading < LowerThreshold){
        IgnoreReading = false;
      }  

      BPM = ((1.0/PulseInterval) * 60.0 * 1000);

      //Serial.print(reading);
      //Serial.print("\t");
      //Serial.print(PulseInterval);
      //Serial.print("\t");
      BPM=map(BPM,9,250,64,85);
      if(counter==400)
      {
      Serial.print(BPM);
      Serial.print(" BPM");
      Serial.print("\t");
      
      Serial.print(cel);
      Serial.print("*C\t");
      }
Serial.flush();

        sensorValue = analogRead(0); // read analog input pin 0

        digitalValue = digitalRead(2); 
        if(sensorValue>400)
        {
        digitalWrite(13, HIGH);
        }
        else
        digitalWrite(13, LOW);
        if(counter==400)
      {
        Serial.print(sensorValue, DEC); // prints the value read
        Serial.print("ppm\n"); 
        //Serial.println(digitalValue, DEC);
        if(sensorValue>=500){Serial.println("DANGER - Chemicals in the Air!! \n<GPS REPORT>\n");state=1;lcd.clear();lcd.print("Danger!Chemicals");     delay(100);}
    counter=0; 
      }
        
        
        
        smartDelay(1); /* Generate precise delay of 1ms */
        unsigned long start;
        double lat_val, lng_val, alt_m_val;
        uint8_t hr_val, min_val, sec_val;
        bool loc_valid, alt_valid, time_valid;
        lat_val = gps.location.lat(); /* Get latitude data */
        loc_valid = gps.location.isValid(); /* Check if valid location data is available */
        lng_val = gps.location.lng(); /* Get longtitude data */
        alt_m_val = gps.altitude.meters();  /* Get altitude data in meters */
        alt_valid = gps.altitude.isValid(); /* Check if valid altitude data is available */
        hr_val = gps.time.hour(); /* Get hour */
        min_val = gps.time.minute();  /* Get minutes */
        sec_val = gps.time.second();  /* Get seconds */
        time_valid = gps.time.isValid();  /* Check if valid time data is available */

        h=0;m=0;
        if(time_valid && cg==500){lcd.clear();char time_string[32];
          if(min_val+30>59){m=min_val+30-60;h=1;} 
          else {m=min_val+30;}
          if(hr_val+5>23){h=h+hr_val+5-24;} 
          else {h=h+hr_val+5;}
          sprintf(time_string, "Time : %02d:%02d:%02d \n", h, m, sec_val);
          lcd.setCursor(0, 1);
          lcd.print(time_string);cg=0;}
        
        if(state==1)
        {
        if (!loc_valid)
        {          
          Serial.print("Latitude : ");
          Serial.println("***");
          Serial.print("Longitude : ");
          Serial.println("***");
        
        }
        else
        {
          DegMinSec(lat_val);
          Serial.print("Latitude in Decimal Degrees : ");
          Serial.println(lat_val, 6);
          Serial.print("Latitude in Degrees Minutes Seconds : ");
          Serial.print(degree);
          Serial.print("\t");
          Serial.print(mins);
          Serial.print("\t");
          Serial.println(secs);
          DegMinSec(lng_val); /* Convert the decimal degree value into degrees minutes seconds form */
          Serial.print("Longitude in Decimal Degrees : ");
          Serial.println(lng_val, 6);
          Serial.print("Longitude in Degrees Minutes Seconds : ");
          Serial.print(degree);
          Serial.print("\t");
          Serial.print(mins);
          Serial.print("\t");
          Serial.println(secs);
        }
        if (!alt_valid)
        {
          Serial.print("Altitude : ");
          Serial.println("***");
        }
        else
        {
          Serial.print("Altitude : ");
          Serial.println(alt_m_val, 6);    
        }
        if (!time_valid)
        {
          Serial.print("Time : ");
          Serial.println("***");
        }
        else
        {
          char time_string[32];
          sprintf(time_string, "Time : %02d/%02d/%02d \n", hr_val, min_val, sec_val);
          Serial.print(time_string);    
        }
        state=0;
        }
        else{state=0;}
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
/* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}

void DegMinSec( double tot_val)   /* Convert data in decimal degrees into degrees minutes seconds form */
{  
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}
