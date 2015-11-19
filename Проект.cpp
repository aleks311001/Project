
#include "TXLib.h"

//typedef void (Vid_op) (double x1, y1, x2, y2, x3, y3);

struct Knop
    {
    double x, y;
    HDC vkl, vikl;
    };

struct XY
    {
    double x, y;
    };

struct Koord_Op
    {
    double x_Stoyka1,   y_Stoyka1,   x_Stoyka2,   y_Stoyka2,   x_Stoyka3,   y_Stoyka3,   x_Stoyka4,   y_Stoyka4, x_Stoyka5, y_Stoyka5, x_Stoyka6, y_Stoyka6, x_Stoyka7, y_Stoyka7, x_Stoyka8, y_Stoyka8;
    double x_travers11, y_travers11, x_travers12, y_travers12, x_travers13, y_travers13, x_travers14, y_travers14;
    double x_travers21, y_travers21, x_travers22, y_travers22, x_travers23, y_travers23, x_travers24, y_travers24;
    double x_travers31, y_travers31, x_travers32, y_travers32, x_travers33, y_travers33, x_travers34, y_travers34;
    };

struct Data
    {
    double Vetr, Gololed;
    double Yp, Ynw;
    };

Koord_Op Opor_kord = {};

double XWindow = 1216 + 250, YWindow = 760;

//double X1 = 0, Y1 = 0, X2 = 0, Y2 = 0, X3 = 0, Y3 = 0;

void Knopka_Vibor (Knop * Kn, Koord_Op op, const char* tekst);
void text (const char * text, double x, double y, double shrift = 15);
void DrawOpora (Koord_Op op, double Zoom, double Sdvig_x, double Sdvig_y);
double Text_Stroka (double *x, double *y);
double Interpolyaciya (double X1, double X2, double Xnugn, double nah1, double nah2);
double Kg   (double Pa);
double Pw   (double Ph_w, double Ynw, double Yp);
double Ph_w (double Aw,   double Kl,  double Kw, double Cx, double W, double F, double b);
double Aw   (double Pa);
double Kl   (double Dl_Proleta);
double F    (double diam,  double l, double Ki = 0, double Kl = 0, double By = 0);
double Kw   (double Visota, const char * tekst);
double Cx   (double d, bool Gololed);
double Pu   (double Ynw, double Yp, double Kw , double Fu, double W0);
double Fu   (double Du,  double Hu, double n,   double N);

int main()
    {
    txCreateWindow (XWindow, YWindow);

    txTextCursor (false);

    HDC Knopka_Vkl  = txLoadImage ("Image//Опоры//Шаблон_Вкл.bmp");
    HDC Knopka_Vikl = txLoadImage ("Image//Опоры//Шаблон_Викл.bmp");

    Knop Kn[5][8] = {};
    FILE* f_r = fopen ("База.cpp", "r");
    //--------------------|         |           |
    Koord_Op OP [5][8]= {{{/*fprintf (f_r, "%f", 1)*/-1.25, 0, -0.375, 18, -0.375, 25, -0.375, 25, 0.375, 25, 0.375, 25, 0.375, 18, 1.25, 0, -2, 19, -0.375, 20, 0.375, 20, 4.1, 19, 2,    23,  0.375, 24, -0.375, 24, -0.375, 23},
                          {-1.25, 0, -0.375, 18, -0.375, 30,  0,     31, 0.375, 31, 0.375, 30, 0.375, 18, 1.25, 0, -2, 19, -0.375, 20, 0.375, 20, 2,   19, -4.1, 23, -0.375, 24,  0.375, 24,  4.1,   23, -2, 27, -0.375, 28, 0.375, 28, 2, 27}}};

    char Nazv [5][8][49] = {{"П 110-1", "П 110-2"}};

    while (txMouseButtons() != 1)
        {
        txBegin ();
        txClear ();
        for (int i = 0; i < 5; i++)
            {
            for (int n = 0; n < 8; n++)
                {
                Kn [i][n].x    = 152 * n + 76;
                Kn [i][n].y    = 152 * i + 76;
                Kn [i][n].vkl  = Knopka_Vkl;
                Kn [i][n].vikl = Knopka_Vikl;
                Knopka_Vibor (&Kn [i][n], OP[i][n], Nazv[i][n]);
                }
            }
        txEnd ();
        }

    txClear ();

    double y = 23, x = 390;

    txSetColor (TX_BLACK);
    text ("Климатические условия",                  0, 0,  30);
    text ("1. Район по ветру                    :", 0, 20, 30);
    text ("2. Район по гололеду             :",     0, 40, 30);
    text ("3. Региональный коэффицент:",            0, 60, 30);
    text ("4. Коэффицент надежности    :",          0, 80, 30);

    double data [4] = {};

    for (int i = 0; !GetAsyncKeyState (VK_RETURN); i ++)
        {
        data [i] = Text_Stroka (&x, &y);
        txSleep (150);
        txSetColor (TX_WHITE);
        txLine (x, y + 7, x, y + 23);
        x = 390;
        y += 20;
        }

    return 0;
    }

