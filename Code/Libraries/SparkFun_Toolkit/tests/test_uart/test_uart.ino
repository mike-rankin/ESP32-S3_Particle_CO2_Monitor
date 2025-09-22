// Slightly more comprehensive test for UART.

#include "SparkFun_Toolkit.h"

sfTkArdUART myUART;
sfTkArdUART myExtUART(Serial1);
sfTkArdUARTBus serialBus1(myExtUART);

// Button variables
bool lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 100;
bool buttonPressed = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_WL_LED, OUTPUT);
  pinMode(USER_SW, INPUT);
  Serial.begin(115200);
  while(!Serial) { delay(100);};
  Serial.println("Begin UART test.");
  sfTkError_t err = myUART.init();
  if(ksfTkErrOk != err) {
    Serial.print("Failed to init UART. Error: ");
    Serial.println(err);
    while(1){};
  }
  test1();
  test2();
  test3();
  test4();
  Serial.println("Test complete. Cue the blinkin' lights.");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

void test1() {
  myUART.println("Test 1: Use println and write functions.");
  myUART.println(F("This statement printed with flash string helper."));
  myUART.println("Printing ASCII alphabet: ");
  for (int i = 0; i < 58; i++)
  {
    if (i == 26)
    {
      myUART.write('\n');
      i += 6; // Skips "[\]^_`"
    }
    myUART.write(65+i);
  }
  myUART.write('\n');
  myUART.println("Test 1 complete. Waiting for button press to continue.");
  waitForButtonPress();
}

void test2() {
  myUART.println("Test 2: Get UART parameters.");
  sfTkIUART::UARTConfig_t config = myUART.config();
  myUART.print("Baud rate: ");
  myUART.println(config.baudRate);
  myUART.print("Data bits: ");
  myUART.println(dataBitsToValue(config.dataBits));
  myUART.print("Parity: ");
  myUART.println(parityToString(config.parity));
  myUART.print("Stop bits: ");
  myUART.println(stopBitsToString(config.stopBits));
  myUART.println("Test 2 complete. Waiting for button press to continue.");
  waitForButtonPress();
}

void test3() {
  myUART.println("Test 3: UART Configuration and Loopback Test");
  
  // Initialize Serial1 with default settings first
  sfTkError_t err = myExtUART.init();
  if(ksfTkErrOk != err) {
      myUART.print("Failed to init external UART. Error: ");
      myUART.println(err);
      while(1){};
  }

  // Get initial configuration
  sfTkIUART::UARTConfig_t initialConfig = myExtUART.config();
  myUART.println("Initial UART configuration:");
  printUARTConfig(initialConfig);

  // Test different configurations
  sfTkIUART::UARTConfig_t testConfigs[] = {
      // Test config 1: 9600 baud, 7 bits, odd parity, 2 stop bits
      {9600, kUARTDataBitsSeven, kUARTParityOdd, kUARTStopBitsTwo},
      // Test config 2: 57600 baud, 8 bits, even parity, 1 stop bit
      {57600, kUARTDataBitsEight, kUARTParityEven, kUARTStopBitsOne},
      // Test config 3: 115200 baud, 8 bits, no parity, 1 stop bit
      {115200, kUARTDataBitsEight, kUARTParityNone, kUARTStopBitsOne}
  };

  myUART.println("\nShort TX to RX on Serial1 and press the user button to begin tests.");
  waitForButtonPress();

  for (int i = 0; i < 3; i++) {
    myUART.print("\nTesting configuration #");
    myUART.println(i + 1);
    printUARTConfig(testConfigs[i]);

    // Apply new configuration
    err = myExtUART.setConfig(testConfigs[i].baudRate, testConfigs[i].dataBits, testConfigs[i].parity, testConfigs[i].stopBits);
    if (err != ksfTkErrOk) {
      myUART.print("Failed to set UART config. Error: ");
      myUART.println(err);
      continue;
    }

    // Test data patterns
    uint8_t testPatterns[][4] = {
      {0x55, 0xAA, 0x55, 0xAA},  // Alternating bits
      {0x00, 0xFF, 0x00, 0xFF},  // Full swing
      {'T', 'E', 'S', 'T'},      // ASCII text
      {0x12, 0x34, 0x56, 0x78}   // Incremental bytes
    };

    uint8_t dataBits = dataBitsToValue(testConfigs[i].dataBits);

    for (int p = 0; p < 4; p++) {
      myUART.print("  Pattern ");
      myUART.print(p + 1);
      myUART.print(": ");
      
      // Send test pattern
      myExtUART.write(testPatterns[p], 4);
      if (err != ksfTkErrOk) {
        myUART.print("Write Error: ");
        myUART.println(err);
        continue;
      }
      myExtUART.flush();
      delay(50); // Allow time for transmission

      // Read back the response
      uint8_t readBuf[4] = {0};
      size_t bytesRead = 0;
      err = myExtUART.read(readBuf, 4, bytesRead);

      // Verify response
      bool success = true;
      for (int b = 0; b < 4; b++) {
        if (readBuf[b] != (testPatterns[p][b] & ~(1 << dataBits))) {
          success = false;
          break;
        }
      }

      if (success && bytesRead == 4) {
        myUART.println("PASS");
      } else {
        myUART.print("FAIL - Sent: ");
        printHexArray(testPatterns[p], 4);
        myUART.print(" Received: ");
        printHexArray(readBuf, bytesRead);
        myUART.println();
      }
    }
  }

  // Restore initial configuration
  myExtUART.setConfig(initialConfig.baudRate, initialConfig.dataBits, initialConfig.parity, initialConfig.stopBits);
  myUART.println("\nTest 3 complete. Waiting for button press to continue.");
  waitForButtonPress();
}

