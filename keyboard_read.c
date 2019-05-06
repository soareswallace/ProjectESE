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

unsigned char bitver = 1;
unsigned char v;

void timer0_int (void) interrupt 1 using 2{ // dispositivo associado é 1 (timer0) no vetor de interrupção
	TR0 = 0;							// Desliga Timer0
	TH0 += VALOR_TH0;			// Programa contagem do Timer0 (já considera possíveis alterações)
	TL0 += VALOR_TL0;
	TR0 = 1;							// Habilita contagem do timer 0
	
	
	if ((bitver & 0x0F) == 0) {
		bitver = 1; // resetar o bit pro valor inicial
	}
	
	bitver = bitver << 1;
	
	if(C1 == 0){
		while(R1 == 0){
			return '1';
		}
		while(R2 == 0){
			return '4';
		}
		while(R3==0){
			return '7';
		}
		while(R4==0){
			return '*';
		}
	}
	
	if(C2 == 0){
		while(R1 == 0){
			return '2';
		}
		while(R2 == 0){
			return '5';
		}
		while(R3==0){
			return '8';
		}
		while(R4==0){
			return '0';
		}
	}
	
	if(C3 == 0){
		while(R1 == 0){
			return '3';
		}
		while(R2 == 0){
			return '6';
		}
		while(R3==0){
			return '9';
		}
		while(R4==0){
			return '#';
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
	C1 = 0;
	C2 = 0;
	C3 = 0;
	timer0_inicializa();
	while(1){
	}
}
