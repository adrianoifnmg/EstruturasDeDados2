/*
 * gconio.h v0.6
 *
 * A replacement library for Borland C-specific conio.h functions for gcc
 * and MSVC.
 *
 * Implemented functions: clrscr, clreol, cursor, delay, gotoxy, 
 *                        textbackground, textcolor, resetcolor, flushall, 
 *                        getch, kbhit, getche, box, lineh, linev.
 *
 *-------------------------------------------------------------------------- 
 * TO INSTALL [LINUX]:
 * COPY THIS FILE TO DIR: /usr/include
 * $> sudo cp gconio.h /usr/include
 *--------------------------------------------------------------------------
 *
 * Copyright (C) 2002-2003 Wence Van der Meersch & Filip Duyck
 *
 * The MSVC versions of setcursortype, textcolor and textbackground were
 * kindly donated by Sebastien Boelpaep <sebastien@pi.be>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * History:
 * v0.6:  - Added function: kbhit(char* key). Author: MSc. Adriano Antunes Prates 
 *        - Added function: cursor(int i). Author: MSc. Adriano Antunes Prates 
 *        - Added function: resetcolor(). Author: MSc. Adriano Antunes Prates 
 *        - Rewrote function: flushall(). Author: MSc. Adriano Antunes Prates
 * v0.5:  - gconio.h should now be compatible with MSVC!
 *        - Rewrote clrscr, lineh, linev functions to evade ANSI codes.
 *        - Added wherex, wherey, setcursortype functions (MSVC only).
 * v0.2:  - Added getch, getche, strlwr, strupr functions.
 *        - Added aliases for stricmp, strnicmp.
 * v0.1:  - First release.
 *
 */

#ifndef __GCONIO_H
#define __GCONIO_H

#include <stdio.h>
#ifdef WIN32
#include <conio.h>
#include <time.h>
#include <windows.h>
#endif
#ifdef __GNUC__
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#endif

/**** DEFINITIONS **************************/
#define kader(x,y,l,h,t) box(x,y,l,h,t)
#define lijnh(x,y,l,c)   lineh(x,y,l,c)
#define lijnv(x,y,l,c)   linev(x,y,l,c)
#ifdef __GNUC__
#define stricmp          strcasecmp 
#define strnicmp         strncasecmp
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define PURPLE 5
#define BROWN 6
#define YELLOW 6
#define WHITE 7
/*#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13*/
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**** PROTOTYPES ***************************/
void clreol (void);
void clrscr (void);
void delay (unsigned long milliseconds);
int get_screen_rows (void);
int get_screen_columns (void);
void textbackground (int newcolor);
void textcolor (int newcolor);
void box (int x, int y, int length, int height, int type);
void lineh (int x, int y, int length, int character);
void linev (int x, int y, int length, int character);
void gotoxy (int x, int y);
#ifdef __GNUC__
void resetcolor (void);
void flushall (void);
int getch (void);
int kbhit(char *key);
int getche (void);
void cursor(int i);
void strlwr (char *text);
void strupr (char *text);
#endif
#ifdef WIN32
int wherex (void);
int wherey (void);
void setcursortype (int type);
#endif

/**** GLOBAL VARIABLES *********************/
#ifdef __GNUC__
int __gconio_h_ansi_attr = 0;
int __gconio_h_ansi_fg = 37;
int __gconio_h_ansi_bg = 40;
#endif
#ifdef WIN32
enum COLORS {
    BLACK        = 0,
    BLUE         = FOREGROUND_BLUE,
    GREEN        = FOREGROUND_GREEN,
    CYAN         = FOREGROUND_GREEN | FOREGROUND_BLUE,
    RED          = FOREGROUND_RED,
    MAGENTA      = FOREGROUND_RED | FOREGROUND_BLUE,
    BROWN        = FOREGROUND_RED | FOREGROUND_GREEN,
    LIGHTGRAY    = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    DARKGRAY     = FOREGROUND_INTENSITY,
    LIGHTBLUE    = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    LIGHTGREEN   = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LIGHTCYAN    = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    LIGHTRED     = FOREGROUND_RED | FOREGROUND_INTENSITY,
    LIGHTMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    YELLOW       = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    WHITE        = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
};
enum CURSOR {
    NORMALCURSOR = 0,
    NOCURSOR = 100,
    SOLIDCURSOR = 20,
};
WORD __gconio_h_ansi_fg = WHITE;
WORD __gconio_h_ansi_bg = BLACK << 4;
#endif

