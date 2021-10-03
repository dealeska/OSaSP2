proc Game.Draw uses esi         ; всЄ рисуетс€ здесь. “ут провер€етс€ состо€ние игры и вызываютс€ другие функции

        invoke  FillRect, [hdcBack], clientRect, ebx      ; отрисовќчка
      ;  invoke  GdiTransparentBlt, [hdcBack], ebx, ebx, 1059, 622, [hdcBackGround], ebx, ebx, 1436, 890, 1
      ;  invoke  GdiTransparentBlt, [hdcBack], [sprite.x], [sprite.y], 62, 62, [hdcSpriteImage], 0, 0, 44, 44, 0
        ret

endp

proc Game.CheckBound

.CheckBelow:
        mov     eax, [sprite.y]
        cmp     eax, [maxY]
        jle     .CheckAbove
        mov     eax, [maxY]
        mov     [sprite.y], eax

.CheckAbove:
        mov     eax, [sprite.y]
        cmp     eax, [minY]
        jge     .CheckLeft
        mov     eax, [minY]
        mov     [sprite.y], eax

.CheckLeft:
        mov     eax, [sprite.x]
        cmp     eax, [minX]
        jge     .CheckRight
        mov     eax, [minX]
        mov     [sprite.x], eax

.CheckRight:
        mov     eax, [sprite.x]
        cmp     eax, [maxX]
        jle     .Return
        mov     eax, [maxX]
        mov     [sprite.x], eax

.Return:
        ret
endp

proc Game.Move

        cmp     [isUnderCursor], true
        je      .Return
.Down:
        cmp     [isDown], true
        jne     .Up
        add     [sprite.y], STEP
        jmp     .Left
.Up:
        cmp     [isUp], true
        jne     .Left
        sub     [sprite.y], STEP
.Left:
        cmp     [isLeft], true
        jne     .Right
        sub     [sprite.x], STEP
.Right:
        cmp     [isRight], true
        jne     .Return
        add     [sprite.x], STEP

.Return:
        ret
endp