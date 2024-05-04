#include <stdio.h>
#include <stdlib.h>
#include <string.h>
///struct Jogador
//{
    //char nome[50];
    //int JogosRealizados;
    //int Vitorias;
//};

typedef struct {
    int id;
    char nome[50];
    int JogosRealizados;
    int Vitorias;
} Jogador;


// Função para ler de ficheiros .csv
void lerCSV(const char *filename) {
    FILE *file = fopen(filename, "r");
    char linha[1024];
    if (file) {
        while (fgets(linha, sizeof(linha), file)) {
            printf("%s", linha);
        }
        fclose(file);
    } else {
        printf("Erro ao abrir o ficheiro.\n");
    }
}

// Função para escrever em ficheiros .csv
void escreverCSV(const char *filename, const char *dados) {
    FILE *file = fopen(filename, "a"); 
    if (file) {
        fprintf(file, "%s\n", dados);
        fclose(file);
    } else {
        printf("Erro ao abrir o ficheiro.\n");
    }
}

// Função para verificar se o nome do jogador já existe
int jogadorExiste(const char *filename, const char *nomeJogador) {
    FILE *arquivo = fopen(filename, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0; // Retorna falso se houver um erro ao abrir o arquivo
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char *nome = strtok(linha, ",");
        if (strcmp(nome, nomeJogador) == 0) {
            fclose(arquivo);
            return 1; // Retorna verdadeiro se o nome do jogador for encontrado
        }
    }

    fclose(arquivo);
    return 0; // Retorna falso se o nome do jogador não for encontrado
}

