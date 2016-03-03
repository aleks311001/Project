
#include "TXLib.h"

/*добавить функцию назад
сделать историю подсчета*/

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

Koord_Op Opor_kord = {};

double XWindow = 1216 + 250, YWindow = 760;
double Kg1 = 0, Pw1 = 0, Pu1 = 0, Gpr = 0, Gg = 0;

//double X1 = 0, Y1 = 0, X2 = 0, Y2 = 0, X3 = 0, Y3 = 0;

void Knopka_Vibor (Knop * Kn, Koord_Op op, const char* tekst);
void Vibor_Opori ();
void Nahogdenie_Ugla (double *do_shap_iz, double *Dlina_iz, int *Kol_vo_iz, double *Diam_iz, double *Stroit_vis);
void Risovanie (double do_shap_iz, double Dlina_iz, int Kol_vo_iz, double Diam_iz, double Stroit_vis);
void Iz (double x, double y, double Zoom, double do_shap_iz, double Dlina_iz, int Kol_vo_iz,
         double Diam_iz, double Stroit_vis, double perX, double perY);
void Iz_Naklon (double x, double y, double x2, double y2, double *Ugol_max, double Zoom, double do_shap_iz,
                double Stroit_vis, double Dlina_iz, int Kol_vo_iz, double Diam_iz, double perX, double perY, const char* number);
void Iz_vmeste (double x, double y, double x2, double y2, double Zoom, double do_shap_iz, double Dlina_iz,
                double Stroit_vis, double Kol_vo_iz, double Diam_iz, double y_b, const char* number);
void Izol_po_otdel (double x, double y, double x2, double y2, double ugol, double Zoom, double do_shap_iz, double Stroit_vis,
                    double Dlina_iz, int Kol_vo_iz, double Diam_iz, double perX, double perY);
void text (const char * text, double x, double y, double shrift = 15);
bool SaveBMP (const char* filename, HDC dc, int sizeX, int sizeY);
void DrawOpora (Koord_Op op, double Zoom, double Sdvig_x, double Sdvig_y);
void Clear (double x1, double y1, double x2, double y2);
void Otobragenie_Ballast (double Ugol_max, int x, int y);
double Nahodim_Gb (double Ugol_max);
double dist (int x1, int y1, int x2, int y2);
double Text_Stroka (double *x, double *y, bool *up_or_down);
void Tablica (double x_razd, double kol_vo_srt, double vis_str);
double Otbros (double x);
double Interpolyaciya (double X1, double X2, double Xnugn, double nah1, double nah2);
double Pa   (double tip);
double Kg   (double Pa);
double Pw   (double Ph_w, double Ynw, double Yp);
double Ph_w (double Aw,   double Kl,  double Kw, double Cx, double W, double F, double b);
double Aw   (double Pa);
double Kl   (double Dl_Proleta);
double F    (double diam,  double l, double Ki = 0, double Kl = 0, double By = 0);
double Kw   (double Visota, int k);
double Cx   (double d, bool Gololed);
double Pu   (double Ynw, double Yp, double Kw , double Fu, double W0);
double Fu   (double Du,  double Hu, double n,   double N);

int main()
    {
    txCreateWindow (XWindow, YWindow);

    txTextCursor (false);

    double do_shap_iz = 0, Dlina_iz = 0, Diam_iz = 0, Stroit_vis = 0;
    int Kol_vo_iz = 0;

    Vibor_Opori ();

    Nahogdenie_Ugla (&do_shap_iz, &Dlina_iz, &Kol_vo_iz, &Diam_iz, &Stroit_vis);

    txSetFillColor (TX_WHITE);
    txClear ();

    Risovanie (do_shap_iz, Dlina_iz, Kol_vo_iz, Diam_iz, Stroit_vis);

    return 0;
    }

