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

#define DEBUG_INFORMATIONS

typedef enum {
    DATA_BITS_5 = 1,
    DATA_BITS_6 = 2,
    DATA_BITS_7 = 4,
    DATA_BITS_8 = 8
} YAPDataSizeEnum;

typedef enum {
    NO_PARITY    = 0,
    ODD_PARITY   = 1,
    EVEN_PARITY  = 2,
    MARK_PARITY  = 3,
    SPACE_PARITY = 4
} YAPParityEnum;

typedef enum {
    ONE_STOP_BIT    = 0,
    ONE5_STOP_BITS  = 1,
    TWO_STOP_BITS   = 2
} YAPStopBitsEnum;

typedef enum {
    BAUD_RATE_110     =  110,
    BAUD_RATE_300     =  300,
    BAUD_RATE_600     =  600,
    BAUD_RATE_1200    =  1200,
    BAUD_RATE_2400    =  2400,
    BAUD_RATE_4800    =  4800,
    BAUD_RATE_9600    =  9600,
    BAUD_RATE_14400   = 14400,
    BAUD_RATE_19200   = 19200,
    BAUD_RATE_38400   = 38400,
    BAUD_RATE_56000   = 56000,
    BAUD_RATE_57600   = 57600,
    BAUD_RATE_115200  = 115200,
    BAUD_RATE_128000  = 128000,
    BAUD_RATE_256000  = 256000
} YAPBaudRateEnum;

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
YAPHandler *YAP_handlerCreate  (uint8_t COMNumber);


void YAP_setBaudRate        (YAPHandler *handler, YAPBaudRateEnum baudRate);
void YAP_setByteSize        (YAPHandler *handler, YAPDataSizeEnum byteSize);
void YAP_setStopBits        (YAPHandler *handler, YAPStopBitsEnum stopBits);
void YAP_setParity          (YAPHandler *handler, YAPParityEnum parity);
void YAP_setAnswerTimeout  (YAPHandler *handler, uint32_t answerTimeout);

void        YAP_packetDestroy       (YAPPacket *packet);
YAPPacket   *YAP_packetCreate       (uint8_t packetID, char *payload);
YAPPacket   *YAP_emptyPacketCreate  (void);

YAPBaudRateEnum YAP_getBaudRate         (YAPHandler *handler);
YAPDataSizeEnum YAP_getByteSize         (YAPHandler *handler);
YAPStopBitsEnum YAP_getStopBits         (YAPHandler *handler);
YAPParityEnum   YAP_getParity           (YAPHandler *handler);
uint32_t        YAP_getAnswerTimeout   (YAPHandler *handler);

uint8_t     YAP_getPacketID             (YAPPacket *packet);
char        *YAP_getPacketPayload       (YAPPacket *packet);

void YAP_sendPacketAsync    (YAPHandler *handler, YAPPacket *packet);
void YAP_receivePacketAsync (YAPHandler *handler, YAPPacket *packet);

void YAP_processByte        (YAPHandler *handler, YAPPacket *emptyPacket, uint8_t *byte);

uint8_t YAP_sendPacket      (YAPHandler *handler, YAPPacket *packet);
uint8_t YAP_receivePacket   (YAPHandler *handler, YAPPacket *packet);
uint8_t YAP_isPacketReady   (YAPPacket *packet);

#endif