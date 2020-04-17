/* Potansiyometre ile ADC okumasi yapiliyor ve okunan degerler DAC a aktariliyor 
   potansiyometreden aldigimiz degere gore led in parlakligi degisecektir */

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_Struct;
ADC_InitTypeDef ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonInitStruct;
DAC_InitTypeDef DAC_InitStruct;

void GPIO_Config() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	// ADC okumasi yapilacak olan pin
	GPIO_Struct.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_Struct.GPIO_OType 	= GPIO_OType_PP;
	GPIO_Struct.GPIO_Pin 	= GPIO_Pin_0;
	GPIO_Struct.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Struct.GPIO_Speed 	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA , &GPIO_Struct);
	
	// DAC cikis pini
	GPIO_Struct.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_Struct.GPIO_OType 	= GPIO_OType_PP;
	GPIO_Struct.GPIO_Pin 	= GPIO_Pin_4;
	GPIO_Struct.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Struct.GPIO_Speed 	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA,&GPIO_Struct);
}
void ADC_Config() {
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE); 	 // Clock hattini aktif ettik.

	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent; 	 // bagimsiz mod da calisma aktif edildi.
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4; // adc max 36 MHz degerinde calisir.Suanda 168 MHz de calistigimiz icin 4 e bolduk.
	ADC_CommonInit(ADC1,&ADC_CommonInitStruct); 		 // adc degeri isleniyor.

	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;	 // Coklu ADC okumasi yapmadigimiz icin sadece cozunurlugu ayarlamamiz yeterli olacak.
	ADC_Init(ADC1,&ADC_InitStruct);
	ADC_Cmd(ADC1,ENABLE); 					 // ADC cevresel birimi oldugu icin cmd kullanarak aktif etmemiz gerekir.
}
void DAC1_Config() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);

	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable; 	// gurultu filtreleyici aktif edildi.
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_None; 			// tetikleme olmasin
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None; 	// dalga uretici olmasin
	DAC_Init(DAC_Channel_1,&DAC_InitStruct);
	DAC_Cmd(DAC_Channel_1,ENABLE); 					// DAC cevresel birim
}

uint16_t adc_value;

uint16_t Read_ADC() { // 12 bit degerinde adc okudugumuz icin 16 bit geri donus degeri olan adc fonk yazmaliyiz.
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);

	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET); // reset ise buradan cik degil ise reset olana kadar bekle
	return ADC_GetConversionValue(ADC1); 		      // ADC de okunan degeri geri donderiyoruz.
}
int main(void) {
	GPIO_Config();
	ADC_Config();
	DAC1_Config();

  while (1) {
	  adc_value = Read_ADC();
	  DAC_SetChannel1Data(DAC_Align_12b_R,adc_value); // adc de okunan degeri dac a atiyoruz
  }
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
