/*********************************************************************************************************************
 * @file       	camera.c
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-25
 * @note				Camera								PIN
 *							八位数据(D0-D7)				PE0-PE7
 *							像素中断(PCLK)				PE9
 *							场中断(VSY)						PE11
 *							行中断(HREF)					PE10
 *							51单片机TX						PC16
 *							51单片机RX						PC17
 ********************************************************************************************************************/

#include "camera.h"
#include "MK60_dma.h"
#include "stdbool.h"

uint8   image[ROW][COL];      //图像数组
uint8		image_binary[ROW][COL/8];
uint8   receive[3];
uint8   receive_num = 0;
uint8   uart_receive_flag = 1;
uint8 THRESHOLD= 130;
uint8 Left_Line[50];
uint8 Left_Add[50];
uint8 Mid_Line[50];
uint8 Right_Add[50];
uint8 Right_Line[50];
uint8 Left_Line_New[50];
uint8 Right_Line_New[50];
uint8 width[50];
uint8 detect_image[35][184];
uint8 certainty[3];
uint8 stop_flag=0;
uint8 Mid_Count=40;
uint8 Left_Line_New[50];
uint8 Right_Line_New[50];
uint8 width_Min;
uint8 Left_Max;
uint8 Right_Min;

uint8	Left_Max = 2;
uint8	Right_Min = 78;
uint8	Left_y = 59;
uint8	Right_y = 59;
uint8	Width_Min = 80;
uint8	Repair_Flag = 0;
uint8	Left_Add_Start = 0;
uint8	Right_Add_Start = 0;
uint8	Left_Add_End = 0;
uint8	Right_Add_End = 0;
uint8	Foresight_Left = 2;
uint8	Foresight_Right = 78;

//需要配置到摄像头的数据
int16 MT9V032_CFG[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
                              //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
    {EXP_TIME,          300}, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
    {FPS,               50},  //图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
    {SET_COL,           COL}, //图像列数量        范围1-752     K60采集不允许超过188
    {SET_ROW,           ROW}, //图像行数量        范围1-480
    {LR_OFFSET,         0},   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {UD_OFFSET,         0},   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {GAIN,              32},  //图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度
    {INIT,              0}    //摄像头开始初始化
};

//从摄像头内部获取到的配置数据
int16 GET_CFG[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},   //自动曝光设置      
    {EXP_TIME,          0},   //曝光时间          
    {FPS,               0},   //图像帧率          
    {SET_COL,           0},   //图像列数量        
    {SET_ROW,           0},   //图像行数量        
    {LR_OFFSET,         0},   //图像左右偏移量    
    {UD_OFFSET,         0},   //图像上下偏移量    
    {GAIN,              0},   //图像增益          
};
///////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************
 *	@brief	MT9V032摄像头串口中断函数
 *	@param	无
 *	@return	无
 *	@note		该函数在ISR文件 串口3中断程序被调用
 ***********************************************************************/
void MT9V032_Uart_Receive(void)
{
	uart_getchar(uart3,&receive[receive_num]);;
	receive_num++;
	
	if(1==receive_num && 0XA5!=receive[0])  receive_num = 0;
	if(3 == receive_num)
	{
		receive_num = 0;
		uart_receive_flag = 1;
	}
}

/************************************************************************
 *	@brief	获取配置数据
 *	@param	无
 *	@return	无
 *	@note		无
 ***********************************************************************/
void Get_Config(void)
{
	uint16 temp, i;
	uint8  send_buffer[4];

	for(i=0; i<CONFIG_FINISH-1; i++)
	{
		send_buffer[0] = 0xA5;
		send_buffer[1] = GET_STATUS;
		temp = GET_CFG[i][0];
		send_buffer[2] = temp>>8;
		send_buffer[3] = (uint8)temp;
		uart_putbuff(uart3,send_buffer,4);
		//等待接受回传数据
		while(!uart_receive_flag);
		uart_receive_flag = 0;
		GET_CFG[i][1] = receive[1]<<8 | receive[2];
	}
}

/************************************************************************
 *	@brief	获取摄像头固件版本
 *	@param	无
 *	@return	无
 *	@note		无
 ***********************************************************************/
