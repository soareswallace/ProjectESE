#include <REG51F.H>

sbit c1 = P2^0; //CL
sbit c2 = P2^1; //CH

void trocarP1L(){
	static char estado = 2;
	static unsigned long int cont = 0, contmax = 11500;
	
	switch(estado) {
		case 0:
			if(c1){
				cont = 0;
				P1 = (P1&0xF0) | (P0&0x0F);
				estado = 1;
				break;
			}
			break;

		case 1:
			if (cont++ == contmax) {
				estado = 2;
				break;
			}
			break;
			
		case 2:
			P1 = (P1&0xF0);
			if (c1 == 0) {
				estado = 0;
			}
			break;
		
		default:
			estado = 2;
			break;
	}
}

void trocarP1H(){
	static char estado = 2;
	static unsigned long int cont = 0, contmax = 11500;
	
	switch(estado) {
		case 0:
			if(c2){
				cont = 0;
				P1 = (P0&0xF0) | (P1&0x0F);
				estado = 1;
				break;
			}
			break;
			
		case 1:
			if (cont++ == contmax) {
				estado = 2;
				break;
			}
			break;
			
		case 2:
			P1 = (P1&0x0F);
			if (c2 == 0) {
				estado = 0;
			}
			break;
		
		default:
			estado = 2;
			break;
	}
}

main()
{
	while(1) 
	{
		trocarP1L();
		trocarP1H();
	}	
}
