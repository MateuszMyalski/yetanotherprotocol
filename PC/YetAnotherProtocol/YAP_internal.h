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
    uint16_t    answearTimeout;
} YAPHandlerInternal;

typedef struct {
    YAPAsyncStateEnum   AsyncState;
    HANDLE              ThreadHandle;
    char                *payload;
    uint8_t             payloadLength;
    uint8_t             packetID;
    uint8_t             crc16;
} YAPPacketInternal;

void YAP_sendTimeouts           (YAPHandler *handler);
void YAP_receiveACKTimeouts     (YAPHandler *handler);
void YAP_receiveTimeouts        (YAPHandler *handler);

uint8_t YAP_poolForAnswear      (YAPHandler *handler, uint8_t question, uint8_t answear);
uint8_t YAP_sendByte            (YAPHandler *handler, char data);
uint8_t YAP_receiveByte         (YAPHandler *handler);

#endif