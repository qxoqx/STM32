#include "adc.h"
#ifdef ADC_H

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

__IO uint16_t ADC_ConvertedValue[3];


// /*
//  * 函数名：ADC1_GPIO_Config
//  * 描述  ：使能ADC1和DMA1的时钟，初始化PC.01
//  * 输入  : 无
//  * 输出  ：无
//  * 调用  ：内部调用
//  */
// static void ADC1_GPIO_Config(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;
	
// 	/* Enable DMA clock */
// 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
// 	/* Enable ADC1 and GPIOC clock */
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
	
// 	/* Configure PC.01  as analog input */
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);				// PC1,输入时不用设置速率
// }


// /* 函数名：ADC1_Mode_Config
//  * 描述  ：配置ADC1的工作模式为MDA模式
//  * 输入  : 无
//  * 输出  ：无
//  * 调用  ：内部调用
//  */
// static void ADC1_Mode_Config(void)
// {
// 	DMA_InitTypeDef DMA_InitStructure;
// 	ADC_InitTypeDef ADC_InitStructure;
	
// 	/* DMA channel1 configuration */
// 	DMA_DeInit(DMA1_Channel1);
// 	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC地址
// 	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;//内存地址
// 	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
// 	DMA_InitStructure.DMA_BufferSize = 1;
// 	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
// 	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //内存地址固定
// 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
// 	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
// 	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
// 	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
// 	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
// 	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
// 	/* Enable DMA channel1 */
// 	DMA_Cmd(DMA1_Channel1, ENABLE);
	
// 	/* ADC1 configuration */
	
// 	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
// 	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 //禁止扫描模式，扫描模式用于多通道采集
// 	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
// 	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
// 	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
// 	ADC_InitStructure.ADC_NbrOfChannel = 1;	 	//要转换的通道数目1
// 	ADC_Init(ADC1, &ADC_InitStructure);
	
// 	/*配置ADC时钟，为PCLK2的8分频，即9Hz*/
// 	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
// 	/*配置ADC1的通道11为55.	5个采样周期，序列为1 */ 
// 	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
	
// 	/* Enable ADC1 DMA */
// 	ADC_DMACmd(ADC1, ENABLE);
	
// 	/* Enable ADC1 */
// 	ADC_Cmd(ADC1, ENABLE);
	
// 	/*复位校准寄存器 */   
// 	ADC_ResetCalibration(ADC1);
// 	/*等待校准寄存器复位完成 */
// 	while(ADC_GetResetCalibrationStatus(ADC1));
	
// 	/* ADC校准 */
// 	ADC_StartCalibration(ADC1);
// 	/* 等待校准完成*/
// 	while(ADC_GetCalibrationStatus(ADC1));
	
// 	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
// 	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
// }

/*
 * 函数名：ADC1_GPIO_Config
 * 描述  ：使能ADC1和DMA1的时钟，初始化PC.01
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
	
	/* Configure PC.01  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;				 //初始化引脚1,2,3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);				// PC1,输入时不用设置速率
}


/* 函数名：ADC1_Mode_Config
 * 描述  ：配置ADC1的工作模式为MDA模式
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址固定
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //禁止扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 3;	 	//要转换的通道数目1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*配置ADC时钟，为PCLK2的8分频，即9Hz*/ //6  12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*配置ADC1的通道11为55.	5个采样周期，序列为1 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 3, ADC_SampleTime_28Cycles5);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*复位校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADC校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// void ADC1S_Init(void)//转换周期 (1/72000000)*6*(12.5+28.5)=3.41us     3.41*5=17.5us
// {
// 	ADC_InitTypeDef ADC_InitStructure;					//ADC初始化结构体
// 	DMA_InitTypeDef DMA_InitStructure;					//DMA初始化结构体
// 	GPIO_InitTypeDef  GPIO_InitStructure;				//GPIO初始化结构体
// 	RCC_ADCCLKConfig(RCC_PCLK2_Div6);					//PLL主时钟（若为72MHz）6分频，作为ADC时钟（12MHz，最大为14MHz）
// 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);   //使能DMA1时钟
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 	| RCC_APB2Periph_AFIO, ENABLE);						//使能ADC1时钟，和引脚复用时钟
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;				 //初始化引脚1,2,3
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		 //模拟输入
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);				 //初始化GPIOA	

// 	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	 //ADC工作在独立模式
// 	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		 //开启扫描模式
// 	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	 //连续转换
// 	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发转换开关关闭
// 	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐方式，右对齐
// 	ADC_InitStructure.ADC_NbrOfChannel = 5;				 //开启5个转换通道
// 	ADC_Init(ADC1, &ADC_InitStructure);					 //初始化ADC1	
// 	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_28Cycles5);//配置ADC1通道0，转换顺序为第1顺位，采样时间28.5个周期    
// 	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_28Cycles5);
// 	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_28Cycles5);
// 	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_28Cycles5);
// 	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_28Cycles5);	
// 	ADC_DMACmd(ADC1, ENABLE); 							//开启ADC1的DMA通道
// 	ADC_Cmd(ADC1, ENABLE);								//开启ADC1
// 	ADC_ResetCalibration(ADC1);							//复位ADC1的校准寄存器
// 	while(ADC_GetResetCalibrationStatus(ADC1));			//等待复位完成
// 	ADC_StartCalibration(ADC1);							//开始指定ADC1的校准状态
// 	while(ADC_GetCalibrationStatus(ADC1));				//获取状态，若为设置状态则等待
// 	ADC_SoftwareStartConvCmd(ADC1, ENABLE);				//开启ADC1（激活软件触发开始转换）	
// 	DMA_DeInit(DMA1_Channel1);							//将DMA的通道1寄存器重设为缺省值
// 	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;//DMA外设ADC基地址
// 	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC1_ConvertedValues;	//DMA内存基地址
// 	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	 //内存作为数据传输的目的地
// 	DMA_InitStructure.DMA_BufferSize = 5;				 //DMA通道的DMA缓存的大小
// 	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不变
// 	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//内存地址寄存器递增
// 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度为16位
// 	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度为16位
// 	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		    //工作在循环缓存模式
// 	DMA_InitStructure.DMA_Priority = DMA_Priority_High;		//DMA通道1拥有高优先级
// 	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			//DMA通道x没有设置为内存到内存传输
// 	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  			//初始化DMA的通道	
// 	DMA_Cmd(DMA1_Channel1, ENABLE);							 //启动DMA通道
// }

/*
 * 函数名：ADC1_Init
 * 描述  ：无
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}


#endif
