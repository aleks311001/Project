//=============================================================================
// MeowPage.cpp v0.1 (c) Ded, 2015
//=============================================================================

#define __MODULE      "MeowPage"
#define __VERSION     "0.1. For internal use only."
#define __DESCRIPTION "A very simple tool for interactive classroom board."
#define __AUTHOR      "Ilya Dedinsky, 2015."

//-----------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include <limits.h>
#include "TXLib.h"

//-----------------------------------------------------------------------------

//{ Constants

    const int      DefPencilWidth   = 3;
    const COLORREF DefPencilClr     = TX_BLACK;
    const int      DefEraserSz      = 30;

    const int      SplinePtsPerLn   = 10;

    const int      MainLoopDelay    = 5,
                   SwitchPageDelay  = 250,
                   DoubleClickTime  = 500,
                   DefSleep         = 50;

    const int      PalGridSize      = 50;
    const COLORREF PalColors[2][5]  = {{ TX_BLACK, RGB (255,32,32), RGB (32,255,32), TX_LIGHTBLUE, RGB (255,125,25) },
                                       { TX_WHITE, TX_RED,          TX_GREEN,        TX_BLUE,      TX_YELLOW        }};
    const int      WaitMenuTime     = 5000;

    const int      RtPencilWidth    = 5;
    const COLORREF RtPencilClr      = TX_BLACK;
    const int      RtEraserWidth    = 1;
    const COLORREF RtEraserClr      = TX_LIGHTGRAY;

    const int      BackGridSz       = 20;
    const COLORREF BackGridClr      = RGB (245, 245, 245),
                   BackGridClr2     = RGB (225, 225, 225);
    const POINT    PageNumPos       = {35, 20};
    const int      PageNumR         = 20;
    const char*    PageNumFont      = "Times";
    const int      PageNumFontSz    = 30;

    const char*    SaveFileNameFmt  = "%%s %Y-%m-%d %H-%M-%S (%%d).bmp";
    const int      StringBufSz      = 100;

    const int      MaxPages         = 10;
    const int      SplinePoints     = 4;

//}

//{ Prototypes

    void DoProcess (char* argv0);
    void AddPoint (double posX[], double posY[], int btns[], int size);
    void Control  (double posX[], double posY[], int btns[], int size, HDC pages[], int nPages, int* curPage, bool* dirty);
    void SwitchPage (HDC pages[], int nPages, int* curPage, int inc, bool* dirty);
    void DoMenu (double x, double y);
    void Clear (double x0, double y0, double x1, double y1, int page);
    HDC  CreateBackDC (int sizeX, int sizeY, COLORREF color);
    void DoSleep (int time);
    void WaitClick (unsigned button, unsigned long timeout);
    bool IsDoubleClick (int btns[2], unsigned button, unsigned timeout);
    void DrawSpline (double x[SplinePoints], double y[SplinePoints], int r);
    double CatmullRom (double y[4], double t);
    bool SavePages (char* argv0, HDC pages[], int nPages, int curPage, int dirty);
    bool SaveBMP (const char* filename, HDC dc, int sizeX, int sizeY);
    long CALLBACK WndProc (HWND, unsigned msg, unsigned, long);
    inline bool IsZero (double x) { return fabs (x) <= 1E6; }

    namespace { namespace TX { namespace Win32 { _TX_DLLIMPORT ("GDI32", HBRUSH, CreatePatternBrush, (HBITMAP bmp)); }}}
//}

//-----------------------------------------------------------------------------

namespace Global
    {
    volatile bool IsRunning    = true;
    volatile bool IsSafeToExit = false;
    volatile bool ShowCursors  = true;
    HDC           TxWndDC      = NULL;
    HPEN          PencilPen    = NULL;
    HPEN          EraserPen    = NULL;
    HBITMAP       BackBmp      = NULL;
    }

//=============================================================================

