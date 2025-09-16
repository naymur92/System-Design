#include <bits/stdc++.h>
using namespace std;

/**
 * @class IObserver
 * @brief Interface for observer pattern (not used in current implementation).
 */
class IObserver {
public:
    /**
     * @brief Update method to be implemented by observers.
     * @param msg The message to notify.
     */
    virtual void update(string msg) = 0;
    virtual ~IObserver() = default;
};

/**
 * @class Symbol
 * @brief Represents a player's symbol in Tic Tac Toe.
 */
class Symbol {
private:
    char mark;  ///< The character representing the symbol

public:
    /**
     * @brief Construct a Symbol with a character mark.
     * @param m The character representing the symbol.
     */
    explicit Symbol(char m) : mark(m) {}

    /**
     * @brief Get the character representing the symbol.
     * @return char The symbol character.
     */
    char getMark() const {
        return mark;
    }
};

/**
 * @class Board
 * @brief Represents the Tic Tac Toe board and manages game state.
 */
class Board {
private:
    vector<vector<Symbol*>> grid;  ///< 2D grid of symbol pointers
    int size;                      ///< Board size (n x n)
    Symbol* emptySymbol;           ///< Symbol representing an empty cell

public:
    /**
     * @brief Construct a Board of given size.
     * @param size The board size (n x n).
     */
    explicit Board(int size)
        : size(size), emptySymbol(new Symbol(' ')), grid(size, vector<Symbol*>(size, nullptr)) {
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j) grid[i][j] = emptySymbol;
    }

    /**
     * @brief Destructor to clean up resources.
     */
    ~Board() {
        delete emptySymbol;
    }

    /**
     * @brief Check if a cell is empty.
     * @param row Row index.
     * @param col Column index.
     * @return bool True if cell is empty, false otherwise.
     */
    bool isCellEmpty(int row, int col) const {
        if (row < 0 || row >= size || col < 0 || col >= size) return false;
        return grid[row][col] == emptySymbol;
    }

    /**
     * @brief Get the symbol at a cell.
     * @param row Row index.
     * @param col Column index.
     * @return Symbol* Pointer to the symbol at the cell, or nullptr if out of bounds.
     */
    Symbol* getCell(int row, int col) const {
        if (row < 0 || row >= size || col < 0 || col >= size) return nullptr;
        return grid[row][col];
    }

    /**
     * @brief Place a move on the board.
     * @param row Row index.
     * @param col Column index.
     * @param s Player's symbol.
     * @return bool True if move is valid and placed, false otherwise.
     */
    bool markCell(int row, int col, Symbol* s) {
        if (!isCellEmpty(row, col)) return false;
        grid[row][col] = s;
        return true;
    }

    /**
     * @brief Get the board size.
     * @return int The board size.
     */
    int getSize() const {
        return size;
    }

    /**
     * @brief Print the board layout.
     */
    void display() const {
        cout << "\n   ";
        for (int c = 0; c < size; c++) cout << c << (c + 1 < 10 ? "   " : "  ");
        cout << "\n";

        for (int r = 0; r < size; r++) {
            cout << r << (r < 10 ? "  " : " ");
            for (int c = 0; c < size; c++) {
                cout << grid[r][c]->getMark();
                if (c + 1 < size) cout << " | ";
            }
            cout << "\n";
            if (r + 1 < size) {
                cout << "  ";
                for (int c = 0; c < size; c++) {
                    cout << "---";
                    if (c + 1 < size) cout << "+";
                }
                cout << "\n";
            }
        }
        cout << "\n";
    }
};

/**
 * @class Player
 * @brief Represents a player in the game, holding their ID, name, symbol, and score.
 */
class Player {
private:
    int id;       ///< Player ID
    string name;  ///< Player name
    Symbol* s;    ///< Player symbol
    int score;    ///< Player score

public:
    /**
     * @brief Constructor to initialize a player with ID, name, and symbol.
     * @param playerId The player's ID.
     * @param n The player's name.
     * @param sym The player's symbol.
     */
    Player(int playerId, string n, Symbol* sym) : id(playerId), name(n), s(sym), score(0) {}

    /**
     * @brief Get the player's name.
     * @return string The player's name.
     */
    string getName() const {
        return name;
    }

    /**
     * @brief Get the player's symbol.
     * @return Symbol* Pointer to the player's symbol.
     */
    Symbol* getSymbol() const {
        return s;
    }

    /**
     * @brief Get the player's score.
     * @return int The player's score.
     */
    int getScore() const {
        return score;
    }

    /**
     * @brief Destructor to clean up the symbol object.
     */
    ~Player() {
        delete s;
    }
};

/**
 * @class Rule
 * @brief Interface for game rules, including win, draw, and move validation checks.
 */
class Rule {
public:
    /**
     * @brief Check for a winning condition for the symbol s.
     * @param s The symbol to check for a win.
     * @return bool True if win, false otherwise.
     */
    virtual bool checkWin(Symbol* s) = 0;

    /**
     * @brief Check for a draw condition.
     * @return bool True if draw, false otherwise.
     */
    virtual bool checkDraw() = 0;

    /**
     * @brief Validate if a move to the given row and column is possible.
     * @param row Row index.
     * @param col Column index.
     * @return bool True if move is valid, false otherwise.
     */
    virtual bool isValidMove(int row, int col) = 0;

    virtual ~Rule() = default;
};

/**
 * @class StandardRule
 * @brief Implements the standard rules for Tic Tac Toe.
 */
