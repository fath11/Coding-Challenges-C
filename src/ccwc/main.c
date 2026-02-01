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
  char *filePath = argc == 2 ? argv[2] : argv[1];

  FILE *stream;
  if (filePath && filePath[1] != '-') {
    stream = fopen(filePath, "r");
    if (stream == NULL) {
      printf("Could not open file: %s", filePath);
      return EXIT_FAILURE;
    }
  } else {
    stream = stdin;
  }

  char buffer[1024] = {0};

  int bytes = 0;
  int lines = 0;
  int words = 0;
  int chars = 0;

  int count;
  bool found_whitespace = true;
  bool is_in_multibyte = false;
  while ((count = fread(buffer, 1, sizeof(*buffer), stream))) {
    bytes += count;
    for (int i = 0; i < count; i++) {
      switch (buffer[i]) {
      case '\n':
        lines++;
      case ' ':
      case '\t':
      case '\r':
      case '\v':
      case '\f':
        found_whitespace = true;
        break;
      default:
        if (found_whitespace == true) {
          words++;
        }
        found_whitespace = false;
        break;
      }

      // This stupid peice of shit is not accurate and I cannot for the life of
      // me figure out why Please yell at me about how stupid my code is so I
      // can fix it and sleep at night
      if ((buffer[i] >> 7 == 0b0)) {
        is_in_multibyte = false;
        chars++;
      } else {
        if (is_in_multibyte == false) {
          chars++;
        }
        is_in_multibyte = true;
      }
    }
  }

  switch (flag) {
  case 'c':
    printf("%d ", bytes);
    break;
  case 'l':
    printf("%d ", lines);
    break;
  case 'w':
    printf("%d ", words);
    break;
  case 'm':
    printf("%d ", chars);
    break;
  default:
    printf("%d %d %d ", lines, words, bytes);
    break;
  }
  printf("%s\n", filePath ? filePath : "");

  fclose(stream);
  return EXIT_SUCCESS;
}
