/*
 Title:		cpmconio.c
 Author:	Badre
 Created:	23/06/2024
 Last Updated: 24/07/2024

 Modinfo:
 24/07/2024   Modify code from editorKey
*/
#include <cpm.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "global.h"
#include "cpmconio.h"


int use_aux = false; /* Set by command line parser in main(). */

/* BIOS functions for send/receive via the console device. */
#define CONST 2
#define CONIN 3
#define CONOUT 4

/* BIOS functions for send/receive via the auxillary device. */
#define AUXOUT  6
#define AUXIN   7
#define AUXIST 18

#define MAX_X 80
#define MAX_Y 25
#define tohex(x) ((x) < 10 ? (x) + 48 : (x) + 87)

unsigned char mask8bit = 0x7f;
const char *getForeAnsiColor(Color c);
const char *getBackAnsiColor(Color c);

/* Use the console by default. */
int bios_char_avail = CONST;
int bios_write_char = CONOUT;
int bios_read_char  = CONIN;

void setup_devices(void) {
    bios_char_avail = use_aux ? AUXIST : CONST;
    bios_write_char = use_aux ? AUXOUT : CONOUT;
    bios_read_char  = use_aux ? AUXIN  : CONIN;
}

int console_char_available(void) { return bios(bios_char_avail, 0, 0) != 0; }
void write_console(int ch) { bios(bios_write_char, ch, 0); }
int read_console(void) { return bios(bios_read_char, 0, 0); }
//int read_console(void) { return bdos(BIOS_CONIN, 0); }
//extern int read_console(void);

/*
 * read bytes as long as rdchk indicates that
 * more data is available.
 */
void rx_purge(void) {
    while (console_char_available())
        read_console();    
}

int ckbhit(void)
{
    return console_char_available();
}

uint8_t keyread = 0;

/*uint8_t ckbhit(void)
{
  uint8_t c1;

  //if we've previously read a key then return 1
  if (keyread)
    return 1;

  //let's get a key
  c1=getk() & 0xff;

  //if it is zero then return 0, no key is ready
  if (c1 == 0)
    return 0;

  //if not we do have a key, cache it in keyread and return 1
  keyread=c1;
  return 1;
}

uint8_t ckey_esc(void)
{
  while (ckbhit())
    if (cgetcar()==27)
      return 1;
  
  return 0;
}*/

unsigned short cgetcar(void)
{
  unsigned short ui1;

  //get key
  ui1=(unsigned short)cgetch();
  
  if (ui1==17) //ctrl-Q used for ^QS and ^QD
  {
    ui1=(unsigned short)(cgetch());
    if (ui1 >= 'a' && ui1 <= 'z')
      ui1-=32;
    if (ui1 >=1 && ui1 <=26)
      ui1 += 64;
    ui1 += 256;
  }
  
  return ui1;
}

/*uint8_t cgetch(void)
{
  uint8_t c1;

  //if we have a cached key, return it
  if (keyread)
    {
      c1=keyread;
      keyread=0;
      return c1;
    }

  //we must wait on a key and return it
  do
    {
      c1=getk();
    } while (c1==0);

  return c1;
}*/

void xprintf(const char *format, ...)
{
	size_t size;
	va_list ap;
	va_start(ap, format);
  char *buf = NULL;
	size = vsnprintf(NULL, 0, format, ap) + 1;
	if (size > 0)
	{
		va_end(ap);
		va_start(ap, format);
		buf = (char *)malloc((size * sizeof(char)) + 1);
		vsnprintf(buf, size, format, ap);
		cputs(buf);
	}
  if(buf)
    free(buf);
	va_end(ap);
}

int setvdpmode(int mode)
{
	cputch(22);
	cputch(mode);
	return mode;
}

void putcon(uint8_t  ch) // Puts a character
{
	uint8_t  c = utf8ascii2(ch);
	cputch(c & mask8bit);	

}

void cputs(const char *str)
{
  while (*str)
  {
    //putcon(*(str++));
    cputch(*(str++));
  }
}

void cputch(int ch)
{      
	//printf("%c", ch); 
  write_console(ch);
}

void cputchutf8(uint8_t  ch)
{
  uint8_t  c = utf8ascii2(ch);
	printf("%c", c);
}

