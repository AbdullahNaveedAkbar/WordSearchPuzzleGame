#include <iostream>
#include <fstream>
#include<string>

using namespace std;

const int BOARD_SIZE = 15;
const int MAX_LIVES = 3;
int score = 0;

void displayMenu();
void newGame();
void displayBoard(char board[][BOARD_SIZE]);
void pauseGame(char board[][BOARD_SIZE]);
bool resumeGame(char board[][BOARD_SIZE]);
bool verifyWord(const string& word);
bool findWord(char board[][BOARD_SIZE], const string& word, int& score, int& lives,const string& playername);
void levelSelection();
void displayHighScore();
void updateHighScore(const string& playername,int score);
void Playagain(const string& playername, int lives);


int main() {
    char choice;

    cout << "----------------- WORD PUZZLE GAME -------------------" << endl;
    cout << endl;
    do {
        displayMenu();
        cout << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 'n':
            newGame();
            break;
        case 'r':
            cout << " Please first START the game for resume  " << endl;
            newGame();
     
            break;
        case 'l':
           levelSelection();
            break;
        case 'h':
            displayHighScore();
            break;
        case 'e':
            cout << "Exiting the game. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 'e');

    return 0;
}

void displayMenu() {
    cout << "Menu:" << endl;
    cout << "Press n for new game" << endl;
    cout << "Press r for resume game" << endl;
    cout << "Press l for level selection" << endl;
    cout << "Press h for high score" << endl;
    cout << "Press e for exit" << endl;
    cout << endl;
}


string toUpperCase(const string& str) {
    string upperStr = str;
    for (char& c : upperStr) {
        c = toupper(c);
    }
    return upperStr;
}

void newGame( ) {
    static bool firsttime = true;
    string playerName;

    char board[BOARD_SIZE][BOARD_SIZE];
    int  lives = MAX_LIVES;
    cout << endl;

    if (firsttime)
    {
        cout << endl;

        cout << "Enter your name: ";
        getline(cin, playerName);
        firsttime = false;
    }
    
    ifstream boardFile("board.txt");
    if (!boardFile) {
        cout << "Error opening board file." << endl;
        
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            boardFile >> board[i][j];
        }
    }

    boardFile.close();

 
    displayBoard(board);

    
    string word;
    cout << "Enter a word or press 'p' to pause: ";
    cin >> word;

    if (word == "p" || word == "P") {
        pauseGame(board);
    }
    else {
        if (verifyWord(word)) {
            if (findWord(board, toUpperCase(word), score, lives,playerName)) {
                cout << "Congratulations! You found all the words. Score: " << score << endl;
                updateHighScore(playerName,score);
                Playagain(playerName,lives);
            }
            else {
                cout << "Game over! Your score: " << score << endl;
                Playagain(playerName,lives);
            }
        }
        else {
            cout << "Invalid word. Please try again." << endl;
            Playagain(playerName,lives);
        }
    }

    firsttime = true;
}



void displayBoard(char board[][BOARD_SIZE]) {
    cout << endl;
    cout << "Current Board:" << endl;
    cout << " ----------------------------";
    cout << endl;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
          cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout <<" -------------------------- - ";
    cout << endl;
    cout << endl;
}

void pauseGame(char board[][BOARD_SIZE]) {
    ofstream pauseFile("Pausedgame.txt");
    if (!pauseFile) {
        cout << "Error creating pause file." << endl;
       
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            pauseFile << board[i][j] << " ";
        }
        pauseFile << endl;
    }

    pauseFile.close();

    char choice;
    cout << "Game paused." << endl;
    do {
        cout << endl;
        cout << "Press e to exit or r to resume: ";
        cin >> choice;

        if (choice == 'r') {
            if (resumeGame(board)) {
                cout << "Game resumed." << endl;
                break;
            }
            else {
                cout << "Error resuming the game. Please try again." << endl;
            }
        }
        else if (choice == 'e') {
            cout << "Exiting the game. Goodbye!" << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }

    } while (true);
}

bool resumeGame(char board[][BOARD_SIZE]) {
    ifstream resumeFile("Pausedgame.txt");
    if (!resumeFile) {
        cerr << "Error opening resume file." << endl;
        return false;
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            resumeFile >> board[i][j];
        }
    }

    resumeFile.close();
    return true;
}

