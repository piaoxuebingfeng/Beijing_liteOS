#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "los_base.h"
#include "los_config.h"
#include "los_typedef.h"
#include "los_hwi.h"
#include "los_task.ph"
#include "los_sem.h"
#include "los_event.h"
#include "los_memory.h"
#include "los_queue.ph"
#include "los_swtmr.h"
#include "timer.h"
#include "time.h"
#include "los_sys.h"
#include "usart.h"
#include "che.h"
#include "RGB_LED.h"
UINT32 g_TestTskHandle;
static u8 light_test_flag = 1;



u8 smart_config_key();
void Uart2_process();
VOID task1(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 count=0;
	while(1)
	{
		count++;
		//printf("this is task 1,count is %d\r\n",count);//���ڴ�ӡ����
		LED0=!LED0;
		uwRet = LOS_TaskDelay(200);//����ϵͳ��ʱ
		if(uwRet !=LOS_OK)
			return;
	}
}

UINT32 creat_task1(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;//�������ȼ�
	task_init_param.pcName = "task1";//������
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;//ָ��������ں���
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//���������ջ��С
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);//�������񴴽�����
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}


VOID task_key(void)
{
	UINT32 uwRet = LOS_OK;
	UINT8 key_value;
	while(1)
	{
		key_value=KEY_Scan(0);
		if(key_value==1)
		{
			send24_GRB(0,0,0);
			key_value=0;
		}
		else if(key_value == 2)
		{
			send24_GRB(0,255,0);
			key_value=0;
		}
		smart_config_key();
		uwRet = LOS_TaskDelay(20);//����ϵͳ��ʱ
		if(uwRet !=LOS_OK)
			return;
	}
}

UINT32 creat_task_key(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 2;//�������ȼ�
	task_init_param.pcName = "task_key";//������
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_key;//ָ��������ں���
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//���������ջ��С
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);//�������񴴽�����
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}

/*
 *�������ݴ�������
*/
VOID task2(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 count=0;
	while(1)
	{
		count++;
		Uart2_process();
		//USART_printf(USART2,"task2_count:%d\r\n",count);
		if(USART_RX_STA)
		{
			USART_printf(USART1,"%s\r\n",USART_RX_BUF);
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
			USART_RX_STA=0;
		}
		if(USART2_RX_STA)
		{
			USART_printf(USART2,"%s\r\n",USART2_RX_BUF);
			memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));
			USART2_RX_STA=0;
		}
		uwRet = LOS_TaskDelay(10);
		if(uwRet !=LOS_OK)
		return;
	}
}

UINT32 creat_task2(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;
	task_init_param.pcName = "task2";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task2;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
	if(uwRet !=LOS_OK)
	{
	return uwRet;
	}
	return uwRet;
}