void Vibor_Opori ()
    {
    HDC Knopka_Vkl  = txLoadImage ("Image//Опоры//Шаблон_Вкл.bmp");
    HDC Knopka_Vikl = txLoadImage ("Image//Опоры//Шаблон_Викл.bmp");

    Knop Kn[5][8] = {};

    //--------------------|   st1   |    st2    |    st3    |    st4    |   st5    |    st6   |   st7    |   st8  |   tr1   |    tr1    |   tr1    |   tr1  |    tr2    |     tr2     |    tr2      |     tr2     |  tr3  |    tr3    |    tr3   | tr3 |
    Koord_Op OP [5][8]= {{{-1.25, 0, -0.375, 18, -0.375, 25, -0.375, 25, 0.375, 25, 0.375, 25, 0.375, 18, 1.25, 0, -2,   19, -0.375, 20, 0.375, 20, 4.1, 19, 2,    23,    0.375, 24,   -0.375, 24,   -0.375, 23},
                          {-1.25, 0, -0.375, 18, -0.375, 30,  0,     31, 0.375, 31, 0.375, 30, 0.375, 18, 1.25, 0, -2,   19, -0.375, 20, 0.375, 20, 2,   19, -4.1, 23,   -0.375, 24,    0.375, 24,    4.1,   23,   -2, 27, -0.375, 28, 0.375, 28, 2, 27},
                          {-3.4,  0, -3.4,   0,  -1.019, 59, -2.5,   61, 2.5,   61, 1.019, 59, 3.4,   0,  3.4,  0, -4.5, 40, -1.892, 38, 1.892, 38, 4.5, 40, -6.5, 47.5, -1.584, 45.4,  1.584, 45.4,  6.5,   47.5, -4, 55, -1.273, 53, 1.273, 53, 4, 55}}};

    char Nazv [5][8][49] = {{"П 110-1", "П 110-2", "ПП110-2/40"}};

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

    txDeleteDC (Knopka_Vkl);
    txDeleteDC (Knopka_Vikl);

    txClear ();
    }

