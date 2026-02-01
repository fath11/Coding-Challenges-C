#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countByte(char buffer[], FILE *stream);
int countLine(char buffer[], FILE *stream);
int countWord(char buffer[], FILE *stream);
int countChar(char buffer[], FILE *stream);

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
    printf("%d %s\n", countByte(fileBuffer, fileStream), filePath);
    break;
  case 'l':
    printf("%d %s\n", countLine(fileBuffer, fileStream), filePath);
    break;
  case 'w':
    printf("%d %s\n", countWord(fileBuffer, fileStream), filePath);
    break;
  case 'm':
    printf("%d %s\n", countChar(fileBuffer, fileStream), filePath);
    break;
  default:
    printf("Invalid flag: %c", flag);
    fclose(fileStream);
    return EXIT_FAILURE;
  }

  fclose(fileStream);
  return EXIT_SUCCESS;
}

int countByte(char buffer[], FILE *stream) {
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

int countWord(char buffer[], FILE *stream) {
  int output = 0;
  int count = 0;
  bool found_whitespace = true;
  while ((count = fread(buffer, 1, sizeof(*buffer), stream))) {
    for (int i = 0; i < count; i++) {
      switch (buffer[i]) {
      case ' ':
      case '\n':
      case '\t':
      case '\r':
      case '\v':
      case '\f':
        found_whitespace = true;
        break;
      default:
        if (found_whitespace == true) {
          output++;
        }
        found_whitespace = false;
        break;
      }
    }
  }

  return output;
}

// This stupid peice of shit is not accurate and I cannot for the life of me figure out why
// Please yell at me about how stupid my code is so I can fix it and sleep at night
int countChar(char buffer[], FILE *stream) {
  int output = 0;
  int count = 0;
  bool is_in_multibyte = false;
  while ((count = fread(buffer, 1, sizeof(*buffer), stream))) {
    for (int i = 0; i < count; i++) {
      if ((buffer[i] >> 7 == 0b0)) {
        is_in_multibyte = false;
        output++;
      } else {
        if (is_in_multibyte == false) {
          output++;
        }
        is_in_multibyte = true;
      }
    }
  }
  return output;
}
