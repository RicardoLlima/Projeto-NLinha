#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sqlite3.h>
#include <stdbool.h>

// Falta a implementação das peças especiais, o default para pôr peça é 1. Temos de criar uma função para receber a quantidade de peças especiais e o quanto valem.
// O sentido da jogada também tem de ser E - Esquerda ou D - Direita
// Tamanho da grelha personalizavel (?)
// Detalhes do jogo - Tamanho da grelha e tipo e quantidade de peças especiais disponíveis para cada jogador.
// Quando se põe na posição 0 inserem duas peças

typedef struct {
    int id;
    char nome[50];
    int JogosRealizados;
    int Vitorias;
    int pecasEspeciais;
} Jogador;

typedef struct {
    int tamanho;
    int quantidade;
} PecaEspecial;

PecaEspecial pecaEspecial;

int jogadorAtualID;

sqlite3 *db;
char *zErrMsg = 0;
int rc;

int Lines_Pre;
int Columns_Pre;

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
        fprintf(stdout, "-------------------------\n");
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
    if (c.id == jogadorAtualID) {
        printf("Erro: Não é possível remover o jogador que está a jogar atualmente.\n");
        return -1;
    }  
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

// Função para carregar jogadores da base de dados
int verificarCarregarJogador(Jogador *jogador) {
    char sql[200];
    sqlite3_stmt *stmt;
    
    sprintf(sql, "SELECT ID, NOME, JOGOSJOGADOS, VITORIAS FROM JOGADORES WHERE NOME = '%s';", jogador->nome);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Carregar dados do jogador na estrutura
        jogador->id = sqlite3_column_int(stmt, 0);
        strcpy(jogador->nome, (const char *)sqlite3_column_text(stmt, 1));
        jogador->JogosRealizados = sqlite3_column_int(stmt, 2);
        jogador->Vitorias = sqlite3_column_int(stmt, 3);
        sqlite3_finalize(stmt);
        return 1; // Jogador encontrado
    } else {
        sqlite3_finalize(stmt);
        return 0; // Jogador não encontrado
    }
}

// Função para atualizar as vitórias do jogador na base de dados
void atualizarVitoriasJogador(const char *playerName) {
    char sql[256];
    snprintf(sql, sizeof(sql), "UPDATE JOGADORES SET VITORIAS = VITORIAS + 1 WHERE NOME = '%s';", playerName);

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro SQL ao atualizar vitórias: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printf("Vitórias do jogador %s atualizadas com sucesso.\n", playerName);
    }
}

// Função para atualizar os jogos realizados do jogador na base de dados
void atualizarJogosRealizados(const char *playerName) {
    char sql[256];
    snprintf(sql, sizeof(sql), "UPDATE JOGADORES SET JOGOSJOGADOS = JOGOSJOGADOS + 1 WHERE NOME = '%s';", playerName);

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro SQL ao atualizar jogos realizados: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printf("Jogos realizados do jogador %s atualizados com sucesso.\n", playerName);
    }
}

// Função para desistências
void desistirJogador(const char *player1, const char *player2) {
    char resposta;
    printf("%s quer desistir.\n", player1);
    printf("%s, também quer desistir? (s/n): ", player2);
    scanf(" %c", &resposta);

    // Ambos jogadores desistiram
    if (resposta == 's' || resposta == 'S') {
        printf("Ambos jogadores desistiram.\n");
        atualizarJogosRealizados(player1);
        atualizarJogosRealizados(player2);
    } else {
        // Apenas o jogador 1 desistiu
        printf("%s não desistiu.\n", player2);
        atualizarJogosRealizados(player1);
        atualizarJogosRealizados(player2);
        atualizarVitoriasJogador(player2);
    }

    printf("Dados atualizados com sucesso.\n");
}

// Função de comparação para ordenar jogadores por nome
int compararJogadores(const void *a, const void *b) {
    Jogador *jogadorA = (Jogador *)a;
    Jogador *jogadorB = (Jogador *)b;
    return strcmp(jogadorA->nome, jogadorB->nome);
}

//FUNÇÕES DE MANIPULAÇÃO DO TABULEIRO ---------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

void BuildBoard(int Lin, int Col, char Brd[Lines_Pre][Columns_Pre])
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

void ShowBoard(int Lin, int Col, char Brd[Lines_Pre][Columns_Pre])
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

void InsertPiece(char PieceType, int StartIndex, char Brd[Lines_Pre][Columns_Pre], int LinNum) {
    int idxLin;
    int auxStartIndex = (StartIndex * 2) - 1;

    for (idxLin = LinNum; idxLin >= 0; idxLin--) {
        if (Brd[idxLin][auxStartIndex] == '_') {
            Brd[idxLin][auxStartIndex] = PieceType;
            break;
        }
    }
}

