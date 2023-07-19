#ifndef _MYFUN
#define _MYFUN

#define 	SPI1_NSS_OFF()			GPIOA->ODR |= GPIO_ODR_OD4
#define 	SPI1_NSS_ON()				GPIOA->ODR &= ~GPIO_ODR_OD4

#include <stm32f407xx.h>
#include <stm32f4xx.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum{LedOff = 0, LedOn = 1, LedTog = 2}eLed;
typedef enum{ButtonNull = 0, Click = 1}eButton;

void delay_ms(uint32_t ms);
void delay(uint32_t nTime);
void Led_Conf(void);
void Led_OnOff(uint8_t num, eLed state);
void SPI1_Conf(void);
static uint8_t SPI1_SendReadByte(uint8_t send);
void SPI1_WriteReg(uint8_t address, uint8_t value);
uint8_t SPI1_ReadReg(uint8_t address);
void MFRC522_WriteRegister(uint8_t addr, uint8_t val);
uint8_t MFRC522_ReadRegister(uint8_t addr);
void MFRC522_Reset(void);
void MFRC522_SetBitMask(uint8_t reg, uint8_t mask);
void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask);
void MFRC522_AntennaOn(void);
void MFRC522_AntennaOff(void);
void MFRC522_Init(void);
void MFRC522_Halt(void);
uint8_t MFRC522_Request(uint8_t requestPICC, uint8_t* cardNumberPoint);
uint8_t MFRC522_Anticoll(uint8_t* sernum);
uint8_t MFRC522_CRC(uint8_t* inputdata, uint8_t length, uint8_t* outputdata);
uint8_t MFRC522_Check(uint8_t* cardnumber);
uint8_t MFRC522_TransmitDataToFIFO(uint8_t command, uint8_t* senddata, uint8_t senddatalength, uint8_t* returndatapoint, uint16_t* returndatalengthpoint);
void USART2_Conf_Basic(void);
void ComSendChar (USART_TypeDef *USARTx, char c);
void ComPuts(USART_TypeDef* USARTx, const char* str);
void all_on(void);
void all_off(void);
bool brelok1(void);
bool karta1(void);
bool brelok2(void);
bool karta2(void);

#define MFRC522_MAX_LEN										16
#define PICC_HALT													0x50   				// Sleep
#define PICC_ANTICOLL											0x93  		  	// Anti-collision
#define PICC_READ													0x30   				// Read Block
#define PICC_WRITE												0xA0   				// write block
#define PICC_REQALL												0x52  			  // find all the cards antenna area
#define PCD_IDLE													0x00   				// No action; Cancel the current command
#define PCD_CALCCRC												0x03   				// CRC Calculate
#define PCD_RESETPHASE 										0x0F          // Reset MFRC522
#define PCD_TRANSCEIVE										0x0C   				// Transmit and receive data
#define PCD_AUTHENT												0x0E   				// Authentication Key

#define MFRC522_REG_COMMAND 							0x01					// Starts and stops command execution
#define MFRC522_REG_IRQ										0x02					// Control bits to enable and disable the passing of interrupt requests
#define MFRC522_TX_MODE_REGISTER					0x12
#define MFRC522_RX_MODE_REGISTER					0x13
#define MFRC522_REG_STATUS2								0x08					// Contains status bits of the receiver, transmitter and data mode detector
#define MFRC522_REG_RF_CFG								0x26					// Configures the receiver gain, reset 48
#define MFRC522_REG_MODE									0x11					// Defines general mode settings for transmitting and receiving, reset 3F
#define MFRC522_REG_TX_CONTROL						0x14					// Controls the logical behavior of the antenna driver, reset 80
#define MFRC522_REG_TX_AUTO								0x15					// Controls transmit modulation settings, reset 00
#define MFRC522_REG_T_MODE								0x2A					// Defines the timer settings, reset 00
#define MFRC522_REG_T_PRESCALER						0x2B					// Defines the timer settings, reset 00
#define MFRC522_REG_T_RELOAD_H						0x2C					// Defines the 16-bit timer reload value (higher bits), reset 00
#define MFRC522_REG_T_RELOAD_L						0x2D					// Defines the 16-bit timer reload value (lower bits), reset 00
#define MFRC522_REG_BIT_FRAMING						0x0D					// Adjustments for bit-oriented frames
#define MFRC522_REG_COLLISION							0x0E					// Defines the first bit-collision detected on the RF interface
#define MFRC522_REG_DIV_IRQ								0x05					// Interrupt request bits
#define MFRC522_REG_COM_IRQ								0x04					// Interrupt request bits
#define MFRC522_REG_FIFO_LEVEL						0x0A					// Indicates the number of bytes stored in the FIFO
#define MFRC522_REG_FIFO_DATA							0x09					// Input and output of 64 byte FIFO buffer
#define MFRC522_REG_CRC_RESULT_H					0x21					// Shows the MSB value of the CRC calculation
#define MFRC522_REG_CRC_RESULT_L					0x22					// Shows the LSB value of the CRC calculation
#define MFRC522_REG_ERROR									0x06 
#define MFRC522_REG_CONTROL								0x0C




#endif
