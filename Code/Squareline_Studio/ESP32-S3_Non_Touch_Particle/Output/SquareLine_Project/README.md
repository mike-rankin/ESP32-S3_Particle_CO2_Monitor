# Arduino project for SquareLine Studio with TFT_eSPI

## Get started

1. Open the `.ino` file in Arduino
2. Go to File/Preferences and set Sketchbook location to the path of your UI project (where this README is located)
3. Go to `libraries/TFT_eSPI` and open `User_Setup.h` or `User_Setup_Select.h` with a text editor to configure pins for your display.
4. Select your board (install if needed)
5. Build the project

Note that this project doesn't contain touch pad driver. You can use an other Arudino library for your touchpad controller.

This is a comparison-table between main driver libraries according to what devices they support (wath in monospace-fontset):
(This list is about half a year old, created first for Raspberry Pi Pico board-template, supported devices might have changed meanwhile.)

LCD:
====

| Model    |lvgl_esp32_drivers|  TFT_eSPI   |    lv_drivers   |   Arduino-GFX       |
|------    |----------------- | ----------- | --------------- | ------------------- |
| (drm         |              |             |        *)       |                     |
| (fbdev       |              |             |        *)       |                     |
| (RGB-Display |              |             |                 |     *)              |
| FT81x        |  *           |             |                 |                     |
| GC9106       |              |             |                 |     *               |
| GC9107       |              |             |                 |     *               |
| GC9A01       |  *           |     *       |        *        |     *               |
| HX8347(C,D)  |              |             |                 |     *               |
| HX8352(C)    |              |             |                 |     *               |
| hx8357       |  *           |             |                 |     *(A)            |
| hx8357b/c/d  |  ?           |     *       |                 |     *(B)            |
| HX8369A      |              |             |                 |     *               |
| il3820       |  *           |             |                 |                     |
| ili9163c     |  *           |     *(-c?)  |                 |                     |
| ili9225      |              |     *       |                 |     *               |
| ILI9331      |              |             |                 |     *               |
| ili9341      |  *           |     *       |       *         |     *               |
| ILI9342      |              |             |                 |     *               |
| ili9481      |  *           |     *       |                 |     *(18bit)        |
| ili9486      |  *           |     *       |                 |     *(normal,18bit) |
| ili9488      |  *           |     *       |                 |     *(normal,18bit) |
| ILI9806      |              |             |                 |     *               |
| JBT6K71      |              |             |                 |     *               |
| jd79653a     |  *           |             |                 |                     |
| NT35310      |              |             |                 |     *               |
| NT35510      |              |             |                 |     *               |
| NT39125      |              |             |                 |     *               |
| NV3041A      |              |             |                 |     *               |
| OTM8009A     |              |             |                 |     *               |
| pcd8544      |  *           |             |                 |                     |
| R61529       |              |             |                 |     *               |
| R61581       |              |     *       |       *         |                     |
| ra8875       |  *           |             |                 |                     |
| RM67162      |              |             |                 |     *               |
| RM68120      |              |     *       |                 |                     |
| RM68140      |              |     *       |                 |                     |
| S6D02A1      |              |     *       |                 |                     |
| SEPS525      |              |             |                 |     *               |
| sh1107       |  *           |             |                 |                     |
| SHARP_MIP    |              |             |       *         |                     |
| SSD1283A     |              |             |                 |     *               |
| ssd1306      |  *           |             |                 |                     |
| SSD1331      |              |             |                 |     *               |
| SSD1351      |              |     *       |                 |     *               |
| SSD1963      |              |     *       |       *         |                     |
| ST7565       |              |             |       *         |                     |
| st7735s      |  *           |     *(-s?)  |                 |     *(-s?)          |
| st7789       |  *           |     *(_2?)  |                 |     *               |
| st7796s      |  *           |     *(-s?)  |                 |     *               |
| UC1610       |              |             |       *         |                     |
| uc8151d      |  *           |             |                 |                     |

TOUCH:
======

| Model        |lvgl_esp32_drivers|  TFT_eSPI   |    lv_drivers  |
| -----        |------------------| ----------- | -------------- |
| adcraw/AD_touch |      *        |             |        *       |
| ft6x6           |      *        |             |                |
| FT6x36?         |               |             |                |
| FT5406EE8       |               |             |        *       |
| FT81x           |      *        |             |                |
| gt911           |      *        |             |                |
| ra8875          |      *        |             |                |
| stmpe610        |      *        |             |                |
| xpt2046         |      *        |      *      |        *       |
| (evdev          |               |             |        *)      |
| (libinput       |               |             |        *)      |
| (xkb            |               |             |        *)      |

