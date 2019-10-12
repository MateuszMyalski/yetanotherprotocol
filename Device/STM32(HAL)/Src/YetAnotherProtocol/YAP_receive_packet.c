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
 * @file YAP_receive_packet.c
 * @author Mateusz Waldemar Myalski
 * @date 8 Oct 2019
 * @brief Functions that allow to parse incoming packet
 */

#include "YetAnotherProtocol/YAP_internal.h"

uint8_t YAP_receivePacket(YAPHandler *handler, YAPPacket *packet) {
	YAPPacketInternal 	*YAPp = (YAPPacketInternal *)packet;

	/* Check payload length */
	if (YAPp->payloadCounter != YAPp->payloadLength) {
		YAP_sendByte(handler, NAK);
		YAPp->transsmisionState = TRANSSMISION_NOT_ENQUIRED;
		return 0;
	}

	/* Check data integrity - last 2 bytes of payload is CRC */
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
