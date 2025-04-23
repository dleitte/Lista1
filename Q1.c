#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_POINTS 100
#define MAX_LINE 1024

typedef struct {
    int x, y;
} Point;

double distance(Point a, Point b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

double distance_to_origin(Point p) {
    return sqrt(p.x * p.x + p.y * p.y);
}

int compare(const void *a, const void *b) {
    Point *p1 = (Point*)a;
    Point *p2 = (Point*)b;
    double d1 = distance_to_origin(*p1);
    double d2 = distance_to_origin(*p2);
    if (d1 < d2) return -1;
    else if (d1 > d2) return 1;
    else return 0;
}

// Agora recebe um FILE * como parâmetro
void process_line(char *line, FILE *fp_out) {
    Point original[MAX_POINTS], sorted[MAX_POINTS];
    int count = 0;
    int x, y;

    char *ptr = line;
    while ((ptr = strchr(ptr, '(')) != NULL) {
        sscanf(ptr, " (%d ,%d)", &x, &y);
        original[count].x = x;
        original[count].y = y;
        sorted[count] = original[count];
        count++;
        ptr++;
    }

    qsort(sorted, count, sizeof(Point), compare);

    fprintf(fp_out, "points");
    for (int i = 0; i < count; i++) {
        fprintf(fp_out, " (%d,%d)", sorted[i].x, sorted[i].y);
    }

    double total = 0.0;
    for (int i = 0; i < count - 1; i++) {
        total += distance(original[i], original[i+1]);
    }

    double shortcut = distance(original[0], original[count-1]);

    fprintf(fp_out, " distance %.2f shortcut %.2f\n", total, shortcut);
}

int main() {
    FILE *fp_in = fopen("L0Q1.in", "r");
    FILE *fp_out = fopen("L0Q1.out", "w");
    if (fp_in == NULL || fp_out == NULL) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp_in)) {
        if (strstr(line, "points") != NULL) {
            process_line(line, fp_out);
        }
    }

    fclose(fp_in);
    fclose(fp_out);
    return 0;
}