void putsmsg(int x, int y, const char *s)
{
  gotoxy(x, y);
  cputs(s);
  cputch('\r');
  cputch('\n');
}

void  cputsxy(int x, int y, const char *s)
{
   gotoxy(x, y);
  while (*s)
  {
     cputch(*s++);
  }
}

void  blank(int width)
{
  if(width < 1 ) return;    
  for(int i = 0; i < width; i++) 
     cputch(' ');  
}

uint8_t   chready(void) // Checks if there's a character ready for input
{
  return ( ckbhit() ? 0xff : 0x00);
  
}

uint8_t   getchNB(void) // Gets a character, non-blocking, no echo
{
  return ( ckbhit() ?  cgetch() : 0x00);
}

void  bell(void)
{
   cputch(7);
}

/*void  clearInputBuffer(void)
{
  while ( ckbhit()) {
     readKbd();  
  }
  
}*/

void termFlush(void)
{
	rx_purge();
}

/*uint8_t kbhit(void)
{
  return (uint8_t)kbd_kbhit();
}*/

int  readKbd(void)
{
  return read_console();
}

uint8_t   getche(void)
{
  uint8_t  ch =  cgetch();
   cputch(ch);
  return (ch);
}

uint8_t cgetch(void)
{		
  return (uint8_t)read_console();
}

void  puthex8(uint8_t  c) // Puts a HH hex string
{
   cputch(tohex(c >> 4));
   cputch(tohex(c & 0x0f));
}

void  puthex16(uint16_t w) // puts a HHHH hex string
{
   puthex8(w >> 8);
   puthex8(w & 0x00ff);
}

void  clearline(char x, char y)
{
   gotoxy(x, y);
   clrline();
}

void  clrline(void)
{
   cputs("\033[2K");
}

void  clrlinetocursor(void)
{
   cputs("\033[1K");
}

void  clrcursortoendline(void)
{
   cputs("\033[0K");
}

void  clrcursortoendscreen(void)
{
   cputs("\033[0J");
}

void  clearlines(int x, int y, int height)
{
	int i = 0;
	for (i = 0; i < height; i++)
	{
	 gotoxy(x, y + i);
	 clreol();
	}
}

void  clreol(void)
{
   cputs("\033[K");
}

void  clrscr(void)
{
   cputs("\033[H\033[J");
}

void cursoroff(void)
{
   cputs("\033[?25l");
}

void  cursoron(void)
{
   cputs("\033[?25h");
}

void  cursorbar(void)
{
   cputs("\033[5 q");
}

void  cursorunderline(void)
{
   cputs("\033[3 q");
}

void  cursorblock(void)
{
   cputs("\033[0 q");
}

void  cursortype(CursorType ct)
{
  printf("\033[%d q", ct);
}

void  savescreen(void)
{
   cputs("\033[?47h");
}

void  restorescreen(void)
{
   cputs("\033[?47l");
}

void  gotoxy(char x, char y)
{
  if (x > MAX_X)
    x = MAX_X;
  else if (x < 1)
    x = 1;

  if (y > MAX_Y)
    y = MAX_Y;
  else if (y < 1)
    y = 1;

   cputch('\033');
   cputch('[');
   cputch((y / 10) + '0');
   cputch((y % 10) + '0');
   cputch(';');
   cputch((x / 10) + '0');
   cputch((x % 10) + '0');
   cputch('f');
}

void  termcolor(void)
{
   textattr(Black, Green);
}

void  textcolor(Color tc)
{
   cputs(getForeAnsiColor(tc));
}

void  textattr(Color bg, Color fg)
{
   textcolor(fg);
   textbackground(bg);
}

void  textbackground(Color c)
{
   cputs(getBackAnsiColor(c));
}

