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

#ifndef YAP_h
#define YAP_h

#include <stdint.h>
#include "usart.h"

typedef enum {
	TRANSSMISION_NOT_ENQUIRED,
    RECEIVE_PAYLOAD_ID,
    RECEIVE_PAYLOAD_LENGTH,
    RECEIVING_PAYLOAD,
    PACKET_READY,
    READY_TO_TRANSSMIT
} YAPStateEnum;

typedef void YAPHandler;
typedef void YAPPacket;

void        YAP_handlerDestroy (void *handler);
YAPHandler 	*YAP_handlerCreate  (UART_HandleTypeDef *uart);

void 		YAP_setAnswerTimeout  (YAPHandler *handler, uint32_t answerTimeout);

void        YAP_packetDestroy       (YAPPacket *packet);
YAPPacket   *YAP_packetCreate       (uint8_t packetID, char *payload);
YAPPacket   *YAP_emptyPacketCreate  (void);

uint32_t    YAP_getAnswerTimeout   (YAPHandler *handler);

uint8_t     YAP_getPacketID         (YAPPacket *packet);
char        *YAP_getPacketPayload   (YAPPacket *packet);

void 	YAP_processByte		(YAPHandler *handler, YAPPacket *emptyPacket, uint8_t *byte);

uint8_t YAP_sendPacket      (YAPHandler *handler, YAPPacket *packet);
uint8_t YAP_receivePacket   (YAPHandler *handler, YAPPacket *packet);
uint8_t YAP_isPacketReady	(YAPPacket *packet);

#endif
