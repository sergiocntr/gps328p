#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

#include <TinyGPS++.h>
// The TinyGPS++ object
TinyGPSPlus gps;
static const uint32_t GPSBaud = 9600;



/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
*/


#include <SoftwareSerial.h>
SoftwareSerial ss(10, 11); // RX, TX
void setup(void) {
  u8g2.begin();
  ss.begin(GPSBaud);
}

void loop(void) {
  u8g2.clearBuffer();// clear the internal memory
  printFloat(gps.speed.kmph(), gps.speed.isValid());
  printInt(gps.satellites.value(), gps.satellites.isValid(), 2);
  printDateTime(gps.time);
  printAlt(gps.altitude.meters(), gps.altitude.isValid());
  u8g2.sendBuffer();					// transfer internal memory to the display
  smartDelay(1000);
}
static void printFloat(double val,bool valid)
{
  u8g2.setFont(u8g2_font_inr24_mn);	// choose a suitable font
  u8g2.setCursor(15 , 60);
  if (!valid | !val)
  {
    u8g2.print("-.-");	// write something to the internal memory
  }
  else
  {
    char buffer[8];
    snprintf(buffer, sizeof buffer, "%f", val);
    u8g2.print(val,1);	// write something to the internal memory
  }
  smartDelay(0);
}
// This custom version of delay() ensures that the gps object
// is being "fed".

static void printDateTime(TinyGPSTime &t)
{
  u8g2.setFont(u8g2_font_crox2c_tn);	// choose a suitable font
  char sz[5];
  sprintf(sz, "%02d:%02d ", t.hour(), t.minute());
  u8g2.drawStr(0,15,sz);	// write something to the internal memory
  smartDelay(0);
}
static void printInt(unsigned long val, bool valid, int len)
{
  u8g2.setFont(u8g2_font_crox2c_tn);	// choose a suitable font
  u8g2.setCursor(60 , 15);
  if (!valid)
  {
    u8g2.print("--");	// write something to the internal memory
  }
  else
  {
    u8g2.print(val);	// write something to the internal memory
  }
  smartDelay(0);
}
static void printAlt(float val,bool valid)
{
  u8g2.setFont(u8g2_font_crox2c_tn);	// choose a suitable font
  u8g2.setCursor(80 , 15);
  if (!valid | !val)
  {
    u8g2.print("---");	// write something to the internal memory
  }
  else
  {
    u8g2.print(val,1);
  }
  smartDelay(0);
}
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
