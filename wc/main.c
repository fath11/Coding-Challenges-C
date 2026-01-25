#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * https://www.ibm.com/docs/en/i/7.6.0?topic=functions-main-function
 *
 * int argc     Argument Count, number of arguments provided
 * char *argv[] Argument Vector, an array of pointers to arrays of char objects.
 *              The arrays char objects are null-terminated strings representing
 *              the arguments that were entered. argv[0] is the program name
 *              and argv[1] and every item after is the program's arguments.
 */
int main(int argc, char *argv[]) {
  char flag = argv[1][1]; // flag starts with "-"
  char *filePath = argv[2];

  FILE *filePointer = fopen(filePath, "r");
  if (filePointer == NULL) {
    printf("Could not open file: %s", filePath);
    return EXIT_FAILURE;
  }

  char fileBuffer[1024] = {0};
  int output = 0;

  switch (flag) {
  case 'c':
    while (fgets(fileBuffer, sizeof(fileBuffer), filePointer) != NULL) {
      output += strlen(fileBuffer);
    }

    printf("%d %s\n", output, filePath);
    break;
  default:
    printf("Invalid flag: %c", flag);
    fclose(filePointer);
    return EXIT_FAILURE;
  }

  fclose(filePointer);
  return EXIT_SUCCESS;
}
