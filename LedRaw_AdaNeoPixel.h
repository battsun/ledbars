#ifndef LEDRAW_ADANEOPIXEL_H
#define LEDRAW_ADANEOPIXEL_H

#include <Adafruit_NeoPixel.h>
#include <StandardCplusplus.h>
#include <vector>
using namespace std;

typedef union {
  struct{
    uint8_t zero;
    uint8_t r;
    uint8_t g;
    uint8_t b;
  } ; //構造体名もタグ名も省略すると、col.tag_.zero　といった表記を縮めて col.zeroとできる　？
  uint32_t rgb;
} col;

typedef void (*voidfunc)();

enum LedBarType{
  SINGLE=1,
  PAIR=2
};
 

class LedRaw_AdaNeoPixel{
	public:
  LedRaw_AdaNeoPixel(void);
  LedRaw_AdaNeoPixel(int pinNum, uint8_t lane, uint8_t row, LedBarType mode);
  ~LedRaw_AdaNeoPixel(void);
  void show(void);
  void begin();
  void setBrightness(uint8_t br);
  void setReferenceTime(void){mReferenceTime=millis();};
  uint8_t getPatternNum(){return mPatternNum;};
  long getReferenceTime(){return mReferenceTime;};

  //lighting patterns
  void patternSnowFall(void);
  void patternSnowFallInit(void);
  void patternBlink2Color(void);
  void patternBlink2ColorInit(uint16_t step, col col1, col col2);
  void patternFalling2Color(void);
  void patternFalling2ColorInit(uint16_t period, col col1, col col2);
  void patternStarrun(void);
  void patternStarrunInit(uint16_t per);
  void patternFlyingLightSpiral(void);
  void patternFlyingLightSpiralInit(void);

  //uint8_t TAILTABLE[TAILLEN]={255,192,144,96,64,32,8,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  //preset colors
  col red; // static col red;　とするとき、cppの方にグローバル変数として col LedRaw_AdaNeoPixel::red; とかく。ヘッダに書くと二重定義になる
  col green;
  col white;
  col blue;


  protected:             
  
  private:  
  Adafruit_NeoPixel mStrip;
  uint8_t mLane;
  uint8_t mRow;
  LedBarType mBarType;
  uint8_t mLeds; 	//LEDの総数
  vector<vector<uint8_t>> mLedMatrix;
  vector<vector<uint8_t>> mLedMatrixP;

  uint8_t mPatternNum;
  long mReferenceTime;
  void setColor (uint8_t lane, uint8_t row,int g, int r, int b);
  void setColorW(uint8_t lane, uint8_t row,int br);
  col ColorUnionInit(uint8_t r,uint8_t g,uint8_t b);

  //void (*patternTable)()[10];
  /* vector<void> (*patternInitTable)(uint32_t[]);
  void addPatternInit(void (*pf)(uint32_t[]));
  void execPatternInit(int);
  vector<voidfunc> patternTable;
  void addPattern(voidfunc);
  void execPattern(int);
 */  

  //パターン１　スノーフォール
  uint8_t* mSnowFallTailAry;
  uint8_t mSnowFallTailLen;
  vector<uint8_t> mSnowFallTopIndex;

  //パターン2 ２色フェーディング
  col mBlink2ColorCol1;
  col mBlink2ColorCol2;
  long mBlink2ColorStep;

  //パターン3 ２色が落ちる
  col mFalling2ColorCol1;
  col mFalling2ColorCol2;
  int mFalling2ColorPeriod;

  //パターン4 starrun
  uint8_t mStarrunCycleN;
  vector<uint8_t> mStarrunAry = {0,1,3,4,5,7,8,9,1,2,3,5,6,7,9};
  uint16_t mStarrunPeriod;

  //パターン5 spiralLight
  float mSpiralVx;
  float mSpiralVy;
  uint8_t mSpiralPeriod;
  const uint8_t mSpiralN=4;//　光の粒の数

};

#endif
