#include <stdio.h>

#define QUANTIDADE_INGRESSOS 1000
#define ALUNO 1
#define COMUNIDADE 2
#define IDOSO 3

// Comidas e Bebidas
typedef struct Bebida{
    char nome[100];
    float preco;
    int quantidadeEstoque;
} Bebida;

// Entidades
typedef struct FilaPessoas{
    int id;
    char nome[100];
} FilaPessoas;

// Filas

typedef struct FilaGeral{
    FilaPessoas idosos;
    FilaPessoas comunidade;
    FilaPessoas alunos;
} FilaGeral;


int comprar_creditos(){

}

int main(){
    return 0;
}


// Colocar data: antes do dia 28: 3 ingressos. No dia: ordem, sem 3




























/* int gerarRegistros() {
    FILE* f = fopen("participantes.csv", "w");
    if (!f) return 0;
    srand(time(NULL));

    for (int i = 0; i < MAX; i++) {
        int comida = rand() % 3 + 1;
        int bebida = rand() % 5 + 1;
        int quantidadeEstoque = rand() % 3 + 1;

        fprintf(f, "%d;%d;%d;%d\n", i + 1, carne, bebida, musica);
    }

    fclose(f);
    return 1;
} */