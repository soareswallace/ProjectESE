#include <REG517A.H>

#define TH1_VALUE 204		//Para Baudrate = 1200bps

unsigned int light = 0;

void timer1_inicializa() {
	TR1 = 0;												//Desliga o Timer 01
	TMOD = (TMOD & 0x0F) | 0x20; 	  //Coloca o Timer 01 no Modo 2
	TH1 = TH1_VALUE;								//Atribui o valor de reload
	TR1 = 1;												//Liga o Timer 01
}

void serial1_inicializa() {
	SM0 = 0;
	SM1 = 1;
	EAL = 1;													//Habilita tratamento de interupções
	ES0 = 1;													//Habilita as interrupções da serial
	S0CON = (S0CON & 0x0F) | 0x70;		//01010000 em Decimal - Definindo SM0 e SM1 como 01 (Serial no Modo 01)
	PCON = (PCON & 0x7F) | 0x80;		//SMOD com valor 01 na fórmula do Baundrate
	REN0 = 1;												//Habilita a recepção
}

void serial_isr (void) interrupt 4 using 2 {
	
	if(RI0) {													//Verifica se a interrupção é de recepção
		RI0 = 0;
		if(S0BUF > 47 &&  S0BUF < 58){
			light = S0BUF - 48;
			CMH0 = 0xFF - 25*light;
			CMH1 = 0xFF - 25*light;
			CMH2 = 0xFF - 25*light;
			CMH3 = 0xFF - 25*light;
			CMH4 = 0xFF - 25*light;
			CMH5 = 0xFF - 25*light;
			CMH6 = 0xFF - 25*light;
			CMH7 = 0xFF - 25*light;
			
			CML0 = 0xFF - 25*light;
			CML1 = 0xFF - 25*light;
			CML2 = 0xFF - 25*light;
			CML3 = 0xFF - 25*light;
			CML4 = 0xFF - 25*light;
			CML5 = 0xFF - 25*light;
			CML6 = 0xFF - 25*light;
			CML7 = 0xFF - 25*light;
			
		}
	}
}

void pwm_start() {
	CMSEL = (CMSEL | 0xFF);
	CMEN = (CMEN | 0xFF);
	CTRELL = 0xFF;
	CTRELH = 0xFF;
	CTCON = (CTCON & 0xF8);
}

void main() {	
	timer1_inicializa();
	serial1_inicializa();
	pwm_start();
		while(1){
		}
}
