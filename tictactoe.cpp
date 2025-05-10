#include <cstdlib>
#include <ctime>

#include <iostream>
#include <string>
#include <vector>

#define E_WIN 1
#define E_CONTINUE 0
#define E_TIE -1
// Error codes that require the move to be done again should be less than E_TIE.
// Check REF_less_than_tie
#define E_OCCUPIED_TILE -2
#define E_OUT_OF_BOUNDS -3

class Move
{
    public:
        int i, j;

        Move() {i = 0; j = 0;};
        Move(int o_i, int o_j) {i = o_i; j = o_j;};
};

class Board
{
    private:
        char boardPos[3][3];
        const char pieces[2] = {'X', 'O'};
        std::vector<Move> moves;

    public:
        int player;
        void print();
        int makeMove(const Move &);
        int unmakeMove(const Move &);
        std::vector<Move> getMoves();
        Board();
        ~Board() {};
        Board(const Board &);
        Board & operator=(const Board &);
        Board(Board &&) noexcept;
        Board & operator=(Board &&) noexcept;
};

class Engine
{
    private:
        int difficulty;
        int evaluate(Board &, Move &);

    public:
        Move chooseMove(Board);
        Engine();
};

Board::Board()
{
    player = 0;
    for(size_t i = 0; i < 3; i++)
    {
        for(size_t j = 0; j < 3; j++)
        {
            boardPos[i][j] = ' ';
        }
    }
}

Board & Board::operator=(const Board &other)
{
    player = other.player;
    for(size_t i = 0; i < 3; i++)
    {
        for(size_t j = 0; j < 3; j++)
        {
            boardPos[i][j] = other.boardPos[i][j];
        }
    }

    return *this;
}

Board::Board(const Board &other)
{
    *this = other;
}

Board & Board::operator=(Board &&other) noexcept
{
    *this = other;

    return *this;
}

Board::Board(Board &&other) noexcept
{
    *this = other;
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

int Board::makeMove(const Move &move)
{
    int row = move.i;
    int column = move.j;

    int counter = 0;

    if(0 > row || 2 < row || 0 > column || 2 < column)
    {
        return E_OUT_OF_BOUNDS;
    }

    if(' ' != boardPos[row][column])
    {
        return E_OCCUPIED_TILE;
    }

    boardPos[row][column] = pieces[player];
    player = !player;

    /*
     * Checking win conditions
     */

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

int Board::unmakeMove(const Move &move)
{
    int row = move.i;
    int column = move.j;

    if(0 > row || 2 < row || 0 > column || 2 < column)
    {
        return E_OUT_OF_BOUNDS;
    }

    boardPos[row][column] = ' ';
    player = !player;

    return E_CONTINUE;
}

std::vector<Move> Board::getMoves()
{
    moves.clear();

    for(size_t i = 0; i < 3; i++)
    {
        for(size_t j = 0; j < 3; j++)
        {
            if(' ' == boardPos[i][j])
            {
                moves.emplace_back(Move(i, j));
            }
        }
    }

    return moves;
}

Engine::Engine()
{
    srand(time(NULL));
    difficulty = rand() % 2;
}

Move Engine::chooseMove(Board board)
{
    std::vector<Move> moves = board.getMoves();
    Move chosenMove = moves[rand() % moves.size()];
    int score = evaluate(board, chosenMove);

    if(difficulty)
    {
        for(size_t i = 0; i < moves.size(); i++)
        {
            int curr_score = evaluate(board, moves[i]);
            if(curr_score > score)
            {
                score = curr_score;
                chosenMove = moves[i];
            }
        }
    }

    return chosenMove;
}

int Engine::evaluate(Board &board, Move &move)
{
    int board_score = board.makeMove(move);
    if(-1 == board_score)
    {
        board_score = 0;
    }
    else if(0 == board_score)
    {
        std::vector<Move> moves = board.getMoves();
        int score = evaluate(board, moves[rand() % moves.size()]);
        for(size_t i = 0; i < moves.size(); i++)
        {
            int curr_score = evaluate(board, moves[i]);
            if(curr_score > score)
            {
                score = curr_score;
            }
        }
        board_score = - score;
    }

    board.unmakeMove(move);

    return board_score;
}

int main(int argc, char** argv)
{
    int state = E_CONTINUE;
    Board board;
    Engine engine;
    char player;
    char computer;

    player = rand() % 2;
    computer = !player;

    board.print();

    while(true)
    {
        std::string currentMove;
        int row = -1;
        int column = -1;

        if(board.player == computer)
        {
            Move computer_move;
            do
            {
                computer_move = engine.chooseMove(board);
                state = board.makeMove(computer_move);
            } while(E_TIE > state); // REF_less_than_tie: This is why.
            // The computer should not choose an invalid move, but just in case.
            std::cout << "Computer's move: " << std::endl;
            std::cout << (char) ('a' + computer_move.j) << 1 + computer_move.i;
            std::cout << std::endl;
        }
        else if(board.player == player)
        {
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

            state = board.makeMove(Move(row, column));

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
        }

        board.print();

        if(E_WIN == state)
        {
            if(board.player == computer)
            {
                std::cout << "You won!" << std::endl;
            }
            else if(board.player == player)
            {
                std::cout << "You lost..." << std::endl;
            }
            break;
        }
        else if(E_TIE == state)
        {
            std::cout << "Tie." << std::endl;
            break;
        }
    }

    return 0;
}
