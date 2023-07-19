// PA4  - (OUT)	SPI1_NSS
// PA5  - (OUT)	SPI1_SCK
// PA6  - (IN)	SPI1_MISO (Master In)
// PA7  - (OUT)	SPI1_MOSI (Master Out)

// MFRC522		STM32F407
// CS (SDA)	PA4			SPI1_NSS	Chip select
// SCK			PA5			SPI1_SCK	Serial Clock
// MOSI			PA7 		SPI1_MOSI	Master In Slave Out
// MISO			PA6			SPI1_MISO	Master Out Slave In
// IRQ			-				Irq
// GND			GND			Ground
// RST			3.3V		Reset pin
// VCC			3.3V		3.3V power

#include "fun.h"

volatile uint32_t tick = 0;
volatile uint8_t cardID[5];
void delay_ms(uint32_t ms)
{
	tick=0;
	while(tick < ms);
}
void SysTick_Handler(void)
{
	tick++;
}
void Led_Conf(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	
	GPIOD->MODER &= ~GPIO_MODER_MODE12;
	GPIOD->MODER |= GPIO_MODER_MODE12_0;
	GPIOD->MODER &= ~GPIO_MODER_MODE13;
	GPIOD->MODER |= GPIO_MODER_MODE13_0;
	GPIOD->MODER &= ~GPIO_MODER_MODE14;
	GPIOD->MODER |= GPIO_MODER_MODE14_0;
	GPIOD->MODER &= ~GPIO_MODER_MODE15;
	GPIOD->MODER |= GPIO_MODER_MODE15_0;
}
void Led_OnOff(uint8_t num, eLed state)
{
	switch(num)
	{
		case 0:
			if(state == LedOff) 			GPIOD->ODR &= ~GPIO_ODR_OD12;
			else if(state == LedOn) 	GPIOD->ODR |= GPIO_ODR_OD12;
			else if(state == LedTog) 	GPIOD->ODR ^= GPIO_ODR_OD12;
		break;
		case 1:
			if(state == LedOff) 			GPIOD->ODR &= ~GPIO_ODR_OD13;
			else if(state == LedOn) 	GPIOD->ODR |= GPIO_ODR_OD13;
			else if(state == LedTog) 	GPIOD->ODR ^= GPIO_ODR_OD13;
		break;
		case 2:
			if(state == LedOff) 			GPIOD->ODR &= ~GPIO_ODR_OD14;
			else if(state == LedOn) 	GPIOD->ODR |= GPIO_ODR_OD14;
			else if(state == LedTog) 	GPIOD->ODR ^= GPIO_ODR_OD14;
		break;
		case 3:
			if(state == LedOff) 			GPIOD->ODR &= ~GPIO_ODR_OD15;
			else if(state == LedOn) 	GPIOD->ODR |= GPIO_ODR_OD15;
			else if(state == LedTog) 	GPIOD->ODR ^= GPIO_ODR_OD15;
		break;
	}
}
void SPI1_Conf(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	
	GPIOA->MODER |= GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
	GPIOA->AFR[0] |= 0x55500000;
  GPIOA->MODER |= GPIO_MODER_MODE4_0;
  GPIOA->PUPDR |= GPIO_PUPDR_PUPD4_0;
	SPI1->CR1 = SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 | SPI_CR1_MSTR 
	| SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_SPE;
}
static uint8_t SPI1_SendReadByte(uint8_t send)
{
	uint8_t read;
	while((SPI1->SR & SPI_SR_TXE) == RESET){;}
	SPI1->DR = send;
	while((SPI1->SR & SPI_SR_RXNE) == RESET){;}
	read = SPI1->DR;
	return read;
}
void SPI1_WriteReg(uint8_t address, uint8_t value) 
{
	SPI1_NSS_ON();										
	SPI1_SendReadByte(address);
	SPI1_SendReadByte(value);
	SPI1_NSS_OFF();										
}
uint8_t SPI1_ReadReg(uint8_t address) 
{
	uint8_t	x;
	SPI1_NSS_ON();										
	SPI1_SendReadByte(address);
	x = SPI1_SendReadByte(0x00);
	SPI1_NSS_OFF();										
	return x;
}
void MFRC522_WriteRegister(uint8_t addr, uint8_t val) 
{
	addr = (addr << 1) & 0x7E;
  SPI1_WriteReg(addr, val);
}
uint8_t MFRC522_ReadRegister(uint8_t addr) 
{
	uint8_t val;
	addr = ((addr << 1) & 0x7E) | 0x80;
	val = SPI1_ReadReg(addr);
	return val;	
}
void MFRC522_Reset(void) 
{
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_RESETPHASE);
}
void MFRC522_SetBitMask(uint8_t reg, uint8_t mask) 
{
	MFRC522_WriteRegister(reg, MFRC522_ReadRegister(reg) | mask);
}
void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask)
{
	MFRC522_WriteRegister(reg, MFRC522_ReadRegister(reg) & (~mask));
}
void MFRC522_AntennaOn(void) 
{
	uint8_t temp;
	temp = MFRC522_ReadRegister(MFRC522_REG_TX_CONTROL);
	if (!(temp & 0x03)) 
	{
		MFRC522_SetBitMask(MFRC522_REG_TX_CONTROL, 0x03);
	}
}
void MFRC522_AntennaOff(void) 
{
	MFRC522_ClearBitMask(MFRC522_REG_TX_CONTROL, 0x03);
}
void MFRC522_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	GPIOG->MODER |= GPIO_MODER_MODE2_0;
  GPIOG->PUPDR |= GPIO_PUPDR_PUPD2_0;
	GPIOG->ODR |= GPIO_ODR_OD2;
	
	MFRC522_Reset();
	MFRC522_WriteRegister(MFRC522_TX_MODE_REGISTER, 0x00);      // Define internal timer settings
  MFRC522_WriteRegister(MFRC522_RX_MODE_REGISTER, 0x00);
	MFRC522_WriteRegister(MFRC522_REG_T_MODE, 0x8D);			// prescaler higher 4 digits: 1101
	MFRC522_WriteRegister(MFRC522_REG_T_PRESCALER, 0x3E);	// prescaler lower 8 digits: 0011 1110
	MFRC522_WriteRegister(MFRC522_REG_T_RELOAD_L, 0xE8);
	MFRC522_WriteRegister(MFRC522_REG_T_RELOAD_H, 0x03);			// reset
	MFRC522_WriteRegister(MFRC522_REG_RF_CFG, 0x70);			// 48dB
	MFRC522_WriteRegister(MFRC522_REG_TX_AUTO, 0x40);
	MFRC522_WriteRegister(MFRC522_REG_MODE, 0x3D);				// CRC preset  6363h
	MFRC522_AntennaOn();
}
void MFRC522_Halt(void)
{
	uint16_t bytes = 0;																		// number of bytes to write
	uint8_t buff[4] = {0};
	uint8_t oppstatus = 0;
	
	buff[0] = PICC_HALT;
	buff[1] = 0;
	
	oppstatus = MFRC522_CRC(buff, 2, &buff[2]);
	
	if(oppstatus != 0)
	{
		return;
	}
	oppstatus = MFRC522_TransmitDataToFIFO(PCD_TRANSCEIVE, buff, 4, buff, &bytes);
   
  if(oppstatus != 0)
   {
     return;
   }
}
uint8_t MFRC522_Request(uint8_t requestPICC, uint8_t* cardNumberPoint)
{
	uint8_t oppstatus = 2;  
  uint16_t retbits = 0;
	
	MFRC522_WriteRegister(MFRC522_REG_BIT_FRAMING, 0x07);
	
	cardNumberPoint[0] = requestPICC;
	oppstatus = MFRC522_TransmitDataToFIFO(PCD_TRANSCEIVE, cardNumberPoint, 1, cardNumberPoint, &retbits);
	
	if ((oppstatus != 0) || (retbits != 0x10))
    {    
      oppstatus = 2;
    }
  return oppstatus;
}
uint8_t MFRC522_Anticoll(uint8_t* sernum)
{
	uint8_t oppstatus = 2;
  uint8_t i = 0;
  uint8_t sernumcheck = 0;
  uint16_t unlen;
	
	MFRC522_ClearBitMask(MFRC522_REG_STATUS2, 0x08);
	MFRC522_ClearBitMask(MFRC522_REG_COLLISION, 0x80);
	MFRC522_WriteRegister(MFRC522_REG_BIT_FRAMING, 0x00);
	
	sernum[0] = PICC_ANTICOLL;
	sernum[1] = 0x20;
	
	oppstatus = MFRC522_TransmitDataToFIFO(PCD_TRANSCEIVE, sernum, 2, sernum, &unlen);
   
	if (oppstatus == 0)
   {
		 for (i = 0; i < 4; i++) 
		 { 
     sernumcheck ^= sernum[i];
		 }
   }
   
    return oppstatus;
}
uint8_t MFRC522_CRC(uint8_t* inputdata, uint8_t length, uint8_t* outputdata)
{
	uint8_t i = 0;
	uint8_t data = 0;
	
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_IDLE);									// Stop then active command
	MFRC522_ClearBitMask(MFRC522_REG_DIV_IRQ, 0x04);									// Clear CRCIrq bit
	MFRC522_SetBitMask(MFRC522_REG_FIFO_LEVEL, 0x80);									// FIFO initialization
	
	//data to FIFO
	for(i=0;i<length;i++)
	{
		MFRC522_WriteRegister(MFRC522_REG_FIFO_DATA, *(inputdata+i));
	}
	
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_CALCCRC);					// Start calculation
	
	// wait to end calculation
	i = 255;
	do{
		data = MFRC522_ReadRegister(MFRC522_REG_DIV_IRQ);
		i--;
		
		if(!(data&0x04))																								// Chceck if CRCIrq is clear
		{
			MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_IDLE);
			outputdata[1] = MFRC522_ReadRegister(MFRC522_REG_CRC_RESULT_L);
			outputdata[0] = MFRC522_ReadRegister(MFRC522_REG_CRC_RESULT_H);
			return 0;
		}
	} while(i!=0);
	return 2;
}
uint8_t MFRC522_Check(uint8_t* cardnumber)
{
	uint8_t oppstatus = 2;
	
	MFRC522_AntennaOn();
	
	oppstatus = MFRC522_Request(PICC_REQALL, cardnumber);
	
	if(oppstatus == 0)
	{
		oppstatus = MFRC522_Anticoll(cardnumber);
	}
	MFRC522_Halt();
	MFRC522_AntennaOff();
	
	return oppstatus;
}
uint8_t MFRC522_TransmitDataToFIFO(uint8_t command, uint8_t* senddata, uint8_t senddatalength, uint8_t* returndatapoint, uint16_t* returndatalengthpoint)
{
	uint8_t oppstatus = 2;
	uint8_t interruptstatus = 0x00;
	uint8_t interruptwait = 0x00;
	uint8_t transmitlastbits = 0;
	uint8_t data = 0;
	uint16_t i = 0;
	uint8_t readstatus;
	
	switch (command)
    {
        case PCD_AUTHENT:
        {
            interruptstatus = 0x12;
            interruptwait = 0x10;
            break;
        }
        case PCD_TRANSCEIVE:
        {
            interruptstatus = 0x77;
            interruptwait = 0x30;
            break;
        }
        default:
        {
            oppstatus = 2;
            return oppstatus;
        }
    }
	MFRC522_WriteRegister(MFRC522_REG_IRQ, interruptstatus | 0x80);
	MFRC522_ClearBitMask(MFRC522_REG_COM_IRQ, 0x80);
	MFRC522_SetBitMask(MFRC522_REG_FIFO_LEVEL, 0x80);
	
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_IDLE);
		
	// data to fifo
	for(i=0;i<senddatalength;i++)
		{
			MFRC522_WriteRegister(MFRC522_REG_FIFO_DATA, senddata[i]);
		}
		
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, command);
	
	if(command == PCD_TRANSCEIVE)
	{
		MFRC522_SetBitMask(MFRC522_REG_BIT_FRAMING, 0x80);				// Transmission of data starts
	}
	
	i = 4000;
	
	do
	{
		data = MFRC522_ReadRegister(MFRC522_REG_COM_IRQ);
		i--;
	} while ((i != 0) && !(data & 0x01) && !(data & interruptwait));
	
	
	if(i != 0)
	{
		readstatus = MFRC522_ReadRegister(MFRC522_REG_ERROR);
		
		if(!(readstatus & 0x1B))
		{
			oppstatus = 0;
			if(command == PCD_TRANSCEIVE)
			{
				data = MFRC522_ReadRegister(MFRC522_REG_FIFO_LEVEL);
				transmitlastbits = MFRC522_ReadRegister(MFRC522_REG_CONTROL) & 0x07;
				if(transmitlastbits)
				{
					*returndatalengthpoint = transmitlastbits + ((data - 1)*8);
				}
				else
				{
					*returndatalengthpoint = (data *8);
				}
				if(data == 0)
				{
					data = 1;
				}
				if(data > MFRC522_MAX_LEN)
				{
					data = MFRC522_MAX_LEN;
				}
				for(i=0;i<data;i++)
				{
					returndatapoint[i] = MFRC522_ReadRegister(MFRC522_REG_FIFO_DATA);
				}
			}
		}
		else
		{
			oppstatus = 2;
		}
	}
	return oppstatus;
}
void USART2_Conf_Basic(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
	GPIOA->MODER &= ~GPIO_MODER_MODER2 & ~GPIO_MODER_MODER3;				
	GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;
	GPIOA->AFR[0] |= 0x00007700;
	
	USART2->BRR = 16000000/9600;
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
}
void ComSendChar(USART_TypeDef *USARTx, char c)
{
	while(!(USARTx->SR & USART_SR_TXE)){;}
		USARTx->DR = c;
}
void ComPuts(USART_TypeDef* USARTx, const char* str)
{	
	while(*str)
		ComSendChar(USARTx, *str++);
}
void all_on(void)
{
	Led_OnOff(0, LedOn);
	Led_OnOff(1, LedOn);
	Led_OnOff(2, LedOn);
	Led_OnOff(3, LedOn);
}
void all_off(void)
{
	Led_OnOff(0, LedOff);
	Led_OnOff(1, LedOff);		
	Led_OnOff(2, LedOff);
	Led_OnOff(3, LedOff);
}
bool brelok1(void)
{
	if(cardID[0]==0x4E && cardID[1]==0x6D && cardID[2]==0x95 && cardID[3]==0x79 && cardID[4]==0xCF) return true;
	return false;
}
bool karta1(void)
{
	if(cardID[0]==0x4F && cardID[1]==0x59 && cardID[2]==0x96 && cardID[3]==0x29 && cardID[4]==0xA9) return true;
	return false;
}
bool brelok2(void)
{
	if(cardID[0]==0x3D && cardID[1]==0xF2 && cardID[2]==0x8B && cardID[3]==0x79 && cardID[4]==0x3D) return true;
	return false;
}
bool karta2(void)
{
	if(cardID[0]==0x4F && cardID[1]==0x3E && cardID[2]==0x1E && cardID[3]==0x29 && cardID[4]==0x46) return true;
	return false;
}
