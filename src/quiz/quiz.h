#ifndef QUIZ_H
#define QUIZ_H

#include <stdint.h>
#include <stdbool.h>

typedef struct quiz_question {
    char question_text[256];
    char options[4][100];
    int correct_option_index;
    int difficulty;
    struct quiz_question *next;
} QuizQuestion;

void initialize_quiz();
void add_question_to_file(const char *filename, char *text, uint8_t difficulty, char **options, uint8_t correct_index);
void load_questions(const char *filename, char answers[4][100], char *question, int *correct_answer, int *difficulty, bool *found, int current_question);
void edit_question_in_file(const char *filename, int question_number, const char *newText, uint8_t new_difficulty, const char *newOptions[], uint8_t newCorrect_index);
void save_questions_to_file(const char *filename);
void cleanup_quiz();

#endif