void Knopka_Vibor (Knop * Kn, Koord_Op op, const char* tekst)
    {
    HDC dc = NULL;

    int Zoom = 4.5;

    int Sdvig_x = Kn->x + 15, Sdvig_y = Kn->y + 60;

    if (txMouseX() < Kn->x + 152 / 2 && txMouseX() > Kn->x - 152 / 2 &&
        txMouseY() < Kn->y + 152 / 2 && txMouseY() > Kn->y - 152 / 2)
        {
        if (txMouseButtons() == 1)
            {
            Opor_kord = op;
            }
        dc = Kn->vkl;
        txSetColor (RGB (0, 0, 0), 2);
        DrawOpora (op, 24, XWindow - 125, YWindow - 5);
        }
    else
        {
        dc = Kn->vikl;
        }

    txBitBlt (txDC(), Kn->x - 152 / 2, Kn->y - 152 / 2, 152, 152, dc, 0, 0);

    txSetColor (RGB (255, 255, 0), 1);
    DrawOpora (op, Zoom, Sdvig_x, Sdvig_y);

    text (tekst, Kn->x - 60, Kn->y - 56);
    }

void text (const char * text, double x, double y, double shrift)
    {
    txSelectFont ("Comic Sans MS", shrift, shrift / 2.5, 0, false, false, false, 0);
    txTextOut (x, y, text);
    }

