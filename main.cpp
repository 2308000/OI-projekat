#include<bits/stdc++.h>
using namespace std;

// Move su koordinate optimalnog poteza
using Move = pair<int, int>;
const char MAXIMIZER = 'X';
const char MINIMIZER = 'O';
const char EMPTY = '_';
const char INF = 100;

// Vraca false ukoliko je tabla popunjena, u suprotnom vraca true
bool isMovePossible(vector<vector<char>> board)
{
    for(auto row : board)
        for(auto cell : row)
            if(cell == EMPTY) return true;

    return false;
}

// Vraca INF/-INF ukoliko je pobjednik X/O, u suprotnom vraca 0
int evaluateBoard(vector<vector<char>> board, int depth)
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
 
/* Radi na principu backtracking odnosno isprobava sve moguće kombinacije
   i na kraju bira optimalan potez za trenutnog igraca. Algoritam je
   ponderisan dubinom odnosno, ukoliko postoje dva ili vise poteza koji dovode do
   optimalnog rjesenja, birace se onaj potez koji dovodi do rjesenja u 
   najmanjem broju koraka. Nacin na koji je algoritam ponderisan je taj što 
   je broj koraka koji je ostao do pobjede(depth) oduziman od vrijednosti poteza
   koji ce biti napravljen u slucaju maximizera, odnosno dodavan, za slucaj minimizera. 
   Tako na primjer ukoliko postoje 2 poteza koja dovode do pobjede do racunara,
   u ovom slucaju maksimizera, recimo potez A i potez B. Neka A dovodi do pobjede u 2
   poteza a B u tri poteza. Tada ce A imati vrijednost INF-2 dok će B imati vrijednost
   INF-3 i maksimum ce uvijek biti potez 2. */
int minimax(vector<vector<char>> board, int depth, bool isMaximizersMove)
{
    int currentScore = evaluateBoard(board, depth);

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
 
// Vraca koordinate optimalnog poteza za X
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
    cout << "Vi ste O." << endl;
    cout << "---------" << endl << endl;
    printBoard(board);
    cout << "Igra pocinje!" << endl;
}

void playGame(vector<vector<char>> &board) {
    int x, y;
    int isOver = 0;
    bool firstMove = true;

    while(isMovePossible && !isOver) {
        Move bestMove = (firstMove) ? make_pair(0, 0) : findBestMove(board);
        firstMove = false;

        cout << "Racunar igra potez (" << bestMove.first << ", " << bestMove.second << ")" << endl;
        board[bestMove.first][bestMove.second] = 'X';
        printBoard(board);

        isOver = evaluateBoard(board, 0);
        if(isOver || !isMovePossible(board)) break;

        
        cout << "Vi ste na potezu!" << endl;
        do {
            cout << "Unesite koordinate za potez (0-indexed):" << endl;
            cout << "Red: ";
            cin >> x;
            cout << "Kolona: ";
            cin >> y;

            if((x < 0 || x >= board.size() || y < 0 || y >= board.size()) || board[x][y] != EMPTY) {
                cout << "Potez koji ste odigrali nije validan!" << endl;
                cout << "Ponovite potez!" << endl;
            }
        } while ((x < 0 || x >= board.size() || y < 0 || y >= board.size()) || board[x][y] != EMPTY);

        cout << "Odigrali ste potez (" << x << ", " << y << ")" << endl;
        board[x][y] = 'O';
        printBoard(board);

        isOver = evaluateBoard(board, 0);
        if(isOver || !isMovePossible(board)) break;
    }
}

void printResult(vector<vector<char>> board) {
    cout << "Igra je gotova!" << endl;
    
    if(evaluateBoard(board, 0) == INF) cout << "Racunar je pobijedio!" << endl;
    else if(evaluateBoard(board, 0) == -INF) cout << "Pobijedili ste!" << endl;
    else cout << "Igra je odigrana nerijeseno!" << endl;
}

int main()
{
    vector<vector<char>> board = {{ '_', '_', '_' },
                                  { '_', '_', '_' },
                                  { '_', '_', '_' }};
     
    startGame(board);
    playGame(board);
    printResult(board);

    return 0;
}