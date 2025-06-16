// COMANDO PARA COMPILAR:  clang -Wall jogoMemoria.c -o jogo -pthread
// COMANDO PARA RODAR: ./jogo
#include <unistd.h>   
#include <sys/types.h> 
#include <string.h>
#include <pthread.h>
#include "logicaJogoMemoria.c"

#define MAXBUFF 1024  // numero de caract. do buffer

List *lista_acertos;
int pipe1[2], pipe2[2];
int pontos_jogador = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void jogador1(int readfd, int writefd, List *lista_aberta);
void jogador2(int readfd, int writefd, List *lista_aberta);
void *ContaPontosJogador(int jogador);
void *FimJogo();

int main(){
	
	pthread_t thread_pontos_jogador, thread_fim_jogo;
   	int	descritor;  // usado para criar o processo filho pelo fork
	
	List *lista_aberta = randGrade();
	lista_acertos = new_list();
	
	//print_list(lista_aberta); SO PARA TESTES
    printf("JOGO DA MEMORIA\n");

   	if (pipe(pipe1) < 0 || pipe(pipe2) < 0){
		printf("Erro na chamada PIPE");
	   	exit(0);
	}

   	//   Fork para criar o processo filho
    if ( (descritor = fork()) <0){
		printf("Erro na chamada FORK");
		exit(0);
	}

	if (descritor >0){
	// PROCESSO PAI
		
		// INICIA A THREAD QUE CONTA OS PONTOS DO JOGADOR 1(processo pai)
   		pthread_create( 
						&thread_pontos_jogador,
						NULL,
						(void *)ContaPontosJogador,
						(int *)1
		);

		// SOMENTE O PAI INICIA A THREAD QUE FINALIZA O JOGO
		pthread_create(
						&thread_fim_jogo,
						NULL,
						(void *)FimJogo,
						NULL
		);

		close(pipe1[0]); // fecha leitura no pipe1
		close(pipe2[1]);  // fecha escrita no pipe2

		jogador1(pipe2[0], pipe1[1], lista_aberta);  // Chama jogador1 no PAI

		// FIM DO PROCESSO PAI
    }else{ 
	// PROCESSO FILHO
		
		// INICIA A THREAD QUE CONTA OS PONTOS DO JOGADOR 2(processo filho)
   		pthread_create(
						&thread_pontos_jogador,
						NULL,
						(void *)ContaPontosJogador,
						(int *)2
		);

		close(pipe1[1]); // fecha escrita no pipe1
		close(pipe2[0]);  // fecha leitura no pipe2

		jogador2(pipe1[0], pipe2[1], lista_aberta);  // Chama jogador2 no FILHO
		
	// FIM DO PROCESSO FILHO
	} 
} // FIM DO MAIN

/* 
--------------------------------------------------------------------------------------------
Funcao jogador1: 	
	Executada no processo PAI
	Joga o jogo da memoria 
	Envia a lista de acertos para FILHO
	Recebe a lista de acertos do FILHO
--------------------------------------------------------------------------------------------
*/
void jogador1(int readfd, int writefd, List *lista_aberta){
	char buff[MAXBUFF];
    List *lista_acertos_jogador1 = new_list();

	while(1){
		int i;

		// APENAS FICA ESPERANDO A THREAD FINALIZAR O JOGO
		while(list_length(lista_acertos) == 10) continue; 

		printf("------------------------\nVEZ DO JOGADOR 1\n");		
		printGrade(lista_aberta, lista_acertos, -1); // FUNCAO QUE PRINTA A GRADE DO JOGO

		pthread_mutex_lock(&mutex); // GARANTE EXCLUSIVIDADE MUTUA PELA VARIAVEL pontos_jogador
        rodadaJogo(lista_aberta, lista_acertos, lista_acertos_jogador1, &pontos_jogador);
		pthread_mutex_unlock(&mutex);
		
		// TRANSFORMA A LISTA EM STRING PARA ENVIAR PARA O FILHO
		for(i = 0; i < list_length(lista_acertos_jogador1); i++){
			buff[i] = get_data_list(lista_acertos_jogador1, i);
		}
		buff[i+1] = '\0';

		write(writefd, buff, MAXBUFF);
		
		read(readfd, buff, MAXBUFF);

		// TRANSFORMA A MENSAGEM DO BUFFER EM LISTA PARA O FUNCIONAMENTO DO JOGO
		for (i = 0; buff[i] != '\0'; i++){
			if(!on_list(lista_acertos, buff[i]))
				append_list(lista_acertos, buff[i]);
		}
    }
} // Fim da Funcao jogador1