int main (int, char* argv[])
    {
    printf ("\n" "%s v.%s (C) %s\n" "%s\n", __MODULE, __VERSION, __AUTHOR, __DESCRIPTION);

    assert (argv); assert (argv[0]);

    txTextCursor (false);
    txDisableAutoPause();
    txSetWindowsHook (WndProc);

    _txWindowStyle &= ~WS_BORDER;
    txCreateWindow (GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN));
    txBegin();
    txClear();
    txSleep (0);

    DoProcess (argv[0]);

    printf ("\n\n" "%s: Done" "\n\n", __MODULE);

    Global::IsSafeToExit = true;
    return 0;
    }

//-----------------------------------------------------------------------------

void DoProcess (char* argv0)
    {
    assert (argv0);

    HDC backDC        = CreateBackDC (BackGridSz, BackGridSz, BackGridClr);      assert (backDC);
    Global::BackBmp   = (HBITMAP) Win32::GetCurrentObject (backDC, OBJ_BITMAP);  assert (Global::BackBmp);

    Global::TxWndDC   = GetDC (txWindow());                                      assert (Global::TxWndDC);
    Global::PencilPen = Win32::CreatePen (PS_SOLID, RtPencilWidth, RtPencilClr); assert (Global::PencilPen);
    Global::EraserPen = Win32::CreatePen (PS_SOLID, RtEraserWidth, RtEraserClr); assert (Global::EraserPen);

    txSetColor (DefPencilClr, DefPencilWidth);
    txSelectObject (Win32::CreatePatternBrush (Global::BackBmp));
    Clear (0, 0, txGetExtentX(), txGetExtentY(), 0);

    HDC    pages[MaxPages] = {};
    int    curPage = 0;
    bool   dirty   = false;

    double posX [SplinePoints] = { txMouseX() },
           posY [SplinePoints] = { txMouseY() };
    int    btns [SplinePoints] = {};

    assert (SIZEARR (posX) == SIZEARR (posY));
    assert (SIZEARR (btns) >= 2);

    while (Global::IsRunning)
        {
        AddPoint (posX, posY, btns, SIZEARR (posX));
        Control  (posX, posY, btns, SIZEARR (posX), pages, SIZEARR (pages), &curPage, &dirty);

        if (GetAsyncKeyState (VK_ESCAPE) &&
            txMessageBox ("  Exit the program?", "MeowPage", MB_YESNO | MB_ICONQUESTION) == IDYES) break;

        DoSleep (MainLoopDelay);
        }

    SavePages (argv0, pages, SIZEARR (pages), curPage, dirty);

    for (int i = 0; i < SIZEARR (pages); i++) txDeleteDC (pages[i]);

    Win32::SelectObject (Global::TxWndDC, Win32::GetStockObject (NULL_PEN));
    Win32::DeleteObject (Global::PencilPen);
    Win32::DeleteObject (Global::EraserPen);
    ReleaseDC (txWindow(), Global::TxWndDC);

    txDeleteDC (backDC);
    }

//-----------------------------------------------------------------------------

void AddPoint (double posX[], double posY[], int btns[], int size)
    {
    assert (posX); assert (posY); assert (btns); assert (size >= SplinePoints);

    POINT pos = txMousePos();
    int   btn = txMouseButtons();

    memmove (posX+1, posX, (size-1) * sizeof (*posX)); posX[0] = pos.x;
    memmove (posY+1, posY, (size-1) * sizeof (*posY)); posY[0] = pos.y;
    memmove (btns+1, btns, (size-1) * sizeof (*btns)); btns[0] = btn;
    }

//-----------------------------------------------------------------------------

