// LEDバークラスを利用する
//255 white 10mA/led  red  3 green  3  blue  3 
//128 white  5mA/led  red  1 green  1  blue  1 
//64  white  2mA/led  red <1 green <1 blue <1
#include "LedRaw_AdaNeoPixel.h"

#define PIN_NEOPIXEL 6
#define LANE 1
#define ROW 10 
#define DELAY 100 
#define PERIOD 10000 //１パターンのループ時間
#define BLINK2STEP 2000 //パターンのパラメータ
#define FALLINGPERIOD 1000 //パターンのパラメータ

LedRaw_AdaNeoPixel strip(PIN_NEOPIXEL,LANE,ROW,SINGLE);

static int period;
static int patN;
static long refTime;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  Serial.println();

  strip.begin();
  strip.setBrightness(64);
  
  strip.patternSnowFallInit();
  strip.patternBlink2ColorInit(BLINK2STEP,strip.red,strip.green);
  strip.patternFalling2ColorInit(FALLINGPERIOD,strip.white,strip.blue);
  strip.patternFlyingLightSpiralInit();
  strip.setReferenceTime();

  period=PERIOD;
  patN=strip.getPatternNum();
  refTime=strip.getReferenceTime();
  strip.show();
}

void loop() {
  
  //パターンの切り替え時間
  long selPattern=((millis()-refTime) % (long)(period*patN)) / period;

  //一定時間で切り替え
  switch(selPattern){
    case 0:
    strip.patternSnowFall();
    break;
    case 1:
    strip.patternBlink2Color();
    break;
    case 2:
    strip.patternFalling2Color();
    break;
    case 3:
    break;
    case 4:
    break;
    default:
    break;
  }
  
  delay(DELAY) ; //delay

  strip.show();
}
