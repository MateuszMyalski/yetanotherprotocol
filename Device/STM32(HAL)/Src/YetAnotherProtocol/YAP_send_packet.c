#include "YetAnotherProtocol/YAP_internal.h"

uint8_t YAP_sendPacket(YAPHandler *handler, YAPPacket *packet) {
	YAPPacketInternal *YAPp = (YAPPacketInternal *)packet;

	// Maximum payload is 250 bytes. This is the fixed size for every device using YAP
	if (YAPp->payloadLength > 250)
		return 0;

//	YAP_sendByte(handler, ENQ);
//	if (!(YAP_receiveByte(handler) == ACK))
//		return 0;

	if (!YAP_poolForAnswear(handler, ENQ, ACK))
		return 0;

	if (!YAP_poolForAnswear(handler, YAPp->packetID, ACK))
		return 0;

	if (!YAP_poolForAnswear(handler, YAPp->payloadLength, ACK))
		return 0;

	uint8_t charCounter = 0;
	while(charCounter != (YAPp->payloadLength - 2)) {
		YAP_sendByte(handler, YAPp->payload[charCounter]);
		++charCounter;
	}

	YAP_sendByte(handler, (uint8_t)(YAPp->crc16 >> 8));
	YAP_sendByte(handler, (uint8_t)(YAPp->crc16));

	if (YAP_poolForAnswear(handler, EOT, ACK))
		return 1;
	else
		return 0;
}