void DrawOpora (Koord_Op op, double Zoom, double Sdvig_x, double Sdvig_y)
    {
    txLine (op.x_Stoyka1 * Zoom + Sdvig_x, -op.y_Stoyka1 * Zoom + Sdvig_y, op.x_Stoyka2 * Zoom + Sdvig_x, -op.y_Stoyka2 * Zoom + Sdvig_y);
    txLine (op.x_Stoyka2 * Zoom + Sdvig_x, -op.y_Stoyka2 * Zoom + Sdvig_y, op.x_Stoyka3 * Zoom + Sdvig_x, -op.y_Stoyka3 * Zoom + Sdvig_y);
    txLine (op.x_Stoyka3 * Zoom + Sdvig_x, -op.y_Stoyka3 * Zoom + Sdvig_y, op.x_Stoyka4 * Zoom + Sdvig_x, -op.y_Stoyka4 * Zoom + Sdvig_y);
    txLine (op.x_Stoyka4 * Zoom + Sdvig_x, -op.y_Stoyka4 * Zoom + Sdvig_y, op.x_Stoyka5 * Zoom + Sdvig_x, -op.y_Stoyka5 * Zoom + Sdvig_y);
    txLine (op.x_Stoyka5 * Zoom + Sdvig_x, -op.y_Stoyka5 * Zoom + Sdvig_y, op.x_Stoyka6 * Zoom + Sdvig_x, -op.y_Stoyka6 * Zoom + Sdvig_y);
    txLine (op.x_Stoyka6 * Zoom + Sdvig_x, -op.y_Stoyka6 * Zoom + Sdvig_y, op.x_Stoyka7 * Zoom + Sdvig_x, -op.y_Stoyka7 * Zoom + Sdvig_y);
    txLine (op.x_Stoyka7 * Zoom + Sdvig_x, -op.y_Stoyka7 * Zoom + Sdvig_y, op.x_Stoyka8 * Zoom + Sdvig_x, -op.y_Stoyka8 * Zoom + Sdvig_y);

    txLine (op.x_travers11 * Zoom + Sdvig_x, -op.y_travers11 * Zoom + Sdvig_y, op.x_travers12 * Zoom + Sdvig_x, -op.y_travers12 * Zoom + Sdvig_y);
    txLine (op.x_travers12 * Zoom + Sdvig_x, -op.y_travers12 * Zoom + Sdvig_y, op.x_travers13 * Zoom + Sdvig_x, -op.y_travers13 * Zoom + Sdvig_y);
    txLine (op.x_travers13 * Zoom + Sdvig_x, -op.y_travers13 * Zoom + Sdvig_y, op.x_travers14 * Zoom + Sdvig_x, -op.y_travers14 * Zoom + Sdvig_y);
    txLine (op.x_travers14 * Zoom + Sdvig_x, -op.y_travers14 * Zoom + Sdvig_y, op.x_travers11 * Zoom + Sdvig_x, -op.y_travers11 * Zoom + Sdvig_y);

    txLine (op.x_travers21 * Zoom + Sdvig_x, -op.y_travers21 * Zoom + Sdvig_y, op.x_travers22 * Zoom + Sdvig_x, -op.y_travers22 * Zoom + Sdvig_y);
    txLine (op.x_travers22 * Zoom + Sdvig_x, -op.y_travers22 * Zoom + Sdvig_y, op.x_travers23 * Zoom + Sdvig_x, -op.y_travers23 * Zoom + Sdvig_y);
    txLine (op.x_travers23 * Zoom + Sdvig_x, -op.y_travers23 * Zoom + Sdvig_y, op.x_travers24 * Zoom + Sdvig_x, -op.y_travers24 * Zoom + Sdvig_y);
    txLine (op.x_travers24 * Zoom + Sdvig_x, -op.y_travers24 * Zoom + Sdvig_y, op.x_travers21 * Zoom + Sdvig_x, -op.y_travers21 * Zoom + Sdvig_y);

    txLine (op.x_travers31 * Zoom + Sdvig_x, -op.y_travers31 * Zoom + Sdvig_y, op.x_travers32 * Zoom + Sdvig_x, -op.y_travers32 * Zoom + Sdvig_y);
    txLine (op.x_travers32 * Zoom + Sdvig_x, -op.y_travers32 * Zoom + Sdvig_y, op.x_travers33 * Zoom + Sdvig_x, -op.y_travers33 * Zoom + Sdvig_y);
    txLine (op.x_travers33 * Zoom + Sdvig_x, -op.y_travers33 * Zoom + Sdvig_y, op.x_travers34 * Zoom + Sdvig_x, -op.y_travers34 * Zoom + Sdvig_y);
    txLine (op.x_travers34 * Zoom + Sdvig_x, -op.y_travers34 * Zoom + Sdvig_y, op.x_travers31 * Zoom + Sdvig_x, -op.y_travers31 * Zoom + Sdvig_y);
    }

double Text_Stroka (double *x, double *y)
    {
    int Kl [11] = {};
    for (int i = 0; i < 10; i ++) Kl [i] = i + 48;
    Kl [10] = 190;

    char Kl_t[11] [40] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "."};

    double Vvod = 0;

    int tochka = 0, kol_zn_posl_zap = 0;

    for (int n = 0; !GetAsyncKeyState (VK_DOWN); n ++)
        {
        for (int i = 0; i < 11; i ++)
            {
            if (GetAsyncKeyState (Kl[i]))
                {
                if (!GetAsyncKeyState (190) && tochka == 0) Vvod = Vvod * 10 + Kl [i] - 48;
                if (!GetAsyncKeyState (190) && tochka == 1)
                    {
                    Vvod += (Kl [i] - 48) / pow (10, kol_zn_posl_zap + 1);
                    kol_zn_posl_zap ++;
                    }
                if (GetAsyncKeyState (190)) tochka = 1;
                txSetColor (TX_WHITE);
                txLine (*x, *y + 7, *x, *y + 23);
                txSetColor (TX_BLACK);
                text (Kl_t[i], *x, *y, 25);
                *x += 25 / 2.5 + 2;
                txSleep (125);
                }
            }

        if (n % 400000 == 200000)
            {
            txSetColor (TX_WHITE);
            txLine (*x, *y + 7, *x, *y + 23);
            }
        if (n % 400000 == 0)
            {
            txSetColor (TX_BLACK);
            txLine (*x, *y + 7, *x, *y + 23);
            }
        }
    return Vvod;
    }

