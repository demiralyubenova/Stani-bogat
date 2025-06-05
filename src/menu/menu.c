// menu.c
#include "menu.h"
#include "../quiz/quiz.h"
#include <string.h>
#ifdef _WIN32
#include <conio.h>
#define GETCH() getch()
#else
#define GETCH() getchar()
#endif

void option_menu(unsigned char c, int *otg, int count) {
    if (c == 224 || c == 0) {
        c = GETCH();
        if (c == 72) {
            (*otg)--;
            if (*otg < 1) *otg = count;
        } else if (c == 80) {
            (*otg)++;
            if (*otg > count) *otg = 1;
        }
    }
}

void show_question(int otg, int *options, int joker5050_used, int jokerObadise_used, int jokerPublika_used, char *vupros, char option[4][100]) {
    system("cls");
    printf("Vupros: %s\n", vupros);
    for (int i = 0; i < 4; i++) {
        if (options[i]) printf("%c. %s\n", 'A' + i, option[i]);
        else printf("%c. ---\n", 'A' + i);
    }
    printf("\nDostupni Jokeri: %s %s %s\n",
        joker5050_used ? "" : "[50/50]",
        jokerObadise_used ? "" : "[Obadi se]",
        jokerPublika_used ? "" : "[Publika]");
}

void joker5050(int correct, int *options) {
    int removed = 0;
    while (removed < 2) {
        int r = rand() % 4;
        if ((r + 1) != correct && options[r]) {
            options[r] = 0;
            removed++;
        }
    }
    printf("Joker 50/50 izpolzvan.\n");
    GETCH();
}

void jokerObadise(int correct, int *options, int difficulty) {
    int prob = difficulty <= 3 ? 80 : (difficulty <= 6 ? 60 : 30);
    int chance = rand() % 100;
    int suggestion = (chance < prob) ? correct : (rand() % 4) + 1;
    while (!options[suggestion - 1] || suggestion == correct) suggestion = (rand() % 4) + 1;
    printf("Prijatelqt vi predpolaga otgovor: %c\n", 'A' + suggestion - 1);
    GETCH();
}

void jokerPublika(int correct, int *options, int difficulty) {
    int prob = difficulty <= 3 ? 80 : (difficulty <= 6 ? 60 : 30);
    int votes[4] = {0};
    int total = 100;
    if (rand() % 100 < prob) votes[correct - 1] = rand() % 41 + 40;
    else votes[correct - 1] = rand() % 20;
    total -= votes[correct - 1];
    for (int i = 0; i < 4; i++) {
        if (i != correct - 1 && options[i]) {
            int share = rand() % (total + 1);
            votes[i] = share;
            total -= share;
        }
    }
    for (int i = 0; i < 4 && total > 0; i++) {
        if (options[i]) { votes[i]++; total--; }
    }
    printf("Publikata glasuva taka:\n");
    for (int i = 0; i < 4; i++) printf("%c: %d%%\n", 'A' + i, votes[i]);
    GETCH();
}

void start_game() {
    int current_question = 0;
    int correct_answer, difficulty;
    int options[4];
    char question[256];
    char answers[4][100];
    bool found;

    int joker5050_used = 0, jokerObadise_used = 0, jokerPublika_used = 0;

    while (current_question < QUESTION_COUNT) {
        found = false;
        for (int i = 0; i < 4; i++) options[i] = 1;
        load_questions("quiz_questions.txt", answers, question, &correct_answer, &difficulty, &found, current_question);
        if (!found) break;

        int otg = 1;
        unsigned char c;
        do {
            show_question(otg, options, joker5050_used, jokerObadise_used, jokerPublika_used, question, answers);
            c = GETCH();
            option_menu(c, &otg, 5);

            if (c == 13) {
                if (otg >= 1 && otg <= 4 && options[otg - 1]) {
                    if (otg == correct_answer) {
                        printf("Pravilen otgovor!\n");
                        current_question++;
                    } else {
                        printf("Greshen otgovor. Kray na igrata.\n");
                        cleanup_quiz();
                        return;
                    }
                    GETCH();
                    break;
                } else if (otg == 5) {
                    int jok = 1;
                    do {
                        system("cls");
                        printf("Izberi Joker:\n");
                        if (!joker5050_used) printf("1. 50/50\n");
                        if (!jokerObadise_used) printf("2. Obadi se\n");
                        if (!jokerPublika_used) printf("3. Publika\n");
                        printf("4. Nazad\n");
                        c = GETCH();
                        option_menu(c, &jok, 4);
                        if (c == 13) {
                            if (jok == 1 && !joker5050_used) { joker5050(correct_answer, options); joker5050_used = 1; break; }
                            if (jok == 2 && !jokerObadise_used) { jokerObadise(correct_answer, options, difficulty); jokerObadise_used = 1; break; }
                            if (jok == 3 && !jokerPublika_used) { jokerPublika(correct_answer, options, difficulty); jokerPublika_used = 1; break; }
                            if (jok == 4) break;
                        }
                    } while (1);
                }
            }
        } while (1);
    }
    printf("Pozdravlenia! Specheli igrata!\n");
    cleanup_quiz();
    GETCH();
}

int menu() {
    unsigned char c;
    int op = 1;
    srand(time(NULL));
    do {
        system("cls");
        printf("*** STANI BOGAT ***\n");
        printf(op == 1 ? ">1. Start\n" : " 1. Start\n");
        printf(op == 2 ? ">2. Izhod\n" : " 2. Izhod\n");
        c = GETCH();
        if (c == 13) {
            if (op == 1) start_game();
            if (op == 2) break;
        }
        option_menu(c, &op, OPTION_COUNT);
    } while (1);
    return 0;
}