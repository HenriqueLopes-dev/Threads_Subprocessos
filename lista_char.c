#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node{
	int index;
	char data;
	struct node *pre;
	struct node *next;
}Node;

typedef struct List{
	Node *first;
	Node *last;
}List;


void pop_last_list(List *list);
void remove_list(List *list, char data);
void extend_list(List *list, List *list2);
void pop_list(List *list, int index);
void reverse_list(List *list);
void copy_list(List *new_list, List *list);
void clear_list(List *list);
void insert_list(int index, List *list, char new_data);
void append_list(List *list, char new_data);
void first_node(List *list, char new_data);
void print_list(List *list);
void destroy_list(List *list);
void *safe_malloc(size_t num_itens, size_t tam);
char get_data_list(List *list, int index);
char get_last_data_list(List *list);
int count_list(List *list, char data);
int list_length(List *list);
bool on_list(List *list, char data);
List *new_list();


/*
int main(){	
	List *lista = new_list();
	
	append_list(lista, 'A');
	append_list(lista, 'B');
	append_list(lista, 'C');
	append_list(lista, 'D');
	
	print_list(lista);

	remove_list(lista, 'A');
	remove_list(lista, 'x');

	printf("\n");
	print_list(lista);
	
	destroy_list(lista);
	
	printf("seu programa finalizou!\n");

	return 0;	
}
*/

void append_list(List *list, char new_data){

	if(!list->first){
		first_node(list, new_data);	
		return;
	}
	
	Node *new_node = safe_malloc(1, sizeof(Node));
	new_node->data = new_data;
	new_node->index = (list->last->index + 1);
	new_node->next = NULL;
	new_node->pre = list->last;

	list->last->next = new_node;

	list->last = new_node;
	
}

void insert_list(int index, List *list, char new_data){
	if(!list || index < 0) return;

	if(!list->first){
		if(index != 0) return;
		first_node(list, new_data);
		return;					
	}

	if(index > list->last->index){
		append_list(list, new_data);
		return;
	}
	
	Node *new_node = safe_malloc(1, sizeof(Node));
	new_node->data = new_data;
	new_node->index = index;
	new_node->pre = NULL;
    new_node->next = NULL;
	
	Node *aux = list->first;
	
	while(aux){
		if(aux->index == index){
			new_node->next = aux;
			new_node->pre = aux->pre;
					
			if(aux->pre){
				aux->pre->next = new_node;
			}else{
				list->first = new_node;
			}
			aux->pre = new_node;
			
			while(aux){
				aux->index += 1;
				aux = aux->next;
			}
			
			return;
		}	
		aux = aux->next;
	}
}

void first_node(List *list, char new_data){
	Node *new_node = safe_malloc(1, sizeof(Node));
	new_node->data = new_data;
	new_node->index = 0;
	new_node->pre = NULL;
	new_node->next = NULL;
	
	list->first = new_node;
	list->last = new_node; 
}

void print_list(List *list){
	if(!list || !list->first) return;
	
	Node *aux = list->first;

	while(aux){
		printf("%c    %d\n", aux->data, aux->index);
		aux = aux->next;
	}
}

List *new_list(){
	List *new_list = safe_malloc(1, sizeof(List));
	
	new_list->first = NULL;
	new_list->last = NULL;
	return new_list;
}

void destroy_list(List *list){
	clear_list(list);
	free(list);
}

void clear_list(List *list){
	if(!list) return;

	Node *cleaner = list->first;

	while(cleaner){
		Node *aux = cleaner->next;
		free(cleaner);
		cleaner = aux;
	}
	list->first = NULL;
	list->last = NULL;
}

void *safe_malloc(size_t num_itens, size_t tam) {
    void *ptr = malloc(num_itens * tam);
    if (ptr == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória.\n");
        exit(EXIT_FAILURE); 
    }
    return ptr;
}

int list_length(List *list){
    if (!list || !list->last) return 0;

	return list->last->index + 1;
}

void copy_list(List *new_list, List *list){
	if(!list) return;
	clear_list(new_list);
	Node *aux = list->first;
	
	while(aux){
		append_list(new_list, aux->data);
		aux = aux->next;	
	}
}

int count_list(List *list, char data){
	if(!list) return -1;

	int i = 0;
	Node *aux = list->first;
		
	while(aux){
		if(aux->data == data) i++;
		aux = aux->next;	
	}
	return i;
}

void reverse_list(List *list){
	if(!list) return;
	
	List *aux_list = new_list();
	copy_list(aux_list, list);
	clear_list(list);
	Node *aux_node = aux_list->last; 
	
	while(aux_node){
		append_list(list, aux_node->data);
		aux_node = aux_node->pre;	
	}
	
	destroy_list(aux_list);
}

void pop_list(List *list, int index){
	if(!list || !list->first) return;

	Node *aux = list->first;
	
	while(aux){
		if(aux->index == index){
			Node *update = aux->next;
			
			if(!aux->pre) list->first = aux->next;
			else aux->pre->next = aux->next;
			
			if(!aux->next) list->last = aux->pre;
			else aux->next->pre = aux->pre;			
			
			free(aux);

			while(update){
				update->index -= 1;
				update = update->next;
			}
			return;
		}
		aux = aux->next;
	}
	
}

void extend_list(List *list, List *list2){	
	Node *aux = list2->first;
	int i;

	for(i = 0; i < list_length(list2); i++){
		append_list(list, aux->data);
		aux = aux->next;
	}
	
}

void remove_list(List *list, char data){
	if (!list || !list->first) return;
	
	Node *aux = list->first;
			
	while(aux){
		if(aux->data == data){
			pop_list(list, aux->index);
			break;
		}		
		aux = aux->next;	
	}	
}

bool on_list(List *list, char data){
	if (!list || !list->first) return false;
	
	Node *aux = list->first;

	while (aux)
	{
		if (aux->data == data) return true;
		
		aux = aux->next;
	}
	return false;
}
	
char get_data_list(List *list, int index){
	if (!list || !list->first) return '\0';

	Node *aux = list->first;

	while (aux){
		if (aux->index == index) return aux->data;
		
		aux = aux->next;
	}
	return '\0';
}

char get_last_data_list(List *list){
	if (!list || !list->first) return '\0';

	return list->last->data;
}

void pop_last_list(List *list){
	if (!list || !list->first) return;

	Node *aux = list->last;

	if(!aux->pre){ // LISTA COM APENAS 1 ELEMENTO
		list->first = NULL;
		list->last = NULL;
		free(aux);
		return;
	}

	aux->pre->next = NULL;
	list->last = aux->pre;
	free(aux);
}
