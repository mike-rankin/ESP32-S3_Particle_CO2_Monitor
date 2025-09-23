# ESP32-S3_Particle_CO2_Monitor
ESP32-S3 based Air Quality Particle and CO2 Monitor

This project is another plug-in USB-C wall charger type that I find clean, cable-free, and convenient for my home. It uses an ultra-small BMV080 particle sensor that allows for a much smaller project than ever before.

![Image](https://github.com/user-attachments/assets/068908f2-491f-460d-baaa-7e56ccb6598f)

Previous designs used the PMSA003, which used a small fan to draw air in and a laser to detect and count particles via scattering. The new Bosch BMV080 is the world's smallest particulate matter sensor that is fanless and measures PM2.5, providing mass concentration data for PM1.0 and PM10.

![Image](https://github.com/user-attachments/assets/e40246da-fd77-4ef6-bbfb-84fd3a0fd16f)

The ESP32-S3 mini is slightly larger than the ESP32-C6 mini but was needed to compile the BMV080 code error-free. Right now, the example Arduino sketches are not compatible with the C6. Below, I've used as few parts as possible to keep the design simple. The 3.3V LDO powers everything, a Qwiic connector is available for adding other sensors, the transistor allows you to adjust the backlight brightness, and the NeoPixel is for rear glow.
![Image](https://github.com/user-attachments/assets/f80bc7f7-3e0d-45f6-b091-c78f25c5b918)

The board protrusion at the top allows all three sensors to sit in a row just above the display, facing forward. Because the BMV080 sensor is so thin but long, it easily sits under the glass in a connector. The STCC4 CO2 sensor sits between the BMV080 and the SHT40 temperature/humidity sensor.
![Image](https://github.com/user-attachments/assets/7f08396c-15c4-4fbb-88d5-c476f8c2b8bd)
![Image](https://github.com/user-attachments/assets/a10d1534-b7d7-421e-adf2-e8bb9022f8b4)

Pricing for components (mostly from Digi-key):
*-1.65" Non-touch was purchased from BuyDisplay: https://www.youtube.com/watch?v=o0nff87zbJU $?.??

*-ESP32-S3-MINI-1-N8 $5.28
*<br/>-STCC4-D-R3 CO2 Sensor $8.27
*<br/>-SHT40-AD1B-R3 Temp/Humidity $1.80
*-BMV080 Particle Sensor $40.90

This design is work in progress so a future revision might include:
*-adjust the board outline to better match the shape of the glass
*-possibly use a touchscreen display
*-pull up the BMV080 sensor to be more inline with the other two
*-source a more rugged verticle usb-c connector

The graphics for this project were done in LVGL using Squareline Studio and the Arduino IDE. Altium Designer was used to create the board and so far only one prototype has been created. I create custom ESP32 projects as a side hustle so feel free to reach out to me @mikerankin on X or 0miker0@gmail.com