void Control (double posX[], double posY[], int btns[], int size, HDC pages[],
              int nPages, int* curPage, bool* dirty)
    {
    assert (posX); assert (posY); assert (btns); assert (size >= SplinePoints);
    assert (pages); assert (nPages >= 2);
    assert (curPage); assert (dirty);

    POINT sz = txGetExtent();

//  printf ("\r   " "%c%c", btns[0]&1? 'L' : ' ', btns[1]&2? 'R' : ' ');

    if (IsDoubleClick (btns, MK_RBUTTON, DoubleClickTime)) DoMenu (posX[0], posY[0]);

    if (GetAsyncKeyState (VK_SPACE))  Clear (0,      0, sz.x,     sz.y, *curPage);
    if (GetAsyncKeyState (VK_LSHIFT)) Clear (0,      0, sz.x/2+1, sz.y, *curPage);
    if (GetAsyncKeyState (VK_RSHIFT)) Clear (sz.x/2, 0, sz.x,     sz.y, *curPage);

    if (GetAsyncKeyState (VK_LEFT))   SwitchPage (pages, nPages, curPage, -1, dirty), Sleep (SwitchPageDelay);
    if (GetAsyncKeyState (VK_RIGHT))  SwitchPage (pages, nPages, curPage, +1, dirty), Sleep (SwitchPageDelay);

    if (btns[0] & MK_RBUTTON)
        {
        COLORREF color = txGetColor();
        txSetColor (TX_NULL);
        DrawSpline (posX, posY, DefEraserSz);
        txSetColor (color, DefPencilWidth);
        }

    else
    if (!(btns[0] & MK_RBUTTON) && ((btns[0] & MK_LBUTTON) || (btns[1] & MK_LBUTTON)))
        {
        DrawSpline (posX, posY, 0);
        *dirty = true;
        }
    }

//-----------------------------------------------------------------------------

void SwitchPage (HDC pages[], int nPages, int* curPage, int inc, bool* dirty)
    {
    assert (pages); assert (nPages >= 2); assert (curPage); assert (inc); assert (dirty);

    POINT size = txGetExtent();

    if (*dirty)
        {
        if (!pages [*curPage])
             pages [*curPage] = txCreateCompatibleDC (size.x, size.y);

        txBitBlt (pages [*curPage], 0, 0, size.x, size.y, txDC(), 0, 0);
        }

    *curPage = (*curPage + inc + nPages) % nPages;

    Clear (0, 0, size.x, size.y, *curPage);
    if (pages [*curPage]) txBitBlt (txDC(), 0, 0, size.x, size.y, pages [*curPage], 0, 0);

    *dirty = false;
    }

//-----------------------------------------------------------------------------

void DoMenu (double x, double y)
    {
    int sz = PalGridSize;
    Global::ShowCursors = false;

    COLORREF color = txGetColor();
    HDC sav = txCreateCompatibleDC (sz * SIZEARR (PalColors[0]), sz * SIZEARR (PalColors)); assert (sav);
    txBitBlt (sav, 0, 0, sz * SIZEARR (PalColors[0]), sz * SIZEARR (PalColors), txDC(), x-sz/2, y-sz/2);

    txSetColor (TX_BLACK);
    for (int iy = 0; iy < SIZEARR (PalColors);     iy++)
    for (int ix = 0; ix < SIZEARR (PalColors[iy]); ix++)
        {
        txSetFillColor (PalColors[iy][ix]);
        txRectangle (x-sz/2 + sz*ix, y-sz/2 + sz*iy, x+sz/2 + sz*ix, y+sz/2 + sz*iy);
        }
    txSleep (0);

    WaitClick (MK_LBUTTON, WaitMenuTime);

    color = txGetPixel (txMouseX(), txMouseY());

    txBitBlt (txDC(), x-sz/2, y-sz/2, sz * SIZEARR (PalColors[0]), sz * SIZEARR (PalColors), sav, 0, 0);
    txSleep (0);

    txDeleteDC (sav);

    txSetColor (color, DefPencilWidth);
    txSelectObject (Win32::CreatePatternBrush (Global::BackBmp));

    Global::ShowCursors = true;
    }

//-----------------------------------------------------------------------------

