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
 * @file YAP_user_API.c
 * @author Mateusz Waldemar Myalski
 * @date 8 Oct 2019
 * @brief File contain abstract layer for YAP protocol
 */

#include "YetAnotherProtocol/YAP_internal.h"

YAPHandler *YAP_handlerCreate(UART_HandleTypeDef *huart) {
    YAPHandlerInternal  *YAPh = (YAPHandlerInternal *)malloc((sizeof *YAPh));

    /* Prepare COM handler for HAL */
    YAPh->huart = huart;

    /* Set default timeouts */
    YAPh->answerTimeout = 0;
    YAPh->receiveTimeout = 50;
    YAPh->selectedTimeout = YAPh->receiveTimeout;

    return YAPh;
}

void YAP_handlerDestroy(YAPHandler *handler) {
    free(handler);
}

void YAP_setAnswerTimeout(YAPHandler *handler, uint32_t answerTimeout) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    tempHandler->answerTimeout      = answerTimeout;
}

uint32_t YAP_getAnswerTimeout(YAPHandler *handler) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    return tempHandler->answerTimeout;
}

YAPPacket *YAP_packetCreate(uint8_t packetID, char *payload) {
    YAPPacketInternal *YAPp   = (YAPPacketInternal *) malloc((sizeof *YAPp));
    YAPp->transsmisionState   = READY_TO_TRANSSMIT;
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
    if(YAPp->transsmisionState != READY_TO_TRANSSMIT)
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
	if (YAPp->transsmisionState == PACKET_READY || YAPp->transsmisionState == READY_TO_TRANSSMIT)
		return 1;
	else
		return 0;
}