void Nahogdenie_Ugla (double *do_shap_iz, double *Dlina_iz, int *Kol_vo_iz, double *Diam_iz, double *Stroit_vis)
    {
    double y = 20, x = 910;

    txSetColor (TX_BLACK);
    //text ("Климатические условия",                                              0, 0,   30);
    text ("1.  Район по ветру:",                                                                         0, 20,  30);
    text ("2.  Район по гололеду:",                                                                      0, 40,  30);
    text ("3.  Региональный коэффицент:",                                                                0, 60,  30);
    text ("4.  Коэффицент надежности по ответственности:",                                               0, 80,  30);
    text ("5.  Тип местности (1 - А; 2 - В; 3 - С):",                                                    0, 100, 30);
    text ("6.  Длина пролета, м:",                                                                       0, 120, 30);
    text ("7.  Высота расположения Приведенного центра тяжести проводов, м:",                            0, 140, 30);
    text ("8.  Диаметр провода, мм:",                                                                    0, 160, 30);
    //text ("9.  Нормативное ветровое давление:",                                                          0, 180, 30);//
    text ("9. Длина ветрового пролета, м:",                                                              0, 180, 30);
    text ("10. Угол между направлением ветра и осью ВЛ, град:",                                          0, 200, 30);
    text ("11. Высота расположения Приведенного центра тяжести изоляторов, м:",                          0, 220, 30);
    text ("12. Диаметр тарелки изоляторов, мм:",                                                         0, 240, 30);
    text ("13. Строительная высота изолятора, мм:",                                                      0, 260, 30);
    text ("14. Число изоляторов в цепи, шт:",                                                            0, 280, 30);
    text ("15. Число цепей изоляторов в гирляде, шт:",                                                   0, 300, 30);
    text ("16. Масса одного метра провода, кг/м:",                                                       0, 320, 30);
    text ("17. Весовой пролет, м:",                                                                      0, 340, 30);
    text ("18. Масса гирлянды, кг:",                                                                     0, 360, 30);
    text ("19. Расстояние до шапки изолятора, мм:",                                                      0, 380, 30);
    text ("20. Длина гирлянды изоляторов, мм:",                                                          0, 400, 30);
    text ("Нажмите M, чтобы посмотреть карты районирования территории РФ по климатическим воздействиям", 0, 420, 30);

    bool up_or_down = false;

    double data [20] = {};

    Tablica (905, 20, 20);

    for (int i = 0; i < 20;)
        {
        data [i] = Text_Stroka (&x, &y, &up_or_down);
        txSleep (150);
        txSetColor (TX_WHITE);
        txLine (x, y + 7, x, y + 26);
        x = 910;
        if (up_or_down == false && i < 20)
            {
            y += 20;
            i ++;
            }
        if (up_or_down == true && i > 0)
            {
            y -= 20;
            i --;
            txSetFillColor (TX_WHITE);
            Clear (910, y + 7, 1000, y + 25);
            }
        }

    *do_shap_iz = data [18];
    *Dlina_iz   = data [19];
    *Kol_vo_iz  = data [13];
    *Diam_iz    = data [11];
    *Stroit_vis = data [12];

    double Pa1 = Pa (data[0]);
           Kg1 = Kg (Pa1); //0.8
    //------------------------------------------------------------------------------
    double Aw1 = Aw (Pa1); //0.7
    double Kl1 = Kl (data [5]); //1
    double Kw_pr1 = Kw (data [6], data [4]); //1.2  тип местности
    double Cx1 = Cx (data [7], false);    //1.1
    double F1 = F  (data [7], data [8]);  //7.35
    //--
    double Ph_w1 = Ph_w (Aw1, Kl1, Kw_pr1, Cx1, Pa1, F1, data [9]); //6791
    //----------
           Pw1 = Pw (Ph_w1, data [3], data [2]);//10682
    //-------------------------------------------------------------------------------
    double Kw_u1 = Kw (data [10], data [4]); //1.325  // тип местности
    double Fu1 = Fu (data [11], data [12], data [13], data [14]);//0.204
    //--------------------------------
           Pu1 = Pu (data [3], data [2], Kw_u1, Fu1, Pa1);//603
    //----------------------------------------------------------------------------
           Gpr = data [15] * data [16] * 9.81; //1765.8
    //-----------------------------------------------------------------------------
           Gg  = data [17] * 9.81; //981
    //------------------------------------------------------------------------------

    int number;
    char name_f [40] = "";
    FILE *name = fopen ("Names.cpp", "r");
    fscanf (name, "%d", &number);
    sprintf (name_f, "История//data %d.cpp", number);
    FILE *story = fopen (name_f, "w");
    for (int i = 0; i < 20; i++) fprintf (story, "%lf \n", data[i]);
    fclose (story);
    fclose (name);
    }

void Risovanie (double do_shap_iz, double Dlina_iz, int Kol_vo_iz, double Diam_iz, double Stroit_vis)
    {
    double Zoom = 2.6 * YWindow / Opor_kord.y_Stoyka4;;
    int n = 235;
    txSetColor (TX_BLACK, 3);
    DrawOpora (Opor_kord, Zoom, 400, 1840);
    Iz_vmeste (Opor_kord.x_travers11, Opor_kord.y_travers11, Opor_kord.x_travers12, Opor_kord.y_travers12, Zoom, do_shap_iz, Dlina_iz,
               Stroit_vis, Kol_vo_iz, Diam_iz, 20  + n, "1");
    Iz_vmeste (Opor_kord.x_travers21, Opor_kord.y_travers21, Opor_kord.x_travers22, Opor_kord.y_travers22, Zoom, do_shap_iz, Dlina_iz,
               Stroit_vis, Kol_vo_iz, Diam_iz, 70  + n, "2");
    Iz_vmeste (Opor_kord.x_travers31, Opor_kord.y_travers31, Opor_kord.x_travers32, Opor_kord.y_travers32, Zoom, do_shap_iz, Dlina_iz,
               Stroit_vis, Kol_vo_iz, Diam_iz, 120 + n, "3");
    Iz_vmeste (Opor_kord.x_travers34, Opor_kord.y_travers34, Opor_kord.x_travers13, Opor_kord.y_travers13, Zoom, do_shap_iz, Dlina_iz,
               Stroit_vis, Kol_vo_iz, Diam_iz, 170 + n, "4");
    Iz_vmeste (Opor_kord.x_travers24, Opor_kord.y_travers24, Opor_kord.x_travers23, Opor_kord.y_travers23, Zoom, do_shap_iz, Dlina_iz,
               Stroit_vis, Kol_vo_iz, Diam_iz, 220 + n, "5");
    Iz_vmeste (Opor_kord.x_travers14, Opor_kord.y_travers14, Opor_kord.x_travers33, Opor_kord.y_travers33, Zoom, do_shap_iz, Dlina_iz,
               Stroit_vis, Kol_vo_iz, Diam_iz, 270 + n, "6");


    int number;
    char name_f [40] = "";
    FILE *name = fopen ("Names.cpp", "r");
    fscanf (name, "%d", &number);
    sprintf (name_f, "История//ballast %d.bmp", number);
    SaveBMP (name_f, txDC(), XWindow, YWindow);
    name = fopen ("Names.cpp", "w");
    number ++;
    fprintf (name, "%d", number);
    fclose (name);
    }

