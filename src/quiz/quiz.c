

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "quiz.h"

// Структура за въпрос
/*
typedef struct quiz_question {
    char question_text[256];
    char options[4][100];
    int correct_option_index;
    int difficulty;
    struct quiz_question *next;
} QuizQuestion;
*/
static QuizQuestion *head = NULL;

void initialize_quiz() {
    head = NULL;
}

void add_question_to_file(const char *filename, char *text, uint8_t difficulty, char **options, uint8_t correct_index) {
    QuizQuestion *new_question = (QuizQuestion *)malloc(sizeof(QuizQuestion));
    if (!new_question) {
        perror("Не може да се задели памет за въпроса");
        return;
    }

    strncpy(new_question->question_text, text, sizeof(new_question->question_text));
    for (int i = 0; i < 4; i++) {
        strncpy(new_question->options[i], options[i], sizeof(new_question->options[i]));
    }

    new_question->correct_option_index = correct_index;
    new_question->difficulty = difficulty;
    new_question->next = head;
    head = new_question;

    FILE *file = fopen(filename, "a");
    if (!file) {
        perror("Не може да се отвори файл за запис");
        return;
    }

    fprintf(file, "%s\n", text);
    for (int i = 0; i < 4; i++) {
        fprintf(file, "%s\n", options[i]);
    }
    fprintf(file, "%d\n%d\n", correct_index, difficulty);

    fclose(file);
}

void edit_question_in_file(const char *filename, int question_number, const char *newText, uint8_t new_difficulty, const char *newOptions[], uint8_t newCorrect_index) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Не може да се отвори файл за четене");
        return;
    }

    FILE *temp_file = fopen("temp.txt", "w");
    if (!temp_file) {
        perror("Не може да се отвори временен файл за писане");
        fclose(file);
        return;
    }

    char buffer[255];
    int line_count = 0;
    int current_question = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (current_question == question_number) {
            if (line_count == 0) {
                fprintf(temp_file, "%s\n", newText);
            } else if (line_count <= 4) {
                fprintf(temp_file, "%s\n", newOptions[line_count - 1]);
            } else if (line_count == 5) {
                fprintf(temp_file, "%d\n", newCorrect_index);
            } else if (line_count == 6) {
                fprintf(temp_file, "%d\n", new_difficulty);
            }
        } else {
            fprintf(temp_file, "%s", buffer);
        }

        line_count++;
        if (line_count == 7) {
            line_count = 0;
            current_question++;
        }
    }

    fclose(file);
    fclose(temp_file);

    remove(filename);
    rename("temp.txt", filename);
}

void save_questions_to_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Грешка при отваряне на файл за запис");
        return;
    }

    QuizQuestion *current = head;
    while (current) {
        fprintf(file, "%s\n", current->question_text);
        for (int i = 0; i < 4; i++) {
            fprintf(file, "%s\n", current->options[i]);
        }
        fprintf(file, "%d\n%d\n", current->correct_option_index, current->difficulty);
        current = current->next;
    }

    fclose(file);

}
void load_questions(const char *filename, char answers[4][100], char *question, int *correct_answer, int *difficulty, bool *found, int current_question) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        *found = false;
        return;
    }

    int index = 0;
    while (!feof(file)) {
        char buffer[512];
        fgets(buffer, sizeof(buffer), file);
        if (index == current_question) {
            sscanf(buffer, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d",
                   question,
                   answers[0], answers[1], answers[2], answers[3],
                   correct_answer, difficulty);
            *found = true;
            fclose(file);
            return;
        }
        index++;
    }

    *found = false;
    fclose(file);
}
void cleanup_quiz() {
    QuizQuestion *current = head;
    while (current) {
        QuizQuestion *next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
}
    