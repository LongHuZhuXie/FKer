#include "Hall.h"


void Hall_Init(void)
{
	gpio_init(Hall_Pin,GPO,1);  //��ʼ��HallΪ�ߵ�ƽ
}

bool Hall_Scan(void)
{
	return Hall_Port->PDIR&(1<<24);
}
