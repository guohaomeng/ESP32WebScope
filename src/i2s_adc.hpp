/*******************************************************************************
****文件路径         : \ESP32WebScope\src\i2s_adc.hpp
****作者名称         : guohaomeng
****文件版本         : V1.0.0
****创建日期         : 2022-07-14 10:43:39
****简要说明         :
****
****版权信息         : 2022 by guohaomeng, All Rights Reserved.
********************************************************************************/
#ifndef I2S_ADC_H_
#define I2S_ADC_H_
#include <Arduino.h>
#include <driver/i2s.h>

#define ADC_CHANNEL ADC1_CHANNEL_7 // GPIO35
#define NUM_SAMPLES 2048

enum TRIGGER_MODE
{
  NONE = 1,
  UP,
  DOWN,
  AUTO
};

class I2S_ADC
{
public:
  i2s_port_t i2s_num = I2S_NUM_0;
  adc1_channel_t channel = ADC1_CHANNEL_7;
  adc_bits_width_t width_bit = ADC_WIDTH_12Bit;
  uint32_t sample_rate = 8000;
  uint32_t sample_rate_old = 8000;
  bool is_sample = false;
  bool is_change_rate = false;
  i2s_config_t i2s_config = {
      // I2S with ADC
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
      .sample_rate = sample_rate,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, // 双声道模式采集会使实际采样率加倍，且得到的波形噪声更少
      .communication_format = I2S_COMM_FORMAT_STAND_I2S,
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 16,
      .dma_buf_len = 1024, // 缓冲区大小 = dma_buf_len * chan_num * bits_per_chan / 8 = NUM_SAMPLES * 1 * 16 / 8 = 2048字节
      .use_apll = true,    // I2S 使用 APLL 作为 I2S 主时钟，使其能够获得准确的时钟
      .tx_desc_auto_clear = false,
      .fixed_mclk = 0};
  uint16_t i2s_read_buff[NUM_SAMPLES]; // i2s读取缓冲区
  float adcBuff[NUM_SAMPLES] = {0};
  float adc_max_value = 0;
  float adc_min_value = 0;
  TRIGGER_MODE trigger_mode = DOWN;

  I2S_ADC();
  I2S_ADC(i2s_port_t i2s_num, uint32_t sample_rate, adc1_channel_t channel, adc_bits_width_t width_bit);
  ~I2S_ADC();
  int get_adc_data(float *po_AdcValues, int length, int step);
  void i2s_reset();
  bool set_sample_rate(uint32_t rate);
  bool set_trigger_mode(int mode);
  int find_trigger_index(int NumSamps);
};

