#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "usart2.h"
#include "timer.h"
#include "gizwits_product.h"
#include "math.h"
#include "adc.h"
#include "stm32f10x_adc.h"
#include "BEEP.h"

//����
int flag=0;
int MQ2_flag,MQ5_flag,MQ9_flag;
int MQ2_threshold,MQ5_threshold,MQ9_threshold;
int beep_flag=0;
uint16_t Heat,MQ2,MQ5,MQ9;/* MQ2:�����⣬MQ5:ú��Һ������⣬MQ9:��ȼ������*/
int main(void)
{	
	int i;
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
// 	Init_LEDpin();			     //LED�˿ڳ�ʼ��
	adc_Init();
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear()  	; 
	TIM3_Int_Init(71,999);				//һ���붨ʱ
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	usart3_init((u32)9600);//WIFI��ʼ��
	gizwitsInit();//��������ʼ��
//	gizwitsSetMode(WIFI_RESET_MODE);
	delay_ms(500);
//	gizwitsSetMode(WIFI_AIRLINK_MODE);
	gizwitsSetMode(WIFI_SOFTAP_MODE);
	BEEP_Init();

	OLED_ShowString(0,2,"MQ2:",16);
	OLED_ShowString(0,4,"MQ5:",16);
	OLED_ShowString(0,6,"MQ9:",16);

	while(1)
	{

		
		/*��ȡ��������ֵ*/
		MQ2 = AD_GetValue(ADC_Channel_1);
		MQ5 = AD_GetValue(ADC_Channel_2);
		MQ9 = AD_GetValue(ADC_Channel_3);
		
		

		
		//������ر�������ֵ���
		if(MQ2>MQ2_threshold&&beep_flag==1)//�����ⱨ��
		{
			MQ2_flag=1;
			BEEP_ON();
		}
		else
		{
			MQ2_flag=0;
		}
		///////////////////////
		if(MQ5>MQ5_threshold&&beep_flag==1)//ú����ⱨ��
		{
			MQ5_flag=1;
			BEEP_ON();
		}
		else
		{
			MQ5_flag=0;
		}
		///////////////////////
		if(MQ9>MQ9_threshold&&beep_flag==1)//һ����̼��ⱨ��
		{
			MQ9_flag=1;
			BEEP_ON();
		}
		else
		{
			MQ9_flag=0;
		}
		
		userHandle();
    gizwitsHandle((dataPoint_t *)&currentDataPoint);
		

		OLED_ShowNum(70,2,MQ2,3,16);
		OLED_ShowNum(70,4,MQ5,3,16);
		OLED_ShowNum(70,6,MQ9,3,16);
}
}









