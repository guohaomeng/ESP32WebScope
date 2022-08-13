/*******************************************************************************
****文件路径         : \ESP32WebScope\src\main.cpp
****作者名称         : guohaomeng
****文件版本         : V1.0.0
****创建日期         : 2022-07-01 13:07:26
****简要说明         :
****
****版权信息         : 2022 by guohaomeng, All Rights Reserved.
********************************************************************************/
#include <Arduino.h>
#include "mywebsocket/mywebsocket.h"
#include <WiFi.h>

#include "i2s_adc.hpp"
#include "wave_gen.hpp"

#define ADC_SAMPLE_SIZE 256
float ADC_sample[ADC_SAMPLE_SIZE];
uint32_t sampleRate = 2000; // 根据实测，真实的I2S采样频率应为此值的一半
int sampleStep = 1;
bool chart_refresh = false;

/* 实例化一个波形发生器 */
WAVE_TYPE wave_type = SAWTOOTH;
WAVE_GEN wave_gen(3.3, 1.65, 50, 100, wave_type);

/* 初始化基于I2S的ADC，默认采样频率8KHz，每次采样16位，ADC精度12位 */
I2S_ADC i2s_adc(I2S_NUM_0, sampleRate, ADC1_CHANNEL_7, ADC_WIDTH_12Bit);

