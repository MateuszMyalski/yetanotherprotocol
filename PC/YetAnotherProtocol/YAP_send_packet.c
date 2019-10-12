#include "YAP_internal.h"

uint8_t YAP_sendPacket(YAPHandler *handler, YAPPacket *packet) {
	YAPPacketInternal *YAPp = (YAPPacketInternal *)packet;

	// Maximum payload is 250 bytes. This is the fixed size for every device using YAP
	if (YAPp->payloadLength > 250)
		return 0;

	#ifdef DEBUG_INFORMATIONS
	printf("[I] Enquiring transmission.\n");
	#endif

	YAP_sendByte(handler, ENQ);
	if (!(YAP_receiveByte(handler) == ACK))
		return 0;

	#ifdef DEBUG_INFORMATIONS
	printf("[I] Transsmision enquired.\n");
	printf("[I] Sending packet ID(%d).\n", YAPp->packetID);
	#endif

	if (!YAP_poolForAnswear(handler, YAPp->packetID, ACK))
		return 0;
	
	#ifdef DEBUG_INFORMATIONS
	printf("[I] Sending packet length(%d).\n", YAPp->payloadLength);
	#endif

	if (!YAP_poolForAnswear(handler, YAPp->payloadLength, ACK))
		return 0;

	#ifdef DEBUG_INFORMATIONS
	printf("[I] Sending packet payload(%d).\n{ ", YAPp->payloadLength);
	#endif

	uint8_t charCounter = 0;
	while(charCounter != (YAPp->payloadLength - 2)) {
		YAP_sendByte(handler, YAPp->payload[charCounter]);
		#ifdef DEBUG_INFORMATIONS
		printf("%c[%d], ", (char)YAPp->payload[charCounter], YAPp->payload[charCounter]);
		#endif
		++charCounter;
	}

	#ifdef DEBUG_INFORMATIONS
	printf("}\n[I] Finishing transsmision.\n");
	#endif

	#ifdef DEBUG_INFORMATIONS
	printf("[I] Sending CRC(0x%x)\n", YAPp->crc16);
	#endif
	YAP_sendByte(handler, (uint8_t)(YAPp->crc16 >> 8));
	YAP_sendByte(handler, (uint8_t)(YAPp->crc16));

	if (YAP_poolForAnswear(handler, EOT, ACK)){
		#ifdef DEBUG_INFORMATIONS
		printf("[I] Succesfull transsmision.\n");
		#endif
		return 1;
	}else {
		#ifdef DEBUG_INFORMATIONS
		printf("[I] Unsuccesfull transsmision.\n");
		#endif
		return 0;
	}
}

void YAP_sendPacketAsync(YAPHandler *handler, YAPPacket *packet) {
	//TODO implement
}
