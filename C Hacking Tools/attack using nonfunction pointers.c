#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *args, *s1 = "/bin/ls", *s2 = "/bin/ps";
    char buffer[128];
    int i;

    if (argc > 1) {
        args = argv[1]; // Get the command-line argument

        // Copy the argument into the buffer, ensuring no buffer overflow
        for (i = 0; i < 128 && args[i] != '\0'; i++) {
            buffer[i] = args[i];
        }
        buffer[i] = '\0'; // Null-terminate the buffer
    }

    // Execute the system command based on the argument
    if (strcmp(buffer, "ls") == 0) {
        system(s1);
    } else if (strcmp(buffer, "ps") == 0) {
        system(s2);
    } else {
        printf("Invalid command!\n");
    }

    return 0;
}
