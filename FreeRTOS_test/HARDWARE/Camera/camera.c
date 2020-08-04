/*********************************************************************************************************************
 * @file       	camera.c
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-25
 * @note				Camera								PIN
 *							��λ����(D0-D7)				PE0-PE7
 *							�����ж�(PCLK)				PE9
 *							���ж�(VSY)						PE11
 *							���ж�(HREF)					PE10
 *							51��Ƭ��TX						PC16
 *							51��Ƭ��RX						PC17
 ********************************************************************************************************************/

#include "camera.h"
#include "MK60_dma.h"
#include "stdbool.h"

uint8   image[ROW][COL];      //ͼ������
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

//��Ҫ���õ�����ͷ������
int16 MT9V032_CFG[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},   //�Զ��ع�����      ��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
                              //һ������ǲ���Ҫ����������ܣ���Ϊ�������ع���һ�㶼�ȽϾ��ȣ�����������߷ǳ������ȵ�������Գ������ø�ֵ������ͼ���ȶ���
    {EXP_TIME,          300}, //�ع�ʱ��          ����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
    {FPS,               50},  //ͼ��֡��          ����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
    {SET_COL,           COL}, //ͼ��������        ��Χ1-752     K60�ɼ���������188
    {SET_ROW,           ROW}, //ͼ��������        ��Χ1-480
    {LR_OFFSET,         0},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {UD_OFFSET,         0},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {GAIN,              32},  //ͼ������          ��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�
    {INIT,              0}    //����ͷ��ʼ��ʼ��
};

//������ͷ�ڲ���ȡ������������
int16 GET_CFG[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},   //�Զ��ع�����      
    {EXP_TIME,          0},   //�ع�ʱ��          
    {FPS,               0},   //ͼ��֡��          
    {SET_COL,           0},   //ͼ��������        
    {SET_ROW,           0},   //ͼ��������        
    {LR_OFFSET,         0},   //ͼ������ƫ����    
    {UD_OFFSET,         0},   //ͼ������ƫ����    
    {GAIN,              0},   //ͼ������          
};
///////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************
 *	@brief	MT9V032����ͷ�����жϺ���
 *	@param	��
 *	@return	��
 *	@note		�ú�����ISR�ļ� ����3�жϳ��򱻵���
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
 *	@brief	��ȡ��������
 *	@param	��
 *	@return	��
 *	@note		��
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
		//�ȴ����ܻش�����
		while(!uart_receive_flag);
		uart_receive_flag = 0;
		GET_CFG[i][1] = receive[1]<<8 | receive[2];
	}
}

/************************************************************************
 *	@brief	��ȡ����ͷ�̼��汾
 *	@param	��
 *	@return	��
 *	@note		��
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
			
	//�ȴ����ܻش�����
	while(!uart_receive_flag);
	uart_receive_flag = 0;
	
	return ((uint16)(receive[1]<<8) | receive[2]);
}


/************************************************************************
 *	@brief	������������ͷ�ع�ʱ��
 *	@param	light   �����ع�ʱ��Խ��ͼ��Խ��
 *									����ͷ�յ������ݷֱ��ʼ�FPS��������ع�ʱ��
 *									������õ����ݹ�����ô����ͷ��������������ֵ
 *	@return	��ǰ�ع�ֵ������ȷ���Ƿ���ȷд��
 *	@note		��
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
	
	//�ȴ����ܻش�����
	while(!uart_receive_flag);
	uart_receive_flag = 0;
	
	temp = receive[1]<<8 | receive[2];
	return temp;
}

/************************************************************************
 *	@brief	������ͷ�ڲ��Ĵ�������д����
 *	@param  addr    ����ͷ�ڲ��Ĵ�����ַ
 *	@param  data    ��Ҫд�������
 *	@return	�Ĵ�����ǰ���ݣ�����ȷ���Ƿ�д��ɹ�
 *	@note		��
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
	
	//�ȴ����ܻش�����
	while(!uart_receive_flag);
	uart_receive_flag = 0;
	
	temp = receive[1]<<8 | receive[2];
	return temp;
}

/************************************************************************
 *	@brief	����ͷ��ʼ��
 *	@param	��
 *	@return	��
 *	@note		��
 ***********************************************************************/
