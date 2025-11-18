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

#define BOARD_LENGTH 3

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
        char boardPos[BOARD_LENGTH][BOARD_LENGTH];
        const char pieces[2] = {'X', 'O'};
        std::vector<Move> moves;

    public:
        int player;
        int length;
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
    length = BOARD_LENGTH;
    player = 0;
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < length; j++)
        {
            boardPos[i][j] = ' ';
        }
    }
}

Board & Board::operator=(const Board &other)
{
    length = other.length;
    player = other.player;
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < length; j++)
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
    for(int i = length - 1; i >= 0; i--)
    {
        std::cout.width(4 * (length + 1) - 1);
        std::cout.fill('-');
        std::cout << std::left << "  " << std::endl;
        std::cout << i + 1 << " |";
        for(int j = 0; j < length; j++)
        {
            std::cout << " " << boardPos[i][j] << " |";
        }
        std::cout << std::endl;
    }
    std::cout.width(4 * (length + 1) - 1);
    std::cout.fill('-');
    std::cout << std::left << "  " << std::endl;
    std::cout << "   ";
    for(int j = 0; j < length; j++)
    {
        std::cout << " " << (char) ('a' + j) << "  ";
    }
    std::cout << std::endl;
}

int Board::makeMove(const Move &move)
{
    int row = move.i;
    int column = move.j;

    int counter = 0;

    if(0 > row || length <= row || 0 > column || length <= column)
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
    for(int i = 0; i < length; i++)
    {
        if(boardPos[i][column] == boardPos[row][column])
        {
            counter++;
        }
    }
    if(length == counter)
    {
        return E_WIN;
    }

    counter = 0;

    // Checking row
    for(int j = 0; j < length; j++)
    {
        if(boardPos[row][j] == boardPos[row][column])
        {
            counter++;
        }
    }
    if(length == counter)
    {
        return E_WIN;
    }

    // Checking diagonals
    if(row == column)
    {
        counter = 0;

        for(int i = 0; i < length; i++)
        {
            if(boardPos[i][i] == boardPos[row][column])
            {
                counter++;
            }
        }
        if(length == counter)
        {
            return E_WIN;
        }

    }
    else if(row + column == length - 1)
    {
        counter = 0;

        for(int i = 0; i < length; i++)
        {
            if(boardPos[i][length - 1 - i] == boardPos[row][column])
            {
                counter++;
            }
        }
        if(length == counter)
        {
            return E_WIN;
        }
    }

    // Checking for tie
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < length; j++)
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

    if(0 > row || length - 1 < row || 0 > column || length - 1 < column)
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

    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < length; j++)
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
    difficulty = 0;
    /**
     * 4x4 boards have O(10^13) different positions.
     */
    if(4 > BOARD_LENGTH)
    {
        difficulty = rand() % 2;
    }

    if(difficulty)
    {
        std::cout << "Difficult" << std::endl;
    }
}

Move Engine::chooseMove(Board board)
{
    std::vector<Move> moves = board.getMoves();
    Move chosenMove = moves[rand() % moves.size()];

    if(difficulty)
    {
        int score = evaluate(board, chosenMove);

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
            std::cout << "Computer's move: " << std::endl;
            do
            {
                computer_move = engine.chooseMove(board);
                state = board.makeMove(computer_move);
            } while(E_TIE > state); // REF_less_than_tie: This is why.
            // The computer should not choose an invalid move, but just in case.
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
                if('a' <= currentMove[i] &&
                    'a' + board.length > currentMove[i])
                {
                    column = currentMove[i] - 'a';
                }
                else if('1' <= currentMove[i] &&
                    '1' + board.length > currentMove[i])
                {
                    row = currentMove[i] - '1';
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
