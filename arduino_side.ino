#include <WiFi.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const int MIN_PULSE_WIDTH = 600;
const int MAX_PULSE_WIDTH = 2600;
const int FREQUENCY = 50;

// L298N pins
const int IN1 = 27; 
const int IN2 = 26; 
const int ENA = 14; 

const int IN3 = 32; 
const int IN4 = 33; 
const int ENA_2 = 25;

const int PHOTO_SENSOR_READ = 35;

const int trigPin = 18;
const int echoPin = 19;
 
// ESP32 specific PWM setup 
const int freq = 100;
const int pwmChannel = 5;
const int resolution = 10; // this determines the pwm range, is specified in bits, 10 bits mean 0-1023, 8 would mean 0-255 etc.

const int freq_pwm2 = 100;
const int pwmChannel_pwm2 = 2;
const int resolution_pwm2 = 10; // this determines the pwm range, is specified in bits, 10 bits mean 0-1023, 8 would mean 0-255 etc.

// WiFi network name and password:
// substitute with the appropriate network properties where it is used
// in this project the network is a smartphone hotspot
const char* ssid = "#########";
const char* password = "#########";

// is led on or off
bool flagLED_ON_OFF = false;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

WiFiServer wifiServer(80);

Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40);

void setup()
{

  //l298n setup
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENA_2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  // L298N PWM setup
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(ENA, pwmChannel);

  ledcSetup(pwmChannel_pwm2, freq_pwm2, resolution_pwm2);
  ledcAttachPin(ENA_2, pwmChannel_pwm2);

  // Serial monitor setup
  Serial.begin(115200);

  // Initialize PCA9685
  pca9685.begin();
  // Set PWM Frequency to 50Hz
  pca9685.setPWMFreq(FREQUENCY);
  
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  delay(2000);
  
  wifiServer.begin();
  
}

