#include <REG51F.H>

sbit c1 = P2^0;
sbit c2 = P2^1;

sbit p10 = P1^0;
sbit p11 = P1^1;
sbit p12 = P1^2;
sbit p13 = P1^3;
sbit p14 = P1^4;
sbit p15 = P1^5;
sbit p16 = P1^6;
sbit p17 = P1^7;

sbit p00 = P0^0;
sbit p01 = P0^1;
sbit p02 = P0^2;
sbit p03 = P0^3;
sbit p04 = P0^4;
sbit p05 = P0^5;
sbit p06 = P0^6;
sbit p07 = P0^7;


main() 
{
	while(1) 
	{
		if (c1)
		{
			p10 = p00;
			p11 = p01;
			p12 = p02;
			p13 = p03;
		}
		else if(!c1)
		{
			p10 = 0;
			p11 = 0;
			p12 = 0;
			p13 = 0;
		}
		if (c2)
		{
			p14 = p04;
			p15 = p05;
			p16 = p06;
			p17 = p07;
		}
		else
		{
			p14 = 0;
			p15 = 0;
			p16 = 0;
			p17 = 0;
		}
		
	}
			
}
