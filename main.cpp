#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
//#include <fstream>

using namespace std;

#define BOARD_LEN 3
#define USER 'X'
#define BOT 'O'

//ofstream fout("board.txt");
//int count = 0;

class Board {
    char board[BOARD_LEN][BOARD_LEN];
    char minimax;
    char turn;
    vector<Board*> possible_moves;
    inline bool win();
    inline bool draw();
public:
    Board();
    Board(Board*,char,pair<short,short>);
    inline void Display(ostream&);
    Board* UserMove(short);
    Board* BotMove();
    ~Board();
};

inline bool Board::win() {
    // Check rows
    for (short nrow = 0; nrow < BOARD_LEN; nrow++) {
        bool win = true;
        for (short ncol = 0; ncol < BOARD_LEN; ncol++)
            if (board[nrow][ncol] != turn) {
                win = false;
                break;
            }
        if (win) return true;
    }

    // Check columns
    for (short ncol = 0; ncol < BOARD_LEN; ncol++) {
        bool win = true;
        for (short nrow = 0; nrow < BOARD_LEN; nrow++)
            if (board[nrow][ncol] != turn) {
                win = false;
                break;
            }
        if (win) return true;
    }

    // Check diagonals
    bool win = true;
    for (short i = 0; i < BOARD_LEN; i++)
        if (board[i][i] != turn) {
            win = false;
            break;
        }
    if (win) return true;

    for (short i = 0; i < BOARD_LEN; i++)
        if (board[BOARD_LEN - 1 - i][i] != turn) {
            return false;
        }
    return true;
}

inline bool Board::draw() {
    for (short nrow = 0; nrow < BOARD_LEN; nrow++)
        for (short ncol = 0; ncol < BOARD_LEN; ncol++)
            if (!board[nrow][ncol]) return false;
    return true;
}

Board::Board() {
    for (short nrow = 0; nrow < BOARD_LEN; nrow++)
        for (short ncol = 0; ncol < BOARD_LEN; ncol++)
            this->board[nrow][ncol] = 0;
    this->minimax = 0;
    this->turn = USER;
}

Board::Board(Board *current, char turn, pair<short,short> location) {
    for (short nrow = 0; nrow < BOARD_LEN; nrow++)
        for (short ncol = 0; ncol < BOARD_LEN; ncol++)
            this->board[nrow][ncol] = current->board[nrow][ncol];
    this->minimax = 0;
    this->turn = turn;
    this->board[location.first][location.second] = turn;
    //this->Display(fout);

    if (this->win()) {
        if (turn == BOT) this->minimax = 1;
        else this->minimax = -1;
        //fout << turn << " won\n";
        return;
    }
    else if (this->draw()) {
        this->minimax = 0;
        //fout << "Draw\n";
        return;
    }
    //fout << "There have been " << ++count << " games\n";
    //fout << "===================\n";

    for (short nrow = 0; nrow < BOARD_LEN; nrow++)
        for (short ncol = 0; ncol < BOARD_LEN; ncol++)
            if (! this->board[nrow][ncol]) {
                if (turn == BOT)
                    this->possible_moves.push_back(new Board(this,USER,make_pair(nrow,ncol)));
                else
                    this->possible_moves.push_back(new Board(this,BOT,make_pair(nrow,ncol)));
            }
    
    // Calculate the minimax value. Each player (user and bot) will pick the worst value for itself (1 and -1 respectively)
    if (turn == USER) {
        minimax = -1;
        for (auto move : possible_moves)
            minimax = max(minimax, move->minimax);
    }
    else {
        minimax = 1;
        for (auto move : possible_moves)
            minimax = min(minimax, move->minimax);
    }
}

inline void Board::Display(ostream& cout) {
    cout << " ";
    for (short i = 1; i < BOARD_LEN; i++)
        cout << "______";
    cout << "_____\n";
    for (short i = 0; i < BOARD_LEN; i++) {
        cout << "|";
        for (short j = 0; j < BOARD_LEN; j++) {
            if (board[i][j]) cout << setw(3) << board[i][j];
            else cout << setw(3) << i * BOARD_LEN + j + 1;
            cout << "  |";
        }
        cout << '\n';
        for (short j = 0; j < BOARD_LEN; j++)
            cout << "|_____";
        cout << "|\n";
    }
    if (this->win()) {
        if (turn == USER) cout << "You won!\n";
        else cout << "You lost!\n";
        system("pause");
        exit(0);
    }
    else if (this->draw()) {
        cout << "Draw!\n";
        system("pause");
        exit(0);
    }
}

Board* Board::UserMove(short location) {
    Board* ret_board = nullptr;
    short x = (location - 1) / BOARD_LEN, y = (location - 1) % BOARD_LEN;
    for (auto move : possible_moves)
        if (move->board[x][y]) ret_board = move;
        else delete move;
    return ret_board;
}

Board* Board::BotMove() { // Bot will take the move with the highest minimax value
    Board* ret_board = possible_moves[0];
    for (int i = 1; i < possible_moves.size(); i++)
        if (possible_moves[i]->minimax > ret_board->minimax) {
            delete ret_board;
            ret_board = possible_moves[i];
        }
        else delete possible_moves[i];
    return ret_board;
}

Board::~Board() {
    for (auto move : possible_moves)
        delete move;
}

int main() {
    Board *current = new Board();

    system("cls");
    current->Display(cout);
    cout << "Note: Type '0' if you want the bot to move first\n";
    cout << "What is your next move? ";
    short location; cin >> location;

    current = new Board(current, USER, make_pair((location - 1) / BOARD_LEN, (location - 1) % BOARD_LEN));
    current = current->BotMove();

    while (true) {
        system("cls");
        current->Display(cout);
        cout << "What is your next move? ";
        cin >> location;

        current = current->UserMove(location);
        system("cls");
        current->Display(cout);
        current = current->BotMove();
    }
    return -1;
}