/**** FUNCTIONS ****************************/

void delay (unsigned long milliseconds)
{
#ifdef __GNUC__
    usleep (milliseconds * 1000);
#endif
#ifdef WIN32
    unsigned long tstart, tnow;
    milliseconds *= CLK_TCK/1000;
    tstart = clock();
    while ((tnow = clock() - tstart) < milliseconds) { }
#endif
}

#ifdef __GNUC__
void flushall (void)
{
    char ch;
    while((ch = getchar())!='\n' && ch!=EOF);
    fflush(stdout);
    fflush(stderr);
}
#endif

#ifdef __GNUC__
int getch (void)
{
    int kbdinput;
    struct termios t_orig, t_new;
    setbuf(stdin,NULL);
    // We need to change terminal settings so getchar() does't
    // require a CR at the end. Also disable local echo.
    tcgetattr(0, &t_orig);
    t_new = t_orig;
    t_new.c_lflag &= ~ICANON; t_new.c_lflag &= ~ECHO;
    t_new.c_lflag &= ~ISIG;   t_new.c_cc[VMIN] = 1;
    t_new.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &t_new);

    // Get actual input
    kbdinput = getchar();

    // Reset terminal settings.
    tcsetattr(0, TCSANOW, &t_orig);

    return (kbdinput);  
}
#endif


#ifdef __GNUC__
int kbhit(char *key){
	struct termios initial_settings, new_settings;
	int peek_character;
	
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
    peek_character=-1;

    unsigned char ch;
    int nread;
    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);

    if (nread == 1){
        *key = ch;
        return 1;
    }
    return 0;
}
#endif



#ifdef __GNUC__
int getche (void)
{
    int kbdinput;
    kbdinput = getch();
    putchar (kbdinput);
    return (kbdinput);
}
#endif

#ifdef __GNUC__
void strlwr (char *text)
{
    while (*text != '\0')
    {
      if ((*text > 64) && (*text < 91)) *text += 32;
      text++;
    }
}
#endif

#ifdef __GNUC__
void strupr (char *text)
{
    while (*text != '\0')
    {
      if ((*text > 96) && (*text < 123)) *text -= 32;
      text++;
    }
}
#endif

#ifdef __GNUC__
int get_win_size (win) struct winsize *win;
{
    int err = ioctl (1, TIOCGWINSZ, (char *) win);
    if (err != 0) err = ioctl (0, TIOCGWINSZ, (char *) win);
    return (err);
}
#endif

int get_screen_columns (void)
{
#ifdef __GNUC__
#ifdef TIOCGWINSZ
    struct winsize win;

    /* In older versions of Solaris, ioctl will fail and set errno
       to EINVAL when using it in a telnet session. We are not going to
       support this, as these versions of Solaris (2.1 - 2.3) are barely
       used anymore, and telnet sessions should not be used. -FD */

    if (!get_win_size (&win))
        if (win.ws_col > 0)
            return win.ws_col;
#endif

    if (getenv ("COLUMNS"))
        return ((unsigned int) atoi (getenv ("COLUMNS")));

    return 80; /* return default */
#endif
#ifdef WIN32
    CONSOLE_SCREEN_BUFFER_INFO conScrBufInfo;
    if (GetConsoleScreenBufferInfo (GetStdHandle(STD_OUTPUT_HANDLE), &conScrBufInfo))
        return ((unsigned int) conScrBufInfo.dwSize.X);
    else
        return (80);
#endif
}

