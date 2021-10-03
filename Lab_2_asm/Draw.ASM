proc Game.Draw uses esi edi         ; всё рисуется здесь. Тут проверяется состояние игры и вызываются другие функции

         locals
                cellWidth       dd      ?
                cellHeight      dd      ?
                cellRect        RECT    0, 0, 0, 0
                hdc             dd      ?
                width           dd      0
                height          dd      0
        endl


        invoke  GetDC, ebx
        mov     [hdc], eax

        invoke  GetTextMetricsA, [hdc], textMetrics

        invoke  GetStockObject, WHITE_BRUSH
        invoke  SelectObject, [hdcBack], eax

        invoke  FillRect, [hdcBack], clientRect, ebx

        invoke  GetStockObject, BLACK_PEN
        invoke  SelectObject, [hdcBack], eax

        mov     eax, [clientRect.right]               ;cellWidth = windowWidth / ROW_COUNT
        div     [ROW_COUNT]
        mov     [cellWidth], eax
        mov     [cellRect.right], eax

        xor     edx, edx
        mov     eax, [MAX_LENGTH]                     ;lineCount = (MAX_LENGTH / cellWidth) * tmAveCharWidth + 1
        mul     [textMetrics.tmAveCharWidth]
        div     [cellWidth]
        inc     eax

        mov     ecx, [textMetrics.tmHeight]           ;textHeight = lineCount * (tmHeight + tmExternalLeading)
        add     ecx, [textMetrics.tmExternalLeading]
        mul     ecx

        mov     ecx, [textMetrics.tmAveCharWidth]     ;cellHeight = textHeight + 2 * tmAveCharWidth
        shl     ecx, 1
        add     eax, ecx
        mov     [cellHeight], eax
        mov     [cellRect.bottom], eax


        mov     eax, [cellHeight]                     ;esi = tableHeight
        mul     [LINE_COUNT]
        mov     esi, eax

        invoke  MoveToEx, [hdcBack], ebx, ebx, ebx
        invoke  LineTo, [hdcBack], [clientRect.right], ebx

        mov     edi, 0
        mov     ecx, [ROW_COUNT]
        add     ecx, 1

.VerticalLine:
        push    ecx

        invoke  MoveToEx, [hdcBack], edi, 0, ebx
        invoke  LineTo, [hdcBack], edi, esi
        add     edi, [cellWidth]

        pop     ecx
        loop    .VerticalLine


        mov     ecx, [LINE_COUNT]
        mov     edi, lines
.LineLoop:
        push    ecx
        push    [cellRect.left]
        push    [cellRect.top]
        push    [cellRect.right]
        push    [cellRect.bottom]

        mov     ecx, [ROW_COUNT]
.RowLoop:
        push    ecx

        stdcall DrawTableText, [cellRect.left], [cellRect.top], [cellRect.right], [cellRect.bottom], [edi]
        add     edi, 4

        mov     eax, [cellWidth]
        add     [cellRect.left], eax
        add     [cellRect.right], eax

        pop     ecx
        loop    .RowLoop

        invoke  MoveToEx, [hdcBack], 0, [cellRect.bottom], ebx
        invoke  LineTo, [hdcBack], [clientRect.right], [cellRect.bottom]

        pop     [cellRect.bottom]
        pop     [cellRect.right]
        pop     [cellRect.top]
        pop     [cellRect.left]
        pop     ecx

        mov     eax, [cellHeight]
        add     [cellRect.top], eax
        add     [cellRect.bottom], eax

        loop    .LineLoop

        xor     eax, eax
        ret

endp

proc DrawTableText uses esi,\
     left, top, right, bottom, lineAdrr

        locals
                textRect        RECT    0, 0, 0, 0
        endl

        mov     esi, [textMetrics.tmAveCharWidth]

        mov     eax, [left]
        add     eax, esi
        mov     [textRect.left], eax

        mov     eax, [top]
        add     eax, esi
        mov     [textRect.top], eax

        mov     eax, [right]
        sub     eax, esi
        mov     [textRect.right], eax

        mov     eax, [bottom]
        sub     eax, esi
        mov     [textRect.bottom], eax

        lea     esi, [textRect]
        invoke  DrawTextEx, [hdcBack], [lineAdrr], -1, esi, textStyle, NULL

        xor     eax, eax
        ret
endp