/*
 * olifex_serial.c
 *
 *  Created on: Apr 13, 2022
 *      Author: omwfo
 */

#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

#include "olifex_serial.h"

UART_HandleTypeDef huart4;
DMA_HandleTypeDef  hdma_uart4_rx;
char olifex_rx_buffer[RX_BUFFER_SIZE];
olifex_cmd_fifo ocf;
/**
 * @brief UART4 Initialization Function
 * @param None
 * @retval None
 */
olifex_cmd_fifo  *  olifex_Serial_init() {



	ocf.cmd_status = IDLE;
	huart4.Instance = UART4;
	huart4.Init.BaudRate = 9600;
	huart4.Init.WordLength = UART_WORDLENGTH_8B;
	huart4.Init.StopBits = UART_STOPBITS_1;
	huart4.Init.Parity = UART_PARITY_NONE;
	huart4.Init.Mode = UART_MODE_TX_RX;
	huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart4.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart4) != HAL_OK) {

	}

	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);

	if (HAL_UART_Receive_DMA(&huart4, (uint8_t*) olifex_rx_buffer,
			RX_BUFFER_SIZE) != HAL_OK) {
	}


	memset(&ocf,0,sizeof(olifex_cmd_fifo));
	return &ocf;

	/* USER CODE END UART4_Init 2 */

}

void olifex_Tx_send(char *msg_ptr, uint8_t msg_len) {

	HAL_UART_Transmit (&huart4, (uint8_t *)msg_ptr, msg_len, 10);

}

void olifex_Rx_restart()
{
	__HAL_UART_CLEAR_IDLEFLAG(&huart4);
	//Restart to start DMA transmission of 255 bytes of data at a time
	HAL_UART_Receive_DMA(&huart4, (uint8_t*) olifex_rx_buffer, RX_BUFFER_SIZE);
//	HAL_UARTEx_ReceiveToIdle_DMA(&huart4, (uint8_t*) olifex_rx_buffer, RX_BUFFER_SIZE);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {


	uint16_t n_char = __HAL_DMA_GET_COUNTER(huart->hdmarx);
	static bool command_write;

	//Stop this DMA transmission
	HAL_UART_DMAStop(&huart4);

	if(n_char == RX_BUFFER_SIZE)
    {
        return;
    }

	command_write = !command_write;

    /* Copy and Process new data */
    for(uint8_t i=0;i<(RX_BUFFER_SIZE - n_char); ++i)
    {
    	ocf.cmd_array[command_write][i] = olifex_rx_buffer[i];
    }
    ocf.cmd_pending = &ocf.cmd_array[command_write][0];

    ocf.cmd_status = CMD_WAITING;

    olifex_Rx_restart();




}




char *  get_cmd()
{
	return ocf.cmd_pending;
}





