/*******************************************************************************************************
 *    MIT License                       
 *                      
 *    Copyright (c) 2019 Mateusz Waldemar Myalski                       
 *                      
 *    Permission is hereby granted, free of charge, to any person obtaining a copy                      
 *    of this software and associated documentation files (the "Software"), to deal                     
 *    in the Software without restriction, including without limitation the rights                      
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell                     
 *    copies of the Software, and to permit persons to whom the Software is                     
 *    furnished to do so, subject to the following conditions:                      
 *                      
 *    The above copyright notice and this permission notice shall be included in all                        
 *    copies or substantial portions of the Software.                       
 *                      
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR                        
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                      
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE                       
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                        
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,                     
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE                     
 *    SOFTWARE.                     
 *                                       
 *******************************************************************************************************/

/**
 * @file YAP_process_byte.c
 * @author Mateusz Waldemar Myalski
 * @date 8 Oct 2019
 * @brief Function to process received byte from UART interface.
 */

#include "YAP_internal.h"

void YAP_processByte(YAPHandler *handler, YAPPacket *emptyPacket, uint8_t *byte) {
	YAPPacketInternal *YAPp = (YAPPacketInternal *)emptyPacket;

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
			
		/* Escape character */
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