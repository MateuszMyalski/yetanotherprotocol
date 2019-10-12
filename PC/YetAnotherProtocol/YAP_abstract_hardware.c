#include "YAP_internal.h"

uint8_t YAP_sendByte(YAPHandler *handler, char data) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	unsigned long bytesToWrite = sizeof(char);
	unsigned long bytesWritten = 0;
	uint8_t status = 0;
	status = WriteFile(
		YAPh->PortHandle,
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
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	COMMTIMEOUTS timeouts = { 0 };

	GetCommTimeouts(YAPh->PortHandle, &timeouts);
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    SetCommTimeouts(YAPh->PortHandle, &timeouts);
}

void YAP_receiveAnswearTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	COMMTIMEOUTS timeouts = { 0 };

	GetCommTimeouts(YAPh->PortHandle, &timeouts);
	timeouts.ReadIntervalTimeout         = YAPh->answearTimeout;
    timeouts.ReadTotalTimeoutConstant    = 0;
    timeouts.ReadTotalTimeoutMultiplier  = YAPh->answearTimeout;

    SetCommTimeouts(YAPh->PortHandle, &timeouts);
}

void YAP_receiveTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	COMMTIMEOUTS timeouts = { 0 };

	GetCommTimeouts(YAPh->PortHandle, &timeouts);
	timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;

    SetCommTimeouts(YAPh->PortHandle, &timeouts);
}

uint8_t YAP_receiveByte(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;

	unsigned long bytesRead = 0;
	uint8_t receivedByte = 0;

	ReadFile(YAPh->PortHandle,
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

	YAP_sendByte(handler, question);
	receivedFlag = YAP_receiveByte(handler);

	YAP_receiveTimeouts(handler);

    if(receivedFlag == answear)
        return 1;
    else
        return 0;
}

uint16_t YAP_crc16(uint8_t *data, uint16_t size) {
  int i, j;
  uint16_t crc = 0x1d0f;
  for (i = 0; i < size; i++) {
    uint16_t xr = data[i] << 8;
    crc = crc ^ xr;

    for (j = 0; j < 8; j++) {
      if (crc & 0x8000) {
        crc = (crc << 1);
        crc = crc ^ 0x1021;
      }
      else {
        crc = crc << 1;
      }
    }
  }
  crc = crc & 0xFFFF;
  return crc;
}    
