/***
	************************************************************************************************************************
	*	@file  	touch_iic.c
	*	@version V1.0
	*  @date    2022-7-12
	*	@author  ���ͿƼ�
	*	@brief   ������IIC�ӿ���غ���
   ************************************************************************************************************************
   *  @description
	*
	*	ʵ��ƽ̨������STM32H750XBH6���İ� ���ͺţ�FK750M5-XBH6��
	*	�Ա���ַ��https://shop212360197.taobao.com
	*	QQ����Ⱥ��536665479
	*		
>>>>>	�ļ�˵����
	*
	*  1.��������ص�IIC����
	* 	2.ʹ��ģ��IIC
	*	3.ͨ���ٶ�Ĭ��Ϊ100KHz
	*
	************************************************************************************************************************
***/

#include "touch_iic.h"  


/*****************************************************************************************
*	�� �� ��: Touch_IIC_GPIO_Config
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: ��ʼ��IIC��GPIO��,�������
*	˵    ��: ����IICͨ���ٶȲ��ߣ������IO���ٶ�����Ϊ2M����
******************************************************************************************/

void Touch_IIC_GPIO_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	Touch_IIC_SCL_CLK_ENABLE;	//��ʼ��IO��ʱ��
	Touch_IIC_SDA_CLK_ENABLE;
	Touch_INT_CLK_ENABLE;	
	Touch_RST_CLK_ENABLE;	
	
	GPIO_InitStruct.Pin 			= Touch_IIC_SCL_PIN;				// SCL����
	GPIO_InitStruct.Mode 		= GPIO_MODE_OUTPUT_OD;			// ��©���
	GPIO_InitStruct.Pull 		= GPIO_NOPULL;						// ����������
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;			// �ٶȵȼ� 
	HAL_GPIO_Init(Touch_IIC_SCL_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 			= Touch_IIC_SDA_PIN;				// SDA����
	HAL_GPIO_Init(Touch_IIC_SDA_PORT, &GPIO_InitStruct);		

	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;      			// �������
	GPIO_InitStruct.Pull  = GPIO_PULLUP;		 					// ����	
	
	GPIO_InitStruct.Pin = Touch_INT_PIN; 							//	INT
	HAL_GPIO_Init(Touch_INT_PORT, &GPIO_InitStruct);				

	GPIO_InitStruct.Pin = Touch_RST_PIN; 							//	RST
	HAL_GPIO_Init(Touch_RST_PORT, &GPIO_InitStruct);					   

	HAL_GPIO_WritePin(Touch_IIC_SCL_PORT, Touch_IIC_SCL_PIN, GPIO_PIN_SET);		// SCL����ߵ�ƽ
	HAL_GPIO_WritePin(Touch_IIC_SDA_PORT, Touch_IIC_SDA_PIN, GPIO_PIN_SET);    // SDA����ߵ�ƽ
	HAL_GPIO_WritePin(Touch_INT_PORT, 	  Touch_INT_PIN,     GPIO_PIN_RESET);  // INT����͵�ƽ
	HAL_GPIO_WritePin(Touch_RST_PORT,     Touch_RST_PIN,     GPIO_PIN_SET);    // RST����ߵ�ƽ

}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_Delay
*	��ڲ���: a - ��ʱʱ��
*	�� �� ֵ: ��
*	��������: ����ʱ����
*	˵    ��: Ϊ����ֲ�ļ�����Ҷ���ʱ����Ҫ�󲻸ߣ����Բ���Ҫʹ�ö�ʱ������ʱ
******************************************************************************************/

void Touch_IIC_Delay(uint32_t a)
{
	volatile uint32_t i;
	while (a --)				
	{
		for (i = 0; i < 200; i++)
        {
            __NOP(); 
        }
	}
}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_INT_Out
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: ����IIC��INT��Ϊ���ģʽ
*	˵    ��: ��
******************************************************************************************/

void Touch_INT_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;      	// ���ģʽ
	GPIO_InitStruct.Pull  = GPIO_PULLUP;		 			// ����	
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;    	// �ٶȵȼ�
	GPIO_InitStruct.Pin   = Touch_INT_PIN ;  				// ��ʼ�� INT ����
	
	HAL_GPIO_Init(Touch_INT_PORT, &GPIO_InitStruct);		
}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_INT_In
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: ����IIC��INT��Ϊ����ģʽ
*	˵    ��: ��
******************************************************************************************/

void Touch_INT_In(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;      		// ����ģʽ
	GPIO_InitStruct.Pull  = GPIO_NOPULL;		 			// ����	
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;    	// �ٶȵȼ�
	GPIO_InitStruct.Pin   = Touch_INT_PIN ;  				// ��ʼ�� INT ����
	
	HAL_GPIO_Init(Touch_INT_PORT, &GPIO_InitStruct);		

}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_Start
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: IIC��ʼ�ź�
*	˵    ��: ��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɸߵ�������Ϊ��ʼ�ź�
******************************************************************************************/

void Touch_IIC_Start(void)
{
	Touch_IIC_SDA(1);		
	Touch_IIC_SCL(1);
	Touch_IIC_Delay(IIC_DelayVaule);
	
	Touch_IIC_SDA(0);
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SCL(0);
	Touch_IIC_Delay(IIC_DelayVaule);
}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_Stop
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: IICֹͣ�ź�
*	˵    ��: ��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�������Ϊ��ʼ�ź�
******************************************************************************************/

