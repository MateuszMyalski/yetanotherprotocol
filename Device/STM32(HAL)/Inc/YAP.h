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
 * @file YAP.h
 * @author Mateusz Waldemar Myalski
 * @date 8 Oct 2019
 * @brief File contain function, enumerations and typedefs declarations for abstract layer
 */

#ifndef YAP_h
#define YAP_h

#include <stdint.h>
#include "usart.h"

/**
 * @brief Enumeration packet state.
 */
typedef enum {
	TRANSSMISION_NOT_ENQUIRED,  ///< Transsmision is not enquired. Packet is waiting for EQN.
    RECEIVE_PAYLOAD_ID,         ///< Packet is waiting for packet ID.
    RECEIVE_PAYLOAD_LENGTH,     ///< Packet is waiting for payload length.
    RECEIVING_PAYLOAD,          ///< Packet is receiving payload.
    PACKET_READY,               ///< Packet is parsed and ready to be parsed.
    READY_TO_TRANSSMIT          ///< Packet is ready to be send.
} YAPStateEnum;

/**
 * @brief Protocol handler.
 */
typedef void YAPHandler;

/**
 * @brief Packet handler.
 */
typedef void YAPPacket;

/** 
 *  @brief Create protocol handler.
 *  
 *  @param uart huart structure from HAL.
 * 
 *  @return YAPHandler* Pointer to the protocol handler structure.
 *  @return NULL When initalisation is failed. 
 */
YAPHandler *YAP_handlerCreate   (UART_HandleTypeDef *uart);
/** 
 *  @brief Destroy protocol handler and close the port COM.
 *  
 *  @param handler protocol handler pointer.
 * 
 *  @return Void. 
 */
void        YAP_handlerDestroy  (void *handler);

/** 
 *  @brief Get the setted answer timeout in ms.
 *  
 *  @param handler Protocol handler pointer.
 * 
 *  @return uint32_t 
 */
void 		YAP_setAnswerTimeout  (YAPHandler *handler, uint32_t answerTimeout);

/** 
 *  @brief Destroy the packet handler and free the memory.
 *  
 *  @param packet Packet to destroy.
 * 
 *  @return Void. 
 */
void        YAP_packetDestroy       (YAPPacket *packet);
/** 
 *  @brief Create packet with specified ID and payload.
 *  
 *  @param packetID Packet ID.
 *  @param payload Pointer to the payload.
 * 
 *  @return YAPPacket* Pointer to the generated packet structure. 
 */
YAPPacket   *YAP_packetCreate       (uint8_t packetID, char *payload);
/** 
 *  @brief Create empty packet structure.
 *  
 *  @return YAPPacket Pointer to the generated packet structure. 
 */
YAPPacket   *YAP_emptyPacketCreate  (void);

/** 
 *  @brief Get the setted answer timeout in ms.
 *  
 *  @param handler Protocol handler pointer.
 * 
 *  @return uint32_t 
 */
uint32_t    YAP_getAnswerTimeout   (YAPHandler *handler);

/** 
 *  @brief Get the packet ID from packet.
 *  
 *  @param packet Packet handler pointer.
 * 
 *  @return uint8_t 
 */
uint8_t     YAP_getPacketID         (YAPPacket *packet);
/** 
 *  @brief Get the payload from packet.
 *  
 *  @param packet Packet handler pointer.
 * 
 *  @return char* Pointer to the payload.
 */
char        *YAP_getPacketPayload   (YAPPacket *packet);


/** 
 *  @brief Process the received byte and bind the data to the new packet.
 * 
 *  This is low level function to use in UART ISR routine.
 *  
 *  @param phandler Pointer to the protocol handler.
 *  @param emptyPacket Pointer to the empty packet where parsed data will be stored.
 *  @param byte Pointer to the received byte.
 * 
 *  @return Void
 */
void 	YAP_processByte		(YAPHandler *handler, YAPPacket *emptyPacket, uint8_t *byte);

/** 
 *  @brief Send the packet via COM port in polling mode.
 *  
 *  @param phandler Pointer to the protocol handler.
 *  @param packet Pointer to the packet that will be send.
 * 
 *  @return uint8_t When function send the data without any errors return 1.
 */
uint8_t YAP_sendPacket      (YAPHandler *handler, YAPPacket *packet);
/** 
 *  @brief Receive the packet from COM port in polling mode.
 *  
 *  @param phandler Pointer to the protocol handler.
 *  @param emptyPacket Pointer to the empty packet where parsed data will be stored.
 * 
 *  @return uint8_t When function receive the data without any errors return 1.
 */
uint8_t YAP_receivePacket   (YAPHandler *handler, YAPPacket *emptyPacket);
/** 
 *  @brief Receive the packet from COM port in polling mode.
 *  
 *  @param packet Packet to check if is ready to parse.
 * 
 *  @return uint8_t When the packet is fully received returns 1.
 */
uint8_t YAP_isPacketReady	(YAPPacket *packet);

#endif
