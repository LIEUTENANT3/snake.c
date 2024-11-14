#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>

#define WIDTH 90
#define HEIGHT 20

int gameOver;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
int speed = 100; // Initial speed in milliseconds
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirecton dir;
int paused = 0;
int id ;
int highScore = 0; // Variable to store the current user's high score

// Updated fileistration structure with highScore
struct fileistration {
    char username[50];
    int age;
    char gender[10];
    char id[50];
    char pass[7]; // Password must be a 6-digit number, so we need space for 6 characters + null terminator.
    int highScore;
};

// Function prototypes
void Setup();
void Draw();
void Input();
void Logic();
void HideCursor();
void instruction();
void pressEnterToContinue();
void login();
int isValidAge(int age);
int isValidGender(char gender[]);
int isValidPassword(char pass[]);
int isIdAlreadyUsed(char id[]);
void fileistration();
void updateHighScore(const char* id, int newHighScore); // New function to update high score

// Updated Setup function
void Setup() {
    gameOver = 0;
    paused = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
    nTail = 0;
    speed = 100; // Reset speed to initial value
}

//Draw Function used to draw the structure of the game
void Draw() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(hConsole, cursorPosition);  // Clear the screen
    int i, j;

    // Top border
    for (i = 0; i < WIDTH + 2; i++)
        printf("-");
    printf("\n");

    // Game area with walls and SNAKE
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (j == 0)
                printf("|");  // Left wall
            if (i == y && j == x)
                printf("*");  // SNAKE head
            else if (i == fruitY && j == fruitX)
                printf("@");  // Fruit
            else {
                int print = 0;
                int k;
                for (k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("*");
                        print = 1;
                    }
                }
                if (!print)
                    printf(" ");
            }

            if (j == WIDTH - 1)
                printf("|");  // Right wall
        }
        printf("\n");
    }

    // Bottom border
    for (i = 0; i < WIDTH + 2; i++)
        printf("-");
    printf("\n");

    // Footer section for game title, controls, and score/status
    printf("            SNAKE GAME\n");
    printf(" Press 'x' to Exit        Press 'p' to Pause/Resume\n");
    printf(" Score: %d    High Score: %d\n", score, highScore); // Display current score and high score
}

// Input function used for take the input from user for the movement of SNAKE
void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (!paused) dir = LEFT;
            break;
        case 'd':
            if (!paused) dir = RIGHT;
            break;
        case 'w':
            if (!paused) dir = UP;
            break;
        case 's':
            if (!paused) dir = DOWN;
            break;
        case 'p':
            paused = !paused;  // Toggle pause state
            break;
        case 'x':
            gameOver = 1;
            break;
        }
    }
}

// Logic function where the speed increases upon eating fruit
void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    int i;
    for (i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break;
    case DOWN: y++; break;
    default: break;
    }

    // If the SNAKE hits the wall, game over
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
        gameOver = 1;

    // If the SNAKE hits its own tail, game over
    for (i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;
    }

    // If the SNAKE eats the fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;

        // Increase the speed of the game
        if (speed > 10) { // Set a minimum speed limit to avoid going too fast
            speed -= 5; // Decrease speed (increase game speed) by reducing delay
        }
        
         if (score > highScore) {
            highScore = score;
        }
    }
}
void updateHighScore(const char* id, int newHighScore) {
    FILE *file = fopen("file.csv", "r+b");
    if (!file) {
        printf("Error: Could not open fileistration file.\n");
        return;
    }

    struct fileistration r;
    while (fread(&r, sizeof(r), 1, file)) {
        if (strcmp(r.id, id) == 0) {
            if (newHighScore > r.highScore) {
                r.highScore = newHighScore;
                fseek(file, -sizeof(r), SEEK_CUR); // Move back to the start of record
                fwrite(&r, sizeof(r), 1, file);
            }
            break;
        }
    }
    fclose(file);
}


