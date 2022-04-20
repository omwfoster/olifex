/*
 * olifex_serial.h
 *
 *  Created on: Apr 13, 2022
 *      Author: omwfo
 */

#ifndef INC_OLIFEX_SERIAL_H_
#define INC_OLIFEX_SERIAL_H_

#include "stm32f4xx_hal.h"
#include "stdlib.h"

#define RX_BUFFER_SIZE 128

typedef enum rw1{
	READING,
	WRITING,
	IDLE,
	CMD_WAITING
}RW_STATUS;

typedef struct olifex_cmd_fifo{
	char   cmd_array[2][8];
	char * cmd_pending;
	RW_STATUS    cmd_status;
}olifex_cmd_fifo;



olifex_cmd_fifo  *  olifex_Serial_init();
void olifex_Tx_send(char * msg_ptr,uint8_t msg_len);
void olifex_Rx_restart();
void olifex_Rx_IDLECallback();



#endif /* INC_OLIFEX_SERIAL_H_ */