void Touch_IIC_Stop(void)
{
	Touch_IIC_SCL(0);
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SDA(0);
	Touch_IIC_Delay(IIC_DelayVaule);
	
	Touch_IIC_SCL(1);
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SDA(1);
	Touch_IIC_Delay(IIC_DelayVaule);
}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_ACK
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: IICӦ���ź�
*	˵    ��: ��SCLΪ�ߵ�ƽ�ڼ䣬SDA�������Ϊ�͵�ƽ������Ӧ���ź�
******************************************************************************************/

void Touch_IIC_ACK(void)
{
	Touch_IIC_SCL(0);
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SDA(0);
	Touch_IIC_Delay(IIC_DelayVaule);	
	Touch_IIC_SCL(1);
	Touch_IIC_Delay(IIC_DelayVaule);
	
	Touch_IIC_SCL(0);		// SCL�����ʱ��SDAӦ�������ߣ��ͷ�����
	Touch_IIC_SDA(1);		
	
	Touch_IIC_Delay(IIC_DelayVaule);

}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_NoACK
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: IIC��Ӧ���ź�
*	˵    ��: ��SCLΪ�ߵ�ƽ�ڼ䣬��SDA����Ϊ�ߵ�ƽ��������Ӧ���ź�
******************************************************************************************/

void Touch_IIC_NoACK(void)
{
	Touch_IIC_SCL(0);	
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SDA(1);
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SCL(1);
	Touch_IIC_Delay(IIC_DelayVaule);
	
	Touch_IIC_SCL(0);
	Touch_IIC_Delay(IIC_DelayVaule);
}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_WaitACK
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: �ȴ������豸����Ӧ���ź�
*	˵    ��: ��SCLΪ�ߵ�ƽ�ڼ䣬����⵽SDA����Ϊ�͵�ƽ��������豸��Ӧ����
******************************************************************************************/

uint8_t Touch_IIC_WaitACK(void)
{
	Touch_IIC_SDA(1);
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SCL(1);
	Touch_IIC_Delay(IIC_DelayVaule);	
	
	if( HAL_GPIO_ReadPin(Touch_IIC_SDA_PORT,Touch_IIC_SDA_PIN) != 0) //�ж��豸�Ƿ���������Ӧ		
	{
		Touch_IIC_SCL(0);	
		Touch_IIC_Delay( IIC_DelayVaule );		
		return ACK_ERR;	//��Ӧ��
	}
	else
	{
		Touch_IIC_SCL(0);	
		Touch_IIC_Delay( IIC_DelayVaule );		
		return ACK_OK;	//Ӧ������
	}
}

/*****************************************************************************************
*	�� �� ��:	Touch_IIC_WriteByte
*	��ڲ���:	IIC_Data - Ҫд���8λ����
*	�� �� ֵ:	ACK_OK  - �豸��Ӧ����
*          	ACK_ERR - �豸��Ӧ����
*	��������:	дһ�ֽ�����
*	˵    ��:��λ��ǰ
******************************************************************************************/

uint8_t Touch_IIC_WriteByte(uint8_t IIC_Data)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		Touch_IIC_SDA(IIC_Data & 0x80);
		
		Touch_IIC_Delay( IIC_DelayVaule );
		Touch_IIC_SCL(1);
		Touch_IIC_Delay( IIC_DelayVaule );
		Touch_IIC_SCL(0);		
		if(i == 7)
		{
			Touch_IIC_SDA(1);			
		}
		IIC_Data <<= 1;
	}

	return Touch_IIC_WaitACK(); //�ȴ��豸��Ӧ
}

/*****************************************************************************************
*	�� �� ��:	Touch_IIC_ReadByte
*	��ڲ���:	ACK_Mode - ��Ӧģʽ������1�򷢳�Ӧ���źţ�����0������Ӧ���ź�
*	�� �� ֵ:	ACK_OK  - �豸��Ӧ����
*          	ACK_ERR - �豸��Ӧ����
*	��������:��һ�ֽ�����
*	˵    ��:1.��λ��ǰ
*				2.Ӧ�������������һ�ֽ�����ʱ���ͷ�Ӧ���ź�
******************************************************************************************/

uint8_t Touch_IIC_ReadByte(uint8_t ACK_Mode)
{
	uint8_t IIC_Data = 0;
	uint8_t i = 0;
	
	for (i = 0; i < 8; i++)
	{
		IIC_Data <<= 1;
		
		Touch_IIC_SCL(1);
		Touch_IIC_Delay( IIC_DelayVaule );
		IIC_Data |= (HAL_GPIO_ReadPin(Touch_IIC_SDA_PORT,Touch_IIC_SDA_PIN) & 0x01);	
		Touch_IIC_SCL(0);
		Touch_IIC_Delay( IIC_DelayVaule );
	}
	
	if ( ACK_Mode == 1 )				//	Ӧ���ź�
		Touch_IIC_ACK();
	else
		Touch_IIC_NoACK();		 	// ��Ӧ���ź�
	
	return IIC_Data; 
}

/********************************************************************************************/
