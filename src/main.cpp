#include <Arduino.h>
#include <driver/i2s.h>
#include "driver/dac.h"

#include <WiFi.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

double uMaxValue = 3.3;    //峰峰值
double offSetValue = 1.65; //偏置电压
int duty = 50;             //占空比%(方波)
int wave = 1;              //波形种类
#define samplePerCycle 255
//定义8位R2R DA输出的对应值
uint8_t waveTab1[samplePerCycle];
uint8_t waveTab[samplePerCycle];
//定义图形周期
int waveindex = 2;
unsigned int freq = 100;
unsigned int freq_old = 100;
//波形模式切换按键
const int button = 12;
char received_data[5] = {0};
unsigned int rec_cnt1 = 0;

const char *ssid = "HONOR-0511YJ";   // Enter SSID
const char *password = "tian5019@."; // Enter Password

/* 创建硬件定时器 */
hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer()
{
  if (waveindex >= 255)
  {
    waveindex = 0;
  }
  dacWrite(25, waveTab[waveindex]);
  waveindex++;
}
void updateTimer()
{
  timerAlarmDisable(timer);                  //先关闭定时器
  uint64_t dacTime = 1000000 / (freq * 255); //波形周期,微秒
  /* *设置闹钟每秒调用onTimer函数1 tick为1us => 1秒为1000000us * /
  / *重复闹钟（第三个参数）*/
  timerAlarmWrite(timer, dacTime, true);
  /* 启动警报*/
  timerAlarmEnable(timer);
}
void waveSelect();
void wave_gen(int index);
void InitWeb();
char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="zh-CN">
<head> 
  <meta charset="UTF-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>波形发生器</title>
  <style>
    *{margin: 0;padding: 0;}
  </style>
</head>
<body onload="javascript:init()">
  <div>
    <p>coming soon~</p>
  </div>  
</body>
<script>
  var webSocket;
  function init(){
    webSocket = new WebSocket('ws://'+window.location.hostname +':81/');
    webSocket.onmessage = function(event){
    var data = JSON.parse(event.data);
    console.log(data);
  }
}
</script>
</html>
)=====";


void setup()
{
  Serial.begin(115200);

  //设置中断程序
  // attachInterrupt(button, waveSelect, RISING);

  //输出端口
  pinMode(25, OUTPUT);
  //默认输出正玄波
  wave_gen(1);

  InitWeb();

  /*  1/(80MHZ/80) = 1us  */
  timer = timerBegin(0, 80, true);

  /* 将onTimer函数附加到我们的计时器 */
  timerAttachInterrupt(timer, &onTimer, true);

  /* *设置闹钟每秒调用onTimer函数1 tick为1us   => 1秒为1000000us * /
  / *重复闹钟（第三个参数）*/
  timerAlarmWrite(timer, 1000000 / (freq * 255), true);

  /* 启动警报*/
  timerAlarmEnable(timer);
}

void loop()
{

}

// 波形选择程序
void waveSelect()
{
  wave++;
  if (wave > 3)
  {
    wave = 1;
  }
  wave_gen(wave);
  // for (int i = 0; i < 255; i++)
  // {
  //   printf("dac:%d\n", wavedigital[i]);
  // }
  delay(500);
}

//波形数值生成

void wave_gen(int index)
{

  if (index == 1)
  {
    double sineValue = 0.0;
    for (int i = 0; i < samplePerCycle; i++)
    {
      sineValue = sin(((2 * PI) / samplePerCycle) * i) * (uMaxValue / 2) + offSetValue;
      waveTab1[i] = (((int)(sineValue * 255 / 3.3)));
    }
    Serial.printf("波形表重设成功，当前为正弦波\n");
  }
  else if (index == 2)
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
  else if (index == 3) //锯齿波
  {
    for (int i = -127; i < 128; i++)
    {
      waveTab1[i+127] = (int)((i + (offSetValue * 255 / 3.3)) * (uMaxValue / 3.3));
      
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
    Serial.printf("wave:%d\n",waveTab[i]);
  }
}

void InitWeb()
{

}