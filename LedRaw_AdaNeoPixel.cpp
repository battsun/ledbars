#include "LedRaw_AdaNeoPixel.h"
#include <Adafruit_NeoPixel.h>
#include <StandardCplusplus.h>
#include <vector>
using namespace std;

//constructor
LedRaw_AdaNeoPixel::LedRaw_AdaNeoPixel(void){
  LedRaw_AdaNeoPixel(6,1,10,SINGLE);
}

LedRaw_AdaNeoPixel::LedRaw_AdaNeoPixel(int pinNum, uint8_t lane, uint8_t row, LedBarType mode){
  mLane=lane;
  mRow=row;
  mBarType=mode;
  mLeds=mLane*mRow*mBarType;

  mStrip=Adafruit_NeoPixel(mLeds, pinNum, NEO_GRB + NEO_KHZ800);
  mLedMatrix =vector<vector<uint8_t>>(mLane, vector<uint8_t>(mRow, 0));
  mLedMatrixP=vector<vector<uint8_t>>(mLane, vector<uint8_t>(mRow, 0));
  mPatternNum=0;

  for(uint8_t i=0;i<mLane;i++){
    for(uint8_t j=0;j<mRow;j++){
      if(mBarType==SINGLE){
        mLedMatrix[i][j]=i*mRow+j;
      } else{
         mLedMatrix[i][j]=2*i*mRow+j;
        mLedMatrixP[i][j]=2*(i+1)*mRow-j-1;
      }
    }
  }

  //プリセット色の初期化
  red  =ColorUnionInit(255,0,0);
  green=ColorUnionInit(0,255,0);
  white=ColorUnionInit(128,128,128);
  blue =ColorUnionInit(0,0,255);
}

//destructor
LedRaw_AdaNeoPixel::~LedRaw_AdaNeoPixel(){
}

void LedRaw_AdaNeoPixel::begin(void){
  mStrip.begin();
}

void LedRaw_AdaNeoPixel::setBrightness(uint8_t br){  
  mStrip.setBrightness(br);
}

//NeoPixel.SHOW()
void LedRaw_AdaNeoPixel::show(void){
  mStrip.show();
}

//set one led color
void LedRaw_AdaNeoPixel::setColor(uint8_t lane, uint8_t row,int g, int r, int b){
  switch(mBarType){
    case SINGLE:
      mStrip.setPixelColor( mLedMatrix[lane][row],g,r,b);
    break;
    case PAIR:
      mStrip.setPixelColor( mLedMatrix[lane][row],g,r,b);
      mStrip.setPixelColor(mLedMatrixP[lane][row],g,r,b);
    break;
    default:
    break;
  }
}

//set one led white level 
void LedRaw_AdaNeoPixel::setColorW(uint8_t lane, uint8_t row,int br){
  setColor(lane, row,br,br,br);
}

//create color
col LedRaw_AdaNeoPixel::ColorUnionInit(uint8_t r,uint8_t g,uint8_t b){
  col col_;
  col_.zero=0;col_.r=r;col_.g=g;col_.b=b;
  return col_;
}

////////////////////////////////////////////////////////////////////////////////  
///////////////   pattern    /////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 

