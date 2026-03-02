#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>
#include<conio.h>
#define MAX_QUESTIONS 30  // 10 per level
#define MAX_NAME_LEN 50
#define TIME_LIMIT 10
#define MIN_CORRECT_TO_ADVANCE 5
#define FIFTY_FIFTY_USES 5

struct Question {
    char question[256];
    char options[4][100];
    char correctOption;
    int difficulty; // 1-easy, 2-medium, 3-hard
};

struct Node {
    struct Question data;
    struct Node* next;
};

struct Node* front = NULL;
struct Node* rear = NULL;

void enqueue(struct Question q) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = q;
    newNode->next = NULL;
    if (rear == NULL) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

struct Question dequeue() {
    struct Question q;
    strcpy(q.question, "");
    if (front == NULL) return q;

    struct Node* temp = front;
    q = temp->data;
    front = front->next;
    if (front == NULL) rear = NULL;
    free(temp);
    return q;
}

int isEmpty() {
    return front == NULL;
}

void shuffle(struct Question questions[], int n) {
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        struct Question temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }
}

void loadQuestions() {
    struct Question qList[MAX_QUESTIONS] = {
        // Easy questions (difficulty 1)
        {"What is the capital of France?", {"Paris", "London", "Berlin", "Madrid"}, 'A', 1},
        {"Which planet is known as the Red Planet?", {"Earth", "Mars", "Jupiter", "Saturn"}, 'B', 1},
        {"What is the largest mammal?", {"Elephant", "Blue Whale", "Giraffe", "Tiger"}, 'B', 1},
        {"Who wrote 'Romeo and Juliet'?", {"Wordsworth", "Shakespeare", "Chaucer", "Milton"}, 'B', 1},
        {"What is the boiling point of water?", {"90°C", "100°C", "110°C", "120°C"}, 'B', 1},
        {"Which gas do plants absorb?", {"Oxygen", "Carbon Dioxide", "Hydrogen", "Nitrogen"}, 'B', 1},
        {"What is the capital of India?", {"Mumbai", "Delhi", "Chennai", "Kolkata"}, 'B', 1},
        {"What is 9 + 10?", {"19", "20", "21", "18"}, 'A', 1},
        {"Fastest land animal?", {"Cheetah", "Lion", "Horse", "Leopard"}, 'A', 1},
        {"Square root of 144?", {"10", "11", "12", "13"}, 'C', 1},

        // Medium questions (difficulty 2)
        {"Which country gifted the Statue of Liberty to the US?", {"France", "England", "Canada", "Spain"}, 'A', 2},
        {"Which element has the chemical symbol 'O'?", {"Gold", "Oxygen", "Silver", "Osmium"}, 'B', 2},
        {"How many continents are there?", {"5", "6", "7", "8"}, 'C', 2},
        {"Which language has the most native speakers?", {"English", "Hindi", "Spanish", "Mandarin"}, 'D', 2},
        {"Who painted the Mona Lisa?", {"Van Gogh", "Picasso", "Da Vinci", "Michelangelo"}, 'C', 2},
        {"What is the largest ocean?", {"Atlantic", "Indian", "Arctic", "Pacific"}, 'D', 2},
        {"Which planet has the most moons?", {"Saturn", "Jupiter", "Neptune", "Mars"}, 'A', 2},
        {"What is the hardest natural substance?", {"Gold", "Iron", "Diamond", "Quartz"}, 'C', 2},
        {"Which country invented tea?", {"India", "China", "England", "Japan"}, 'B', 2},
        {"How many bones in the human body?", {"206", "300", "150", "412"}, 'A', 2},

        // Hard questions (difficulty 3)
        {"What is the atomic number of Einsteinium?", {"99", "100", "101", "102"}, 'A', 3},
        {"Which country has the most time zones?", {"Russia", "USA", "France", "China"}, 'C', 3},
        {"What is the only even prime number?", {"0", "1", "2", "4"}, 'C', 3},
        {"Which planet rotates clockwise?", {"Mars", "Venus", "Jupiter", "Saturn"}, 'B', 3},
        {"Which country has the world's shortest coastline?", {"Monaco", "Vatican City", "Nauru", "Tuvalu"}, 'A', 3},
        {"What is the smallest country by population?", {"Vatican City", "Monaco", "San Marino", "Liechtenstein"}, 'A', 3},
        {"Which element is liquid at room temperature?", {"Bromine", "Mercury", "Both", "Neither"}, 'C', 3},
        {"What is the world's longest river?", {"Nile", "Amazon", "Yangtze", "Mississippi"}, 'B', 3},
        {"Which country has the most pyramids?", {"Egypt", "Mexico", "Sudan", "Peru"}, 'C', 3},
        {"What is the largest desert in the world?", {"Sahara", "Arabian", "Gobi", "Antarctic"}, 'D', 3}
    };

    shuffle(qList, MAX_QUESTIONS);

    for (int i = 0; i < MAX_QUESTIONS; i++) {
        enqueue(qList[i]);
    }
}

void fiftyFifty(struct Question* q) {
    char optionsToKeep[2] = {q->correctOption, 0};
    char wrongOptions[3];
    int wrongCount = 0;

    // Find wrong options
    for (char c = 'A'; c <= 'D'; c++) {
        if (c != q->correctOption) {
            wrongOptions[wrongCount++] = c;
        }
    }

    // Randomly select one wrong option to keep
    int keepWrong = rand() % 3;
    optionsToKeep[1] = wrongOptions[keepWrong];

    // Hide the other two options
    for (char c = 'A'; c <= 'D'; c++) {
        if (c != optionsToKeep[0] && c != optionsToKeep[1]) {
            strcpy(q->options[c-'A'], "[hidden]");
        }
    }
}

