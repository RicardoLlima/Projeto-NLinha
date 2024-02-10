#include <stdio.h>


void BuildBoard(int Lin, int Col, char Brd[1000][1000])
{
    int idxCol, idxLin, aux;

    for (idxCol = 0; idxCol < Col; idxCol++)
    {
        for (idxLin = 0; idxLin <= Lin; idxLin++)
        {
            aux = idxLin % 2;

            if(aux == 0)
                Brd[idxCol][idxLin] = '|';
            else
                Brd[idxCol][idxLin] = '_';
        }
    }
}

void ShowBoard(int Lin, int Col, char Brd[1000][1000])
{
    int idxCol, idxLin;

    for (idxLin = 0; idxLin < Col; idxLin++) // Linhas
    {
        for (idxCol = 0; idxCol <= Lin; idxCol++) //Colunas
        {
            printf("%c", Brd[idxLin][idxCol]);
        }
        printf("\n");
    }
}

void InsertPiece(int PieceQntty, int StartIndex, int Direction, char Brd[1000][1000], int LinNum) //LinNum é o numero de linhas do tabuleiro
{
    //StartIndex é o numero da coluna
    //Verificar as linhas da coluna StartIndex de baixo para cima,
    //para perceber quais as linhas nessa coluna já estão ou não ocupadas

    int idxLin;

    for(idxLin=LinNum; idxLin >= 0; )
    {

    }
} 


int main()
{
    char board[1000][1000];

    int Lines;
    int Columns;

    printf("Indique o numero de linhas: ");
    scanf("%d", &Lines);
    printf("Indique o numero de colunas: ");
    scanf("%d", &Columns);

    Lines = Lines * 2;
    Columns = Columns * 2;



    //Montagem e demonstração do tabuleiro
    BuildBoard(Lines,Columns, board);
    ShowBoard(Lines,Columns, board);


    //Colocação das peças
    int PcQntty;
    int StrtIdx;
    int Drctn; // 1 - Direita   0 - Esquerda

    printf("Quantidade de peças a jogar: ");
    scanf("%d", &PcQntty);
    printf("Posição Inicial: ");
    scanf("%d", &StrtIdx);
    printf("Sentido da jogada: ");
    scanf("%d", &Drctn);

    InsertPiece(PcQntty,StrtIdx,Drctn,board,Lines);

    return 0;
}