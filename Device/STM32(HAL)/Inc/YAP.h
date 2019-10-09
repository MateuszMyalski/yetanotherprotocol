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
    WAIT_FOR_ENQ,
    WAIT_FOR_STX,
    WAIT_FOR_ETX,
    WAIT_FOR_EOT,
    ENQUIRING_TRANSMISSION,
    FINISHING_TRANSMISSION,
    RECEIVING_PAYLOAD,
    SENDING_PAYLOAD,
    PAYLOAD_CHECK,
    PAYLOAD_CHECK_FATAL,
    PAYLOAD_CHECK_SUCCESS,
    PAYLOAD_READY,
} YAPAsyncStateEnum;

typedef void YAPHandler;
typedef void YAPPacket;

void        YAP_handlerDestroy (void *handler);
YAPHandler 	*YAP_handlerCreate  (UART_HandleTypeDef *uart);

void 		YAP_setAnswearTimeout  (YAPHandler *handler, uint16_t answearTimeout);

void        YAP_packetDestroy       (YAPPacket *packet);
YAPPacket   *YAP_packetCreate       (uint8_t packetID, char *payload);
YAPPacket   *YAP_emptyPacketCreate  (void);

uint16_t    YAP_getAnswearTimeout   (YAPHandler *handler);

uint8_t     YAP_getPacketID         (YAPPacket *packet);
char        *YAP_getPacketPayload   (YAPPacket *packet);

void YAP_sendPacketAsync    (YAPHandler *handler, YAPPacket *packet);
void YAP_receivePacketAsync (YAPHandler *handler, YAPPacket *packet);

uint8_t YAP_sendPacket      (YAPHandler *handler, YAPPacket *packet);
uint8_t YAP_receivePacket   (YAPHandler *handler, YAPPacket *packet, uint8_t *transsmisionEnquiry);

#endif
