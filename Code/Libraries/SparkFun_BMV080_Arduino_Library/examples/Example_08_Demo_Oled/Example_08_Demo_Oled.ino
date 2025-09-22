/*
  Using the BMV080 Particulate Matter PM2.5 Sensor

  This example shows how display the PM1 and Pm2.5 readings on a SparkFun Qwiic 
  OLED Display, 1.3" wide.
  
  It uses the sensor in "continuous mode" to get
  particulate matter readings once every second.

  It uses polling of the device to check if new data is available.

  By: Pete Lewis
  SparkFun Electronics
  Date: September, 2024
  SparkFun code, firmware, and software is released under the MIT License.
    Please see LICENSE.md for further details.

  Hardware Connections:
  IoT RedBoard --> QWIIC
  QWIIC --> BMV080
  QWIIC --> QWIIC OLED Display

  BMV080 "mode" jumper set to I2C (default)

  Serial.print it out at 115200 baud to serial monitor.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/26554
*/

#define StatLedPin 13

// BMV080 Specifics
#include "SparkFun_BMV080_Arduino_Library.h" // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_BMV080
#include <Wire.h>

SparkFunBMV080 bmv080;           // Create an instance of the BMV080 class
#define BMV080_ADDR 0x57 // SparkFun BMV080 Breakout defaults to 0x57

float pm1Value = 0.0; // PM1 value - global so we can update it in the loop and 
float pm25Value = 0.0; // PM2.5 value - global so we can update it in the loop

// OLED Specifics
#include <SparkFun_Qwiic_OLED.h> //http://librarymanager/All#SparkFun_Qwiic_OLED

// The Library supports four different types of SparkFun boards. The demo uses the following
// defines to determine which device is being used. Uncomment the device being used for this demo.

//QwiicMicroOLED myOLED;
//QwiicTransparentOLED myOLED;
//QwiicNarrowOLED myOLED;
Qwiic1in3OLED myOLED;

#include "res/qw_bmp_sparkfun.h"

// Fonts
#include <res/qw_fnt_5x7.h>
#include <res/qw_fnt_8x16.h>
#include <res/qw_fnt_31x48.h>
#include <res/qw_fnt_7segment.h>
#include <res/qw_fnt_largenum.h>

// An array of fonts to loop over
QwiicFont *demoFonts[] = {
    &QW_FONT_5X7,
    &QW_FONT_8X16,
    &QW_FONT_31X48,
    &QW_FONT_LARGENUM,
    &QW_FONT_7SEGMENT};
int nFONTS = sizeof(demoFonts) / sizeof(demoFonts[0]);
int iFont = 0;

// Some vars for the title.
String strTitle = "<<Font>>";
QwiicFont *pFntTitle = &QW_FONT_5X7;

QwiicFont *pFntLabel = &QW_FONT_5X7;
QwiicFont *pFntValue = &QW_FONT_LARGENUM;

int width;
int height;

// x position of the PM2.5 label, this will be set in the 
// writeStaticDisplayItems() function
int xPosPM25; 

// Fuel Guage Specifics
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library
SFE_MAX1704X lipo(MAX1704X_MAX17048); // Create a MAX17048
double soc = 0; // Variable to keep track of LiPo state-of-charge (SOC)


// Some Dev boards have their QWIIC connector on Wire or Wire1
// This #ifdef will help this sketch work across more products

#ifdef ARDUINO_SPARKFUN_THINGPLUS_RP2040
#define wirePort   Wire1
#else
#define wirePort  Wire
#endif

