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
#define NUM_SAMPLES 1024

class I2S_ADC
{
public:
  i2s_port_t i2s_num = I2S_NUM_0;
  adc1_channel_t channel = ADC1_CHANNEL_7;
  adc_bits_width_t width_bit = ADC_WIDTH_12Bit;
  uint32_t sample_rate = 8000;
  i2s_config_t i2s_config = {
      // I2S with ADC
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
      .sample_rate = sample_rate,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
      .communication_format = I2S_COMM_FORMAT_I2S_MSB,
      .intr_alloc_flags = 0,
      .dma_buf_count = 4,
      .dma_buf_len = NUM_SAMPLES, // 缓冲区大小 = dma_buf_len * chan_num * bits_per_chan / 8 = NUM_SAMPLES * 1 * 16 / 8 = 2048字节
      .use_apll = true,           // I2S 使用 APLL 作为 I2S 主时钟，使其能够获得准确的时钟
  };
  uint16_t i2s_read_buff[NUM_SAMPLES * 2]; // i2s读取缓冲区
  float adcBuff[NUM_SAMPLES] = {0};

  I2S_ADC();
  I2S_ADC(i2s_port_t i2s_num, uint32_t sample_rate, adc1_channel_t channel, adc_bits_width_t width_bit);
  ~I2S_ADC();
  int get_adc_data(float *po_AdcValues, int length, int step);
  void i2s_reset();
  bool set_sample_rate(uint32_t rate);
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
      .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
      .communication_format = I2S_COMM_FORMAT_I2S_MSB,
      .intr_alloc_flags = 0,
      .dma_buf_count = 4,
      .dma_buf_len = NUM_SAMPLES,
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
  // uint16_t *i2s_read_buff = malloc(sizeof(uint16_t) * NUM_SAMPLES*2);
  // uint16_t i2s_read_buff[NUM_SAMPLES*2];
  size_t num_bytes_read = 0;

  i2s_read(I2S_NUM_0, (void *)i2s_read_buff, NUM_SAMPLES * 2 * sizeof(uint16_t), &num_bytes_read, portMAX_DELAY);

  int NumSamps = num_bytes_read / (2 * sizeof(uint16_t));

  for (int i = 0, j = 0; i < NumSamps; i++)
  { //将12位值转换为电压
    adcBuff[i] = 3.3 * ((float)(i2s_read_buff[i * 2] & 0x0FFF)) / 0x0FFF;
    if (NumSamps % step == 0 && step > 0)
    {
      if (j < length)
        po_AdcValues[j] = adcBuff[i];
      j+=step;
    }
  }
  // free(i2s_read_buff);
  return NumSamps; // 返回读取的样本数
}

void I2S_ADC::i2s_reset()
{
  i2s_driver_uninstall(i2s_num);
  adc1_config_channel_atten(channel, ADC_ATTEN_11db);
  adc1_config_width(width_bit);
  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
  i2s_set_adc_mode(ADC_UNIT_1, channel);
  i2s_adc_enable(i2s_num);
}

bool I2S_ADC::set_sample_rate(uint32_t rate)
{
  if (rate > 128000 || rate < 1000)
  {
    return false;
  }
  this->sample_rate = rate;
  // ESP_ERROR_CHECK(i2s_set_sample_rates(i2s_num, rate));
  esp_err_t ret = i2s_set_sample_rates(i2s_num, rate);
  return (ret == ESP_OK) ? true : false;
}

#endif