bool verifyWord(const string& word) {
    ifstream dictionaryFile("dictionary.txt");
    if (!dictionaryFile) {
        cout << "Error opening dictionary file." << endl;
        return false;
    }

    string dictWord;
    while (dictionaryFile >> dictWord) {
        if (dictWord == word) {
            cout << endl;
            cout << "word found in dictionary "<<endl;
            dictionaryFile.close();
            return true;
        }
    }

    dictionaryFile.close();
    return false;
}
  




bool findWord(char board[][BOARD_SIZE],const string& word, int& score, int& lives, const string& playername) {
    {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                
                if (board[i][j] == word[0]) {
                    int len = word.length();
                    bool found = true;

                    
                    for (int k = 1; k < len; ++k) {
                        if (i + k >= BOARD_SIZE || board[i + k][j] != word[k]) {
                            found = false;
                            break;
                        }
                    }

                  
                    if (found) {
                        score    += 10;
                        updateHighScore(playername, score);
                        return true;
                    }
                    else
                    {
                        lives = lives-1;
                    }

                    found = true; 

                    
                    for (int k = 1; k < len; ++k) {
                        if (j + k >= BOARD_SIZE || board[i][j + k] != word[k]) {
                            found = false;
                            break;
                        }
                    }

                    if (found) {
                        score += 10;
                        updateHighScore(playername, score);

                        return true;
                    }
                    else
                    {
                        lives = lives-1;
                    }
                }
            }
        }

        return false; 
    }
    
    return true; 
}

void levelSelection() {
    int choice;
    
    cout << "Select difficulty level ()." << endl;
    cout << "Press 1 for Easy" << endl;
    cout << "Press 2 for Medium" << endl;
    cout << "Press 3 for Hard" << endl;
    cin >> choice;
    cout << " Your level is selected Please play the game : ";
}

void displayHighScore( ) {
    
    string playerName;
     int playerscore;
    ifstream Score;
    Score.open("HighScore.txt");
    if (Score.is_open())
    {
        while (Score >> playerName >> playerscore)
        {
            cout << endl;
            cout << playerName << "- High Score: " << playerscore << endl;
        }
        Score.close();
    }
    else
    {
        cout << "Error opening highscore.txt" << endl;
    }
   
   
}

void updateHighScore(const string& playername,int score) {
    
    
    ifstream currentScores("HighScore.txt");
    ofstream updatedScores("tempHighScore.txt");

    if (currentScores.is_open() && updatedScores.is_open()) {
        string name;
        int currentScore;

        bool playerExists = false;

        
        while (currentScores >> name >> currentScore) {
            if (name == playername) {
                playerExists = true;
                updatedScores << playername << " " << score << endl;
            }
            else {
                updatedScores << name << " " << currentScore << endl;
            }
        }

        
        if (!playerExists) {
            updatedScores << playername << " " << score << endl;
        }

        currentScores.close();
        updatedScores.close();

     
        
        if (remove("HighScore.txt") != 0) {
            cout << "Error updating high score. Please check file permissions or disk space." << endl;
        }
        else {
            if (rename("tempHighScore.txt", "HighScore.txt") != 0) {
                cout << "Error updating high score. Please check file permissions or disk space." << endl;
            }
            else {
                cout << "High score updated" << endl;
            }
        }
    }
      
    
    else {
        cout << "Error updating highscore" << endl;
    }
}

void Playagain(const string& playername, int lives)
{
    char choice;
    
    if(lives > 0)
    {
        cout << endl;
    cout << "Would you like to play again (y/n) " << endl;
    cin >> choice;
    }
    else
    {
        cout << "You have run out of lives . Game over ! Your final score is :  " << score << endl;
        cout << "Would you like to play again (y/n) " << endl;
        cin >> choice;
    }



    if (choice == 'y' || choice == 'Y')
    {
        newGame();
    }
    else if (choice == 'n' || choice == 'N')
    {
        cout << "Existing the  Game. Goodbye! " << endl;
        
    }
    else
    {
        cout << "Inavlid choice. Exsiting the game. Goodbye! " << endl;
    }

}


