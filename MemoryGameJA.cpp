#include <iostream>    
#include <string>      
#include <vector>     
#include <algorithm>   // For  shuffle
#include <random>      // For random number generation
#include <ctime>       // For seeding random number generator
#include <limits>      // For the same kind of input validation i always use (numeric_limits)

using namespace std;

class Card {
private:
    int value;        // Num between (1-8)
    bool isFaceUp;    // True if the card is face up, false if face down

public:
    // Default - Create a card with value 0 - face down
    Card() {
        value = 0;
        isFaceUp = false;
    }

    // Param Constructor

    Card(int v) {
        value = v;
        isFaceUp = false;
    }

    // mutators
    void setValue(int v) {
        value = v;
    }


    void setFaceUp(bool state) {
        isFaceUp = state;
    }

    // accessors
    int getValue() const {
        return value;
    }

    bool getIsFaceUp() const {
        return isFaceUp;
    }
};

class MemoryGame {
private:
    int rows;                          // Num rows in the board - 4
    int cols;                          // Num columns in the board - 4
    vector<vector<Card>> board;        // 2D vector for the game board
    int attempts;                      // Num attempts made by the player

public:

    // default - create 4 x 4 board and set attempts to 0
    MemoryGame() {
        rows = 4;
        cols = 4;
        attempts = 0;
        // create 2D board with default Cards
        board.resize(rows, vector<Card>(cols));
    }

    // Param Constructor
    // Allows setting a custom board size
    MemoryGame(int r, int c) : rows(r), cols(c), attempts(0) {
        board.resize(rows, vector<Card>(cols));
    }

