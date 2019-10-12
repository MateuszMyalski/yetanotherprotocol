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

#ifndef YAP_internal_h
#define YAP_internal_h

#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <windows.h>
#include "../YAP.h"
#ifdef DEBUG_INFORMATIONS
#include <time.h>
#endif

#define STX	0x02	//< Start of TeXt
#define ETX	0x03	//< End of TeXt
#define EOT	0x04	//< End of Transmission
#define ENQ	0x05	//< ENQuiry
#define ACK	0x06	//< ACKnowledge
#define DLE	0x10	//< Data Link Escape
#define NAK	0x15	//< Negative AcKnowledge

typedef struct {
    HANDLE      PortHandle;
    DCB         SerialParams;
    uint16_t    answerTimeout;
} YAPHandlerInternal;

typedef struct {
   	YAPStateEnum    transsmisionState;
    uint16_t        crc16;
    uint8_t         payloadLength;
    uint8_t		    payloadCounter;
    uint8_t         packetID;
    char            *payload;
} YAPPacketInternal;

void YAP_sendTimeouts           (YAPHandler *handler);
void YAP_receiveACKTimeouts     (YAPHandler *handler);
void YAP_receiveTimeouts        (YAPHandler *handler);

uint8_t YAP_poolForAnswer       (YAPHandler *handler, uint8_t question, uint8_t answer);
uint8_t YAP_sendByte            (YAPHandler *handler, uint8_t data);
uint8_t YAP_receiveByte         (YAPHandler *handler);

uint16_t YAP_crc16              (uint8_t *data, uint16_t size);

#endif