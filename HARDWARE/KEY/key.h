#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


//#define KEY0 PCin(5)   	
//#define KEY1 PAin(15)	 
//#define WK_UP  PAin(0)	 
 
#define KEY_C4  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)//��ȡ����C4
#define KEY_C5  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//��ȡ����C5
 
#define KEYC4_PRES	1		//KEYC4_PRES  
#define KEYC5_PRES	2		//KEYC5_PRES 


void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8 mode);  	//����ɨ�躯��					    
#endif
