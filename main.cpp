#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "head.h"
using namespace std;

/**

TODO:
* User interface
* Resource scripts
* Graphical elements (like images)
* Dynamic Link Libraries.
* Files. For example - to save and restore program's status or keep some logs like high-scores to show scores.

**/


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

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
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
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
           _T("Tic Tac Toe"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           windowWidth + 14,                 /* The programs width */
           windowWidth + 28,                 /* and height in pixels */
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
    int wmId, wmEvent;
	PAINTSTRUCT ps;
    HDC hdc;
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_PAINT:
		{
            hdc = BeginPaint(hwnd, &ps);
            // Draw the board lines
            DrawBoard(hdc, windowWidth, lineLen);

            // Draw Xs and Os
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (GameBoard[i][j] == X) {
                       // DrawX(hdc, i, j, lineLen);
                       DrawImage(hdc, i, j, XIcon);
                    } else if (GameBoard[i][j] == O) {
                        //DrawO(hdc, i, j, lineLen);
                        DrawImage(hdc, i, j, OIcon);
                    }
                }
            }

            // Check for end of game conditions
            if (HasWon()) {
                if (playerTurn == O) {
                    SaveToFile("Player 1 (X) Won");
                    ::MessageBox(hwnd, _T("Player 1 Won"), _T("New Game?"), MB_OK);
                } else {
                    SaveToFile("Player 2 (O) Won");
                    ::MessageBox(hwnd, _T("Player 2 Won"), _T("New Game?"), MB_OK);
                }
                ResetGame();
                InvalidateRect(hwnd, 0, TRUE);
            } else {
                // If there is no win, check for a draw
                if (IsBoardFull()) {
                    SaveToFile("Game was a draw");
                    ::MessageBox(hwnd, _T("It's a Draw"), _T("New Game?"), MB_OK);
                    ResetGame();
                    // Repaint the window after the update
                    InvalidateRect(hwnd, 0, TRUE);
                }
            }
            EndPaint(hwnd, &ps);
		}
		break;
		case WM_LBUTTONDOWN:
		{

			bool isValidMove = ValidateMove(lParam, playerTurn, lineLen);
			if (isValidMove) {
                playerTurn = playerTurn == X ? O : X;
			} else {
				::MessageBox(hwnd, _T("Invalid Move"), _T("Error"), MB_OK);
			}
			// Repaint the window after the update
			InvalidateRect(hwnd, 0, TRUE);
		    break;
		}
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
