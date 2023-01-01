#include<bits/stdc++.h>
using namespace std;

// Move su koordinate optimalnog poteza
using Move = pair<int, int>;
const char MAXIMIZER = 'O';
const char MINIMIZER = 'X';
const char EMPTY = '_';
const int INF = 100;

// Vraca false ukoliko je tabla popunjena, u suprotnom vraca true
bool isMovePossible(vector<vector<char>> board)
{
    for(auto row : board)
        for(auto cell : row)
            if(cell == EMPTY) return true;

    return false;
}

// Vraca INF/-INF ukoliko je pobjednik X/O, u suprotnom vraca 0
int evaluateBoard(vector<vector<char>> board)
{
    // 3 u redu
    for(int i = 0; i < board.size(); i++) {
        /*cout << "Proveravam da li ima 3 u redu" << endl;
        cout << "Uslov za empty je: " << board[i][0] << endl;*/
        if(board[i][0] != EMPTY && (board[i][0] == board[i][1]) && (board[i][1] == board[i][2])) {
            if(board[i][0] == MAXIMIZER) return INF;
            return -INF;
        }
    }
 
    // 3 u koloni
    for(int i = 0; i < board.size(); i++) {
        if(board[0][i] != EMPTY && (board[0][i] == board[1][i]) && (board[1][i] == board[2][i])) {
            if(board[0][i] == MAXIMIZER) return INF;
            return -INF;
        }
    }
 
    // Glavna dijagonala
    if (board[1][1] != EMPTY && (board[0][0] == board[1][1]) && (board[1][1] == board[2][2]))
    {
        if(board[1][1] == MAXIMIZER) return INF;
        return -INF;
    }
 
    // Sporedna dijagonala
    if (board[1][1] != EMPTY && (board[0][2] == board[1][1]) && (board[1][1] == board[2][0]))
    {
        if(board[1][1] == MAXIMIZER) return INF;
        return -INF;
    }
 
    // Trenutno nema pobjednika
    return 0;
}
 
/* Radi na principu backtrackinga odnosno isprobava sve moguće kombinacije
   i na kraju bira optimalan potez za trenutnog igraca. Algoritam je
   ponderisan dubinom odnosno, ukoliko postoje dva ili vise poteza koji dovode do
   optimalnog rjesenja, birace se onaj potez koji dovodi do rjesenja u 
   najmanjem broju koraka. Nacin na koji je algoritam ponderisan je taj što 
   je broj koraka koji je ostao do pobjede(depth) oduziman od vrijednosti poteza
   koji ce biti napravljen u slucaju maximizera, odnosno dodavan, za slucaj minimizera. 
   Tako na primjer ukoliko postoje 2 poteza koja dovode do pobjede do racunara,
   u ovom slucaju maksimizera, recimo potez A i potez B. Neka A dovodi do pobjede u 2
   poteza a B u tri poteza. Tada ce A imati vrijednost INF-2 dok će B imati vrijednost
   INF-3 i maksimum ce uvijek biti potez A. */
int minimax(vector<vector<char>> board, int depth, bool isMaximizersMove)
{
    int currentScore = evaluateBoard(board);

    if(currentScore == INF || currentScore == -INF) return currentScore;
    if (!isMovePossible(board)) return 0;
 
    int bestMoveValue = (isMaximizersMove) ? -INF : INF;

    for(auto &row : board) {
        for(auto &cell : row) {
            if(cell == EMPTY) {
                if(isMaximizersMove) {
                    cell = MAXIMIZER;
                    bestMoveValue = max(bestMoveValue, (minimax(board, depth + 1, !isMaximizersMove) - depth));
                    // backtracking
                    cell = EMPTY;
                } else {
                    cell = MINIMIZER;
                    bestMoveValue = min(bestMoveValue, (minimax(board, depth + 1, !isMaximizersMove) + depth));
                    // backtracking
                    cell = EMPTY;
                }
            }
        }
    }

    return bestMoveValue;
}
 
// Vraca koordinate optimalnog poteza za racunar
Move findBestMove(vector<vector<char>> &board)
{
    int bestMoveValue = -INF;
    Move bestMove = {-1, -1};
 
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board.size(); j++) {
            if(board[i][j] == EMPTY) {
                board[i][j] = MAXIMIZER;
                int currentMoveValue = minimax(board, 0, false);
                // bactracking
                board[i][j] = EMPTY;

                if(currentMoveValue > bestMoveValue) {
                    bestMoveValue = currentMoveValue;
                    bestMove.first = i;
                    bestMove.second = j;
                }
            }
        }
    }
 
    return bestMove;
}

/*
-----------------------------------------------
---------------UTILITY FUNKCIJE----------------
-----------------------------------------------
*/

void printBoard(vector<vector<char>> &board) {
    for(auto row : board) {
        for(auto cell : row)
            cout << cell << " ";
        cout << endl;
    }
}