/*******************************************************************************
****函数功能: I2S_ADC默认构造函数
****出口参数: 无
****函数备注: 初始化单片机内置I2S_0,并将其连接到ADC1的通道7，即GPIO36
********************************************************************************/
I2S_ADC::I2S_ADC()
{
  i2s_config_t i2s_config = {
      // I2S with ADC
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
      .sample_rate = (8000),
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .intr_alloc_flags = 0,
      .dma_buf_count = 4,
      .dma_buf_len = 1024,
      .use_apll = false,
  };

  adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_11db);
  adc1_config_width(ADC_WIDTH_12Bit);
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_adc_mode(ADC_UNIT_1, ADC_CHANNEL);
  i2s_adc_enable(I2S_NUM_0);
}
/*******************************************************************************
****函数功能: I2S_ADC带参数的构造函数
****入口参数: i2s_num: i2s端口号
****入口参数: i2s_config: i2s配置结构体
****入口参数: channel: 绑定到的adc通道
****入口参数: width_bit: 采样数据位宽
****出口参数: 无
****函数备注: I2S_ADC带参数的构造函数
********************************************************************************/
I2S_ADC::I2S_ADC(i2s_port_t i2s_num, uint32_t sample_rate, adc1_channel_t channel, adc_bits_width_t width_bit)
{
  this->i2s_num = i2s_num;
  this->sample_rate = sample_rate;
  this->i2s_config.sample_rate = sample_rate;
  this->channel = channel;
  this->width_bit = width_bit;

  adc1_config_channel_atten(channel, ADC_ATTEN_11db);
  adc1_config_width(width_bit);
  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
  i2s_set_adc_mode(ADC_UNIT_1, channel);
  i2s_adc_enable(i2s_num);
  set_sample_rate(sample_rate);
}
I2S_ADC::~I2S_ADC() {}
/*******************************************************************************
****函数功能: 获取ADC数据，并将其送入i2s_read_buff数组
****入口参数: *po_AdcValues:目标数组，adc值读出后放入此数组
****入口参数: length:目标数组的长度，从adcBuff数组中取出指定长度放入目标数组
****入口参数: step:放入间隔，在adcBuff数组中每隔step个取出一个放入目标数组
****出口参数: Numsamps，返回读取的样本数
****函数备注: 同时刷新对应的电压数组po_AdcValues
********************************************************************************/
int I2S_ADC::get_adc_data(float *po_AdcValues, int length, int step)
{
  i2s_adc_enable(I2S_NUM_0);
  is_sample = true;
  size_t num_bytes_read = 0;
  i2s_zero_dma_buffer(I2S_NUM_0);
  memset(i2s_read_buff, 0x00, NUM_SAMPLES);
  memset(adcBuff, 0x00, NUM_SAMPLES);
  i2s_read(I2S_NUM_0, &i2s_read_buff, sizeof(i2s_read_buff), &num_bytes_read, (int)(20 * (277000 / sample_rate)));
  /* 第一个for循环，数据预处理 */
  int NumSamps = num_bytes_read / (2);
  for (int i = 0; i < NumSamps; i++)
  { //将12位值转换为电压
    adcBuff[i] = 3.3 * (float)((i2s_read_buff[i] & 0x0FFF)) / 0x0FFF;
    /* 找出峰峰值 */
    if (adcBuff[i] > adc_max_value)
      adc_max_value = adcBuff[i];
    if (adcBuff[i] < adc_min_value)
      adc_min_value = adcBuff[i];
  } /* 软件尝试边沿触发 */
  int trigger_index = find_trigger_index(NumSamps);
  /* 第二个for循环，获取触发后的波形 */
  for (int i = 0, j = 0; i < NumSamps; i++, j++)
  {
    if ((j < length) && (i * step) < NumSamps)
    {
      if (trigger_index != -1)
        po_AdcValues[j] = adcBuff[i * step + trigger_index];
      else
        po_AdcValues[j] = adcBuff[i * step];
    }
  }
  is_sample = false;
  i2s_adc_disable(I2S_NUM_0);
  return NumSamps; // 返回读取的样本数
}
/*******************************************************************************
****函数功能: 软件触发，找到第一个上升沿/下降沿位置
****出口参数: index: 第一个上升沿/下降沿位置，-1表示未找到
****函数备注: 无
********************************************************************************/
int I2S_ADC::find_trigger_index(int NumSamps)
{
  if (trigger_mode == NONE)
    return -1;
  uint8_t cmpBuff[NUM_SAMPLES] = {0};
  memset(cmpBuff, 0, NUM_SAMPLES);
  int index = -1;
  float adc_mid_value = (adc_max_value - adc_min_value) / 2;
  for (int i = 0; i < NumSamps; i++)
  {
    cmpBuff[i] = (adcBuff[i] < adc_mid_value ? 0 : 1);
    if (trigger_mode == UP && cmpBuff[i] == 1 && cmpBuff[i - 1] == 0)
    { /* (上升沿) */
      if (i <= NUM_SAMPLES / 2)
      { /* 如果上升沿的位置在波形的前一半，则可以往后得到完整波形 */
        index = i;
        return index;
      }
      else
      { /* 否则判定为触发失败 */
        return -1;
      }
    }
    else if (trigger_mode == DOWN && cmpBuff[i] == 0 && cmpBuff[i - 1] == 1)
    { /* (下降沿) */
      if (i <= NUM_SAMPLES / 2)
      { /* 如果下降沿的位置在波形的前一半，则可以往后得到完整波形 */
        index = i;
        return index;
      }
      else
      { /* 否则判定为触发失败 */
        return -1;
      }
    }
    else if (trigger_mode == AUTO && (abs(cmpBuff[i] - adcBuff[i - 1] == 1)))
    { /* (上升沿||下降沿) */
      if (i <= NUM_SAMPLES / 2)
      { /* 如果边沿的位置在波形的前一半，则可以往后得到完整波形 */
        index = i;
        return index;
      }
      else
      { /* 否则判定为触发失败 */
        return -1;
      }
    }
  } /* 未发现上升沿&下降沿 */
  return -1;
}
/*******************************************************************************
****函数功能: 重置i2s_adc外设
****出口参数: 无
****函数备注: 无
********************************************************************************/
void I2S_ADC::i2s_reset()
{
  i2s_driver_uninstall(i2s_num);
  adc1_config_channel_atten(channel, ADC_ATTEN_11db);
  adc1_config_width(width_bit);
  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
  i2s_set_adc_mode(ADC_UNIT_1, channel);
  i2s_adc_enable(i2s_num);
}
/*******************************************************************************
****函数功能: 设置示波器采样率
****入口参数: rate:示波器采样率，实测真实采样率为此值的一半
****出口参数: true:成功 false:失败
****函数备注: 似乎最高能到277k？https://github.com/espressif/arduino-esp32/blob/
             master/libraries/ESP32/examples/I2S/HiFreq_ADC/HiFreq_ADC.ino
********************************************************************************/
bool I2S_ADC::set_sample_rate(uint32_t rate)
{
  if (rate > 550000 || rate < 1000)
  {
    return false;
  }
  is_change_rate = true;
  if (i2s_config.channel_format <= 2)
    rate /= 2; // 左右声道模式采集会导致真实采样率变为设定值的一倍，这里补回来
  sample_rate_old = sample_rate;
  sample_rate = rate;
  esp_err_t ret;
  while (1)
  {
    if (is_sample == false)
    {
      ret = i2s_set_sample_rates(i2s_num, rate);
      break;
    }
    // 不加延时程序会死循环最后触发看门狗重启
    vTaskDelay(2 / portTICK_PERIOD_MS);
  }
  is_change_rate = false;
  // ESP_ERROR_CHECK(i2s_set_sample_rates(i2s_num, rate));
  return (ret == ESP_OK) ? true : false;
}
/*******************************************************************************
****函数功能: 设置触发模式
****入口参数: mode: 见TRIGGER_MODE
****出口参数: 返回结果成功或失败
****函数备注: 无
********************************************************************************/
bool I2S_ADC::set_trigger_mode(int mode)
{
  if (mode >= 1 && mode <= 4)
  {
    trigger_mode = (enum TRIGGER_MODE)(mode);
    return true;
  }
  else
  {
    Serial.println("触发模式参数错误");
    return false;
  }
}

#endif