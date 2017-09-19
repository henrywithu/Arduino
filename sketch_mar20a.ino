#include <dht.h>
#include <IRremote.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <LiquidCrystal.h>

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

LiquidCrystal lcd(7, 2, 3, 4, 5 , 6);

#include <dht.h>
dht DHT;
#define DHT11_PIN 11

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
}



void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.write("Angle1: ");
  lcd.setCursor(0, 1);
  lcd.write("Angle2: ");

  Serial.begin(9600);

  Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  } 
  
  
  /* Display some basic information on this sensor */
  displaySensorDetails();

}

void loop(void) {
  Serial.println();

  float sensorValue = analogRead(A0); // read the input on analog pin 0:
  Serial.print("Raw yaw input value = ");
  Serial.println(sensorValue);
  float voltage = sensorValue * (5.0 / 1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.print("Scaled yaw input value = ");
  Serial.println(voltage);
  float temp;
  if (voltage > 2.5) {
    temp = 0;
  }
  if ((2.2 < voltage ) && (2.5 >= voltage)) {
    temp = -2.667 * voltage + 6.667;
  }
  if ((1.9 < voltage) && (2.2 >= voltage)) {
    temp = -5 * voltage + 11.8;
  }
  if ((1.6 < voltage) && (1.9 >= voltage)) {
    temp = -9 * voltage + 19.4;
  }
  if (voltage <= 1.6) {
    temp = 5;
  }
  Serial.print("Open loop input value = ");
  Serial.println(temp);

  float flex = analogRead(A2);
  //float flex1=flex1 * (5.0 /1023.0);
  //Serial.print("Raw flex input value = ");
  //Serial.println(flex);
  float flex2 = -0.05 * flex + 34.3;
  //Serial.print("Scaled flex input value comparing to yaw = ");
  //Serial.println(flex2);


  float voltageFinal2 = temp / 5 * 128;
 
  sensors_event_t event; 
  mag.getEvent(&event);
 
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

  float heading = atan2(event.magnetic.x, event.magnetic.y);
  
  float declinationAngle = 0.22;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  float hd=headingDegrees;
  hd=(hd-232)/27*45;
  if (hd < 0) {
      hd = 0;
    }
    if (hd > 45) {
      hd = 45;
    }
  if (hd<13){
    hd=hd+4;
  }
  if ((hd>=13) &&(hd<21)){
    hd=hd+1;
  }
  if ((hd>=21) && (hd<41)) {
    hd=hd-1;
  }
    
  Serial.print("Heading (degrees): "); Serial.println(hd);

    float angle1;
    if (voltage > 2.5) {
      angle1 = 0;
    }
    if (voltage < 1.6) {
      angle1 = 45;
    }
    if ((voltage >= 1.6) && (voltage <= 2.5)) {
      angle1 = -50 * voltage + 125;
    }

    if (angle1-hd>6){
      voltageFinal2=voltageFinal2+0.3;
    }

    analogWrite(9, voltageFinal2);



//    float angle2;
//    if (flex2 > 2.5) {
//      angle2 = 0;
//    }
//    if (flex2 < 1.6) {
//      angle2 = 45;
//    }
//    if ((flex2 >= 1.6) && (flex2 <= 2.5)) {
//
//      angle2 = -50 * flex2 + 125;
//
//    }
    lcd.setCursor(8, 0);
    lcd.print(angle1);
    lcd.setCursor(8, 1);
    lcd.print(hd);


    int chk = DHT.read11(DHT11_PIN);
    Serial.print("Temperature = ");
    Serial.println(DHT.temperature, 1);
    Serial.print("Humidity = ");
    Serial.println(DHT.humidity, 1);

    
    delay(2000);
  }


