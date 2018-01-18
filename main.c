#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

int main(int argc, char **argv) {
	FILE *arq;
	PGM img;

	if(argc != 2) {
		printf("Uso: %s nome_da_imagem\n", *argv);
		exit(1);
	}

	if(!(arq = fopen(*(argv+1), "rb"))) {
		printf("Não foi possível abrir o arquivo.\n");
		exit(1);
	}

	img = ler_imagem(arq, img);

	imprimir_mtr(img.mtr, img.l, img.c);

	free(img.mtr);
	fclose(arq);

	return 0;
}

//decodifica uma imagem PGM.
PGM ler_imagem(FILE *arq, PGM img) {
	fscanf(arq, "%s", img.chave);
	fscanf(arq, "%hu %hu %hhu", &img.l, &img.c, &img.max);

	img.mtr = malloc(img.l * img.c * sizeof(unsigned char));
	if(!img.mtr) {
		printf("Não foi possível alocar memória para a matriz da imagem.\n");
		exit(1);
	}

	ler_matriz(arq, img);

	return img;
}

//lê a matriz de uma imagem PGM, de acordo com seu tipo.
void ler_matriz(FILE *arq, PGM img) {
	if((*(img.chave) == 'P') && (*(img.chave + 1) == '2')) {
		ler_mtr_p2(arq, img.mtr, img.l, img.c);
	} else if((*(img.chave) == 'P') && (*(img.chave + 1) == '5')) {
		ler_mtr_p5(arq, img.mtr, img.l, img.c);
	} else {
		printf("Formato inválido ou não identificado.\n");
		exit(1);
	}
}

//lê matriz P2 (ascii).
void ler_mtr_p2(FILE *arq, unsigned char *mtr, unsigned short l, unsigned short c) {
	register int i, j;
	for(i = 0; i < l; i++) {
		for(j = 0; j < c; j++) {
			fscanf(arq, "%hhu", (mtr + (i * c) + j));
		}
	}
}

//lê matriz P5 (bin).
void ler_mtr_p5(FILE *arq, unsigned char *mtr, unsigned short l, unsigned short c) {
	register int i, j;
	for(i = 0; i < l; i++) {
		for(j = 0; j < c; j++) {
			fread((mtr + (i * c) + j), 1, 1, arq);
		}
	}
}

//imprime a matriz de uma imagem PGM.
void imprimir_mtr(unsigned char *mtr, unsigned short l, unsigned short c) {
	register int i, j;
	for(i = 0; i < l; i++) {
		for(j = 0; j < c; j++) {
			printf("%3hhu ", *(mtr + (i * c) + j));
		}
		printf("\n");
	}
}

//aloca uma nova fila.
Fila* criar_fila() {
	Fila *fl = (Fila*) malloc(sizeof(Fila));
	fl->inicio = NULL;
	fl->tamanho = 0;
	return fl;
}

//insere um valor no final da fila.
void inserir_final(Fila *fl, int valor) {
	No *novo_no = (No*) malloc(sizeof(No));
	novo_no->valor = valor;
	novo_no->prox = NULL;
	if(fila_vazia(fl)) {
		fl->inicio = novo_no;
		fl->tamanho++;
	} else {
		ultimo_no(fl)->prox = novo_no;
		fl->tamanho++;
	}
}

//remove um valor da fila, de acordo com FIFO.
int remover_inicio(Fila *fl) {
	int valor_removido;
	No *aux1 = fl->inicio;
	No *aux2 = fl->inicio->prox;
	fl->inicio = aux2;
	valor_removido = aux1->valor;
	free(aux1);
	fl->tamanho--;
	return valor_removido;
}

//retorna o ultimo ponteiro da fila.
No* ultimo_no(Fila *fl) {
	No *aux = fl->inicio;
	while(aux->prox != NULL) {
		aux = aux->prox;
	}
	return aux;
}

//retorna 1 (true) ou 0 (false), indicando se a fila está vazia.
int fila_vazia(Fila *fl) {
	return (fl->inicio == NULL);
}

//esvazia a fila e libera a memória alocada.
void liberar_fila(Fila *fl) {
	No *aux1;
	No *aux2;
	while(aux2 != NULL) {
		aux1 = fl->inicio;
		aux2 = fl->inicio->prox;
		fl->inicio = aux2;
		free(aux1);
		fl->tamanho--;
	}
	free(aux2);
}

//imprime a fila.
void imprimir_fila(Fila *fl) {
	No *aux = fl->inicio;
	while(aux != NULL) {
		printf("%d --> ", aux->valor);
		aux = aux->prox;
	}
	printf("\n");
}

//aplica o algoritmo watershed à matriz da imagem.
unsigned char* wshed(unsigned char *mtr, unsigned short l, unsigned short c) {
	int mask = -2;
	int wshd = 0;
	int init = -1;
	int inqe = -3;
	int levels = 256;
	int curlab = 0;
	int flag = 0;
	Fila *fifo = criar_fila();
	int *labels = (int*) malloc(l * c * sizeof(int));

	for(int i = 0; i < l; i++) {
		for(int j = 0; j < c; j++) {
			*(labels + (i * c) + j) = init;
		}
	}
	
}