//////////////////////////////////////// 
//SnowFall
void LedRaw_AdaNeoPixel::patternSnowFallInit(void){
uint8_t TAILTABLE[40]={255,192,168,144,121,96,64,32,16,8,4,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int tailLen=40;
mPatternNum++;

  for(uint8_t i=0;i<tailLen;i++){
    mSnowFallTailAry[i]=TAILTABLE[i];
  }
  mSnowFallTailLen=tailLen;

  mSnowFallTopIndex=vector<uint8_t>(mLane);
  for(int i=0;i<mLane;i++){
    mSnowFallTopIndex[i]=random( mSnowFallTailLen);
  }
}

void LedRaw_AdaNeoPixel::patternSnowFall(void){
   // forward the first led's status one step on each Bar
  for(int i=0;i<mLane;i++){
    mSnowFallTopIndex[i]=(mSnowFallTopIndex[i]+1) % mSnowFallTailLen;
  }

  //Change all leds color
  for(uint8_t i=0;i<mLane;i++){
    for(uint8_t j=0;j<mRow;j++){
      uint8_t br=mSnowFallTailAry[(mSnowFallTopIndex[i]+mRow-j) % mSnowFallTailLen];
      setColorW(i,j,br);
    }
  }
}

/////////////////////////////////////
//二色の間の点滅パターン
void LedRaw_AdaNeoPixel::patternBlink2ColorInit(uint16_t step,col col1,col col2){
  mBlink2ColorStep=step;
  mBlink2ColorCol1=col1;
  mBlink2ColorCol2=col2;
  mPatternNum++;

}

void LedRaw_AdaNeoPixel::patternBlink2Color(void){
  int r=(millis()-mReferenceTime) % (mBlink2ColorStep *4);
  int c=constrain(abs(r-5 * mBlink2ColorStep / 2) - mBlink2ColorStep/2, 0, mBlink2ColorStep);

  //全LEDについて書き換え
  for(uint8_t i=0;i<mLane;i++){
    for(uint8_t j=0;j<mRow;j++){
      if((i+j)%2==0){
        setColor(i,j,map(c,0,mBlink2ColorStep,mBlink2ColorCol1.g,mBlink2ColorCol2.g),
                     map(c,0,mBlink2ColorStep,mBlink2ColorCol1.r,mBlink2ColorCol2.r),
                     map(c,0,mBlink2ColorStep,mBlink2ColorCol1.b,mBlink2ColorCol2.b));
      } else{
        setColor(i,j,map(c,0,mBlink2ColorStep,mBlink2ColorCol2.g,mBlink2ColorCol1.g),
                     map(c,0,mBlink2ColorStep,mBlink2ColorCol2.r,mBlink2ColorCol1.r),
                     map(c,0,mBlink2ColorStep,mBlink2ColorCol2.b,mBlink2ColorCol1.b));
      }
    }
  }
}

/////////////////////////////////////
//氷と雨くるくるパターン
void LedRaw_AdaNeoPixel::patternFalling2ColorInit(uint16_t period, col col1, col col2){
  mFalling2ColorPeriod=period;
  mFalling2ColorCol1=col1;
  mFalling2ColorCol2=col2;
  mPatternNum++;
}

void LedRaw_AdaNeoPixel::patternFalling2Color(void){
  //周期は4LEDなので、
  int c=((millis()-mReferenceTime) % (mFalling2ColorPeriod*2)) / mFalling2ColorPeriod;
  
  //全LEDについて書き換え
  for(uint8_t i=0;i<mLane;i++){
    for(uint8_t j=0;j<mRow;j++){
      switch((i + mRow - j + c) % 4){
        case 0:
        setColor(i,j,mFalling2ColorCol1.g,mFalling2ColorCol1.r,mFalling2ColorCol1.b);
        break;
        case 1:
        setColor(i,j,mFalling2ColorCol2.g,mFalling2ColorCol2.r,mFalling2ColorCol2.b);
        break;
        default:
        break;
      }
    }
  }
}

/////////////////////////////////////
//光がらせんを飛ぶパターン
void LedRaw_AdaNeoPixel::patternFlyingLightSpiralInit(){
  mSpiralVx=1;
  mSpiralVy=0.4;
  mSpiralPeriod=100;
}

void LedRaw_AdaNeoPixel::patternFlyingLightSpiral(){

  //LEDバーを座標系に見立てて光を飛ばす
  float t=(float)millis() / mSpiralPeriod;
  int lightx[mSpiralN];
  int lighty[mSpiralN];
  lightx[0]=(int)(mSpiralVx*t) % mLane;
  lighty[0]=(int)(mSpiralVy*t) % mRow;
  for(int i=1;i<mSpiralN;i++){
    lightx[i]=(lightx[i-1]+1)% mLane;
    lighty[i]=lighty[i-1]+0;
  }

  //全LEDについてクリア
  for(uint8_t ip=0;ip<mLane;ip++){
    for(uint8_t j=0;j<mRow;j++){
      setColorW(ip,j,0);
    }
  }
  //光粒の座標を点灯させる
  for(int i=0;i<mSpiralN;i++){
    setColorW(lightx[i],lighty[i],4+i*24);
  }

}

/////////////////////////////////////
//Star Run
void LedRaw_AdaNeoPixel::patternStarrunInit(uint16_t per=100){
  mStarrunCycleN=15;
  mStarrunPeriod=per;
  mPatternNum++;
}

void LedRaw_AdaNeoPixel::patternStarrun(){
  int c=((millis()-mReferenceTime) % (mStarrunPeriod * mStarrunCycleN)) / mStarrunPeriod;

  //全LEDについてクリア
  for(uint8_t i=0;i<mLane;i++){
    for(uint8_t j=0;j<mRow;j++){
      setColorW(i,j,0);
    }
  }
  setColorW(0,mStarrunAry[c],128);
}

/////////////////////////////////////////////////////////////////////
// Under development
/////////////////////////////////////////////////////////////////////
/* void LedRaw_AdaNeoPixel::addPatternInit(void (*pf)(uint32_t[])){
  patternInitTable->emplace_back((*pf)(uint32_t[]));
}

void LedRaw_AdaNeoPixel::execPatternInit(int i, uint32_t arg[]){
  patternInitTable[i](arg[]);
} 

void LedRaw_AdaNeoPixel::addPattern(voidfunc f){
  patternTable->emplace_back(f);
}

void LedRaw_AdaNeoPixel::execPattern(int i){
  patternTable[i]();
}
*/

