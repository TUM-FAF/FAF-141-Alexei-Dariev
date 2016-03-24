#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <time.h>
#include <vector>
#include <string>
#include <sstream>
#include <windowsx.h>

#define ID_BUTTON 100
#define IDB_BEZIER 102
#define IDB_LINE 104
#define IDB_PEN 106
#define IDB_ELLIPSE 108
#define IDB_RECTANGLE 110
#define IDB_ERASER   112
#define HK_DRAW_ELLIPSE       500
#define HK_BLUE_COLOR         501


POINT pointPen;
void fillWithGradient(HDC hdc, HBRUSH hBrush, RECT tempRect, RECT gradientRect, int colorNr);
int random(int n)
{
    return rand() % (n+1);
}

bool bDrawLine		= false;
bool bDrawEllipse	= false;

BOOL PolyBezier(HDC hdc, CONST POINT *lppt, DWORD cPoints);



/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void Initialization(void);
void Erease(int x, int y);



/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
HINSTANCE hInstance;

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
    wincl.style = CS_HREDRAW | CS_VREDRAW;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("PW LAB3"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           1000,                 /* The programs width */
           1000,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);

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
 static HWND button, BezierTool, LineTool,Pen,EllipseTool,RectangleTool,EraserTool;
    int width;
    int xMouse, yMouse;
    HDC hdc = GetDC(hwnd);

    static POINT line, lineStart;
    static BOOL lineStarted;

    static BOOL ellipseStarted;
    static RECT ellipse;

    static BOOL rectangleStarted;
    static RECT rectangle;

    static int bezierStage = 0;
    static POINT bezierPoints[4];
    static POINT bezierPoint;
    static RECT tempRect;


    HDC hdcMem;
    BITMAP bitmap;
    HGDIOBJ oldBitmap;
    HBITMAP hbmplogo = NULL;
    hbmplogo = (HBITMAP)LoadImage(hInstance, "sad.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmplogo, sizeof(bitmap), &bitmap);
	HDC hDC;
    PAINTSTRUCT Ps;
	HBRUSH  NewBrush;
	HPEN linePen;
	HPEN linePen2;
	HPEN linePen3;
	linePen= CreatePen(PS_SOLID,2,RGB(255,255,0));
	linePen2=CreatePen(PS_DASHDOTDOT,4,RGB(255,0,255));
	linePen3=CreatePen(PS_STYLE_MASK,6,RGB(0,128,255));
	SelectObject(hdc,linePen);

    static RECT redGradientRect = {100, 480, 200, 505};
    static RECT greenGradientRect = {100,500, 200, 520};



    POINT Pt[4] = { {  20,  12 }, {  88, 246 }, { 364, 192 }, { 250,  48 } };
    PolyBezierTo(hdc,Pt,4);
    switch (message)                  /* handle the messages */
    {
     case WM_CREATE:

          BezierTool = CreateWindowEx(
                0,
                "Button",
                "Bezier",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 50,
                100, 20,
                hwnd,
                (HMENU)IDB_BEZIER,
                hInstance,
                NULL);

                LineTool = CreateWindowEx(
                0,
                "Button",
                "Line",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 75,
                100, 20,
                hwnd,
                (HMENU)IDB_LINE,
                hInstance,
                NULL);
               Pen = CreateWindowEx(
                0,
                "Button",
                "Pen",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 100,
                100, 20,
                hwnd,
                (HMENU)IDB_PEN,
                hInstance,
                NULL);
                EllipseTool = CreateWindowEx(
                0,
                "Button",
                "Ellipse",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 125,
                100, 20,
                hwnd,
                (HMENU)IDB_ELLIPSE,
                hInstance,
                NULL);
                RectangleTool = CreateWindowEx(
                0,
                "Button",
                "Rectangle",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 150,
                100, 20,
                hwnd,
                (HMENU)IDB_RECTANGLE,
                hInstance,
                NULL);

                EraserTool = CreateWindowEx(
                0,
                "Button",
                "Eraser",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 175,
                100, 20,
                hwnd,
                (HMENU)IDB_ERASER,
                hInstance,
                NULL);



          hbmplogo = (HBITMAP)LoadImage(hInstance, "sad.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

            return 0;

      case WM_MOUSEMOVE:



            xMouse = GET_X_LPARAM(lParam);
            yMouse = GET_Y_LPARAM(lParam);

             if (xMouse > 250 && xMouse < 880 && yMouse > 20 && yMouse < 550 )
            {
                if(wParam & MK_LBUTTON)
                {
                    if (Button_GetCheck(Pen) == BST_CHECKED)
                    {
                        width = 2;
                        linePen = CreatePen(PS_SOLID, width, RGB(0, 0, 0));
                        SelectObject(hdc, linePen);
                        MoveToEx(hdc, xMouse, yMouse, NULL);
                        LineTo(hdc, pointPen.x, pointPen.y);
                        DeleteObject(linePen);
                        pointPen.x = xMouse;
                        pointPen.y = yMouse;
                    }

                    if (Button_GetCheck(LineTool) == BST_CHECKED)
                    {
                        width = 2;

                        // draw previous line with white
                        linePen = CreatePen(PS_SOLID, width, RGB(255, 255, 255));
                        SelectObject(hdc, linePen);
                        MoveToEx(hdc, lineStart.x, lineStart.y, NULL);
                        LineTo(hdc, line.x, line.y);
                        DeleteObject(linePen);


                        // draw the line
                        linePen = CreatePen(PS_SOLID, width, RGB(0, 0, 0));
                        SelectObject(hdc, linePen);
                        MoveToEx(hdc, lineStart.x, lineStart.y, NULL);
                        LineTo(hdc, xMouse, yMouse);
                        DeleteObject(linePen);

                        line.x = xMouse;
                        line.y = yMouse;
                    }

                    if((wParam == MK_LBUTTON) && (Button_GetCheck(EraserTool) == BST_CHECKED))
                    {
                        width = 6;

                        rectangle.left = xMouse - (width / 2);
                        rectangle.right = xMouse + (width / 2);
                        rectangle.top = yMouse - (width / 2);
                        rectangle.bottom = yMouse + (width / 2);
                        InvalidateRect(hwnd, &rectangle, FALSE);
                        SendMessage(hwnd, WM_PAINT, 0, 0);
                    }
                }


            }
            break;

      case WM_LBUTTONUP :
            xMouse = GET_X_LPARAM(lParam);
            yMouse = GET_Y_LPARAM(lParam);
            if (xMouse > 250 && xMouse < 880 && yMouse > 20 && yMouse < 550 ) {
                if (Button_GetCheck(LineTool) == BST_CHECKED)
                {
                    width = 2;


                    SelectObject(hdc, CreatePen(PS_SOLID, width, RGB(0, 0, 0)));
                    MoveToEx(hdc, lineStart.x, lineStart.y, NULL);
                    LineTo(hdc, line.x, line.y);



                    linePen = CreatePen(PS_SOLID, width, RGB(0, 0, 0));
                    SelectObject(hdc, linePen);
                    MoveToEx(hdc, lineStart.x, lineStart.y, NULL);
                    LineTo(hdc, xMouse, yMouse);
                    DeleteObject(linePen);

                    line.x = xMouse;
                    line.y = yMouse;
                }
                if(ellipseStarted)
                {
                    SelectObject(hdc, linePen2);
                    SelectObject(hdc, NewBrush);

                    Ellipse(hdc, ellipse.left, ellipse.top, xMouse, yMouse);
                    DeleteObject(linePen2);
                    DeleteObject(NewBrush);

                    ellipseStarted = false;
                }

                if(rectangleStarted)
                {
                    SelectObject(hdc, linePen);
                    SelectObject(hdc, NewBrush);
                    Rectangle(hdc, rectangle.left, rectangle.top, xMouse, yMouse);

                    DeleteObject(linePen);
                    DeleteObject(NewBrush);

                    rectangleStarted = false;
                }

                if(bezierStage == 1)
                {
                    bezierPoint.x = xMouse;
                    bezierPoint.y = yMouse;
                    bezierPoints[1] = bezierPoint;
                    bezierStage = 2;
                }

                if(bezierStage == 3)
                {
                    bezierPoint.x = xMouse;
                    bezierPoint.y = yMouse;
                    bezierPoints[3] = bezierPoint;
                    bezierStage = 0;
                    SelectObject(hdc, linePen3);
                    PolyBezier(hdc, bezierPoints, 4);
                    DeleteObject(linePen3);
                }
            }

        break;


        case WM_LBUTTONDOWN:
            xMouse = GET_X_LPARAM(lParam);
            yMouse = GET_Y_LPARAM(lParam);


            if (xMouse > 250 && xMouse < 880 && yMouse > 20 && yMouse < 550 )
            {
                if(wParam == MK_LBUTTON)
                {
                    if (Button_GetCheck(Pen) == BST_CHECKED)
                    {
                        pointPen.x = xMouse;
                        pointPen.y = yMouse;
                    }
                    if (Button_GetCheck(LineTool) == BST_CHECKED)
                    {
                        lineStart.x = xMouse;
                        lineStart.y = yMouse;
                        line.x = xMouse;
                        line.y = yMouse;
                    }
                    if((wParam == MK_LBUTTON) && (Button_GetCheck(EllipseTool) == BST_CHECKED))
                    {
                        ellipse.left = xMouse;
                        ellipse.top = yMouse;
                        ellipseStarted = true;

                    }

                    if((wParam == MK_LBUTTON) && (Button_GetCheck(RectangleTool) == BST_CHECKED))
                    {
                        rectangle.left = xMouse;
                        rectangle.top = yMouse;
                        rectangleStarted = true;
                    }

                    if((wParam == MK_LBUTTON) && (Button_GetCheck(BezierTool) == BST_CHECKED))
                    {
                        if(bezierStage == 0)
                        {
                            bezierPoint.x = xMouse;
                            bezierPoint.y = yMouse;
                            bezierPoints[0] = bezierPoint;
                            bezierStage = 1;
                        }
                        else
                        {
                            bezierPoint.x = xMouse;
                            bezierPoint.y = yMouse;
                            bezierPoints[2] = bezierPoint;
                            bezierStage = 3;
                        }
                    }



                }
            }
            break;


    case WM_PAINT:

         hDC = BeginPaint(hwnd, &Ps);
         HBRUSH hBrush;

        NewBrush = CreateSolidBrush(RGB(255, 255, 255));

        SelectObject(hDC, NewBrush);
        Rectangle(hDC, 250, 20, 880, 550);
        DeleteObject(NewBrush);

        tempRect.top = redGradientRect.top;
        tempRect.bottom = redGradientRect.bottom;
        fillWithGradient(hdc, hBrush, tempRect, redGradientRect, 0);

        tempRect.top = greenGradientRect.top;
        tempRect.bottom = greenGradientRect.bottom;
        fillWithGradient(hdc, hBrush, tempRect, greenGradientRect, 1);


            hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hbmplogo);


            BitBlt(hdc,
                170,
                550,
                bitmap.bmWidth, bitmap.bmHeight,
                hdcMem, 0, 0, SRCCOPY);


            DeleteDC(hdcMem);

		EndPaint(hwnd, &Ps);

		break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }


    return 0;

}

    void fillWithGradient(HDC hdc, HBRUSH hBrush, RECT tempRect, RECT gradientRect, int colorNr)
{
    for(int i = 0; i < (gradientRect.right - gradientRect.left); i++) {
        int color;
        color = i * 255 / (gradientRect.right - gradientRect.left);
        tempRect.left  = gradientRect.left  + i;
        tempRect.right = gradientRect.left + i + 1;
        if (colorNr == 0)
            hBrush = CreateSolidBrush(RGB(color, 0, 0));
        else if (colorNr == 1)
            hBrush = CreateSolidBrush(RGB(0, color, 0));
        else
            hBrush = CreateSolidBrush(RGB(0, 0, color));
        FillRect(hdc, &tempRect, hBrush);
        DeleteObject(hBrush);
    }
}



