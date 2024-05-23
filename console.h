#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include<windows.h>

void setWindowSize(SHORT width, SHORT height);
void SetScreenBufferSize(SHORT width, SHORT height);
void ShowScrollbar(BOOL Show);
void hideConsoleButton();
void DisableResizeWindow();
void initConsole();
void header();


#endif // CONSOLE_H_INCLUDED