void Iz (double x, double y, double Zoom, double do_shap_iz, double Dlina_iz, int Kol_vo_iz,
         double Diam_iz, double Stroit_vis, double perX, double perY)
    {
    txLine (x * Zoom + perX, -y * Zoom + perY, x * Zoom + perX, -(y - Dlina_iz / 1000) * Zoom + perY);
    for (int i = 0; i < Kol_vo_iz; i += 1)
        {txLine ((x - Diam_iz / 2000) * Zoom + perX, -(y - do_shap_iz / 1000 - 0.5 * Stroit_vis / 1000 - i * Stroit_vis / 1000) * Zoom + perY,
                 (x + Diam_iz / 2000) * Zoom + perX, -(y - do_shap_iz / 1000 - 0.5 * Stroit_vis / 1000 - i * Stroit_vis / 1000) * Zoom + perY);}
    }

void Iz_Naklon (double x, double y, double x2, double y2, double *Ugol_max, double Zoom, double do_shap_iz, double Stroit_vis,
                double Dlina_iz, int Kol_vo_iz, double Diam_iz, double perX, double perY, const char* number)
    {
    double tan_Ugol = (Kg1 * Pw1 + Pu1) / (Gpr + 0.5 * Gg);//4.0555
    double ugol = atan (tan_Ugol);

    double y_nakl = cos (ugol) * Dlina_iz;   //mm
    double x_nakl = sin (ugol) * Dlina_iz;

    if (y2 > y) *Ugol_max = 3.1514/2 - atan(Diam_iz / (2 * (do_shap_iz + Stroit_vis / 2)));
    else        *Ugol_max = acos (do_shap_iz / dist (x, y, x + do_shap_iz, do_shap_iz * (y2 - y) / (x2 - x) + y)) - atan(Diam_iz / (2 * (do_shap_iz + Stroit_vis / 2)));

    double y_nakl_pr = cos (*Ugol_max) * Dlina_iz;   //mm
    double x_nakl_pr = sin (*Ugol_max) * Dlina_iz;

    if (*Ugol_max > ugol) txSetColor (RGB (0, 0, 255), 3);
    else                  txSetColor (RGB (255, 0, 0), 3);

    if (x > 0) txTextOut (x * Zoom + perX + 30, -y * Zoom + perY, number);
    if (x < 0) txTextOut (x * Zoom + perX - 30, -y * Zoom + perY, number);

    if (x > 0) txLine (x * Zoom + perX, -y * Zoom + perY, (x - x_nakl / 1000) * Zoom + perX, -(y - y_nakl / 1000) * Zoom + perY);
    if (x < 0) txLine (x * Zoom + perX, -y * Zoom + perY, (x + x_nakl / 1000) * Zoom + perX, -(y - y_nakl / 1000) * Zoom + perY);
    Izol_po_otdel (x, y, x2, y2, ugol, Zoom, do_shap_iz, Stroit_vis, Dlina_iz, Kol_vo_iz, Diam_iz, perX, perY);

    if (*Ugol_max < ugol)
        {
        txSetColor (RGB (0, 255, 0), 3);
        if (x > 0) txLine (x * Zoom + perX, -y * Zoom + perY, (x - x_nakl_pr / 1000) * Zoom + perX, -(y - y_nakl_pr / 1000) * Zoom + perY);
        if (x < 0) txLine (x * Zoom + perX, -y * Zoom + perY, (x + x_nakl_pr / 1000) * Zoom + perX, -(y - y_nakl_pr / 1000) * Zoom + perY);
        Izol_po_otdel (x, y, x2, y2, *Ugol_max, Zoom, do_shap_iz, Stroit_vis, Dlina_iz, Kol_vo_iz, Diam_iz, perX, perY);
        }
    }