uint16 Get_Version(void)
{
	uint16 temp;
	uint8  send_buffer[4];
	send_buffer[0] = 0xA5;
	send_buffer[1] = GET_STATUS;
	temp = GET_VERSION;
	send_buffer[2] = temp>>8;
	send_buffer[3] = (uint8)temp;
	
	uart_putbuff(uart3,send_buffer,4);
			
	//等待接受回传数据
	while(!uart_receive_flag);
	uart_receive_flag = 0;
	
	return ((uint16)(receive[1]<<8) | receive[2]);
}


/************************************************************************
 *	@brief	单独设置摄像头曝光时间
 *	@param	light   设置曝光时间越大图像越亮
 *									摄像头收到后会根据分辨率及FPS计算最大曝光时间
 *									如果设置的数据过大，那么摄像头将会设置这个最大值
 *	@return	当前曝光值，用于确认是否正确写入
 *	@note		无
 ***********************************************************************/
uint16 Set_Exposure_Time(uint16 light)
{
	uint16 temp;
	uint8  send_buffer[4];

	send_buffer[0] = 0xA5;
	send_buffer[1] = SET_EXP_TIME;
	temp = light;
	send_buffer[2] = temp>>8;
	send_buffer[3] = (uint8)temp;
	
	uart_putbuff(uart3,send_buffer,4);
	
	//等待接受回传数据
	while(!uart_receive_flag);
	uart_receive_flag = 0;
	
	temp = receive[1]<<8 | receive[2];
	return temp;
}

/************************************************************************
 *	@brief	对摄像头内部寄存器进行写操作
 *	@param  addr    摄像头内部寄存器地址
 *	@param  data    需要写入的数据
 *	@return	寄存器当前数据，用于确认是否写入成功
 *	@note		无
 ***********************************************************************/
uint16 Set_MT9V032_Reg(uint8 addr, uint16 data)
{
	uint16 temp;
	uint8  send_buffer[4];
	
	send_buffer[0] = 0xA5;
	send_buffer[1] = SET_ADDR;
	temp = addr;
	send_buffer[2] = temp>>8;
	send_buffer[3] = (uint8)temp;
	
	uart_putbuff(uart3,send_buffer,4);
	for(int i = 0; i < 10000; i++);
	
	send_buffer[0] = 0xA5;
	send_buffer[1] = SET_DATA;
	temp = data;
	send_buffer[2] = temp>>8;
	send_buffer[3] = (uint8)temp;
	
	uart_putbuff(uart3,send_buffer,4);
	
	//等待接受回传数据
	while(!uart_receive_flag);
	uart_receive_flag = 0;
	
	temp = receive[1]<<8 | receive[2];
	return temp;
}

/************************************************************************
 *	@brief	摄像头初始化
 *	@param	无
 *	@return	无
 *	@note		无
 ***********************************************************************/
void Camera_Init(void)
{
	//设置参数    具体请参看使用手册
	uint16 temp, i;
	uint8  send_buffer[4];


	uart_init (uart3, 9600);	//初始换串口 配置摄像头    
	uart_rx_irq_en(uart3);


	//等待摄像头上电初始化成功
	for(int j = 0; j < 100; j++)//延时1s
	{
		for(int k = 0; k < 1000; k++);
	}
	uart_receive_flag = 0;

	//开始配置摄像头并重新初始化
	for(i=0; i<CONFIG_FINISH; i++)
	{
			send_buffer[0] = 0xA5;
			send_buffer[1] = MT9V032_CFG[i][0];
			temp = MT9V032_CFG[i][1];
			send_buffer[2] = temp>>8;
			send_buffer[3] = (uint8)temp;
			
			uart_putbuff(uart3,send_buffer,4);
			for(int j = 0; j < 1000; j++);//延时2ms
	}
	//等待摄像头初始化成功
	while(!uart_receive_flag);
	uart_receive_flag = 0;
	while((0xff != receive[1]) || (0xff != receive[2]));
	//以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
	//利用Set_Exposure_Time函数单独配置的曝光数据不存储在eeprom中


	//获取配置便于查看配置是否正确
	Get_Config();


	//摄像头采集初始化
	DisableInterrupts;
	//DMA通道0初始化，PTE9触发源(默认上升沿)，源地址为(PTE0-PTE7)，目的地址为：image，每次传输1Byte 传输完毕保持目的地址
	dma_portx2buff_init(DMA_CH0, (void *)&E_PDIR8(0), (void *)image, E9, DMA_BYTE1, COL*ROW, DADDR_KEEPON);
	port_init(E9, ALT1 | DMA_RISING | PULLDOWN);  		//PCLK  触发源设置
	DMA_DIS(DMA_CH0);                                 //禁用DMA通道
	DMA_IRQ_CLEAN(DMA_CH0);                           //清除通道传输中断标志位
	DMA_IRQ_EN(DMA_CH0);                              //允许DMA通道中断
	DMA_EN(DMA_CH0);  																//使能DMA

	disable_irq(PORTE_IRQn);                       		//关闭PTC的中断
	//port_init(E10, ALT1 | IRQ_FALLING | PULLDOWN);		//行中断
	port_init(E11, ALT1 | IRQ_FALLING | PULLDOWN);  		//场中断，下降沿触发中断、下拉
	set_irq_priority(PORTE_IRQn,1);                		//中断优先级
	enable_irq (PORTE_IRQn);
	EnableInterrupts;
}