    // Function to Initialize and Shuffle the Board with Pairs
    void initializeBoard() {

        // to all the numbers we need 
        // pushback 1- 8 twice
        vector<int> pairs;
        for (int i = 1; i <= 8; ++i) {
            pairs.push_back(i);
            pairs.push_back(i);
        }

        // have to shuffle pairs using a random number generator
        // Seed the random number generator with current time for randomness
        // time(0) - randomize every second
        unsigned seed = time(0);

        // shuffle =  <algorithm>  - rand rearranges elements within a certain range
        // pairs.begin() - first element of pairs vector
        // pairs.end() - 1 past the last element in pairs vector
        // default_random_engine(seed)  seed set to time(0) makes sure the seed is different every second
        shuffle(pairs.begin(), pairs.end(), default_random_engine(seed));

        // Assign shuffled numbers to the board
        // standard, just like 2D array (i, j [r][c]])
        int i = 0; // Index to iterate through shuffled pairs
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                board[r][c].setValue(pairs[i++]); // Set card value
                board[r][c].setFaceUp(false);          // card is face down
            }
        }
    }

    // Function to Display the Current Board
    // we want to show face up cards
    // we want to hide face down cards
    void displayBoard() const {
        // Display column headers
        cout << "    ";  // start with spaces to align the column numbers above the actual board

        // loop to print column number up top and a space after each one
        for (int c = 1; c <= cols; ++c) {
            cout << c << "   ";
        }
        // print a border across the top of each column
       // border is ---+  (the + makes the squares)
        cout << "\n  +";
        for (int c = 0; c < cols; ++c) {
            cout << "---+";
        }
        cout << endl;

        // Display each row with row number
        for (int r = 0; r < rows; ++r) {
            cout << r + 1 << " | ";     // print the row number starting at 1 with a separator '|'

            // display all of the card values and check if card is faceUp
            for (int c = 0; c < cols; ++c) {

                // if the card is face up - display the value with a separator
                if (board[r][c].getIsFaceUp()) {
                    cout << board[r][c].getValue() << " | ";
                }
                // card is face down
                else {
                    cout << "* | ";   // else print * with a separator
                }
            }

            // after we finish displaying the row we start the row's bottom border
            cout << "\n  +";

            // print our border
            for (int c = 0; c < cols; ++c) {
                cout << "---+";
            }
            cout << endl;
        }
    }



    // Function to Get and Validate User Input for Selecting Two Cards
    // - row1, col1: store the first card's row and column
    // - row2, col2: store the second card's row and column
    // Returns:
    // - true if input is valid 
    // - else false 
    bool getUserInput(int& row1, int& col1, int& row2, int& col2) {
        bool valid = false; // for checking input


        while (!valid) {
            // Prompt the user to enter coordinates for the first card
            cout << "Enter Coordinates For The First Card (row (1 - 4) Followed By A 'SPACE' Then Column (1 - 4)): ";
            cin >> row1 >> col1;

            // Check for fail and range
            if (cin.fail() || row1 < 1 || row1 > 4 || col1 < 1 || col1 > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Input. Please Enter Two Numbers Between (1-4) Seperated By A Space:\n";
                continue;
            }


            // Prompt the user to enter coordinates for the second card
            cout << "Enter Coordinates For The Second Card (row and column): ";
            cin >> row2 >> col2;

            // check for fail and range
            if (cin.fail() || row2 < 1 || row2 > 4 || col2 < 1 || col2 > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Input. Please Enter Two Numbers Between (1-4).\n";
                continue;
            }


            // Cannot pick the same card twice
            if (row1 == row2 && col1 == col2) {
                cout << "You Selected The Same Card Twice. Please Choose Two Different Cards.\n";
                continue;
            }

            // If all checks pass, set the valid flag to true to exit the loop
            valid = true;
        }

        // return true if user selected 2 different locations of cards and choices were in range
        return true;
    }


    // Function to Check if the Two Selected Cards Match
    // passing in the user selected coordinates
    bool checkMatch(int row1, int col1, int row2, int col2) {

        // was displaying and checking the wrong coordinate
        // have to -1 from both user choices
        row1 -= 1;
        col1 -= 1;
        row2 -= 1;
        col2 -= 1;

        // Reveal the selected cards
        board[row1][col1].setFaceUp(true);
        board[row2][col2].setFaceUp(true);

        // Display the board with the selected cards face up
        displayBoard();

        // add 1 to attempts
        attempts++;

        // Check if the two cards have the same value
        if (board[row1][col1].getValue() == board[row2][col2].getValue()) {
            cout << "It's A Match!\n";
            return true; // match
        }
        else {
            cout << "Not A Match.\n";
            // Pause to let the player see the cards
            cout << "Press Enter To Continue...";

            // New (Start Using) Allows for an input before we continue to make sure the user seen the output
            cin.ignore(); // Ignore the leftover newline character
            cin.get();    // Wait for the player to press Enter

            // Flip the cards that the user selced back face down
            board[row1][col1].setFaceUp(false);
            board[row2][col2].setFaceUp(false);
            return false; // Not a match
        }
    }

    // Function to Check if the Game is Over (All Pairs Found)
    // Are all cards face up?
    bool isGameOver() const {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (!board[r][c].getIsFaceUp()) {
                    return false; // Found at least one card still face down
                }
            }
        }
        return true; // All cards are face up
    }


    // Function to Ask the Player if They Want to Play Again with checking
    bool playAgain() const {
        char choice; // store the player's choice

        while (true) {
            cout << "Do You Want To Play Again? (Y/N): ";
            cin >> choice;

            // Convert to upper
            choice = toupper(choice);

            // Check for fail
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please Enter 'Y' For Yes or 'N' for No:\n";
                continue;
            }

            // Check if choice is 'Y'
            if (choice == 'Y') {
                return true; // true - play again
            }
            // Check if choice is 'N' 
            else if (choice == 'N') {
                return false; // false - exit
            }
            // Not Y or N
            else {
                cout << "Invalid Choice. Please Enter 'Y' For Yes or 'N' for No:\n";
            }
        }

        // return false because i kept getting a warning, thought handled in loop but will research further (compiler)
        return false;
    }


    // Function to keep playing
    void play() {
        char initialChoice; // to store users choice to (Play Again)

        do {
            // create our board
            initializeBoard();

            // Set attempts to 0
            attempts = 0;

            // Continue the game until all pairs are found
            while (!isGameOver()) {
                // Display the board
                displayBoard();

                int row1, col1, row2, col2;

                // Call our valid input function
                bool validInput = false;
                while (!validInput) {
                    validInput = getUserInput(row1, col1, row2, col2);
                }

                // Check if the selected cards match
                checkMatch(row1, col1, row2, col2);
            }

            // Player found all the pairs - display attempts
            cout << "Good Job! You Found All The Pairs In " << attempts << " Attempts.\n";

            // Ask if the player wants to play again
        } while (playAgain());

        cout << "See You Soon: \n"; // Exit message
    };

};


// Boo Ya! Empty main()
int main() {
    // Create a MemoryGame object
    MemoryGame game;

    // Start the game by calling the 'play' method
    game.play();

    return 0; 
}
