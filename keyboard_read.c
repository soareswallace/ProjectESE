#include <REG517A.H>

#define CORRECAO 7
#define FreqTimer0_emHz 100
#define TH1_VALUE 204		//Para Baudrate = 1200bps
#define FREQCLK 12000000
#define VALOR_TH0 ((65536 - (FREQCLK / (12 * FreqTimer0_emHz)) + CORRECAO) >>8)
#define VALOR_TL0 ((65536 - (FREQCLK / (12 * FreqTimer0_emHz)) + CORRECAO) & 0xFF)

sbit R1 = P5^0;
sbit R2 = P5^1;
sbit R3 = P5^2;
sbit R4 = P5^3;

sbit C1 = P5^4;
sbit C2 = P5^5;
sbit C3 = P5^6;

char ch = '\0';
unsigned char sh;

void timer0_inicializa() {
	TR0 = 0;											// Desliga Timer0
	TMOD = (TMOD & 0xF0) | 0x01;	// Timer 0 programado como timer de 16 bits
	TL0 = VALOR_TL0;
	TH0 = VALOR_TH0 + (unsigned char) CY;							// Programa contagem do Timer0
	ET0 = 1;											// Habilita interrupcao do timer 0
	TR0 = 1;											// Habilita contagem do timer 0
}

void serial1_inicializa() {
	SM0 = 0;
	SM1 = 1;
	EAL = 1;													//Habilita tratamento de interup��es
	ES0 = 1;													//Habilita as interrup��es da serial
	S0CON = (S0CON & 0x0F) | 0x70;		//01010000 em Decimal - Definindo SM0 e SM1 como 01 (Serial no Modo 01)
	PCON = (PCON & 0x7F) | 0x80;		//SMOD com valor 01 na f�rmula do Baundrate
	REN0 = 1;												//Habilita a recep��o
}

void serial_isr (void) interrupt 4 using 2 {
	if(TI0){													//Verifica se a interrup��o � de transmiss�o
		TI0 = 0;	
		S0BUF = ch;
	}	
}

void timer0_int (void) interrupt 1 using 2{ // dispositivo associado � 1 (timer0) no vetor de interrup��o	

	/*
	1110
	1101
	1011
	0111
	*/
	
	ch = '\0';

	sh = ((sh << 1) | 0x01) & 0x0F;
	
	P5 = (0xF0 & P5) | (0x0F & sh); 
	
	if (R1 | C1 == 0) {
			ch = '1';
	}
	
	if (R1 | C2 == 0) {
			ch = '2';
	}

	if (R1 | C3 == 0) {
			ch = '3';
	}
	
	if (R2 | C1 == 0) {
			ch = '4';
	}
	
	if (R2 | C2 == 0) {
			ch = '5';
	}
	
	if (R2 | C3 == 0) {
			ch = '6';
	}
	
	if (R3 | C1 == 0) {
			ch = '7';
	}
	
	if (R3 | C2 == 0) {
			ch = '8';
	}
	
	if (R3 | C3 == 0) {
			ch = '9';
	}
	
	if (R4 | C1 == 0) {
			ch = '*';
	}
	
	if (R4 | C2 == 0) {
			ch = '0';
	}
	
	if (R4 | C3 == 0) {
			ch = '#';
	}
	
	if(ch) {
		TI0 = 1;
	}
	
	if(sh >= 0x0f) {
		sh = 0x0E
	}
}

void main(){
	P5 = (P5 & 0xF0) | 0x0E;
	sh = P5 & 0x0F;
	
	timer0_inicializa();
	serial1_inicializa();
	
	while(1){
	}
}
