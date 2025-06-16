#include <time.h>
#include "lista_char.c"

void printGrade(List *lista, List *lista_acertos, int indice);
void rodadaJogo(List *lista, List *lista_acertos, List *lista_acertos_jogadorX, int *pontos_jogador);
int calculoIndice(int simbolo);
List *randGrade();
List *inicilizar_pecas();

void rodadaJogo(List *lista, List *lista_acertos, List *lista_acertos_jogadorX, int *pontos_jogador){
    int indice1, indice2;
    char simbolo1, simbolo2;

    do{
        indice1 = calculoIndice(1);
    }while(indice1 == -1 || on_list(lista_acertos, get_data_list(lista, indice1)));

    printGrade(lista, lista_acertos, indice1);

    do{
        indice2 = calculoIndice(2);
    }while(indice2 == -1 || indice2 == indice1 || on_list(lista_acertos, get_data_list(lista, indice2)));

    printGrade(lista, lista_acertos, indice2);

    simbolo1 = get_data_list(lista, indice1);
    simbolo2 = get_data_list(lista, indice2);

    if (simbolo1 == simbolo2){
        append_list(lista_acertos, simbolo1);
        append_list(lista_acertos_jogadorX, simbolo1);
        
        (*pontos_jogador)++;
        printf("Voce acertou!\n");
        return;
    }
    
    printf("Voce errou!\n");
    return; 
}

int calculoIndice(int simbolo){
    char input[5];
    int ch, input_to_int0, input_to_int2, indice;

    if(simbolo == 1)
        printf("Digite a coordenada do primeiro simbolo:");
    else
        printf("Digite a coordenada do segundo simbolo:");
    
    fgets(input, sizeof(input), stdin);

    if(input[3] != '\n')
        while ((ch = getchar()) != '\n' && ch != EOF);

    input_to_int0 = input[0] - '0';
    input_to_int2 = input[2] - '0';

    indice = (input_to_int0 * 5) + (input_to_int2);

    if(strlen(input) != 4 || input[3] != '\n' || input[1] != ',' 
        || input_to_int0 > 3 || input_to_int2 > 4 || indice < 0 || indice >= 20
    ){
        printf("Coordenadas invalidas. Tente novamente.\n");
        return -1;
    }

    return indice;
}

void printGrade(List *lista, List *lista_acertos, int indice_tentativa){
    char caracter_atual;

    printf("------------------------\n");
    printf("  | 0| 1| 2| 3| 4|\n");
                 
    for (int i = 0; i < 20; i++){

        if ((i+1) % 5 == 1){
            if(i != 0)
                printf("|\n %d", i/5);
            else   
                printf(" 0");
        }
        
        caracter_atual = get_data_list(lista, i);
        
        if (i == indice_tentativa )
            printf("| %c", caracter_atual - 32);

        else if (on_list(lista_acertos, caracter_atual))
            printf("| %c", caracter_atual);
        else
            printf("|%c ", 999);
        
    }
    printf("|\n\n");
}

List *randGrade(){
    int elemento_aleatorio, tam;
    char nova_peca;
    List *pecas = inicilizar_pecas(); 
    List *pecas_finais = new_list();
    srand(time(NULL));

    tam = list_length(pecas);
    
    while(tam > 0){
        elemento_aleatorio = rand() % tam;
        
        nova_peca = get_data_list(pecas, elemento_aleatorio);
        
        pop_list(pecas, elemento_aleatorio);
        
        append_list(pecas_finais, nova_peca);
        tam = list_length(pecas);
    }

    destroy_list(pecas);

    return pecas_finais;
}

List *inicilizar_pecas(){
    List *nova_lista = new_list();

    append_list(nova_lista, 'a');
    append_list(nova_lista, 'a');
    append_list(nova_lista, 'b');
    append_list(nova_lista, 'b');
    append_list(nova_lista, 'c');
    append_list(nova_lista, 'c');
    append_list(nova_lista, 'd');
    append_list(nova_lista, 'd');
    append_list(nova_lista, 'e');
    append_list(nova_lista, 'e');
    append_list(nova_lista, 'f');
    append_list(nova_lista, 'f');
    append_list(nova_lista, 'g');
    append_list(nova_lista, 'g');
    append_list(nova_lista, 'h');
    append_list(nova_lista, 'h');
    append_list(nova_lista, 'i');
    append_list(nova_lista, 'i');
    append_list(nova_lista, 'j');
    append_list(nova_lista, 'j');
    return nova_lista;
}