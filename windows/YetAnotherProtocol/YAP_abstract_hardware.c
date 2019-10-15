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

#include "YAP_internal.h"

uint8_t YAP_sendByte(YAPHandler *handler, uint8_t data) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	unsigned long bytesToWrite = sizeof(uint8_t);
	unsigned long bytesWritten = 0;
	uint8_t status = 0;
	status = WriteFile(
		YAPh->PortHandle,
		&data,
		bytesToWrite,
		&bytesWritten,
		NULL);

	if (!status) {
		#ifdef DEBUG_INFORMATIONS
		printf("[Debug] Error when writting to file.\n");
		#endif
		return 0;
	}
	if (bytesToWrite != bytesWritten) {
		#ifdef DEBUG_INFORMATIONS
		printf("[Debug] Error while sending data.\n");
		#endif
		return 0;
	}
	return 1;
}

void YAP_sendTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	COMMTIMEOUTS timeouts = { 0 };

	GetCommTimeouts(YAPh->PortHandle, &timeouts);
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    SetCommTimeouts(YAPh->PortHandle, &timeouts);
}

void YAP_receiveAnswerTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	COMMTIMEOUTS timeouts = { 0 };

	GetCommTimeouts(YAPh->PortHandle, &timeouts);
	timeouts.ReadIntervalTimeout         = YAPh->answerTimeout;
    timeouts.ReadTotalTimeoutConstant    = 0;
    timeouts.ReadTotalTimeoutMultiplier  = YAPh->answerTimeout;

    SetCommTimeouts(YAPh->PortHandle, &timeouts);
}

void YAP_receiveTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	COMMTIMEOUTS timeouts = { 0 };

	GetCommTimeouts(YAPh->PortHandle, &timeouts);
	timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;

    SetCommTimeouts(YAPh->PortHandle, &timeouts);
}

uint8_t YAP_receiveByte(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;

	unsigned long bytesRead = 0;
	uint8_t receivedByte = 0;

	ReadFile(YAPh->PortHandle,
			 &receivedByte,
			 sizeof(receivedByte),
			 &bytesRead,
			 NULL);

	return receivedByte;
}

uint8_t YAP_waitForByte(YAPHandler *handler, uint8_t byte) {
	while(YAP_receiveByte(handler) != byte)
		;
	return byte;
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
