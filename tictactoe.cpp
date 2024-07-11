#include <cstdlib>
#include <ctime>

#include <iostream>
#include <string>

#define E_WIN 1
#define E_CONTINUE 0
#define E_TIE -1
// Error codes that require the move to be done again should be less than E_TIE.
// Check REF_less_than_tie
#define E_OCCUPIED_TILE -2
#define E_OUT_OF_BOUNDS -3

class Engine
{
    public:
        void chooseMove(int *, int *);
        Engine();
};

Engine::Engine()
{
    srand(time(NULL));
}

void Engine::chooseMove(int *i, int *j)
{
    *i = rand() % 3;
    *j = rand() % 3;
}

class Board
{
    public:
        void print();
        int makeMove(int, int, char);
        Board();
    private:
        char boardPos[3][3];

};

Board::Board()
{
    for(size_t i = 0; i < 3; i++)
    {
        for(size_t j = 0; j < 3; j++)
        {
            boardPos[i][j] = ' ';
        }
    }
}

void Board::print()
{

    std::cout << "3  " << boardPos[2][0] << " | " << boardPos[2][1] << " | " << boardPos[2][2] << " " << std::endl;
    std::cout << "  -----------" << std::endl;
    std::cout << "2  " << boardPos[1][0] << " | " << boardPos[1][1] << " | " << boardPos[1][2] << " " << std::endl;
    std::cout << "  -----------" << std::endl;
    std::cout << "1  " << boardPos[0][0] << " | " << boardPos[0][1] << " | " << boardPos[0][2] << " " << std::endl;
    std::cout << "   a   b   c " << std::endl;
}

int Board::makeMove(int row, int column, char c)
{
    int counter = 0;

    if(0 > row || 2 < row || 0 > column || 2 < column)
    {
        return E_OUT_OF_BOUNDS;
    }

    if(' ' != boardPos[row][column])
    {
        return E_OCCUPIED_TILE;
    }

    boardPos[row][column] = c;

    // Checking column
    for(size_t i = 0; i < 3; i++)
    {
        if(boardPos[i][column] == boardPos[row][column])
        {
            counter++;
        }
    }
    if(3 == counter)
    {
        return E_WIN;
    }

    counter = 0;

    // Checking row
    for(size_t j = 0; j < 3; j++)
    {
        if(boardPos[row][j] == boardPos[row][column])
        {
            counter++;
        }
    }
    if(3 == counter)
    {
        return E_WIN;
    }

    // Checking diagonals
    if((row + column) % 2 == 0 && boardPos[row][column] == boardPos[1][1])
    {
        counter = 0;

        for(size_t i = 0; i < 3; i+=2)
        {
            if(boardPos[i][i] == boardPos[1][1])
            {
                counter++;
            }
        }
        if(2 == counter)
        {
            return E_WIN;
        }

        counter = 0;

        for(size_t i = 0; i < 3; i+=2)
        {
            if(boardPos[i][2 - i] == boardPos[1][1])
            {
                counter++;
            }
        }
        if(2 == counter)
        {
            return E_WIN;
        }
    }

    // Checking for tie
    for(size_t i = 0; i < 3; i++)
    {
        for(size_t j = 0; j < 3; j++)
        {
            if(' ' == boardPos[i][j])
            {
                return E_CONTINUE;
            }
        }
    }

    return E_TIE;
}

int main(int argc, char** argv)
{
    int row, column;
    int state = E_CONTINUE;
    std::string currentMove;
    Board board;
    Engine engine;

    while(true)
    {
        row = -1;
        column = -1;

        board.print();

        std::cout << "Your move:" << std::endl;
        if(!(std::cin >> currentMove))
        {

            std::cerr << "Error: Unable to read move." << std::endl;
            return 1;
        }

        for(size_t i = 0; i < 2; i++)
        {
            switch(currentMove[i])
            {
                case 'a':
                    column = 0;
                    break;
                case 'b':
                    column = 1;
                    break;
                case 'c':
                    column = 2;
                    break;
                case '1':
                    row = 0;
                    break;
                case '2':
                    row = 1;
                    break;
                case '3':
                    row = 2;
                    break;
            }
        }

        state = board.makeMove(row, column, 'X');
        if(E_CONTINUE != state)
        {
            if(E_OCCUPIED_TILE == state)
            {
                std::cerr << "Invalid move. Tile taken." << std::endl;
                continue;
            }
            else if(E_OUT_OF_BOUNDS == state)
            {
                std::cerr << "Invalid move. Tile non existent." << std::endl;
                continue;
            }
            else if(E_WIN == state)
            {
                board.print();
                std::cout << "You won!" << std::endl;
                break;
            }
            else if(E_TIE == state)
            {
                board.print();
                std::cout << "Tie." << std::endl;
                break;
            }
        }

        do
        {
            engine.chooseMove(&row, &column);
            state = board.makeMove(row, column, 'O');
        } while(E_TIE > state); // REF_less_than_tie: This is why.
        if(E_WIN == state)
        {
            board.print();
            std::cout << "You lose..." << std::endl;
            break;
        }
        else if(E_TIE == state)
        {
            board.print();
            std::cout << "Tie. After compucter." << std::endl;
            break;
        }
    }

    return 0;
}
