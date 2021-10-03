#include <Windows.h>
#include <WindowsX.h>

#define minY 23
#define maxY 488
#define minX 102
#define maxX 888
#define false 0
#define true 1
#define STEP 4
#define WINDOW_STYLE (WS_VISIBLE | WS_OVERLAPPEDWINDOW) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX)
#define WINDOW_WIDTH 1065
#define WINDOW_HEIGHT 650

typedef char bool;

typedef struct _Image
{
	int xoriginDest;
	int yoriginDest;
} Image;

LRESULT CALLBACK MyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Check();
void Init(HWND hwnd);
void GoUp();
void GoDown();
void GoLeft();
void GoRight();
void Draw();
void Cls_OnDestroy(HWND hwnd);
bool Cls_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy);
void Cls_OnPaint(HWND hwnd);
void Cls_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
void Cls_OnDropFiles(HWND hwnd, HDROP hdrop);
void Cls_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys);
void Cls_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);