void Camera_Init(void)
{
	//���ò���    ������ο�ʹ���ֲ�
	uint16 temp, i;
	uint8  send_buffer[4];


	uart_init (uart3, 9600);	//��ʼ������ ��������ͷ    
	uart_rx_irq_en(uart3);


	//�ȴ�����ͷ�ϵ��ʼ���ɹ�
	for(int j = 0; j < 100; j++)//��ʱ1s
	{
		for(int k = 0; k < 1000; k++);
	}
	uart_receive_flag = 0;

	//��ʼ��������ͷ�����³�ʼ��
	for(i=0; i<CONFIG_FINISH; i++)
	{
			send_buffer[0] = 0xA5;
			send_buffer[1] = MT9V032_CFG[i][0];
			temp = MT9V032_CFG[i][1];
			send_buffer[2] = temp>>8;
			send_buffer[3] = (uint8)temp;
			
			uart_putbuff(uart3,send_buffer,4);
			for(int j = 0; j < 1000; j++);//��ʱ2ms
	}
	//�ȴ�����ͷ��ʼ���ɹ�
	while(!uart_receive_flag);
	uart_receive_flag = 0;
	while((0xff != receive[1]) || (0xff != receive[2]));
	//���ϲ��ֶ�����ͷ���õ�����ȫ�����ᱣ��������ͷ��51��Ƭ����eeprom��
	//����Set_Exposure_Time�����������õ��ع����ݲ��洢��eeprom��


	//��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
	Get_Config();


	//����ͷ�ɼ���ʼ��
	DisableInterrupts;
	//DMAͨ��0��ʼ����PTE9����Դ(Ĭ��������)��Դ��ַΪ(PTE0-PTE7)��Ŀ�ĵ�ַΪ��image��ÿ�δ���1Byte ������ϱ���Ŀ�ĵ�ַ
	dma_portx2buff_init(DMA_CH0, (void *)&E_PDIR8(0), (void *)image, E9, DMA_BYTE1, COL*ROW, DADDR_KEEPON);
	port_init(E9, ALT1 | DMA_RISING | PULLDOWN);  		//PCLK  ����Դ����
	DMA_DIS(DMA_CH0);                                 //����DMAͨ��
	DMA_IRQ_CLEAN(DMA_CH0);                           //���ͨ�������жϱ�־λ
	DMA_IRQ_EN(DMA_CH0);                              //����DMAͨ���ж�
	DMA_EN(DMA_CH0);  																//ʹ��DMA

	disable_irq(PORTE_IRQn);                       		//�ر�PTC���ж�
	//port_init(E10, ALT1 | IRQ_FALLING | PULLDOWN);		//���ж�
	port_init(E11, ALT1 | IRQ_FALLING | PULLDOWN);  		//���жϣ��½��ش����жϡ�����
	set_irq_priority(PORTE_IRQn,1);                		//�ж����ȼ�
	enable_irq (PORTE_IRQn);
	EnableInterrupts;
}

uint8   Image_Finish_Flag = 0;      //һ��ͼ��ɼ���ɱ�־λ
/************************************************************************
 *	@brief	MT9V032����ͷ���ж�
 *	@param	��
 *	@return	��
 *	@note		��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
 ***********************************************************************/
void VSYNC(void)
{
	if(!Image_Finish_Flag)
		dma_repeat(DMA_CH0,(void *)&E_PDIR8(0),(void *)image[0],COL*ROW);
}


/************************************************************************
 *	@brief	MT9V032����ͷDMA����ж�
 *	@param	��
 *	@return	��
 *	@note		��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
 ***********************************************************************/
void Row_Finished(void)
{
	Image_Finish_Flag = 1;
	//�����̶Ժڱ߲������������ʹ�����ݵ�ʱ��ʹ��image��������������ұ߼���
	//����ֱ��ʹ��󣬾�û�취ת���ˣ���ΪK60��RAM�᲻��
  //һ��ͼ��Ӳɼ���ʼ���ɼ�������ʱ3.33MS����(50FPS��188*120�ֱ���)
}

/************************************************************************
 *	@brief	����ͼ��
 *	@param	��
 *	@return	��
 *	@note		��
 ***********************************************************************/