uint8   Image_Finish_Flag = 0;      //一场图像采集完成标志位
/************************************************************************
 *	@brief	MT9V032摄像头场中断
 *	@param	无
 *	@return	无
 *	@note		在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
 ***********************************************************************/
void VSYNC(void)
{
	if(!Image_Finish_Flag)
		dma_repeat(DMA_CH0,(void *)&E_PDIR8(0),(void *)image[0],COL*ROW);
}


/************************************************************************
 *	@brief	MT9V032摄像头DMA完成中断
 *	@param	无
 *	@return	无
 *	@note		在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
 ***********************************************************************/
void Row_Finished(void)
{
	Image_Finish_Flag = 1;
	//本例程对黑边不做处理，大家在使用数据的时候不使用image数组最左边与最右边即可
	//如果分辨率过大，就没办法转存了，因为K60的RAM会不够
  //一副图像从采集开始到采集结束耗时3.33MS左右(50FPS、188*120分辨率)
}

/************************************************************************
 *	@brief	发送图像
 *	@param	无
 *	@return	无
 *	@note		无
 ***********************************************************************/
void Send_Image(void)
{
	uart_putchar(DEBUG_PORT,0x00);
	uart_putchar(DEBUG_PORT,0xff);
	uart_putchar(DEBUG_PORT,0x01);
	uart_putchar(DEBUG_PORT,0x01);//发送命令
  uart_putbuff(DEBUG_PORT, (uint8_t *)image_binary, ROW*(COL/8));  //发送图像
	//uart_putbuff(DEBUG_PORT, (uint8_t *)image, ROW*COL);
}

void Image_manage()
{
	int i ;
			First_linecope((uint8 *)image);
		for (i=ROW-2;i >= ROW-Mid_Count;i--)
		{			
			Traversal_Mid_Line(i,(uint8 *)image);
		}
}
	

void Image_Binary()
{
	uint8 pos=0;
	uint8 tmpdst;
	uint8 j;
	uint8 i;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL ; j++)
		{
			if (image[i][j] < THRESHOLD) {image[i][j]=0;}
			else {image[i][j]=1;}
//			pos++;
//			if (pos == 8)
//			{
//				pos = 0;
//				image_binary[i][k] = tmpdst;
//				k+=1;
//			}
			//printf("pos=%d [%d][%d]\n",pos,i,k);
		}
	}

	
		
}

void Image_recontract()
{
	uint8 pos=0;
	uint8 tmpdst;
	uint8 k=0;
	uint8 j;
	uint8 i;
	for (i = 0; i < ROW; i++)
	{
		k=0;
		for (j = 0; j < COL ; j++)
		{
			if (!image[i][j]) {tmpdst = (tmpdst<<1);}
			else {tmpdst = (tmpdst<<1)| 0x01;}
			pos++;
			if (pos == 8)
			{
				pos = 0;
				image_binary[i][k] = tmpdst;
				k+=1;
			}
		}
	}
}

