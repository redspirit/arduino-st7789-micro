#define TFT_DC    9 // DC pin
#define TFT_RST   8 // RES pin
#include <SPI.h>

#define ST7789_SWRESET 0x01
#define ST7789_SLPIN   0x10  // sleep on
#define ST7789_SLPOUT  0x11  // sleep off
#define ST7789_PTLON   0x12  // partial on
#define ST7789_NORON   0x13  // partial off
#define ST7789_INVOFF  0x20  // invert off
#define ST7789_INVON   0x21  // invert on
#define ST7789_DISPOFF 0x28  // display off
#define ST7789_DISPON  0x29  // display on
#define ST7789_IDMOFF  0x38  // idle off
#define ST7789_IDMON   0x39  // idle on

#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

#define ST7789_POWSAVE    0xbc
#define ST7789_DLPOFFSAVE 0xbd

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0

uint8_t _width = 240;
uint8_t _height = 240;

#define DC_DATA     digitalWrite(TFT_DC, HIGH)
#define DC_COMMAND  digitalWrite(TFT_DC, LOW)

void writeSPI(uint8_t c) {
  SPI.transfer(c);
}

void writeMulti(uint16_t color, uint16_t num) {
  while(num--) { 
    SPI.transfer(color>>8);  
    SPI.transfer(color); 
  }
} 

void writeCmd(uint8_t c) {
  DC_COMMAND;
  writeSPI(c);
}

void writeData(uint8_t d8) {
  DC_DATA;
  writeSPI(d8);
}

void writeData16(uint16_t d16) {
  DC_DATA;
  writeMulti(d16, 1);
}

void fillScreen(uint16_t color) {
  fillRect(0, 0, _width, _height, color);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  if(x+w-1>=_width)  w=_width -x;
  if(y+h-1>=_height) h=_height-y;
  setAddrWindow(x, y, x+w-1, y+h-1);

  writeMulti(color,w*h);
}

void drawPixel(int16_t x, int16_t y, uint16_t color) {
  setAddrWindow(x,y,x+1,y+1);

  writeSPI(color>>8); writeSPI(color);
}

void setAddrWindow(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye){
  writeCmd(ST7789_CASET);
  writeData16(xs);
  writeData16(xe);

  writeCmd(ST7789_RASET);
  writeData16(ys);
  writeData16(ye);
  
  writeCmd(ST7789_RAMWR);

  DC_DATA;
}

void displayInit() {
  writeCmd(ST7789_SWRESET);
  delay(150);
  writeCmd(ST7789_SLPOUT);
  delay(500);
  writeCmd(ST7789_COLMOD); writeData(0x55);
  delay(10);
  writeCmd(ST7789_MADCTL); writeData(0);
  writeCmd(ST7789_CASET); writeData16(0); writeData16(240);
  writeCmd(ST7789_RASET); writeData16(0); writeData16(240);
  writeCmd(ST7789_INVON);
  delay(10);
  writeCmd(ST7789_NORON);
  delay(10);
  writeCmd(ST7789_DISPON);
  delay(20);
}

void wiresInit() {
  pinMode(TFT_DC, OUTPUT);

  SPI.begin();
  SPI.setDataMode(SPI_MODE3); // use SPI mode 3

  // reset display (optional)
  pinMode(TFT_RST, OUTPUT);
  digitalWrite(TFT_RST, HIGH);
  delay(50);
  digitalWrite(TFT_RST, LOW);
  delay(50);
  digitalWrite(TFT_RST, HIGH);
  delay(50);
}

void setup(void) {
  wiresInit();
  displayInit();
  fillScreen(0);  // clear screen
  //drawPixel(120, 120, RED);
  //drawPixel(130, 120, BLUE);
}

void loop(){
  // display colored squares in a random place on the screen
  fillRect(random(230), random(230), 10, 10, random(65000));
}
