#include <stdio.h>
#include <string.h>

#define NUM_QUESTOES 10

int main(void) {
    // Perguntas e opções para cada pergunta
    char* perguntas[NUM_QUESTOES] = {
        "Quanto é 2 + 2?",
        "Quanto é 5 - 3?",
        "Quanto é 3 * 3?",
        "Quanto é 10 / 2?",
        "Quanto é 7 + 6?",
        "Quanto é 9 - 4?",
        "Quanto é 6 * 2?",
        "Quanto é 8 / 2?",
        "Quanto é 10 + 10?",
        "Quanto é 15 - 5?"
    };

    char* opcoes[NUM_QUESTOES][4] = {
        {"A) 4", "B) 5", "C) 6", "D) 3"},
        {"A) 1", "B) 2", "C) 3", "D) 4"},
        {"A) 6", "B) 9", "C) 12", "D) 15"},
        {"A) 3", "B) 4", "C) 5", "D) 6"},
        {"A) 11", "B) 12", "C) 13", "D) 14"},
        {"A) 5", "B) 4", "C) 6", "D) 3"},
        {"A) 8", "B) 12", "C) 10", "D) 11"},
        {"A) 3", "B) 5", "C) 4", "D) 6"},
        {"A) 20", "B) 25", "C) 15", "D) 10"},
        {"A) 10", "B) 15", "C) 20", "D) 25"}
    };

    // Respostas corretas para cada questão
    char* respostas_corretas[NUM_QUESTOES] = {
        "A", "B", "B", "C", "C", "A", "B", "C", "A", "A"
    };

    int pontuacao = 0;
    char resposta[10];

    // Itera sobre as perguntas
    for (int i = 0; i < NUM_QUESTOES; i++) {
        printf("%d. %s\n", i + 1, perguntas[i]);
        for (int j = 0; j < 4; j++) {
            printf("%s\n", opcoes[i][j]);
        }

        printf("Digite sua resposta: ");
        scanf("%s", resposta);

        // Verifica se a resposta está correta
        if (strcmp(resposta, respostas_corretas[i]) == 0) {
            printf("Resposta correta!\n");
            pontuacao++;
        } else {
            printf("Resposta errada. A resposta correta é %s.\n", respostas_corretas[i]);
        }

        printf("\n");
    }

    printf("Sua pontuação total foi: %d/%d.\n", pontuacao, NUM_QUESTOES);

    return 0;
}
