/*
 * olifex_serial.h
 *
 *  Created on: Apr 13, 2022
 *      Author: omwfo
 */

#ifndef INC_OLIFEX_SERIAL_H_
#define INC_OLIFEX_SERIAL_H_

#include "stm32f4xx_hal.h"


void olifex_serial_init(void);
void olifex_serial_transmit(char * msg_ptr,uint8_t msg_len);



#endif /* INC_OLIFEX_SERIAL_H_ */
