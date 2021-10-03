proc LoadBitmapDC,\      ; создание дескриптора дл€ картинок, использу€ им€ файла, результат - еах
     fileName

        locals
                resultDC        dd      ?
                hdc             dd      ?
                hBitmap         dd      ?
        endl

        invoke  LoadImage, ebx, [fileName], IMAGE_BITMAP, ebx, ebx, LR_LOADFROMFILE
        mov     [hBitmap], eax

        invoke  GetDC, ebx
        mov     [hdc], eax
        invoke  CreateCompatibleDC, eax
        mov     [resultDC], eax
        invoke  SelectObject, [resultDC], [hBitmap]

        invoke  ReleaseDC, ebx, [hdc]

        mov     eax, [resultDC]

        ret
endp

proc Init uses esi edi

        locals
                hbmBack         dd      ?
                hdc             dd      ?
                wndW            dd      ?
                wndH            dd      ?
        endl

        invoke  GetTickCount
        mov     [lastTime], eax

        mov     esi, LoadBitmapDC

        stdcall esi, strBackGround
        mov     [hdcBackGround], eax

        stdcall esi, strMainImage
        mov     [hdcSpriteImage], eax

        mov     [sprite.x], 101
        mov     [sprite.y], 23


        invoke  RegisterClass, wndClass

        invoke  GetSystemMetrics, 0
        sub     eax, WINDOW_WIDTH
        shr     eax, 1
        mov     [wndW], eax

        invoke  GetSystemMetrics, 1
        sub     eax, WINDOW_HEIGHT
        shr     eax, 1
        mov     [wndH], eax

        invoke  CreateWindowEx, ebx, className, className, WINDOW_STYLE,\
                        [wndW], [wndH], WINDOW_WIDTH, WINDOW_HEIGHT, ebx, ebx, ebx, ebx
        mov     [hMainWindow], eax

        invoke  GetClientRect, [hMainWindow], clientRect

        invoke  GetDC, ebx
        mov     [hdc], eax
        invoke  CreateCompatibleDC, eax
        mov     [hdcBack], eax
        invoke  CreateCompatibleBitmap, [hdc], [clientRect.right], [clientRect.bottom]
        mov     [hbmBack], eax
        invoke  SelectObject, [hdcBack], [hbmBack]

        invoke  ReleaseDC, ebx, [hdc]

        ret
endp