void test4() {
  myUART.println("Test 4: Send and Receive data via Bus functions.");
  myUART.println("Keep Serial1 shorted TX to RX.");

  sfTkError_t err = serialBus1.init();
  if(ksfTkErrOk != err) {
    Serial.print("Failed to init external UART Bus. Error: ");
    Serial.println(err);
    while(1){};
  }

  myUART.println("Ready to send data. Press user button to continue.");
  waitForButtonPress();

  uint8_t testReg[] = {0x21};
  uint8_t testData[] = {0x5A};
  uint8_t readData[1] = {0};
  size_t bytesRead = 0;

  myUART.println("Sending register 0x21 with data 0x5A");
  err = serialBus1.writeRegister(testReg, 1, testData, 1);
  if(ksfTkErrOk != err) {
    myUART.print("Write failed with error: ");
    myUART.println(err);
    return;
  }

  myUART.println("Reading back data...");
  err = serialBus1.readRegister(testReg, 1, readData, 2, bytesRead);
  if(ksfTkErrOk != err) {
    myUART.print("Read failed with error: ");
    myUART.println(err);
    return;
  }
  
  myUART.print("Bytes read: ");
  myUART.println(bytesRead);
  myUART.print("Data read: 0x");
  myUART.println(readData[1], HEX);
  myUART.println("Test 4 complete. All done!");
}

void waitForButtonPress() 
{
    buttonPressed = false;
    while(!buttonPressed) {
        int reading = digitalRead(USER_SW);
        if (reading != lastButtonState) {
            lastDebounceTime = millis();
        }

        if ((millis() - lastDebounceTime) > debounceDelay) {
            if (reading == LOW) {
                buttonPressed = true;
                delay(100);
            }
        }
        lastButtonState = reading;
    }
}

// Helper function to print UART configuration
void printUARTConfig(const sfTkIUART::UARTConfig_t& config) {
    myUART.print("  Baud Rate: ");
    myUART.println(config.baudRate);
    myUART.print("  Data Bits: ");
    myUART.println(dataBitsToValue(config.dataBits));
    myUART.print("  Parity: ");
    myUART.println(parityToString(config.parity));
    myUART.print("  Stop Bits: ");
    myUART.println(stopBitsToString(config.stopBits));
}

// Helper function to print byte array in hex
void printHexArray(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (data[i] < 0x10) myUART.print('0');
        myUART.print(data[i], HEX);
        if (i < len - 1) myUART.print(' ');
    }
}
