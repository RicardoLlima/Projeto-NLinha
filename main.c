#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char nome[50];
    int JogosRealizados;
    int Vitorias;
} Jogador;

// Função para verificar se o nome do jogador já existe no arquivo
int jogadorExiste(const char *filename, const char *nomeJogador) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    char linha[50 * 2]; // Para armazenar o ID e o nome do jogador numa linha
    char nome[50];
    while (fgets(linha, sizeof(linha), file) != NULL) {
        // Extrair o nome do jogador da linha
        sscanf(linha, "%*d,%s", nome);
        // Comparar o nome do jogador com o nome fornecido
        if (strcmp(nome, nomeJogador) == 0) {
            fclose(file);
            return 1; // Retorna verdadeiro se o nome do jogador for encontrado
        }
    }

    fclose(file);
    return 0; // Retorna falso se o nome do jogador não for encontrado
}

// Função para obter o próximo ID disponível
int obterProximoID(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 1; // Se o arquivo não existir, retorna 1 como o primeiro ID
    }

    int id;
    char linha[50]; // Para armazenar o ID e o nome do jogador em uma linha
    while (fgets(linha, sizeof(linha), file) != NULL) {
        // Ler o ID do jogador da linha
        sscanf(linha, "%d,", &id);
    }

    fclose(file);
    return id + 1; // Retorna o próximo ID após o último encontrado no arquivo
}

// Função para remover um jogador pelo seu ID
void removerJogador(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int id;
    printf("Digite o ID do jogador que deseja remover: ");
    scanf("%d", &id);

    // Criar um arquivo temporário para escrever os jogadores, exceto aquele que queremos remover
    FILE *tempFile = fopen("temp.csv", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(file);
        return;
    }

    char linha[50];
    int jogadorEncontrado = 0; // Variável para indicar se o jogador foi encontrado
    while (fgets(linha, sizeof(linha), file) != NULL) {
        int jogadorID;
        sscanf(linha, "%d,", &jogadorID);
        if (jogadorID == id) {
            jogadorEncontrado = 1;
        } else {
            // Se o ID do jogador não for igual ao ID fornecido, escreva a linha no arquivo temporário
            fprintf(tempFile, "%s", linha);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Remover o arquivo original apenas se o jogador foi encontrado e removido
    if (jogadorEncontrado) {
        remove(filename);
        // Renomear o arquivo temporário para o nome do arquivo original
        rename("temp.csv", filename);
        printf("Jogador com o ID '%d' foi removido com sucesso.\n", id);
    } else {
        // Se o jogador não foi encontrado, exibir uma mensagem de erro
        printf("Erro: Jogador com o ID '%d' não encontrado.\n", id);
        remove("temp.csv"); // Remover o arquivo temporário
    }
}

// Função para adicionar um novo jogador ao arquivo CSV
void adicionarJogador(const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Ler o nome do novo jogador
    char nome[50];
    printf("Digite o nome do novo jogador: ");
    scanf("%s", nome);

    // Verificar se o jogador já existe
    if (jogadorExiste(filename, nome)) {
        printf("Jogador com o nome '%s' já existe.\n", nome);
        fclose(file);
        return;
    }

    // Gerar um novo ID para o jogador
    int novoID = obterProximoID(filename);

    // Escrever o novo jogador no arquivo CSV
    fprintf(file, "%d,%s\n", novoID, nome);

    printf("Jogador '%s' adicionado com sucesso (ID: %d).\n", nome, novoID);

    fclose(file);
}

// Função para listar todos os jogadores no arquivo
void listarJogadores(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("ID\tNome\n");
    char linha[50];
    while (fgets(linha, sizeof(linha), file) != NULL) {
        printf("%s", linha);
    }

    fclose(file);
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
    int opcao;
    char filename[] = "jogadores.csv";

    do {
        printf("**********Bem-vindo!**********");
        printf("\nMenu:\n1 - Adicionar Jogador\n2 - Listar Jogadores\n3 - Remover Jogador\n4 - Comecar Jogo\n0 - Sair\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarJogador(filename);
                break;
            case 2:
                listarJogadores(filename);
                break;
            case 3:
                removerJogador(filename);
                break;
            case 0:
                return 0;
            case 4:
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