void loop() {

  // Serial.println("In the loop");
   
  WiFiClient client = wifiServer.available();
 
  if (client) {

    bool stoppe_now = false;
    int prev_readout0 = 100;
    int prev_readout1 = 100;
    int prev_readout2 = 100;
    int prev_readout3 = 100;
    int prev_readout4 = 100;
    int prev_readout5 = 100;
    bool lessThan25 = false;
    Serial.println("Before the client connected loop");
    while (client.connected()) {
      
      bool flagF = false;
      bool flagB = false;
      bool flagL = false;
      bool flagR = false;

      // stop main engine
      bool flagX = false;

      // turn back to middle
      bool flagM = false;

      // turn LED on/off
      bool flagLIGHT = false;

      char ch_ = 'K';
      
      // client turnoff
      bool flagSTOP = false;

      

      char outBuf[20];   
      // Serial.println("Client is connected!");
      // Serial.println("Before the client available loop");

      int analog_value = analogRead(PHOTO_SENSOR_READ); // 4096 values
      // Serial.print("PHOTO_SENSOR_READ read value: ");
      // Serial.println(analog_value);
        
      if(analog_value < 100){
        LightOn();
      }
      if(analog_value >= 100){
        LightOff();
      }
      int prox_readout = proximitySensorReadout();
      prev_readout0 = prox_readout;
      prev_readout1 = prev_readout0; 
      prev_readout2 = prev_readout1;
      prev_readout3 = prev_readout2; 
      
      Serial.print("PROX SENSOR READOUT: ");
      Serial.println(prox_readout);
      Serial.println();
      if( prox_readout < 40 and !stoppe_now and prev_readout0 < 40 and prev_readout1 < 40){
        backwardAcceleration();
        delayMicroseconds(500);
        satu();
        satu();
        lessThan25 = true;

        Serial.println("Less than 25");
        Serial.println("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
      }
      if(prox_readout >= 40) lessThan25 = false;
      if(prox_readout < 40) lessThan25 = true;
      Serial.println();
      Serial.print("stoppe_now VALUE: ");
      Serial.println(stoppe_now);
      Serial.println();
      Serial.print("lessThan25 VALUE: ");
      Serial.println(lessThan25); 
      Serial.println();
      Serial.println();
      Serial.println();
      Serial.println();
      while (client.available()>0) {
        
        // ch_ = 'K';
        
        int num_ = client.read();
        // if(Serial.available()){
          // Serial.println("Serial available");
        ch_ = num_;  // Serial.read();
        // }
        
        ////////// FORWARD
        if(ch_ == 'F' && !lessThan25){
          flagF = true;
          stoppe_now = false;
        }        
        if(flagF){
          if(ch_ == 'W'){
            Serial.println("Forward!");
            
            // sprintf(outBuf,"sensor:change:%d\r\n", 108);
            // wifiServer.write(outBuf);
            forwardAcceleration();
            flagF = false;
            stoppe_now = false;
          }  
        }

        ////////// BACKWARD
        if(ch_ == 'B'){
          flagB = true;
        }        
        if(flagB){
          if(ch_ == 'W'){
            Serial.println("Backward!");
            // sprintf(outBuf,"sensor:change:%d\r\n", 64);
            // wifiServer.write(outBuf);
            backwardAcceleration();
            flagB = false;
            stoppe_now = true;
          }  
        }

        ////////// DISCONNECT
        if(ch_ == 'D'){
          flagSTOP = true;
        }        
        if(flagSTOP){
          if(ch_ == 'C'){
            goto STOP;
          }  
        }

        ////////// LEFT
        if(ch_ == 'L'){
          flagL = true;
        }
        if(flagL){
          if(ch_ == 'T'){
            Serial.println("Left!");
            // sprintf(outBuf,"sensor:change:%d\r\n", 16);
            // wifiServer.write(outBuf);
            turnByDegree(60);
            flagL = false;
          }
        }

        ////////// RIGHT
        if(ch_ == 'R'){
          flagR = true;
        }
        if(flagR){
          if(ch_ == 'T'){
            Serial.println("Right!");
            // sprintf(outBuf,"sensor:change:%d\r\n", 32);
            // wifiServer.write(outBuf);
            turnByDegree(130);
            flagR = false;
          }
        }

        prox_readout = proximitySensorReadout();
        prev_readout0 = prox_readout;
        prev_readout1 = prev_readout0; 
        prev_readout2 = prev_readout1;
        prev_readout3 = prev_readout2; 
        Serial.print("Prox sensor readout: ");
        Serial.println(prox_readout);
        

        ////////// SATU = STOP MOTOR
        if(ch_ == 'X'){
          flagX = true;  
        }
        if(flagX){
          if(ch_ == 'Z'){
            satu();
            flagX = false;
          }
        }

        
        

        ////////// STEER BACK TO MIDDLE 
        if(ch_ == 'M'){
          flagM = true;
        }
        if(flagM){
          if(ch_ == 'D'){
            turnBackMiddle();
            flagM = false;
            Serial.println("Middle!");
          }  
        }

        ////////// CONTROL LIGHTING
        if(ch_ == 'J'){
          flagLIGHT = true;
        }
        if(flagLIGHT){
          if(ch_ == 'I'){
            if(flagLED_ON_OFF){
              Serial.println("Turn OFF");
              LightOff();
              flagLED_ON_OFF = false;
            }
            else if(!flagLED_ON_OFF){
              Serial.println("Turn ON");
              LightOn();
              flagLED_ON_OFF = true;
            }
            flagLIGHT = false;
          }  
        }

        

        if(ch_ != 'K'){
          // Serial.print(ch_);
        }
      }
    }

    STOP:
    //// if client is killed
    client.stop();

    Serial.println("Client disconnected");

    delay(100);
  }
}


void turnByDegree(int degree){
  /* here i set up the desired angle*/
  pca9685.setPWM(0, 0, pulseWidth(degree));
  /* i give some time for the servo to setup, this depends on voltage, and the servo itself */
  delay(150);

}

void turnBackMiddle(){
  /* in my case, the "100" given to the pulseWidth means middle, it depends on the setup */
  pca9685.setPWM(0, 0, pulseWidth(100));
  delay(150);
}



/* this fn. returns an appropriate analog value according to the given angle
   asszem full felesleges TODO-megoldani en??lk??l 
*/
int pulseWidth(int angle){
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}

void forwardAcceleration(){
  // motor turn ON
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  ledcWrite(pwmChannel, 810);
}

void backwardAcceleration(){
  // motor turn ON
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  ledcWrite(pwmChannel, 810);
}

void LightOn(){
  // turn on led, half duty cycle
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(pwmChannel_pwm2, 832);
}

void LightOff(){
  // turn off led
  ledcWrite(pwmChannel_pwm2, 0);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW); 
}

void satu(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  
  ledcWrite(pwmChannel, 0);
  Serial.println("SATU");
}

int proximitySensorReadout(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;

  return int(distanceCm); 
}
