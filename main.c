#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sqlite3.h>

#define Medida 20

typedef struct {
    int id;
    char nome[50];
    int JogosRealizados;
    int Vitorias;
} Jogador;


sqlite3 *db;
char *zErrMsg = 0;
int rc;


//FUNÇÕES DE MANIPULAÇÃO DO UTILIZADOR ---------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

// Função de retorno para consultas SQL
static int callback(void *NaoUsado, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// Função para abrir a base de dados
void abrirBaseDeDados() {
    rc = sqlite3_open("jogadores.db", &db);
    if (rc) {
        fprintf(stderr, "Não foi possível abrir a base de dados: %s\n", sqlite3_errmsg(db));
        exit(0);
    } else {
        fprintf(stdout, "Base de dados aberta com sucesso.\n");
    }
}

// Função para criar a tabela de clientes, se necessário
void criarTabelaClientes() {
    const char *sql = "CREATE TABLE IF NOT EXISTS JOGADORES("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "NOME TEXT NOT NULL,"
                      "JOGOSJOGADOS INT DEFAULT 0,"
                      "VITORIAS INT DEFAULT 0);";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Tabela JOGADORES criada ou já existente.\n");
    }
}

// Função para adicionar um cliente à base de dados
int adicionarJogador() {
    Jogador c;
    printf("Introduza o nome do novo jogador: ");
    scanf(" %s", c.nome);  
    char sql[200];
    sprintf(sql, "INSERT INTO JOGADORES (NOME) VALUES ('%s');", c.nome);
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    } else {
        fprintf(stdout, "Jogador adicionado com sucesso.\n");
        return sqlite3_last_insert_rowid(db);
    }
}

// Função para remover um jogador à base de dados
int removerJogador() {
    Jogador c;
    printf("Introduza o ID do jogador: ");
    scanf("%d", &c.id);  
    char sql[200]; 
    sprintf(sql, "DELETE FROM JOGADORES WHERE ID = %d;", c.id);
        rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    } else {
        fprintf(stdout, "Jogador removido com sucesso.\n");
        return 0;
    }
}

// Função para apresentar todos os jogadores
void listarJogador() {
    const char *sql = "SELECT * FROM JOGADORES;";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}


//FUNÇÕES DE MANIPULAÇÃO DO TABULEIRO ---------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

void BuildBoard(int Lin, int Col, char Brd[Medida][Medida])
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

void ShowBoard(int Lin, int Col, char Brd[Medida][Medida])
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

void InsertPiece(char PieceType, int PieceQntty, int StartIndex, int Direction, char Brd[Medida][Medida], int LinNum)   
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
                Brd[idxLin][auxStartIndex] = PieceType;
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

int VictoryVerification(int Lin, int Col, char Brd[Medida][Medida], char CurrentPlayerChar, int VictSeq) //CurrentPlayerChar representa o objeto do jogador
{
    int idxCol, idxLin, aux, idxDigLin, idxDigCol;
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
                LinSeqCount = 0; // A Contagem da sequencia volta a 1 porque a peça asseguir não é igual
        }
    }


    //VERIFICAÇÃO DE VITÓRIA DAS COLUNAS
    for (idxCol = 1; idxCol <= Col; idxCol++) // Colunas
    {
        LinSeqCount = 0;

        aux = (idxCol * 2) - 1;

        for (idxLin = 0; idxLin < Lin; idxLin++) // Linhas
        {
            if(Brd[idxLin][aux] == CurrentPlayerChar)
            {
                LinSeqCount++;

                if(LinSeqCount == VictSeq)
                    return 1;
                
            }
            else
                LinSeqCount = 0; // A Contagem da sequencia volta a 1 porque a peça asseguir não é igual
        }
    }


    //VERIFICAÇÃO DE VITÓRIAS DAS DIAGONAIS

    //DIREITA PARA ESQUERDA
    for(idxDigLin = 0, idxDigCol = VictSeq; idxDigLin < VictSeq; idxDigLin++, idxDigCol--) //Linhas
    {
        aux = (idxDigCol * 2) - 1; //Colunas

        if(Brd[idxDigLin][aux] == CurrentPlayerChar)
        {
            LinSeqCount++;
    
            if(LinSeqCount == VictSeq)
                return 1;
        }
        else
            LinSeqCount = 0; // A Contagem da sequencia volta a 1 porque a peça asseguir não é igual
    }

    //ESQUERDA PARA DIREITA
    for(idxDigLin = 0; idxDigLin < VictSeq; idxDigLin++) //Linhas
    {
        aux = ((idxDigLin + 1) * 2) - 1; //Colunas

        if(Brd[idxDigLin][aux] == CurrentPlayerChar)
        {
            LinSeqCount++;
    
            if(LinSeqCount == VictSeq)
                return 1;
        }
        else
            LinSeqCount = 0; // A Contagem da sequencia volta a 1 porque a peça asseguir não é igual
    }


    return 0;
}

