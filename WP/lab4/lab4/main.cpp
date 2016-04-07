#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <time.h>
#include "resource.h"
#define ID_TIMER 1

typedef struct BALL
{
    HBITMAP hBitmap;                  //the elipse will be transformed in a bitmap
    COLORREF cr;                      //ball's collor
    int cxClient,
        cyClient,                     //window size                                         //
        xCenter,
        yCenter,                      //ball's start
        cxTotal,
        cyTotal,                      //dimension for non border passing
        cxRadius,
        cyRadius,                     //ellispe diameter
        cxMove,
        cyMove;                       //pixeel to move
}BALL;

BALL ball[20];                        //max number of balls 20
HBRUSH hBrush;
HDC hdcMem, hdc;
int speed = 1;                        //animation speed

BITMAP bitmap;
PAINTSTRUCT ps;
int width = 600,
    height = 600;                      //initial window size


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
HINSTANCE hInst;
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
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255,255,255));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Lab4 WP"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           width,                 /* The programs width */
           height,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    if (!SetTimer(hwnd, ID_TIMER, speed, NULL)) {
        MessageBox(hwnd, "Too many clocks or timers!", szClassName, MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }

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
void set_ball_params(LPARAM lParam, int nr) {
    ball[nr].cxClient = width;
    ball[nr].cyClient = height;
    ball[nr].xCenter =width/2;
    ball[nr].yCenter = height/ 2;
    srand (time(NULL));
    ball[nr].cxMove = rand()%15;
    ball[nr].cyMove = rand()%15;
    ball[nr].cr = RGB(rand()%256,rand()%256,rand()%256);
    ball[nr].cxRadius = 15;
    ball[nr].cyRadius = 15;
    ball[nr].cxTotal = 2 *(ball[nr].cxRadius + ball[nr].cxMove);
    ball[nr].cyTotal = 2 *(ball[nr].cyRadius + ball[nr].cyMove);
}

void create(HWND nhwnd, LPARAM lParam, int nr){
    set_ball_params(lParam, nr);
    if (ball[nr].hBitmap){                                                      //erases the previous image of ball
        DeleteObject(ball[nr].hBitmap);
    }
    hdc = GetDC(nhwnd);
    hdcMem = CreateCompatibleDC(hdc);                                           // Create a memory device compatible with the above DC variable
    ball[nr].hBitmap = CreateCompatibleBitmap(hdc, ball[nr].cxTotal, ball[nr].cyTotal);
    ReleaseDC(nhwnd, hdc);
    SelectObject(hdcMem, ball[nr].hBitmap);
    Rectangle(hdcMem, -1, -1, ball[nr].cxTotal + 1, ball[nr].cyTotal + 1);      //define space for ball
    hBrush = CreateSolidBrush(ball[nr].cr);
    SelectObject(hdcMem, hBrush);
    SetBkColor(hdcMem, RGB(0, 0, 0));
    Ellipse(hdcMem, ball[nr].cxMove, ball[nr].cyMove,
                    ball[nr].cxTotal - ball[nr].cxMove,
                    ball[nr].cyTotal - ball[nr].cyMove);                        //draw ball with specified values
    DeleteDC(hdcMem);
    DeleteObject(hBrush);
}


void moove(HWND nhwnd, int nr) {
    if(!ball[nr].hBitmap) {                                                     //if there are balls, return
        return;
    }
    hdc = GetDC(nhwnd);
    hdcMem = CreateCompatibleDC(hdc);                                           // Create a memory device compatible with the above DC variable
    SelectObject(hdcMem, ball[nr].hBitmap);
    BitBlt(hdc, ball[nr].xCenter - ball[nr].cxTotal / 2,
                ball[nr].yCenter - ball[nr].cyTotal / 2,
                ball[nr].cxTotal, ball[nr].cyTotal, hdcMem, 0, 0, SRCCOPY);     // Copy the bits from the memory DC into the current dc
    ReleaseDC(nhwnd, hdc);
    DeleteDC(hdcMem);                                                           // Restore the old bitmap

    ball[nr].xCenter += ball[nr].cxMove;                                        //ball move
    ball[nr].yCenter += ball[nr].cyMove;
    if ((ball[nr].xCenter + ball[nr].cxRadius >= ball[nr].cxClient) ||
       (ball[nr].xCenter - ball[nr].cxRadius <= 0)) {
        ball[nr].cxMove = -ball[nr].cxMove;
    }
    if ((ball[nr].yCenter + ball[nr].cyRadius >= ball[nr].cyClient) ||
       (ball[nr].yCenter - ball[nr].cyRadius <= 60)) {
        ball[nr].cyMove = -ball[nr].cyMove;
       }

}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL newBall = true;
    static int nrBalls = 0;

    switch (message)                  /* handle the messages */
    {

        case WM_SIZE :
            width = LOWORD(lParam);
            height = HIWORD(lParam);
            if (newBall){
                create(hwnd, lParam, nrBalls);
                newBall = false;
            }
            for (int i = 0; i < nrBalls; i++) {
                ball[i].cxClient = width;
                ball[i].cyClient = height;
                ball[i].xCenter =width/2;
                ball[i].yCenter = height/ 2;
            }
            return 0;
        case WM_MOUSEWHEEL:
            if(GET_WHEEL_DELTA_WPARAM(wParam) < 0) {                            //// SStops to min/max speed
                if(speed < 100){speed += 10;}
                else{return 0;}
            } else {
                if(speed > 10){speed -= 10;}
                else{return 0;}
            }
            KillTimer(hwnd, ID_TIMER);
            SetTimer(hwnd, ID_TIMER, speed, NULL);
            return 0;
        case WM_LBUTTONDOWN :
            if(nrBalls < 19){                                                   //max number of 20 balls
                nrBalls++;
                create(hwnd, lParam, nrBalls);
            }
            return 0;
        case WM_TIMER :
            for (int i = 0; i <= nrBalls; i++){
                moove(hwnd, i);
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

