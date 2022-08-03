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
#include "driver/dac.h"
#include "driver/timer.h"

#define SAMPLE_PER_CYCLE 256
#define ADC_MAX_VALUE 255
#define ADC_MAX_VOLTAGE 3.3f

enum WAVE_TYPE
{
  SIN = 1,
  SQUARE,
  SAWTOOTH
};
int waveindex = 2;                 // 当前波形位置
int wave_index_step = 1;           // 波形索引递增步长
uint8_t waveTab[SAMPLE_PER_CYCLE]; // 最终根据配置生成的波形数据
/* 创建硬件定时器 */
hw_timer_t *timer = NULL;
class WAVE_GEN
{
public:
  /* 波形发生器参数配置 */
  double uMaxValue = 3.3;         //峰峰值
  double offSetValue = 1.65;      //偏置电压
  int duty = 50;                  //占空比%(方波)
  WAVE_TYPE wave_type = SAWTOOTH; //波形种类
  unsigned int freq = 100;        // 频率
  String param = "";

  unsigned int freq_old = 100;        // 上一次的频率
  uint8_t waveTab1[SAMPLE_PER_CYCLE]; // 生成的波形数据

  /* 波形模式切换按键 */
  const int button = 12; // 波形切换引脚位置
  char received_data[5] = {0};
  unsigned int rec_cnt1 = 0;
  /* 各种函数，波形发生器相关以及定时器相关的 */
  WAVE_GEN(double uMaxValue, double offSetValue, int duty, unsigned int freq, WAVE_TYPE wave_type);
  ~WAVE_GEN();
  int set_uMaxValue(double value);
  int set_offSetValue(double value);
  int set_duty(int value);
  int set_freq(int value);
  int set_wave_type(WAVE_TYPE wave_type);
  String get_param();

  void initTimer();
  void updateTimer();
  void get_waveindex();
  void waveSelect(int wave);
  void waveGen(WAVE_TYPE wave_type);
  void adjust_step();
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
  if (waveindex >= SAMPLE_PER_CYCLE)
  {
    waveindex = 0;
  }
  dac_output_voltage(DAC_CHANNEL_1, waveTab[waveindex]);
  waveindex += wave_index_step;
}
/*******************************************************************************
****函数功能: 初始化波形发生器
****出口参数: 无
****函数备注: 将25号引脚作为输出，生成默认波形，并配置好定时器
********************************************************************************/
void WAVE_GEN::initTimer()
{
  /* 输出端口DAC_CHANNEL_1,即GPIO25 */
  dac_output_enable(DAC_CHANNEL_1);
  /* 默认输出正弦波 */
  waveGen(wave_type);

  /*  1/(80MHZ/80) = 1us  */
  timer = timerBegin(0, 80, true);
  Serial.printf("OK");
  /* 将onTimer函数附加到我们的计时器 */
  timerAttachInterrupt(timer, &onTimer, true);
  Serial.printf("OK");
  adjust_step();
  Serial.printf("freq:%d\n", freq);
  /* 设置闹钟每秒调用onTimer函数1 tick为1us   => 1秒为1000000us * /
  /* 重复闹钟（第三个参数）*/
  uint64_t T = (1000000 * wave_index_step) / (freq * SAMPLE_PER_CYCLE);
  Serial.printf("T:%d\n", T);
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
  timerAlarmDisable(timer); //先关闭定时器
  adjust_step();
  uint64_t dacTime = (1000000 * wave_index_step) / (freq * SAMPLE_PER_CYCLE); //波形周期,微秒
  /* 设置闹钟每秒调用onTimer函数1 tick为1us => 1秒为1000000us */
  /* 重复闹钟（第三个参数）*/
  timerAlarmWrite(timer, dacTime, true);
  /* 启动警报 */
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
    for (int i = 0; i < SAMPLE_PER_CYCLE; i++)
    {
      sineValue = sin(((2 * PI) / SAMPLE_PER_CYCLE) * i) * (uMaxValue / 2) + offSetValue;
      waveTab1[i] = (((int)(sineValue * ADC_MAX_VALUE / ADC_MAX_VOLTAGE)));
    }
    Serial.printf("波形表重设成功，当前为正弦波\n");
  }
  else if (wave_type == SQUARE)
  {
    float x = SAMPLE_PER_CYCLE * ((float)duty / 100.0);
    int x1 = (int)x;
    for (int i = 0; i < SAMPLE_PER_CYCLE; i++)
    {
      if (i < x)
      {
        waveTab1[i] = (int)(ADC_MAX_VALUE * (uMaxValue / 2 + offSetValue) / ADC_MAX_VOLTAGE);
      }
      else
      {
        waveTab1[i] = (int)(ADC_MAX_VALUE * (-(uMaxValue / 2) + offSetValue) / ADC_MAX_VOLTAGE);
      }
    }
    Serial.printf("波形表重设成功，当前为方波,占空比:%d\n", duty);
  }
  else if (wave_type == SAWTOOTH) //锯齿波
  {
    for (int i = -(SAMPLE_PER_CYCLE / 2); i < (SAMPLE_PER_CYCLE / 2); i++)
    {
      waveTab1[i + (SAMPLE_PER_CYCLE / 2)] = (int)(i * (uMaxValue / ADC_MAX_VOLTAGE) + (offSetValue * ADC_MAX_VALUE / ADC_MAX_VOLTAGE));
    }
    Serial.println("波形表重设成功，当前为锯齿波");
  }
  for (int i = 0; i < SAMPLE_PER_CYCLE; i++)
  {
    if (waveTab1[i] > ADC_MAX_VALUE)
    {
      waveTab1[i] = ADC_MAX_VALUE;
    }
    if (waveTab1[i] < 0)
    {
      waveTab1[i] = 0;
    }
    waveTab[i] = (uint8_t)waveTab1[i];
    // Serial.printf("wave:%d\n", waveTab[i]);
  }
}