double Interpolyaciya (double X1, double X2, double Xnugn, double nah1, double nah2)
    {
    return nah1 + (nah2 - nah1) * (Xnugn - X1) / (X2 - X1);
    }

double Kg (double Pa)
    {
    if (             Pa < 310) return 1;
    if (310 <= Pa && Pa < 350) return Interpolyaciya (310, 350, Pa, 1,    0.95);//1    - 0.05 * (Pa - 310) / 40;
    if (350 <= Pa && Pa < 425) return Interpolyaciya (350, 425, Pa, 0.95, 0.9 );//0.95 - 0.05 * (Pa - 350) / 75;
    if (425 <= Pa && Pa < 500) return Interpolyaciya (425, 500, Pa, 0.9,  0.85);//0.9  - 0.05 * (Pa - 425) / 75;
    if (500 <= Pa && Pa < 615) return Interpolyaciya (500, 615, Pa, 0.85, 0.8 );//0.85 - 0.05 * (Pa - 500) / 115;
    if (615 <= Pa            ) return 0.8;
    }

double Pw (double Ph_w, double Ynw, double Yp)
    {
    return Ph_w * Ynw * Yp * 1.1;
    }

double Ph_w (double Aw, double Kl, double Kw, double Cx, double W, double F, double b)
    {
    return Aw * Kl * Kw * Cx * W * F * b;
    }

double Aw (double Pa)
    {
    if (            Pa < 200) return 1;
    if (200 <= Pa && Pa < 240) return 1    - 0.06 * (Pa - 200) / 40;
    if (240 <= Pa && Pa < 280) return 0.94 - 0.06 * (Pa - 240) / 40;
    if (280 <= Pa && Pa < 300) return 0.88 - 0.03 * (Pa - 280) / 20;
    if (300 <= Pa && Pa < 320) return 0.85 - 0.02 * (Pa - 300) / 20;
    if (320 <= Pa && Pa < 360) return 0.83 - 0.03 * (Pa - 320) / 40;
    if (360 <= Pa && Pa < 400) return 0.80 - 0.04 * (Pa - 360) / 40;
    if (400 <= Pa && Pa < 500) return 0.76 - 0.05 * (Pa - 400) / 100;
    if (500 <= Pa && Pa < 580) return 0.71 - 0.01 * (Pa - 500) / 80;
    if (580 <= Pa           ) return 0.7;
    }

double Kl (double Dl_Proleta)
    {
    if (                     Dl_Proleta < 50)  return 1.20;
    if (50  <= Dl_Proleta && Dl_Proleta < 100) return Interpolyaciya (50,  100, Dl_Proleta, 1.20, 1.1 );//1.20 - 0.10 * (Dl_Proleta - 50 ) / 50;
    if (100 <= Dl_Proleta && Dl_Proleta < 150) return Interpolyaciya (100, 150, Dl_Proleta, 1.1,  1.05);//1.10 - 0.05 * (Dl_Proleta - 100) / 50;
    if (150 <= Dl_Proleta && Dl_Proleta < 250) return Interpolyaciya (150, 250, Dl_Proleta, 1.05, 1   );//1.05 - 0.05 * (Dl_Proleta - 150) / 100;
    if (250 <= Dl_Proleta                    ) return 1.00;
    }

double F (double diam, double l, double Ki/*= 0*/, double Kl /*= 0*/, double By /*= 0*/)
    {
    return (diam + 2 * Kl * Ki * By) * l * 0.001;
    }

