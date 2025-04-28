#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LINE 1024
#define MAX_TOKENS 128

typedef struct {
    double x, y;
    double dist;
    char original[32];
} Point;

int is_integer(const char *s) {
    char *end;
    strtol(s, &end, 10);
    return *end == '\0';
}

int is_float(const char *s) {
    char *end;
    strtod(s, &end);
    return *end == '\0' && strchr(s, '.');
}

int is_point(const char *s, double *x, double *y, char *original) {
    if (s[0] != '(' || s[strlen(s) - 1] != ')') return 0;
    char temp[64];
    strncpy(temp, s + 1, strlen(s) - 2);
    temp[strlen(s) - 2] = '\0';
    char *comma = strchr(temp, ',');
    if (!comma) return 0;
    *comma = '\0';
    char *xstr = temp;
    char *ystr = comma + 1;
    char *end1, *end2;
    *x = strtod(xstr, &end1);
    *y = strtod(ystr, &end2);
    if (*end1 || *end2) return 0;
    strcpy(original, s);
    return 1;
}

int cmp_str(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int cmp_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int cmp_double(const void *a, const void *b) {
    double diff = *(double *)a - *(double *)b;
    if (diff < 0) return -1;
    else if (diff > 0) return 1;
    else return 0;
}

int cmp_point(const void *a, const void *b) {
    double d1 = ((Point *)a)->dist;
    double d2 = ((Point *)b)->dist;
    if (d1 < d2) return -1;
    else if (d1 > d2) return 1;
    else return 0;
}

int main() {
    FILE *fp_in = fopen("L0Q2.in", "r");
    FILE *fp_out = fopen("L0Q2.out", "w");
    
    if (fp_in == NULL || fp_out == NULL) {
        perror("Erro ao abrir arquivo");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE];

    while (fgets(line, sizeof(line), fp_in)) {
        line[strcspn(line, "\n")] = '\0';  

        char *tokens[MAX_TOKENS];
        int num_tokens = 0;
        char *token = strtok(line, " ");
        while (token) {
            tokens[num_tokens++] = token;
            token = strtok(NULL, " ");
        }

        char *strings[MAX_TOKENS];
        int ints[MAX_TOKENS], num_ints = 0;
        double floats[MAX_TOKENS];
        int num_str = 0, num_float = 0;
        Point points[MAX_TOKENS];
        int num_points = 0;

        for (int i = 0; i < num_tokens; i++) {
            double x, y;
            char original[32];
            if (is_point(tokens[i], &x, &y, original)) {
                points[num_points].x = x;
                points[num_points].y = y;
                points[num_points].dist = sqrt(x * x + y * y);
                strcpy(points[num_points].original, original);
                num_points++;
            } else if (is_integer(tokens[i])) {
                ints[num_ints++] = atoi(tokens[i]);
            } else if (is_float(tokens[i])) {
                floats[num_float++] = atof(tokens[i]);
            } else {
                strings[num_str++] = tokens[i];
            }
        }

        qsort(strings, num_str, sizeof(char *), cmp_str);
        qsort(ints, num_ints, sizeof(int), cmp_int);
        qsort(floats, num_float, sizeof(double), cmp_double);
        qsort(points, num_points, sizeof(Point), cmp_point);

        fprintf(fp_out, "str:");
        for (int i = 0; i < num_str; i++) {
            if (i > 0) fprintf(fp_out, " ");
            fprintf(fp_out, "%s", strings[i]);
        }

        fprintf(fp_out, " int:");
        for (int i = 0; i < num_ints; i++) {
            if (i > 0) fprintf(fp_out, " ");
            fprintf(fp_out, "%d", ints[i]);
        }

        fprintf(fp_out, " float:");
        for (int i = 0; i < num_float; i++) {
            if (i > 0) fprintf(fp_out, " ");
            fprintf(fp_out, "%g", floats[i]);
        }

        fprintf(fp_out, " p:");
        for (int i = 0; i < num_points; i++) {
            if (i > 0) fprintf(fp_out, " ");
            fprintf(fp_out, "%s", points[i].original);
        }

        fprintf(fp_out, "\n");
    }
    
    fclose(fp_in);
    fclose(fp_out);
    
    return 0;
}