void Send_Image(void)
{
	uart_putchar(DEBUG_PORT,0x00);
	uart_putchar(DEBUG_PORT,0xff);
	uart_putchar(DEBUG_PORT,0x01);
	uart_putchar(DEBUG_PORT,0x01);//��������
  uart_putbuff(DEBUG_PORT, (uint8_t *)image_binary, ROW*(COL/8));  //����ͼ��
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
	
	for (j = Mid_Line[49]; j >= 2; j--)	//��ǰһ���е�Ϊ���������ұ߽�
	{
		if (!data[184*i+j] && !data[184*i+j-1])//��⵽���������ڵ㣬���˲���Ч��
		{
			Left_Add[i] = 0;		 //��߽粻��Ҫ���ߣ������־λ
			Left_Line[i] = j;		 //��¼��ǰjֵΪ��i����߽�
//			data[80 * i + j + 2] = 0;//����2λ��ʾ��߽磬������Թ۲�
			
			break;
		}
	}
	if (Left_Add[i])
	{
		Left_Line[i] = 0;
	}
	for (j = Mid_Line[49]; j < 183; j++)//��ǰһ���е�Ϊ������Ҳ����ұ߽�
	{
		if (!data[184*i+j] && !data[184*i+j+1])//��⵽���������ڵ㣬���˲���Ч��
		{
			Right_Add[i] = 0;		 //�ұ߽粻��Ҫ���ߣ������־λ
			Right_Line[i] = j;		 //��¼��ǰjֵΪ��i���ұ߽�
//			data[80 * i + j - 2] = 0;//����2λ��ʾ�ұ߽磬������Թ۲�
			
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
void Traversal_Mid_Line(uint8 i, uint8 *data)	//���м�������ɨ��߽�
{
	uint8 j;
	static uint8 c_pos=0;
	static uint8 c_res=0;
	//Mid_Line[49]= 92;
	uint8 left_dc=0;
	uint8 right_dc=0;
	for (j = Mid_Line[i + 1]; j >= 2; j--)	//��ǰһ���е�Ϊ���������ұ߽�
	{
		if (!data[184*i+j] && !data[184*i+j-1])//��⵽���������ڵ㣬���˲���Ч��
		{
			Left_Add[i] = 0;		 //��߽粻��Ҫ���ߣ������־λ
			Left_Line[i] = j;		 //��¼��ǰjֵΪ��i����߽�
//			data[80 * i + j + 2] = 0;//����2λ��ʾ��߽磬������Թ۲�
			
			break;
		}
	}
	if (Left_Add[i])
	{
		Left_Line[i] = 0;
	}
	for (j = Mid_Line[i + 1]; j < 183; j++)//��ǰһ���е�Ϊ������Ҳ����ұ߽�
	{
		if (!data[184*i+j] && !data[184*i+j+1])//��⵽���������ڵ㣬���˲���Ч��
		{
			Right_Add[i] = 0;		 //�ұ߽粻��Ҫ���ߣ������־λ
			Right_Line[i] = j;		 //��¼��ǰjֵΪ��i���ұ߽�
//			data[80 * i + j - 2] = 0;//����2λ��ʾ�ұ߽磬������Թ۲�
			
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
				for (j = Left_Line[i]; j >= 2; j--)	//��ǰһ���е�Ϊ���������ұ߽�
		{
			if (data[184*i+j] && data[184*i+j-1])//��⵽���������׵㣬���˲���Ч��
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
			for (j = Right_Line[i]; j < 183; j++)//��ǰһ���е�Ϊ������Ҳ����ұ߽�
	{
		if (!data[184*i+j] && !data[184*i+j+1])//��⵽���������׵㣬���˲���Ч��
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

void iteration_Thresholdfigure()//������ֵ����
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


void Mid_Filtering(uint8 *data)		//�����˲�
{
	uint8 i, Count;
	
	Count = ROW - Mid_Count;	//�ж�һ���ж��ٶ��ٿ����е�
	for (i = ROW-2; i > Count; i--)	//���������˲�
	{
		//printf("ML[%d] = %d\n",i,Mid_Line[i]);
		Mid_Line[i] = Mid_Line[i+1]*0.3 + Mid_Line[i]*0.7;
	}
	for (i = Count; i < COL-1; i++)	//�������»����˲�
	{
		Mid_Line[i] = Mid_Line[i-1]*0.3 + Mid_Line[i]*0.7;
	}
	for (i = ROW-1; i >= Count+3; i--)
	{
		data[184*i + Mid_Line[i]] = 0;	//��������ͼ������ʾ����
	}
}

void Traversal_Left_Side(uint8 i, uint8 *data)
{
	uint8 j;
	
	for (j = Left_Line[i+1]; j < 78; j++)
	{
//		if (data[80*i+j] && data[80*i+j-1])//��⵽���������׵㣬���˲���Ч��
		if (data[80*i+j])//��⵽�׵�
		{
			Left_Add[i] = 0;		 //��߽粻��Ҫ���ߣ������־λ
			Left_Line[i] = j;		 //��¼��ǰjֵΪ��i����߽�
			
			break;
		}
	}
}

void Traversal_Right_Side(uint8 i, uint8 *data)
{
	uint8 j;
	
	for (j = Right_Line[i+1]; j > 2; j--)
	{
//		if (data[80*i+j] && data[80*i+j-1])//��⵽���������׵㣬���˲���Ч��
		if (data[80*i+j])//��⵽�׵�
		{
			Right_Add[i] = 0;		 //�ұ߽粻��Ҫ���ߣ������־λ
			Right_Line[i] = j;		 //��¼��ǰjֵΪ��i���ұ߽�
			
			break;
		}
	}
}

void Repair_check(uint8 i)
{
	
	if (Left_Add[i+1])	//ǰһ�в�����
	{
		if (Left_Line[i] < Left_Line_New[i+1]-1)	//��ǰһ�е���߽�ʵ�߱Ƚ�	
		{
			Left_Add[i] = 1;
		}
	}
	else	//ǰһ��û�в���
	{
		if (Left_Line[i] < Left_Line[i+1]-1)	//��ǰһ�е���߽�ʵ�߱Ƚ�	
		{
			Left_Add[i] = 1;
		}
	}
	
	if (Right_Add[i+1])	//ǰһ���ұ߽粹����	
	{
		if (Right_Line[i] > Right_Line_New[i+1]+1)	//��ǰһ�е��ұ߽�ʵ�߽��бȽ�
		{
			Right_Add[i] = 1;
		}
	}
	else	//ǰһ���ұ߽�û�в���
	{
		if (Right_Line[i] > Right_Line[i+1]+1)		//��ǰһ�е��ұ߽�ʵ�߽��бȽ�
		{
			Right_Add[i] = 1;
		}
	}
}
