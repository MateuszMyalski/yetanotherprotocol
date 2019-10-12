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
        printf("Error in opening serial port %s.\n", COMPortName);
        #endif

        return NULL;
    }else{
        #ifdef DEBUG_INFORMATIONS
        printf("Opening serial port %s successful.\n", COMPortName);
        #endif
    }
    free(COMPortName);


    /* Bind YAP protocole structure */
    YAPh->PortHandle                = hComm;
    YAPh->answearTimeout            = 0;
    YAPh->SerialParams              = paramsComm;
    YAPh->SerialParams.DCBlength    = sizeof(paramsComm);

    /* Set default timeouts */
    YAP_sendTimeouts(YAPh);
    YAP_receiveTimeouts(YAPh);

    return YAPh;
}

void YAP_handlerDestroy(YAPHandler *handler) {
    CloseHandle(((YAPHandlerInternal *)handler)->PortHandle);
    #ifdef DEBUG_INFORMATIONS
    printf("Closing serial port successful.\n");
    #endif
    free(handler);
}

void YAP_setBaudRate(YAPHandler *handler, YAPBaudRateEnum baudRate) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    tempHandler->SerialParams.BaudRate = baudRate;
    uint8_t state = SetCommState(tempHandler->PortHandle, &(tempHandler->SerialParams));
    #ifdef DEBUG_INFORMATIONS
    if(state != 0)
        printf("Parameter set successfully.\n");
    else
        printf("Error while setting parameter.\n");
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
        printf("Parameter set successfully.\n");
    else
        printf("Error while setting parameter.\n");
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
        printf("Parameter set successfully.\n");
    else
        printf("Error while setting parameter.\n");
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
        printf("Parameter set successfully.\n");
    else
        printf("Error while setting parameter.\n");
    #endif
}

YAPParityEnum YAP_getParity(YAPHandler *handler) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    return tempHandler->SerialParams.Parity;
}

void YAP_setAnswearTimeout(YAPHandler *handler, uint16_t answearTimeout) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    tempHandler->answearTimeout = answearTimeout;
    #ifdef DEBUG_INFORMATIONS
    printf("Parameter set successfully.\n");
    #endif
}

uint16_t YAP_getAnswearTimeout(YAPHandler *handler) {
    YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
    return tempHandler->answearTimeout;
}

YAPPacket *YAP_packetCreate(uint8_t packetID, char *payload) {
    YAPPacketInternal *YAPp   = (YAPPacketInternal *) malloc((sizeof *YAPp));
    YAPp->packetID            = packetID;
    YAPp->payload             = payload;
    YAPp->payloadLength       = strlen(payload) + 2;
    YAPp->crc16               = YAP_crc16((uint8_t *)YAPp->payload, YAPp->payloadLength - 2);
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
	if (YAPp->transsmisionState == PACKET_READY)
		return 1;
	else
		return 0;
}