int get_screen_rows (void)
{
#ifdef __GNUC__
#ifdef TIOCGWINSZ
    struct winsize win;

    /* In older versions of Solaris, ioctl will fail and set errno
       to EINVAL when using it in a telnet session. We are not going to
       support this, as these versions of Solaris (2.1 - 2.3) are barely
       used anymore, and telnet sessions should not be used. -FD */

    if (!get_win_size (&win))
        if (win.ws_row > 0)
            return win.ws_row;
#endif

    if (getenv ("LINES"))
        return ((unsigned int) atoi(getenv ("LINES")));

    return 25; /* return default */
#endif
#ifdef WIN32
    CONSOLE_SCREEN_BUFFER_INFO conScrBufInfo;
    if (GetConsoleScreenBufferInfo (GetStdHandle(STD_OUTPUT_HANDLE), &conScrBufInfo))
        return ((unsigned int) (conScrBufInfo.srWindow.Bottom - conScrBufInfo.srWindow.Top + 1));
    else
        return (25);
#endif
}

#ifdef __GNUC__
void clrscr (void) 
{
    int count;
    for (count = 0; count < get_screen_rows(); count++)
    {
        gotoxy (1, count+1);
        printf ("\r\033[K\r");
    }
    gotoxy (1,1);
}
#endif

void clreol() 
{
#ifdef __GNUC__
    printf ("\033[2K\r");
    //printf ("\r\033[K\r");
#endif
#ifdef WIN32
    int xcount, ycur;
    ycur = wherey();
    gotoxy (0, ycur);
    for (xcount = 0; xcount < get_screen_columns(); xcount++) 
        putchar (' ');
    gotoxy (0, ycur);
#endif
}

void gotoxy (int x, int y)
{
#ifdef __GNUC__
    if ( (x <= get_screen_columns()) && (y <= get_screen_rows()) )
        printf("\033[%d;%dH", y, x);
    else
        return -1;
#endif
#ifdef WIN32
    COORD cxy = { x, y };
    CONSOLE_SCREEN_BUFFER_INFO conScrBufInfo;
    if (GetConsoleScreenBufferInfo (GetStdHandle(STD_OUTPUT_HANDLE), &conScrBufInfo))
        cxy.Y = y + conScrBufInfo.srWindow.Top;
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), cxy);
#endif
}

void linev (int x, int y, int length, int character)
{
    int count;
    if ((y + length - 1) <= get_screen_rows())
    {
        gotoxy (x,y);
        for (count = 0; count < length; count++)
        {
            gotoxy (x, y + count); 
            putchar (character);
        }
    }
}

void lineh (int x, int y, int length, int character)
{
    int count;
    if ((x + length - 1) <= get_screen_columns())
    {
        gotoxy (x,y);
        for (count = 0; count < length; count++) 
            putchar (character);
    }
}

void box (int x, int y, int length, int height, int type)
{

    char charset[10];

    if ( (x<0) || (y<0) || (length<0) || (height<0) || 
         ((x+length)>get_screen_columns()) || ((y+height)>get_screen_rows()))
        return;
    else {
        switch (type)
        {

 #ifdef __GNUC__
            case 1: sprintf (charset, "++++-|"); break;
            case 2: sprintf (charset, "++++=|"); break;
            default: sprintf (charset, "++++-|"); break;
#endif
#ifdef WIN32
            case 1: sprintf (charset, "%c%c%c%c%c%c", 218, 192, 191, 217, 196, 179);
                    break;
            case 2: sprintf (charset, "%c%c%c%c%c%c", 201, 200, 187, 188, 205, 186);
                    break;
            default: sprintf (charset, "%c%c%c%c%c%c", 218, 192, 191, 217, '-', '|');
#endif
        }

        lineh (x, y, length, charset[4]);
        lineh (x, y + height - 1, length , charset[4]);
        linev (x, y, height, charset[5]);
        linev (x + length - 1, y, height, charset[5]);

        gotoxy (x, y);                           putchar (charset[0]);
        gotoxy (x, y + height - 1);              putchar (charset[1]);
        gotoxy (x + length - 1, y);              putchar (charset[2]);
        gotoxy (x + length - 1, y + height - 1); putchar (charset[3]);

        gotoxy(0, y + height + 2);
    }

}