void displayWelcomeMessage(char* name) {
    printf("\n===============================================\n");
    printf("          WELCOME TO THE TRIVIA CHALLENGE!\n");
    printf("===============================================\n\n");
    printf("Hello, %s!\n\n", name);
    printf("Get ready to test your knowledge across three exciting levels:\n");
    printf("1. EASY - Basic general knowledge questions\n");
    printf("2. MEDIUM - Tougher questions that require more thought\n");
    printf("3. HARD - Expert-level questions that will truly challenge you\n\n");
    printf("Game Rules:\n");
    printf("- You have %d seconds to answer each question\n", TIME_LIMIT);
    printf("- You need at least %d correct answers in each level to advance\n", MIN_CORRECT_TO_ADVANCE);
    printf("- In the HARD level, you get %d '50:50' lifelines\n", FIFTY_FIFTY_USES);
    printf("- The lifeline will eliminate two wrong answers\n\n");
    printf("Your journey begins now! Good luck!\n");
    printf("===============================================\n\n");
}

void displayConclusion(char* name, int score, int maxPossible, int levelReached) {
    printf("\n===============================================\n");
    printf("             QUIZ COMPLETE!\n");
    printf("===============================================\n\n");
    printf("Congratulations, %s!\n\n", name);
    printf("Your final score: %d/%d\n", score, maxPossible);

    // Calculate percentage
    float percentage = (float)score / maxPossible * 100;
    printf("That's %.1f%% correct!\n\n", percentage);

    printf("Level reached: ");
    switch(levelReached) {
        case 1: printf("EASY\n"); break;
        case 2: printf("MEDIUM\n"); break;
        case 3: printf("HARD - CHAMPION LEVEL!\n"); break;
    }

    printf("\nPerformance Analysis:\n");
    if (percentage >= 90) {
        printf("Outstanding! You're a trivia master!\n");
    } else if (percentage >= 75) {
        printf("Excellent performance! You know your stuff!\n");
    } else if (percentage >= 50) {
        printf("Good effort! With a bit more practice, you'll be great!\n");
    } else {
        printf("Keep learning! Every expert was once a beginner.\n");
    }

    printf("\nThank you for playing the Trivia Challenge!\n");
    printf("We hope you enjoyed testing your knowledge.\n");
    printf("Come back soon for another round!\n");
    printf("===============================================\n");
}

void playGame() {
    char name[MAX_NAME_LEN];
    printf("Enter your name: ");
    fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = 0;

    displayWelcomeMessage(name);

    int score = 0;
    int level = 1; // Start with easy
    int questionsInLevel = 0;
    int correctInLevel = 0;
    int fiftyFiftyRemaining = 0;
    int totalQuestionsAttempted = 0;

    while (!isEmpty()) {
        struct Question q = dequeue();

        // Skip questions not for current level
        if (q.difficulty != level) {
            enqueue(q); // Put it back in queue
            continue;
        }

        questionsInLevel++;
        totalQuestionsAttempted++;

        if (level == 3 && fiftyFiftyRemaining > 0) {
            printf("\n50:50 Lifelines remaining: %d\n", fiftyFiftyRemaining);
            printf("Press 'H' to use 50:50 or any other key to continue: ");
            char choice = toupper(getchar());
            while (getchar() != '\n'); // Clear input buffer

            if (choice == 'H') {
                fiftyFifty(&q);
                fiftyFiftyRemaining--;
            }
        }

        printf("\nLevel %d - Question %d/%d\n", level, questionsInLevel, 10);
        printf("%s\n", q.question);
        printf("A. %s\n", q.options[0]);
        printf("B. %s\n", q.options[1]);
        printf("C. %s\n", q.options[2]);
        printf("D. %s\n", q.options[3]);

        time_t start = time(NULL);
        char answer;
        int timedOut = 1;

        // Run a timer loop while checking for input
        while (difftime(time(NULL), start) < TIME_LIMIT) {
            printf("\rTime left: %d seconds ", TIME_LIMIT - (int)difftime(time(NULL), start));
            fflush(stdout);

            if (_kbhit()) {
                answer = toupper(getchar());
                while (getchar() != '\n');  // Clear input buffer
                timedOut = 0;
                break;
            }
            Sleep(1000);
        }
        printf("\n");

        if (timedOut) {
            printf("Time's up! No answer provided.\n");
        } else if (answer == q.correctOption) {
            printf("Correct!\n");
            score++;
            correctInLevel++;
        } else {
            printf("Wrong! Correct answer was %c.\n", q.correctOption);
        }

        // Check level progression
        if (questionsInLevel >= 10) {
            printf("\nLevel %d completed! Correct answers: %d/%d\n",
                  level, correctInLevel, questionsInLevel);

            if (correctInLevel >= MIN_CORRECT_TO_ADVANCE && level < 3) {
                level++;
                questionsInLevel = 0;
                correctInLevel = 0;
                if (level == 3) {
                    fiftyFiftyRemaining = FIFTY_FIFTY_USES;
                    printf("\n===============================================\n");
                    printf("CONGRATULATIONS! You've reached the HARD level!\n");
                    printf("You now have %d '50:50' lifelines to help you.\n", fiftyFiftyRemaining);
                    printf("Use them wisely by pressing 'H' when prompted.\n");
                    printf("===============================================\n\n");
                } else {
                    printf("\nAdvanced to MEDIUM level! The questions get tougher now.\n\n");
                }
            } else if (level == 3) {
                break; // Game over after hard level
            } else {
                printf("\nYou needed %d correct answers to advance. Game over.\n", MIN_CORRECT_TO_ADVANCE);
                break;
            }
        }
    }

    displayConclusion(name, score, totalQuestionsAttempted, level);
}

int main() {
    loadQuestions();
    playGame();
    return 0;
}