void startGame(vector<vector<char>> &board) {
    cout << "Vi ste X." << endl;
    cout << "---------" << endl;
    printBoard(board);
    cout << endl << "Igra pocinje!" << endl;
    cout << "-------------" << endl << endl;
}

void playGameX(vector<vector<char>> &board) {
    int row, col;

    while(true) {
        cout << endl << "Vi ste na potezu!" << endl;
        do {
            cout << "Unesite koordinate za potez (0-indexed):" << endl;
            cout << "Red: ";
            cin >> row;
            cout << "Kolona: ";
            cin >> col;

            if(row < 0 || row >= board.size() || col < 0 || col >= board.size()) {
                cout << "Potez koji ste odigrali nije validan!" << endl;
                cout << "Tabla je velicine 3x3, dakle indeksi redova i kolona su [0, 1, 2]" << endl;
                cout << "Ponovite unos!" << endl;
            } else if(board[row][col] != EMPTY) {
                cout << "Potez koji ste odigrali nije validan!" << endl;
                cout << "Polje na koordinatama (" << row << ", " << col << ") je vec zauzeto!" << endl;
                cout << "Ponovite unos!" << endl;
            }
        } while ((row < 0 || row >= board.size() || col < 0 || col >= board.size()) || board[row][col] != EMPTY);

        cout << "Odigrali ste potez (" << row << ", " << col << ")" << endl;
        board[row][col] = MINIMIZER;
        printBoard(board);
        // evaluateBoard je ne-nula samo ukoliko je neko pobijedio
        if(evaluateBoard(board) || !isMovePossible(board)) break;

        cout << endl << "Racunar je na potezu!" << endl;
        Move bestMove = findBestMove(board);
        cout << "Racunar igra potez (" << bestMove.first << ", " << bestMove.second << ")" << endl;
        board[bestMove.first][bestMove.second] = MAXIMIZER;
        printBoard(board);
        // evaluateBoard je ne-nula samo ukoliko je neko pobijedio
        if(evaluateBoard(board) || !isMovePossible(board)) break;
    }
}

void playGameO(vector<vector<char>> &board) {
    int row, col;
    bool firstMove = true;

    while(true) {
        cout << endl << "Racunar je na potezu" << endl;
        Move bestMove = (firstMove) ? make_pair(0, 0) : findBestMove(board);
        firstMove = false;
        cout << "Racunar igra potez (" << bestMove.first << ", " << bestMove.second << ")" << endl;
        board[bestMove.first][bestMove.second] = MAXIMIZER;
        printBoard(board);
        // evaluateBoard je ne-nula samo ukoliko je neko pobijedio
        if(evaluateBoard(board) || !isMovePossible(board)) break;

        cout << endl << "Vi ste na potezu!" << endl;
        do {
            cout << "Unesite koordinate za potez (0-indexed):" << endl;
            cout << "Red: ";
            cin >> row;
            cout << "Kolona: ";
            cin >> col;

            if(row < 0 || row >= board.size() || col < 0 || col >= board.size()) {
                cout << "Potez koji ste odigrali nije validan!" << endl;
                cout << "Tabla je velicine 3x3, dakle indeksi redova i kolona su [0, 1, 2]" << endl;
                cout << "Ponovite unos!" << endl;
            } else if(board[row][col] != EMPTY) {
                cout << "Potez koji ste odigrali nije validan!" << endl;
                cout << "Polje na koordinatama (" << row << ", " << col << ") je vec zauzeto!" << endl;
                cout << "Ponovite unos!" << endl;
            }
        } while ((row < 0 || row >= board.size() || col < 0 || col >= board.size()) || board[row][col] != EMPTY);

        cout << "Odigrali ste potez (" << row << ", " << col << ")" << endl;
        board[row][col] = MINIMIZER;
        printBoard(board);
        // evaluateBoard je ne-nula samo ukoliko je neko pobijedio
        if(evaluateBoard(board) || !isMovePossible(board)) break;
    }
}

void printResult(vector<vector<char>> board) {
    cout << "Igra je gotova!" << endl;
    
    if(evaluateBoard(board) == INF) cout << "Racunar je pobijedio!" << endl;
    else if(evaluateBoard(board) == -INF) cout << "Pobijedili ste!" << endl;
    else cout << "Igra je odigrana nerijeseno!" << endl;
}

int main()
{
    vector<vector<char>> board = {{ '_', '_', '_' },
                                  { '_', '_', '_' },
                                  { '_', '_', '_' }};
    
    /* Ko igra prvi se bira nasumicno, ukoliko je pseudorandom generisani broj
       0 ili 1, ukoliko se generise 1, korisnik igra prvi, ukoliko je 0, racunar igra prvi. */
    srand(time(0));
    startGame(board);
    (rand() % 2) ? playGameX(board) : playGameO(board);
    printResult(board);

    return 0;
}