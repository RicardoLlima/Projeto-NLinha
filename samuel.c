#include <stdio.h>

void BuildBoard(int Col, int Lin, char* Brd[1000][1000])
{
    int idxCol, idxLin;

    for (idxCol = 0; idxCol < Col; idxCol++)
    {
        for (idxLin = 0; idxLin < Lin; idxLin++)
        {
            Brd[idxCol][idxLin] = "| _ |";
        }
    }
}

void ShowBoard(int Col, int Lin, char* Brd[1000][1000])
{
    int idxCol, idxLin;

    for (idxCol = 0; idxCol < Col; idxCol++)
    {
        for (idxLin = 0; idxLin < Lin; idxLin++)
        {
            printf("%s", Brd[idxCol][idxLin]);
        }
    }
}


int main()
{
    char* board[1000][1000];

    BuildBoard(5,5, board);

    ShowBoard(5,5, board);

    return 0;
}