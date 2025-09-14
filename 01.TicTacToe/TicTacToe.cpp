#include <bits/stdc++.h>
using namespace std;

/**
 * @class Player
 * @brief Represents a player in Tic Tac Toe.
 *
 * Each player has:
 *  - A name (string)
 *  - A symbol ('X' or 'O')
 *  - A numeric value (+1 for 'X', -1 for 'O') used for optimized win checking
 *
 * Responsibilities:
 *  - Store identity and symbol
 *  - Provide value for board scoring
 */
class Player {
public:
    string name;
    char symbol;
    int value;

    Player(string name, char symbol) : name(move(name)), symbol(symbol) {
        value = (symbol == 'X' ? 1 : -1);
    };
};

/**
 * @class Board
 * @brief Represents the Tic Tac Toe board and game state.
 *
 * Attributes:
 *  - n: board size
 *  - grid: n x n char matrix
 *  - rows, cols, diagonal, antiDiagonal: counters for O(1) win detection
 *  - movesCount: track number of moves for draw detection
 *
 * Responsibilities:
 *  - Place moves
 *  - Validate moves
 *  - Detect wins/draws in O(1)
 *  - Print the board
 */
class Board {
    int n;
    vector<int> rows, cols;
    int diagonal = 0, antiDiagonal = 0;
    vector<vector<char>> grid;
    int movesCount = 0;

public:
    Board(int size) : n(size), rows(size, 0), cols(size, 0), grid(size, vector<char>(size, ' ')) {};

    /**
     * @brief Place a move for a player on the board.
     *
     * @param r Row index (0-based)
     * @param c Column index (0-based)
     * @param p Reference to Player making the move
     * @return int
     *     -1 if invalid move
     *      0 if valid move, game continues
     *      1 if the move results in a win
     *      2 if the move results in a draw
     */
    int placeMove(int r, int c, const Player& p) {
        if (r < 0 || r >= n || c < 0 || c >= n) return -1;
        if (grid[r][c] != ' ') return -1;

        grid[r][c] = p.symbol;
        movesCount++;

        rows[r] += p.value;
        cols[c] += p.value;
        if (r == c) diagonal += p.value;
        if (r + c == n - 1) antiDiagonal += p.value;

        if (abs(rows[r]) == n || abs(cols[c]) == n || abs(diagonal) == n || abs(antiDiagonal) == n)
            return 1;                       // win
        if (movesCount == n * n) return 2;  // draw
        return 0;                           // continue
    }

    /**
     * @brief Print the board layout.
     *
     * @return void
     */
    void printBoard() const {
        cout << "\n   ";
        for (int c = 0; c < n; c++) cout << c << (c + 1 < 10 ? "   " : "  ");
        cout << "\n";

        for (int r = 0; r < n; r++) {
            cout << r << (r < 10 ? "  " : " ");
            for (int c = 0; c < n; c++) {
                cout << grid[r][c];
                if (c + 1 < n) cout << " | ";
            }
            cout << "\n";
            if (r + 1 < n) {
                cout << "  ";
                for (int c = 0; c < n; c++) {
                    cout << "---";
                    if (c + 1 < n) cout << "+";
                }
                cout << "\n";
            }
        }
        cout << "\n";
    }
};

/**
 * @class TicTacToe
 * @brief Handles the game loop between two players.
 *
 * Responsibilities:
 *  - Manage players and board
 *  - Alternate turns
 *  - Collect user input
 *  - Display results
 *
 * Notes:
 *  - Player X always starts
 *  - Uses Board::placeMove for O(1) win checking
 */
class TicTacToe {
    Board board;
    Player p1, p2;
    Player* current;

public:
    TicTacToe(int n, string name1, string name2) : board(n), p1(name1, 'X'), p2(name2, 'O') {
        current = &p1;
    };

    /**
     * @brief Switch current player.
     *
     * @return void
     */
    void switchTurn() {
        current = (current == &p1) ? &p2 : &p1;
    }

    /**
     * @brief Play the game.
     *
     * @return void
     */
    void play() {
        cout << "\n--- Tic Tac Toe (" << p1.name << " vs " << p2.name << ") ---\n";

        int r, c, res;
        while (true) {
            board.printBoard();

            cout << current->name << " (" << current->symbol << "), enter row and col: ";

            cin >> r >> c;

            res = board.placeMove(r, c, *current);
            if (res == -1) {
                cerr << "Invalid move. Try again.\n";
                continue;
            } else if (res == 1) {
                board.printBoard();
                cout << current->name << " (" << current->symbol << ") wins!\n";
                break;
            } else if (res == 2) {
                board.printBoard();
                cout << "It's a draw.\n";
                break;
            }

            switchTurn();
        }
    }
};

int main() {
    int n;
    cout << "Enter board size n (3 - 15): ";
    cin >> n;

    if (n < 3 || n > 15) {
        cerr << "Invalid board size! Please choose between 3 and 15.\n";
        return 1;
    }

    string name1, name2;
    cout << "Enter Player 1 name (X): ";
    cin >> name1;
    cout << "Enter Player 2 name (O): ";
    cin >> name2;

    TicTacToe game(n, name1, name2);
    game.play();

    return 0;
}