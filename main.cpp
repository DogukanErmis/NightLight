#include "resource.h"
#include <stdlib.h>

UINT WM_TASKBAR = 0;
HWND Hwnd;
HMENU Hmenu;
NOTIFYICONDATA notifyIconData;
TCHAR szTIP[64] = TEXT("Night Light");
char szClassName[ ] = "Night Light";

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void InitNotifyIconData();
void ChangeColorTemp(int,int);


int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    WNDCLASSEX wincl;
    WM_TASKBAR = RegisterWindowMessageA("TaskbarCreated");
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    wincl.hIconSm = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
    if(!RegisterClassEx(&wincl)) return 0;
    Hwnd = CreateWindowEx (0, szClassName, szClassName, WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, HWND_DESKTOP, NULL, hThisInstance, NULL);
    if(!Hwnd) return 0;

    // Initialize NOTIFYICONDATA structure and show system tray icon.
    InitNotifyIconData();

    //Change Color Temperature to 2600K by default
    ChangeColorTemp(165,79);

    MSG msg;
    while (GetMessage (&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message){
    case WM_CREATE:
        Hmenu = CreatePopupMenu();
        AppendMenu(Hmenu, MF_STRING, ID_TRAY_1900K,  TEXT("1900K: Candle"));
        AppendMenu(Hmenu, MF_STRING, ID_TRAY_2300K,  TEXT("2300K: Dim Incandescent"));
        AppendMenu(Hmenu, MF_STRING, ID_TRAY_2600K,  TEXT("2600K: Incandescent"));
        AppendMenu(Hmenu, MF_STRING, ID_TRAY_3400K,  TEXT("3400K: Halogen"));
        AppendMenu(Hmenu, MF_STRING, ID_TRAY_4200K,  TEXT("4200K: Fluorescent"));
        AppendMenu(Hmenu, MF_STRING, ID_TRAY_5500K,  TEXT("5500K: Sunlight"));
        AppendMenu(Hmenu, MF_STRING, ID_TRAY_6500K,  TEXT("6500K: Daylight"));
        AppendMenu(Hmenu, MF_STRING, ID_TRAY_EXIT,  TEXT("Exit"));
        break;

    case WM_SYSICON:{
        if (lParam == WM_RBUTTONDOWN){
            POINT curPoint;
            GetCursorPos(&curPoint);
			SetForegroundWindow(Hwnd);
            UINT clicked = TrackPopupMenu(Hmenu,TPM_RETURNCMD | TPM_NONOTIFY,curPoint.x,curPoint.y,0,hwnd,NULL);
            SendMessage(hwnd, WM_NULL, 0, 0);

            if (clicked == ID_TRAY_EXIT){
                Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
                ChangeColorTemp(249,253);
                DestroyWindow(hwnd);
            }
            else if (clicked == ID_TRAY_6500K)
                ChangeColorTemp(249,253);
            else if (clicked == ID_TRAY_5500K)
                ChangeColorTemp(236,224);
            else if (clicked == ID_TRAY_4200K)
                ChangeColorTemp(213,173);
            else if (clicked == ID_TRAY_3400K)
                ChangeColorTemp(193,132);
            else if (clicked == ID_TRAY_2600K)
                ChangeColorTemp(165,79);
            else if (clicked == ID_TRAY_2300K)
                ChangeColorTemp(152,54);
            else if (clicked == ID_TRAY_1900K)
                ChangeColorTemp(131,0);
        }
    }
    break;

    case WM_NCHITTEST:{
       UINT uHitTest = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
        if(uHitTest == HTCLIENT)
            return HTCAPTION;
        else
            return uHitTest;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
        break;
    }
    return 0;
}

void InitNotifyIconData()
{
    memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));
    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    notifyIconData.hWnd = Hwnd;
    notifyIconData.uID = ID_TRAY_APP_ICON;
    notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    notifyIconData.uCallbackMessage = WM_SYSICON;
    notifyIconData.hIcon = (HICON)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    strncpy(notifyIconData.szTip, szTIP, sizeof(szTIP));
    Shell_NotifyIcon(NIM_ADD, &notifyIconData);
}

void ChangeColorTemp(int green, int blue)
{
    WORD  GammaArray[3][256];
    HDC   hGammaDC = ::GetDC(NULL);

    ::GetDeviceGammaRamp (hGammaDC, GammaArray);

    for (int i = 0; i < 256; i++){
        GammaArray[0][i] = (WORD)i*255;
        GammaArray[1][i] = (WORD)i*green;
        GammaArray[2][i] = (WORD)i*blue;
    }
    ::SetDeviceGammaRamp (hGammaDC, GammaArray);
    Sleep (100);
    ::ReleaseDC(NULL, hGammaDC);
}
