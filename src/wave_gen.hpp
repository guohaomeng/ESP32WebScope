/*******************************************************************************
****文件路径         : \ESP32WebScope\src\wave_gen.hpp
****作者名称         : guohaomeng
****文件版本         : V1.0.0
****创建日期         : 2022-07-01 11:43:37
****简要说明         : 波形发生器类及方法
****
****版权信息         : 2022 by guohaomeng, All Rights Reserved.
********************************************************************************/
#ifndef WAVE_GEN_H_
#define WAVE_GEN_H_
#include <Arduino.h>

#define samplePerCycle 255

enum WAVE_TYPE
{
  SIN,
  SQUARE,
  SAWTOOTH
};
int waveindex = 2;               // 当前波形位置
uint8_t waveTab[samplePerCycle]; // 最终根据配置生成的波形数据
/* 创建硬件定时器 */
hw_timer_t *timer = NULL;
class WAVE_GEN
{
public:
  /* 波形发生器参数配置 */
  double uMaxValue = 3.3;    //峰峰值
  double offSetValue = 1.65; //偏置电压
  int duty = 50;             //占空比%(方波)
  WAVE_TYPE wave_type;       //波形种类
  unsigned int freq = 100;   // 频率

  unsigned int freq_old = 100;      // 上一次的频率
  uint8_t waveTab1[samplePerCycle]; // 生成的波形数据

  /* 波形模式切换按键 */
  const int button = 12; // 波形切换引脚位置
  char received_data[5] = {0};
  unsigned int rec_cnt1 = 0;
  /* 各种函数，波形发生器相关以及定时器相关的 */
  WAVE_GEN(double uMaxValue, double offSetValue, int duty, unsigned int freq, WAVE_TYPE wave_type);
  ~WAVE_GEN();
  int set_uMaxValue(double uMaxValue);
  int set_offSetValue(double offSetValue);
  int set_duty(int duty);
  int set_freq(int freq);
  int set_wave_type(WAVE_TYPE wave_type);

  void initTimer();
  void updateTimer();
  void get_waveindex();
  void waveSelect();
  void waveGen(WAVE_TYPE wave_type);
};

/*******************************************************************************
****函数功能: 波形发生器构造函数
****入口参数: uMaxValue:峰峰值
****入口参数: offSetValue:偏置电压
****入口参数: duty:占空比
****入口参数: freq:频率
****入口参数: wave_type:波形种类
****出口参数: 无
****函数备注: 根据以上参数，构造一个波形发生器
********************************************************************************/
WAVE_GEN::WAVE_GEN(double uMaxValue, double offSetValue, int duty, unsigned int freq, WAVE_TYPE wave_type)
{
  this->uMaxValue = uMaxValue;
  this->offSetValue = offSetValue;
  this->duty = duty;
  this->freq = freq;
  this->wave_type = wave_type;
}

WAVE_GEN::~WAVE_GEN() {}
/*******************************************************************************
****函数功能: 硬件定时器中断函数
****出口参数: 无
****函数备注: 无
********************************************************************************/
void IRAM_ATTR onTimer()
{
  if (waveindex >= samplePerCycle)
  {
    waveindex = 0;
  }
  dacWrite(25, waveTab[waveindex]);
  waveindex++;
}
/*******************************************************************************
****函数功能: 初始化波形发生器
****出口参数: 无
****函数备注: 将25号引脚作为输出，生成默认波形，并配置好定时器
********************************************************************************/
void WAVE_GEN::initTimer()
{
  /* 输出端口 */
  pinMode(25, OUTPUT);
  /* 默认输出正弦波 */
  waveGen(SIN);

  /*  1/(80MHZ/80) = 1us  */
  timer = timerBegin(0, 80, true);

  /* 将onTimer函数附加到我们的计时器 */
  timerAttachInterrupt(timer, &onTimer, true);

  /* *设置闹钟每秒调用onTimer函数1 tick为1us   => 1秒为1000000us * /
  / *重复闹钟（第三个参数）*/
  uint64_t T =  1000000 / (freq * 255);
  timerAlarmWrite(timer, T, true);
  
  /* 启动定时器 */
  timerAlarmEnable(timer);
  Serial.println("定时器启动成功");
}
/*******************************************************************************
****函数功能: 更新定时器
****出口参数: 无
****函数备注: 在波形发生器频率发生改变后，重设定时器
********************************************************************************/
void WAVE_GEN::updateTimer()
{
  timerAlarmDisable(timer);                  //先关闭定时器
  uint64_t dacTime = 1000000 / (freq * 255); //波形周期,微秒
  /* *设置闹钟每秒调用onTimer函数1 tick为1us => 1秒为1000000us * /
  / *重复闹钟（第三个参数）*/
  timerAlarmWrite(timer, dacTime, true);
  /* 启动警报*/
  timerAlarmEnable(timer);
}
/*******************************************************************************
****函数功能: 波形表生成函数
****入口参数: wave_type:波形种类，有SIN、SQUARE、SAWTOOTH
****出口参数: 无
****函数备注: 根据波形发生器的参数生成相应的波形，并将其储存到waveTab[]数组中
********************************************************************************/
void WAVE_GEN::waveGen(WAVE_TYPE wave_type)
{

  if (wave_type == SIN)
  {
    double sineValue = 0.0;
    for (int i = 0; i < samplePerCycle; i++)
    {
      sineValue = sin(((2 * PI) / samplePerCycle) * i) * (uMaxValue / 2) + offSetValue;
      waveTab1[i] = (((int)(sineValue * 255 / 3.3)));
    }
    Serial.printf("波形表重设成功，当前为正弦波\n");
  }
  else if (wave_type == SQUARE)
  {
    float x = samplePerCycle * ((float)duty / 100.0);
    int x1 = (int)x;
    for (int i = 0; i < samplePerCycle; i++)
    {
      if (i < x)
      {
        waveTab1[i] = (int)(255 * (uMaxValue / 2 + offSetValue) / 3.3);
      }
      else
      {
        waveTab1[i] = (int)(255 * (-(uMaxValue / 2) + offSetValue) / 3.3);
      }
    }
    Serial.printf("波形表重设成功，当前为方波,占空比:%d\n", duty);
  }
  else if (wave_type == SAWTOOTH) //锯齿波
  {
    for (int i = -127; i < 128; i++)
    {
      waveTab1[i + 127] = (int)((i + (offSetValue * 255 / 3.3)) * (uMaxValue / 3.3));
    }
    Serial.println("波形表重设成功，当前为锯齿波");
  }
  for (int i = 0; i < samplePerCycle; i++)
  {
    if (waveTab1[i] > 255)
    {
      waveTab1[i] = 255;
    }
    if (waveTab1[i] < 0)
    {
      waveTab1[i] = 0;
    }
    waveTab[i] = (uint8_t)waveTab1[i];
    // Serial.printf("wave:%d\n", waveTab[i]);
  }
}

void WAVE_GEN::waveSelect()
{
  // wave_type++;
  if (wave_type > 3)
  {
    wave_type = SIN;
  }
  waveGen(wave_type);
  delay(500);
}

#endif