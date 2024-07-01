#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sqlite3.h>
typedef struct {
    int id;
    char nome[50];
    int JogosRealizados;
    int Vitorias;
} Jogador;

sqlite3 *db;
char *zErrMsg = 0;
int rc;

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

// Função para criar a tabela de jogadores, se necessário
void criarTabelaClientes() {
    const char *sql = "CREATE TABLE IF NOT EXISTS JOGADORES("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "NOME TEXT NOT NULL UNIQUE,"
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

// Função para adicionar um jogador à base de dados
int adicionarJogador() {
    Jogador c;
    printf("Introduza o nome do novo jogador: ");
    scanf(" %99s", c.nome);  // Certifique-se de que não ultrapasse o limite do buffer
    
    // Verifique se o nome já existe na base de dados
    char sql[200];
    sprintf(sql, "SELECT COUNT(*) FROM JOGADORES WHERE NOME = '%s';", c.nome);
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro SQL: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
        printf("Erro: O nome do jogador já existe.\n");
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);

    // Insere o novo jogador
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
    setlocale(LC_ALL, "pt_PT.UTF-8");
    abrirBaseDeDados();
    criarTabelaClientes();

    int opcao;

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

                printf("\nQuantidade de pecas a jogar: ");
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
                
                return 1;
            case 2:
                printf("Ainda não disponivel :(");
                return 0;
            case 3:
            return 0;
        }
    } while (1);

    return 0;
}