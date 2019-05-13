#include <REG517A.H>

#define CORRECAO 7
#define FreqTimer0_emHz 100
#define TH1_VALUE 204		//Para Baudrate = 1200bps
#define FREQCLK 12000000
#define VALOR_TH0 ((65536 - (FREQCLK / (12 * FreqTimer0_emHz)) + CORRECAO) >>8)
#define VALOR_TL0 ((65536 - (FREQCLK / (12 * FreqTimer0_emHz)) + CORRECAO) & 0xFF)

sbit R1 = P1^1;
sbit R2 = P1^2;
sbit R3 = P1^3;
sbit R4 = P1^4;

sbit C1 = P1^5;
sbit C2 = P1^6;
sbit C3 = P1^7;

void timer0_int (void) interrupt 1 using 2{ // dispositivo associado é 1 (timer0) no vetor de interrupção	
	
	if (R1 == 0) {
		if (C1 == 0) {
			S0BUF = '1';
		}
		
		if (C2 == 0) {
			S0BUF = '2';
		}
		
		if (C3 == 0) {
			S0BUF = '3';
		}
	}
	
	if (R2 == 0) {
		if (C1 == 0) {
			S0BUF = '4';
		}
		
		if (C2 == 0) {
			S0BUF = '5';
		}
		
		if (C3 == 0) {
			S0BUF = '6';
		}
	}
	
	if (R3 == 0) {
		if (C1 == 0) {
			S0BUF = '7';
		}
		
		if (C2 == 0) {
			S0BUF = '8';
		}
		
		if (C3 == 0) {
			S0BUF = '9';
		}
	}
	
	if (R4 == 0) {
		if (C1 == 0) {
			S0BUF = '*';
		}
		
		if (C2 == 0) {
			S0BUF = '0';
		}
		
		if (C3 == 0) {
			S0BUF = '#';
		}
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

void main(){
	timer0_inicializa();
	while(1){
	}
}
