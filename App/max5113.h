#ifndef __MAX5113_H
#define __MAX5113_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"


void Init_Max5113(void);
void Read_Max5113(void);
void AnalysisUartData(void);

void Test_Write_Max5113(void);
void Test_Write_Max5113_2(void);
void CloseOpwer(void);

extern u8 Flash_Write_Flag;

#endif /* __MAIN_H */