void Clear (double x0, double y0, double x1, double y1, int page)
    {
    COLORREF color = txGetColor();

    txSetColor (BackGridClr2);
    txRectangle (x0, y0, x1, y1);

    txLine ((x0 + x1) / 2, y0, (x0 + x1) / 2, y1);

    POINT sz = txGetExtent();
    txCircle (sz.x - PageNumPos.x, PageNumPos.y, PageNumR);

    txSelectFont (PageNumFont, PageNumFontSz);
    txSetTextAlign();
    char s[StringBufSz] = "";
    sprintf (s, "%d", page);
    txTextOut (sz.x - PageNumPos.x, PageNumPos.y*3/2, s);

    txSetColor (color, DefPencilWidth);
    }

//-----------------------------------------------------------------------------

HDC CreateBackDC (int sizeX, int sizeY, COLORREF color)
    {
    HDC dc = txCreateCompatibleDC (sizeX, sizeY); assert (dc);

    txSetColor (color);
    txRectangle (0, 0, sizeX, sizeY);
    txBitBlt (dc, 0, 0, sizeX, sizeY, txDC(), 0, 0);

    return dc;
    }

//-----------------------------------------------------------------------------

void DoSleep (int time)
    {
    static long t0 = GetTickCount();

    txSleep (time/2);

    for (;;)
        {
        long t = GetTickCount();
        if (t - t0 >= time) { t0 = t; break; }
        Sleep (0);
        }
    }

//-----------------------------------------------------------------------------

void WaitClick (unsigned button, unsigned long timeout)
    {
    for (unsigned long t = GetTickCount(); GetTickCount() - t < timeout/2; Sleep (DefSleep))
        if ((txMouseButtons() & button) != 0) break;

    for (unsigned long t = GetTickCount(); GetTickCount() - t < timeout/2; Sleep (DefSleep))
        if ((txMouseButtons() & button) == 0) break;
    }

//-----------------------------------------------------------------------------

bool IsDoubleClick (int p[2], unsigned button, unsigned timeout)
    {
    assert (p);

    static long time = LONG_MAX;
    bool isClick = false;

    if ((p[0] & button) && !(p[1] & button))
        {
        if (GetTickCount() - time <= timeout) isClick = true;

        time = GetTickCount();
        }

    return isClick;
    }

//-----------------------------------------------------------------------------

void DrawSpline (double x[SplinePoints], double y[SplinePoints], int r)
    {
    assert (x); assert (y);

    double x0 = CatmullRom (x, 0), y0 = CatmullRom (y, 0);

    if (r) txCircle (x0, y0, r);

    for (double t = 1.0/SplinePtsPerLn; t <= 1; t += 1.0/SplinePtsPerLn)
        {
        double x1 = CatmullRom (x, t), y1 = CatmullRom (y, t);

        if (r)
            {
            txCircle (x1, y1, r);

            double dx = x1 - x0, dy = y1 - y0, h = hypot (dx, dy);

            double r_dx_h = (IsZero (h)? 0 : (r-1) * dx / h),
                   r_dy_h = (IsZero (h)? 0 : (r-1) * dy / h);

            POINT p[] = {{ROUND (-r_dy_h + x0), ROUND ( r_dx_h + y0)},
                         {ROUND ( r_dy_h + x0), ROUND (-r_dx_h + y0)},
                         {ROUND ( r_dy_h + x1), ROUND (-r_dx_h + y1)},
                         {ROUND (-r_dy_h + x1), ROUND ( r_dx_h + y1)}};
            txPolygon (p, SIZEARR (p));
          }
        else
            {
            txLine (x0, y0, x1, y1);
            }

        x0 = x1;
        y0 = y1;
        }
    }

//-----------------------------------------------------------------------------

