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
uint32_t sampleRate = 2000;
int sampleStep = 1;
bool chart_refresh = false;

/* 实例化一个波形发生器 */
WAVE_TYPE wave_type = SIN;
WAVE_GEN wave_gen(3.3, 1.65, 50, 10, SIN);

/* 初始化基于I2S的ADC，默认采样频率8KHz，每次采样16位，ADC精度12位 */
I2S_ADC i2s_adc(I2S_NUM_0,sampleRate,ADC1_CHANNEL_7,ADC_WIDTH_12Bit);

/* 建立http及websocket服务器 */
myWebSocket::CombinedServer server;
IPAddress APIP = IPAddress(192, 168, 8, 1);
IPAddress subnet = IPAddress(255, 255, 255, 0);
myWebSocket::WebSocketClient *client1 = nullptr;
bool websocket_init();

/*******************************************************************************
****函数功能: 核心0上运行的任务2，运行websocket服务器与http服务器，与上位机通过WiFi进行通信
****入口参数: *arg:
****出口参数: 无
****函数备注: 将主程序与WiFi通信程序分别放两个核心上运行，提高执行速度
********************************************************************************/
void Task2(void *arg)
{

  vTaskDelay(500 / portTICK_PERIOD_MS);
  while (true)
  {
    server.loop();
    if (client1 != nullptr && chart_refresh == true)
    {
      for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
      { /* 向上位机发送数据 */
        client1->send(String(i) + String(":") + String(ADC_sample[i]));
      }
    }
    vTaskDelay(15 / portTICK_PERIOD_MS);
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
  vTaskDelay(500 / portTICK_PERIOD_MS);
}
/*******************************************************************************
****函数功能: loop函数
****出口参数: 无
****函数备注: 程序执行完setup函数后，循环执行loop函数
********************************************************************************/
void loop()
{
  // 进行一次采样
  i2s_adc.get_adc_data(ADC_sample,ADC_SAMPLE_SIZE,sampleStep);
  for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
  {
    Serial.printf("adc:%d,%.3f\n",i,ADC_sample[i]);
  }
  
  vTaskDelay(50 / portTICK_PERIOD_MS);
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
            // Serial.print("ID:");
            // Serial.println(client->getID());
            Serial.println("Got text data:");
            // Serial.println(String((char *)payload));
            char received_chars[10];
            memset(received_chars, '\0', 10);
            for (int i = 0; i < length; i++)
            {
              if (length >= 10)
                break;
              received_chars[i] = payload[i];
            }
            Serial.printf("char:%s\n", received_chars);
            /* 上位机发送指令IDx来建立连接，x标记为指定设备ID */
            if (payload[0] == 'I' && payload[1] == 'D')
            {
              client->setID(payload[2]);
              client1 = client;
              Serial.printf("ID:%c\n", (char *)payload[2]);
            }

            client->send(String(received_chars));
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
