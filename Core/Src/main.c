/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/


#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include <stdbool.h>
#include <olifex_fx.h>
#include <olifex_pixel.h>
#include <olifex_serial.h>
#include <olifex_cli.h>


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define NUMBER_OF_PIXELS    	    40
#define PIXEL_ROWS					5
#define PIXEL_COLUMNS				8
#define CELL_SIZE					3

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
DMA_HandleTypeDef hdma_tim3_ch3;


/* USER CODE BEGIN PV */

uint16_t output_array[BUFFER_LENGTH] __attribute__ ((aligned (4)));
pwm_output_struct pixel_out_pwm;
ws2812_rgb_struct pixel_in_rgb;

uint8_t t_startup = 1;
bool frame_tick = false;
fx_config  fx_cfg1;

cli_t  * cli1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

void send_frame();
void test_frame();
void test_pixel();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void init_pwm_output_struct(uint16_t *_array, pwm_output_struct *_init) {

	_init->output_buffer = output_array;
	_init->ptr_start = &_array[ZERO_PADDING];
	_init->length = ((NUMBER_OF_PIXELS * WORDS_PER_PIXEL) + ZERO_PADDING);
	_init->ptr_end = &_init->output_buffer[_init->length];
	_init->frame_status = WAITING;
	_init->cursor = _array;

}

void init_olifex_rgb(UCOL *_array, ws2812_rgb_struct *_init) {
	_init->length = NUMBER_OF_PIXELS;
	_init->ptr_start = _array;
	_init->ptr_end = &_array[NUMBER_OF_PIXELS - 1];
	_init->cursor = _array;
	_init->cursor_up = _array;
	_init->cursor_down = _array;
	_init->frame_direction = UP;
	_init->n_row = PIXEL_ROWS;
	_init->n_col = PIXEL_COLUMNS;

}

void reset_gpio_pwm(TIM_HandleTypeDef *htim) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	if (htim->Instance == TIM3) {
		/* USER CODE BEGIN TIM3_MspPostInit 0 */

		/* USER CODE END TIM3_MspPostInit 0 */

		__HAL_RCC_GPIOC_CLK_ENABLE();
		/**TIM3 GPIO Configuration
		 PC8     ------> TIM3_CH3
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		/* USER CODE BEGIN TIM3_MspPostInit 1 */

		/* USER CODE END TIM3_MspPostInit 1 */
	}

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	memset(&htim3,0,sizeof(htim3));
	memset(&htim4,0,sizeof(htim4));
	memset(&hdma_tim3_ch3,0,sizeof(hdma_tim3_ch3));

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
	MX_DMA_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();

  /* USER CODE BEGIN 2 */






    init_olifex_fx(&fx_cfg1,PIXEL_ROWS,PIXEL_COLUMNS,CELL_SIZE);

    init_olifex_cli(&cli1);

	olifex_cmd_fifo * cmd_fifo1 = olifex_Serial_init();
	cli_register_println((println_func_ptr_t)olifex_Tx_send,cli1);

	memset(cmd_fifo1,0,sizeof(olifex_cmd_fifo));
    memset(output_array, 0, sizeof(output_array));


	init_pwm_output_struct(output_array, &pixel_out_pwm);
	init_olifex_rgb(fx_cfg1.pixel_array, &pixel_in_rgb);



	__HAL_DMA_ENABLE_IT(&hdma_tim3_ch3, DMA_IT_TC);

	reset_gpio_pwm(&htim3);

	if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK) {
		/* Starting Error */
		Error_Handler();
	}


	if (HAL_TIM_Base_Start_IT(&htim4) != HAL_OK) {
		/* Starting Error */
		Error_Handler();
	}

	if(!(cli1->cmd_running==NULL))
			{

		    cli1->cmd_running->func(&pixel_in_rgb,&fx_cfg1);

			}


	if (t_startup == 1) {
		send_frame();
		t_startup = 0;

	}


  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


	while (1) {


		if(cmd_fifo1->cmd_status == CMD_WAITING)
		{
			cli_process(cli1,cmd_fifo1->cmd_pending);
		}


 		if ((hdma_tim3_ch3.State == HAL_DMA_STATE_READY)&&(frame_tick == true)) {

 		if(!(cli1==NULL))
 		{
 			cli1->cmd_running->func(&pixel_in_rgb, &fx_cfg1);
 			write_frame_to_output(&pixel_in_rgb, &pixel_out_pwm);
 			frame_tick = false;
 			send_frame();
 		}

	}

	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLRCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 52;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 10000-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 1000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}


/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
  /* DMA1_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *dma) {

	HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_3);

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
	pixel_out_pwm.frame_status = WAITING;

}

// Callback: timer has rolled over
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	// Check which version of the timer triggered this callback and toggle LED

	if (htim == &htim4) {
    frame_tick = true;
	}
}



// sends a complete frame through dma channel As is.
// output buffer is not modified

void send_frame() {

	//htim3.Init.Period = output_array[0];
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3, (uint32_t*) output_array,
			(sizeof(output_array) / sizeof(uint16_t)));


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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
