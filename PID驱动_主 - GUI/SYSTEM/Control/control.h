#ifndef __CONTROL_H
#define __CONTROL_H 
#include "sys.h"	 
#include "stdlib.h"
#include <stdbool.h>
#define WM_TOUCHDOWN    0x0001
#define WM_TOUCHUP      0x0002
#define WM_TOUCHMOVE    0x0003
#define WM_TOUCHMESSAGE 0x0003
#define WM_TRACKPOSMOVE 0x0010
#define WM_COMMAND      0x1000
#define WM_TIMER        0x2000
#define SWP_NOPOS       0x0001
#define SWP_NOSIZE      0x0002
#define SW_SHOW         0x0001
#define SW_HIDE         0x0000
typedef struct {
    int size;
	char name[10];
} HFONT;
typedef struct {
    HFONT *font;
    u8 text[20];
    u16 state,num,x,y;
    u16 edgewidth,edgeheight,cwidth,cheight,width,height,laststate,setstate;
    bool havepressed,flag,show;
} HBUTTON;
typedef struct {
    u16 num,x,y,width,height,pos,range;
	u16 x0,y0;
	HBUTTON *button;
    bool flag,move,show;
} HTRACK;
typedef struct {
    u16 x,y,width,height;
	u8 text[50];
    bool flag,cursor,cursorshow,show;
} HEDIT;
typedef struct {
    u16 x,y,width,height,TabNum,TabShow,TabLastShow,num;
	u8 text[10][20];
    bool flag,show;
} HTAB;
typedef struct {
    u16 x,y,width,height,num,data_sum;
	float data[240][3];
    bool flag,show;
} HPANEL;


u16 SendMessage(void *window,u16 Amessage,u16 Wparam,u16 Lparam);
void ShowWindow(void *window, u16 show);
HFONT *CreateFont(char *name,u16 size);
HBUTTON *CreateButton(u16 x,u16 y,u16 width,u16 height,char *name,HFONT *font,u16 cmd);
void ButtonDown(HBUTTON *now,int x,int y);
void ButtonMouseMove(HBUTTON *now,int x,int y);
void ButtonUp(HBUTTON *now,int x,int y);
void SetButtonState(HBUTTON *now,int state);
void UnSetButtonState(HBUTTON *now);
HTRACK *CreateTrackBar(u16 x,u16 y,u16 width,u16 height,u16 range,u16 cmd);
void TrackButtonDown(HTRACK *now,int x,int y);
void TrackMouseMove(HTRACK *now,int x,int y);
void TrackButtonUp(HTRACK *now,int x,int y);
void SetTrackPos(HTRACK *now,u16 x);
HEDIT *CreateEdit(u16 x,u16 y,u16 width,u16 height,u8 *text);
HTAB *CreateTab(u16 x,u16 y,u16 width,u16 height,u16 num);
void AddTabText(HTAB *now, u8 *text);
void SetTabShow(HTAB *now, u16 show);
void TabButtonDown(HTAB *now,int x,int y);
HPANEL *CreatePanel(u16 x,u16 y,u16 width,u16 height,u16 num);
void UpdateWindow(void *window);
#endif
