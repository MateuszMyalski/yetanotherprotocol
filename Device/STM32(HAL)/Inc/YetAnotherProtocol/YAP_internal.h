#ifndef YAP_internal_h
#define YAP_internal_h
/*
 * PACKET STRUCTURE:
 *
 * 		DLE
 * 		STX
 *		Information Field
 * 		DLE
 * 		STX
 *
 * INFORMATION FIELD:
 *
 * 		Packet ID
 * 		Packet Size (Payload size to receive)
 * 		Data
 * 		.
 * 		.
 *
 * COMMAND FIELD:
 *
 * 		0-255 (Command number)
 *
 * DATA:
 *
 * 		(Max 250 bytes)
 *
 */


#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "usart.h"
#include "../YAP.h"

#define STX	0x02	//< Start of TeXt
#define ETX	0x03	//< End of TeXt
#define EOT	0x04	//< End of Transmission
#define ENQ	0x05	//< ENQuiry
#define ACK	0x06	//< ACKnowledge
#define DLE	0x10	//< Data Link Escape
#define NAK	0x15	//< Negative AcKnowledge

typedef struct {
    UART_HandleTypeDef      *huart;
    uint32_t    			answearTimeout;
    uint32_t				receiveTimeout;
    uint32_t				selectedTimeout;
} YAPHandlerInternal;

typedef struct {
   	YAPStateEnum		transsmisionState;
    char                *payload;
    uint16_t             crc16;
    uint8_t             payloadLength;
    uint8_t				payloadCounter;
    uint8_t             packetID;
} YAPPacketInternal;

void YAP_sendTimeouts           (YAPHandler *handler);
void YAP_receiveACKTimeouts     (YAPHandler *handler);
void YAP_receiveTimeouts        (YAPHandler *handler);

uint8_t YAP_poolForAnswear      (YAPHandler *handler, uint8_t question, uint8_t answear);
uint8_t YAP_sendByte            (YAPHandler *handler, uint8_t data);
uint8_t YAP_receiveByte         (YAPHandler *handler);

uint16_t YAP_crc16(uint8_t *data, uint16_t size);

#endif
