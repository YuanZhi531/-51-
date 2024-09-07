#include <reg52.h> 
unsigned char code dispcode[]={0x3F,0x06,0x5B,0x4F,0x66,
0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00,0x40};//数码管显示码0-15 与 - （对应第17个）
unsigned char dispbuf[8]={0,0,0,0,0,0,0,0,};   //数据暂存数组
unsigned char temp[8]; 					  	
unsigned char T0count; 					  
unsigned int timecount; 				  
bit flag; 								  
unsigned long x; 					 //脉冲的数据比较大，用无符号长整形变量定义

sbit P2_4=P2^4;
sbit P2_3=P2^3;
sbit P2_2=P2^2;	


void delay(int i)
{
	while(i--);	
}



 
void display()							  //显示函数
{
	unsigned int j;
	unsigned int i;				  				//定义变量用于显示延时	
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
		for(j=50;j>0;j--);	 //延时
		P0=0x00;     //清零数码管	  											
		
	} 
}
void main(void) 						  //主函数
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

	while(1) 							  //进入循环
	{	
		display();				   //调用显示函数
	 							  
		if(flag) 						  //判断1秒时间是否到
		{ 
			flag=0; 					  //标志位清零，重新开始计时计数
			x=(T0count*65536+TH0*256+TL0);//读取1秒时间内的计数个数（T0count为每次计数超过65536时进入计数中断加1）
			for(i=0;i<8;i++) 			  
			{ 
				temp[i]=17; 
			} 	  
			i=0; 						  //i清零
			while(x/10) 				  //将x拆分
			{ 
				temp[i]=x%10; 			  
				x=x/10; 				  
				i++; 					  
			} 
			temp[i]=x; 					 
			for(i=0;i<8;i++) 			  //将数据送入显示暂存数组
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
  
void t0() interrupt 1					  //计数器中断函数
{ 
  T0count++; 							  
} 
  
void t1() interrupt 3					  //定时器中断函数
{ 
	TH1=(65536-46066)/256; 
	TL1=(65536-46066)%256;				  //50ms进入一次中断，重新赋初值
	timecount++; 						  
	if(timecount==20) 					  //1秒时间到了
    { 
      TR0=0;							  
	  TR1=0; 							  
      timecount=0; 						  //计时变量清零
      flag=1; 							  //一秒时间到，该标志位置1，主函数中执行脉冲数运算与显示函数
    } 
} 

