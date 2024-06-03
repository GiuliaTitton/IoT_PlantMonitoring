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
#include "ad7606.h"
#include "stm32_gpio.h"
#include "stm32_spi.h"
#include "no_os_spi.h"

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

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

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
	//------------------------ GPIO INITIALIZATION --------------------------
	//-------- RESET PIN ----------
	//struct stm32_gpio_desc *stm32_reset_desc;
	struct no_os_gpio_init_param gpio_reset = { .port = 1,		// GPIO: PB9
			.number = GPIO_PIN_9,              // GPIO pin number for RESET
			.platform_ops = &stm32_gpio_ops, // Platform-specific operations (set to NULL if not needed)
			.extra = NULL             // Extra parameters if needed
			};
	struct no_os_gpio_desc gpio_reset_desc = { .port = 1, .number = GPIO_PIN_8,
			.platform_ops = &stm32_gpio_ops, .extra = NULL };

	//---------------------------------- CONVST PIN --------------------------------------------
	struct stm32_gpio_init_param stm32_convst_init_param = {
			/** Output mode */
			.mode = GPIO_MODE_OUTPUT_PP,
			.speed = GPIO_SPEED_FREQ_HIGH,
			.alternate = 0
		};
	struct stm32_gpio_desc stm32_convst_desc = {
			.port = GPIOC,
			.gpio_config = {
					.Pin = GPIO_PIN_7,
					.Mode = GPIO_MODE_OUTPUT_PP,
					.Speed = GPIO_SPEED_FREQ_HIGH,
					.Alternate = 0,
			}
	};

	struct no_os_gpio_init_param gpio_convst = { .port = 2,		// GPIO: PC7
			.number = GPIO_PIN_7,              // GPIO pin number for CONVST
			.platform_ops = &stm32_gpio_ops, // Platform-specific operations (set to NULL if not needed)
			.extra = &stm32_convst_desc,             // Extra parameters if needed
			.extra = &stm32_convst_init_param
			};
	struct no_os_gpio_desc gpio_convst_desc = {
			.port = 2,
			.number = GPIO_PIN_7,
			.platform_ops = &stm32_gpio_ops,
			.extra = &stm32_convst_desc,
			.extra = &stm32_convst_init_param
	};

	//----------------------------------- BUSY PIN -------------------------------------------------
	struct stm32_gpio_desc stm32_busy_desc = {
			.port = GPIOA,
			.gpio_config = {
					.Pin = GPIO_PIN_8,
					.Mode = GPIO_MODE_OUTPUT_PP,
					.Speed = GPIO_SPEED_FREQ_HIGH,
					.Alternate = 0,
			}
	};
	struct stm32_gpio_init_param stm32_busy_init_param = {
				/** Output mode */
				.mode = GPIO_MODE_OUTPUT_PP,
				.speed = GPIO_SPEED_FREQ_HIGH,
				.alternate = 0
			};
	struct no_os_gpio_init_param gpio_busy = { .port = 0,		// GPIO: PA8
			.number = GPIO_PIN_8,              // GPIO pin number for BUSY
			.platform_ops = &stm32_gpio_ops, // Platform-specific operations (set to NULL if not needed)
			.extra = &stm32_busy_desc,             // Extra parameters if needed
			.extra = &stm32_busy_init_param
			};
	struct no_os_gpio_desc gpio_busy_desc = {
			.port = 0,
			.number = GPIO_PIN_8,
			.platform_ops = &stm32_gpio_ops,
			.extra = &stm32_busy_desc,
			.extra = &stm32_busy_init_param
	};

	//------ STBY_N PIN ---------
	//struct stm32_gpio_desc *stm32_stdby_n_desc;
	struct no_os_gpio_init_param gpio_stby_n = { .port = 0,	// GPIO: PA12
			.number = GPIO_PIN_12,              // GPIO pin number for STBYn
			.platform_ops = &stm32_gpio_ops, // Platform-specific operations (set to NULL if not needed)
			.extra = NULL             // Extra parameters if needed
			};
	struct no_os_gpio_desc gpio_stby_n_desc = { .port = 0,
			.number = GPIO_PIN_12, .platform_ops = &stm32_gpio_ops, .extra = NULL };
	//------ RANGE PIN -------
	//struct stm32_gpio_desc *stm32_range_desc;
	struct no_os_gpio_init_param gpio_range = { .port = 0,	// GPIO: PA10
			.number = GPIO_PIN_10,              // GPIO pin number for RANGE
			.platform_ops = &stm32_gpio_ops, // Platform-specific operations (set to NULL if not needed)
			.extra = NULL             // Extra parameters if needed
			};
	struct no_os_gpio_desc gpio_range_desc = { .port = 0, .number = GPIO_PIN_10,
			.platform_ops = &stm32_gpio_ops, .extra = NULL };
	//----- OS0 PIN -----
	//struct stm32_gpio_desc *stm32_os0_desc;
	struct no_os_gpio_init_param gpio_os0 = { .port = 3,		// GPIO: PD2
			.number = GPIO_PIN_2,              // GPIO pin number for OS0
			.platform_ops = &stm32_gpio_ops, // Platform-specific operations (set to NULL if not needed)
			.extra = NULL             // Extra parameters if needed
			};
	struct no_os_gpio_desc gpio_os0_desc = { .port = 3, .number = GPIO_PIN_2,
			.platform_ops = &stm32_gpio_ops, .extra = NULL };
	//----OS1 PIN -----
	//struct stm32_gpio_desc *stm32_os1_desc;
	struct no_os_gpio_init_param gpio_os1 = { .port = 1,		// GPIO: PB5
			.number = GPIO_PIN_5,              // GPIO pin number for OS1
			.platform_ops = &stm32_gpio_ops, // Platform-specific operations (set to NULL if not needed)
			.extra = NULL             // Extra parameters if needed
			};
	struct no_os_gpio_desc gpio_os1_desc = { .port = 1, .number = GPIO_PIN_5,
			.platform_ops = &stm32_gpio_ops, .extra = NULL };
	//------- OS2 PIN ---
	//struct stm32_gpio_desc *stm32_os2_desc;
	struct no_os_gpio_init_param gpio_os2 = { .port = 0,		// GPIO: PA11
			.number = GPIO_PIN_11,              // GPIO pin number for OS2
			.platform_ops = &stm32_gpio_ops, // Platform-specific operations (set to NULL if not needed)
			.extra = NULL             // Extra parameters if needed
			};
	struct no_os_gpio_desc gpio_os2_desc = { .port = 0, .number = GPIO_PIN_11,
			.platform_ops = &stm32_gpio_ops, .extra = NULL };
	//--------- PAR_SER PIN ----
	//struct stm32_gpio_desc *stm32_par_ser_desc;
	struct no_os_gpio_init_param gpio_par_ser = { .port = 1,	// GPIO: PB6
			.number = GPIO_PIN_6,              // GPIO pin number for PARn/SER
			.platform_ops = &stm32_gpio_ops, // Platform-specific operations (set to NULL if not needed)
			.extra = NULL             // Extra parameters if needed
			};
	struct no_os_gpio_desc gpio_par_ser_desc =
			{ .port = 1, .number = GPIO_PIN_6, .platform_ops = &stm32_gpio_ops,
					.extra = NULL };

	// -------------- SPI INITIALIZATION ---------------
	struct no_os_platform_spi_delays platform_spi_delays = {
			.cs_delay_first = 1, .cs_delay_last = 1 };
	struct stm32_spi_init_param stm32_spi_init_param = {
			.chip_select_port = 1,
			.get_input_clock = HAL_RCC_GetPCLK2Freq,
	};
	struct no_os_spi_init_param spi_init_param = {
			.device_id = ID_AD7606_8,
			.max_speed_hz = 10000000,
			.chip_select = &gpio_par_ser.port,   //DA CONTROLLARE
			.mode = NO_OS_SPI_MODE_0,
			.bit_order = NO_OS_SPI_BIT_ORDER_MSB_FIRST,
			.platform_ops =	&stm32_spi_ops,
			.platform_delays = platform_spi_delays,
			.extra = &stm32_spi_init_param
	};
	struct no_os_spibus_desc spibus_desc = {
			.device_id = ID_AD7606_8,
			.max_speed_hz = 10000000,
			.mode = NO_OS_SPI_MODE_0,
			.bit_order = NO_OS_SPI_BIT_ORDER_MSB_FIRST,
			.platform_ops = &stm32_spi_ops,
	};
	struct stm32_spi_desc stm32_spi_desc = {
			.hspi = &hspi3,
			.input_clock = 10000000,
			.chip_select = &gpio_par_ser_desc,
	};
	struct no_os_spi_desc spi_desc = {
			.bus = &spibus_desc,
			.device_id = ID_AD7606_8,
			.max_speed_hz = 10000000,
			.chip_select = &gpio_par_ser.port,
			.mode = NO_OS_SPI_MODE_0,
			.bit_order = NO_OS_SPI_BIT_ORDER_MSB_FIRST,
			.platform_ops = &stm32_spi_ops,
			.platform_delays = platform_spi_delays,
			.extra = &stm32_spi_desc,
			.extra = &stm32_spi_init_param
	};

	//spi_desc->extra = &stm32_spi_desc;

	//------------ DEVICE AD7606 -------------
	struct ad7606_config ad7606_config = { .op_mode = AD7606_NORMAL,
			.dout_format = AD7606_1_DOUT, .ext_os_clock = false,
			.status_header = true };
	struct ad7606_range ad7606_range = { .min = -5000, .max = 5000,
			.differential = false };
	struct ad7606_oversampling ad7606_oversampling = { .os_pad = 4, .os_ratio =
			4 };
	struct ad7606_digital_diag *ad7606_digital_diag;
	uint8_t data[28];
	struct ad7606_dev dev = {
			.spi_desc = &spi_desc,
			.gpio_reset = &gpio_reset_desc, .gpio_convst = &gpio_convst_desc, .gpio_busy =
			&gpio_busy_desc, .gpio_stby_n = &gpio_stby_n_desc, .gpio_range =
			&gpio_range_desc, .gpio_os0 = &gpio_os0_desc, .gpio_os1 =
			&gpio_os1_desc, .gpio_os2 = &gpio_os2_desc, .gpio_par_ser =
			&gpio_par_ser_desc, .device_id = ID_AD7606_8, .oversampling =
			ad7606_oversampling,
			.sw_mode = false, // Whether the device is running in hardware or software mode: false=hardware mode ????
			.reg_mode = false, // Whether the device is running in register or ADC reading mode: false = ADC reading mode ????
			.max_dout_lines = AD7606_1_DOUT, .config = ad7606_config,
			.digital_diag_enable = ad7606_digital_diag, .num_channels = 8,
			.range_ch = ad7606_range, .data = data //Data buffer (used internally by the SPI communication functions)
			};

	struct ad7606_init_param ad7606_init_param = {
			.spi_init = spi_init_param,
			.gpio_reset = &gpio_reset, .gpio_convst = &gpio_convst, .gpio_busy =
					&gpio_busy, .gpio_stby_n = &gpio_stby_n, .gpio_range =
					&gpio_range, .gpio_os0 = &gpio_os0, .gpio_os1 = &gpio_os1,
			.gpio_os2 = &gpio_os2, .gpio_par_ser = &gpio_par_ser, .device_id =
					ID_AD7606_8, .oversampling = ad7606_oversampling, .sw_mode =
					false, .config = ad7606_config, .digital_diag_enable =
					ad7606_digital_diag, .range_ch = ad7606_range };

	//initialization of AD7606 device
	int32_t ret_init = ad7606_init(&dev, &ad7606_init_param);
	char message_init[50];
	if (ret_init == 0) {
		snprintf(message_init, sizeof(message_init), "INITIALIZATION of AD7606 successful");
		HAL_UART_Transmit(&huart2, (uint8_t*)message_init, strlen(message_init), HAL_MAX_DELAY);
		printf("INITIALIZATION of AD7606 successful");
	} else {
		printf("ERROR in initialization of AD7606");
		return;
	}
	// array to store data from conversion
	uint32_t data_read[8];
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */
		// read data!!!
		int32_t ret_data = ad7606_read(&dev, data_read);
		if(ret_data == 0){
			printf("Data acquired: ");
			for(int i=0; i<8; i++){
				printf("%d, ", data_read[i]);
			}
			printf("\n");
		}
		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	RCC_OscInitStruct.PLL.PLLN = 60;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief SPI3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI3_Init(void) {

	/* USER CODE BEGIN SPI3_Init 0 */

	/* USER CODE END SPI3_Init 0 */

	/* USER CODE BEGIN SPI3_Init 1 */

	/* USER CODE END SPI3_Init 1 */
	/* SPI3 parameter configuration*/
	hspi3.Instance = SPI3;
	hspi3.Init.Mode = SPI_MODE_MASTER;
	hspi3.Init.Direction = SPI_DIRECTION_1LINE;
	hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi3.Init.NSS = SPI_NSS_SOFT;
	hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi3.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi3) != HAL_OK) {
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
static void MX_TIM10_Init(void) {

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
	if (HAL_TIM_Base_Init(&htim10) != HAL_OK) {
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
static void MX_USART2_UART_Init(void) {

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
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
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
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA,
			LD2_Pin | GPIO_PIN_8 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_9,
			GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LD2_Pin PA8 PA10 PA11
	 PA12 */
	GPIO_InitStruct.Pin = LD2_Pin | GPIO_PIN_8 | GPIO_PIN_10 | GPIO_PIN_11
			| GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PC7 */
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : PD2 */
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : PB5 PB6 PB9 */
	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
