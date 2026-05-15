#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QUANTIDADE_MAX_INGRESSOS 1000

// Identificadores que vão dizer o que a pessoa é
#define IDENTIFICADOR_ALUNO 1
#define IDENTIFICADOR_COMUNIDADE 2
#define IDENTIFICADOR_IDOSO 3

// Estrutura base que servirá para todo mundo. Diferenciação APENAS por
// identificador
typedef struct Pessoa {
    char nome[255];
    int identificador;
    char cpf[15];
    double credito;
    struct Pessoa* proximaPessoa;
} Pessoa;

typedef struct Fila {
    Pessoa* inicio;
    Pessoa* fim;
} Fila;

// STANDBY: Fila geral que vai guardar as outras filas
// typedef struct GerenciadorFila{
//     Fila filaAlunos;
//     Fila filaIdosos;
//     Fila filaComunidade;
//     int quantidadePessoas;
// } GerenciadorFila;

// Menu para Testes
void menu() {
    printf("\nc - Comprar Ingressos\n");
    printf("a - Exibir Fila Alunos\n");
    printf("i - Exibir Fila Idosos\n");
    printf("k - Exibir Fila Comunidade\n");
    printf("s - Comprar Créditos\n");
}

// Funções de Fila
int inicializar(Fila* fila) {
    if (fila == NULL) {
        return 0;
    }
    fila->inicio = NULL;
    fila->fim = NULL;
    return 1;
}

// Essa função apenas centraliza as chamadas de inicialização
int inicializarTodasFilas(Fila* fAlunos, Fila* fComunidade, Fila* fIdosos) {
    inicializar(fAlunos);
    inicializar(fComunidade);
    inicializar(fIdosos);
    return 1;
}

int enfileirar(Fila* fila, char nome[255], char cpf[15], int identificador) {
    if (fila == NULL) {
        return 0;
    }
    Pessoa* novoAluno = (Pessoa*)malloc(sizeof(Pessoa));

    if (novoAluno == NULL) {
        return 0;
    }
    strcpy(novoAluno->nome, nome);
    strcpy(novoAluno->cpf, cpf);
    novoAluno->identificador = identificador;
    novoAluno->proximaPessoa = NULL;

    // Lista Vazia
    if (fila->inicio == NULL) {
        fila->inicio = novoAluno;
        fila->fim = novoAluno;
        return 1;
    }

    Pessoa* ultimoAluno = fila->fim;
    ultimoAluno->proximaPessoa = novoAluno;
    fila->fim = novoAluno;
    return 1;
}

// Verifica se a fila está vazia (1 ou 0)
int is_vazia(Fila* fila) { return fila->inicio == NULL; }

void exibirFila(Fila* fila) {
    if (fila == NULL || is_vazia(fila)) {
        return;
    }

    printf("\n");
    Pessoa *primeiroFila = fila->inicio;
    while(primeiroFila != NULL){
        printf("Nome: %s | CPF: %s | Identificador: %d | Créditos: R$%.2f\n",
                primeiroFila->nome,
                primeiroFila->cpf,
                primeiroFila->identificador,
                primeiroFila->credito
        );
        primeiroFila = primeiroFila->proximaPessoa;
    }
}

// Verifica em qual fila a pessoa se encaixa e retorna o índice do identificador
int destinarFila(int identificador) {
    if (identificador != IDENTIFICADOR_ALUNO &&
        identificador != IDENTIFICADOR_COMUNIDADE &&
        identificador != IDENTIFICADOR_IDOSO) {
        return 0;
    }

    if (identificador == IDENTIFICADOR_ALUNO) {
        return IDENTIFICADOR_ALUNO;
    } else if (identificador == IDENTIFICADOR_COMUNIDADE) {
        return IDENTIFICADOR_COMUNIDADE;
    } else {
        return IDENTIFICADOR_IDOSO;
    }
}

// Adiciona créditos no "objeto". Somente para aluno
// Na chamada da função: colocar verificação para NÃO deixar colocar valor = 0 or < 0
double comprarCreditos(Pessoa *pessoa, double valorCreditoAdicionar){
    if(pessoa == NULL){
        return 0;
    }

    if (pessoa->identificador != IDENTIFICADOR_ALUNO) {
        return 0;
    }

    pessoa->credito += valorCreditoAdicionar;
    return valorCreditoAdicionar;
}

long double calculadorSegundos(int dia, int mes, int ano) {
    return dia * 86400.0L + mes * 2.628e+6L + ano * 3.154e+7L;
}