const char *getForeAnsiColor(Color c)
{
  switch (c)
  {
  case Black:
    return ("\033[30m");
    break;
  case Red:
    return ("\033[31m");
    break;
  case Green:
    return ("\033[32m");
    break;
  case Yellow:
    return ("\033[33m");
    break;
  case Blue:
    return ("\033[34m");
    break;
  case Magenta:
    return ("\033[35m");
    break;
  case Cyan:
    return ("\033[36m");
    break;
  case White:
    return ("\033[37m");
    break;
  case BrightBlack:
    return ("\033[90m");
    break;
  case BrightRed:
    return ("\033[91m");
    break;
  case BrightGreen:
    return ("\033[92m");
    break;
  case BrightYellow:
    return ("\033[93m");
    break;
  case BrightBlue:
    return ("\033[94m");
    break;
  case BrightMagenta:
    return ("\033[95m");
    break;
  case BrightCyan:
    return ("\033[96m");
    break;
  case BrightWhite:
    return ("\033[97m");
    break;
  case Lightgray:
    return ("\033[38;2;211;211;211m");  //ESC[38;2;{r};{g};{b}m
    break;
  case Orange:
    return ("\033[38;2;265;165;0m");  //ESC[38;2;{r};{g};{b}m
    break;
  case DarkYellow:
    return ("\033[38;2;204;204;0m");  //ESC[38;2;{r};{g};{b}m
    break;
  case DarkBlue:
    return ("\033[38;2;0;0;139m");  //ESC[38;2;{r};{g};{b}m
    break;
  case DarkRed:
    return ("\033[38;2;139;0;0m");  //ESC[38;2;{r};{g};{b}m
    break;
  case DarkGrey:
    return ("\033[38;2;169;169;169m");  //ESC[38;2;{r};{g};{b}m
    break;
  default:
    return ("\033[92m");
    break;
  }
}

const char *getBackAnsiColor(Color c)
{
  switch (c)
  {
  case Black:
    return ("\033[40m");
    break;
  case Red:
    return ("\033[41m");
    break;
  case Green:
    return ("\033[42m");
    break;
  case Yellow:
    return ("\033[43m");
    break;
  case Blue:
    return ("\033[44m");
    break;
  case Magenta:
    return ("\033[45m");
    break;
  case Cyan:
    return ("\033[46m");
    break;
  case White:
    return ("\033[47m");
    break;
  case BrightBlack:
    return ("\033[100m");
    break;
  case BrightRed:
    return ("\033[101m");
    break;
  case BrightGreen:
    return ("\033[102m");
    break;
  case BrightYellow:
    return ("\033[103m");
    break;
  case BrightBlue:
    return ("\033[104m");
    break;
  case BrightMagenta:
    return ("\033[105m");    
    break;
  case BrightCyan:
    return ("\033[106m");
    break;
  case BrightWhite:
    return ("\033[107m");
    break;
  case Lightgray:
    return ("\033[48;2;211;211;211m"); //ESC[48;2;{r};{g};{b}m
    break;
  case Orange:
    return ("\033[48;2;265;165;0m"); 
    break;
  case DarkYellow:
    return ("\033[48;2;204;204;0m");  
    break;
  case DarkBlue:
    return ("\033[48;2;0;0;139m");  
    break;
  case DarkRed:
    return ("\033[48;2;139;0;0m");  
    break;
  case DarkGrey:
    return ("\033[48;2;169;169;169m");  
    break;
  default:
    return ("\033[92m");
    break;
  }
}

void  moveCursor(Curs curs, int col)
{
  if (curs == RIGHT)
  {
     cursorfwd((char)col);
  }
  else if (curs == LEFT)
  {
     cursorback((char)col);
  }
  else
    return;
}

void  moveCursorCol(int col)
{
  printf("\033[%dG", col);
}

void  saveCursor(void)
{
   cputs("\033[s");
}

void  restoreCursor(void)
{
   cputs("\033[u");
}

void  cursorHome(void)
{
   cputch('\r');
}

void  cursorfwd(int n)
{
  printf("\033[%dC", n);
}

void  cursorback(int n)
{
  printf("\033[%dD", n);
}

void  cursorright(void)
{
   cputs("\033[C");
}

void  cursorleft(void)
{
   cputs("\033[D");
}

void scroll_region(char top, char bottom)
{
   cputch('\033');
   cputch('[');
   cputch(top);
   cputch(';');
   cputch(bottom);
   cputch('r');
}

void  scroll_all(void)
{
   cputch('\033');
   cputch('[');
   cputch('r');
}