void Izol_po_otdel (double x, double y, double x2, double y2, double ugol, double Zoom, double do_shap_iz, double Stroit_vis,
                    double Dlina_iz, int Kol_vo_iz, double Diam_iz, double perX, double perY)
    {
    double x_nakl_iz = 0, y_nakl_iz = 0, x_iz_l = 0, y_iz_l = 0, x_iz_r = 0, y_iz_r = 0;
    for (int i = 0; i < Kol_vo_iz; i++)
        {
        y_nakl_iz = cos (ugol) * (do_shap_iz + Stroit_vis * (i + 0.5));   //mm
        x_nakl_iz = sin (ugol) * (do_shap_iz + Stroit_vis * (i + 0.5));
        x_iz_l = x_nakl_iz - Diam_iz * sin (txPI / 2 - ugol) / 2;
        y_iz_l = y_nakl_iz + Diam_iz * cos (txPI / 2 - ugol) / 2;
        x_iz_r = x_nakl_iz + Diam_iz * sin (txPI / 2 - ugol) / 2;
        y_iz_r = y_nakl_iz - Diam_iz * cos (txPI / 2 - ugol) / 2;
        if (x > 0) txLine ((x - x_iz_l / 1000) * Zoom + perX, -(y - y_iz_l / 1000) * Zoom + perY, (x - x_iz_r / 1000) * Zoom + perX, -(y - y_iz_r / 1000) * Zoom + perY);
        if (x < 0) txLine ((x + x_iz_l / 1000) * Zoom + perX, -(y - y_iz_l / 1000) * Zoom + perY, (x + x_iz_r / 1000) * Zoom + perX, -(y - y_iz_r / 1000) * Zoom + perY);
        //printf ("[%d] : %lf, %lf", i, (x - x_iz_l / 1000) * Zoom + perX, -(y - y_iz_l / 1000) * Zoom + perY);
        }
    }

double Nahodim_Gb (double Ugol_max)
    {
    double Ugol = 100000;
    double Gb = -0.005;
    for (; Ugol >= Ugol_max; Gb += 0.005)
        {
        Ugol = atan ((Kg1 * Pw1 + Pu1) / (Gpr + 0.5 * Gg + Gb));
        }
    return Gb;
    }

void Otobragenie_Ballast (double Ugol_max, int x, int y)
    {
    double M_ballast = Nahodim_Gb (Ugol_max) / 9.81;
    txSetColor (RGB(0, 255, 0));
    txSetFillColor (TX_WHITE);
    txRectangle (x - 200, y - 20, x + 200, y + 20);
    char text [100] = "";
    sprintf (text, "Нужен балласт массой %lg кг", M_ballast);
    txSelectFont ("Arial", 30, 12, 0);
    txTextOut (x - 200, y - 10, text);
    }

void Iz_vmeste (double x, double y, double x2, double y2, double Zoom, double do_shap_iz, double Dlina_iz,
                double Stroit_vis, double Kol_vo_iz, double Diam_iz, double y_b, const char* number)
    {
    if (x != x2)
        {
        double Ugol_max = 0;
        txSetColor (TX_BLACK, 3);
        Iz (x, y, Zoom, do_shap_iz, Dlina_iz, Kol_vo_iz, Diam_iz, Stroit_vis, 400, 1840);
        Iz_Naklon (x, y, x2, y2, &Ugol_max, Zoom, do_shap_iz, Stroit_vis, Dlina_iz, Kol_vo_iz, Diam_iz, 400, 1840, number);
        Otobragenie_Ballast (Ugol_max, XWindow - 500, YWindow - y_b);
        }
    }

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

    txLock();
    Win32::GetDIBits (dc, (HBITMAP) Win32::GetCurrentObject (dc, OBJ_BITMAP),
                      0, sizeY, buf, (BITMAPINFO*) &info, DIB_RGB_COLORS);
    txUnlock();

    fwrite (&hdr,  sizeof (hdr),  1, f);
    fwrite (&info, sizeof (info), 1, f);
    fwrite (buf,   szImg,         1, f);

    delete[] buf;

    fclose (f);
    return true;

    #pragma GCC diagnostic pop
    }

