#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countChar(char buffer[], FILE *stream);
int countLine(char buffer[], FILE *stream);

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

  FILE *fileStream = fopen(filePath, "r");
  if (fileStream == NULL) {
    printf("Could not open file: %s", filePath);
    return EXIT_FAILURE;
  }

  char fileBuffer[1024] = {0};

  switch (flag) {
  case 'c':
    printf("%d %s\n", countChar(fileBuffer, fileStream), filePath);
    break;
  case 'l':
    printf("%d %s\n", countLine(fileBuffer, fileStream), filePath);
    break;
  default:
    printf("Invalid flag: %c", flag);
    fclose(fileStream);
    return EXIT_FAILURE;
  }

  fclose(fileStream);
  return EXIT_SUCCESS;
}

int countChar(char buffer[], FILE *stream) {
  int output = 0;
  int count = 0;
  while ((count = fread(buffer, 1, sizeof(*buffer), stream))) {
    output += count;
  }
  return output;
}

int countLine(char buffer[], FILE *stream) {
  int output = 0;
  int count = 0;
  while ((count = fread(buffer, 1, sizeof(*buffer), stream))) {
    for (int i = 0; i < count; i++) {
      if (buffer[i] == '\n') {
        output++;
      }
    }
  }
  return output;
}
