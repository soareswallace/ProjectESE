#include <REG51F.H>

#define CORRECAO 7
#define FrClk 12000000
#define FreqTimer0_emHz 100
#define VALOR_TH0 ((65536 - (FrClk / (12 * FreqTimer0_emHz)) + CORRECAO) >>8)			// >>8 é um shift (deslocamento binário) de 8 posições para direita
#define VALOR_TL0 ((65536 - (FrClk / (12 * FreqTimer0_emHz)) + CORRECAO) & 0xFF)

unsigned short int cont_P1L = 0;
unsigned short int cont_P1H = 0;

// Interrupt -> cria um jump no vetor de interrupção
void timer0_int (void) interrupt 1 using 2{ // dispositivo associado é 1 (timer0) no vetor de interrupção
	TR0 = 0;							// Desliga Timer0
	TH0 += VALOR_TH0;			// Programa contagem do Timer0 (já considera possíveis alterações)
	TL0 += VALOR_TL0;
	TR0 = 1;							// Habilita contagem do timer 0
	
	if(cont_P1L >= 100){
		cont_P1L = 0;
	}	else{
		cont_P1L++;
	}
	if(cont_P1H >= 100){
		cont_P1H = 0;
	}	else{
		cont_P1H++;
	}
}

void timer0_inicializa() {
	TR0 = 0;											// Desliga Timer0
	TMOD = (TMOD & 0xF0) | 0x01;	// Timer 0 programado como timer de 16 bits
	TL0 = VALOR_TL0;
	TH0 = VALOR_TH0 + (unsigned char) CY;							// Programa contagem do Timer0
	ET0 = 1;											// Habilita interrupcao do timer 0
	TR0 = 1;											// Habilita contagem do timer 0
}
//Calcule o valor de CORRECAO!!! -> a correção é referente ao número de instruções executadas enquanto o timer está parado (não incrementa)
sbit c1 = P2^0; //CL
sbit c2 = P2^1; //CH

void trocarP1L(){
	static char estado = 2;
	
	switch(estado) {
		case 0:
			if(c1){
				P1 = (P1&0xF0) | (P0&0x0F);
				estado = 1;
				break;
			}
			break;

		case 1:
			if(cont_P1L == 0){
				estado = 2;
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
	
	switch(estado) {
		case 0:
			if(c2){
				P1 = (P0&0xF0) | (P1&0x0F);
				estado = 1;			
				break;
			}
			break;
			
		case 1:
			if(cont_P1H == 0){
				estado = 2;
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

main(){	
	EA = 0;
	
	timer0_inicializa(); // inicializa timer0
	
	EA=1; //Habilita o tratamento de interrupções (depois de inicializar para garantir a estabilidade do sistema - garantir que não sejam geradas interruções)

	// Configuração do Timer1 para ser timer/temporizador no modo 1
	/*
	gate = 0;
	C/T = 0;
	M1 = 0;
	M0 = 1;
	*/
	while(1){
		trocarP1L();
		trocarP1H();
	}
}
