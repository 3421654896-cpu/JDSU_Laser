#include "main.h"

uint16_t item=0;

uint16_t adc1[Number]={0};
uint16_t adc2[Number]={0};
uint16_t adc3[Number]={0};
uint16_t adc4[Number]={0};

float adc_norvec[Number]={0};//全局变量，否则数据出错

uint16_t peaks1[PEAKS_COUNT]={0};
uint16_t peaks2[PEAKS_COUNT]={0};
uint16_t peaks3[PEAKS_COUNT]={0};
uint16_t peaks4[PEAKS_COUNT]={0};

uint8_t packPeaks[4*4*PEAKS_COUNT+5] = {0};

uint8_t Find_Peaks(uint16_t *adc_vec, uint16_t *peaks_vec, uint16_t adc_length){
	uint16_t initials[PEAKS_COUNT]={0};
	memset(peaks_vec, 0, sizeof(uint16_t)*PEAKS_COUNT);
	
	float ma = adc_vec[0], mi = adc_vec[0];
	for(int i=0;i<adc_length;i++){
		if(adc_vec[i]>ma) ma = adc_vec[i];
		if(adc_vec[i]<mi) mi = adc_vec[i];
	}
	
	float mean = 0;
	float gap = ma-mi;
//	float *adc_norvec = (float*)calloc(3000, sizeof(float));
	for(int i=0;i<adc_length;i++){
		adc_norvec[i] = (adc_vec[i]-mi)/gap;
		mean+=adc_norvec[i];
	}
	mean = mean / (float)adc_length;
	Find_Initial(adc_norvec, initials, adc_length, mean);
	
	uint8_t i=0;
	for(;i<PEAKS_COUNT;i++){
		if(initials[i]==0) break;
		int start = initials[i]-INTERVAL>=0?initials[i]-INTERVAL:0;
		int end = initials[i]+INTERVAL<adc_length?initials[i]+INTERVAL:adc_length;
		int sumxy=0, sumy=0;
		for(int j=start;j<end;j++){
			sumxy+=adc_vec[j]*j;
			sumy+=adc_vec[j];
		}
		peaks_vec[i] = sumxy/sumy;//position
	}
	return i;
}

void Find_Initial(float *adc_norvec, uint16_t *initials, uint16_t adc_length, float threshold){
	if(threshold>0.15) return;
	uint16_t it = 0;
	for(uint16_t i=1;i<adc_length-1;i++){
		if(it>=PEAKS_COUNT) break;
		int start = i-INTERVAL>=0?i-INTERVAL:0;
		int end = i+INTERVAL<=adc_length?i+INTERVAL:adc_length;
		
		float front=-0xFFFF,back=-0xFFFF;
		for(int j=start;j<i;j++){
			if(adc_norvec[j]>front) front = adc_norvec[j];
		}
		for(int j=i+1;j<end;j++){
			if(adc_norvec[j]>back) back = adc_norvec[j];
		}
		if((adc_norvec[i]>front||adc_norvec[i]==front)&&(adc_norvec[i]>back||adc_norvec[i]==back)&&adc_norvec[i]>0.6){
			initials[it] = i;
			i+=INTERVAL;
			it++;
		}
	}
}

void SendPeaks(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4){
		uint8_t indexCount = 0;
		packPeaks[indexCount++] = 0xAB;
//		packPeaks[indexCount++] = 0x04;
		
//		packPeaks[indexCount++] = 0xEF;
		packPeaks[indexCount++] = p1;
		int i=0;
		for(;i<p1;i++){
			packPeaks[indexCount++] = (Wave_DATA[peaks1[i]][0]>>8) & 0xFF;
			packPeaks[indexCount++] = Wave_DATA[peaks1[i]][0] & 0xFF;
			packPeaks[indexCount++] = (Wave_DATA[peaks1[i]][1]>>8) & 0xFF;
			packPeaks[indexCount++] = Wave_DATA[peaks1[i]][1] & 0xFF;
		}
		for(;i<PEAKS_COUNT;i++){
			packPeaks[indexCount++] = 0x00;
			packPeaks[indexCount++] = 0x00;
			packPeaks[indexCount++] = 0x00;
			packPeaks[indexCount++] = 0x00;
		}
//		packPeaks[indexCount++] = 0xFE;
		
		i=0;
//		packPeaks[indexCount++] = 0xEF;
		packPeaks[indexCount++] = p2;
		for(;i<p1;i++){
			packPeaks[indexCount++] = (Wave_DATA[peaks2[i]][0]>>8) & 0xFF;
			packPeaks[indexCount++] = Wave_DATA[peaks2[i]][0] & 0xFF;
			packPeaks[indexCount++] = (Wave_DATA[peaks2[i]][1]>>8) & 0xFF;
			packPeaks[indexCount++] = Wave_DATA[peaks2[i]][1] & 0xFF;
		}
		for(;i<PEAKS_COUNT;i++){
			packPeaks[indexCount++] = 0x00;
			packPeaks[indexCount++] = 0x00;
			packPeaks[indexCount++] = 0x00;
			packPeaks[indexCount++] = 0x00;
		}
//		packPeaks[indexCount++] = 0xFE;
		
		i=0;
//		packPeaks[indexCount++] = 0xEF;
		packPeaks[indexCount++] = p3;
		for(;i<p1;i++){
			packPeaks[indexCount++] = (Wave_DATA[peaks3[i]][0]>>8) & 0xFF;
			packPeaks[indexCount++] = Wave_DATA[peaks3[i]][0] & 0xFF;
			packPeaks[indexCount++] = (Wave_DATA[peaks3[i]][1]>>8) & 0xFF;
			packPeaks[indexCount++] = Wave_DATA[peaks3[i]][1] & 0xFF;
		}
		for(;i<PEAKS_COUNT;i++){
			packPeaks[indexCount++] = 0x00;
			packPeaks[indexCount++] = 0x00;
			packPeaks[indexCount++] = 0x00;
			packPeaks[indexCount++] = 0x00;
		}
//		packPeaks[indexCount++] = 0xFE;
		
		i=0;
//		packPeaks[indexCount++] = 0xEF;
		packPeaks[indexCount++] = p4;
		for(;i<p1;i++){
			packPeaks[indexCount++] = (Wave_DATA[peaks4[i]][0]>>8) & 0xFF;
			packPeaks[indexCount++] = Wave_DATA[peaks4[i]][0] & 0xFF;
			packPeaks[indexCount++] = (Wave_DATA[peaks4[i]][1]>>8) & 0xFF;
			packPeaks[indexCount++] = Wave_DATA[peaks4[i]][1] & 0xFF;
		}
		for(;i<PEAKS_COUNT;i++){
			packPeaks[indexCount++] = 0x00;
			packPeaks[indexCount++] = 0x00;
			packPeaks[indexCount++] = 0x00;
			packPeaks[indexCount++] = 0x00;
		}
//		packPeaks[indexCount++] = 0xFE;
		
//		packPeaks[indexCount] = 0xBA;
		
		USART_DMA_Send(packPeaks, 4*4*PEAKS_COUNT+5);
}
