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

#include "YAP_internal.h"

YAPHandler *YAP_handlerCreate(uint8_t COMNumber) {
    YAPHandlerInternal  *YAPh       = (YAPHandlerInternal *)malloc((sizeof *YAPh));
    HANDLE              hComm       = {0};
    DCB                 paramsComm  = {0};

    /* Prepare COM handler for windows */
    char *COMPortName = (char *) calloc(11 * (sizeof *COMPortName), 11 * (sizeof *COMPortName));
    sprintf(COMPortName, "\\\\.\\COM%u", COMNumber);    // WinApi protect higher ports with these stupid slashes
    hComm = CreateFile(COMPortName,
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       0,
                       NULL);
    if(hComm == INVALID_HANDLE_VALUE){
        #ifdef DEBUG_INFORMATIONS
        printf("[Debug] Error in opening serial port %s.\n", COMPortName);
        #endif
        return NULL;
    }else{
        #ifdef DEBUG_INFORMATIONS
        printf("[Debug] Opening serial port %s successful.\n", COMPortName);
        #endif
    }
    free(COMPortName);


    /* Bind YAP protocole structure */
    YAPh->PortHandle                = hComm;
    YAPh->answerTimeout            = 0;
    YAPh->SerialParams              = paramsComm;
    YAPh->SerialParams.DCBlength    = sizeof(paramsComm);

    /* Set default timeouts */
    YAP_sendTimeouts(YAPh);
    YAP_receiveTimeouts(YAPh);

    return YAPh;
}

void YAP_handlerDestroy(YAPHandler *handler) {
    CloseHandle(((YAPHandlerInternal *)handler)->PortHandle);
    free(handler);
    #ifdef DEBUG_INFORMATIONS
    printf("[Debug] Closing serial port successful.\n");
    #endif
}

void YAP_setBaudRate(YAPHandler *handler, YAPBaudRateEnum baudRate) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    tempHandler->SerialParams.BaudRate = baudRate;
    uint8_t state = SetCommState(tempHandler->PortHandle, &(tempHandler->SerialParams));
    #ifdef DEBUG_INFORMATIONS
    if(state != 0)
        printf("[Debug] Parameter set successfully.\n");
    else
        printf("[Debug] Error while setting parameter.\n");
    #endif
}

YAPBaudRateEnum YAP_getBaudRate(YAPHandler *handler) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    return tempHandler->SerialParams.BaudRate;
}

void YAP_setByteSize(YAPHandler *handler, YAPDataSizeEnum byteSize) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    tempHandler->SerialParams.ByteSize = byteSize;
    uint8_t state = SetCommState(tempHandler->PortHandle, &(tempHandler->SerialParams));
    #ifdef DEBUG_INFORMATIONS
    if(state != 0)
        printf("[Debug] Parameter set successfully.\n");
    else
        printf("[Debug] Error while setting parameter.\n");
    #endif
}

YAPDataSizeEnum YAP_getByteSize(YAPHandler *handler) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    return tempHandler->SerialParams.ByteSize;
}

void YAP_setStopBits(YAPHandler *handler, YAPStopBitsEnum stopBits) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    tempHandler->SerialParams.StopBits = stopBits;
    uint8_t state = SetCommState(tempHandler->PortHandle, &(tempHandler->SerialParams));
    #ifdef DEBUG_INFORMATIONS
    if(state != 0)
        printf("[Debug] Parameter set successfully.\n");
    else
        printf("[Debug] Error while setting parameter.\n");
    #endif
}

YAPStopBitsEnum YAP_getStopBits(YAPHandler *handler) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    return tempHandler->SerialParams.StopBits;
}

void YAP_setParity(YAPHandler *handler, YAPParityEnum parity) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    tempHandler->SerialParams.Parity = parity;
    uint8_t state = SetCommState(tempHandler->PortHandle, &(tempHandler->SerialParams));
    #ifdef DEBUG_INFORMATIONS
    if(state != 0)
        printf("[Debug] Parameter set successfully.\n");
    else
        printf("[Debug] Error while setting parameter.\n");
    #endif
}

YAPParityEnum YAP_getParity(YAPHandler *handler) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    return tempHandler->SerialParams.Parity;
}

void YAP_setAnswerTimeout(YAPHandler *handler, uint32_t answerTimeout) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    tempHandler->answerTimeout      = answerTimeout;
    #ifdef DEBUG_INFORMATIONS
    printf("[Debug] Parameter set successfully.\n");
    #endif
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
    YAPp->crc16               = YAP_crc16((uint8_t *)YAPp->payload, YAPp->payloadLength - 2);
    return YAPp;
}

YAPPacket *YAP_emptyPacketCreate(void) {
    YAPPacketInternal *YAPp = (YAPPacketInternal *) malloc((sizeof *YAPp));
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