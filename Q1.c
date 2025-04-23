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

void process_line(char *line) {
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

    printf("points");
    for (int i = 0; i < count; i++) {
        printf(" (%d,%d)", sorted[i].x, sorted[i].y);
    }

    double total = 0.0;
    for (int i = 0; i < count - 1; i++) {
        total += distance(original[i], original[i+1]);
    }

    double shortcut = distance(original[0], original[count-1]);

    printf(" distance %.2f shortcut %.2f\n", total, shortcut);
}

int main() {
    FILE *f = fopen("L0Q1.in", "r");
    if (!f) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "points") != NULL) {
            process_line(line);
        }
    }

    fclose(f);
    return 0;
}
