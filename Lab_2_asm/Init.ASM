proc Init uses esi edi

        locals
                hbmBack         dd      ?
                hdc             dd      ?
        endl


        invoke  RegisterClass, wndClass

        invoke  GetSystemMetrics, 0
        mov     [wndW], eax

        invoke  GetSystemMetrics, 1
        mov     [wndH], eax

        invoke  CreateWindowEx, ebx, className, className, WINDOW_STYLE,\
                        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, ebx, ebx, ebx, ebx
        mov     [hMainWindow], eax

        invoke  GetClientRect, [hMainWindow], clientRect

        invoke  GetDC, ebx
        mov     [hdc], eax
        invoke  CreateCompatibleDC, eax
        mov     [hdcBack], eax
        invoke  CreateCompatibleBitmap, [hdc], [wndW], [wndH]
        mov     [hbmBack], eax
        invoke  SelectObject, [hdcBack], [hbmBack]

        invoke  ReleaseDC, ebx, [hdc]

        ret
endp