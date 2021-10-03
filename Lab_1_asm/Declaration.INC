include         "..\win32a.inc"

include         "..\api\kernel32.inc"
include         "..\api\user32.inc"
include         "..\api\gdi32.inc"

include         "Resources.inc"
include         "Images.inc"

data import

        library kernel32,       "KERNEL32.DLL",\
                user32,         "USER32.DLL",\
                gdi32,          "GDI32.DLL"

end data


macro switch value
{
        mov     eax, value
}

macro JumpIf label, [value]
{
        cmp     eax, value
        je      label
}

struct Image
        x       dd      ?
        y       dd      ?
ends

WINDOW_STYLE            =         (WS_VISIBLE or WS_OVERLAPPEDWINDOW) and not(WS_SIZEBOX or WS_MAXIMIZEBOX)
WINDOW_WIDTH            =         1063
WINDOW_HEIGHT           =         650

true                    =       1
false                   =       0

minX                    dd      102
minY                    dd      23
maxX                    dd      895
maxY                    dd      502

isUp                    dd      false
isDown                  dd      false
isLeft                  dd      false
isRight                 dd      false
isUnderCursor           dd      false

lastTime                dd      ?

STEP                    =       5

