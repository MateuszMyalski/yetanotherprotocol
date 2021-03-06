/*
 *
 * Protocole.
 * Author: Mateusz Myalski
 *
 */
#include <stdio.h>
#include <string.h>
#include "YAP.h"

int main(int argc, char* argv[]) {
	char payload[] = "ABCDEFG\0";
	YAPPacket *sendPacket = YAP_packetCreate(52, payload);
	YAPPacket *receivedPacket = YAP_emptyPacketCreate();

	/* Create protocole handler */
	YAPHandler *protocolHandler = YAP_handlerCreate(20);

	/* Set parameters */
	YAP_setBaudRate(protocolHandler, BAUD_RATE_115200);
	YAP_setByteSize(protocolHandler, DATA_BITS_8 );
	YAP_setStopBits(protocolHandler, ONE_STOP_BIT);
	YAP_setParity(protocolHandler, NO_PARITY);
	YAP_setAnswerTimeout(protocolHandler, 4000);

	/* Display parameters */
	printf("===Protocole settings===\n");
	printf("\tBaud rate: %db/s\n", YAP_getBaudRate(protocolHandler));
	printf("\tByte size: %dbits\n", YAP_getByteSize(protocolHandler));
	printf("\tParity: %d\n", YAP_getParity(protocolHandler));
	printf("\tACK Timeout: %dms\n", YAP_getAnswerTimeout(protocolHandler));

	printf("\n\n");
	printf("SENDING:\n");

	YAP_sendPacket(protocolHandler, sendPacket);
	
	printf("\n\n");
	printf("RECEIVING:\n");

	printf("\tResult: %d\n", YAP_receivePacket(protocolHandler, receivedPacket));
	printf("\tPacketID: %d\n", YAP_getPacketID(receivedPacket));
	printf("\tPacket payload: %s\n", YAP_getPacketPayload(receivedPacket));

	/* Destroy objects */
	YAP_packetDestroy(sendPacket);
	YAP_handlerDestroy(protocolHandler);

	return 0;

}


