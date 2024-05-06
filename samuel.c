#include <stdio.h>

struct Jogador
{
    char nome[50];
    int JogosRealizados;
    int Vitorias;
};

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
    char board[1000][1000];

    int WinSeq;
    int Lines;
    int Columns;

    struct Jogador player[2];
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