#include <stdio.h>
#include <stdbool.h>

struct Jogador
{
    char nome[50];
    int JogosRealizados;
    int Vitorias;
};

#define Medida 20

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
    int idxCol, idxLin, idxDigLin = 0, idxDigCol = 0;
    int AuxCol = 0, AuxLin = 0, auxCol2 ,AuxLin2, auxCol3 = 0, aux;
    int ColSeqCount = 0, LinSeqCount = 0;

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
    
   //ESQUERDA PARA DIREITA
    /*for (AuxLin = 1; AuxLin <= Lin; AuxLin++) // Linhas
    {
        while (auxCol3 < Col)
        {
            for (auxCol2 = 0, AuxLin2 = AuxLin; auxCol2 < VictSeq; auxCol2++, AuxLin2--)
            {
                AuxCol = ((auxCol2 + 1) * 2) - 1; // Colunas

                if (Brd[AuxLin2][AuxCol] == CurrentPlayerChar)
                {
                    LinSeqCount++;

                    if (LinSeqCount == VictSeq)
                        return 1;
                }
                else
                    LinSeqCount = 0; // A Contagem da sequencia volta a 1 porque a peça asseguir não é igual
            }

            auxCol3 = auxCol2;
        }
    }*/

    return 0;
}

int main()
{
    char board[Medida][Medida];

    int WinSeq;
    int Lines;
    int Columns;

    struct Jogador player[2];
    int idxPl;

    bool PlayerRole = true; //Identificar que jogador está a jogar no momento

    printf("\nIndique o nome do primeiro jogador: ");
    scanf("%s", &player[0].nome);
    printf("Indique o nome do segundo jogador: ");
    scanf("%s", &player[1].nome);
    printf("Indique a sequencia vencedora: ");
    scanf("%d", &WinSeq);

    //Verificar pela base de dados se o jogador já existe
    //--------------------------------------------------

    printf("\nIndique o numero de colunas: ");
    scanf("%d", &Columns);
    printf("Indique o numero de linhas: ");
    scanf("%d", &Lines);
    
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

        //Verificação de qual jogador vai jogar         
        //Jogador 1 - TRUE     Jogador 2 - FALSE
        if(PlayerRole)
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
                printf("\n\nVitoria do JOGADOR %s!!\n", player[0].nome);
            }
            else
            {
                printf("\n\nVitoria do JOGADOR %s!!\n", player[1].nome);
            }

            return 0;
        }

        if(PlayerRole) //Manipular a variavel para que o jogador não mude, se vencer
            PlayerRole = false;
        else
            PlayerRole = true;

    } while (1);

    return 0;
}