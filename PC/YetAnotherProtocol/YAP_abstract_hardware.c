#include "YAP_internal.h"

uint8_t YAP_sendByte(YAPHandler *handler, char data) {
	YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
	unsigned long bytesToWrite = sizeof(char);
	unsigned long bytesWritten = 0;
	#ifdef DEBUG_INFORMATIONS
	printf("Sending: %d[%c].\n", (uint8_t)data, data);
	#endif
	uint8_t status = 0;
	status = WriteFile(
		tempHandler->PortHandle,
		&data,
		bytesToWrite,
		&bytesWritten,
		NULL);

	if (!status) {
		#ifdef DEBUG_INFORMATIONS
		printf("Error when writting to file.\n");
		#endif
		return 0;
	}
	if (bytesToWrite != bytesWritten) {
		#ifdef DEBUG_INFORMATIONS
		printf("Error while sending data.\n");
		#endif
		return 0;
	}
	return 1;
}

void YAP_sendTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
	COMMTIMEOUTS timeouts = { 0 };

	GetCommTimeouts(tempHandler->PortHandle, &timeouts);
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    SetCommTimeouts(tempHandler->PortHandle, &timeouts);
}

void YAP_receiveAnswearTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
	COMMTIMEOUTS timeouts = { 0 };

	GetCommTimeouts(tempHandler->PortHandle, &timeouts);
	timeouts.ReadIntervalTimeout         = tempHandler->answearTimeout;
    timeouts.ReadTotalTimeoutConstant    = 0;
    timeouts.ReadTotalTimeoutMultiplier  = tempHandler->answearTimeout;

    SetCommTimeouts(tempHandler->PortHandle, &timeouts);
}

void YAP_receiveTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
	COMMTIMEOUTS timeouts = { 0 };

	GetCommTimeouts(tempHandler->PortHandle, &timeouts);
	timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;

    SetCommTimeouts(tempHandler->PortHandle, &timeouts);
}

uint8_t YAP_receiveByte(YAPHandler *handler) {
	YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;

	unsigned long bytesRead = 0;
	uint8_t receivedByte = 0;

	ReadFile(tempHandler->PortHandle,
			 &receivedByte,
			 sizeof(receivedByte),
			 &bytesRead,
			 NULL);

	return receivedByte;
}

uint8_t YAP_waitForByte(YAPHandler *handler, uint8_t byte) {
	while(YAP_receiveByte(handler) != byte)
		;
	return byte;
}

uint8_t YAP_poolForAnswear(YAPHandler *handler, uint8_t question, uint8_t answear) {
	char receivedFlag = 0;

	YAP_receiveAnswearTimeouts(handler);

	for(uint8_t tries = 0; tries < 4; ++tries) {
		#ifdef DEBUG_INFORMATIONS
		clock_t startTime = clock();
		#endif

        if(question != 0)
            YAP_sendByte(handler, question);
        receivedFlag = YAP_receiveByte(handler);

		if(receivedFlag != 0) {
			#ifdef DEBUG_INFORMATIONS
			printf("Response in %ldms (Tries: %d).\n", clock() - startTime, tries + 1);
			#endif

			break;
		}else {
			#ifdef DEBUG_INFORMATIONS
			printf("No response in %ldms (Tries: %d).\n", clock() - startTime, tries + 1);
			#endif
		}
	}

	YAP_receiveTimeouts(handler);

    if(receivedFlag == answear)
        return 1;
    else
        return 0;
}