void InsertSpecialPiece(char PieceType, int PieceQntty, int StartIndex, char Direction, char Brd[Lines_Pre][Columns_Pre], int LinNum) {
    int idxLin, idxColumn;
    int auxStartIndex = (StartIndex * 2) - 1;

    for (idxColumn = 0; idxColumn < PieceQntty; idxColumn++) {
        for (idxLin = LinNum; idxLin >= 0; idxLin--) {
            if (Brd[idxLin][auxStartIndex] == '_') {
                Brd[idxLin][auxStartIndex] = PieceType;
                break;
            }
        }

        if (Direction == 'D' || Direction == 'd') {
            auxStartIndex += 2;
        } else if (Direction == 'E' || Direction == 'e') {
            auxStartIndex -= 2;
        }
    }
}

int VictoryVerification(int Lin, int Col, char Brd[Lines_Pre][Columns_Pre], char CurrentPlayerChar, int VictSeq) //CurrentPlayerChar representa o objeto do jogador
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

    int Lines;
    int Columns;

    int opcao;
    char board[Lines][Columns];

    int WinSeq;

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
                removerJogador(-1);
                break;
            case 4:
                printf("A começar o jogo...\n");
                break;
            case 0:
                printf("Jogo abortado com sucesso");
                return 0;
            default:
                printf("Por favor, tente novamente. Opção invalida!\n");
        }
    } while (opcao != 4 && opcao != 0);

    // Verificar e carregar informações do primeiro jogador
    printf("Indique o nome do primeiro jogador: ");
    scanf("%s", player[0].nome);
    if (verificarCarregarJogador(&player[0])) {
        printf("Jogador 1 carregado: ID=%d, Nome=%s, JogosJogados=%d, Vitorias=%d\n", player[0].id, player[0].nome, player[0].JogosRealizados, player[0].Vitorias);
    } else {
        printf("Jogador 1 não encontrado.\nA encerrar o jogo...\n"); // Só joga se tiver previamente registado
        return 0;
    }

   // Verificar e carregar informações do segundo jogador
    printf("Indique o nome do segundo jogador: ");
    scanf("%s", player[1].nome);
    if (verificarCarregarJogador(&player[1])) {
        printf("Jogador 2 carregado: ID=%d, Nome=%s, JogosJogados=%d, Vitorias=%d\n", player[1].id, player[1].nome, player[1].JogosRealizados, player[1].Vitorias);
    } else {
        printf("Jogador 2 não encontrado\nA encerrar o jogo...\n");
        return 0;
    }

    sqlite3_close(db);

    printf("Indique a sequência vencedora: ");
    scanf("%d", &WinSeq);
    printf("Indique o número de linhas do tabuleiro: ");
    scanf("%d", &Lines);
    printf("Introduza o número de colunas do tabuleiro: ");
    scanf("%d", &Columns);
    printf("Defina o tamanho da peça especial (quantas casas ocupa): ");
    scanf("%d", &pecaEspecial.tamanho);
    printf("Quantas peças especiais cada jogador pode ter: ");
    scanf("%d", &pecaEspecial.quantidade);

    Columns = Columns * 2;

    // Montagem e demonstração do tabuleiro
    BuildBoard(Lines, Columns, board);
    ShowBoard(Lines, Columns, board);

    // Array para conter os IDs dos jogadores
    int jogadorAtualId[2];
    jogadorAtualId[0] = player[0].id;
    jogadorAtualId[1] = player[1].id;

    // Definir o número de peças especiais para cada jogador
    player[0].pecasEspeciais = pecaEspecial.quantidade;
    player[1].pecasEspeciais = pecaEspecial.quantidade;

    //Execução do jogo
    do
    {
        int opcao_jogo;
        printf("\nJogo:\n1 - Visualizar tabuleiro\n2 - Colocar Peça\n3 - Colocar Peça Especial\n4 - Detalhes do Jogo\n5 - Adicionar Jogador\n6 - Remover Jogador\n7 - Desistir\n");
        scanf("%d", &opcao_jogo);

        switch(opcao_jogo){
            case 1:
            ShowBoard(Lines, Columns, board);
            break;
            case 2:
                // Colocação das peças normais
                int StrtIdx;
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

                    printf("Posição Inicial: ");
                    scanf("%d", &StrtIdx);

            InsertPiece(piece, StrtIdx, board, Lines);
            ShowBoard(Lines, Columns, board);

                // Função de verificação de vitória
                if (VictoryVerification(Lines, Columns / 2, board, piece, WinSeq))
                {
                    if (PlayerRole)
                    {
                        printf("\n\nVitória do jogador: %s\n\n", player[0].nome);
                        CntVictPlayer1++;
                        abrirBaseDeDados();
                        atualizarVitoriasJogador(player[0].nome);
                        sqlite3_close(db);
                    }
                    else
                    {
                        printf("\n\nVitória do jogador: %s\n\n", player[1].nome);
                        CntVictPlayer2++;
                        abrirBaseDeDados();
                        atualizarVitoriasJogador(player[1].nome);
                        sqlite3_close(db);
                    }

                    abrirBaseDeDados();
                    atualizarJogosRealizados(player[0].nome);
                    atualizarJogosRealizados(player[1].nome);
                    sqlite3_close(db);

                    return 0;
                }

                PlayerRole = !PlayerRole;

                break;
            case 3:
             // Colocação das peças especiais 
            if ((PlayerRole && player[0].pecasEspeciais <= pecaEspecial.quantidade && player[0].pecasEspeciais > 0) ||
                (!PlayerRole && player[1].pecasEspeciais <= pecaEspecial.quantidade && player[1].pecasEspeciais > 0)) {
                int PcQntty = pecaEspecial.tamanho;
                char Drctn;
                char piece;

                // Verificação de qual jogador vai jogar
                if (PlayerRole) {
                    printf("\n\nJOGADOR %s:\n", player[0].nome);
                } else {
                    printf("\n\nJOGADOR %s:\n", player[1].nome);
                }

                printf("Posição Inicial: ");
                scanf("%d", &StrtIdx);
                printf("Sentido da jogada (E para Esquerda, D para Direita): ");
                scanf(" %c", &Drctn);

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

                InsertSpecialPiece(piece, PcQntty, StrtIdx, Drctn, board, Lines);
                ShowBoard(Lines, Columns, board);

                // Atualizar a contagem de peças especiais usadas
                if (PlayerRole) {
                    player[0].pecasEspeciais--;
                } else {
                    player[1].pecasEspeciais--;
                }

                // Verificação de vitória
                if (VictoryVerification(Lines, Columns / 2, board, piece, WinSeq)) {
                    if (PlayerRole) {
                        printf("\n\nVitória do jogador: %s\n\n", player[0].nome);
                        CntVictPlayer1++;
                        abrirBaseDeDados();
                        atualizarVitoriasJogador(player[0].nome);
                        sqlite3_close(db);
                    } else {
                        printf("\n\nVitória do jogador: %s\n\n", player[1].nome);
                        CntVictPlayer2++;
                        abrirBaseDeDados();
                        atualizarVitoriasJogador(player[1].nome);
                        sqlite3_close(db);
                    }

                    abrirBaseDeDados();
                    atualizarJogosRealizados(player[0].nome);
                    atualizarJogosRealizados(player[1].nome);
                    sqlite3_close(db);

                    return 0;
                }

                PlayerRole = !PlayerRole;
            } else {
                printf("Número máximo de peças especiais já utilizado.\n");
            }
            break;
            case 4:
                qsort(player, 2, sizeof(Jogador), compararJogadores);
                printf("\nDETALHES DO JOGO: \n");
                printf("\nJOGADOR %s:\n%d jogos realizados \n%d vitórias \n", player[0].nome, player[0].JogosRealizados, player[0].Vitorias);
                printf("JOGADOR %s:\n%d jogos realizados \n%d vitórias \n", player[1].nome, player[1].JogosRealizados, player[1].Vitorias);
                break;
            case 5:
                abrirBaseDeDados();
                adicionarJogador();
                sqlite3_close(db);
                break;
            case 6:
                  printf("Indique o ID do jogador a ser removido: ");
                int jogadorId;
                scanf("%d", &jogadorId);
                if (jogadorId == player[0].id || jogadorId == player[1].id) {
                    printf("Erro: Não é possível remover um jogador que está atualmente no jogo.\n");
                } else {
                    abrirBaseDeDados();
                    removerJogador(jogadorId);
                    sqlite3_close(db);
                }
                break;
            case 7:
                if (PlayerRole)
                {
                    abrirBaseDeDados();
                    desistirJogador(player[0].nome, player[1].nome);
                    sqlite3_close(db);
                }
                else
                {
                    abrirBaseDeDados();
                    desistirJogador(player[1].nome, player[0].nome);
                    sqlite3_close(db);
                }
                return 0;
        }
    } while (1);

    return 0;
}