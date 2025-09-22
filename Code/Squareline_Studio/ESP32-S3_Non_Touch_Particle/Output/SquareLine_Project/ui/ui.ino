//Arduino IDE: Boards, ESP32 version 2.0.13
//Latest TFT_eSPI

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ui.h>
#include <Wire.h>
#include <SensirionI2cStcc4.h>
#include "SparkFun_BMV080_Arduino_Library.h"
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 1
#define LED_PIN   48

#define TFT_BL 14
#define STCC4_I2C_ADDR_64 0x65
#define BMV080_ADDR 0x57  // SparkFun BMV080 Breakout defaults to 0x57

// Sensor objects
Adafruit_NeoPixel pixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
SensirionI2cStcc4 sensor;
SparkFunBMV080 bmv080; 

// Error handling
static char errorMessage[64];
static int16_t error;
#define NO_ERROR 0

// Sensor data variables
int16_t co2Concentration = 0;
float temperature = 0.0;
float relativeHumidity = 0.0;
uint16_t status = 0;

// Particle sensor data variables
float pm10 = 0.0;
float pm25 = 0.0;
float pm1 = 0.0;

// Timer for sensor readings
unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 30000; // 30 seconds

/*Change to your screen resolution*/
static const uint16_t screenWidth = 300;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); 

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif

// LED helper function
void setLED(uint8_t r, uint8_t g, uint8_t b) {
  pixel.clear();
  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show();
}

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t touchX = 0, touchY = 0;

  bool touched = false; // tft.getTouch( &touchX, &touchY, 600 );

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;

    Serial.print("Data x ");
    Serial.println(touchX);

    Serial.print("Data y ");
    Serial.println(touchY);
  }
}

// Function to update UI with sensor values
void updateSensorDisplay()
{
  // Update CO2 value
  lv_label_set_text_fmt(ui_CO2Value, "%d", co2Concentration);
  
  // Update temperature value
  char tempBuffer[10];
  snprintf(tempBuffer, sizeof(tempBuffer), "%.1f", temperature);
  lv_label_set_text(ui_TempValue, tempBuffer);
  
  // Update humidity value
  char humidityBuffer[10];
  snprintf(humidityBuffer, sizeof(humidityBuffer), "%.1f", relativeHumidity);
  lv_label_set_text(ui_HumidityValue, humidityBuffer);
  
  // Update particle values
  char pm1Buffer[10];
  snprintf(pm1Buffer, sizeof(pm1Buffer), "%.1f", pm1);
  lv_label_set_text(ui_PM1Value, pm1Buffer);
  
  char pm25Buffer[10];
  snprintf(pm25Buffer, sizeof(pm25Buffer), "%.1f", pm25);
  lv_label_set_text(ui_PM25Value, pm25Buffer);
  
  char pm10Buffer[10];
  snprintf(pm10Buffer, sizeof(pm10Buffer), "%.1f", pm10);
  lv_label_set_text(ui_PM10Value, pm10Buffer);
  
  // Update arc indicator (scaled to fit the arc range 15-35)
  // Map CO2 value (400-5000) to arc range (15-35)
  int arcValue = map(co2Concentration, 400, 5000, 15, 35);
  lv_arc_set_value(ui_CO2Arc, arcValue);
}