void HideCursor() {                                           // For hiding the coursor from the game
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100; // Set the size of the cursor
    info.bVisible = FALSE; // Set the cursor visibility to false
    SetConsoleCursorInfo(consoleHandle, &info);
}




 void instruction()                                                      //Instruction for the game
{
	printf("\n\n welcome to SNAKE game");
	printf("\n this game is basically for fun and enjoyment");
	printf("\n the SNAKE can be moved using 'a','s','w','d'");
	printf("\n 'a' For moving left");
	printf("\n 's' For moving down");
	printf("\n 'w' For moving up");
	printf("\n 'd' For moving right");
	printf("\n You have to make sure capslock button is off ");
	printf("\n Score will be counted according to how many fruits you have eat \n");
	
}


void pressEnterToContinue() {
    printf("\n Press Enter to continue...");
    // Wait for Enter key press
    while (getchar() != '\n'); // Clears any existing input
    getchar(); // Waits for actual Enter press
}

void login() {                                      //login function
    system("cls");
    system("color 0A");

    char id[50], pass[10];

    FILE *file;
    file = fopen("file.csv", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    struct fileistration r;

    printf("						********************\n");
    printf("						* WELCOME TO LOGIN *\n");
    printf("						********************\n\n");
    printf("	-----------------------------------------------------------------------------------------------------\n");
    printf("				Please Enter your login credentials below\n");
    printf("	-----------------------------------------------------------------------------------------------------\n");

    // User ID input validation
    printf("			* Enter your User ID: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0'; // Remove the newline character
    while (strlen(id) == 0) {
        printf("			* User ID cannot be empty! Please enter a valid User ID: ");
        fgets(id, sizeof(id), stdin);
        id[strcspn(id, "\n")] = '\0';
    }

    // Password input validation
    printf("			* Enter your password: ");
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = '\0'; // Remove the newline character
    while (strlen(pass) == 0) {
        printf("			* Password cannot be empty! Please enter your password: ");
        fgets(pass, sizeof(pass), stdin);
        pass[strcspn(pass, "\n")] = '\0';
    }

    // Validate user credentials
    int loginSuccessful = 0; // Flag to check if login is successful

    while (fread(&r, sizeof(r), 1, file)) {
        if (strcmp(id, r.id) == 0 && strcmp(pass, r.pass) == 0) {
        	highScore = r.highScore; 
            printf("					* Successful Login\n");
            printf("				>>>>>  Let's Play The Game!....<<<<<\n");
            fclose(file);
            system("cls");
			instruction();
			pressEnterToContinue();
            HideCursor(); // Assuming this function hides the cursor
            Setup();      // Initialize the game

                       // Updated game loop with pause check
            while (!gameOver) {
    Input();  // Always check for input to handle pause toggle

    if (paused) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pausePosition;
        
        // Calculate centered position for the pause message
        pausePosition.X = (WIDTH - 30) / 2; // Center horizontally
        pausePosition.Y = HEIGHT / 2;       // Center vertically
        
        SetConsoleCursorPosition(hConsole, pausePosition); // Move cursor to center
        printf("Game Paused. Press 'p' to resume.");
        
        Sleep(200);  // Delay to reduce CPU usage during pause
        continue;    // Skip Draw() and Logic() when paused
    }

    Draw();   // Draw the game screen when not paused
    Logic();  // Update game state when not paused

    Sleep(100); // Control the game speed; adjust as needed
}
	 updateHighScore(id, highScore);
            return;


    
        }
    }

    // If login fails
    printf("					* Incorrect Login Details\n");
    printf("			* Please enter the correct credentials\n");

    fclose(file);
    
}


//validations
int isValidAge(int age) {
    return (age > 0 && age <= 120); // Check if the age is in a reasonable range.
}

int isValidGender(char gender[]) {
    return (strcmp(gender, "M") == 0 || strcmp(gender, "F") == 0 || strcmp(gender, "O") == 0);
}

int isValidPassword(char pass[]) {
    if (strlen(pass) != 6) {
        return 0;
    }
    int i;
    for (i = 0; i < 6; i++) {
        if (!isdigit(pass[i])) {
            return 0;
        }
    }
    return 1;
}
// Function to check if the ID is already Registrationed
int isIdAlreadyUsed(char id[]) {
    FILE *file = fopen("file.csv", "r");
    if (file == NULL) {
        return 0; // If file doesn't exist yet, no IDs are used
    }

    struct fileistration temp;
    while (fread(&temp, sizeof(temp), 1, file)) {
        if (strcmp(temp.id, id) == 0) {
            fclose(file);
            return 1; // ID is already used
        }
    }

    fclose(file);
    return 0; // ID is not used
}


void fileistration() {                                      //fileistration function 
    FILE *file;
    file = fopen("file.csv", "a");
    struct fileistration r;
    system("color 0A");
    
    printf("						***************************\n");
    printf("						* WELCOME TO REGISTRATION *\n");
    printf("						***************************\n\n");
    printf("	---------------------------------------------------------------------------------------------\n");
    printf("					Please enter some detail to Registration......\n ");
    printf("	---------------------------------------------------------------------------------------------\n");
    
    // Username validation
    printf("			* Enter your name: ");
    scanf("%s", r.username);
    while (strlen(r.username) == 0) {
        printf("			* Username cannot be empty! Please enter a valid name: ");
        scanf("%s", r.username);
    }
    
    // Age validation
    printf("			* Enter your age: ");
    scanf("%d", &r.age);
    while (!isValidAge(r.age)) {
        printf("			* Invalid age! Please enter a valid age between 1 and 120: ");
        scanf("%d", &r.age);
    }
    
    // Gender validation
    printf("			* Enter your gender (M/F/O): ");
    scanf("%s", r.gender);
    while (!isValidGender(r.gender)) {
        printf("			* Invalid gender! Please enter M, F, or O: ");
        scanf("%s", r.gender);
    }
    
    
    // User ID validation with duplicate check
    printf("			* Enter your user ID: ");
    scanf("%s", r.id);
    while (strlen(r.id) == 0 || isIdAlreadyUsed(r.id)) {
        if (isIdAlreadyUsed(r.id)) {
            printf("			* This ID is already used! Please enter a different ID: ");
        } else {
            printf("			* User ID cannot be empty! Please enter a valid ID: ");
        }
        scanf("%s", r.id);
    }
    
    // Password validation
    printf("			* Enter your password (exactly 6 digits): ");
    scanf("%s", r.pass);
    while (!isValidPassword(r.pass)) {
        printf("			* Invalid password! Please enter a 6-digit number: ");
        scanf("%s", r.pass);
    }
    
    fwrite(&r, sizeof(r), 1, file);
    fclose(file);
    
    printf("	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("        ---->  NOTE:\n");
    printf("		* Press 'Enter key' to Enter in the game.. 	\n");
    printf("		* players you can use 'Arrow key' to move SNAKE..\n");
    getchar();
    login();
}

 int main() {
    system("color 0A");    
    printf("                        *************************\n");
    printf("                        * WELCOME TO SNAKE GAME *\n");
    printf("                        *************************\n\n");

    int option;
    printf("    ----------------------------------------------------------------------------------------------------------------\n");
    printf("\n          Note:-\n");
    printf("                Press '1' to Registration \n");
    printf("                Press '2' to Login \n");
    printf("    -----------------------------------------------------------------------------------------------------------------\n");
    scanf("%d", &option);
    getchar();  

    if (option == 1) {
        system("cls");
        fileistration();
    } else if (option == 2) {
        system("cls");
        login();
    }

    printf("    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    Setup();  // Initialize game

    while (!gameOver) {
        Input();  // Allow input to handle pause toggle

        if (paused) {
            system("cls"); // Clear screen to show only pause message
            printf("\nGame Paused. Press 'p' to resume.\n");
            Sleep(200);  // Reduce CPU usage when paused
            continue;    // Skip the rest of the loop when paused
        }

        Draw();   // Draw the game screen
        Logic();  // Only update game state if not paused

        Sleep(speed);  // Adjust delay based on current speed
    }

    // Add this block after the game loop ends
    if (gameOver) {
        updateHighScore(id, highScore); // Save high score to file at game end
        printf("\nGame Over! Your final score: %d, High Score: %d\n", score, highScore);
    }

    return 0;
}





