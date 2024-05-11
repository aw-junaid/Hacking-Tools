#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])

    {
char *input = malloc (20);
char *output = malloc (20);
strcpy (output, "normal output");
strcpy (input, argv[1]);

printf ("input at %p: %s\n", input, input);
printf ("output at %p: %s\n", output, output);

printf("\n\n%s\n", output);
}
