#include "YetAnotherProtocol/YAP_internal.h"

uint8_t YAP_sendByte(YAPHandler *handler, char data) {
	YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;

	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_UART_Transmit(tempHandler->huart, (uint8_t *)&data, sizeof(char), tempHandler->sendTimeout);

	if (status != HAL_OK){
		Error_Handler();
		return 0;
	}

	return 1;
}

void YAP_sendTimeouts(YAPHandler *handler) {

}

void YAP_receiveAnswearTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
	tempHandler->selectedTimeout = tempHandler->answearTimeout;
}

void YAP_receiveTimeouts(YAPHandler *handler) {
	YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
	tempHandler->selectedTimeout = tempHandler->receiveTimeout;
}

uint8_t YAP_receiveByte(YAPHandler *handler) {
	YAPHandlerInternal *tempHandler = (YAPHandlerInternal *)handler;
	uint8_t receivedByte = 0;
	HAL_UART_Receive(tempHandler->huart, &receivedByte, sizeof(uint8_t), tempHandler->selectedTimeout);

	return receivedByte;
}

uint8_t YAP_poolForAnswear(YAPHandler *handler, uint8_t question, uint8_t answear) {
	char receivedFlag = 0;

	YAP_receiveAnswearTimeouts(handler);

	for(uint8_t tries = 0; tries < 4; ++tries) {
        if(question != 0)
            YAP_sendByte(handler, question);
        receivedFlag = YAP_receiveByte(handler);

        if(receivedFlag != 0)
			break;
	}

	YAP_receiveTimeouts(handler);

    if(receivedFlag == answear)
        return 1;
    else
        return 0;
}