// Essa função busca uma pessoa de uma fila passada pelo CPF
// TO DO: ver a possibilidade de um ID incremental para a busca: resolveria o problema de 1 pessoa estar várias vezes (3) na fila
// - Surgiria a dúvida: esse crédito é para qual ingresso? Já que o aluno pode ter 3
Pessoa *getPessoa(Fila *fila, char cpfBuscar[15]){
    if(fila == NULL || is_vazia(fila)){
        return NULL;
    }

    Pessoa *primeiroFila = fila->inicio;
    while(primeiroFila != NULL){
        if(strcmp(primeiroFila->cpf, cpfBuscar) == 0){
            return primeiroFila;
        }

        primeiroFila = primeiroFila->proximaPessoa;
    }

    return NULL;
}

int main(){
    int estaNoDia = 0;

    time_t agora = time(NULL);

    struct tm dataFesta = {0};
    dataFesta.tm_mday = 28;
    dataFesta.tm_mon = 4;     // meses vão de 0 a 11
    dataFesta.tm_year = 126;  // 2026 - 1900

    time_t festa = mktime(&dataFesta);

    if (difftime(festa, agora) > 0) {
        estaNoDia = 1;
    }

    char opcao;
    int continuar = 1;

    Fila filaAlunos;
    Fila filaComunidade;
    Fila filaIdosos;
    inicializarTodasFilas(&filaAlunos, &filaComunidade, &filaIdosos);

    while (continuar) {
        menu();
        printf("\nOpção: ");
        scanf(" %c", &opcao);

        switch (opcao) {
            case 'c':
                char nome[255], cpf[15];
                int identificadorPessoa;

                while(getchar() != '\n');  // Limpando o buffer, resolvendo bug de leitura

                printf("\nInforme seu nome: ");
                fgets(nome, 255, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("Informe seu CPF: ");
                fgets(cpf, 15, stdin);
                cpf[strcspn(cpf, "\n")] = '\0';

                printf("1 - Sou Aluno | 2 - Sou Comunidade | 3 - Sou Idoso: ");
                scanf("%d", &identificadorPessoa);

                int numeroFila = destinarFila(identificadorPessoa);

                // Verifica qual é a fila destinada
                if (numeroFila != 0) {
                    if (numeroFila == IDENTIFICADOR_ALUNO) {
                        enfileirar(&filaAlunos, nome, cpf, IDENTIFICADOR_ALUNO);

                    } else if (numeroFila == IDENTIFICADOR_IDOSO) {
                        enfileirar(&filaIdosos, nome, cpf, IDENTIFICADOR_IDOSO);

                    } else {
                        enfileirar(&filaComunidade, nome, cpf, IDENTIFICADOR_COMUNIDADE);
                    }

                } else {
                    printf("\nNENHUM TIPO DE VÍNCULO SUPORTADO FOI SELECIONADO!\n");
                }

                break;
            
            case 'a': exibirFila(&filaAlunos); break;
            case 'i': exibirFila(&filaIdosos); break;
            case 'k': exibirFila(&filaComunidade); break;
            case 's':
                char cpfBuscar[15];
                while(getchar() != '\n');

                printf("\nInforme seu CPF: ");
                fgets(cpfBuscar, 15, stdin);
                cpfBuscar[strcspn(cpfBuscar, "\n")] = '\0';

                Pessoa *pessoaBuscada = getPessoa(&filaAlunos, cpfBuscar);
                if(pessoaBuscada == NULL){
                    printf("\nCPF não pertence a um aluno OU a fila está vazia\n");

                } else{
                    double valorCreditoColocar;
                    printf("Informe a quantidade de crédito: ");
                    scanf("%lf", &valorCreditoColocar);

                    if(valorCreditoColocar <= 0){
                        printf("\nO valor deve ser maior que R$0.00!\n");

                    } else{
                        double valorResultado = comprarCreditos(pessoaBuscada, valorCreditoColocar);

                        if(valorResultado == 0){
                            printf("\nOcorreu algum erro na compra dos créditos!\n");

                        } else{
                            printf("R$%.2f de créditos adicionado com sucesso!\n", valorResultado);
                        }
                    }
                }

                break;
            default: printf("\nOPÇÃO INVÁLIDA!\n"); break;
        }
    }

    return 0;
}

/* OBSERVAÇÕES FUTURAS PARA IMPLEMENTAÇÃO: */

// 1 - Colocar data: antes do dia 28: 3 ingressos. No dia: ordem, sem 3
// 2 - Código de geração de CSV aleatório: Enzo:

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