double dist (int x1, int y1, int x2, int y2)
    {
    return sqrt ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

void Knopka_Vibor (Knop * Kn, Koord_Op op, const char* tekst)
    {
    HDC dc = NULL;
    double Zoom = 10, Zoom_b = 10;
    if (op.y_Stoyka4 != 0) {Zoom = 100 / op.y_Stoyka4; Zoom_b = (YWindow - 20) / op.y_Stoyka4;}

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
        DrawOpora (op, Zoom_b, XWindow - 125, YWindow - 5);
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
    txSetFillColor (TX_TRANSPARENT);
    //txSetColor (TX_BLACK);
    POINT Stoyka [8] = {{op.x_Stoyka1 * Zoom + Sdvig_x, -op.y_Stoyka1 * Zoom + Sdvig_y},
                        {op.x_Stoyka2 * Zoom + Sdvig_x, -op.y_Stoyka2 * Zoom + Sdvig_y},
                        {op.x_Stoyka3 * Zoom + Sdvig_x, -op.y_Stoyka3 * Zoom + Sdvig_y},
                        {op.x_Stoyka4 * Zoom + Sdvig_x, -op.y_Stoyka4 * Zoom + Sdvig_y},
                        {op.x_Stoyka5 * Zoom + Sdvig_x, -op.y_Stoyka5 * Zoom + Sdvig_y},
                        {op.x_Stoyka6 * Zoom + Sdvig_x, -op.y_Stoyka6 * Zoom + Sdvig_y},
                        {op.x_Stoyka7 * Zoom + Sdvig_x, -op.y_Stoyka7 * Zoom + Sdvig_y},
                        {op.x_Stoyka8 * Zoom + Sdvig_x, -op.y_Stoyka8 * Zoom + Sdvig_y}};

    POINT Traversa1 [4] = {{op.x_travers11 * Zoom + Sdvig_x, -op.y_travers11 * Zoom + Sdvig_y},
                           {op.x_travers12 * Zoom + Sdvig_x, -op.y_travers12 * Zoom + Sdvig_y},
                           {op.x_travers13 * Zoom + Sdvig_x, -op.y_travers13 * Zoom + Sdvig_y},
                           {op.x_travers14 * Zoom + Sdvig_x, -op.y_travers14 * Zoom + Sdvig_y}};

    POINT Traversa2 [4] = {{op.x_travers21 * Zoom + Sdvig_x, -op.y_travers21 * Zoom + Sdvig_y},
                           {op.x_travers22 * Zoom + Sdvig_x, -op.y_travers22 * Zoom + Sdvig_y},
                           {op.x_travers23 * Zoom + Sdvig_x, -op.y_travers23 * Zoom + Sdvig_y},
                           {op.x_travers24 * Zoom + Sdvig_x, -op.y_travers24 * Zoom + Sdvig_y}};

    POINT Traversa3 [4] = {{op.x_travers31 * Zoom + Sdvig_x, -op.y_travers31 * Zoom + Sdvig_y},
                           {op.x_travers32 * Zoom + Sdvig_x, -op.y_travers32 * Zoom + Sdvig_y},
                           {op.x_travers33 * Zoom + Sdvig_x, -op.y_travers33 * Zoom + Sdvig_y},
                           {op.x_travers34 * Zoom + Sdvig_x, -op.y_travers34 * Zoom + Sdvig_y}};

    txPolygon (Stoyka, 8);
    txPolygon (Traversa1, 4);
    txPolygon (Traversa2, 4);
    txPolygon (Traversa3, 4);

    txSetFillColor (TX_WHITE);
    }

double Text_Stroka (double *x, double *y, bool *up_or_down)
    {
    int X_min = *x;
    int Kl [11] = {};
    for (int i = 0; i < 10; i ++) Kl [i] = i + 48;
    Kl [10] = 190;

    char Kl_t[11] [40] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "."};

    double Vvod = 0;

    int tochka = 0, kol_zn_posl_zap = 0;

    for (int n = 0; ; n ++)
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
                txLine (*x, *y + 8, *x, *y + 25);
                txSetColor (TX_BLACK);
                text (Kl_t[i], *x, *y + 3, 25);
                *x += 25 / 2.5 + 2;
                txSleep (150);
                }
            }

        if (GetAsyncKeyState (VK_BACK) && tochka == 0 && X_min < *x)
            {
            Vvod = Otbros (Vvod / 10);
            txSetFillColor (TX_WHITE);
            txSetColor (TX_WHITE);
            Clear (*x, *y + 7, *x - 25 / 2.5 - 2, *y + 25);
            *x -= (25 / 2.5 + 2);
            txSleep (150);
            }
        if (GetAsyncKeyState (VK_BACK) && tochka == 1 && X_min < *x)
            {
            if   (kol_zn_posl_zap != 1) Vvod = Otbros (Vvod * (kol_zn_posl_zap - 1)) / (kol_zn_posl_zap - 1);
            else
                {
                Vvod = Otbros (Vvod);
                }
            kol_zn_posl_zap --;
            txSetFillColor (TX_WHITE);
            txSetColor (TX_WHITE);
            Clear (*x, *y + 7, *x - 25 / 2.5 - 2, *y + 25);
            if (kol_zn_posl_zap <= 0)
                {
                tochka = 0;
                Clear (*x - 25 / 2.5 - 2, *y + 7, *x - 50 / 2.5 - 4, *y + 25);
                *x -= (25 / 2.5 + 2);
                }
            *x -= (25 / 2.5 + 2);
            txSleep (150);
            }

        if (GetAsyncKeyState ('M')) system ("Karti.pdf");

        if (GetAsyncKeyState (VK_DOWN) || GetAsyncKeyState (VK_RETURN)) {*up_or_down = false; break;}
        if (GetAsyncKeyState (VK_UP))   {*up_or_down = true ; break;}

        if (n % 200000 == 100000)
            {
            txSetColor (TX_WHITE);
            txLine (*x, *y + 8, *x, *y + 25);
            }
        if (n % 200000 == 0)
            {
            txSetColor (TX_BLACK);
            txLine (*x, *y + 8, *x, *y + 25);
            }
        }

    return Vvod;
    }

