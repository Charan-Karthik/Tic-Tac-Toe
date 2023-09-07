#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

// Constants
const int ROWS = 3;
const int COLUMNS = 3;
const char EMPTY = ' ';
const char HUMAN = 'X';
const char COMPUTER = 'O';

// Function Prototypes
void ProgramGreeting();
int StartMenu();
void GameInstructions();
void DisplayBoard(const char board[ROWS][COLUMNS]);
bool CheckForWinner(const char board[ROWS][COLUMNS]);
bool CheckForTie(const char board[ROWS][COLUMNS]);
void HumanTurn(char board[ROWS][COLUMNS]);
void ComputerTurn(char board[ROWS][COLUMNS]);
int minimax(char board[ROWS][COLUMNS], bool maximizing);
void HardComputerTurn(char board[ROWS][COLUMNS]);
int SelectDifficulty();

int main() {
  // Seed the random number generator for true randomness
  srand(static_cast<unsigned>(time(nullptr)));

  ProgramGreeting();

  bool keepPlaying = true;
  while (keepPlaying) {
    int userMenuSelection = StartMenu();

    if (userMenuSelection == 3) {
      cout << "\nThank you for playing! Goodbye!\n" << endl;
      break; // exit the loop to end the game
    } else if (userMenuSelection == 1) {
      GameInstructions();
      continue; // start from the top again
    }

    int difficulty = SelectDifficulty();

    char board[ROWS][COLUMNS] = {
        {EMPTY, EMPTY, EMPTY}, {EMPTY, EMPTY, EMPTY}, {EMPTY, EMPTY, EMPTY}};

    DisplayBoard(board);

    bool isHumanMove = rand() % 2; // randomize first move
    cout << (isHumanMove ? "Human starts the game! \n"
                         : "Computer starts the game! \n")
         << endl;

    // Main Game Loop
    while (!CheckForWinner(board) && !CheckForTie(board)) {
      if (isHumanMove) {
        HumanTurn(board);
      } else {
        if (difficulty == 1) { // difficulty 1 is easy, difficulty 2 is hard
          ComputerTurn(board);
        } else {
          HardComputerTurn(board);
        }
      }

      DisplayBoard(board);
      isHumanMove = !isHumanMove;
    }

    cout << (CheckForWinner(board)
                 ? (isHumanMove ? "\nCOMPUTER WINS!\n" : "\nHUMAN WINS!\n")
                 : "\nIT'S A TIE!\n");

    char replayChoice;
    bool validInput;

    do {
      cout << "Do you want to play again? (Y/N): ";
      cin >> replayChoice;

      replayChoice = toupper(replayChoice);
      validInput = (replayChoice == 'Y' || replayChoice == 'N');

      if (!validInput) {
        cout << "\nInvalid choice. Please enter Y or N.\n" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
      }

    } while (!validInput);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    keepPlaying = (replayChoice == 'Y');
  }

  return 0;
}

void ProgramGreeting() {
  cout << "Welcome! Are you ready to play some Tic Tac Toe?\n";
  time_t t = time(nullptr);
  tm *timePtr = localtime(&t);
  cout << "Today's date is: " << put_time(timePtr, "%B %d, %Y") << endl;
}

int StartMenu() {
  int choice;

  while (true) {
    cout << "\nMENU\n"
         << "1 - Explain Game\n"
         << "2 - Play\n"
         << "3 - Quit\n"
         << "Enter your choice: ";

    cin >> choice;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "\nPlease enter 1, 2, or 3.\n" << endl;
    } else if (choice < 1 || choice > 3) {
      cout << "\nPlease enter 1, 2, or 3.\n" << endl;
    } else {
      break;
    }
  }
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  return choice;
}

void GameInstructions() {
  cout << "Tic Tac Toe is a game played between two players on a 3x3 board.\n"
       << "The goal is to get three of your symbols (X or O) in a row.\n";
}

void DisplayBoard(const char board[ROWS][COLUMNS]) {
  cout << endl;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      cout << board[i][j] << (j < 2 ? " | " : "");
    }
    cout << (i < 2 ? "\n---------\n" : "\n");
  }
  cout << endl;
}