/* 
--------------------------------------------------------------------------------------------
Funcao jogador1: 	
	Executada no processo FILHO
	Recebe a lista de acertos do PAI
	Joga o jogo da memoria 
	Envia a lista de acertos para o PAI
--------------------------------------------------------------------------------------------
*/
void jogador2(int readfd, int writefd, List *lista_aberta){
	char buff[MAXBUFF];
    List *lista_acertos_jogador2 = new_list();

	while(1){
		int i;
	    read(readfd,buff,MAXBUFF); 

		// VERIFICA MENSAGEM DE FIM DE JOGO ENVIADA PELA THREAD DO PAI
		if (strcmp(buff, "00") == 0) { 
			close(readfd);  
			close(writefd); 
			exit(0); // FINALIZA O PROCESSO FILHO
		} 
		
		// TRANSFORMA A MENSAGEM DO BUFFER EM LISTA PARA O FUNCIONAMENTO DO JOGO
		for (i = 0; buff[i] != '\0'; i++){ 
			if(!on_list(lista_acertos, buff[i]))
				append_list(lista_acertos, buff[i]);
		}
		
		printf("------------------------\nVEZ DO JOGADOR 2\n");		
		printGrade(lista_aberta, lista_acertos, -1); // FUNCAO QUE PRINTA A GRADE DO JOGO

		pthread_mutex_lock(&mutex); // GARANTE EXCLUSIVIDADE MUTUA PELA VARIAVEL pontos_jogador
		rodadaJogo(lista_aberta, lista_acertos, lista_acertos_jogador2, &pontos_jogador); // FUNCAO DA LOGICA DO JOGO
		pthread_mutex_unlock(&mutex);
		
		// TRANSFORMA A LISTA EM STRING PARA ENVIAR PARA O PAI
		for(i = 0; i < list_length(lista_acertos_jogador2); i++){
			buff[i] = get_data_list(lista_acertos_jogador2, i);
		}
		buff[i+1] = '\0';

		write(writefd, buff, MAXBUFF);
    }
} // Fim da Funcao jogador2

// A CADA 30 SEGUNDOS PRINTA OS PONTOS DO JOGADOR, UM PARA CADA PROCESSO
void *ContaPontosJogador(int jogador){
	while(1){
		sleep(30); 
		pthread_mutex_lock(&mutex);                                     
		printf("\n\n========================\nPONTOS DO JOGADOR %d: %d\n========================\n\n", jogador, pontos_jogador);
		pthread_mutex_unlock(&mutex);
	}
}

// SOMENTE EXECUTADA PELO PROCESSO PAI (FINALIZA O JOGO)
void *FimJogo(){
	while (1){
		if(list_length(lista_acertos) == 10){ // TODAS AS PECAS FORAM VIRADAS
		
			write(pipe1[1], "00", MAXBUFF); // ENVIA NO BUFFER QUE O JOGO TERMINOU
			
			printf("\n========================\nFIM DE JOGO:\n------------------------\n");

			if(pontos_jogador == 5) // SE O PROCESSO PAI TEM 5 PONTOS, O FILHO TAMBEM TEM
				printf("EMPATE\n========================\n");
			else if(pontos_jogador > 5) // SE O PROCESSO PAI TEM MAIS DE 5 PONTOS ELE GANHOU
				printf("VITORIA DO JOGADOR 1\n========================\n");
			else // PAI PERDEU
				printf("VITORIA DO JOGADOR 2\n========================\n");


			close(pipe2[0]);  
			close(pipe1[1]); 
			exit(0); // FIM DO PROCESSO PAI
		}
	}
}