void WAVE_GEN::adjust_step()
{
  if (freq > 0 && freq <= 100)
  {
    wave_index_step = 1;
  }
  else if (freq > 100 && freq <= 1500)
  {
    wave_index_step = (int)(freq / 100) + 1;
  }
  else
  {
  }
}

void WAVE_GEN::waveSelect(int wave)
{
  if (wave < 1 || wave > 3)
  {
    Serial.println("波形种类设置错误");
    return;
  }
  wave_type = (enum WAVE_TYPE)(wave);
  waveGen(wave_type);
}

int WAVE_GEN::set_uMaxValue(double value)
{
  if (value >= 0 && value <= 3.3)
  {
    uMaxValue = value;
    waveGen(wave_type);
    return 0;
  }
  else
  {
    Serial.println("峰峰值设置超出范围0-3.3V");
    return -1;
  }
}
int WAVE_GEN::set_offSetValue(double value)
{
  if (value >= 0 && value <= 3.3)
  {
    offSetValue = value;
    waveGen(wave_type);
    return 0;
  }
  else
  {
    Serial.println("偏置电压设置超出范围0-3.3V");
    return -1;
  }
}
int WAVE_GEN::set_duty(int value)
{
  if (value >= 0 && value <= 100)
  {
    duty = value;
    waveGen(wave_type);
    return 0;
  }
  else
  {
    Serial.println("占空比设置超出范围0-100");
    return -1;
  }
}
int WAVE_GEN::set_freq(int value)
{
  if (value > 0 && value <= 1500)
  {
    freq_old = freq;
    freq = value;
    if (freq != freq_old)
      updateTimer();
    return 0;
  }
  else
  {
    Serial.println("频率设置超出范围0~1.5kHz");
    return -1;
  }
}

String WAVE_GEN::get_param()
{
  param = "";
  /* 发送如下格式字符串 {"param":{"U":3.3,"B":1.65,"D":50,"F":100,"W":1}} */
  param += "{\"param\":{\"U\":";
  param += String(uMaxValue);
  param += String(",\"B\":");
  param += String(offSetValue);
  param += String(",\"D\":");
  param += String(duty);
  param += String(",\"F\":");
  param += String(freq);
  param += String(",\"W\":");
  param += String((int)wave_type);
  param += String("}}");
  return param;
}

#endif