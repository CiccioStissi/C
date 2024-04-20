#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 31
#define MAX_WORDS 100

struct InputParams {
    char filename[MAX_STRING_LENGTH];
    int k;
};

struct Queue {
    char strings[MAX_WORDS][MAX_STRING_LENGTH];
    int front, rear;
};

struct InputParams readInput(int argc, char *argv[]) {
    struct InputParams params;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <k>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strcpy(params.filename, argv[1]);
    params.k = atoi(argv[2]);

    if (params.k < 5 || params.k > 15) {
        fprintf(stderr, "Value of k should be between 5 and 15\n");
        exit(EXIT_FAILURE);
    }

    return params;
}

void enqueueWord(struct Queue *q, const char *word) {
    if (q->rear == MAX_WORDS - 1) {
        fprintf(stderr, "Queue is full\n");
        exit(EXIT_FAILURE);
    }

    strcpy(q->strings[q->rear++], word);
}

char *dequeueWord(struct Queue *q) {
    if (q->front == q->rear) {
        fprintf(stderr, "Queue is empty\n");
        exit(EXIT_FAILURE);
    }

    return q->strings[q->front++];
}

void buildQueue(struct Queue *q, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char word[MAX_STRING_LENGTH];
    while (fgets(word, sizeof(word), file) != NULL) {
        word[strcspn(word, "\n")] = '\0'; // Remove newline character if present
        enqueueWord(q, word);
    }

    fclose(file);
}

void buildStringArray(struct Queue *q, char *stringArray[], int k) {
    int i;
    for (i = 0; i < k && q->front != q->rear; ++i) {
        char *word = dequeueWord(q);
        printf("%s\n", word);
        stringArray[i] = strdup(word);
        free(word);
    }
}

int compareStrings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void sortArray(char *stringArray[], int k) {
    qsort(stringArray, k, sizeof(char *), compareStrings);
}

void printArray(char *stringArray[], int k) {
    int i;
    for (i = 0; i < k; ++i) {
        printf("%s\n", stringArray[i]);
    }
}

int main(int argc, char *argv[]) {
    struct InputParams params = readInput(argc, argv);
    struct Queue q = { .front = 0, .rear = 0 };
    char *stringArray[MAX_WORDS];

    buildQueue(&q, params.filename);
    buildStringArray(&q, stringArray, params.k);
    sortArray(stringArray, params.k);
    printArray(stringArray, params.k);

    return 0;
}
