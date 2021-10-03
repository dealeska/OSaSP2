#include "EntryPoint.h"

HDC hdcBack;
HBITMAP hbmBack;
RECT rcClient;
HDC spriteImage;
HDC bckImage;
HWND hWnd;
Image image = { minX, minY };

bool isUp = false, isLeft = false, isDown = false, isRight = false;

void InitializeBackBuffer(HWND hWnd, int width, int height)
{
	HDC hdcWindow;
	hdcWindow = GetDC(hWnd);
	hdcBack = CreateCompatibleDC(hdcWindow);
	hbmBack = CreateCompatibleBitmap(hdcWindow, width, height);
	ReleaseDC(hWnd, hdcWindow);

	SaveDC(hdcBack);
	SelectObject(hdcBack, hbmBack);
}

void FinalizeBackBuffer()
{
	if (hdcBack)
	{
		RestoreDC(hdcBack, -1);
		DeleteObject(hbmBack);
		DeleteDC(hdcBack);
		hdcBack = 0;
	}
}

HDC LoadBitmapDC(HWND hwnd, char* fileName)
{
	HANDLE hBitmap = LoadImageW(0, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	int i = GetLastError();
	HDC hdc = GetDC(hwnd);
	HDC resultDC = CreateCompatibleDC(hdc);
	SelectObject(resultDC, hBitmap);
	ReleaseDC(0, hdc);

	return resultDC;
}

void GoLeft()
{
	if (image.xoriginDest > minX)
	{
		image.xoriginDest -= STEP;
	}
	else
	{
		image.xoriginDest += STEP * 4;
	}
}

void GoRight()
{
	if (image.xoriginDest < maxX)
	{
		image.xoriginDest += STEP;
	}
	else
	{
		image.xoriginDest -= STEP * 4;
	}
}

void GoUp()
{
	if (image.yoriginDest > minY)
	{
		image.yoriginDest -= STEP;
	}
	else
	{
		image.yoriginDest += STEP * 4;
	}
}

void GoDown()
{
	if (image.yoriginDest < maxY)
	{
		image.yoriginDest += STEP;
	}
	else
	{
		image.yoriginDest -= STEP * 4;
	}
}

void Check()
{
	if (isLeft)
	{
		GoLeft();

	}
	if (isRight)
	{
		GoRight();
	}
	if (isUp)
	{
		GoUp();
	}
	if (isDown)
	{
		GoDown();
	}

}

void Init(HWND hwnd)
{
	bckImage = LoadBitmapDC(hwnd, L"BackGround.bmp\0");
	spriteImage = LoadBitmapDC(hwnd, L"BlockGreen.bmp\0");
	DragAcceptFiles(hwnd, TRUE);
}

void Draw()
{
	PAINTSTRUCT ps;
	FillRect(hdcBack, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
	GdiTransparentBlt(hdcBack, 0, 0, 1051, 612, bckImage, 0, 0, 1436, 890, 1);
	GdiTransparentBlt(hdcBack, image.xoriginDest, image.yoriginDest, 62, 62, spriteImage, 0, 0, 44, 44, 0);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX), 0, MyWindowProc, 0, 0, hInstance, 0, LoadCursor(0, IDC_HAND),
		(HBRUSH)(COLOR_WINDOW + 1), NULL, L"First OSaSP lab", 0
	};
	MSG msg;

	RegisterClassEx(&wcex);

	hWnd = CreateWindowEx(WS_EX_ACCEPTFILES, L"First OSaSP lab", L"Lab_1_C", WINDOW_STYLE,
		(GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2, (GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2,
		WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInstance, NULL);

	while (GetMessage(&msg, 0, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK MyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC; 
	HBRUSH hBmp;
	LPWSTR lpszFile = NULL;

	switch (uMsg)
	{				
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_SIZE, Cls_OnSize);
		HANDLE_MSG(hWnd, WM_PAINT, Cls_OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_KEYDOWN, Cls_OnKey);
		HANDLE_MSG(hWnd, WM_KEYUP, Cls_OnKey);		
		HANDLE_MSG(hWnd, WM_DROPFILES, Cls_OnDropFiles);
		HANDLE_MSG(hWnd, WM_MOUSEWHEEL, Cls_OnMouseWheel);	
		HANDLE_MSG(hWnd, WM_MOUSEMOVE, Cls_OnMouseMove);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



void Cls_OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
	FinalizeBackBuffer();
}

bool Cls_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	Init(hwnd);
}

void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	GetClientRect(hwnd, &rcClient);
	FinalizeBackBuffer();
	InitializeBackBuffer(hwnd, cx, cy);
	InvalidateRect(hwnd, NULL, FALSE);
}

void Cls_OnPaint(HWND hwnd)
{	
	PAINTSTRUCT ps;
	HDC hDC;
	Draw();

	hDC = BeginPaint(hwnd, &ps);
	BitBlt(hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		hdcBack, 0, 0, SRCCOPY);
	EndPaint(hwnd, &ps);
}

void Cls_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	bool state = false;
	if (fDown)
	{
		state = true;
	}

	switch (vk)
	{
	case VK_UP:
		isUp = state;
		break;
	case VK_DOWN:
		isDown = state;
		break;
	case VK_LEFT:
		isLeft = state;
		break;
	case VK_RIGHT:
		isRight = state;
		break;
	case VK_ESCAPE:
		PostQuitMessage(0);
		break;
	}
	Check();
	InvalidateRect(hwnd, 0, 0);
}

void Cls_OnDropFiles(HWND hwnd, HDROP hdrop)
{
	UINT nFilenameSize = DragQueryFile(hdrop, 0, NULL, NULL);
	TCHAR FilePath[MAX_PATH];
	DragQueryFile(hdrop, 0, FilePath, nFilenameSize + 1);
	spriteImage = LoadBitmapDC(hwnd, FilePath);
	InvalidateRect(hwnd, NULL, FALSE);
}

void Cls_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{
	if (zDelta > 0)
	{
		if (fwKeys == MK_SHIFT)
		{
			GoRight();		
		}
		else
		{
			GoUp();
		}
	}
	else
	{
		if (fwKeys == MK_SHIFT)
		{
			GoLeft();
		}
		else
		{
			GoDown();
		}
	}

	InvalidateRect(hwnd, 0, 0);
}

void Cls_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
	if (keyFlags == MK_LBUTTON)
	{
		int dx = x - 31;
		int dy = y - 31;
		if (dx < maxX && dx > minX)
		{
			image.xoriginDest = x - 31;
		}
		if (dy < maxY && dy > minY)
		{
			image.yoriginDest = y - 31;
		}
	}
	InvalidateRect(hwnd, 0, 0);
}