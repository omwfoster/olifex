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

typedef struct olifex_cmd_fifo{
	char command_array[2][8];
	char * command_pending;
}olifex_cmd_fifo;

void olifex_serial_init(void);
void olifex_serial_transmit(char * msg_ptr,uint8_t msg_len);
void olifex_serial_IDLECallback();



#endif /* INC_OLIFEX_SERIAL_H_ */