// Function to read CO2 sensor data
void readCO2SensorData()
{
  // Exit sleep mode to put the sensor into idle mode
  error = sensor.exitSleepMode();
  if (error != NO_ERROR)
  {
    Serial.print("Error trying to execute exitSleepMode(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }

  // Perform a single shot measurement and read the sensor data
  error = sensor.measureSingleShot();
  if (error != NO_ERROR)
  {
    Serial.print("Error trying to execute measureSingleShot(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }

  error = sensor.readMeasurement(co2Concentration, temperature,
                                relativeHumidity, status);
  if (error != NO_ERROR)
  {
    // A failed read can be caused by clock shifting. We advise to retry
    // after a delay of 150ms.
    Serial.print("Error trying to execute readMeasurement() (retry in 150ms): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    delay(150);
    error = sensor.readMeasurement(co2Concentration, temperature,
                                  relativeHumidity, status);
    if (error != NO_ERROR)
    {
      Serial.print("Error trying to execute readMeasurement() after additional delay: ");
      errorToString(error, errorMessage, sizeof errorMessage);
      Serial.println(errorMessage);
      return;
    }
  }

  // Power down the sensor to reduce power consumption.
  error = sensor.enterSleepMode();
  if (error != NO_ERROR)
  {
    Serial.print("Error trying to execute enterSleepMode(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }

  // Print results to serial
  Serial.print("CO2 concentration [ppm] = ");
  Serial.println(co2Concentration);
  Serial.print("Temperature [°C] = ");
  Serial.println(temperature);
  Serial.print("Humidity [RH] = ");
  Serial.println(relativeHumidity);
  Serial.print("Status = ");
  Serial.println(status);
}

// Function to read particle sensor data
void readParticleSensorData()
{
  if(bmv080.readSensor())
  {
    pm10 = bmv080.PM10();
    pm25 = bmv080.PM25();
    pm1 = bmv080.PM1();

    // Print results to serial
    Serial.print("PM10: ");
    Serial.print(pm10);
    Serial.print("\tPM2.5: ");
    Serial.print(pm25);
    Serial.print("\tPM1: ");
    Serial.println(pm1);

    if(bmv080.isObstructed() == true)
    {
      Serial.println("\tObstructed");
    }
  }
  else
  {
    Serial.println("Error reading BMV080 sensor");
  }
}

// Function to read all sensor data
void readAllSensorData()
{
  readCO2SensorData();
  readParticleSensorData();
  
  // Update the display
  updateSensorDisplay();
}

void setup()
{
  Serial.begin(115200); /* prepare for possible serial debug */

  pixel.begin();
  setLED(50, 0, 0); delay(500); setLED(0, 50, 0); delay(500); setLED(0, 0, 50); delay(500);

  analogWrite(TFT_BL, 85); // PWM Backlight

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  tft.begin();          /* TFT init */
  tft.setRotation(1); /* Landscape orientation, flipped */

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  // Initialize I2C
  Wire.begin(8, 9);

  // Initialize CO2 sensor
  sensor.begin(Wire, STCC4_I2C_ADDR_64);

  delay(6);
  
  // Ensure CO2 sensor is in idle state
  error = sensor.exitSleepMode();
  if (error != NO_ERROR)
  {
    Serial.print("Error trying to execute exitSleepMode(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
  }
  
  error = sensor.stopContinuousMeasurement();
  if (error != NO_ERROR)
  {
    Serial.print("Error trying to execute stopContinuousMeasurement(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
  }
  
  // Enter sleep mode initially for CO2 sensor
  error = sensor.enterSleepMode();
  if (error != NO_ERROR)
  {
    Serial.print("Error trying to execute enterSleepMode(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
  }

  // Initialize particle sensor
  Serial.println("Initializing BMV080 Particle Sensor...");
  if (bmv080.begin(BMV080_ADDR, Wire) == false) {
    Serial.println("BMV080 not detected at default I2C address. Check your jumpers and the hookup guide.");
  } else {
    Serial.println("BMV080 found!");
    bmv080.init();

    /* Set the sensor Duty Cycling Period (seconds)*/
    uint16_t duty_cycling_period = 20;
    if(bmv080.setDutyCyclingPeriod(duty_cycling_period) == true)
    {
        Serial.println("BMV080 set to 20 second duty cycle period");
    }
    else
    {
        Serial.println("Error setting BMV080 duty cycle period");
    }

    /* Set the sensor mode to Duty Cycle mode */
    if(bmv080.setMode(SF_BMV080_MODE_DUTY_CYCLE) == true)
    {
        Serial.println("BMV080 set to Duty Cycle mode");
    }
    else
    {
        Serial.println("Error setting BMV080 mode");
    }
  }

  ui_init();

  setLED(0, 20, 0);

  Serial.println("Setup done");

  // Read sensors once immediately
  readAllSensorData();
  lastSensorRead = millis();
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */

  // Check if it's time to read the sensors again
  unsigned long currentTime = millis();
  if (currentTime - lastSensorRead >= sensorInterval)
  {
    readAllSensorData();
    lastSensorRead = currentTime;
  }

  delay(5);
}