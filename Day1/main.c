#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

typedef struct BigInt {
    long long number;
} BigInt;

BigInt make_bigint(long long x) {
    BigInt b;
    b.number = x;
    return b;
}

BigInt add_big(BigInt a, BigInt b) {
    BigInt r;
    r.number = a.number + b.number;
    return r;
}

BigInt sub_big(BigInt a, BigInt b) {
    BigInt r;
    r.number = a.number - b.number;
    return r;
}

BigInt mod_big(BigInt a, long long m) {
    BigInt r;
    if (m == 0) {
        r.number = 0;
        return r;
    }
    r.number = a.number % m;
    if (r.number < 0) {
        r.number = r.number + m;
    }
    return r;
}

long long div_big(BigInt a, long long m) {
    if (m == 0) {
        return 0;
    }
    return a.number / m;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("need file path\n");
        return 1;
    }

    char *file_name = argv[1];
    printf("In file %s\n", file_name);

    FILE *fp;
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("could not open file\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    if (file_size < 0) file_size = 0;
    char *text = (char *)malloc((size_t)file_size + 100);
    if (!text) {
        printf("malloc failed\n");
        fclose(fp);
        return 1;
    }
    size_t read_bytes = fread(text, 1, (size_t)file_size, fp);
    text[read_bytes] = '\0';
    fclose(fp);

    char *words[9999];
    int word_count = 0;
    char *token = strtok(text, " \t\r\n");
    while (token != NULL && word_count < 9999) {
        words[word_count] = token;
        word_count = word_count + 1;
        token = strtok(NULL, " \t\r\n");
    }

    printf("With %d words\n", word_count);

    long long MAX_POINTER = 99;
    long long RANGE_SIZE = MAX_POINTER + 1;
    BigInt pointer = make_bigint(50);
    BigInt zero_hits = make_bigint(0);

    int i = 0;
    while (i < word_count) {
        char *cmd = words[i];
        char first = cmd[0];
        long long num = atoll(cmd + 1); 

        BigInt delta;
        if (first == 'R') {
            delta = make_bigint(num);
        } else if (first == 'L') {
            delta = make_bigint(-num);
        } else {
            printf("Invalid command: %s\n", cmd);
            i = i + 1;
            continue;
        }

        long long zero_passes = 0;
        if (delta.number >= 0) {
            zero_passes = div_big(add_big(pointer, delta), RANGE_SIZE);
        } else {
            long long steps = 0 - delta.number;
            if (pointer.number == 0) {
                zero_passes = steps / RANGE_SIZE;
            } else if (steps >= pointer.number) {
                zero_passes = 1 + (steps - pointer.number) / RANGE_SIZE;
            } else {
                zero_passes = 0;
            }
        }

        zero_hits = make_bigint(zero_hits.number + zero_passes);
        pointer = mod_big(add_big(pointer, delta), RANGE_SIZE);

        if (zero_passes == 0 && pointer.number == 0) {
            zero_hits = add_big(zero_hits, make_bigint(1));
        }

        printf("%lld ", pointer.number);
        if (zero_passes > 0) {
            printf("[crossed 0 %lld time(s)] ", zero_passes);
        }

        i = i + 1;
    }

    printf("\nTotal times pointer was at 0 or crossed it: %lld\n", zero_hits.number);

    free(text);
    return 0;
}