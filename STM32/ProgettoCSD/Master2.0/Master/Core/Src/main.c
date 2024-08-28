/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>
#include <string.h>
#include "../LIBR/Metodi/ManageStr.h"
#include "../LIBR/Metodi/invio.h"
#include "../LIBR/fuzzy/fuzzy.h"
//parametri Tx e Rx
#define BUFFER_LEN  1
#define BUFFER_SIZE 50
#define stop  115
#define spazio  32

uint8_t RX_BUFFER[BUFFER_LEN] = {0};
uint8_t RX_BUFFER1[BUFFER_LEN] = {0};
uint8_t RX_BUFFER2[BUFFER_LEN] = {0};

uint8_t rx_buffer[BUFFER_SIZE];
int rx_index = 0;
bool rx_complete = false;

uint8_t rx_buffer1[BUFFER_SIZE];
int rx_index1 = 0;
bool rx_complete1 = false;


uint8_t rx_buffer2[BUFFER_SIZE];
int rx_index2 = 0;
bool rx_complete2 = false;




/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

bool StartSystem=false;
int ModFunz=0;
char StartRx='1';
int modo0=0,modo1=0,StatoC=0;

float temp0=0,temp1=0,hmdy0=0,hmdy1=0;
double uscita0 = 0;
Stringa_TypeDef valS0,valS1;




void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
                       if(StatoC==0){
                    	   HAL_UART_Transmit(&huart1, &StartRx, strlen(StartRx), HAL_MAX_DELAY);
                    	   HAL_UART_Transmit(&huart2, &StartRx, strlen(StartRx), HAL_MAX_DELAY);
                    	   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10,GPIO_PIN_SET);
                    	   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11,GPIO_PIN_RESET);
                    	   StatoC=1;

                       }

}









void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == huart1.Instance)
    {
		HAL_UART_Receive_IT(&huart1, RX_BUFFER, BUFFER_LEN);
		if (RX_BUFFER[0] == stop) {
				   rx_complete = true;
					valS1=CutString(&rx_buffer);
					 temp0=valS1.Temp;
					 hmdy0=valS1.Hmdy;
					 modo0=valS1.Mode;
					rx_index = 0;
					for(int i=0;i<50;i++)rx_buffer[i]=0;
			   } else {
				   rx_buffer[rx_index] = RX_BUFFER[0];
				   rx_index++;
			   }

    }
    if(huart->Instance == huart2.Instance)
    {
       HAL_UART_Receive_IT(&huart2, RX_BUFFER1, BUFFER_LEN);
       if (RX_BUFFER1[0] == stop) {
                   rx_complete1 = true;
    	   	   	   valS1=CutString(&rx_buffer1);
    	       	    temp1=valS1.Temp;
    	       	    hmdy1=valS1.Hmdy;
    	       	    modo1=valS1.Mode;
    	       	   rx_index1 = 0;
    	       	for(int i=0;i<50;i++)rx_buffer1[i]=0;
                } else {
                    rx_buffer1[rx_index1] = RX_BUFFER1[0];
                    rx_index1++;
                }
    }
    if(rx_complete == true &&  rx_complete1 == true){
		//funzione calcola_psrsmetri
		uscita0 = calcola_parametri(temp0, hmdy0, StatoC, modo0);
		double uscita1 = calcola_parametri(temp1, hmdy1, StatoC, modo1);
		//funzione invio
    	invio_tx(temp0, hmdy0, uscita0, huart4);
    	invio_tx(temp1, hmdy1, uscita1, huart4);
    	rx_complete = false;
    	rx_complete1 = false;
    }
}


int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_UART4_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart2, RX_BUFFER1, BUFFER_LEN);
	HAL_UART_Receive_IT(&huart1, RX_BUFFER, BUFFER_LEN);
	for(int i=0;i<50;i++)rx_buffer[i]=0;

while (1)
{
				if (StatoC == 1){
	 	      	    	   if (uscita0 <= -5) {
	 	      	    		   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_14, GPIO_PIN_RESET);
	 	      	    		   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	 	      	    	    } if (uscita0 <= -15.0) {
	 	      	    	        // Accendi il LED 2
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_14, GPIO_PIN_RESET);
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);
	 	      	    	    } if (uscita0 <= -25.0) {
	 	      	    	        // Accendi il LED 3
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_14, GPIO_PIN_RESET);
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6, GPIO_PIN_SET);
	 	      	    	    } if (uscita0 <= -35.0) {
	 	      	    	        // Accendi il LED 4
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_14, GPIO_PIN_RESET);
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);
	 	      	    	    } if (uscita0 <= -45.0) {
	 	      	    	        // Accendi il LED 5
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_14, GPIO_PIN_RESET);
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5, GPIO_PIN_SET);
	 	      	    	    } if (uscita0 <= -52.0) {
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_14, GPIO_PIN_RESET);
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4, GPIO_PIN_SET);
	 	      	    	    } if (uscita0 <= -55.0) {
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_14, GPIO_PIN_RESET);
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3, GPIO_PIN_SET);
	 	      	    	    } if (uscita0 <= -60.0) {
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_14, GPIO_PIN_RESET);
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_11, GPIO_PIN_SET);
	 	      	    	    } if (uscita0 <= -65.0){
	 	      	    	        // Accendi il LED 9
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_14, GPIO_PIN_RESET);
	 	      	    	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_14, GPIO_PIN_SET);
	 	      	    	    }
				}
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_UART4;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PE9 PE10 PE11 PE12
                           PE13 PE14 PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 PB12 PB13
                           PB14 PB3 PB4 PB5
                           PB6 PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

