#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <DHT.h>


// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     D1
#define TFT_RST    D0 // you can also connect this to the Arduino reset
                       // in which case, set this #define pin to -1!
#define TFT_DC     D2
#define DHTPIN     D4
#define DTHUPDATE  5000         // miliseconds for read DHT sensor
// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)

// For 1.44" and 1.8" TFT with ST7735 use
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
bool invert = false;
// For 1.54" TFT with ST7789
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
//#define TFT_SCLK 13   // set these to be whatever pins you like!
//#define TFT_MOSI 11   // set these to be whatever pins you like!
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

DHT dht;
unsigned long tdelay = millis();

class ProgressBar {
      uint16_t _x;
      uint16_t _y;
      uint16_t _steps;
      uint16_t _current_step;
      uint16_t _h;
      uint16_t _w;
      uint16_t _color;
      bool _horizontal;
    public:
      void setColor(uint16_t color);
      void setSteps(uint16_t steps); //default 100
      uint16_t getStep();
      void drawStep();
      void reset();
      ProgressBar();  //x = 0, y = 0, steps = 100, h = 1, w = end of LCD, color = white, horizontal true
      ProgressBar(uint16_t, uint16_t);
      ProgressBar(uint16_t, uint16_t, uint16_t);
      ProgressBar(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
      ProgressBar(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
      ProgressBar(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, bool);
};

//x = 0, y = 0, steps = 100, h = 1, w = 100, color = white, horizontal true
ProgressBar::ProgressBar() : ProgressBar(0, 0, 100, 1, 100, 0 - 1,true) {}

ProgressBar::ProgressBar(uint16_t x, uint16_t y) : ProgressBar(x, y, 100, 1, 100, 0 - 1,true) {}

ProgressBar::ProgressBar(uint16_t x, uint16_t y, uint16_t steps) :
  ProgressBar(x, y, steps, 1, 100, 0 - 1,true) {}

ProgressBar::ProgressBar(uint16_t x, uint16_t y, uint16_t steps, uint16_t h, uint16_t w) :
  ProgressBar(x, y, steps, h, w, 0 - 1,true) {}

ProgressBar::ProgressBar(uint16_t x, uint16_t y, uint16_t steps, uint16_t h, uint16_t w, uint16_t color) :
  ProgressBar(x, y, steps, h, w, color,true) {}

ProgressBar::ProgressBar(uint16_t x, uint16_t y, uint16_t steps, uint16_t h, uint16_t w, uint16_t color, bool horizontal) {
  _x = x; _y = y; _steps = steps; _h = h; _w = w; _color = color; _horizontal = horizontal;
  _current_step = 0;
}

void ProgressBar::setColor(uint16_t color) {
  _color = color;
}

void ProgressBar::setSteps(uint16_t steps) {
  _steps = steps;
}

uint16_t ProgressBar::getStep() {
  return _current_step;
}

void ProgressBar::drawStep() {
  //tft.println(_w);
  //tft.println(_h);
  //tft.println(_steps);
  //tft.fillRect(_x, _y, _w, _h, ST7735_BLUE);
}

ProgressBar bar1(0,10,100,5,150);





void printSensor() {
  if(tdelay + DTHUPDATE < millis()) {
    // Serial.println(tdelay);
    // Serial.println(millis());
    tdelay = millis();
    // tft.invertDisplay(invert);
    // invert = !invert;
    float h = dht.getHumidity();
    float t = dht.getTemperature();
    // Serial.print("Teplota: ");
    // Serial.println(t);
    // Serial.print("Vlhkost: ");
    // Serial.println(h);
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 30);
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(1);
    tft.print("Teplota: ");
    tft.setTextSize(2);
    tft.print(t);
    tft.println(" C");
    tft.setCursor(0, 60);
    tft.setTextColor(ST77XX_BLUE);
    tft.setTextSize(1);
    tft.print("Vlhkost: ");
    tft.setTextSize(2);
    tft.print(h);
    tft.println(" %");
    //tft.drawRect(0, 0, 50, 10, ST7735_BLUE);
    bar1.drawStep();
  }
}



void setup(void) {
  Serial.begin(115200);
  Serial.print("Hello! Adafruit ST77XX test");

  // Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  dht.setup(DHTPIN);

  // Use this initializer (uncomment) if you're using a 1.44" TFT
  //tft.initR(INITR_144GREENTAB);   // initialize a ST7735S chip, black tab

  // Use this initializer (uncomment) if you're using a 0.96" 180x60 TFT
  //tft.initR(INITR_MINI160x80);   // initialize a ST7735S chip, mini display

  // Use this initializer (uncomment) if you're using a 1.54" 240x240 TFT
  //tft.init(240, 240);   // initialize a ST7789 chip, 240x240 pixels

  Serial.println("init");
  tft.setTextWrap(false); // Allow text to run off right edge
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  //testText();
}

void loop(void) {
  printSensor();
}
