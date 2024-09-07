#include <reg52.h> 
unsigned char code dispcode[]={0x3F,0x06,0x5B,0x4F,0x66,
0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00,0x40};//�������ʾ��0-15 �� - ����Ӧ��17����
unsigned char dispbuf[8]={0,0,0,0,0,0,0,0,};   //�����ݴ�����
unsigned char temp[8]; 					  	
unsigned char T0count; 					  
unsigned int timecount; 				  
bit flag; 								  
unsigned long x; 					 //��������ݱȽϴ����޷��ų����α�������

sbit P2_4=P2^4;
sbit P2_3=P2^3;
sbit P2_2=P2^2;	


void delay(int i)
{
	while(i--);	
}



 
void display()							  //��ʾ����
{
	unsigned int j;
	unsigned int i;				  				//�������������ʾ��ʱ	
	for(i=8;i>0;i--) 			  
	{ 
		switch(i)    
		{
			case 1:P2_4=1;P2_3=1;P2_2=1;break;  	  
			case 2:P2_4=1;P2_3=1;P2_2=0;break; 
			case 3:P2_4=1;P2_3=0;P2_2=1;break; 
			case 4:P2_4=1;P2_3=0;P2_2=0;break; 
			case 5:P2_4=0;P2_3=1;P2_2=1;break; 
			case 6:P2_4=0;P2_3=1;P2_2=0;break; 
			case 7:P2_4=0;P2_3=0;P2_2=1;break; 
			case 8:P2_4=0;P2_3=0;P2_2=0;break;
		} 

		P0 = dispcode[dispbuf[8-i]];
		for(j=50;j>0;j--);	 //��ʱ
		P0=0x00;     //���������	  											
		
	} 
}
void main(void) 						  //������
{ 
	unsigned char i; 
	TMOD=0x15; 							  
	TH0=0; 
	TL0=0; 								  
	TH1=(65536-57600)/256; 
	TL1=(65536-57600)%256;			      
	TR1=1; 								  
	TR0=1; 								  
	ET0=1; 								  
	ET1=1;								  
	EA=1; 								  

	while(1) 							  //����ѭ��
	{	
		display();				   //������ʾ����
	 							  
		if(flag) 						  //�ж�1��ʱ���Ƿ�
		{ 
			flag=0; 					  //��־λ���㣬���¿�ʼ��ʱ����
			x=(T0count*65536+TH0*256+TL0);//��ȡ1��ʱ���ڵļ���������T0countΪÿ�μ�������65536ʱ��������жϼ�1��
			for(i=0;i<8;i++) 			  
			{ 
				temp[i]=17; 
			} 	  
			i=0; 						  //i����
			while(x/10) 				  //��x���
			{ 
				temp[i]=x%10; 			  
				x=x/10; 				  
				i++; 					  
			} 
			temp[i]=x; 					 
			for(i=0;i<8;i++) 			  //������������ʾ�ݴ�����
			{ 
				dispbuf[i]=temp[i]; 	  
			} 
			T0count=0; 					  
			TH0=0; 
			TL0=0; 						  
			TR0=1;
			TR1=1;						  
		} 
	} 
} 
  
void t0() interrupt 1					  //�������жϺ���
{ 
  T0count++; 							  
} 
  
void t1() interrupt 3					  //��ʱ���жϺ���
{ 
	TH1=(65536-46066)/256; 
	TL1=(65536-46066)%256;				  //50ms����һ���жϣ����¸���ֵ
	timecount++; 						  
	if(timecount==20) 					  //1��ʱ�䵽��
    { 
      TR0=0;							  
	  TR1=0; 							  
      timecount=0; 						  //��ʱ��������
      flag=1; 							  //һ��ʱ�䵽���ñ�־λ��1����������ִ����������������ʾ����
    } 
} 

