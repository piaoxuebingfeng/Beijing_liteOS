#ifndef __CHE_H
#define __CHE_H	 
#include "sys.h"


//����  ǰ
#define CHE_LF_M PBout(12)	// PB12  CS
#define CHE_LF_B PBout(13)	// PB13  SCK	
//����  ��
#define CHE_LB_M PBout(14)	// PB14  MISO
#define CHE_LB_B PBout(15)	// PB15  MOSI


//���� ǰ
#define CHE_RF_M PCout(10)	// PC10
#define CHE_RF_B PCout(11)	// PC11

//���� ��

#define CHE_RB_M PCout(12)	// PC12
#define CHE_RB_B PBout(03)	// PB03


void CHE_Init(void);//��ʼ��
void move_on();
void move_back();

void move_left();
void move_right();
void stop();
		 				    
#endif