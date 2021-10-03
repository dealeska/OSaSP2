format PE GUI 4.0
entry WinMain

        include         "Declaration.inc"

        className       db              "Lab_1_asm"
        wndClass        WNDCLASS        0, WindowProc, 0, 0, 0, 0, 0, 0, 0, className
        clientRect      RECT
        hMainWindow     dd              ?
        hdcBack         dd              ?
        sprite          Image           ?

        include         "Game.asm"
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
        JumpIf  .KeyUp,      WM_KEYUP
        JumpIf  .Paint,      WM_PAINT
        JumpIf  .MouseMove,  WM_MOUSEMOVE
        JumpIf  .MouseWheel, WM_MOUSEWHEEL
        JumpIf  .DropFiles,  WM_DROPFILES

        invoke  DefWindowProc, [hWnd], [uMsg], [wParam], [lParam]

        jmp     .Return

.MouseWheel:
        stdcall MouseWheel, [wParam]
        stdcall Game.CheckBound
        jmp     .ReturnZero

.Paint:
        invoke  GetTickCount
        mov     [time], eax
        sub     eax, 17
        cmp     eax, [lastTime]
        jng     .SkipMovement
        mov     eax, [time]
        mov     [lastTime], eax
        stdcall Game.Move
.SkipMovement:
        stdcall Game.CheckBound
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

.DropFiles:

        jmp     .ReturnZero

.MouseMove:
        movzx   eax, word[lParam]
        mov     [x], eax

        movzx   eax, word[lParam + 2]
        mov     [y], eax

        mov     eax, [wParam]
        mov     [flag], eax

        stdcall MouseMove, [x], [y], [flag], [hWnd]
        jmp     .ReturnZero

.KeyDown:
        stdcall KeyDown, [wParam]
        jmp    .ReturnZero

.KeyUp:
        stdcall KeyUp, [wParam]
        jmp     .ReturnZero

.Destroy:
        invoke  ExitProcess, ebx

.ReturnZero:
        xor     eax, eax

.Return:
        ret
endp


proc MouseMove,\
     x, y, flag, hWnd

        locals
                dx      dd      ?
                dy      dd      ?
        endl

        cmp     [flag], 1
        jne     .MouseUp

        mov     [isUnderCursor], true

        mov     eax, [x]
        mov     [dx], eax
        sub     [dx], 31
        mov     eax, [dx]
        mov     [sprite.x], eax
        mov     eax, [y]
        mov     [dy], eax
        sub     [dy], 31
        mov     eax, [dy]
        mov     [sprite.y],eax
        jmp     .Return

.MouseUp:
        mov     [isUnderCursor], false

.Return:
        invoke  InvalidateRect, [hWnd], ebx, ebx
        xor     eax, eax

        ret
endp

proc MouseWheel,\
     wheel

        locals
                fwKeys  dd      ?
        endl

        cmp     [isUnderCursor], true
        je      .Return

        movzx   eax, word[wheel]
        mov     [fwKeys], eax

        cmp    [wheel], 0
        jle    .Down

        cmp     [fwKeys], 4
        jne     .GoUp

        add     [sprite.x], STEP
        jmp     .Return

.GoUp:
        sub     [sprite.y], STEP
        jmp     .Return

.Down:
        cmp     [fwKeys], 4
        jne     .GoDown

        sub     [sprite.x], STEP
        jmp     .Return

.GoDown:
        add     [sprite.y], STEP
        jmp     .Return

.Return:
        ret
endp

proc KeyDown,\                  ; обработчик нажатия клавиши
     keyCode

.Switch:
        switch  [keyCode]
        JumpIf  .Left,          VK_LEFT, '4', 'A'
        JumpIf  .Right,         VK_RIGHT, '6', 'D'
        JumpIf  .Up,            VK_UP, VK_SPACE, '8', 'W'
        JumpIf  .Down,          VK_DOWN,  '5', 'S'
        JumpIf  .Escape,        VK_ESCAPE

        jmp     .ReturnZero

.Left:
        mov     [isLeft], true
        jmp     .ReturnZero

.Right:
        mov     [isRight], true
        jmp     .ReturnZero

.Up:
        mov     [isUp], true
        jmp     .ReturnZero

.Down:
        mov     [isDown], true
        jmp     .ReturnZero

.Escape:
        invoke  ExitProcess, ebx

.ReturnZero:
        xor     eax, eax

        ret
endp

proc KeyUp,\                    ; обработчик отпуска клавиши
     keyCode

        switch  [keyCode]
        JumpIf  .Left,          VK_LEFT, 'A', '4'
        JumpIf  .Right,         VK_RIGHT, 'D', '6'
        JumpIf  .Down,          VK_DOWN,  '5', 'S'
        JumpIf  .Up,            VK_UP, VK_SPACE, 'W', '8'

        jmp     .ReturnZero

.Right:
        mov     [isRight], false
        jmp     .ReturnZero

.Left:
        mov     [isLeft], false
        jmp     .ReturnZero

.Up:
        mov     [isUp], false
        jmp     .ReturnZero

.Down:
        mov     [isDown], false
        jmp     .ReturnZero

.ReturnZero:
        xor     eax, eax

        ret
endp