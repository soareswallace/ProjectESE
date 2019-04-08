	// VAZIO: in == out
// CHEIO: (in + 1) % TAM_BUFFER == out
// SE TAM_BUFFER FOR POTENCIA DE 2: (in +1)&0x0F == out    


#include <REG517A.H>
#include <string.h>

#define FREQCLK 12000000
#define CORRECAO 7
#define FreqTimer0_emHz 100
#define TH1_VALUE 204		//Para Boundrate = 1200bps
#define BAUDRATE ((2^(SMOD1)/64)*(FREQCLK/(12*(256 - TH1_VALUE))))
#define TAM_BUFFER 16
#define VALOR_TH0 ((65536 - (FREQCLK / (12 * FreqTimer0_emHz)) + CORRECAO) >>8)			// >>8 é um shift (deslocamento binário) de 8 posições para direita
#define VALOR_TL0 ((65536 - (FREQCLK / (12 * FreqTimer0_emHz)) + CORRECAO) & 0xFF)

char buffer_circular_transm[16];  
char buffer_circular_recept[16]; 

unsigned short int cont_LED = 0;


unsigned int IN_REC = 0;
unsigned int IN_TRANS = 0;
unsigned int OUT_REC = 0;
unsigned int OUT_TRANS = 0;
unsigned int TX_OCUPADO = 0;


void timer0_inicializa();
void timer1_inicializa();
void serial1_inicializa();
void sendChar(char c);
char receiveChar();
int bufferVazio(IN_REC, OUT_REC);

int incomingChar = 0;
int readThisChar = 0;



void main() {	
	timer1_inicializa();
	serial1_inicializa();
	if (bufferVazio(IN_REC, OUT_REC)) {
		while(1){
			unsigned char c;
			c = receiveChar();
			sendChar(c);
		}
	}
}

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
	if(TI0){													//Verifica se a interrupção é de transmissão
		TI0 = 0;
		if (IN_TRANS != OUT_TRANS) {
			if (OUT_TRANS + 1 > 0x0F) {
				OUT_TRANS = 0;
				S0BUF = buffer_circular_transm[OUT_TRANS];
				OUT_TRANS++;
     	}
			else {
				S0BUF = buffer_circular_transm[OUT_TRANS];
				OUT_TRANS++;
     	}
    }
   
		TX_OCUPADO = 0;
	}	
	
	if(RI0) {													//Verifica se a interrupção é de recepção
		RI0 = 0;
		incomingChar = 1;
		if((IN_REC <= 0x0F) && (IN_REC + 1 != OUT_REC)){
			if (IN_REC + 1 > 0x0F) {
				if (OUT_REC > 0) {
					IN_REC = 0;
					buffer_circular_recept[IN_REC] = S0BUF + 1;
					IN_REC++;
				}
			}
			else {
				buffer_circular_recept[IN_REC] = S0BUF + 1;
				IN_REC++;
			}
		}
	}
}


void sendChar(char c){ // unico q coloca caracteres no buffer de transmissao
	if (readThisChar) {
		readThisChar = 0;
		OUT_REC++;
		if((IN_TRANS <= 0x0F) && (IN_TRANS + 1 != OUT_TRANS)){
			if (IN_TRANS + 1 > 0x0F && OUT_TRANS > 0) {
				IN_TRANS = 0;
				buffer_circular_transm[IN_TRANS] = c;
				IN_TRANS++;
			}
			else {
				buffer_circular_transm[IN_TRANS] = c;
				IN_TRANS++;
			}
		}
		
		if (!TX_OCUPADO) {
			TX_OCUPADO = 1;
			TI0 = 1; // chamar a interrupção
		}
	}	
}

char receiveChar(){
	unsigned char c;
	if (incomingChar) {
		if (IN_REC != OUT_REC) {
			readThisChar = 1;
			incomingChar = 0;
			if (OUT_REC + 1 > 0x0F) {
				OUT_REC = 0;
				c = buffer_circular_recept[OUT_REC];
      }
			else {
				c = buffer_circular_recept[OUT_REC];
      }
    }
	}

	return c;
}

int bufferVazio(IN_REC, OUT_REC) {
	if (IN_REC == OUT_REC) {
		return 1;
	}
	else {
		return 0;
	}
}
