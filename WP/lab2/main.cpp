#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "resource.h"
#include <string.h>
#include <stdio.h>
bool gbDrawLine     = false;
bool gbDrawEllipse  = false;

BOOL RegClass(WNDPROC, LPCTSTR, UINT);
HINSTANCE hInst;



INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

char *string="Set Color"; //текст в окне
char *string1="Your Color: ";
char *string2="HEXcode of a color:";

char Buf[]="AAAAAA"; //буфер для 16-ричных значений

int i=0;
int red=128, green=128, blue=128; //значения цветов
/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);
    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
    wincl.hIconSm = (HICON)LoadImage (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0);
    wincl.hCursor = LoadCursor (hThisInstance, MAKEINTRESOURCE(IDI_MYCURSOR));
    wincl.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Windows Programming Lab2 420 m8"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmld, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    HBRUSH hBrush;
    RECT Rect, Client;
    static HWND hRedScroll, hGreenScroll, hBlueScroll;
    static HWND hEditRed, hEditGreen, hEditBlue;
    static HWND hEditHex;
    static HWND hwndListBox, hwndAddButton, hwndNew, hwndRemove;

    static int nxPos, nxMin, nxMax, nyPos, nyMin, nyMax;
    LRESULT textsize;
    char storedata[20];

    switch (message)                  /* handle the messages */
   {

  case WM_CREATE:
        {
            HMENU hMenu, hSubMenu;

            hMenu = CreateMenu();

            hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit    Ctrl+E");
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

            hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING, ID_STUFF_GO, "&Info");
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&About");

            hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING, IDM_LINE, "&Draw line   Ctrl+L");
            AppendMenu(hSubMenu, MF_STRING, IDM_ELLIPSE, "&Draw ellipse    Ctrl+F");
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Draw");

            SetMenu(hwnd, hMenu);

            RegisterHotKey(hwnd, HK_EXIT, MOD_CONTROL, 0X45);
            RegisterHotKey(hwnd, HK_DRAWLINE, MOD_CONTROL, 0X4C);
            RegisterHotKey(hwnd, HK_DRAWELLIPSE, MOD_CONTROL, 0X46);

            nxPos=128;
            nxMin=0;
            nxMax=255;

            hRedScroll=CreateWindow("scrollbar", NULL,
                                    WS_CHILD|WS_VISIBLE|SBS_HORZ, 20, 60, 200, 25,
                                    hwnd,(HMENU)-1, hInst, NULL);

            hGreenScroll=CreateWindow("scrollbar", NULL,
                                    WS_CHILD|WS_VISIBLE|SBS_HORZ, 20, 160, 200, 25,
                                    hwnd,(HMENU)-1, hInst, NULL);
            hBlueScroll=CreateWindow("scrollbar", NULL,
                                     WS_CHILD|WS_VISIBLE|SBS_HORZ, 20, 260, 200, 25,
                                     hwnd,(HMENU)-1, hInst, NULL);

            SetScrollRange(hRedScroll, SB_CTL, nxMin, nxMax, TRUE);
            SetScrollRange(hGreenScroll, SB_CTL, nxMin, nxMax, TRUE);
            SetScrollRange(hBlueScroll, SB_CTL, nxMin, nxMax, TRUE);

            SetScrollPos(hRedScroll, SB_CTL, nxPos, TRUE);
            SetScrollPos(hGreenScroll, SB_CTL, nxPos, TRUE);
            SetScrollPos(hBlueScroll, SB_CTL, nxPos, TRUE);

            hEditRed=CreateWindowEx(WS_EX_STATICEDGE, "edit", "127",
                                    WS_CHILD|WS_VISIBLE|WS_BORDER, 240, 60, 30, 25,
                                    hwnd,(HMENU)ID_EDITRED, hInst, NULL);

            hEditGreen=CreateWindowEx(WS_EX_STATICEDGE, "edit", "127",
                                      WS_CHILD|WS_VISIBLE|WS_BORDER, 240, 160, 30, 25,
                                      hwnd,(HMENU)ID_EDITGREEN, hInst, NULL);

            hEditBlue=CreateWindowEx(WS_EX_STATICEDGE, "edit", "127",
                                     WS_CHILD|WS_VISIBLE|WS_BORDER, 240, 260, 30, 25,
                                     hwnd,(HMENU)ID_EDITBLUE, hInst, NULL);

            hEditHex=CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "7F7F7F",
                                    WS_CHILD|WS_VISIBLE|WS_BORDER, 380, 260, 60, 25,
                                    hwnd,(HMENU)ID_EDITHEX, hInst, NULL);

            hwndListBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Listbox"), TEXT(""),
                                         WS_VISIBLE | WS_CHILD | LBS_NOTIFY, 50, 300, 150, 200,
                                         hwnd, (HMENU) IDM_LISTBOX, GetModuleHandle(NULL), NULL);

            hwndAddButton = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Add string"),
                                           WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 350, 380, 100, 30,
                                           hwnd, (HMENU) IDM_ADD, GetModuleHandle(NULL), NULL);

            hwndNew = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(""),
                                        WS_VISIBLE | WS_CHILD,
                                        300, 320, 180, 50,
                                        hwnd, (HMENU) IDM_NEW, GetModuleHandle(NULL), NULL);



        }
        break;

        case WM_HSCROLL:
            if(hRedScroll==(HWND)lParam) i=0;
            if(hGreenScroll==(HWND)lParam) i=1;
            if(hBlueScroll==(HWND)lParam) i=2;

            switch(LOWORD(wParam)) {

            case SB_PAGERIGHT:
                nxPos+=10;
                break;

            case SB_LINERIGHT:
                     nxPos+=1;
                     break;

            case SB_PAGELEFT:
                   nxPos-=10;
                   break;

            case SB_LINELEFT:
                    nxPos-=1;
                    break;

            case SB_TOP:
                  nxPos=nxMax;
                  break;

            case SB_BOTTOM:
                nxPos=nxMax;

                break;

             case SB_THUMBPOSITION:
             case SB_THUMBTRACK:
                 nxPos=HIWORD(wParam);
                 break;

                 default: break;
                 }

                 if(nxPos>nxMax)
                    {
                    nxPos=nxMax;

                    }
                    if(nxPos<nxMin)
                        { nxPos=nxMin;

                    }

                  if(nxPos==nxMax)

                       switch(i) {
                                  case 0: EnableScrollBar(hRedScroll, SB_CTL, ESB_DISABLE_RIGHT); break;
                                  case 1: EnableScrollBar(hRedScroll, SB_CTL, ESB_DISABLE_RIGHT); break;
                                  case 2: EnableScrollBar(hRedScroll, SB_CTL, ESB_DISABLE_RIGHT); break;
                                  }

                        switch(i) {
                            case 0:
                                SetScrollPos(hRedScroll, SB_CTL, nxPos, TRUE);
                                SetDlgItemInt(hwnd, ID_EDITRED, nxPos,0);
                                break;

                            case 1:
                                SetScrollPos(hGreenScroll, SB_CTL, nxPos, TRUE);
                                SetDlgItemInt(hwnd, ID_EDITGREEN,nxPos,0);
                                break;

                                case 2:
                                    SetScrollPos(hBlueScroll, SB_CTL, nxPos, TRUE);
                                    SetDlgItemInt(hwnd, ID_EDITBLUE,nxPos,0);
                                    break;
                                    }

                red=GetDlgItemInt(hwnd, ID_EDITRED, NULL, 0);
                green=GetDlgItemInt(hwnd, ID_EDITGREEN, NULL, 0);
                blue=GetDlgItemInt(hwnd, ID_EDITBLUE, NULL, 0);

                Client.left=330;
                Client.top=60;
                Client.right=480;
                Client.bottom=220;

                InvalidateRect(hwnd, &Client, 1);
                sprintf(Buf, "%X%X%X", red, green ,blue);
                SetDlgItemText(hwnd, ID_EDITHEX,Buf);



        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case ID_FILE_EXIT:
                    PostMessage(hwnd, WM_CLOSE, 0, 0);

                break;
                case ID_STUFF_GO:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);

                break;
                case IDM_LINE:
                    gbDrawLine = !gbDrawLine;
                    InvalidateRect(hwnd, 0, TRUE);
                    break;
                case IDM_ELLIPSE:
                    gbDrawEllipse = !gbDrawEllipse;
                    InvalidateRect(hwnd, 0, TRUE);
                    break;

                case IDM_ADD:
                    textsize = SendMessage(hwndNew, WM_GETTEXT, 100, (LPARAM)storedata);
                    storedata[textsize]=_T('\0');
                    SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)storedata);
                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    SendMessage(hwndNew, WM_SETTEXT, NULL, NULL);
                    break;

                case IDM_REMOVE:
                    if ((HWND)wParam == hwndListBox)
                    {
                        int index = SendMessage(GetDlgItem(hwnd, IDM_LISTBOX), LB_GETCURSEL, 0 ,0);
                        SendMessage(GetDlgItem(hwnd, IDM_LISTBOX), LB_DELETESTRING, index, 0);

                    }
                    break;

            }
        break;

                case WM_CONTEXTMENU:
                    {
                        if ((HWND)wParam == hwndListBox)
                        {
                            if(MessageBox(hwnd, "Delete string?", "REKTSTRING?", MB_YESNO) == IDYES )
                        {
                            int index = SendMessage(GetDlgItem(hwnd, IDM_LISTBOX), LB_GETCURSEL, 0 ,0);
                        SendMessage(GetDlgItem(hwnd, IDM_LISTBOX), LB_DELETESTRING, index, 0);
                        }
                    }
                    }
                    break;

                case WM_HOTKEY:
                    {
                        switch(wParam)
                        {
                        case HK_EXIT:
                            PostQuitMessage(0);
                            break;

                        case HK_DRAWLINE:
                            gbDrawLine = !gbDrawLine;
                            InvalidateRect(hwnd, 0, TRUE);
                            break;

                        case HK_DRAWELLIPSE:
                             gbDrawEllipse = !gbDrawEllipse;
                             InvalidateRect(hwnd, 0, TRUE);
                             break;
                        }
                    }
                case WM_PAINT:


                    hdc = BeginPaint(hwnd, &ps);

                    hBrush=CreateSolidBrush(RGB(red, green, blue));
                    SelectObject(hdc, hBrush);
                    Ellipse(hdc, 350,80,460,200);
                    TextOut(hdc, 60,20,string, strlen(string));
                    TextOut(hdc, 370,20,string1, strlen(string1));
                    TextOut(hdc, 325, 234, string2, strlen(string2));
                    SetTextColor(hdc, RGB(255,0,0));
                    TextOut(hdc, 240,42,"Red:",4);
                    SetTextColor(hdc, RGB(0,255,0));
                    TextOut(hdc, 234,140,"Green:", 6);
                    SetTextColor(hdc, RGB(0,0,255));
                    TextOut(hdc, 240,240,"Blue:", 5);

                    HPEN hPenOld;
                    if (gbDrawLine) {

                            HPEN hLinePen;
                            COLORREF qLineColor;
                            qLineColor = RGB(255, 0, 0);
                            hLinePen = CreatePen(PS_SOLID, 7, qLineColor);
                            hPenOld = (HPEN)SelectObject(hdc, hLinePen);

                            MoveToEx(hdc, 100, 100, NULL);
                            LineTo(hdc, 500, 250);

                            SelectObject(hdc, hPenOld);
                            DeleteObject(hLinePen);
                            }

                   if (gbDrawEllipse) {
                        HPEN hEllipsePen;
                        COLORREF qEllipseColor;
                        qEllipseColor = RGB(0, 0, 255);
                        hEllipsePen = CreatePen(PS_SOLID, 3, qEllipseColor);
                        hPenOld = (HPEN)SelectObject(hdc, hEllipsePen);

                        Arc(hdc, 100, 100, 500, 250, 0, 0, 0, 0);

                        SelectObject(hdc, hPenOld);
                        DeleteObject(hEllipsePen);
                        }
                        EndPaint(hwnd, &ps);
                        break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
