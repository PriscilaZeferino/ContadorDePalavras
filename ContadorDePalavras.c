#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#define tamanhoArray 10
#define tamanhoChars 50

int sizeArray = tamanhoArray;
int sizeChars = tamanhoChars;

typedef struct palavras {
    int count;
    char words[tamanhoChars];
} PALAVRAS;

PALAVRAS *palavras;

int posWord = 0;
int pos = 0;

FILE *arq_ler;
FILE *arq_escrever;

char caminho[tamanhoChars];

int AlocarMemoria() {
    PALAVRAS *ponteiro = (PALAVRAS *) malloc(sizeArray * sizeof(PALAVRAS));
    if(ponteiro == NULL)
    {
        return 1;
    }
        palavras = ponteiro;
    return 0;
}

int realocarMemoria() {
    sizeArray++;
    PALAVRAS *ponteiro = (PALAVRAS *) realloc(palavras, sizeArray * sizeof(PALAVRAS));
    if(ponteiro == NULL)
    {
        return 1;
    }     
    palavras = ponteiro;
    return 0;
}

void limparDados()
{
    for(int i = 0; i <= sizeArray; i++) {
        strcpy(palavras[i].words, "vazio");
    }

    posWord = 0;
    pos = 0;
}

void limparWord(char word[])
{
    for(int i = 0; i <= tamanhoChars; i++)
    {
        word[i] = '\0';
    }
}

int abrirArquivoParaLeitura() 
{
    arq_ler = fopen(caminho, "r");
    if(arq_ler == NULL) 
        return 1;
    else
        return 0;
}

int procuraPalavra(char wordSearch[]) {
    int count = 0;
    for(int i = 0; i <= sizeArray; i++)
    {
        if(strcmp(palavras[i].words, wordSearch) == 0)
        {
            return count;
        }
        count++;
    }
    return -1;
}

int lerArquivo()
{
    char c;
    char word[sizeChars];
    posWord = 0;
    
    while(!feof(arq_ler))
    {
        c = getc(arq_ler);
        if(c == ' ' || c == '\n' || c == '\t' || c == '-' || c == ';' || c == ':' || c == '?' || c == ',' || c == '.' || c == '\0' || c == '\v' || c == '\f' || c == '\r')
        {

            if(posWord < sizeArray)
            {

            if((strcmp("", word) != 0) && (strcmp(" ", word) != 0) && word[0] != '\0' && word[0] != '\n') 
            {
                int procura = procuraPalavra(word);
                if(procura == -1)
                {

                        strcpy(palavras[posWord].words, word);
                        palavras[posWord].count++;
                        limparWord(word);
                        pos = 0;
                        posWord++;
                }
                else
                {
                    palavras[procura].count++;
                    limparWord(word);
                    pos = 0;
                }
            }

            }  
            else
            {
                                sizeArray++;
                int memoria_realocada = realocarMemoria();
                if(memoria_realocada == 1) {
                    printf("Não há mais espaço disponivel! \n");
                    break;
                }
            }

        }
        else
        {
            word[pos] = c;
            pos++;
        }
    }
    return 0;
}

// void exibirDados()
// {    
//     printf("ID      QTD_DE_PALAVRAS         PALAVRAS \n");

//     for(int i = 0; i <= sizeArray; i++)
//     {
//         if(strcmp("vazio", palavras[i].words) != 0)
//         {
//             printf("%d              %d      %s \n", i, palavras[i].count, palavras[i].words);
//         }
//     }
// }

int abrirArquivoParaEscrita() 
{
    arq_escrever = fopen(caminho, "w");
    if(arq_escrever == NULL) 
        return 1;
    else
        return 0;
}

int salvarDadosNoArquivo(){
    for(int i = 0; i < sizeArray; i++)
    {
        fprintf(arq_escrever, "%d    %s\n", palavras[i].count, palavras[i].words);
    }    
    return 0;    
}

int main (int argc, const char * argv[])
{
    int resposta = AlocarMemoria();

    if(resposta == 1) 
    {
        printf("Não foi possivel alocar memória! \n");
    }
    else {    
        //printf("Memória alocada com sucesso! \n");
        limparDados();

        printf("Informe o caminho para leitura do arquivo: \n");
        printf("Exemplo: \n");
        printf("/Users/priscila/Desktop/Contador/teste.txt\n");

        fflush(stdin);
        scanf("%[^\n]", caminho);

        int abrir_arquivo = abrirArquivoParaLeitura();

        if(abrir_arquivo == 0)
        {   
            printf("Lendo arquivo...");
            int leitura_arquivo = lerArquivo(); 
            if(leitura_arquivo == 0)
            {
                printf("Leitura concluída! \n");
                fclose(arq_ler);
                //exibirDados();

                printf("Informe o caminho para o novo arquivo: \n");
                printf("Exemplo: \n");
                printf("/Users/priscila/Desktop/Contador/nomedoarquivo.txt\n");

                fflush(stdin);
                scanf("%[^\n]", caminho);

                int escrita_arquivo = abrirArquivoParaEscrita();
                if(escrita_arquivo == 1)
                {
                    printf("Infelizmente, não foi possível criar o arquivo. \n");
                }
                else
                {
                    printf("Arquivo criado. Salvando dados.... \n");
                    int dados_salvo = salvarDadosNoArquivo();
                    if(dados_salvo == 0)
                        printf("Dados salvos com sucesso! \n");
                    else
                        printf("Algo deu errado... :(\n");
                }
            }
        }
        else
        {    printf("Infelizmente, não foi possivel abrir o arquivo: \n");
        }
    }
    fclose(arq_escrever);
    free(palavras);
    return 0;
}