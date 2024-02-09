#include <stdio.h>

void BuildBoard(int Col, int Lin, char Brd[1000][1000])
{
    int idxCol, idxLin, aux;

    for (idxCol = 0; idxCol < Col; idxCol++)
    {
        for (idxLin = 0; idxLin <= Lin+5; idxLin++)
        {
            aux = idxLin % 2;

            if(aux == 0)
                Brd[idxCol][idxLin] = '|';
            else
                Brd[idxCol][idxLin] = ' ';
        }
    }
}

void ShowBoard(int Col, int Lin, char Brd[1000][1000])
{
    int idxCol, idxLin;

    for (idxLin = 0; idxLin < Col; idxLin++) // Linhas
    {
        for (idxCol = 0; idxCol <= Lin+5; idxCol++) //Colunas
        {
            printf("%c", Brd[idxLin][idxCol]);
        }
        printf("\n");
    }
}


int main()
{
    char board[1000][1000];

    BuildBoard(5,4, board);

    ShowBoard(5,4, board);

    return 0;
}