int main()
{
    setlocale(LC_ALL, "pt_PT.UTF-8");
    abrirBaseDeDados();
    criarTabelaClientes();

    int opcao;
    char board[1000][1000];

    int WinSeq;
    int Lines;
    int Columns;

    Jogador player[2];
    int idxPl;

    // Variaveis de controlo do numero de vitorias do jogo a decorrer
    int CntVictPlayer1 = 0;
    int CntVictPlayer2 = 0;

    bool PlayerRole = true; //Identificar que jogador está a jogar no momento


    do {
        printf("**********Bem-vindo!**********");
        printf("\nMenu:\n1 - Adicionar Jogador\n2 - Listar Jogadores\n3 - Remover Jogador\n4 - Comecar Jogo\n0 - Sair\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarJogador();
                break;
            case 2:
                listarJogador();
                break;
            case 3:
                removerJogador();
                break;
            case 4:
                sqlite3_close(db);
                printf("A começar o jogo...\n");
                break;
            case 0:
                printf("Jogo abortado com sucesso");
                return 0;
            default:
                printf("Por favor, tente novamente. Opção invalida!\n");
        }
    } while (opcao != 4 && opcao != 0);


    printf("Indique o nome do primeiro jogador: ");
    scanf("%s", &player[0].nome);
    printf("Indique o nome do segundo jogador: ");
    scanf("%s", &player[1].nome);

    //Verificar pela base de dados se o jogador já existe
    //--------------------------------------------------

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
        int opcao_jogo;
        printf("\nJogo:\n1 - Colocar Peça\n2 - Detalhes do Jogo\n3 - Desistir\n");
        scanf("%d", &opcao_jogo);

        switch(opcao_jogo){
            case 1:
                // Colocação das peças
                int PcQntty;
                int StrtIdx;
                int Drctn; // 1 - Direita   0 - Esquerda
                char piece;

                // Verificação de qual jogador vai jogar
                // Jogador 1 - TRUE     Jogador 2 - FALSE
                if (PlayerRole)
                {
                    printf("\n\nJOGADOR %s:\n", player[0].nome);
                    piece = 'X';
                }
                else
                {
                    printf("\n\nJOGADOR %s:\n", player[1].nome);
                    piece = 'O';
                }

                printf("\nQuantidade de pecas a jogar: ");
                scanf("%d", &PcQntty);
                printf("Posicao Inicial: ");
                scanf("%d", &StrtIdx);
                printf("Sentido da jogada: ");
                scanf("%d", &Drctn);

                InsertPiece(piece, PcQntty, StrtIdx, Drctn, board, Lines);
                ShowBoard(Lines, Columns, board);

                // Função de verificação de vitória
                if (VictoryVerification(Lines, Columns / 2, board, piece, WinSeq))
                {
                    if (PlayerRole)
                    {
                        printf("\n\nVITORIA DO JOGADOR %s:\n", player[0].nome);
                        CntVictPlayer1++;
                        //Dar update no campo das vitorias do jogador na BD
                    }
                    else
                    {
                        printf("\n\nVITORIA DO JOGADOR %s:\n", player[1].nome);
                        CntVictPlayer2++;
                        //Dar update no campo das vitorias do jogador na BD
                    }

                    return 0;
                }

                if (PlayerRole) // Manipular a variavel para que o jogador não mude, se vencer
                    PlayerRole = false;
                else
                    PlayerRole = true;


            case 2:
                    printf("\nDETALHES DO JOGO: \n");
                    printf("\nJOGADOR %s: %d vitorias \n", player[0].nome, CntVictPlayer1);
                    printf("JOGADOR %s: %d vitorias \n", player[1].nome, CntVictPlayer2);
                return 0;
            case 3:
                return 0;
        }
    } while (1);

    return 0;
}