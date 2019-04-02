#include <REG51.H>
#include <string.h>

// VAZIO: in == out
// CHEIO: (in +1) % TAM_BUFFER == out
// SE TAM_BUFFER FOR POTENCIA DE 2: (in +1)&0x0F == out    



#define FREQCLK 12000000
#define TH1_VALUE 204		//Para Boundrate = 1200bps
#define BAUDRATE ((2^(SMOD1)/32)*(FREQCLK/(12*(256 - TH1_VALUE))))
#define TAM_BUFFER 16
#define CARACTER_FINAL = ' ';

char buffer_circular_transm[16];  
char buffer_circular_recept[16]; 


unsigned int i = 0;
unsigned int j = 0;

unsigned int IN_REC = 0;
unsigned int IN_TRANS = 0;
unsigned int OUT_REC = 0;
unsigned int OUT_TRANS = 0;

unsigned int TX_OCUPADO;


void timer1_inicializa();
void serial1_inicializa();
void sendChar(char c);
char receiveChar();

int TxBufferTransVazio();
int RxBufferRecVazio();
int incoming;
int readThis;



void main() {
	timer1_inicializa();
	serial1_inicializa();
	EA = 1;													//Habilita tratamento de interupções
	ES = 1;													//Habilita as interrupções da serial
	
	while(1){
		unsigned char c;
		
    	c = receiveChar();
    	sendChar(c);
  	}
}

void timer1_inicializa(){
	TR1 = 0;												//Desliga o Timer 01
	TMOD = (TMOD & 0x0F) | 0x20; 	//Coloca o Timer 01 no Modo 2
	TH1 = TH1_VALUE;								//Atribui o valor de reload
	TR1 = 1;												//Liga o Timer 01
}

void serial1_inicializa(){
	SCON = (SCON & 0x3F) | 0x40;		//01000000 em Decimal - Definindo SM0 e SM1 como 01 (Serial no Modo 01)
	PCON = (PCON & 0x7F) | 0x80;		//SMOD com valor 01 na fórmula do Baundrate
	REN = 1;												//Habilita a recepção
}

void serial_isr (void) interrupt 4 using 2{
	if(TI){													//Verifica se a interrupção é de transmissão
		TI = 0;
		if (IN_TRANS != OUT_TRANS) {
      		SBUF = buffer_circular_transm[OUT_TRANS];
      		if (OUT_TRANS + 1 == 0x0F) {
        		OUT_TRANS = 0;
        		if (IN_TRANS == 0x0F) {
          			IN_TRANS = 0;
        		}
      		}
      		else {
        		OUT_TRANS++;
      		}
    	}

    	TX_OCUPADO = 0;
	}	
	
	if(RI){													//Verifica se a interrupção é de recepção
		RI = 0;
		incoming = 1;
    	if (RxBufferRecVazio()) {
      		buffer_circular_recept[IN_REC] = SBUF + 1;
      		IN_REC++;
    	}
		
		else if((IN_REC < 0x0F) && (IN_REC + 1 != OUT_REC)){
			if (IN_REC + 1 == 0x0F) {
				if (OUT_REC > 0) {
					IN_REC = 0;
					buffer_circular_recept[IN_REC] = SBUF + 1;
					IN_REC++;
				}
			}
			else {
				buffer_circular_recept[IN_REC] = SBUF + 1;
				IN_REC++;
			}
		}
	}
}


void sendChar(char c){ // unico q coloca caracteres no buffer de transmissao
	if (readThis) {
		readThis = 0;
		if (TxBufferTransVazio()) {
      		buffer_circular_transm[IN_TRANS] = c;
      		IN_TRANS++;
    	}
		else if((IN_TRANS < 0x0F) && (IN_TRANS + 1 != OUT_TRANS)){
			buffer_circular_transm[IN_TRANS] = c;
      		IN_TRANS++;
		}
    	else if ((IN_TRANS == 0x0F) && OUT_TRANS > 1){
      		IN_TRANS = 0;
      		buffer_circular_transm[IN_TRANS] = c;
      		IN_TRANS++;
    	}
  
		if (!TX_OCUPADO) {
			TX_OCUPADO = 1;
			TI = 1; // chamar a interrupção
		}
	}	
}

char receiveChar(){
	unsigned char c;
	if (incoming) {
		if (IN_REC != OUT_REC) {
			readThis = 1;
			incoming = 0;
      		c = buffer_circular_recept[OUT_REC];
      		if (OUT_REC + 1 == 0x0F) {
        		OUT_REC = 0;
      		}
      		else {
        		OUT_REC++;
      		}
    	}
	}

	return c;
}
        
int RxBufferRecVazio(){
	 if (IN_REC == OUT_REC){
		 return 1;
	 }
	 else {
		 return 0;
	 }
}

int TxBufferTransVazio(){
	 if (IN_TRANS == OUT_TRANS){
		 return 1;
	 }
	 else {
		 return 0;
	 }
}

// possivelmente na funcao recChar criar uma flag para sinalizar a recepção de strings RI foi pra 1, 
// qd TI ta 1, ele acabou de trnsmitir um caracter
// crie a flag TX_OCUPADO q é ligado no programa principal, 
// se 
