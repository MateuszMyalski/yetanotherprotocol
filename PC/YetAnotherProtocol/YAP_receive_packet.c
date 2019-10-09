#include "YAP_internal.h"

uint8_t YAP_receivePacket(YAPHandler *handler, YAPPacket *packet, uint8_t *transsmisionEnquiry) {
	YAPPacketInternal 	*YAPp = (YAPPacketInternal *)packet;
	YAPHandlerInternal	*YAPh = (YAPHandlerInternal *)handler;

	/* Clear pending data in buffer */
	PurgeComm(YAPh->PortHandle, PURGE_RXCLEAR);

	if(*transsmisionEnquiry != ENQ)
		return 0;

	*transsmisionEnquiry = 0;
	YAP_sendByte(handler, ACK);

	uint8_t tries = 0;
	uint8_t payloadCounter;
	do {
		/* Prepare transmission */
		payloadCounter = 0;
		YAPp->AsyncState = WAIT_FOR_STX;
		if(!YAP_poolForAnswear(handler, 0, STX))
			return 0;
		YAP_sendByte(handler, ACK);

		/* Prepare payload type data */
		YAPp->packetID 		= YAP_receiveByte(handler);
		YAPp->payloadLength = YAP_receiveByte(handler);
		
		#ifdef DEBUG_INFORMATIONS
		printf("PACKET ID: %d\n", YAPp->packetID);
		printf("PAYLOAD LEN: %d\n", YAPp->payloadLength);
		#endif

		YAPp->payload = (char *)malloc((sizeof *(YAPp->payload)) * YAPp->payloadLength);
		
		/* Receive payload */
		YAPp->AsyncState 		= RECEIVING_PAYLOAD;
		char 	receivedChar 	= 0;
		while (payloadCounter != YAPp->payloadLength) {
			receivedChar = (char)YAP_receiveByte(handler);
			if (receivedChar == DLE)
				continue;
			
			if (receivedChar == ETX)
				break;

			#ifdef DEBUG_INFORMATIONS
			printf("Received byte: %d[%c]\n", (uint8_t)receivedChar, receivedChar);
			#endif
			YAPp->payload[payloadCounter] = receivedChar;
			payloadCounter++;
		}

		YAPp->AsyncState = WAIT_FOR_ETX;
		if(!YAP_poolForAnswear(handler, 0, ETX)) {
			free(YAPp->payload);
			return 0;
		}

		/* Check payload */
		if (payloadCounter == YAPp->payloadLength){
			YAP_sendByte(handler, ACK);
		}else {
			YAP_sendByte(handler, NAK);
			free(YAPp->payload);
			YAPp->AsyncState = PAYLOAD_CHECK_FATAL;
			++tries;
		}

	} while (payloadCounter != YAPp->payloadLength && tries < 4);

	if(YAPp->AsyncState != PAYLOAD_CHECK_FATAL) {
		YAPp->AsyncState = PAYLOAD_CHECK_SUCCESS;
		YAPp->AsyncState = WAIT_FOR_EOT;
	}

	/* Finish transmission */
	if(YAP_poolForAnswear(handler, 0, EOT))
		YAP_sendByte(handler, ACK);
	else
		return 0;
	return 1;
}

void YAP_receivePacketAsync(YAPHandler *handler, YAPPacket *packet) {
	//TODO implement
}