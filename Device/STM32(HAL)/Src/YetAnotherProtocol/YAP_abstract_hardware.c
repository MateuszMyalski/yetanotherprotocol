/*******************************************************************************************************
 *    MIT License                       
 *                      
 *    Copyright (c) 2019 Mateusz Waldemar Myalski                       
 *                      
 *    Permission is hereby granted, free of charge, to any person obtaining a copy                      
 *    of this software and associated documentation files (the "Software"), to deal                     
 *    in the Software without restriction, including without limitation the rights                      
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell                     
 *    copies of the Software, and to permit persons to whom the Software is                     
 *    furnished to do so, subject to the following conditions:                      
 *                      
 *    The above copyright notice and this permission notice shall be included in all                        
 *    copies or substantial portions of the Software.                       
 *                      
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR                        
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                      
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE                       
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                        
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,                     
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE                     
 *    SOFTWARE.                     
 *                                       
 *******************************************************************************************************/

/**
 * @file YAP_abstract_hardware.c
 * @author Mateusz Waldemar Myalski
 * @date 8 Oct 2019
 * @brief File contain abstract layer for hardware operations
 */

#include "YetAnotherProtocol/YAP_internal.h"

uint8_t YAP_sendByte(YAPHandler *handler, uint8_t data) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;

	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_UART_Transmit(YAPh->huart, &data, sizeof(uint8_t), 50);
	if (status != HAL_OK){
		Error_Handler();
		return 0;
	}

	return 1;
}

uint8_t YAP_receiveByte(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	uint8_t receivedByte = 0;
	HAL_StatusTypeDef status = HAL_OK;
	HAL_UART_AbortReceive_IT(YAPh->huart);
	status = HAL_UART_Receive(YAPh->huart, &receivedByte, sizeof(uint8_t), YAPh->selectedTimeout);
	if (status != HAL_OK){
		Error_Handler();
		return 0;
	}
	return receivedByte;
}


void YAP_receiveAnswerTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	YAPh->selectedTimeout = YAPh->answerTimeout;
}


void YAP_receiveTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	YAPh->selectedTimeout = YAPh->receiveTimeout;
}


uint8_t YAP_poolForAnswer(YAPHandler *handler, uint8_t question, uint8_t answer) {
	char receivedFlag = 0;

	YAP_receiveAnswerTimeouts(handler);

	YAP_sendByte(handler, question);
	receivedFlag = YAP_receiveByte(handler);

	YAP_receiveTimeouts(handler);

	if(receivedFlag == answer)
		return 1;
	else
		return 0;
}

uint16_t YAP_crc16(uint8_t *data, uint16_t size) {
  int i, j;
  uint16_t crc = 0x1d0f;
  for (i = 0; i < size; i++) {
    uint16_t xr = data[i] << 8;
    crc = crc ^ xr;

    for (j = 0; j < 8; j++) {
      if (crc & 0x8000) {
        crc = (crc << 1);
        crc = crc ^ 0x1021;
      }
      else {
        crc = crc << 1;
      }
    }
  }
  crc = crc & 0xFFFF;
  return crc;
}
