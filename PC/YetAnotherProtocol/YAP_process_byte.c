#include "YAP_internal.h"

void YAP_processByte(YAPHandler *handler, YAPPacket *emptyPacket, uint8_t *byte) {
	YAPPacketInternal 	*YAPp = (YAPPacketInternal *)emptyPacket;

	switch (*byte) {
		/* Transmission enquire */
		case ENQ:
			if(YAPp->transsmisionState == TRANSSMISION_NOT_ENQUIRED) {
				/* Clear out buffer, and prepare for receiving */
				YAPp->transsmisionState = RECEIVE_PAYLOAD_ID;
				YAPp->crc16 			= 0;
				YAPp->packetID 			= 0;
				YAPp->payloadLength		= 0;
				YAPp->payloadCounter	= 0;
				free(YAPp->payload);
				YAP_sendByte(handler, ACK);
			} else {
				YAP_sendByte(handler, NAK);
			}
			break;

		case DLE:
			break;

		/* End of transmission */
		case EOT:
			YAPp->transsmisionState = PACKET_READY;
			break;

		default:
			if(YAPp->transsmisionState == RECEIVE_PAYLOAD_ID) {
				YAPp->packetID = *byte;
				YAPp->transsmisionState = RECEIVE_PAYLOAD_LENGTH;
				YAP_sendByte(handler, ACK);
				break;
			}

			if(YAPp->transsmisionState == RECEIVE_PAYLOAD_LENGTH) {
				YAPp->payloadLength = *byte;
				YAPp->payload = (char *)malloc((sizeof *(YAPp->payload)) * YAPp->payloadLength);
				YAPp->transsmisionState = RECEIVING_PAYLOAD;
				YAP_sendByte(handler, ACK);
				break;
			}

			if(YAPp->transsmisionState == RECEIVING_PAYLOAD) {
				YAPp->payload[YAPp->payloadCounter] = (char)*byte;
				++YAPp->payloadCounter;
			}
			break;
	}

	*byte = 0;
}