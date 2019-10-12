#include "YetAnotherProtocol/YAP_internal.h"

YAPHandler *YAP_handlerCreate(UART_HandleTypeDef *huart) {
    YAPHandlerInternal  *YAPh       = (YAPHandlerInternal *)malloc((sizeof *YAPh));

    /* Prepare COM handler for HAL */
    YAPh->huart = huart;

    /* Set default timeouts */
    YAPh->answearTimeout = 0;
    YAPh->receiveTimeout = 50;
    YAPh->selectedTimeout = YAPh->receiveTimeout;

    return YAPh;
}

void YAP_handlerDestroy(YAPHandler *handler) {
    free(handler);
}

void YAP_setAnswearTimeout(YAPHandler *handler, uint32_t answearTimeout) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    tempHandler->answearTimeout = answearTimeout;
}

uint16_t YAP_getAnswearTimeout(YAPHandler *handler) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    return tempHandler->answearTimeout;
}

YAPPacket *YAP_packetCreate(uint8_t packetID, char *payload) {
    YAPPacketInternal *YAPp   = (YAPPacketInternal *) malloc((sizeof *YAPp));
    YAPp->packetID            = packetID;
    YAPp->payload             = payload;
    YAPp->payloadLength       = strlen(payload) + 2;
    YAPp->crc16				  = YAP_crc16((uint8_t *)YAPp->payload, YAPp->payloadLength - 2);
    return YAPp;
}

YAPPacket *YAP_emptyPacketCreate(void) {
    YAPPacketInternal *YAPp   = (YAPPacketInternal *) malloc((sizeof *YAPp));
    YAPp->transsmisionState = TRANSSMISION_NOT_ENQUIRED;
	YAPp->crc16 			= 0;
	YAPp->packetID 			= 0;
	YAPp->payloadLength		= 0;
	YAPp->payloadCounter	= 0;
	YAPp->payload			= NULL;

    return YAPp;
}

void YAP_packetDestroy(YAPPacket *packet) {
    YAPPacketInternal *YAPp = (YAPPacketInternal *)packet;
    free(YAPp->payload);
    free(packet);
}

uint8_t YAP_getPacketID(YAPPacket *packet) {
    YAPPacketInternal *YAPp = (YAPPacketInternal *)packet;
    return YAPp->packetID;
}

char *YAP_getPacketPayload(YAPPacket *packet) {
    YAPPacketInternal *YAPp = (YAPPacketInternal *)packet;
    return YAPp->payload;
}

uint8_t YAP_isPacketReady(YAPPacket *packet) {
	YAPPacketInternal *YAPp = (YAPPacketInternal *)packet;
	if (YAPp->transsmisionState == PACKET_READY)
		return 1;
	else
		return 0;
}

