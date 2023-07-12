

#include "LDR.h"
#include "LDR_cfg.h"


static ADC_HandleTypeDef hadc = {0};
static ADC_ChannelConfTypeDef sConfig = {0};
static uint8_t calibrated = 0;

void LDR_Init(uint16_t LDR_Instance)
{
	if(LM35_CfgParam[LM35_Instance].LDR_GPIO == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(LM35_CfgParam[LDR_Instance].LDR_GPIO == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if(LM35_CfgParam[LDR_Instance].LDR_GPIO == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if (LM35_CfgParam[LDR_Instance].LDR_GPIO == GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	hadc.Instance = LDR_CfgParam[LDR_Instance].ADC_Instance;
	hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.NbrOfConversion = 1;
	HAL_ADC_Init(&hadc);
    sConfig.Channel = LDR_CfgParam[LDR_Instance].ADC_CH;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);

	if(calibrated == 0)
	{
		HAL_ADCEx_Calibration_Start(&hadc);
		calibrated = 1;
	}
	
}


float LDR_Read(uint16_t LDR_Instance,float DC_Multiplier)
{
	uint32_t AD_RES;
	float Tensione = 0;

	// Select The Sensor Instance ADC Channel
	sConfig.Channel = LM35_CfgParam[LDR_Instance].ADC_CH;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);
	// Start ADC Conversion
	HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);
    // Get The Result & Convert To Temp In c_degrees
	AD_RES = HAL_ADC_GetValue(&hadc);
	Tensione = (AD_RES-DC_Multiplier)*DC_Multiplier;

	return Tensione;
}



float LDR_FirstRead(uint16_t LDR_Instance)
{
	uint32_t AD_RES;
	float Soglia_Rif = 0;

	// Select The Sensor Instance ADC Channel
	sConfig.Channel = LM35_CfgParam[LDR_Instance].ADC_CH;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);
	// Start ADC Conversion
	HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);
    // Get The Result & Convert To Temp In c_degrees
	Vamb = HAL_ADC_GetValue(&hadc1);
       Soglia_Rif = 65535/(4096-Vamb);

	return Soglia_Rif;
}