/* 建立http及websocket服务器 */
myWebSocket::CombinedServer server;
IPAddress APIP = IPAddress(192, 168, 8, 1);
IPAddress subnet = IPAddress(255, 255, 255, 0);
myWebSocket::WebSocketClient *client1 = nullptr;
bool websocket_init();
void command_loop(void);
void command_loop2(char *received_chars);
/*******************************************************************************
****函数功能: 核心0上运行的任务2，运行websocket服务器与http服务器，与上位机通过WiFi进行通信
****入口参数: *arg:
****出口参数: 无
****函数备注: 将主程序与WiFi通信程序分别放两个核心上运行，提高执行速度
********************************************************************************/
void Task2(void *arg)
{
  int k = 0;
  vTaskDelay(50 / portTICK_PERIOD_MS);
  while (true)
  {
    server.loop();
    if (k >= 30)
    {
      k = 0;
      String str = "{\"a\":[";
      for (int i = 0; i < ADC_SAMPLE_SIZE - 1; i++)
      {
        str += String(ADC_sample[i]) + String(",");
      }
      str += (String(ADC_sample[ADC_SAMPLE_SIZE - 1]) + String("]}"));
      if (client1 != nullptr && chart_refresh == true)
      { /* 向上位机发送数据 */
        client1->send(str);
      }
    }
    k++;
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}
/*******************************************************************************
****函数功能: 主程序入口
****出口参数: 无
****函数备注: 启动后首先执行setup函数，只执行一次
********************************************************************************/
void setup()
{
  Serial.begin(115200);

  /* 初始化波形发生器 */
  wave_gen.initTimer();
  Serial.println("波形发生器初始化成功");
  websocket_init();
  Serial.println("websocket初始化成功");
  /* 创建任务2，建立并保持与上位机的通信 */
  xTaskCreatePinnedToCore(Task2, "Task2", 24 * 4096, NULL, 1, NULL, 0);
  vTaskDelay(50 / portTICK_PERIOD_MS);
}
/*******************************************************************************
****函数功能: loop函数
****出口参数: 无
****函数备注: 程序执行完setup函数后，循环执行loop函数
********************************************************************************/
void loop()
{
  // 进行一次采样
  if (i2s_adc.is_change_rate == false)
    i2s_adc.get_adc_data(ADC_sample, ADC_SAMPLE_SIZE, sampleStep);
  // for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
  // {
  //   Serial.printf("adc:%d,%.3f\n", i, ADC_sample[i]);
  // }
  command_loop();
  vTaskDelay(45 / portTICK_PERIOD_MS);
}

/*******************************************************************************
****函数功能: websocket服务器及http服务器初始化
****出口参数: 初始化结果 true/false
****函数备注: websocket服务器及http服务器初始化，返回初始化结果：成功/失败
********************************************************************************/
bool websocket_init()
{
  //调用函数启用ESP32硬件加速
  mycrypto::SHA::initialize();
  WiFi.softAP("ESP32_WebSocketServer");
  vTaskDelay(300 / portTICK_PERIOD_MS);
  WiFi.softAPConfig(APIP, APIP, subnet);
  // 设置websocket服务器及回调函数
  server.setCallback(
      [](myWebSocket::WebSocketClient *client, myWebSocket::WebSocketEvents type, uint8_t *payload, uint64_t length)
      {
        if (length)
        {
          if (type == myWebSocket::TYPE_TEXT)
          {
            char received_chars[15];
            memset(received_chars, '\0', 15);
            for (int i = 0; i < length; i++)
            {
              if (length >= 15)
                break;
              received_chars[i] = payload[i];
            }
            Serial.printf("%s\n", received_chars);
            /* 上位机发送指令IDx来建立连接，x标记为指定设备ID */
            if (payload[0] == 'I' && payload[1] == 'D')
            {
              client->setID(payload[2]);
              client1 = client;
            }
            client->send(String(received_chars));
            command_loop2(received_chars);
          }
          else if (type == myWebSocket::TYPE_BIN)
          {
            Serial.println("Got binary data, length: " + String((long)length));
            Serial.println("First byte: " + String(payload[0]));
            Serial.println("Last byte: " + String(payload[length - 1]));
          }
          else if (type == myWebSocket::WS_DISCONNECTED)
          {
            Serial.println("Websocket disconnected.");
          }
          else if (type == myWebSocket::WS_CONNECTED)
          {
            Serial.println("Websocket connected.");
          }
        }
      });
  // 开启网页服务器
  server.on(
      "/",
      [](myWebSocket::ExtendedWiFiClient *client, myWebSocket::HttpMethod method, uint8_t *data, uint64_t len)
      {
        client->send(R"(
<!DOCTYPE html>
<html>

<head>
  <title>ESP32 Combined Server</title>
</head>

<body>
  <h1>Hello World!</h1>

</body>

</html>
)");
        client->close();
      });
  server.begin(80);
  return true;
}

/*******************************************************************************
****函数功能: 串口通信函数
****出口参数: 无
****函数备注: 解析串口接收到的上位机指令
********************************************************************************/
void command_loop(void)
{
  // 如果串口是空的直接返回
  if (Serial.available() == 0)
    return;
  char received_chars[10];
  memset(received_chars, '\0', 10);
  vTaskDelay(1 / portTICK_PERIOD_MS);
  // 从串口读取返回的数据，读取20个字符
  Serial.read(received_chars, 10);

  // 根据指令做不同动作
  if (received_chars[0] == 'F') // F指令设置波形发生器频率
  {
    int F = atoi(received_chars + 1);
    wave_gen.set_freq(F);
    Serial.printf("%s,%d\n", received_chars, F);
  }
  if (received_chars[0] == 'D') // D指令设置波形发生器占空比
  {
    int D = atoi(received_chars + 1);
    wave_gen.set_duty(D);
    Serial.printf("%s,%d\n", received_chars, D);
  }
  if (received_chars[0] == 'U') // U指令设置波形发生器峰峰值
  {
    double U = atof(received_chars + 1);
    wave_gen.set_uMaxValue(U);
    Serial.printf("%s,%.3f\n", received_chars, U);
  }
  if (received_chars[0] == 'B') // B指令设置波形发生器偏置电压
  {
    double B = atof(received_chars + 1);
    wave_gen.set_offSetValue(B);
    Serial.printf("%s,%.3f\n", received_chars, B);
  }
  if (received_chars[0] == 'R') // R指令设置I2S_ADC采样速率
  {
    int R = atoi(received_chars + 1);
    i2s_adc.set_sample_rate((uint32_t)R);
    Serial.printf("%s,%d\n", received_chars, R);
  }
  if (received_chars[0] == 'W') // R指令设置I2S_ADC采样速率
  {
    int W = atoi(received_chars + 1);
    wave_gen.waveSelect(W);
  }
  //  最后清空串口
  while (Serial.read() >= 0)
    ;
}

// 解析wifi接收到的上位机指令
void command_loop2(char *received_chars)
{
  // 根据指令做不同动作
  if (received_chars[0] == 'F') // F指令设置波形发生器频率
  {
    int F = atoi(received_chars + 1);
    wave_gen.set_freq(F);
    Serial.printf("%s,%d\n", received_chars, F);
  }
  if (received_chars[0] == 'D') // D指令设置波形发生器占空比
  {
    int D = atoi(received_chars + 1);
    wave_gen.set_duty(D);
    Serial.printf("%s,%d\n", received_chars, D);
  }
  if (received_chars[0] == 'U') // U指令设置波形发生器峰峰值
  {
    double U = atof(received_chars + 1);
    wave_gen.set_uMaxValue(U);
    Serial.printf("%s,%.3f\n", received_chars, U);
  }
  if (received_chars[0] == 'B') // B指令设置波形发生器偏置电压
  {
    double B = atof(received_chars + 1);
    wave_gen.set_offSetValue(B);
    Serial.printf("%s,%.3f\n", received_chars, B);
  }
  if (received_chars[0] == 'R') // R指令设置I2S_ADC采样速率
  {
    uint32_t R = atoi(received_chars + 1);
    if(i2s_adc.set_sample_rate(R))
      wave_gen.sample_rate = R;
    Serial.printf("%s,%d\n", received_chars, R);
  }
  if (received_chars[0] == 'W') // W指令设置波形
  {
    int W = atoi(received_chars + 1);
    wave_gen.waveSelect(W);
  }
  if (received_chars[0] == 'C' && received_chars[1] == 'T')
  { // CT使能发送采样数据
    chart_refresh = true;
  }
  if (received_chars[0] == 'C' && received_chars[1] == 'F')
  { // CF停止发送采样数据
    chart_refresh = false;
  }
  if (received_chars[0] == 'G' && received_chars[1] == 'P')
  { // GP指令获取波形发生器全部参数
    if (client1 != nullptr)
    { /* 向上位机发送数据 */
      client1->send(wave_gen.get_param());
    }
  }
}