double Kw (double Visota, const char * tekst)
    {
    if (tekst == "A" && tekst == "a")
        {
        if (                 Visota < 15)  return 1;
        if (15 <= Visota  && Visota < 20)  return Interpolyaciya (15,  20,  Visota, 1,    1.25);
        if (20 <= Visota  && Visota < 40)  return Interpolyaciya (20,  40,  Visota, 1.25, 1.5);
        if (40 <= Visota  && Visota < 60)  return Interpolyaciya (40,  60,  Visota, 1.5,  1.7);
        if (60 <= Visota  && Visota < 80)  return Interpolyaciya (60,  80,  Visota, 1.7,  1.85);
        if (80 <= Visota  && Visota < 100) return Interpolyaciya (80,  100, Visota, 1.85, 2);
        if (100 <= Visota && Visota < 150) return Interpolyaciya (100, 150, Visota, 2,    2.25);
        if (150 <= Visota && Visota < 200) return Interpolyaciya (150, 200, Visota, 2.25, 2.45);
        if (200 <= Visota && Visota < 250) return Interpolyaciya (200, 250, Visota, 2.45, 2.65);
        if (250 <= Visota && Visota < 300) return Interpolyaciya (250, 300, Visota, 2.65, 2.75);
        if (300 <= Visota && Visota < 350) return Interpolyaciya (300, 350, Visota, 2.75, 2.75);
        if (350 <= Visota                ) return 2.75;
        }

    if (tekst == "B" && tekst == "b")
        {
        if (                 Visota < 15)  return 0.65;
        if (15 <= Visota  && Visota < 20)  return Interpolyaciya (15,  20,  Visota, 0.65, 0.85);
        if (20 <= Visota  && Visota < 40)  return Interpolyaciya (20,  40,  Visota, 0.85, 1.1 );
        if (40 <= Visota  && Visota < 60)  return Interpolyaciya (40,  60,  Visota, 1.1,  1.3 );
        if (60 <= Visota  && Visota < 80)  return Interpolyaciya (60,  80,  Visota, 1.3,  1.45);
        if (80 <= Visota  && Visota < 100) return Interpolyaciya (80,  100, Visota, 1.45, 1.6 );
        if (100 <= Visota && Visota < 150) return Interpolyaciya (100, 150, Visota, 1.6,  1.9 );
        if (150 <= Visota && Visota < 200) return Interpolyaciya (150, 200, Visota, 1.9,  2.1 );
        if (200 <= Visota && Visota < 250) return Interpolyaciya (200, 250, Visota, 2.1,  2.3 );
        if (250 <= Visota && Visota < 300) return Interpolyaciya (250, 300, Visota, 2.3,  2.5 );
        if (300 <= Visota && Visota < 350) return Interpolyaciya (300, 350, Visota, 2.5,  2.75);
        if (350 <= Visota                ) return 2.75;
        }

    if (tekst == "C" && tekst == "c")
        {
        if (                 Visota < 15)  return 1;
        if (15 <= Visota  && Visota < 20)  return Interpolyaciya (15,  20,  Visota, 0.4,  0.55);
        if (20 <= Visota  && Visota < 40)  return Interpolyaciya (20,  40,  Visota, 0.55, 0.8 );
        if (40 <= Visota  && Visota < 60)  return Interpolyaciya (40,  60,  Visota, 0.8,  1   );
        if (60 <= Visota  && Visota < 80)  return Interpolyaciya (60,  80,  Visota, 1,    1.15);
        if (80 <= Visota  && Visota < 100) return Interpolyaciya (80,  100, Visota, 1.15, 1.25);
        if (100 <= Visota && Visota < 150) return Interpolyaciya (100, 150, Visota, 1.25, 1.55);
        if (150 <= Visota && Visota < 200) return Interpolyaciya (150, 200, Visota, 1.55, 1.8 );
        if (200 <= Visota && Visota < 250) return Interpolyaciya (200, 250, Visota, 1.8,  2   );
        if (250 <= Visota && Visota < 300) return Interpolyaciya (250, 300, Visota, 2,    2.2 );
        if (300 <= Visota && Visota < 350) return Interpolyaciya (300, 350, Visota, 2.2,  2.35);
        if (350 <= Visota                ) return 2.35;
        }

    }

double Cx (double d, bool Gololed)
    {
    if (d >= 20 && Gololed == false) return 1.1;
    else                             return 1.2;
    }

double Pu (double Ynw, double Yp, double Kw , double Fu, double W0)
    {
    return Ynw * Yp * Kw * 1.2 * Fu * W0 * 1.3;
    }

double Fu (double Du, double Hu, double n, double N)
    {
    return 0.7 * Du * Hu * n * N * 0.000001;
    }
