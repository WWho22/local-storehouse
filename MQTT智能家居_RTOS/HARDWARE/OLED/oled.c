#include "includes.h"
//#include "OLED_Font.h"

/**/
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)(x))

/**/
#define I2CT_FLAG_TIMEOUT   ((uint32_t)0x1000) 
#define I2CT_LONG_TIMEOUT   ((uint32_t)(10 * I2CT_FLAG_TIMEOUT)) 
#define OLED_Address         0x01

uint32_t I2C2_Timeout;

/**/
//void OLED_I2C_Init(void)
//{
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	OLED_W_SCL(1);
//	OLED_W_SDA(1);
//}


//OLED初始化
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIOB_InitStructure;
	I2C_InitTypeDef   IIC2_InitStructure;
//	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	
	/* 连接引脚源 PB10 到 I2C_SCL*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_I2C2);
	/* 连接引脚源 PB11 到 I2C_SDA*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_I2C2);
	
	GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIOB_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIOB_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOB_InitStructure.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(GPIOB,&GPIOB_InitStructure);
	
	IIC2_InitStructure.I2C_Ack = I2C_Ack_Enable;
	IIC2_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	IIC2_InitStructure.I2C_ClockSpeed = 100000;
	IIC2_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	IIC2_InitStructure.I2C_Mode = I2C_Mode_I2C;
	IIC2_InitStructure.I2C_OwnAddress1 = 0x0A;
	I2C_Init(I2C2,&IIC2_InitStructure);
	
	I2C_Cmd(I2C2,ENABLE);
}



/**
  * @brief  硬件I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */

uint32_t OLED_I2C2_SendByte(I2C_TypeDef* I2Cx,char xByte)
{
	I2C_GenerateSTART(I2Cx,ENABLE);
	I2C2_Timeout = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2C2_Timeout--)==0) return 0;
	}
	I2C_Send7bitAddress(I2Cx,OLED_Address,I2C_Direction_Transmitter);
	I2C2_Timeout = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2C2_Timeout--)==0) return 0;
	}
	I2C_SendData(I2Cx,xByte);
	
	
	return 0;
}



//void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
//{
//	uint32_t count = 10000;
//	while(I2C_CheckEvent(I2Cx,I2C_EVENT)!=SUCCESS)
//	{
//		count--;
//		if(count==0)
//		{
//			break;
//		}
//	}
//}


//void MPU6050_SendByte(uint8_t Reg_Address,uint8_t Data)
//{	
//	I2C_GenerateSTART(I2C2,ENABLE);
//	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);
//	
//	I2C_Send7bitAddress(I2C2,MPU6050_Address,I2C_Direction_Transmitter);
//	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
//	
//	I2C_SendData(I2C2,Reg_Address);
//	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING);
//	
//	I2C_SendData(I2C2,Data);
//	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);	
//	
//	I2C_GenerateSTOP(I2C2,ENABLE);
//}

//uint8_t MPU6050_ReadByte(uint8_t Reg_Address)
//{
//	uint8_t ReadData = 0;
//	
//	I2C_GenerateSTART(I2C2,ENABLE);
//	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);
//	
//	I2C_Send7bitAddress(I2C2,MPU6050_Address,I2C_Direction_Transmitter);
//	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
//	
//	I2C_SendData(I2C2,Reg_Address);
//	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING);
//	
//	I2C_GenerateSTART(I2C2,ENABLE);
//	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);

//	I2C_Send7bitAddress(I2C2,MPU6050_Address,I2C_Direction_Receiver);
//	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);

//	I2C_AcknowledgeConfig(I2C2,DISABLE);
//	I2C_GenerateSTOP(I2C2,ENABLE);
//	
//	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED);
//	
//	ReadData = I2C_ReceiveData(I2C2);
//	
//	I2C_AcknowledgeConfig(I2C2,ENABLE);
//	
//	return ReadData;
//}

//void MPU6050_Init(void)
//{
//	IIC_Init();
//	MPU6050_SendByte(PWR_MGMT_1,0x01);
//	MPU6050_SendByte(PWR_MGMT_2,0x00);
//	MPU6050_SendByte(SMPLRT_DIV,0x09);
//	MPU6050_SendByte(CONFIG,0x06);
//	MPU6050_SendByte(GYRO_CONFIG,0x18);
//	MPU6050_SendByte(ACCEL_CONFIG,0x18);
//}

//void Get_MPU6050_Data(MPU6050_DATA *mpu6050_data)
//{
//	mpu6050_data->Acc_X = (MPU6050_ReadByte(ACCEL_XOUT_H)<<8)|MPU6050_ReadByte(ACCEL_XOUT_L);
//	mpu6050_data->Acc_Y = (MPU6050_ReadByte(ACCEL_YOUT_H)<<8)|MPU6050_ReadByte(ACCEL_YOUT_L);
//	mpu6050_data->Acc_Z = (MPU6050_ReadByte(ACCEL_ZOUT_H)<<8)|MPU6050_ReadByte(ACCEL_ZOUT_L);
//	mpu6050_data->Gyro_X = (MPU6050_ReadByte(GYRO_XOUT_H)<<8)|MPU6050_ReadByte(GYRO_XOUT_L);
//	mpu6050_data->Gyro_Y = (MPU6050_ReadByte(GYRO_YOUT_H)<<8)|MPU6050_ReadByte(GYRO_YOUT_L);
//	mpu6050_data->Gyro_Z = (MPU6050_ReadByte(GYRO_ZOUT_H)<<8)|MPU6050_ReadByte(GYRO_ZOUT_L);
//}

//uint8_t Get_MPU6050_ID(void)
//{
//	return MPU6050_ReadByte(WHO_AM_I);
//}