void Tablica (double x_razd, double kol_vo_srt, double vis_str)
    {
    for (int i = 1; i <= kol_vo_srt + 1; i++)
        {
        txLine (0, 6 + i * vis_str, XWindow, 6 + i * vis_str);
        }
    txLine (x_razd, vis_str + 6, x_razd, (kol_vo_srt + 1) * vis_str + 6);
    }

void Clear (double x1, double y1, double x2, double y2)
    {
    POINT Pryamoug [4] = {{x1, y1}, {x1, y2}, {x2, y2}, {x2, y1}};
    txPolygon (Pryamoug, 4);
    }

double Otbros (double x)
    {
    int y = x;
    return y;
    }

double Interpolyaciya (double X1, double X2, double Xnugn, double nah1, double nah2)
    {
    return nah1 + (nah2 - nah1) * (Xnugn - X1) / (X2 - X1);
    }

double Pa (double tip)
    {
    if (tip == 1) return 400;
    if (tip == 2) return 500;
    if (tip == 3) return 650;
    if (tip == 4) return 800;
    if (tip == 5) return 1000;
    if (tip == 6) return 1250;
    if (tip == 7) return 1500;
    if (tip >= 8) return 1500;
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
    return Aw * Kl * Kw * Cx * W * F/* * sin (b) * sin (b)*/;
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

double Kw (double Visota, int k)
    {
    if (k == 1)
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

    if (k == 2)
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

    if (k == 3)
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
