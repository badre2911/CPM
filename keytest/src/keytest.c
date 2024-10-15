/*
 Title:		keytest.c
 Author:	Badre
 Created:	25/07/2024
*/
#include <conio.h>
#include <stdio.h>

#define TIMEOUT 1000
#define END 0

static unsigned char t_getch(int timeout) {
  char ch = END;
  int t = timeout;
  while (t && !kbhit()) {
    t--;
  }
  if (kbhit()) {
    ch = getch();
  }
  return ch;
}

void describe(unsigned char c) {
  if (!c) {
    printf("<nul> ");
  } else if (c == 27) {
    printf("<esc> ");
  } else if (c == 13) {
    printf("<cr> ");
  } else if (c == 10) {
    printf("<lf> ");
  } else if (c == 9) {
    printf("<tab> ");
  } else if (c == 32) {
    printf("<spc> ");
  } else if (c<32) {
    printf("^%c ", c+64);
  } else if (c>127) {
    printf("<%d> ", c);
  } else {
    printf("%c ", c);
  }
}

int main(void) {
  unsigned char i, c;
  unsigned char c1[10];
  printf("Press keys or ^C to exit:\n");

  while (1) {
    i = 0;
    c1[i++] = getch();
    do {
        c = t_getch(TIMEOUT);
        c1[i++] = c;
    } while (c != END);
    printf("You pressed: ");
    for (i=0; c1[i]!=END; i++) {
      describe(c1[i]);
    }
    printf("\n");
    if(c1[0] == 3) {
      printf("Exiting...\n");
      break;
    }
  }
  return 0;
}