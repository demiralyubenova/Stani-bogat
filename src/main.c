#include "menu/menu.h"  // Увери се, че това е включено
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quiz/quiz.h"     // <-- ЗАДЪЛЖИТЕЛНО!!!
#include "menu/menu.h"     // ако ползваш меню

int main() {
    initialize_quiz();

    char *options1[] = {"35", "36", "37", "38"};
    add_question_to_file("quiz_questions.txt", "How old is Andrew Tate?", 1, options1, 3);

    char *options2[] = {"A", "B", "C", "D"};
    for (int i = 1; i <= 9; i++) {
        char question_text[100];
        sprintf(question_text, "ABCD Vupros %d", i);
        add_question_to_file("quiz_questions.txt", question_text, i, options2, 1);
    }

    // 💡 Добави това:
    menu();  // показва менюто на потребителя

    cleanup_quiz();
    return 0;
}