#include <util/delay.h>
#include "uart.h"
#include "message_decoder.h"
#include "atmega328_adc.h"
#include "term.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	enum UsartAutomat{RX_, TX, DECODE} COM_AUTOMAT = RX_;
	MessageStruct UartMessage[10];
	uint8_t messages_num=0;
	volatile uint8_t Adc_CurrentValue = 0;
	volatile double Adc_Voltage = 0;
	char ReceivedString[32]="";
	char Adc_stringValue[10]="";
	char Temp_stringValue[10]="";
	char cVolt_term[]="   V\r";
	volatile uint16_t ReceivedTemperature = 0;
	enum ErrorFlag eErrorFlag = OK;

	Uart_9600();
	Uart_InitRX();
	Adc_InitFreerun(0b00, &Adc_CurrentValue);
	Adc_StartFreerun();
	sei();

	while(1)
	{
		switch(COM_AUTOMAT){
		case RX_:
			if(READY == Eight_ReadStatus(&UsartCurrentData)){
				COM_AUTOMAT = DECODE;
			}
			else if(OVERFLOW == Eight_ReadStatus(&UsartCurrentData)){
				Eight_EraseBuffer(&UsartCurrentData);
				COM_AUTOMAT = RX_;
			}
			else if(BUSY == Eight_ReadStatus(&UsartCurrentData)){
				COM_AUTOMAT = RX_;
			}
			else{
				Uart_InitRX();
				COM_AUTOMAT = RX_;
			}
			break;
		case TX:
			if(BUSY == Eight_ReadStatus(&UsartCurrentData)){
				COM_AUTOMAT = TX;
			}
			else if(FREE == Eight_ReadStatus(&UsartCurrentData)){
				Uart_InitRX();
				COM_AUTOMAT = RX_;
				//COM_AUTOMAT = DECODE;
			}
			else if(OVERFLOW == Eight_ReadStatus(&UsartCurrentData)
					|| READY == Eight_ReadStatus(&UsartCurrentData)){
				Eight_EraseBuffer(&UsartCurrentData);
				Uart_InitRX();
				COM_AUTOMAT = RX_;
				//COM_AUTOMAT = DECODE;
			}
			break;
		case DECODE:
			strcpy(ReceivedString, UsartCurrentData.Data);
			messages_num = DecodeMessages(ReceivedString, &UartMessage[0]);

			switch(UartMessage[0].MessageType){
			case NUMBER:
				Eight_WriteStringToBuffer(&UsartCurrentData,"number\r");
				Uart_InitTX();
				Usart_send(Eight_CopyDataFromBuffer(&UsartCurrentData, 1));
				COM_AUTOMAT = TX;
				break;
			case STRING:
				Eight_WriteStringToBuffer(&UsartCurrentData,"string\r");
				Uart_InitTX();
				Usart_send(Eight_CopyDataFromBuffer(&UsartCurrentData, 1));
				COM_AUTOMAT = TX;
				break;
			case M_COMMAND:
				switch(UartMessage[0].Content.Keyword){
				case ADC_VAL:
					// Convert ADC value
					Adc_Voltage = (double)((double)Adc_CurrentValue/256.0)*5.0;
					dtostrf(Adc_Voltage, 3, 2, Adc_stringValue);
					strcat(Adc_stringValue, cVolt_term);
					// Fill the buffer with that value and start sending
					Eight_WriteStringToBuffer(&UsartCurrentData,Adc_stringValue);
					Uart_InitTX();
					Usart_send(Eight_CopyDataFromBuffer(&UsartCurrentData, 1));
					COM_AUTOMAT = TX;
					break;
				case TEMP:
					InitDS18B20(&eErrorFlag);
					if( 0x01 == Reset_presence()){
						ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
							ReceivedTemperature = Read_Temperature_One_Device();
						}
						sprintf(Temp_stringValue, "%+3d.%04d *C\r", (ReceivedTemperature/16), (ReceivedTemperature & 0x0F)*625);
					}
					else{
						sprintf(Temp_stringValue, "NO SENSOR\r");
					}

					Eight_WriteStringToBuffer(&UsartCurrentData,Temp_stringValue);
					Uart_InitTX();
					Usart_send(Eight_CopyDataFromBuffer(&UsartCurrentData, 1));
					COM_AUTOMAT = TX;
					break;
				case RESET:
					Eight_WriteStringToBuffer(&UsartCurrentData,"                     \r");
					Uart_InitTX();
					Usart_send(Eight_CopyDataFromBuffer(&UsartCurrentData, 1));
					COM_AUTOMAT = TX;
					break;
				default:
					Eight_WriteStringToBuffer(&UsartCurrentData,"Unavailable command\r");
					Uart_InitTX();
					Usart_send(Eight_CopyDataFromBuffer(&UsartCurrentData, 1));
					COM_AUTOMAT = TX;
					break;
				}
				break;
			default:
				Eight_EraseBuffer(&UsartCurrentData);
				COM_AUTOMAT = RX_;
			}
			break;
		}
		_delay_ms(20);
	}
}

