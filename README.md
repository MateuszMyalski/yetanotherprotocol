# YAP - Yet Another Protocol

__TODO:__
[O] Answear -> answer spelling
[O] Code cleanup
[O] Snippetsd
[O] Code documentation
[O] Examples
[O] Add graphs
[O] Licence

Protocol designed to communicate between two devices (etc. PC -> uC) using COM interface and UART communication without external handshaking connections. Algorithm is using only external interrupts on uC without wasting DMA channels. Sender can set the packet ID and payload field. After creating the packet handler, the program will care for the proper CRC16 CITT and message length field. This simplicity allow coder to implement easily proper communication between two devices without worrying if the data is passed through without any changes. The user API will just return the status of the transmission and will inform if the data came or were sent corrupted. 

Right now the library for PC is written using WINAPI in ANSI C. This still allow to use simple functions, to send or receive packet form another device.
For uC implementation HAL library is used to be able port library to mostly all STM microcontrollers. 

# Communication graph

![Comminication_diagram](assets\Comminication_diagram.jpg)

# Usage for Windows

## 1. Initialisation

```C
#include "YAP.h"

#define COM_NUMBER 20
#define ANSWER_TIME 4000

YAPHandler *protocolHandler = YAP_handlerCreate(COM_NUMBER);
YAP_setBaudRate(protocolHandler, BAUD_RATE_115200);
YAP_setByteSize(protocolHandler, DATA_BITS_8);
YAP_setParity(protocolHandler, NO_PARITY);
YAP_setAnswearTimeout(protocolHandler, ANSWER_TIME);
```

## 2. Sending packet

```C
#define PACKET_ID 52

char payload[] = "Hello World!";
YAPPacket *packetToSend = YAP_packetCreate(PACKET_ID, payload);
if (YAP_sendPacket(protocoleHandler, packetToSend) != 1)
    printf("Error while sending packet!");
```

## 3. Receiving packet (polling)

```C
YAPPacket *packetToReceive = YAP_emptyPacketCreate();
if (YAP_receivePacket(protocolHandler, packetToReceive) != 1)
    printf("Error while receiving packet!");
```

## 4. Getting fields from packet

```C
printf("Packet id: %d", YAP_getPacketID(packetToReceive));
printf("Packet payload: %s", YAP_getPacketPayload(packetToReceive));
```

## 5. Freeing memory and port closing

```C
YAP_packetDestroy(packetToReceive);
YAP_packetDestroy(packetToSend);
YAP_handlerDestroy(protocolHandler);	// This will automatically close the COM port
```

# Usage for STM

## 1. Hardware initialisation

1.1 Using CubeMx software, select UART pins, and setup the popper configuration for the interface. 
__1.2 Enable global UART interrupts in NVIC!__

## 2. Protocol initialisation, sending and receiving packet

```C
#include "YAP.h"
#include "usart.h"

YAPHandler *protocolHandler;
YAPPacket  *packetToReceive;
uint8_t rxByte;

int main(void) {
    protocolHandler = YAP_handlerCreate(&huart1);	// Select the propper huart typedef
    YAP_setAnswearTimeout(protocolHandler, 4000);
    
    packetToReceive = YAP_emptyPacketCreate();
    
    HAL_UART_Receive_IT(&huart1, &byte, sizeof(byte));	// Start interrupt for incoming packet
    
    while(1) {
        if (YAP_isPacketReady(packetToReceive)) {
		  YAP_receivePacket(protocolHandler, packetToReceive);	// Parsing received packet
		  YAP_sendPacket(protocolHandler, packetHandler);	// Sending received packet back to the sender
		  HAL_UART_Receive_IT(&huart1, &byte, sizeof(byte));	// Start interrupt for next incoming packet

	  }
    }
}
```

## 3. Interrupt callback

```C
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	YAP_processByte(protocolHandler, packetToReceive, &rxByte);
	HAL_UART_Receive_IT(&huart1, &rxByte, sizeof(rxByte));
}
```

 