#include "Hall.h"


void Hall_Init(void)
{
	gpio_init(Hall_Pin,GPI,1);  //初始化Hall为高电平
}

bool Hall_Scan(void)
{
	return Hall_Port->PDIR&(1<<24);
}