double CatmullRom (double y[SplinePoints], double t)
    {
    assert (y); assert (SplinePoints >= 4);

    double hermite00 =  2*t*t*t - 3*t*t + 1,
           hermite10 =    t*t*t - 2*t*t + t,
           hermite01 = -2*t*t*t + 3*t*t,
           hermite11 =    t*t*t -   t*t;

    return hermite00 * y[1] + hermite10 * (y[2] - y[0]) / 2 +
           hermite01 * y[2] + hermite11 * (y[3] - y[1]) / 2;
    }

//-----------------------------------------------------------------------------

bool SavePages (char* argv0, HDC pages[], int nPages, int curPage, int dirty)
    {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat-nonliteral"

    assert (argv0); assert (pages);

    argv0 = strrchr (argv0, '\\') + 1; assert (argv0);
    *strrchr (argv0, '.') = 0;

    time_t t = 0; time (&t);
    struct tm* tm = localtime (&t);

    char nameFmt [StringBufSz] = "";
    strftime (nameFmt, sizeof (nameFmt) - 1, SaveFileNameFmt, tm);

    char dir[MAX_PATH] = "";
    _getcwd (dir, sizeof (dir) - 1);

    POINT size = txGetExtent();

    for (int i = 0; i < nPages; i++)
        {
        if (! (pages[i] || (i == curPage && dirty))) continue;

        char filename [sizeof (nameFmt)] = "";
        sprintf (filename, nameFmt, argv0, i);

        printf ("\n" "Saving: %s\\%s...", dir, filename);
        SaveBMP (filename, (i == curPage)? txDC() : pages[i], size.x, size.y);
        }

    return true;

    #pragma GCC diagnostic pop
    }

//-----------------------------------------------------------------------------

bool SaveBMP (const char* filename, HDC dc, int sizeX, int sizeY)
    {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmultichar"

    assert (filename); assert (dc);

    FILE* f = fopen (filename, "wb");
    if (!f) return false;

    size_t szHdrs = sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER),
           szImg  = (sizeX * sizeY) * sizeof (RGBQUAD);

    BITMAPFILEHEADER hdr  = { 'MB', szHdrs + szImg, 0, 0, szHdrs };
    BITMAPINFOHEADER info = { sizeof (info), sizeX, sizeY, 1, WORD (sizeof (RGBQUAD) * 8), BI_RGB };

    RGBQUAD* buf = new RGBQUAD [sizeX * sizeY]; assert (buf);

    Win32::GetDIBits (dc, (HBITMAP) Win32::GetCurrentObject (dc, OBJ_BITMAP),
                      0, sizeY, buf, (BITMAPINFO*) &info, DIB_RGB_COLORS);

    fwrite (&hdr,  sizeof (hdr),  1, f);
    fwrite (&info, sizeof (info), 1, f);
    fwrite (buf,   szImg,         1, f);

    delete[] buf;

    fclose (f);
    return true;

    #pragma GCC diagnostic pop
    }

//-----------------------------------------------------------------------------

long CALLBACK WndProc (HWND, unsigned msg, unsigned wPar, long lPar)
    {
    if (msg == WM_ERASEBKGND)
        return 1;

    if (msg == WM_MOUSEMOVE && Global::ShowCursors)
        {
        int x = LOWORD (lPar), y = HIWORD (lPar);

        if (wPar & MK_LBUTTON)
            {
            Win32::SelectObject (Global::TxWndDC, Global::PencilPen);
            Win32::MoveToEx     (Global::TxWndDC, x, y, NULL);
            Win32::LineTo       (Global::TxWndDC, x, y);
            }

        if (wPar & MK_RBUTTON)
            {
            Win32::SelectObject (Global::TxWndDC, Global::EraserPen);
            Win32::Ellipse      (Global::TxWndDC, x-DefEraserSz, y-DefEraserSz, x+DefEraserSz, y+DefEraserSz);
            }
        }

    if (msg == WM_DESTROY)
        {
        Global::IsRunning = false;
        while (!Global::IsSafeToExit) Sleep (DefSleep);
        }

    return 0;
    }

