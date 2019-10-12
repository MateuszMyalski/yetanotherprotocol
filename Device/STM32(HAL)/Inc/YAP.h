/*
 *      Yet Another Protocole using winapi to communicate with hardware via virtual COM using UART.
 *      Library is not using any handshaking signals. Just simple RX and TX link.
 * 
 *      Author: Mateusz Myalski
 * 
 */

#ifndef YAP_h
#define YAP_h

#include <stdint.h>
#include "usart.h"

typedef enum {
	TRANSSMISION_NOT_ENQUIRED,
    RECEIVE_PAYLOAD_ID,
    RECEIVE_PAYLOAD_LENGTH,
    RECEIVING_PAYLOAD,
    PACKET_READY
} YAPStateEnum;

typedef void YAPHandler;
typedef void YAPPacket;

void        YAP_handlerDestroy (void *handler);
YAPHandler 	*YAP_handlerCreate  (UART_HandleTypeDef *uart);

void 		YAP_setAnswearTimeout  (YAPHandler *handler, uint32_t answearTimeout);

void        YAP_packetDestroy       (YAPPacket *packet);
YAPPacket   *YAP_packetCreate       (uint8_t packetID, char *payload);
YAPPacket   *YAP_emptyPacketCreate  (void);

uint16_t    YAP_getAnswearTimeout   (YAPHandler *handler);

uint8_t     YAP_getPacketID         (YAPPacket *packet);
char        *YAP_getPacketPayload   (YAPPacket *packet);

void 	YAP_processByte		(YAPHandler *handler, YAPPacket *emptyPacket, uint8_t *byte);

uint8_t YAP_sendPacket      (YAPHandler *handler, YAPPacket *packet);
uint8_t YAP_receivePacket   (YAPHandler *handler, YAPPacket *packet);
uint8_t YAP_isPacketReady	(YAPPacket *packet);

#endif