#ifdef __GNUC__
void cursor(int i){
    if(i)
        printf("\e[?25h");
    else
        printf("\e[?25l");
}
#endif

void textcolor (int newcolor)
{
#ifdef __GNUC__
    if (newcolor > 7) { __gconio_h_ansi_attr = 1; newcolor -= 8; }
    else { __gconio_h_ansi_attr = 0; }
    switch(newcolor)
    {
        case 0:  __gconio_h_ansi_fg = 30; break; // black
        case 1:  __gconio_h_ansi_fg = 34; break; // blue
        case 2:  __gconio_h_ansi_fg = 32; break; // green
        case 3:  __gconio_h_ansi_fg = 36; break; // cyan
        case 4:  __gconio_h_ansi_fg = 31; break; // red
        case 5:  __gconio_h_ansi_fg = 35; break; // magenta
        case 6:  __gconio_h_ansi_fg = 33; break; // brown
        case 7:  __gconio_h_ansi_fg = 37; break; // gray
	default: return -1;
    }
    printf ("\033[%d;%d;%dm", __gconio_h_ansi_attr, __gconio_h_ansi_fg, __gconio_h_ansi_bg);
#endif
#ifdef WIN32
    __gconio_h_ansi_fg = newcolor;
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), (WORD) (__gconio_h_ansi_fg | __gconio_h_ansi_bg));
#endif
}

void textbackground (int newcolor)
{
#ifdef __GNUC__
    switch(newcolor)
    {
        case 0:  __gconio_h_ansi_bg = 40; break;
        case 1:  __gconio_h_ansi_bg = 44; break;
        case 2:  __gconio_h_ansi_bg = 42; break;
        case 3:  __gconio_h_ansi_bg = 46; break;
        case 4:  __gconio_h_ansi_bg = 41; break;
        case 5:  __gconio_h_ansi_bg = 45; break;
        case 6:  __gconio_h_ansi_bg = 43; break;
        case 7:  __gconio_h_ansi_bg = 47; break;
        default: return -1;
    }
    printf ("\033[%d;%d;%dm", __gconio_h_ansi_attr, __gconio_h_ansi_fg, __gconio_h_ansi_bg);
#endif
#ifdef WIN32
    __gconio_h_ansi_bg = newcolor << 4;
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), (WORD) (__gconio_h_ansi_fg | __gconio_h_ansi_bg));
#endif
}

#ifdef __GNUC__
void resetcolor ()
{
    printf ("\x1b[0m");
}
#endif

#ifdef WIN32
int wherex (void)
{
    CONSOLE_SCREEN_BUFFER_INFO conScrBufInfo;
    GetConsoleScreenBufferInfo (GetStdHandle(STD_OUTPUT_HANDLE), &conScrBufInfo);
    return conScrBufInfo.dwCursorPosition.X - conScrBufInfo.srWindow.Left;
}
#endif

#ifdef WIN32
int wherey (void)
{
    CONSOLE_SCREEN_BUFFER_INFO conScrBufInfo;
    GetConsoleScreenBufferInfo (GetStdHandle(STD_OUTPUT_HANDLE), &conScrBufInfo);
    return conScrBufInfo.dwCursorPosition.Y - conScrBufInfo.srWindow.Top;
}
#endif

void setcursortype (int type)
{
#ifdef WIN32
    CONSOLE_CURSOR_INFO conCursInfo = { type, (type != NOCURSOR) };
    SetConsoleCursorInfo (GetStdHandle(STD_OUTPUT_HANDLE), &conCursInfo);
#endif
#ifdef __GNUC__
    // not implemented
#endif
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GCONIO_H */