void First_linecope(uint8 * data)
{
	uint8 j;
	uint8 i=ROW-1;
	static uint8 c_pos=0;
	static uint8 c_res=0;
	uint8 left_dc=0;
	uint8 right_dc=0;
	
	if (data[184*i+COL/2])
	{
		Mid_Line[49] = COL/2;
	}
	else if(data[184*i+ COL/3])
	{
		Mid_Line[49]= COL/3;
	}
	else if(data[(int)(184*i+COL*3.0f/4)])
	{
		Mid_Line[49]= COL*3.0f/4;
	}
	
	for (j = Mid_Line[49]; j >= 2; j--)	//以前一行中点为起点向左查找边界
	{
		if (!data[184*i+j] && !data[184*i+j-1])//检测到连续两个黑点，起到滤波的效果
		{
			Left_Add[i] = 0;		 //左边界不需要补线，清除标志位
			Left_Line[i] = j;		 //记录当前j值为第i行左边界
//			data[80 * i + j + 2] = 0;//右移2位显示左边界，方便调试观察
			
			break;
		}
	}
	if (Left_Add[i])
	{
		Left_Line[i] = 0;
	}
	for (j = Mid_Line[49]; j < 183; j++)//以前一行中点为起点向右查找右边界
	{
		if (!data[184*i+j] && !data[184*i+j+1])//检测到连续两个黑点，起到滤波的效果
		{
			Right_Add[i] = 0;		 //右边界不需要补线，清除标志位
			Right_Line[i] = j;		 //记录当前j值为第i行右边界
//			data[80 * i + j - 2] = 0;//左移2位显示右边界，方便调试观察
			
			break;
		}
	}
	if (Right_Add[i])
	{
		Right_Line[i] = COL-1;
	}
	Mid_Line[i] = (Right_Line[i] + Left_Line[i])/2 ;
	width[i] = Right_Line[i] - Left_Line[i];

}
void Traversal_Mid_Line(uint8 i, uint8 *data)	//从中间向两边扫描边界
{
	uint8 j;
	static uint8 c_pos=0;
	static uint8 c_res=0;
	//Mid_Line[49]= 92;
	uint8 left_dc=0;
	uint8 right_dc=0;
	for (j = Mid_Line[i + 1]; j >= 2; j--)	//以前一行中点为起点向左查找边界
	{
		if (!data[184*i+j] && !data[184*i+j-1])//检测到连续两个黑点，起到滤波的效果
		{
			Left_Add[i] = 0;		 //左边界不需要补线，清除标志位
			Left_Line[i] = j;		 //记录当前j值为第i行左边界
//			data[80 * i + j + 2] = 0;//右移2位显示左边界，方便调试观察
			
			break;
		}
	}
	if (Left_Add[i])
	{
		Left_Line[i] = 0;
	}
	for (j = Mid_Line[i + 1]; j < 183; j++)//以前一行中点为起点向右查找右边界
	{
		if (!data[184*i+j] && !data[184*i+j+1])//检测到连续两个黑点，起到滤波的效果
		{
			Right_Add[i] = 0;		 //右边界不需要补线，清除标志位
			Right_Line[i] = j;		 //记录当前j值为第i行右边界
//			data[80 * i + j - 2] = 0;//左移2位显示右边界，方便调试观察
			
			break;
		}
	}
	if (Right_Add[i])
	{
		Right_Line[i] = COL-1;
	}
	Mid_Line[i] = (Right_Line[i] + Left_Line[i])/2 ;
	width[i] = Right_Line[i] - Left_Line[i];
	//printf("width[%d] = %d\n",i,width[i]);
	if (width[i] < 25 && Mid_Line[i+1]!=0&&width[i]!=0)
	{
				for (j = Left_Line[i]; j >= 2; j--)	//以前一行中点为起点向左查找边界
		{
			if (data[184*i+j] && data[184*i+j-1])//检测到连续两个白点，起到滤波的效果
			{

				if (Left_Line[i]-j < 20)
				{
					left_dc=1;
				}
				else
					left_dc=0;
				break;
			}
		}
			for (j = Right_Line[i]; j < 183; j++)//以前一行中点为起点向右查找右边界
	{
		if (!data[184*i+j] && !data[184*i+j+1])//检测到连续两个白点，起到滤波的效果
		{
						if (j-Right_Line[i] < 20)
				{
					right_dc=1;
				}
				else
					right_dc=0;

			
			break;
		}
	}
	if (right_dc&&left_dc)
	{
		//printf("check maybe start\n");
		c_pos++;
		if (c_pos==4)
		{
			//printf("realy check start\n");
			c_pos=0;
						if(c_res == 8)
			{
				printf("8 check\n");
				stop_flag=1;
				c_res=0;
			}
			c_res++;
		}
	}
	else
	{
		c_pos = 0;
	}
	}
}

