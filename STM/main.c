#include "fun.h"

extern uint8_t cardID[5];
char temp = 69;
uint8_t i;

int main()
{
	SysTick_Config(16000000 / 1000);
	Led_Conf();
	SPI1_Conf();
	MFRC522_Init();
	USART2_Conf_Basic();
	while(1) 
		{					
			MFRC522_Check(cardID);
			
			if(brelok1() || brelok2() || karta1() || karta2())
			{
				ComSendChar(USART2, temp);
				for(i=0;i<5;i++)
				{
					ComSendChar(USART2, cardID[i]);
					delay_ms(1);
				}
				delay_ms(1000);
			}
			
			if(USART2->DR=='a')
				{
					all_on();
				}
			if(USART2->DR=='b')
				{
					all_off();
				}
		}
}
