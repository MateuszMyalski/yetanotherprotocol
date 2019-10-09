#include "YetAnotherProtocol/YAP_internal.h"

uint8_t YAP_sendPacket(YAPHandler *handler, YAPPacket *packet) {
	YAPPacketInternal *YAPp = (YAPPacketInternal *)packet;

	// Maximum payload is 250 bytes. This is the fixed size for every device using YAP
	if (YAPp->payloadLength > 250)
		return 0;

	/* Prepare transmission */
	YAPp->AsyncState = ENQUIRING_TRANSMISSION;
	if(!YAP_poolForAnswear(handler, ENQ, ACK))
		return 0;

	/* Send payload */
	uint8_t tries = 0;
	do {
		YAPp->AsyncState = SENDING_PAYLOAD;

		/* Prepare device for receiving the data */
		if(!YAP_poolForAnswear(handler, STX, ACK))
			return 0;

		/* Send settings frame and payload length */
		YAP_sendByte(handler, YAPp->packetID);
		YAP_sendByte(handler, YAPp->payloadLength);

		/* Send payload */
		uint8_t charCounter;
				charCounter = 0;
		while (YAPp->payloadLength != charCounter) {
			if (!(YAPp->payload[charCounter] >= 0x20 && YAPp->payload[charCounter] <= 0x7E))
				YAP_sendByte(handler, DLE);

			YAP_sendByte(handler, YAPp->payload[charCounter]);

			++charCounter;
		}
		// TODO add CRC
		++tries;
	} while (!YAP_poolForAnswear(handler, ETX, ACK) && tries < 4);

	if (tries >= 3)
		return 0;

	/* Finish transmission */
	YAPp->AsyncState = FINISHING_TRANSMISSION;
	return YAP_poolForAnswear(handler, EOT, ACK);
}

void YAP_sendPacketAsync(YAPHandler *handler, YAPPacket *packet) {
	//TODO implement
}
