#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>

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
  int bytes = -1; // -1 means disabled.
  int lines = -1; // Reusing variables here is fine because this is a really small project.
  int words = -1; // I won't do this in bigger projects for my own sanity :)
  int chars = -1;

  int opt;
  while ((opt = getopt(argc, argv, "clwm")) != -1) {
    switch (opt) {
    case 'c':
      bytes = 0;
      break;
    case 'l':
      lines = 0;
      break;
    case 'w':
      words = 0;
      break;
    case 'm':
      chars = 0;
      break;
    case '?':
      printf("Invalid flag %c", opt);
      exit(EXIT_FAILURE);
    }
  }

  if (bytes == -1 && lines == -1 && words == -1 && chars == -1) {
    bytes = 0;
    lines = 0;
    words = 0;
  }

  char *filePath = argv[2] ? argv[2] : argv[1];
  FILE *stream;
  if (filePath && filePath[0] != '-') {
    stream = fopen(filePath, "r");
    if (stream == NULL) {
      printf("Could not open file: %s", filePath);
      return EXIT_FAILURE;
    }
  } else {
    stream = stdin;
    filePath = "";
  }

  wint_t current_char;
  char mbCharBuffer[4];
  bool found_whitespace = true; // We assume the beginning of the file has a whitespace.
  setlocale(LC_CTYPE, "");
  while ((current_char = fgetwc(stream)) != WEOF) {
    if (bytes != -1) {
      // Convert the current widechracter to multibyte character and puts it in mbCharBuffer.
      // This function also returns the size of the resulting multibyte character in bytes.
      bytes += wctomb(mbCharBuffer, current_char);
    }

    if (lines != -1 && current_char == '\n') {
      lines++;
    }

    if (chars != -1) {
      chars++;
    }

    if (words != -1) {
      if (iswspace(current_char)) {
        found_whitespace = true;
      } else if (found_whitespace == true) { 
        // This code only runs when we found a non-whitespace character after a whitespace character.
        words++;
        found_whitespace = false;
      }
    }
  }

  fclose(stream);

  if (bytes != -1) {
    printf("%d ", bytes);
  }
  if (lines != -1) {
    printf("%d ", lines);
  }
  if (words != -1) {
    printf("%d ", words);
  }
  if (chars != -1) {
    printf("%d ", chars);
  }

  printf("%s\n", filePath);

  exit(EXIT_SUCCESS);
}
