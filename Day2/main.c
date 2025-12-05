#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BigInt {
    long long num;
} BigInt;

BigInt make_big(long long x) { BigInt b; b.num = x; return b; }
BigInt add_big(BigInt a, BigInt b) { BigInt r; r.num = a.num + b.num; return r; }
BigInt mul_big(BigInt a, long long b) { BigInt r; r.num = a.num * b; return r; }
long long div_big(BigInt a, long long d) { if (d == 0) return 0; return a.num / d; }
BigInt max_big(BigInt a, BigInt b) { return a.num > b.num ? a : b; }
BigInt min_big(BigInt a, BigInt b) { return a.num < b.num ? a : b; }

long long pow10_int(int n) {
    long long r = 1;
    int i = 0;
    while (i < n) { r = r * 10; i = i + 1; }
    return r;
}

long long ceil_div_ll(long long n, long long d) {
    if (d == 0) return 0;
    return (n + d - 1) / d;
}

BigInt sum_invalid_in_range(long long start, long long end) {
    if (start > end) {
        return make_big(0);
    }

    long long found[5000];
    int found_count = 0;

    int max_digits = 0;
    long long tmp = end;
    if (tmp <= 0) tmp = 1;
    while (tmp > 0) {
        tmp = tmp / 10;
        max_digits = max_digits + 1;
    }

    int chunk_len = 1;
    while (chunk_len <= max_digits) {
        long long chunk_min = pow10_int(chunk_len - 1);
        long long chunk_max = pow10_int(chunk_len) - 1;
        int max_repeats = max_digits / chunk_len;
        int repeats = 2;
        while (repeats <= max_repeats) {
            int total_len = chunk_len * repeats;
            long long pow_chunk = pow10_int(chunk_len);
            long long pow_total = pow10_int(total_len);
            long long factor = (pow_total - 1) / (pow_chunk - 1);

            long long chunk = ceil_div_ll(start, factor);
            if (chunk < chunk_min) chunk = chunk_min;
            long long chunk_limit = end / factor;
            if (chunk_limit > chunk_max) chunk_limit = chunk_max;

            while (chunk <= chunk_limit) {
                long long candidate = chunk * factor;
                if (candidate >= start && candidate <= end) {
                    if (found_count < (int)(sizeof(found)/sizeof(found[0]))) {
                        found[found_count] = candidate;
                        found_count = found_count + 1;
                    }
                }
                chunk = chunk + 1;
            }

            repeats = repeats + 1;
        }
        chunk_len = chunk_len + 1;
    }

    BigInt sum = make_big(0);
    int k = 0;
    while (k < found_count) {
        sum = add_big(sum, make_big(found[k]));
        k = k + 1;
    }
    return sum;
}

int parse_range(char *s, long long *start, long long *end) {
    char *dash = strchr(s, '-');
    if (!dash) {
        return 0;
    }
    *dash = '\0';
    *start = atoll(s);
    *end = atoll(dash + 1);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("need file path\n");
        return 1;
    }

    char *file_name = argv[1];
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        printf("cant open file\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    if (size < 0) size = 0;
    char *buffer = (char *)malloc((size_t)size + 20);
    if (!buffer) {
        printf("no memory\n");
        fclose(fp);
        return 1;
    }
    size_t read_bytes = fread(buffer, 1, (size_t)size, fp);
    buffer[read_bytes] = '\0';
    fclose(fp);

    BigInt total = make_big(0);
    char *part = strtok(buffer, ",");
    while (part != NULL) {
        while (*part == ' ' || *part == '\n' || *part == '\r' || *part == '\t') {
            part++;
        }
        long long start = 0;
        long long end = 0;
        if (parse_range(part, &start, &end)) {
            BigInt add_me = sum_invalid_in_range(start, end);
            total = add_big(total, add_me);
        } else {
            printf("bad range: %s\n", part);
        }
        part = strtok(NULL, ",");
    }

    printf("%lld\n", total.num);
    free(buffer);
    return 0;
}