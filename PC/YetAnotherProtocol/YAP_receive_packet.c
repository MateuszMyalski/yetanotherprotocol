#include "YAP_internal.h"

uint8_t YAP_receivePacket(YAPHandler *handler, YAPPacket *packet) {
	YAPPacketInternal 	*YAPp = (YAPPacketInternal *)packet;
	YAPHandlerInternal	*YAPh = (YAPHandlerInternal *)handler;

	/* Clear pending data in buffer */
	PurgeComm(YAPh->PortHandle, PURGE_RXCLEAR);

	while (YAPp->transsmisionState != PACKET_READY) {
		uint8_t byte = YAP_receiveByte(YAPh);
		YAP_processByte(YAPh, YAPp, &byte);
	}

	/* Check payload length */
	if (YAPp->payloadCounter != YAPp->payloadLength) {
		YAP_sendByte(handler, NAK);
		YAPp->transsmisionState = TRANSSMISION_NOT_ENQUIRED;
		return 0;
	}

	/* Check data integrity - last 2 bytes is CRC*/
	YAPp->crc16 = YAPp->payload[YAPp->payloadLength - 1]
			     | YAPp->payload[YAPp->payloadLength - 2] << 8;
	uint16_t crc16_generated = YAP_crc16((uint8_t *)YAPp->payload, YAPp->payloadLength - 2);

	/* Cut off CRC from payload */
	YAPp->payload[YAPp->payloadLength - 2] = '\0';

	if (crc16_generated != YAPp->crc16) {
		YAP_sendByte(handler, NAK);
		YAPp->transsmisionState = TRANSSMISION_NOT_ENQUIRED;
		return 0;
	}

	YAP_sendByte(handler, ACK);
	YAPp->transsmisionState = TRANSSMISION_NOT_ENQUIRED;
	return 1;
	
}

void YAP_receivePacketAsync(YAPHandler *handler, YAPPacket *packet) {
	//TODO implement
}