#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>

#define WIDTH 90
#define HEIGHT 20

 
 
 struct registration r; // Declare globally at the top of the file

// Now `r` can be used in Draw without needing to pass it as a parameter
char currentUserID[50];
int gameOver,speed;
int level = 1;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirecton dir;
int paused = 0; // Variable to keep track of the pause state

void updateHighScore(char id[], int newScore);
int getHighScore(char id[]);


void pressEnterToContinue() {
    printf("\n Press Enter to continue...");
    // Wait for Enter key press
    while (getchar() != '\n'); // Clears any existing input
    getchar(); // Waits for actual Enter press
}

//setup function 
void Setup() { 
    gameOver = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    //rand function is an inbuilt function in stdlib.h
    
//    which length is between 0 and rand_max which is fixed in stdlib.h
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
}

//Draw Function used to draw the structure of the game
void Draw(int currentHighScore) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(hConsole, cursorPosition);	  // Clear the screen
	int i,j;
	
    for (i = 0; i < WIDTH + 2; i++)
        printf("-");
    printf("\n");

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (j == 0)
                printf("|");  // Wall on the left side
            if (i == y && j == x)
                printf("*");  // Snake head
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
                printf("|");  // Wall on the right side
        }
        printf("\n");
    }




    for (i = 0; i < WIDTH + 2; i++)
        printf("-");
    printf("\n");
    int userHighScore = getHighScore(currentUserID);
printf("Score: %d  |  High Score: %d  | Press p to pause\n", score, currentHighScore);
 if (paused) { // Display pause message in the center of the game area
        int centerX = WIDTH / 2 - 10;
        int centerY = HEIGHT / 2;
        cursorPosition.X = centerX;
        cursorPosition.Y = centerY;
        SetConsoleCursorPosition(hConsole, cursorPosition);
        printf("Press 'P' again to continue");

   
   
}
}

// Input function used for take the input from user for the movement of Snake
void Input() {
   if (_kbhit()) {
        char ch = _getch();
        switch (ch) {
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
        case 'x':
            gameOver = 1;
            break;
        case 'p': // Toggle pause when 'P' is pressed
            paused = !paused;
            break;
        }
    }
}


//logic function 
void Logic() {
    if (paused) return; // If paused, skip the logic updates

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

    // Easy Level: Wrap around walls
    if (level == 1) {
        if (x >= WIDTH) x = 0;
        else if (x < 0) x = WIDTH - 1;
        if (y >= HEIGHT) y = 0;
        else if (y < 0) y = HEIGHT - 1;
    } else { // Medium and Hard Levels: End game on wall collision
        if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
            gameOver = 1;
    }

    // Check for tail collision (common for all levels)
    for ( i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;
    }

    // Fruit collection logic
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
    }
}


void HideCursor() {                                           // For hiding the coursor from the game
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100; // Set the size of the cursor
    info.bVisible = FALSE; // Set the cursor visibility to false
    SetConsoleCursorInfo(consoleHandle, &info);
}


struct registration {
    char username[50];
    int age;
    char gender[10];
    char id[50];
    char pass[7]; // Password must be a 6-digit number, so we need space for 6 characters + null terminator.
};


 instruction()                                                      //Instruction for the game
{
	system("cls");
	printf("\n\n welcome to snake game");
	printf("\n this game is basically for fun and enjoyment");
	printf("\n the snake can be moved using 'a','s','w','d'");
	printf("\n 'a' For moving left");
	printf("\n 's' For moving down");
	printf("\n 'w' For moving up");
	printf("\n 'd' For moving right");
	printf("\n You have to make sure capslock button is off ");
	printf("\n Score will be counted according to how many fruits you have eat \n");
	 pressEnterToContinue();
}

struct highscore {
    char id[50];
    int score;
};


int getHighScore(char id[]) {
    FILE *highscoreFile = fopen("highscore.csv", "r");
    if (highscoreFile == NULL) {
        return 0; // No file yet, so high score is zero
    }
    
    struct highscore hs;
    while (fread(&hs, sizeof(hs), 1, highscoreFile)) {
        if (strcmp(hs.id, id) == 0) {
            fclose(highscoreFile);
            return hs.score;
        }
    }

    fclose(highscoreFile);
    return 0; // If the user ID is not found, return zero
}

void updateHighScore(char id[], int newScore) {
    FILE *highscoreFile = fopen("highscore.csv", "r+b");
    struct highscore hs;
    int found = 0;

    if (highscoreFile == NULL) {
        highscoreFile = fopen("highscore.csv", "wb");
    }

    while (fread(&hs, sizeof(hs), 1, highscoreFile)) {
        if (strcmp(hs.id, id) == 0) {
            if (newScore > hs.score) {
                fseek(highscoreFile, -sizeof(hs), SEEK_CUR);
                hs.score = newScore;
                fwrite(&hs, sizeof(hs), 1, highscoreFile);
            }
            found = 1;
            break;
        }
    }

    if (!found) { // If the ID wasn't found, add a new entry
        strcpy(hs.id, id);
        hs.score = newScore;
        fwrite(&hs, sizeof(hs), 1, highscoreFile);
    }

    fclose(highscoreFile);
}