void setup()
{
    pinMode(StatLedPin, OUTPUT);
    digitalWrite(StatLedPin, LOW);

    lipo.disableDebugging(); // disable debugging for the MAX1704X fuel gauge

    Serial.begin(115200);

    while (!Serial)
        delay(10); // Wait for Serial to become available.
    // Necessary for boards with native USB (like the SAMD51 Thing+).
    // For a final version of a project that does not need serial debug (or a USB cable plugged in),
    // Comment out this while loop, or it will prevent the remaining code from running.

    Serial.println();
    Serial.println("BMV080 Example 8 - OLED Display");

    wirePort.begin();

    // Initalize the OLED device and related graphics system
    if (myOLED.begin(wirePort) == false)
    {
        Serial.println("OLED Device begin failed. Freezing...");
        writeCenteredStringToDisplay("OLED Failure", true);
        while (true)
            ;
    }
    Serial.println("OLED Begin success");

      // save device dims for the test routines
    width = myOLED.getWidth();
    height = myOLED.getHeight();

    showSplash();

    myOLED.setFont(demoFonts[1]);

    if (bmv080.begin(BMV080_ADDR, wirePort) == false)
    {
        Serial.println(
            "BMV080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
            writeCenteredStringToDisplay("BMV080 Failure", true);
        while (1)
            ;
    }
    Serial.println("BMV080 found!");
    writeCenteredStringToDisplay("BMV080 Found", true);

    // wirePort.setClock(400000); //Increase I2C data rate to 400kHz

    /* Initialize the Sensor (read driver, open, reset, id etc.)*/
    bmv080.init();

    /* Set the sensor mode to continuous mode */
    if (bmv080.setMode(SF_BMV080_MODE_CONTINUOUS) == true)
    {
        Serial.println("BMV080 set to continuous mode");
        writeCenteredStringToDisplay("Continuous Mode Set", true);
    }
    else
    {
        Serial.println("Error setting BMV080 mode");
        writeCenteredStringToDisplay("BMV080 Mode Error", true);
    }

      // Set up the MAX17048 LiPo fuel gauge:
    if (lipo.begin() == false) // Connect to the MAX17043 using the default wire port
    {
        Serial.println(F("MAX17048 not detected. Please check wiring. Freezing."));
        writeCenteredStringToDisplay("MAX17048 Failure", true);
        while (1)
        ;
    }
    Serial.println("MAX17048 found!");

    // Quick start restarts the MAX17043 in hopes of getting a more accurate
	// guess for the SOC.
	lipo.quickStart();
}

void loop()
{
    if (bmv080.readSensor())
    {
        pm25Value = bmv080.PM25();
        pm1Value = bmv080.PM1();

        Serial.print(pm1Value);
        Serial.print("\t");
        Serial.print(pm25Value);
        writeStaticDisplayItems();
        writeValuesToDisplay();
        // lipo.getSOC() returns the estimated state of charge (e.g. 79%)
        soc = lipo.getSOC();
        Serial.print("\tBatt: ");
        Serial.print(soc); // Print the battery state of charge
        Serial.print(" %");
        writeBatteryLevelToDisplay();
        myOLED.display(); // actually command the display to show the scene

        if (bmv080.isObstructed() == true)
        {
            Serial.print("\tObstructed");
            writeObstructedBoarder();
            writeCenteredStringToDisplay("Obstructed", false); // don't clear the screen
            // so the obstructed message is displayed on top of the PM values
        }

        Serial.println();
        toggleHeartbeat();
    }
    delay(100);
}

void showSplash()
{
  int x0 = (width - QW_BMP_SPARKFUN.width) / 2;
  if (x0 < 0)
    x0 = 0;

  int y0 = (height - QW_BMP_SPARKFUN.height) / 2;
  if (y0 < 0)
    y0 = 0;

  myOLED.erase();
  myOLED.bitmap(x0, y0, QW_BMP_SPARKFUN);
  myOLED.display();
  delay(2000);

  // Clear the screen
  myOLED.erase();
  myOLED.display();
}

// Write the static display items to the screen
void writeStaticDisplayItems()
{
    // clear the screen
    myOLED.erase();

    myOLED.setFont(&QW_FONT_8X16);

    // draw the PM1 static text label
    // calculate the x position of the PM1 label
    // this is 1/4 the screen width minus 1/2 the width of the label
    int xPosPM1Text = myOLED.getWidth() / 4 - myOLED.getStringWidth("PM1") / 2;
    myOLED.text(xPosPM1Text, 0, "PM1", 1);

    // draw the PM2.5 static text label
    // calculate the x position of the PM2.5 label
    // this is 3/4 the screen width minus 1/2 the width of the label
    int xPosPM25Text = (myOLED.getWidth() / 4) * 3 - myOLED.getStringWidth("PM2.5") / 2;
    myOLED.text(xPosPM25Text, 0, "PM2.5", 1);

    // // draw the vertical separator line
    // myOLED.line(myOLED.getWidth() / 2, 0, myOLED.getWidth() / 2, myOLED.getHeight(), 1);
    // // draw a second line to make it more visible
    // myOLED.line(myOLED.getWidth() / 2 + 1, 0, myOLED.getWidth() / 2 + 1, myOLED.getHeight(), 1);

}

// Write the PM1 and PM2.5 values to the display
void writeValuesToDisplay()
{
    // set the font to the large number font
    myOLED.setFont(&QW_FONT_LARGENUM);

    // draw the PM1 value
    String toPrint = "blank";
    toPrint = String(int(pm1Value));

    // calculate the x position of the PM1 value
    // we want it to be centered in the left half of the screen
    int xPosPM1Value = (myOLED.getWidth() / 4) - (myOLED.getStringWidth(toPrint) / 2);

    // calculate the y start position of the PM1 value
    // we want the bottom of the text to align with the bottom of the screen
    int yPosPM1Value = myOLED.getHeight() - myOLED.getStringHeight(toPrint);
    myOLED.text(xPosPM1Value, yPosPM1Value, toPrint, 1);

    // draw the PM2.5 value
    // calculate the x position of the PM2.5 value
    // we want it to be centered in the right half of the screen
    int xPosPM25Value = (myOLED.getWidth() / 4) * 3 - (myOLED.getStringWidth(toPrint) / 2);
    toPrint = String(int(pm25Value));
    myOLED.text(xPosPM25Value, yPosPM1Value, toPrint, 1); // same y position as the PM1 value
}

// Write a string to the display that is centered horizontally and vertically
void writeCenteredStringToDisplay(String toPrint, bool clearScreen)
{
    // clear the screen
    if(clearScreen)
    {
        myOLED.erase();
    }

    // set the font to the 8x16 font
    myOLED.setFont(&QW_FONT_5X7);

    // calculate the x position of the toPrint text
    // we want it to be centered in the screen horizontally
    // and vertically
    int xPosToPrint = (myOLED.getWidth() / 2) - (myOLED.getStringWidth(toPrint) / 2);
    int yPosToPrint = (myOLED.getHeight() / 2) - (myOLED.getStringHeight(toPrint) / 2);

    // draw the string as text
    myOLED.text(xPosToPrint, yPosToPrint, toPrint, 1);
    myOLED.display();
}

// blink the status LED
void blinkStatLed()
{
    digitalWrite(StatLedPin, HIGH);
    delay(10);
    digitalWrite(StatLedPin, LOW);
}

// toggle "heartbeat" rectangle in the upper right corner of the screen
void toggleHeartbeat()
{
    static bool bHeartbeat = false;

    // heartbeat rectangle is 3x3 pixels
    uint8_t rectWidth = 3;
    uint8_t rectHeight = 3;

    // heartbeat rectangle is in the upper right corner of the screen
    uint8_t rectStartX = myOLED.getWidth() - rectWidth;
    uint8_t rectStartY = 0;

    // draw the rectangle
    myOLED.rectangleFill(rectStartX, rectStartY, rectWidth, rectHeight, 1);

    // toggle the heartbeat
    bHeartbeat = !bHeartbeat;

    // if the heartbeat is off, erase the rectangle
    if (!bHeartbeat)
    {
        myOLED.rectangleFill(rectStartX, rectStartY, rectWidth, rectHeight, 0);
    }

    myOLED.display();
}

// Write the battery level to the display
// Create a rectangular battery level indicator on the bottom right side of the screen
// fill in the rectangle with a percentage of the battery level
// each section of the rectangle represents 25% of the battery level
void writeBatteryLevelToDisplay()
{
    // calculate the x position of the battery level indicator
    // this is the right side of the screen minus 14 pixels
    int xPosBatteryLevelIndicator = (myOLED.getWidth()- 14);

    // draw the battery level indicator outline
    // this rectangle is an outline of the battery level indicator
    // it looks like a small battery
    // it is 6 pixels tall and 12 pixels wide
    // the inside of the rectangle is not filled in
    // the rectangle is drawn in white
    // the inside of the rectangle will be filled in with the battery level
    myOLED.rectangle(xPosBatteryLevelIndicator, myOLED.getHeight() - 6, 12, 6, 1);

    // draw the end shape of the battery level indicator
    // this makes it look like a traditional battery level indicator
    // like the end of a AA battery
    // draw a filled in rectangle that will be the end shape of the battery level indicator
    // it is 2 pixels tall and 2 pixels wide
    // it is drawn in white
    myOLED.rectangle(xPosBatteryLevelIndicator + 12, myOLED.getHeight() - 4, 2, 2, 1);

    // draw the inner sections of the battery level indicator
    // There are 3 blocks inside the battery level indicator
    // each block represents 33% of the battery level
    // the left most block represents 0-33% of the battery level
    // the middle block represents 34-66% of the battery level
    // the right most block represents 67-100% of the battery level
    // the blocks are filled in with white
    // the blocks are 2 pixels tall and 2 pixels wide
    // there is a 1 pixel gap between each block

    // calculate the y position of the battery level indicator blocks
    // this is the bottom of the screen minus 4 pixels
    int yPosBatteryLevelBlocks = myOLED.getHeight() - 4;

    // calculate the x position of the left most block
    // this is the right side of the screen minus 12 pixels
    int xPosLeftBlock = (myOLED.getWidth() - 12);

    // calculate the x position of the middle block
    // this is the right side of the screen minus 9 pixels
    int xPosMiddleBlock = (myOLED.getWidth() - 9);

    // calculate the x position of the right most block
    // this is the right side of the screen minus 6 pixels
    int xPosRightBlock = (myOLED.getWidth() - 6);

    // write all 3 battery block indicators as black rectangles
    myOLED.rectangle(xPosLeftBlock, yPosBatteryLevelBlocks, 2, 2, 0);
    myOLED.rectangle(xPosMiddleBlock, yPosBatteryLevelBlocks, 2, 2, 0);
    myOLED.rectangle(xPosRightBlock, yPosBatteryLevelBlocks, 2, 2, 0);

    // if the battery level is at least 33%
    // fill in the left most block
    if (soc > 32)
    {
        myOLED.rectangle(xPosLeftBlock, yPosBatteryLevelBlocks, 2, 2, 1);
    }

    // if the battery level is between 34% and 66%
    // fill in the middle block
    if (soc > 33)
    {
        myOLED.rectangle(xPosMiddleBlock, yPosBatteryLevelBlocks, 2, 2, 1);
    }

    // if the battery level is greater than 66%
    // fill in the right most block
    if (soc > 66)
    {
        myOLED.rectangle(xPosRightBlock, yPosBatteryLevelBlocks, 2, 2, 1);
    }
}

void writeObstructedBoarder()
{
    // set fort to 5x7
    myOLED.setFont(&QW_FONT_5X7);

    int xPosObstructed = (myOLED.getWidth() / 2) - (myOLED.getStringWidth("Obstructed") / 2) - 2;
    int yPosObstructed = (myOLED.getHeight() / 2) - (myOLED.getStringHeight("Obstructed") / 2) - 2;
    int widthObstructed = myOLED.getStringWidth("Obstructed") + 4;
    int heightObstructed = myOLED.getStringHeight("Obstructed") + 4;

    // draw the black filled rectangle
    myOLED.rectangleFill(xPosObstructed, yPosObstructed, widthObstructed, heightObstructed, 0);
}