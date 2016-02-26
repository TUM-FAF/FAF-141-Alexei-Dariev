#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

#define BUTTON_SUBMIT	1			// Button identifier
#define BUTTON_F1	2			// Edit box identifier
#define BUTTTON_F2	3			// Edit box identifier
#define QUIT_BUTTON	4			// Edit box identifier
#define IDM_MYMENURESOURCE   3
#define IDR_MYMENU 101
#define IDI_MYICON 201

#define ID_FILE_EXIT 9001
#define ID_STUFF_GO 9002
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
HWND button,button1,buttonexit, hEdit,hwndOutputText,MAKEINTERSOURCE;
HINSTANCE hinst;
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
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
    wincl.lpszMenuName = MAKEINTRESOURCE(IDM_MYMENURESOURCE);                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH) ;
;




    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Laboratoty work 1 PW"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
          if (hwnd == NULL)
        return FALSE;

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow (hwnd) ;

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
    int iScreenW, iScreenH, iClientWidth, iClientHeight;
	static int cxChar, cyChar, iColor = 1;
	const char * szMove = "MOVE";
	static char szSubmitBuffer[512];
    HINSTANCE hProgramInstance;
    HFONT hfFont;
    HDC hDC ;
    PAINTSTRUCT paintStruct;
    RECT rect ;
    RECT rcWindow, rcClient;
    switch (message)                  /* handle the messages */
    {

        case WM_PAINT:
            GetClientRect(hwnd, &rcClient);
			iClientWidth = rcClient.right - rcClient.left;
			iClientHeight = rcClient.bottom - rcClient.top;
			hDC = BeginPaint(hwnd,&paintStruct);
            GetClientRect (hwnd, &rect) ;
			SetTextColor(hDC, COLORREF(0x0099CC00));
            SetBkColor(hDC, COLORREF(0x00000000));
            /*Display text in middle of window*/

            DrawText(hDC,TEXT("Done with Pride and Prejudice by Dariev Alexei"),-1,&rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);



			EndPaint(hwnd, &paintStruct);
		    break;

        case WM_CREATE:
            cxChar = LOWORD (GetDialogBaseUnits ());
			cyChar = HIWORD (GetDialogBaseUnits ());

			iScreenW = GetSystemMetrics(SM_CXSCREEN);
			iScreenH = GetSystemMetrics(SM_CYSCREEN);

			GetClientRect(hwnd, &rcClient);
			iClientWidth = rcClient.right - rcClient.left;
			iClientHeight = rcClient.bottom - rcClient.top;

			button = CreateWindow("BUTTON",
                                 "Change font",
                                 WS_VISIBLE | WS_CHILD |WS_BORDER,
                                 140,
                                 130,
                                 100,
                                 20,
                                 hwnd,
                                 (HMENU)BUTTON_F1,
                                 hProgramInstance,
                                 NULL);



              hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,
                                 "EDIT",
		                         "Enter your text here.",
                                 WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
		                         0,
		                         0,
                                 0,
		                         0,
		                         hwnd,
		                         (HMENU) 5,
		                         GetModuleHandle(NULL),
                                 NULL);

               hwndOutputText = CreateWindowEx(
				                 (DWORD)NULL,
				                  TEXT("edit"),
				                  TEXT("New Font."),
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE,
                                  20,
				                  100,
				                  400,
				                  20,
				                  hwnd,
				                  (HMENU) 2,
				                  GetModuleHandle(NULL),
                                  NULL);

                button1 = CreateWindow("BUTTON",
                                  "Apply Font",
                                  WS_VISIBLE | WS_CHILD |WS_BORDER,
                                  320,130,50,20,
                                  hwnd, (HMENU) BUTTON_SUBMIT, NULL, NULL);
                buttonexit= CreateWindowEx(
                (DWORD)NULL,
                TEXT("button"),
                TEXT("Quit"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
                435, 307, 60, 20,
                hwnd,
                (HMENU) QUIT_BUTTON,
                hProgramInstance,
                NULL);
	    break;

        case WM_COMMAND:
	         switch(LOWORD(wParam))
	         {
            case BUTTON_F1: {
                        hfFont = CreateFont(40, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, TEXT("Change Font"));
                        SendMessage(hwndOutputText, WM_SETFONT, (WPARAM)hfFont, 1);

            break;
            }


            case QUIT_BUTTON: {
                    if (MessageBox(NULL, TEXT("Are you sure?"), TEXT("Exit?"),MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING) == IDYES) {
                    SendMessage(hwnd, WM_DESTROY, 0, 0);

                    };

                    break;
                    }
              case BUTTON_SUBMIT:
		     {
                    SendMessage(hEdit,
						WM_GETTEXT,
						sizeof(szSubmitBuffer)/sizeof(szSubmitBuffer[0]),
						reinterpret_cast<LPARAM>(szSubmitBuffer));

					if (MessageBox(NULL,
							TEXT("Apply Changes?"),
							TEXT("Submit?"),
							MB_YESNO | MB_DEFBUTTON1 | MB_ICONWARNING) == IDYES) {
						SendMessage(hwndOutputText, WM_SETTEXT, 0, (LPARAM)szSubmitBuffer);
					};

					if ( strcmp(szMove, szSubmitBuffer) == 0 )
					{
						ShowWindow(hwnd, SW_MAXIMIZE);
					}


		break;
	}

	break;

	         }
	case WM_SIZE:
			GetClientRect(hwnd, &rcClient);
			iClientWidth = rcClient.right - rcClient.left;
			iClientHeight = rcClient.bottom - rcClient.top;

			MoveWindow(hEdit,
				iClientWidth * 1/10,
				iClientHeight * 2/10,
				iClientWidth * 78/100,
				iClientHeight * 13/100,
				TRUE);

			MoveWindow(hwndOutputText,
				iClientWidth * 1/10,
				iClientHeight * 6/10,
				iClientWidth * 78/100,
				iClientHeight * 3/10,
				TRUE);

            MoveWindow(buttonexit,
				iClientWidth * 79/100,
				iClientHeight * 89/100,
				iClientWidth * 20/100,
                iClientHeight * 10/100,
				TRUE);
			MoveWindow(button1,
				iClientWidth * 60/100,
				iClientHeight * 35/100,
				iClientWidth * 20/100,
                iClientHeight * 10/100,
				TRUE);

			MoveWindow(button,
				iClientWidth * 20/100,
				iClientHeight * 35/100,
                iClientWidth * 30/100,
                iClientHeight * 10/100,
				TRUE);


			break;
			break;

            case WM_DRAWITEM: {
                if ((UINT)wParam == QUIT_BUTTON) {
                    LPDRAWITEMSTRUCT lpdis = (DRAWITEMSTRUCT*)lParam;
                    SIZE size;
                    char szQuitBtnText[5];

                    strcpy(szQuitBtnText, "Exit");
                    GetTextExtentPoint32(lpdis->hDC, szQuitBtnText, strlen(szQuitBtnText), &size);
                    SetTextColor(lpdis->hDC, RGB(0, 0, 0));
                    SetBkColor(lpdis->hDC, RGB(200, 50, 50));

                    ExtTextOut(
                        lpdis->hDC,
                        ((lpdis->rcItem.right - lpdis->rcItem.left) - size.cx) / 2,
                        ((lpdis->rcItem.bottom - lpdis->rcItem.top) - size.cy) / 2,
                        ETO_OPAQUE | ETO_CLIPPED,
                        &lpdis->rcItem,
                        szQuitBtnText,
                        strlen(szQuitBtnText),
                        NULL);

                    DrawEdge(
                        lpdis->hDC,
                        &lpdis->rcItem,
                        (lpdis->itemState & ODS_SELECTED ? EDGE_SUNKEN : EDGE_RAISED ),
                        BF_RECT);
                    return TRUE;
                }

            break;
            }

       case WM_SYSCOMMAND:
		{
			switch(wParam)
			{
				case SC_MINIMIZE:
					return MessageBox(NULL,
						TEXT("Cannot minimize"),
						TEXT("Minimize function is not available"),
						MB_OK | MB_ICONWARNING);
					break;

				case SC_CLOSE:
					return MessageBox(NULL,
						TEXT("Click on exit button"),
						TEXT("Changed Exit Button"),
						MB_OK | MB_ICONASTERISK);
					break;


				default:
					return DefWindowProc(hwnd, message, wParam, lParam);
			}
		}

            case WM_DESTROY:
                PostQuitMessage (0);

            break;

		default:                     /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