void  clrbottom(void)
{
   cputch('\033');
   cputch('[');
   cputch('0');
   cputch('J');
}

void  clrtop(void)
{
   cputch('\033');
   cputch('[');
   cputch('1');
   cputch('J');
}

void  setmode(int mode)
{
   cputch('\033');
   cputch('[');
   cputch('=');

  if (mode & 0x7)
     cputch(mode + '0');
  else
  {
     cputch('1');
     cputch(((mode & 0x1f) % 10) + '0');
  }
   cputch('h');
}

void  screenmode(int mode)
{
   cputch('\033');
   cputch('[');
   cputch('=');
   cputch(mode);
   cputch('h');
}

void  resetmode(int mode)
{
   cputch('\033');
   cputch('[');
   cputch('=');

  if (mode & 0x7)
     cputch(mode + '0');
  else
  {
     cputch('1');
     cputch(((mode & 0x1f) % 10) + '0');
  }
   cputch('l');
}

bool waitMsg(const char *msg)
{
  bool ret = false;
  int ch;
   crlf();
   cputs(msg);
   crlf();
   termFlush();
  ch =  cgetch();
  if (ch == CTRL_KEY('c'))
    ret = true;
  return ret;
}

void waitKey(void)
{
  cputs("\r\nTry any key to continue ...");  
  read_console();
  
}

void  pause(void)
{
  cputs("\r\nTry any key to continue ...");  
  cgetch(); 
}

// Return
void  crlf(void)
{
   cputch('\r');
   cputch('\n');
}

void  resetblink(void)
{
   cputs("\033[25m");
}

void  resetunderline(void)
{
   cputs("\033[24m");
}

void  resetitalic(void)
{
   cputs("\033[23m");
}

void  resetbold(void)
{
   cputs("\033[22m");
}

void  resetstrikethrough(void)
{
   cputs("\033[29m");
}

void  resetinverse(void)
{
   cputs("\033[27m");
}

void  setcolors(int foreground, int background, int attribute)
{
  printf("\033[%i;%i;%im", attribute, foreground, background);  
}

void  blink(void)
{
   cputs("\033[5m");
}

void  underline(void)
{
   cputs("\033[4m");
}

void  italic(void)
{
   cputs("\033[3m");
}

void  bold(void)
{
   cputs("\033[1m");
}

void  strikethrough(void)
{
   cputs("\033[9m");
}

void  inverse(void)
{
   cputs("\033[7m");
}

void  resetall(void)
{
   cputs("\033[0m");
}

void  scrollscreen(void)
{
   cputs("\033[r");
}

void  scrolldown(void)
{
   cputs("\033[D");
}

void  scrollup(void)
{
   cputs("\033[M");
}

void  scroll(int deb, int fin)
{
  printf("\033[%d;%d", deb, fin);
}

#ifdef RUNCPM
int editorReadKey(void)
{
  uint8_t  c;  
    
  c =  readKbd(); 
  
  switch (c)
  {
    case CTRL_KEY('e'):
      return ARROW_UP;
    case CTRL_KEY('x'):
      return ARROW_DOWN;
    case CTRL_KEY('s'):
      return ARROW_LEFT;
    case CTRL_KEY('d'):
      return ARROW_RIGHT;
    case ENTER_KEY:
      return ENTER_KEY;
    case ESC_KEY:
      return ESC_KEY;
    case CTRL_KEY('w'):
      return PAGE_UP;
    case CTRL_KEY('z'):
      return PAGE_DOWN;    
    case CTRL_KEY('a'):
      return HOME_KEY;
    case CTRL_KEY('f'):
      return END_KEY; 
    case CTRL_KEY('c'):
      return CTRL_C_KEY; 
  }
  return c;  
}
#else
int editorReadKey(void)
{
  uint8_t  c;  
    
  c =  readKbd(); 
  
  switch (c)
  {
    case CTRL_KEY('e'):
      return ARROW_UP;
    case CTRL_KEY('x'):
      return ARROW_DOWN;
    case CTRL_KEY('s'):
      return ARROW_LEFT;
    case CTRL_KEY('d'):
      return ARROW_RIGHT;
    case ENTER_KEY:
      return ENTER_KEY;
    case ESC_KEY:
      return ESC_KEY;
    case CTRL_KEY('w'):
      return PAGE_UP;
    case CTRL_KEY('z'):
      return PAGE_DOWN;    
    case CTRL_KEY('a'):
      return HOME_KEY;
    case CTRL_KEY('f'):
      return END_KEY; 
    case CTRL_KEY('c'):
      return CTRL_C_KEY; 
  }
  return c;  
}