// Função para adicionar um jogador ao arquivo CSV
int adicionarJogador(const char *filename, int id_atual) {
    Jogador c;
    //(*id_atual) += 1;
    c.id = id_atual;
    printf("VELHOTE '%d'", c.id);
    printf("Digite o nome do novo jogador: ");
    scanf("%s", c.nome);

    if (jogadorExiste(filename, c.nome)) {
        printf("Jogador com o nome '%s' já existe.\n", c.nome);
        return 0;
    }

    FILE *arquivo = fopen(filename, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    fprintf(arquivo, "%d,%s\n", c.id, c.nome);
    fclose(arquivo);
    
    return id_atual + 1;
}

void listarJogadores(const char *filename) {
    lerCSV(filename);
}

void BuildBoard(int Lin, int Col, char Brd[1000][1000])
{
    int idxCol, idxLin, aux;

    for (idxLin = 0; idxLin < Lin; idxLin++)
    {
        for (idxCol = 0; idxCol <= Col; idxCol++)
        {
            aux = idxCol % 2;

            if (aux == 0)
                Brd[idxLin][idxCol] = '|';
            else
                Brd[idxLin][idxCol] = '_';
        }
    }
}

void ShowBoard(int Lin, int Col, char Brd[1000][1000])
{
    int idxCol, idxLin;

    for (idxLin = 0; idxLin < Lin; idxLin++) // Linhas
    {
        for (idxCol = 0; idxCol <= Col; idxCol++) // Colunas
        {
            printf("%c", Brd[idxLin][idxCol]);
        }
        printf("\n");
    }
}

void InsertPiece(char PieceType, int PieceQntty, int StartIndex, int Direction, char Brd[1000][1000], int LinNum)   
{
    // StartIndex é o numero da coluna
    // PieceType é o caracter a jogar
    // PieceQntty é a quantidade de peças a jogar
    // Direction é a direção que vão ser jogadas as peças, esquerda ou direita          // 1 - Direita   0 - Esquerda
    // LinNum é o numero de linhas do tabuleiro

    int aux;
    int idxLin;
    int idxColumn;
    int auxStartIndex = (StartIndex * 2) - 1; // Controla o numero da coluna que está a ser rodado
    //(No caso de direita vai andar para a frente, caso de esquerda para trás)

    // Controlar a passagem das colunas se for mais que 1 peça

    for (idxColumn = 0; idxColumn < PieceQntty; idxColumn++)
    {
        // Verificar as linhas da coluna StartIndex de baixo para cima,
        // para perceber quais as linhas nessa coluna já estão ou não ocupadas

        for (idxLin = LinNum; idxLin >= 0; idxLin--)
        {

            if (Brd[idxLin][auxStartIndex] == '_')
            {
                Brd[idxLin][auxStartIndex] = 'X';
                break;
            }
        }

        if (Direction)
        {
            // Jogada para a direita
            auxStartIndex = auxStartIndex + 2;
        }
        else
        {
            // Jogada para a esquerda

            if (auxStartIndex != 1)
                auxStartIndex = auxStartIndex - 2;
        }
    }
}

int VictoryVerification(int Lin, int Col, char Brd[1000][1000], char CurrentPlayerChar, int VictSeq) //CurrentPlayerChar representa o objeto do jogador
{
    int idxCol, idxLin, aux;
    int ColSeqCount = 1, LinSeqCount = 1;

    //VERIFICAÇÃO DE VITÓRIA DAS LINHAS
    for (idxLin = 0; idxLin < Lin; idxLin++) // Linhas
    {
        LinSeqCount = 0;

        for (idxCol = 1; idxCol <= Col; idxCol++) // Colunas
        {
            aux = (idxCol * 2) - 1;
        
            if(Brd[idxLin][aux] == CurrentPlayerChar)
            {
                LinSeqCount++;

                if(LinSeqCount == VictSeq)
                    return 1;
            }
            else
                LinSeqCount = 1; // A Contagem da sequencia volta a 1 porque a peça asseguir não é igual
        }
    }

    //VERIFICAÇÃO DE VITÓRIA DAS COLUNAS
    for (idxCol = 0; idxCol < Col; idxCol++) // Colunas
    {
        for (idxLin = 1; idxLin <= Lin; idxLin++) // Linhas
        {
            
        }
    }


    return 0;
}

int main()
{
    int id_atual = 0;
    int opcao;
    char filename[] = "jogadores.csv";

    do {
        printf("**********Bem-vindo!**********");
        printf("\nMenu:\n1 - Adicionar Jogador\n2 - Listar Jogadores\n0 - Começar Jogo\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Valor atual do ID antes de adicionar jogador: %d\n", id_atual);
                id_atual = adicionarJogador(filename, id_atual);
                printf("Valor atual do ID após adicionar jogador: %d\n", id_atual);
                break;
            case 2:
                listarJogadores(filename);
                break;
            case 0:
                break;
            default:
                printf("Por favor, tente novamente. Opção invalida!\n");
        }
    } while (opcao != 0);

    char board[1000][1000];

    int WinSeq;
    int Lines;
    int Columns;

    Jogador player[2];
    int idxPl;

    printf("Indique a sequencia vencedora: ");
    scanf("%d", &WinSeq);

    Lines = WinSeq;
    Columns = WinSeq;
    
    Columns = Columns * 2;

    // Montagem e demonstração do tabuleiro
    BuildBoard(Lines, Columns, board);
    ShowBoard(Lines, Columns, board);

    //Execução do jogo
    do
    {
        // Colocação das peças
        int PcQntty;
        int StrtIdx;
        int Drctn; // 1 - Direita   0 - Esquerda
        char piece;

        printf("\n\nQuantidade de pecas a jogar: ");
        scanf("%d", &PcQntty);
        printf("Posicao Inicial: ");
        scanf("%d", &StrtIdx);
        printf("Sentido da jogada: ");
        scanf("%d", &Drctn);

        InsertPiece(piece, PcQntty, StrtIdx, Drctn, board, Lines);
        ShowBoard(Lines, Columns, board);
        
        // Função de verificação de vitória
        if(VictoryVerification(Lines,Columns/2,board,'X',WinSeq))
            printf("\n\nVITORIA!");
        
    } while (1);

    return 0;
}