void Uart2_process()
{
	u8 len;
	u8 Red;
	u8 Blue;	 
	u8 Green;
	u8 ID;
	if(USART2_RX_STA&0x8000)
	{
		len=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		ID=USART2_RX_BUF[4];
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_printf(USART1,"%s",USART2_RX_BUF);
	
	if(USART2_RX_BUF[8]=='L')
	{
		
		// move_on();
		
		 light_test_flag=0;
		 send24_GRB(60,50,45);  //�򿪵ƻ�
		 TIM_Cmd(TIM4, ENABLE);
				TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
				TIM4, //TIM4
				TIM_IT_Update ,
				ENABLE
			);
	}
	
	if(USART2_RX_BUF[8]=='l')
	{
		//stop();
			//light_test_flag=1;
//		TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
//		TIM4, //TIM4
//		TIM_IT_Update ,
//		DISABLE
//		);
		 send24_GRB(0,0,0);
	}
	
	if(USART2_RX_BUF[8]=='M')//�Թ��ߴ��������п��� �򿪹��߸�Ӧ����
	{
			//move_back();
		 	light_test_flag=1;
	}
	if(USART2_RX_BUF[8]=='m')//�Թ��ߴ��������п��� �رչ��߸�Ӧ����
	{
		//stop();	
		 light_test_flag=0;
	}
	if(USART2_RX_BUF[8]=='D')
	{				
		send24_GRB(255,255,255);
//		TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
//		TIM3, //TIM3
//		TIM_IT_Update ,
//		ENABLE  //ʹ��
//		);
		//delay_white();
			
	}
	if(USART2_RX_BUF[8]=='d')
	{
//		TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
//		TIM3, //TIM3
//		TIM_IT_Update ,
//		DISABLE  //ʹ��
//		);
		//delay_white();
	}

	if((USART2_RX_BUF[9]=='G')&&(USART2_RX_BUF[10]=='R')&&(USART2_RX_BUF[11]=='B'))//GRB
	{
		light_test_flag=0;
		Green=(USART2_RX_BUF[12]-0x30)*100+(USART2_RX_BUF[13]-0x30)*10+(USART2_RX_BUF[14]-0x30)*1;
		Red=(USART2_RX_BUF[15]-0x30)*100+(USART2_RX_BUF[16]-0x30)*10+(USART2_RX_BUF[17]-0x30)*1;
		Blue=(USART2_RX_BUF[18]-0x30)*100+(USART2_RX_BUF[19]-0x30)*10+(USART2_RX_BUF[20]-0x30)*1;
		send24_GRB(Green,Red,Blue);
		Green=0;
		Red=0;
		Blue=0;
	}
	
	//  ����С��
	// A ǰ��
	// B ����
	// C ��ת
	// D ��ת
//	if(USART2_RX_BUF[8]=='A')
//	{		
//		move_on();
//	}
//	else if(USART2_RX_BUF[8]=='B')
//	{		
//		move_back();
//	}
//	else if(USART2_RX_BUF[8]=='C')
//	{		
//		move_left();
//	}
//	else if(USART2_RX_BUF[8]=='D')
//	{		
//		move_right();
//	}
//	else if(USART2_RX_BUF[8]=='a' || USART2_RX_BUF[8]=='b'||USART2_RX_BUF[8]=='c' ||USART2_RX_BUF[8]=='d')
//	{
//		stop();
//	}
	USART2_RX_STA=0;
	memset(USART2_RX_BUF,0,USART_REC_LEN);
	}
}
//��������
u8 smart_config_key()
{
	u16 i;
	if(KEY_C4==0)
	{
		while(KEY_C4==0)
		{
			i++;
			Delay(0xF0000);
			if(i>10)
			{
				USART_ClearFlag(USART2, USART_FLAG_TC);
				USART_printf(USART2,"AT+CWSMARTSTART=1\r\n");
				//�߰汾�� ESP8266 ATָ� ʹ�������ָ�����SMART-CONF
				//USART_printf(USART2,"AT+CWSTARTSMART=1\r\n");
				Delay(0xff);
				send24_GRB(0,75,75);
			}
		}
	}
}

UINT32 osAppInit(void)
{
	UINT32 uwRet = 0;
	delay_init();
	LED_Init();
	KEY_Init();
	uart_init(9600);
	uart2_init(115200);
	//TIM4_Int_Init(4999,7199);
	LOS_HwiCreate(30,0,0,TIM4_IRQHandler,NULL);  //��ʱ���жϺ���
	LOS_HwiCreate(37,0,0,USART1_IRQHandler,NULL);
	LOS_HwiCreate(38,0,0,USART2_IRQHandler,NULL);
	USART_printf(USART1,"\r\n");
	USART_printf(USART1,"hello liteos\r\n");
	//Example_TskCaseEntry();

	delay_ms(5);
	send_circle_cycle(1,45);
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_printf(USART2,"AT+CIPMUX=1\r\n");
	delay_ms(500);
	send_semicircle();
	
	send24_GRB(0,0,0);
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_printf(USART2,"AT+CIPSERVER=1,8080\r\n");
	send_circle_cycle(1,45);
	LED0=1;	
	delay_ms(300);
	LED0=0;	
	delay_ms(300);
	LED0=0;	
	delay_ms(500);
	
	uwRet = creat_task1();
	if(uwRet !=LOS_OK)
	{
	return uwRet;
	}
	uwRet = creat_task2();
	if(uwRet !=LOS_OK)
	{
	return uwRet;
	}
	uwRet = creat_task_key();
	if(uwRet !=LOS_OK)
	{
	return uwRet;
	}
	return LOS_OK;
}

