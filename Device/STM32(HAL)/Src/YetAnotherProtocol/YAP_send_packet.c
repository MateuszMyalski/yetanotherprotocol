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
 * @file YAP_send_packet.c
 * @author Mateusz Waldemar Myalski
 * @date 8 Oct 2019
 * @brief Functions that allow to send packet
 */

#include "YetAnotherProtocol/YAP_internal.h"

uint8_t YAP_sendPacket(YAPHandler *handler, YAPPacket *packet) {
	YAPPacketInternal *YAPp = (YAPPacketInternal *)packet;

	// Maximum payload is 250 bytes. This is the fixed size for every device using YAP
	if (YAPp->payloadLength > 250)
		return 0;

	if (!YAP_poolForAnswer(handler, ENQ, ACK))
		return 0;

	if (!YAP_poolForAnswer(handler, YAPp->packetID, ACK))
		return 0;

	if (!YAP_poolForAnswer(handler, YAPp->payloadLength, ACK))
		return 0;

	uint8_t charCounter = 0;
	while(charCounter != (YAPp->payloadLength - 2)) {
		YAP_sendByte(handler, YAPp->payload[charCounter]);
		++charCounter;
	}

	YAP_sendByte(handler, (uint8_t)(YAPp->crc16 >> 8));
	YAP_sendByte(handler, (uint8_t)(YAPp->crc16));

	if (YAP_poolForAnswer(handler, EOT, ACK))
		return 1;
	else
		return 0;
}
