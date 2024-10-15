/*
 Title:		cpmconio.h
 Author:	Badre
 Created:	23/06/2024
*/

#ifndef _CONIO_H
#define _CONIO_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "global.h"
//#include "vdpmode.h"


#define VT 11

#define CTRL_KEY(k) ((k) & 0x1f)
#define ALT(k)      ((k) + (161 - 'a')) //not work
#define CTRL_ALT(k) ((k) + (129 - 'a')) //not work

//extern void saveScrMmode(void);
//extern void setSavedScrMode(void);
//extern uint8_t kbd_getch(void);
//extern uint8_t ckbhit(void);
//extern void kbd_flush(void);
//extern void cputch(char c);
//extern void cputs(const char *str);

//static int scr_mode = VDPMODE_640x480_16C; 

void setup_devices(void);

bool waitMsg(const char * msg);
void waitKey(void);
void xprintf(const char * format, ...);
int setvdpmode(int mode);

void bell(void);
//void clearInputBuffer(void);
void termFlush(void);
void putcon(uint8_t ch);
void cputch(int c);
void cputchutf8(uint8_t ch);
void cputs(const char *str);
void putsmsg(int x, int y, const char *s);
void cputsxy(int, int, const char *);
void blank(int width);
void puthex8(uint8_t c);
void puthex16(uint16_t c);
//bool kbhit(void);
void rx_purge(void);
uint8_t ckey_esc(void);
unsigned short cgetcar(void);
int ckbhit(void);
uint8_t cgetch(void);
uint8_t getche(void);
uint8_t chready(void);
uint8_t getchNB(void);

int readKbd(void);
void clrscr(void);
void clreol(void);
void clearline(char x, char y);
void clrline(void);
void clrlinetocursor(void);
void clrcursortoendline(void);
void clrcursortoendscreen(void);
void clearlines(int x, int y, int height);
void cursoroff(void);
void cursoron(void);
void cursorbar(void);
void cursorunderline(void);
void cursorblock(void);
void cursortype(CursorType ct);
void savescreen(void);
void restorescreen(void);
void gotoxy(char x, char y);
void textbackground(Color c);
void textattr(Color bg, Color fg);
void textcolor(Color tc);
void termcolor(void);
void moveCursor(Curs curs, int col);
void moveCursorCol(int col);
void saveCursor(void);
void restoreCursor(void);
void cursorHome(void);
void cursorfwd(int n);
void cursorback(int n);
void cursorright(void);
void cursorleft(void);
void scroll_region(char top, char bottom);
void scroll_all(void);
void clrbottom(void);
void clrtop(void);
void setmode(int mode);
void screenmode(int mode);
void resetmode(int mode);
void pause(void);
void crlf(void);
void resetblink(void);
void resetunderline(void);
void resetitalic(void);
void resetbold(void);
void resetstrikethrough(void);
void resetinverse(void);
void setcolors(int foreground, int background, int attribute);
void blink(void);
void underline(void);
void italic(void);
void bold(void);
void strikethrough(void);
void resetall(void);
void scrollscreen(void);
void scrolldown(void);
void scrollup(void);
void scroll(int deb, int fin);
void centermsg(int y, const char * msg, Color bg, Color fg);

int editorReadKey(void);
void utf8ascii(uint8_t *s, size_t len);
uint8_t utf8ascii2(uint8_t ascii);


#endif
