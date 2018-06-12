#include "resource.h"

UINT WM_TASKBAR = 0;
HWND Hwnd;
HMENU Hmenu;
NOTIFYICONDATA notifyIconData;
TCHAR szTIP[64] = TEXT("Night Light");
char szClassName[ ] = "Night Light";

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void InitNotifyIconData();
void ChangeColorTemp(int,int);
void SetColorTempBasedOnTime();

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

    // Check local time every 5 min and set color temperature accordingly
    // Default settings:
    // Day: 4200K
    // Evening: 3400K
    // Night: 2300K
    SetTimer(Hwnd, IDT_TIMER1, 300000, (TIMERPROC) NULL);
    SetColorTempBasedOnTime();

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
        AppendMenu(Hmenu, MF_STRING, ID_TRAY_EXIT,  TEXT("Exit"));
        break;

    case WM_SYSICON:{
        if (lParam == WM_RBUTTONDOWN){
            POINT curPoint;
            GetCursorPos(&curPoint);
			SetForegroundWindow(hwnd);
            UINT clicked = TrackPopupMenu(Hmenu,TPM_RETURNCMD | TPM_NONOTIFY,curPoint.x,curPoint.y,0,hwnd,NULL);
            SendMessage(hwnd, WM_NULL, 0, 0);

            if (clicked == ID_TRAY_EXIT){
                Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
                ChangeColorTemp(249,253);
                DestroyWindow(hwnd);
            }
        }
    }
    break;

    case WM_TIMER:
    switch (wParam)
    {
        case IDT_TIMER1:
            SetColorTempBasedOnTime();
            return 0;
    }

    case WM_NCHITTEST:{
       UINT uHitTest = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
        if(uHitTest == HTCLIENT)
            return HTCAPTION;
        else
            return uHitTest;
    }
    case WM_DESTROY:
        KillTimer(hwnd, IDT_TIMER1);
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

void SetColorTempBasedOnTime()
{
    SYSTEMTIME lt;
    GetLocalTime(&lt);
    if(lt.wHour < 8 || lt.wHour > 21)
        ChangeColorTemp(152,54); // 2300K
    else if(lt.wHour > 8 && lt.wHour < 18)
        ChangeColorTemp(213,173); // 4200K
    else
        ChangeColorTemp(193,132); // 3400K
}
