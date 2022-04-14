/*
 * olifex_serial.c
 *
 *  Created on: Apr 13, 2022
 *      Author: omwfo
 */

#include "stdlib.h"

#include "olifex_serial.h"

UART_HandleTypeDef huart4;
DMA_HandleTypeDef  hdma_uart4_rx;
char olifex_rx_buffer[RX_BUFFER_SIZE];


/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
void olifex_serial_init(void)
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
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
//    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */
  __HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);

  /* USER CODE END UART4_Init 2 */

}

void olifex_serial_transmit(char * msg_ptr,uint8_t msg_len)
{
}

void olifex_serial_IDLECallback(UART_HandleTypeDef *huart)
{
	//Stop this DMA transmission
    HAL_UART_DMAStop(&huart4);

    //Calculate the length of the received data
    uint8_t data_length  = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&olifex_rx_buffer);



	//Zero Receiving Buffer
    memset(olifex_rx_buffer,0,RX_BUFFER_SIZE);
    data_length = 0;

    //Restart to start DMA transmission of 255 bytes of data at a time
    HAL_UART_Receive_DMA(&huart4, (uint8_t*)olifex_rx_buffer, 128);
}


