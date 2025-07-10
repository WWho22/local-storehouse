#include "includes.h"


//°´¼ü³õÊ¼»¯
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIOE_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIOE_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIOE_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIOE_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_15;
	GPIOE_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOE_InitStructure.GPIO_Speed = GPIO_Low_Speed;
	
	GPIO_Init(GPIOE,&GPIOE_InitStructure);
}