void displayScoreboard() {
	system("cls");
    FILE *highscoreFile = fopen("highscore.csv", "rb");
    if (highscoreFile == NULL) {
        printf("No high scores available yet.\n");
        return;
    }

    struct highscore hs;
    printf("\n\n====================== SCOREBOARD ======================\n");
    printf("   Username\t\tHigh Score\n");
    printf("-------------------------------------------------------\n");

  
    struct highscore scores[100];
int count = 0,i,j;
while (fread(&hs, sizeof(hs), 1, highscoreFile)) {
    scores[count++] = hs;
}
fclose(highscoreFile);

// Sort scores
for ( i = 0; i < count - 1; i++) {
    for ( j = i + 1; j < count; j++) {
        if (scores[i].score < scores[j].score) {
            struct highscore temp = scores[i];
            scores[i] = scores[j];
            scores[j] = temp;
        }
    }
}

// Display sorted scores
printf("   Username\t\tHigh Score\n");
for (i = 0; i < count; i++) {
    printf("   %-15s\t%d\n", scores[i].id, scores[i].score);
}


    printf("=======================================================\n\n");

    fclose(highscoreFile);
    pressEnterToContinue(); // Function that waits for Enter to continue
}


 // 1 = Easy, 2 = Medium, 3 = Hard

// Level selection
void selectLevel() {
	system("cls");
    printf("\nChoose a difficulty level:\n");
    printf("1. Easy\n");
    printf("2. Medium\n");
    printf("3. Hard\n");
    printf("Enter your choice: ");
    scanf("%d", &level);

    switch (level) {
    case 1:
        speed = 100; // Slow speed
        printf("Easy level selected.\n");
        break;
    case 2:
        speed = 50; // Moderate speed
        printf("Medium level selected.\n");
        break;
    case 3:
        speed = 25; // Fast speed
        printf("Hard level selected.\n");
        break;
    default:
        printf("Invalid choice! Defaulting to Easy level.\n");
        level = 1;
        speed = 100;
    }
    pressEnterToContinue();
}



void login() {                                      //login function
    system("cls");
    system("color 0A");

    char id[50], pass[10];
     int currentScore;
	
  

    FILE *reg;
    reg = fopen("reg.csv", "r");
    if (reg == NULL) {
        printf("Error opening file!\n");
        return;
    }

    struct registration r;

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

    while (fread(&r, sizeof(r), 1, reg)) {
        if (strcmp(id, r.id) == 0 && strcmp(pass, r.pass) == 0) {
            printf("					* Successful Login\n");
            printf("				>>>>>  Let's Play The Game!....<<<<<\n");
            fclose(reg);
            system("cls");
            displayScoreboard();
			instruction();
			selectLevel();
            HideCursor(); // Assuming this function hides the cursor
            Setup();      // Initialize the game
             int currentHighScore = getHighScore(r.id);
			strcpy(currentUserID, r.id);
            while (!gameOver) {
            
        	Draw(currentHighScore);
        	printf("%s", r.username);
        	Input();
       		Logic();
        	Sleep(speed);
   			 }
	
if (score > currentHighScore) {
    updateHighScore(r.id, score);  // Update high score in file
    printf("New high score: %d\n", score);
} else {
    printf("Your final score was: %d\n", score);
}

        }
        else{
        	// If login fails
    printf("					* Incorrect Login Details\n");
    printf("			* Please enter the correct credentials\n");
		}
    }

    

    fclose(reg);
    
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
// Function to check if the ID is already registered
int isIdAlreadyUsed(char id[]) {
    FILE *reg = fopen("reg.csv", "r");
    if (reg == NULL) {
        return 0; // If file doesn't exist yet, no IDs are used
    }

    struct registration temp;
    while (fread(&temp, sizeof(temp), 1, reg)) {
        if (strcmp(temp.id, id) == 0) {
            fclose(reg);
            return 1; // ID is already used
        }
    }

    fclose(reg);
    return 0; // ID is not used
}


void registration() {                                      //Registration function 
    FILE *reg;
    reg = fopen("reg.csv", "a");
    struct registration r;
    system("color 0A");
    
    printf("						***************************\n");
    printf("						* WELCOME TO REGISTRATION *\n");
    printf("						***************************\n\n");
    printf("	---------------------------------------------------------------------------------------------\n");
    printf("					Please enter some detail to register......\n ");
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
    
    fwrite(&r, sizeof(r), 1, reg);
    fclose(reg);
    
    printf("	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("        ---->  NOTE:\n");
    printf("		* Press 'Enter key' to Enter in the game.. 	\n");
    printf("		* players you can use 'Arrow key' to move snake..\n");
    getchar();
    login();
}



int main()                                                                    //Main function 
{
	system("color 0A");	
	printf("						*************************\n");
	printf("						* WELCOME TO SNAKE GAME *\n");
	printf("						*************************\n\n");
    int option;
	printf("	----------------------------------------------------------------------------------------------------------------\n");
	printf("\n      	Note:-\n");
    printf("				Press '1' to Register \n");
    printf("				Press '2' to Login \n");
    printf("	-----------------------------------------------------------------------------------------------------------------\n");
	scanf("%d",&option);
    getchar();  
	
	
    if(option == 1)
        {
            system("cls");
            registration();
        } 
        
    else if(option == 2)
        {
            system("cls");
            login();
        }
    printf("	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
