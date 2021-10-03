include         "..\win32a.inc"

include         "..\api\kernel32.inc"
include         "..\api\user32.inc"
include         "..\api\gdi32.inc"

include         "Resources.inc"

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

WINDOW_STYLE            =         (WS_VISIBLE or WS_OVERLAPPEDWINDOW)
WINDOW_WIDTH            =         1063
WINDOW_HEIGHT           =         650

true                    =       1
false                   =       0

wndW                    dd      ?
wndH                    dd      ?

ROW_COUNT               dd      5
LINE_COUNT              dd      4
MAX_LENGTH              dd      87

textMetrics     TEXTMETRIC

textStyle       =        DT_WORDBREAK or DT_CENTER or DT_EDITCONTROL


