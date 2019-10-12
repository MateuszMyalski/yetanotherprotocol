# YAP - Yet Another Protocol

[TOC]

Protocol designed to communicate between two devices (etc. PC -> uC) using COM interface and UART communication without external handshaking connections. Algorithm is using only external interrupts on uC without wasting DMA channels. Sender can set the packet ID and payload field. After creating the packet handler, the program will take care for the proper CRC16 CITT generation and message length field. This simplicity allow coder to implement easily proper communication between two devices without worrying if the data is passed through without any changes. The user API will return the status of the transmission and will inform if the data came or were sent corrupted. 

Right now the library for PC is written using only WINAPI in ANSI C, I am planing to expand this project to UNIX compatibility. Makefiles are included to compile the program by yourself and to make your own changes if necessary.
For uC implementation HAL library is used to be able to port this library for mostly all STM microcontrollers. 


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
YAP_setStopBits(protocolHandler, ONE_STOP_BIT);
YAP_setParity(protocolHandler, NO_PARITY);
YAP_setAnswerTimeout(protocolHandler, ANSWER_TIME);
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

# Usage for STM devices

## 1. Hardware initialisation

1.1 Using CubeMx software, select UART pins, and setup the popper configuration for the interface. 

1.2 __Enable global UART interrupts in NVIC.__

1.3 Drag files to the corresponding generated folders.

## 2. Protocol initialisation, sending and receiving packet

```C
#include "YAP.h"
#include "usart.h"

YAPHandler *protocolHandler;
YAPPacket  *packetToReceive;
uint8_t rxByte;

int main(void) {
    protocolHandler = YAP_handlerCreate(&huart1);	// Select the propper huart typedef
    YAP_setAnswerTimeout(protocolHandler, 4000);
    
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

 

# License

MIT License

Copyright (c) 2019 Mateusz Waldemar Myalski

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.