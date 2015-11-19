
#include "TXLib.h"

void text (double x, double y, double razmer, const char * tekst);
double Text_Stroka (double *x, double *y);

int main()
    {
    txCreateWindow (800, 600);

    txTextCursor (false);

    double y = 500, x = 0, peremennay = 0;

    while (!GetAsyncKeyState (VK_RETURN))
        {
        peremennay = Text_Stroka (&x, &y);
        txSleep (150);
        printf ("%f", peremennay);
        txSetColor (TX_BLACK);
        txLine (x, y + 3, x, y + 27);
        x = 0;
        y += 35; // I am not a mouse now
        }
    return 0;
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
                txSetColor (TX_BLACK);
                txLine (*x, *y + 3, *x, *y + 27);
                txSetColor (TX_WHITE);
                text (*x, *y, 30, Kl_t[i]);
                *x += 30 / 2.5 + 2;
                txSleep (125);
                }
            }

        if (n % 400000 == 200000)
            {
            txSetColor (TX_BLACK);
            txLine (*x, *y + 3, *x, *y + 27);
            }
        if (n % 400000 == 0)
            {
            txSetColor (TX_WHITE);
            txLine (*x, *y + 3, *x, *y + 27);
            }
        }
    return Vvod;
    }

void text (double x, double y, double razmer, const char * tekst)
    {
    txSelectFont ("Arial", razmer, razmer / 2.5, 0, false, false, false, 0);
    txTextOut (x, y, tekst);
    }
