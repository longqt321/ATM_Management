#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <iostream>
#include "console.h"

using namespace std;

void SetWindowSize(SHORT width, SHORT height)
{
    HANDLE hStdout;
    COORD coord;
    BOOL ok;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    coord.X = width;
    coord.Y = height;
    ok = SetConsoleScreenBufferSize(hStdout, coord);


    SMALL_RECT WindowSize;
    WindowSize.Top = 0;
    WindowSize.Left = 0;
    WindowSize.Right = width - 1;
    WindowSize.Bottom = height - 1;

    SetConsoleWindowInfo(hStdout, ok, &WindowSize);
}
void ShowScrollbar(BOOL Show)
{
    HWND hWnd = GetConsoleWindow();
    ShowScrollBar(hWnd, SB_BOTH, Show);
}

void hideConsoleButton() {
    HWND hwnd = GetConsoleWindow();

    if (hwnd != NULL) {
        HMENU hMenu = GetSystemMenu(hwnd, FALSE);
        if (hMenu != NULL) {
            DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
            DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
            DeleteMenu(hMenu, SC_MAXIMIZE,MF_BYCOMMAND);
        }
    }
}

void DisableResizeWindow()
{
    HWND hWnd = GetConsoleWindow();
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void initConsole(){
    ShowScrollbar(0);
    hideConsoleButton();
    SetConsoleOutputCP(65001);
    SetConsoleTitle(TEXT("ATM Management"));
    DisableResizeWindow();

    SetWindowSize(65,50);
}

void header() {
    system("cls");
    cout << " ______________________________________________________________ " << '\n';
    cout << "|                                                              |" << '\n';
    cout << "|            ĐỒ ÁN PBL1: DỰ ÁN LẬP TRÌNH TÍNH TOÁN             |" << '\n';
    cout << "|                                                              |" << '\n';
    cout << "|                ĐỀ TÀI: XÂY DỰNG ỨNG DỤNG                     |" << '\n';
    cout << "|              THỰC HIỆN GIAO DỊCH TRÊN MÁY ATM                |" << '\n';
    cout << "|                                                              |" << '\n';
    cout << "|        GVHD: Trần Hồ Thủy Tiên                               |" << '\n';
    cout << "|        Nhóm thực hiện:                                       |" << '\n';
    cout << "|              Trần Đức Long     (23T_Nhat1)  MSSV: 102230027  |" << '\n';
    cout << "|              Hoàng Vũ Tấn Phát (19TCLC_DT4) MSSV: 102190182  |" << '\n';
    cout << "|              Nguyễn Gia Khánh  (19TCLC_DT3) MSSV: 102190120  |" << '\n';
    cout << "|                                                              |" << '\n';
    cout << "|______________________________________________________________|" << '\n';
}

