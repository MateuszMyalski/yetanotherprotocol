#include "YetAnotherProtocol/YAP_internal.h"

uint8_t YAP_sendByte(YAPHandler *handler, uint8_t data) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;

	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_UART_Transmit(YAPh->huart, &data, sizeof(uint8_t), 50);
	if (status != HAL_OK){
		Error_Handler();
		return 0;
	}

	return 1;
}

uint8_t YAP_receiveByte(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	uint8_t receivedByte = 0;
	HAL_StatusTypeDef status = HAL_OK;
	HAL_UART_AbortReceive_IT(YAPh->huart);
	status = HAL_UART_Receive(YAPh->huart, &receivedByte, sizeof(uint8_t), YAPh->selectedTimeout);
	if (status != HAL_OK){
		Error_Handler();
		return 0;
	}
	return receivedByte;
}


void YAP_receiveAnswearTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	YAPh->selectedTimeout = YAPh->answearTimeout;
}


void YAP_receiveTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *YAPh = (YAPHandlerInternal *)handler;
	YAPh->selectedTimeout = YAPh->receiveTimeout;
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
