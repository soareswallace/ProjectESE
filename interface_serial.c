#include <REG51.H>
#include <string.h>

#define FREQCLK 12000000
#define TH1_VALUE 204		//Para Boundrate = 1200bps
#define BAUDRATE ((2^(SMOD1)/32)*(FREQCLK/(12*(256 - TH1_VALUE))))
#define TAM_BUFFER 20

unsigned int i = 0;
unsigned int j = 0;

void timer1_inicializa();
void serial1_inicializa();

void main() {
	timer1_inicializa();
	serial1_inicializa();
	EA = 1;													//Habilita tratamento de interupções
	ES = 1;													//Habilita as interrupções da serial
	while(1);
}

void timer1_inicializa(){
	TR1 = 0;												//Desliga o Timer 01
	TMOD =  (TMOD & 0x0F) | 0x20; 	//Coloca o Timer 01 no Modo 2
	TH1 = TH1_VALUE;								//Atribui o valor de reload
	TR1 = 1;												//Liga o Timer 01
}

void serial1_inicializa(){
	SCON = (SCON & 0x3F) | 0x40;		//01000000 em Decimal - Definindo SM0 e SM1 como 01 (Serial no Modo 01)
	PCON = (PCON & 0x7F) | 0x80;		//SMOD com valor 01 na fórmula do Baundrate
	REN = 1;												//Habilita a recepção
}

void serial_isr (void) interrupt 4 using 2{
	unsigned char c;
	if(TI){													//Verifica se a interrupção é de transmissão
		TI = 0;
	}	
	if(RI){													//Verifica se a interrupção é de recepção
		RI = 0;
		c = SBUF;
		SBUF = c + 1;		//Lê do registrador SBUF de tranmissão 
	}
}