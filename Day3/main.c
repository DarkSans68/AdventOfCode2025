#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("need file path\n");
        return 1;
    }

    char *file_name = argv[1];
    printf("In file %s\n", file_name);

    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        printf("cant open file\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    if (file_size < 0) file_size = 0;
    char *buffer = (char *)malloc((size_t)file_size + 10);
    if (!buffer) {
        printf("no mem\n");
        fclose(fp);
        return 1;
    }
    size_t read_bytes = fread(buffer, 1, (size_t)file_size, fp);
    buffer[read_bytes] = '\0';
    fclose(fp);

    char *parts[10000];
    int parts_count = 0;
    char *tok = strtok(buffer, " \t\r\n");
    while (tok != NULL && parts_count < 10000) {
        parts[parts_count] = tok;
        parts_count = parts_count + 1;
        tok = strtok(NULL, " \t\r\n");
    }

    int g_h = 0;

    int idx = 0;
    while (idx < parts_count) {
        char *p = parts[idx];
        int highest = 0;
        int highest_a_pos = 0;
        int highest_b_pos = 0;

        int i = 0;
        while (p[i] != '\0') {
            char c1 = p[i];
            if (c1 >= '0' && c1 <= '9') {
                int tens = c1 - '0';
                int j = i + 1;
                while (p[j] != '\0') {
                    char c2 = p[j];
                    if (c2 >= '0' && c2 <= '9') {
                        int ones = c2 - '0';
                        int candidate = tens * 10 + ones;
                        if (candidate > highest) {
                            highest = candidate;
                            highest_a_pos = i + 1; 
                            highest_b_pos = j + 1;
                        }
                    }
                    j = j + 1;
                }
            }
            i = i + 1;
        }

        g_h = g_h + highest;
        printf("In part %s, highest combined number is %d (positions %d, %d)\n", p, highest, highest_a_pos, highest_b_pos);

        idx = idx + 1;
    }

    printf("Grand highest combined number is %d\n", g_h);

    free(buffer);
    return 0;
}