/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

  // SET PIN CONFIGURATIONS
  /* AD7606   ->    STM32
   * BUSY 		 -> PC7
   * RESET 		 -> PC6
   * CONVST 	 -> PB15
   * CHIP_SELECT -> PB14 (SPI CS)
   * D7_OUT 	 -> PC11  (MISO)  PB13
   * D8_OUT      -> PC8
   * RD 		 -> PB12    //PC10 (SPI SCLK)  (prima era pb12)
   * */
#define BUSY_PORT GPIOC
#define BUSY_PIN GPIO_PIN_7
#define RESET_PORT GPIOC
#define RESET_PIN GPIO_PIN_6
#define CONVST_PORT GPIOB
#define CONVST_PIN GPIO_PIN_15
#define CS_PORT GPIOB
#define CS_PIN GPIO_PIN_14
#define D7_PORT GPIOC
#define D7_PIN GPIO_PIN_11
#define RD_PORT GPIOB
#define RD_PIN GPIO_PIN_12
#define D8_PORT GPIOC
#define D8_PIN GPIO_PIN_8

#define MAXBUFFERSIZE 100


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI3_Init(void);
static void MX_TIM10_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define TOTAL_RAW_BYTES 128   //16 byte = 128 bit
uint16_t parsed[8];  //was 8
uint16_t raw[TOTAL_RAW_BYTES];

uint8_t RxBuf[MAXBUFFERSIZE];
uint8_t TxBuf[MAXBUFFERSIZE];
int char_counter = 0;
uint8_t WTxBuf [MAXBUFFERSIZE] = "Hello from STM32 \r\n";  // Message to be transmitted over UART

uint8_t commd_on [MAXBUFFERSIZE] = "ON";
uint8_t commd_off [MAXBUFFERSIZE] = "OFF";

void parseRawBytes();

void read(int16_t *rawdatabuffer);
int twos_complement(int num, int bits);

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  // SET CS LOW
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, 0);
  // SET CONVST HIGH
  HAL_GPIO_WritePin(CONVST_PORT, CONVST_PIN, 1);  // convst high
  //RESET LOW
  HAL_GPIO_WritePin(RESET_PORT, RESET_PIN, 0);
  // RESET HIGH
  HAL_GPIO_WritePin(RESET_PORT, RESET_PIN, 1);
  //RESET LOW
  HAL_GPIO_WritePin(RESET_PORT, RESET_PIN, 0);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_SPI3_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
  //HAL_UART_Receive_IT(&huart2, RxBuf, MAXBUFFERSIZE);
  HAL_UART_RxCpltCallback(&huart2);

  int bytesToRead = TOTAL_RAW_BYTES;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  uint16_t data[8];
	  float convertedData[8];
	  // READ DATA
	  read(data);

	  for(int i=0; i<8; i++){
		  int complementTwo = twos_complement(data[i], 16);
		  convertedData[i] = complementTwo*(5*2.5)/(32768*2);
	  }
	  for(int i=0; i<8; i++) {
		  char message_channel[60];
		  snprintf(message_channel, sizeof(message_channel), "%d,%f\n", i+1, convertedData[i]);
		  HAL_UART_Transmit(&huart2, (uint8_t *)message_channel, strlen(message_channel), HAL_MAX_DELAY);
	  }
	  HAL_Delay(3000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
  hspi3.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 0;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 65535;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

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
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_8;
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC7 PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

//callback for usart when receive data
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_UART_Transmit(&huart2, WTxBuf, sizeof(WTxBuf),100);
	HAL_UART_Receive_IT(&huart2, RxBuf, 1);
}

void cmd_check()
{
	char *ptr = strtok(TxBuf, ",");
	// cast to char needed because strcmp accepts signed values, not unsigned
	if (strcmp((char *)ptr, (char *) commd_on) == 0) // equal to zero if match
	{
		// fai cose
		//HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5,GPIO_PIN_SET);
		// if fai cose has success :
		HAL_UART_Transmit(&huart2, (uint8_t *)TxBuf , sizeof(TxBuf),10);  //ritorna il messaggio iniziale
	}
	else if (strcmp((char *)TxBuf, (char *) commd_off) == 0)
	{
		//non serve perchè riceverà solo ON
		//HAL_UART_Transmit(&huart2, (uint8_t *)TxBuf, sizeof(TxBuf),10);
		//HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5,GPIO_PIN_RESET);
	}
	else
	{
		HAL_UART_Transmit(&huart2, (uint8_t *)"WRONG CMD\r\n", sizeof("WRONG CMD\r\n"),10);
	}
}

//receive and store data coming from uart. We receive: topic, payload
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (RxBuf[0] == 13){	// Check if character is enter
		HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", sizeof("\r\n"),10);
		cmd_check();
		memset(TxBuf, '\0', sizeof(TxBuf));  //strcpy(TxBuf, ""); //TxBuf[0] = '\0'; //flush buffer
		char_counter = 0; //set char_counter to zero
	}
	else if(RxBuf[0] == 8){  // Check if character is backspace
		HAL_UART_Transmit(&huart2, (uint8_t *)"\b \b", sizeof("\b \b"),10);
		TxBuf[--char_counter] = NULL; //remove from our buffer previous letter
	}else{	//Otherwise store new character inside our buffer
		TxBuf[char_counter++] = RxBuf[0];
		HAL_UART_Transmit(&huart2, RxBuf, sizeof(RxBuf),10); //Echo single character
	}
	HAL_UART_Receive_IT(&huart2 , RxBuf, 1); // Restart UART in Interrupt mode
}



void parseRawBytes() {
  for(int i = 0; i<(sizeof(parsed) / sizeof(int));i++)
    {
      parsed[i] = (raw[i*2] << 8) + raw[(i*2)+1];
    }
}

void read(int16_t *rawdatabuffer){
	uint16_t value1 = 0;
	uint16_t value2 = 0;

	//pulse convst to start conversion
	HAL_GPIO_WritePin(CONVST_PORT, CONVST_PIN, 0);
	HAL_GPIO_WritePin(CONVST_PORT, CONVST_PIN, 1);

	//set CS low to enable DOUT lines to shift out data
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, 0);

	// Wait until busy goes to 0
	while(HAL_GPIO_ReadPin(BUSY_PORT, BUSY_PIN) == 1){
		//wait
	}

	// read data
	for(int k=0; k<4; k++){
		for(int i=15; i>=0; i--){
			//pulse RD to get data -> should already be pulsed by sclk pin
			HAL_GPIO_WritePin(RD_PORT, RD_PIN, 1);
			HAL_GPIO_WritePin(RD_PORT, RD_PIN, 0);

			value1 += HAL_GPIO_ReadPin(D7_PORT, D7_PIN) << i;
			value2 += HAL_GPIO_ReadPin(D8_PORT, D8_PIN) << i;
		}
		*(rawdatabuffer + k) = value1;
		*(rawdatabuffer + 4 + k) = value2;
	}
	//set cs high again
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, 1);
}

int twos_complement(int num, int bits) {
    // Calculate the mask for the given number of bits
    int mask = (1 << (bits - 1)) - 1;  // Create a mask of all 1s for given number of bits

    // Check if num is negative
    if (num & (1 << (bits - 1))) {
        // Negative number: apply two's complement
        return (num & mask) - (1 << (bits - 1));
    } else {
        // Positive number or zero: two's complement is the number itself
        return num;
    }
}

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
