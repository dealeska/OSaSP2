format PE GUI 4.0               ; проверить на перемещение блока (сдвиг игроком) + сделать краны
entry WinMain

        include         "Declaration.inc"

        className       db              "Lab_2_asm"
        wndClass        WNDCLASS        0, WindowProc, 0, 0, 0, 0, 0, 0, 0, className
        clientRect      RECT
        hMainWindow     dd              ?
        hdcBack         dd              ?

        include         "Draw.asm"
        include         "Init.asm"

proc WinMain                     ; точка входа и цикл обработки сообщений

        locals
                msg     MSG
        endl

        xor     ebx, ebx

        invoke  LoadCursor, ebx, IDC_ARROW
        mov     [wndClass.hCursor], eax

        stdcall Init

        lea     esi, [msg]

.cycle:
        invoke  GetMessage, esi, ebx, ebx, ebx
        invoke  DispatchMessage, esi
        jmp     .cycle

endp

proc WindowProc uses esi edi ebx,\      ; оконная процедура
     hWnd, uMsg, wParam, lParam

        locals
                ps      PAINTSTRUCT
                x       dd              ?
                y       dd              ?
                flag    dd              ?
                time    dd              ?
        endl

        xor     ebx, ebx

        switch  [uMsg]
        JumpIf  .Destroy,    WM_DESTROY
        JumpIf  .KeyDown,    WM_KEYDOWN
        JumpIf  .Paint,      WM_PAINT
        JumpIf  .Size,       WM_SIZE

        invoke  DefWindowProc, [hWnd], [uMsg], [wParam], [lParam]

        jmp     .Return


.Paint:
        stdcall Game.Draw

        lea     esi, [ps]
        invoke  BeginPaint, [hWnd], esi
        invoke  StretchBlt, dword[esi + PAINTSTRUCT.hdc],\
                        ebx, ebx, [clientRect.right], [clientRect.bottom],\
                        [hdcBack], ebx, ebx, [clientRect.right], [clientRect.bottom], SRCCOPY
        invoke  EndPaint, [hWnd], esi
        invoke  InvalidateRect, [hWnd], ebx, ebx
        jmp     .ReturnZero

.Exit:
        jmp     .Destroy

.Size:
        invoke  GetClientRect, [hMainWindow], clientRect
        jmp    .ReturnZero

.KeyDown:
        stdcall KeyDown, [wParam]
        jmp    .ReturnZero

.Destroy:
        invoke  ExitProcess, ebx

.ReturnZero:
        xor     eax, eax

.Return:
        ret
endp


proc KeyDown,\                  ; обработчик нажатия клавиши
     keyCode

.Switch:
        switch  [keyCode]
        JumpIf  .Escape,        VK_ESCAPE

        jmp     .ReturnZero

.Escape:
        invoke  ExitProcess, ebx

.ReturnZero:
        xor     eax, eax

        ret
endp