void iteration_Thresholdfigure()//迭代阈值计算
{
	int Low_temp;
	int high_temp;
	uint8 T0;
	uint8 T1;
	uint8 max_G=0;
	uint8 min_G=255;
	uint8 i,j;
	uint16 h_num;
	uint16 l_num;
		for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL ; j++)
		{
			if (min_G>image[i][j])
			{
				min_G = image[i][j];
			}
			if (max_G<image[i][j])
			{
				max_G=image[i][j];
			}
		}
	}
	T0 = min_G+max_G/2; 
	while (1)
	{
		high_temp = 0;
		Low_temp = 0;
		l_num=0;
		h_num=0;
				for (i = 0; i < ROW; i++)
		{
			for (j = 0; j < COL ; j++)
			{
				if (image[i][j]>=T0)
				{
					high_temp+=image[i][j];
					h_num++;
				}
				else
				{
					Low_temp +=image[i][j];
					l_num++;
				}
			}
		}
		T1 = (high_temp/h_num+Low_temp/l_num)/2;
		if (T1 == T0)
		{
			THRESHOLD = 1.4*T0;
			break;
		}
		else
		{
			T0 = T1;
		}
	}

}


void Mid_Filtering(uint8 *data)		//中线滤波
{
	uint8 i, Count;
	
	Count = ROW - Mid_Count;	//判断一共有多少多少可用中点
	for (i = ROW-2; i > Count; i--)	//从下向上滤波
	{
		//printf("ML[%d] = %d\n",i,Mid_Line[i]);
		Mid_Line[i] = Mid_Line[i+1]*0.3 + Mid_Line[i]*0.7;
	}
	for (i = Count; i < COL-1; i++)	//从上向下互补滤波
	{
		Mid_Line[i] = Mid_Line[i-1]*0.3 + Mid_Line[i]*0.7;
	}
	for (i = ROW-1; i >= Count+3; i--)
	{
		data[184*i + Mid_Line[i]] = 0;	//将中线在图像上显示出来
	}
}

void Traversal_Left_Side(uint8 i, uint8 *data)
{
	uint8 j;
	
	for (j = Left_Line[i+1]; j < 78; j++)
	{
//		if (data[80*i+j] && data[80*i+j-1])//检测到连续两个白点，起到滤波的效果
		if (data[80*i+j])//检测到白点
		{
			Left_Add[i] = 0;		 //左边界不需要补线，清除标志位
			Left_Line[i] = j;		 //记录当前j值为第i行左边界
			
			break;
		}
	}
}

void Traversal_Right_Side(uint8 i, uint8 *data)
{
	uint8 j;
	
	for (j = Right_Line[i+1]; j > 2; j--)
	{
//		if (data[80*i+j] && data[80*i+j-1])//检测到连续两个白点，起到滤波的效果
		if (data[80*i+j])//检测到白点
		{
			Right_Add[i] = 0;		 //右边界不需要补线，清除标志位
			Right_Line[i] = j;		 //记录当前j值为第i行右边界
			
			break;
		}
	}
}

void Repair_check(uint8 i)
{
	
	if (Left_Add[i+1])	//前一行补线了
	{
		if (Left_Line[i] < Left_Line_New[i+1]-1)	//与前一行的左边界实线比较	
		{
			Left_Add[i] = 1;
		}
	}
	else	//前一行没有补线
	{
		if (Left_Line[i] < Left_Line[i+1]-1)	//与前一行的左边界实线比较	
		{
			Left_Add[i] = 1;
		}
	}
	
	if (Right_Add[i+1])	//前一行右边界补线了	
	{
		if (Right_Line[i] > Right_Line_New[i+1]+1)	//与前一行的右边界实线进行比较
		{
			Right_Add[i] = 1;
		}
	}
	else	//前一行右边界没有补线
	{
		if (Right_Line[i] > Right_Line[i+1]+1)		//与前一行的右边界实线进行比较
		{
			Right_Add[i] = 1;
		}
	}
}
