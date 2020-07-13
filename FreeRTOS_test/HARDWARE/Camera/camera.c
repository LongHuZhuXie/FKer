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
uint8 THRESHOLD= 120;


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

void Image_Binary()
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
			if (image[i][j] < THRESHOLD) tmpdst = (tmpdst<<1);
			else tmpdst = (tmpdst<<1)| 0x01;
			pos++;
			if (pos == 8)
			{
				pos = 0;
				image_binary[i][k] = tmpdst;
				k+=1;
			}
			//printf("pos=%d [%d][%d]\n",pos,i,k);
		}
	}
}