/*int editorReadKey(void)
{
  uint8_t  c;  
    
  c =  readKbd(); 
 
  if (c == '\033')
  {
    char seq[6];

    seq[0] =  readKbd();   
    seq[1] =  readKbd();
    if (seq[0] == '[')
    {
      if (seq[1] >= '0' && seq[1] <= '9')
      {
        seq[2] =  readKbd();
        if (seq[2] == '~')
        {
          switch (seq[1])
          {
          case '1':
            return HOME_KEY;
          case '2':
            return INS_KEY;
          case '3':
            return DEL_KEY;
          case '4':
            return END_KEY;
          case '5':
            return PAGE_UP;
          case '6':
            return PAGE_DOWN;
          case '7':
            return HOME_KEY;
          case '8':
            return END_KEY;
          }
        }
        else
        { // F5 to F8
          seq[3] =  readKbd();
          if (seq[3] == '~' && seq[1] == '1')
          {
            switch (seq[2])
            {
            case '5':
              return F5_KEY;
            case '7':
              return F6_KEY;
            case '8':
              return F7_KEY;
            case '9':
              return F8_KEY;
            }
          }
          else if (seq[3] == '~' && seq[1] == '2')
          { // F9 to F12
            switch (seq[2])
            {
            case '0':
              return F9_KEY;
            case '1':
              return F10_KEY;
            case '3':
              return F11_KEY;
            case '4':
              return F12_KEY;
            }
          }
        }
      }
      else
      {
        switch (seq[1])
        {
        case 'A':
          return ARROW_UP;
        case 'B':
          return ARROW_DOWN;
        case 'C':
          return ARROW_RIGHT;
        case 'D':
          return ARROW_LEFT;
        case 'H':
          return HOME_KEY;
        case 'F':
          return END_KEY;
        }
      }
    }
    else if (seq[0] == 'O')
    { // F1 to F4
      switch (seq[1])
      {
      case 'P':
        return F1_KEY;
      case 'Q':
        return F2_KEY;
      case 'R':
        return F3_KEY;
      case 'S':
        return F4_KEY;
      }
    }
    else if (seq[0] == '0')
    {
      switch (seq[1])
      {
      case 'H':
        return HOME_KEY;
      case 'F':
        return END_KEY;
      }
    }

    return '\033';
  }
  else
  {
    return c;
  }  
}*/


#endif
// ****** UTF8-Decoder: convert UTF8-string to extended ASCII *******
static uint8_t  c1; // Last character buffer

// Convert a single Character from UTF8 to Extended ASCII
// Return "0" if a byte has to be ignored
uint8_t utf8ascii2(uint8_t  ascii)
{
	uint8_t  last;
	
  if (ascii < 128) // Standard ASCII-set 0..0x7F handling
  {
    c1 = 0;
    return (ascii);
  }

  // get previous input
  last = c1; // get last char
  c1 = ascii;     // remember actual character

  switch (last) // conversion depending on first UTF8-character
  {
  case 0xC2:
    return (ascii);
    break;
  case 0xC3:
    return (ascii | 0xC0);
    break;
  case 0x82:
    if (ascii == 0xAC)
      return (0x80); // special case Euro-symbol
  }

  return (0); // otherwise: return zero, if character has to be ignored
}

// In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
void utf8ascii(uint8_t  *s, size_t len)
{
	int k = 0;
	size_t i;
	uint8_t  c;
	for (i = 0; i < len; i++)
	{
		c = utf8ascii2(s[i]);
		if (c != 0)
			s[k++] = c;
	}
	s[k] = 0;
}

void  centermsg(int y, const char *msg, Color bg, Color fg)
{
   textattr(bg, fg);
   gotoxy(1, y);
   clrline();
   gotoxy((MAX_X - (strlen(msg))) / 2, y);
   cputs(msg);
   clreol();
}