class StandardRule : public Rule {
private:
    Board* board;  ///< Pointer to the game board

public:
    /**
     * @brief Constructor to initialize the rule with a board.
     * @param b Pointer to the board.
     */
    explicit StandardRule(Board* b) : board(b) {}

    /**
     * @brief Check for a winning condition for the symbol s.
     * @param s The symbol to check for a win.
     * @return bool True if win, false otherwise.
     */
    bool checkWin(Symbol* s) override {
        int n = board->getSize();
        // Check rows and columns
        for (int i = 0; i < n; i++) {
            bool rowWin = true, colWin = true;
            for (int j = 0; j < n; j++) {
                if (board->getCell(i, j) != s) rowWin = false;
                if (board->getCell(j, i) != s) colWin = false;
            }
            if (rowWin || colWin) return true;
        }

        // Check diagonals
        bool diagWin = true, antiDiagWin = true;
        for (int i = 0; i < n; i++) {
            if (board->getCell(i, i) != s) diagWin = false;
            if (board->getCell(i, n - 1 - i) != s) antiDiagWin = false;
        }
        if (diagWin || antiDiagWin) return true;

        return false;
    }

    /**
     * @brief Check for a draw condition.
     * @return bool True if draw, false otherwise.
     */
    bool checkDraw() override {
        int n = board->getSize();
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                if (board->isCellEmpty(r, c)) return false;
            }
        }
        return true;
    }

    /**
     * @brief Validate if a move to the given row and column is possible.
     * @param row Row index.
     * @param col Column index.
     * @return bool True if move is valid, false otherwise.
     */
    bool isValidMove(int row, int col) override {
        return board->isCellEmpty(row, col);
    }
};

/**
 * @class TicTacToe
 * @brief Manages the game flow, player turns, and game state.
 */
class TicTacToe {
private:
    Board* board;                  ///< Pointer to the game board
    deque<Player*> players;        ///< Queue of players
    Rule* rule;                    ///< Pointer to the game rule
    vector<IObserver*> observers;  ///< List of observers
    bool gameOver;                 ///< Game over flag

public:
    /**
     * @brief Construct a TicTacToe game.
     * @param b Pointer to the board.
     * @param r Pointer to the rule.
     */
    TicTacToe(Board* b, Rule* r) : board(b), rule(r), gameOver(false) {}

    /**
     * @brief Add a player to the game.
     * @param player Pointer to the player to be added.
     */
    void addPlayer(Player* player) {
        players.push_back(player);
    }

    /**
     * @brief Add an observer to the game.
     * @param o Pointer to the observer to be added.
     */
    void addObserver(IObserver* o) {
        observers.push_back(o);
    }

    /**
     * @brief Notify all observers with a message.
     * @param msg The message to be sent to observers.
     */
    void notify(string msg) {
        for (auto o : observers) o->update(msg);
    }

    /**
     * @brief Start and manage the game play.
     */
    void play() {
        int currentPlayerIndex = 0;
        board->display();

        while (!gameOver) {
            Player* currentPlayer = players[currentPlayerIndex];
            int row, col;

            cout << currentPlayer->getName() << "'s turn (" << currentPlayer->getSymbol()->getMark()
                 << "). Enter row and column: ";
            cin >> row >> col;

            if (!rule->isValidMove(row, col)) {
                cout << "Invalid move! Try again.\n";
                continue;
            }

            board->markCell(row, col, currentPlayer->getSymbol());
            board->display();

            if (rule->checkWin(currentPlayer->getSymbol())) {
                cout << currentPlayer->getName() << " wins!\n";
                gameOver = true;
            } else if (rule->checkDraw()) {
                cout << "It's a draw!\n";
                gameOver = true;
            } else {
                currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
            }
        }
    }

    /**
     * @brief Destructor to free allocated resources.
     */
    ~TicTacToe() {
        delete board;
        delete rule;
        for (auto p : players) delete p;
    }
};

/**
 * @enum GameType
 * @brief Enum for different game types.
 */
enum GameType { STANDARD };

/**
 * @class GameFactory
 * @brief Factory for creating TicTacToe games.
 */
class GameFactory {
public:
    /**
     * @brief Create a TicTacToe game of the given type and size.
     * @param t The game type.
     * @param size The board size.
     * @return TicTacToe* Pointer to the created game.
     */
    static TicTacToe* createGame(GameType t, int size) {
        if (t == STANDARD) {
            Board* board = new Board(size);
            Rule* rule = new StandardRule(board);
            TicTacToe* game = new TicTacToe(board, rule);
            return game;
        }
        return nullptr;
    }
};

/**
 * @class ConsoleNotifier
 * @brief Observer for console notifications.
 */
class ConsoleNotifier : public IObserver {
public:
    /**
     * @brief Update method to print notifications to the console.
     * @param msg The message to notify.
     */
    void update(string msg) override {
        cout << "Notification: " << msg << endl;
    }
};

/**
 * @brief Main function to run the Tic Tac Toe game.
 */
int main() {
    int boardSize;
    cout << "Enter board size (e.g., 3 for 3x3): ";
    cin >> boardSize;

    // Use GameFactory to create the game
    TicTacToe* game = GameFactory::createGame(STANDARD, boardSize);

    IObserver* notifier = new ConsoleNotifier();
    game->addObserver(notifier);

    // Test notifier
    game->notify("This is a test notification!");

    // Create players
    Player* player1 = new Player(1, "Player 1", new Symbol('X'));
    Player* player2 = new Player(2, "Player 2", new Symbol('O'));

    game->addPlayer(player1);
    game->addPlayer(player2);

    game->play();

    delete game;
    delete notifier;

    return 0;
}