bool CheckForWinner(const char board[ROWS][COLUMNS]) {
  for (int i = 0; i < 3; ++i) {
    // Check for horizontal win
    if (board[i][0] != EMPTY && board[i][0] == board[i][1] &&
        board[i][1] == board[i][2])
      return true;
    // Check for vertical win
    if (board[0][i] != EMPTY && board[0][i] == board[1][i] &&
        board[1][i] == board[2][i])
      return true;
  }
  // Check for diagonal win
  return (board[0][0] != EMPTY && board[0][0] == board[1][1] &&
          board[1][1] == board[2][2]) // top-left to bottom-right
         || (board[0][2] != EMPTY && board[0][2] == board[1][1] &&
             board[1][1] == board[2][0]); // top-right to bottom-left
}

bool CheckForTie(const char board[ROWS][COLUMNS]) {
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (board[i][j] == EMPTY) // check for an empty space
        return false;
    }
  }
  return true;
}

void HumanTurn(char board[ROWS][COLUMNS]) {
  int x, y;
  bool firstAttempt = true;

  do {
    if (!firstAttempt) {
      cout << "\nOops! Something went wrong. Either that square is already "
              "filled or there was an entry error."
           << "\nPlease remember that rows are numbered 1-3 from top to bottom "
              "and that columns are numbered 1-3 from left to right."
           << " Please try again.\n"
           << endl;
    }

    cout << "Enter the row number (top to bottom, 1-3): ";
    cin >> x;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      firstAttempt = false;
      continue;
    }

    x -= 1;

    cout << "Enter the column number (left to right, 1-3): ";
    cin >> y;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      firstAttempt = false;
      continue;
    }

    y -= 1;

    firstAttempt = false;
  } while (x < 0 || x > 2 || y < 0 || y > 2 || board[x][y] != EMPTY);

  board[x][y] = HUMAN;
}

void ComputerTurn(char board[ROWS][COLUMNS]) {
  int x, y;
  do {
    x = rand() / ((RAND_MAX + 1u) / 3); // random row
    y = rand() / ((RAND_MAX + 1u) / 3); // random column
  } while (board[x][y] != EMPTY);
  board[x][y] = COMPUTER;
}

// Minimax function to evaluate the best move
int minimax(char board[ROWS][COLUMNS], bool maximizing) {
  // Base cases
  if (CheckForWinner(board)) {
    return maximizing ? -10 : 10;
  }
  if (CheckForTie(board)) {
    return 0; // neutral value
  }

  int bestScore = maximizing ? -10 : 10;

  // Find all possible moves
  for (int i = 0; i < ROWS; ++i) {
    for (int j = 0; j < COLUMNS; ++j) {
      if (board[i][j] == EMPTY) {
        board[i][j] = maximizing ? COMPUTER : HUMAN; // temp marker placed
        // Recursively evaluate each possibility
        int score = minimax(board, !maximizing);
        board[i][j] = EMPTY; // temp marker removed
        bestScore = maximizing ? max(score, bestScore) : min(score, bestScore);
      }
    }
  }

  return bestScore;
}

void HardComputerTurn(char board[ROWS][COLUMNS]) {
  int bestScore = -10;
  int bestMoveX = -1;
  int bestMoveY = -1;

  // Find all possible moves
  for (int i = 0; i < ROWS; ++i) {
    for (int j = 0; j < COLUMNS; ++j) {
      if (board[i][j] == EMPTY) {
        board[i][j] = COMPUTER;            // temp Computer marker placed
        int score = minimax(board, false); // evaluate Computer move
        board[i][j] = EMPTY;               // temp Computer marker removed
        if (score > bestScore) {
          // update best score and best move
          bestScore = score;
          bestMoveX = i;
          bestMoveY = j;
        }
      }
    }
  }

  board[bestMoveX][bestMoveY] = COMPUTER;
}

int SelectDifficulty() {
  int choice;

  while (true) {
    cout << "\nSelect difficulty level:\n"
         << "1. Easy\n"
         << "2. Hard\n"
         << "Enter your choice: ";

    cin >> choice;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "\nPlease enter 1 or 2\n" << endl;
    } else if (choice != 1 && choice != 2) {
      cout << "\nPlease enter 1 or 2\n" << endl;
    } else {
      break;
    }
  }

  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  return choice;
}
