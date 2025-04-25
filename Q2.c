#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_TOKENS 200
#define MAX_STR_LEN 64
#define MAX_LINE_LEN 1024

typedef struct {
    float x, y;
} Ponto;

float distancia(Ponto p) {
    return sqrtf(p.x * p.x + p.y * p.y);
}

int cmp_str(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int cmp_int(const void *a, const void *b) {
    return (*(const int *)a - *(const int *)b);
}

int cmp_float(const void *a, const void *b) {
    float f1 = *(const float *)a, f2 = *(const float *)b;
    if (f1 < f2) return -1;
    if (f1 > f2) return 1;
    return 0;
}

int cmp_ponto(const void *a, const void *b) {
    float d1 = distancia(*(const Ponto *)a);
    float d2 = distancia(*(const Ponto *)b);
    if (d1 < d2) return -1;
    if (d1 > d2) return 1;
    return 0;
}

int tentar_ler_ponto(const char *str, Ponto *p) {
    float x, y;
    int res = sscanf(str, "(%f,%f)", &x, &y);
    if (res == 2) {
        p->x = x;
        p->y = y;
        return 1;
    }
    return 0;
}

void limpar_token(char *token) {
    int i, j = 0;
    char temp[MAX_STR_LEN];
    for (i = 0; token[i]; i++) {
        if (!isspace((unsigned char)token[i])) {
            temp[j++] = token[i];
        }
    }
    temp[j] = '\0';
    strcpy(token, temp);
}

int main(void) {
    char linha[MAX_LINE_LEN];

    char *strings[MAX_TOKENS];
    int inteiros[MAX_TOKENS];
    float reais[MAX_TOKENS];
    Ponto pontos[MAX_TOKENS];

    while (fgets(linha, sizeof(linha), stdin)) {
        int qtd_str = 0, qtd_int = 0, qtd_float = 0, qtd_ponto = 0;

        char *tokens[MAX_TOKENS];
        int n_tokens = 0;

        char *token = strtok(linha, " ");
        while (token && n_tokens < MAX_TOKENS) {
            limpar_token(token);
            tokens[n_tokens++] = token;
            token = strtok(NULL, " ");
        }

        int i = 0;
        while (i < n_tokens) {
            Ponto p;
            char combinado[MAX_STR_LEN * 3];

            if (i + 2 < n_tokens) {
                snprintf(combinado, sizeof(combinado), "%s%s%s", tokens[i], tokens[i+1], tokens[i+2]);
                limpar_token(combinado);
                if (tentar_ler_ponto(combinado, &p)) {
                    pontos[qtd_ponto++] = p;
                    i += 3;
                    continue;
                }
            }

            if (tentar_ler_ponto(tokens[i], &p)) {
                pontos[qtd_ponto++] = p;
                i++;
                continue;
            }

            int valor_int;
            if (sscanf(tokens[i], "%d", &valor_int) == 1 && strchr(tokens[i], '.') == NULL) {
                inteiros[qtd_int++] = valor_int;
                i++;
                continue;
            }

            float valor_float;
            if (sscanf(tokens[i], "%f", &valor_float) == 1 && strchr(tokens[i], '.') != NULL) {
                reais[qtd_float++] = valor_float;
                i++;
                continue;
            }

            strings[qtd_str] = strdup(tokens[i]);
            qtd_str++;
            i++;
        }

        qsort(strings, qtd_str, sizeof(char *), cmp_str);
        qsort(inteiros, qtd_int, sizeof(int), cmp_int);
        qsort(reais, qtd_float, sizeof(float), cmp_float);
        qsort(pontos, qtd_ponto, sizeof(Ponto), cmp_ponto);

        printf("str:");
        for (int i = 0; i < qtd_str; i++) {
            printf("%s%s", (i ? " " : ""), strings[i]);
            free(strings[i]);
        }

        printf(" int:");
        for (int i = 0; i < qtd_int; i++) {
            printf("%s%d", (i ? " " : ""), inteiros[i]);
        }

        printf(" float:");
        for (int i = 0; i < qtd_float; i++) {
            printf("%s%.2g", (i ? " " : ""), reais[i]);
        }

        printf(" p:");
        for (int i = 0; i < qtd_ponto; i++) {
            printf("%s(%.1f,%.1f)", (i ? " " : ""), pontos[i].x, pontos[i].y);
        }

        printf("\n");
    }

    return 0;
}
