#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *input = malloc(20);
    char *output = malloc(20);

    if (input == NULL || output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    strncpy(output, "normal output", 20);
    output[19] = '\0'; // Ensure null termination

    if (argc > 1) {
        strncpy(input, argv[1], 19); // Copy at most 19 characters
        input[19] = '\0'; // Ensure null termination
    } else {
        fprintf(stderr, "No input provided\n");
        return 1;
    }

    printf("input at %p: %s\n", (void *)input, input);
    printf("output at %p: %s\n", (void *)output, output);
    printf("\n\n%s\n", output);

    // Free allocated memory before exiting
    free(input);
    free(output);

    return 0;
}
