/**================================================================================================
 * ?                                           ABOUT:
 ** @author        :  Francesco Pio Nocerino
 * @email          :  nocerpio9@gmail.com
 * @createdOn      :  Jun-2022
 * @dir            :  https://github.com/KekkoDev/Tris_Tic-Tac-Toe.git
 * @filename       :  tris_uomo_vs_pcv2.cpp
 * @description    :  Gioco del Tris (Tic-Tac-Toe)
 *================================================================================================**/
//! AGGIORNAMENTO LE POSIZIONI SCELTE DALLA MACCHINA NON SONO DEL TUTTO CASUALI!
#ifdef _WIN32 //? implementazione Win
#include <windows.h>
/*================================ COLORI ==============================*/
#define GREEN 10
#define RED 12
#define WHITE 15
#define YELLOW 14
#define BLUE 9
#define MAGENTA 13
/*==============*/
#elif __APPLE__
#define GREEN "92"
#define RED "91"
#define YELLOW "93"
#define BLUE "94"
#define WHITE "97"
#define MAGENTA "95"
#define BOLD "1"
#endif
/*=======================================================================*/
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <time.h>
#include <thread>
#include <chrono>
using namespace std;

#define C 3 //! numero di colonne
#define R 3 //! numero di righe

/*================================ INIZIO PROTOTIPI ==============================*/
void cl(); // pulisci schermo
#ifdef _WIN32
void set_console_color(int dim);   //*Win
void size_shell(int h, int w);     //* Ridimensiona la shell
ostream &bold_on(std::ostream &os) //! testo in bold per win
{
    return os << "\e[1m";
}

ostream &bold_off(std::ostream &os)
{
    return os << "\e[0m";
}
#elif __APPLE__ //*MacOs
//! overloading color
void size_shell_MacOS(string h, string w); //* RIdimensiona shell
string color(string word, string back, string frase);
string color(string word, string back, char frase);
string color(string word, string back, int num);
string color(string word, string back, float num);
#endif
/*========================================================================*/

void gen(char &ply, char &pc, int &nG);    // generatore di simbolo
int num(int Max);                          // generazione numero random
void inizializza(int dim, char tab[R][C]); // inizalizza la tavola
void sleep(int d);
void info(char umano, char pc, int punteggio_uomo, int punteggio_pc, int n_partite);
void run(char segno, char segno_macchina, char tab[R][C], bool full, int &n_ply); //* Inserimento da parte dei ply
void ps(int x, int &r, int &c);                                                   // Posizioni
void cntrl_num(int &n);
void giocatore(char segno, char tab[R][C], bool full); // giocatore 1
void giocatore_macchina(char pc, char tab[R][C], bool full);
void stampa(int dim, char segno_umano, char tab[R][C], int n_color); // stampa tavola
/*================================ LOGICA ==============================*/

bool mosse(int dim, char tab[R][C], char segno_uomo, bool &win);
bool orzizzontale(int dim, char tab[R][C], char segno, bool &win);    // controllo
bool verticale(int dim, char tab[R][C], bool &win, char segno_uomo);  // controllo
bool diagonale(int dim, char tab[R][C], bool &win, char segno_uomo);  // controllo
bool diagonale2(int dim, char tab[R][C], bool &win, char segno_uomo); // controllo
bool piena(int dim, char tab[R][C]);
/*================================ GRAFICA ==============================*/

void banner();                   // banner
void banner_vincita(char segno); // Stampa i banner
void banner_X();                 // banner 'X'
void banner_O();                 // banner 'O'
void banner_parita();            // banner parita
void guida();
/*============================ FINE PROTOTIPI ============================*/

int main()
{
    char umano, pc, tab[R][C];
    int conta = 1, punteggio_uomo = 0, punteggio_pc = 0, n_ply;
    bool fine, fine2, fine3, fine4, win, win2;
    bool full;
    const int N_PARTITE = 3, PUNT_MAX = 3; //! numero partite da disputare, numero punteggio massimo
    const int slp1 = 1000, slp2 = 2000;    //* slp1 = 1s, slp2 = 2s
    gen(umano, pc, n_ply);                 //*generazione dei simboli 'X' o 'O'

    inizializza(R, tab);
//? Ridimensionamento shell
#ifdef _WIN32
    size_shell(1100, 600);
#elif __APPLE__
    size_shell_MacOS("130", "25");
#endif

    banner(); //! Stampa banner

#ifdef _WIN32
    size_shell(800, 600);
#elif __APPLE__
    size_shell_MacOS("85", "20");
#endif

    while (conta <= N_PARTITE)
    {
        fine = false;
        fine2 = false;
        full = false;
        win = false;
        win2 = false;

        do
        {
            info(umano, pc, punteggio_uomo, punteggio_pc, conta);
            run(umano, pc, tab, full, n_ply); //! INSERIMENTO
            full = piena(R, tab);             // controllo capacita della matrice
            fine = mosse(R, tab, umano, win);
            fine2 = mosse(R, tab, pc, win2);
            if (fine == true || fine2 == true)
            {
                sleep(slp2);
                cl();
                info(umano, pc, punteggio_uomo, punteggio_pc, conta);
                stampa(R, umano, tab, n_ply);
                fflush(stdin);
                cout << "Premi invio per continuare a giocare.... ";
                cin.get();
                cl();
            }
            else
            {
                sleep(slp1);
                cl();
            }

        } while (fine == false && fine2 == false && full == false);

        if (win == true)
            punteggio_uomo++;
        if (win2 == true)
            punteggio_pc++;

        conta++;
        inizializza(R, tab);
        sleep(slp2);
        cl();
    }
    conta--;
    if (punteggio_uomo > punteggio_pc)
    {
        info(umano, pc, punteggio_uomo, punteggio_pc, conta);
        banner_vincita(umano);
    }
    if (punteggio_pc > punteggio_uomo)
    {
        info(umano, pc, punteggio_uomo, punteggio_pc, conta);
        banner_vincita(pc);
    }

    if (punteggio_uomo == punteggio_pc)
    {
        info(umano, pc, punteggio_uomo, punteggio_pc, conta);
        banner_parita();
    }
    cin.get();
    return 0;
}

/*================================ INIZIO FUNZIONI ==============================*/

void cl()
{
    /**----------------------
     **   CL
     *? Pulisci schermo 
     *@param void  
     *@return void
     *------------------------**/
#ifdef _WIN32
    system("cls");
#elif (__APPLE__)
    system("clear");
#endif
}

//? Colori x tutti i SO
#ifdef _WIN32
/**----------------------
 **   SET_CONSOLE_COLOR
 *? Colori x shell Win
 *@param cl int  
 *@return void
 *------------------------**/
void set_console_color(int cl)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, cl);
}
/**----------------------
 **   SIZE_SHELL
 *? Ridimensionamento terminale Win
 *@param cl int  
 *@return void
 *------------------------**/
void size_shell(int h, int w) //! Ridimensiona shell data altezza e larghezza
{
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);

    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, h, w, TRUE);
}
/*=============================================*/
#elif __APPLE__
/**----------------------
 **   SIZE_SHELL_MacOS
 *? Ridimensionamento terminale MacOS
 *@param h e w string  
 *@return void
 *------------------------**/
void size_shell_MacOS(string h, string w) //!Ridimensiona shell data altezza e largehzza su UNIX
{
    cout.flush();
    cout << "\e[8;" + w + ";" + h + "t";
}
/**----------------------
 **   COLOR
 *? Colori x shell MacOS
 *@param word, back e frase string  
 *@return string
 *------------------------**/
string color(string word, string back, string frase)
{
    string tot = "\033[" + back + ";" + word + "m" + frase + "\033[0m";
    return tot;
}
string color(string word, string back, char frase)
{
    string frase2(1, frase);
    string tot = "\033[" + back + ";" + word + "m" + frase2 + "\033[0m";
    return tot;
}
string color(string word, string back, int num)
{
    string n = to_string(num);
    string tot = "\033[" + back + ";" + word + "m" + n + "\033[0m";
    return tot;
}
string color(string word, string back, float num)
{
    string n = to_string(num);
    string tot = "\033[" + back + ";" + word + "m" + n + "\033[0m";
    return tot;
}
#endif

/*========================================================================*/

void gen(char &ply, char &pc, int &nG)
{
    /**------------------------------------------------------------------------
     **                           GEN
     *?  Assegna i simboli ai giocatori a random
     *@param &ply, &pc char  
     *@param &nG int  
     *@return void
     *------------------------------------------------------------------------**/
    int n = num(2), n2 = num(2);
    bool trv = false;
    char v[2] = {'X', 'O'};

    ply = v[n];
    pc = v[n2];

    do
    {
        if (ply != pc)
            trv = true;
        if (trv == false)
        {
            n = num(2);
            n2 = num(2);
            ply = v[n];
            pc = v[n2];
        }
    } while (!trv);

    nG = n;
}

int num(int Max)
{
    /**----------------------------------------------
     **              NUM
     *?  Generatore di numeri casuali 
     *@param Max int  
     *@return int
     *---------------------------------------------**/
    srand(unsigned(time(NULL)));
    int n = rand() % Max;
    return n;
}

void inizializza(int dim, char tab[R][C])
{
    /**----------------------
     **   INIZIALIZZA
     *? Inizializzazione tabella 
     *@param dim int
     *@param tab[][] char
     *@return void
     *------------------------**/
    int conta = 1;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            tab[i][j] = conta + '0';
            conta++;
        }
    }
}

void sleep(int d)
{
/**----------------------
 **   SLEEP
 *?   Implementazione sleep x tutti i SO
 *@param d int  
 *@return void
 *------------------------**/
#ifdef _WIN32
    Sleep(d);
#elif __APPLE__
    this_thread::sleep_for(chrono::milliseconds(d));
#endif
}

void info(char umano, char pc, int punteggio_uomo, int punteggio_pc, int n_partite)
{
    /**----------------------------------------------
     **              INFO
     *?  Stampa informazioni sulla partita  punteggio e numero partite
     *@param umano, pc char  
     *@param punteggio_uomo, punteggio_pc e n_partite int  
     *@return voids
     *---------------------------------------------**/
#ifdef _WIN32
    cout << "PLY > ";
    set_console_color(GREEN);
    cout << umano;
    set_console_color(WHITE);
    cout << setw(8) << "PC > ";
    set_console_color(RED);
    cout << pc << endl;
    set_console_color(WHITE);
    cout << "------------------\n";
    set_console_color(GREEN);
    cout << "PLY: ";
    set_console_color(WHITE);
    cout << punteggio_uomo;
    set_console_color(RED);
    cout << setw(8) << "PC: ";
    set_console_color(WHITE);
    cout << punteggio_pc << endl;
    set_console_color(WHITE);
    cout << "------------------\n";
#elif __APPLE__
    cout << color(YELLOW, BOLD, "PARTITE : ") << color(BLUE, "", n_partite) << endl;
    cout << "------------------\n";
    cout << "PLY > " << color(GREEN, "", umano) << setw(8) << "PC > " << color(RED, "", pc) << endl;
    cout << "------------------\n";
    cout << color(GREEN, BOLD, "PLY : ") << punteggio_uomo << setw(10) << color(RED, BOLD, "   PC : ") << punteggio_pc << endl;
    cout << "------------------\n";
#endif
}

void run(char segno, char segno_macchina, char tab[R][C], bool full, int &n_ply)
{
    /**------------------------------------------------------------------------
   **                           RUN
   *?  Inserimento e stampa 
   *@param segno, segno_macchina e tab[][] char  
   *@param full bool  
   *@param &n_ply int
   *@return void
   *------------------------------------------------------------------------**/
#ifdef _WIN32
    if (n_ply == 0)
    {
        cout << "TURNO > ";
        set_console_color(GREEN);
        cout << segno << endl;
        set_console_color(WHITE);
        stampa(C, segno, tab, n_ply);
        giocatore(segno, tab, full);
        n_ply++;
    }
    else
    {

        cout << "TURNO > ";
        set_console_color(RED);
        cout << segno_macchina << endl;
        set_console_color(WHITE);
        stampa(C, segno, tab, n_ply);
        giocatore_macchina(segno_macchina, tab, full);
        n_ply--;
    }
#elif __APPLE__
    if (n_ply == 0)
    {
        cout << "TURNO > " << color(GREEN, "", segno) << endl;
        stampa(C, segno, tab, n_ply);
        giocatore(segno, tab, full);
        n_ply++;
    }
    else
    {
        cout << "TURNO > " << color(RED, "", segno_macchina) << endl;
        stampa(C, segno, tab, n_ply);
        giocatore_macchina(segno_macchina, tab, full);
        n_ply--;
    }
#endif
}

void ps(int x, int &r, int &c)
{
    /**------------------------------------------------------------------------
     **                           PS
     *?  Scelto il numero dalla griglia restituisce la posizione 
     *@param &x, &r e &c int  
     *@return void
     *------------------------------------------------------------------------**/

    switch (x)
    {
    case 1:
        r = 0;
        c = 0;
        break;
    case 2:
        r = 0;
        c = 1;
        break;
    case 3:
        r = 0;
        c = 2;
        break;
    case 4:
        r = 1;
        c = 0;
        break;
    case 5:
        r = 1;
        c = 1;
        break;
    case 6:
        r = 1;
        c = 2;
        break;
    case 7:
        r = 2;
        c = 0;
        break;
    case 8:
        r = 2;
        c = 1;
        break;
    case 9:
        r = 2;
        c = 2;
        break;
    }
}

void cntrl_num(int &x)
{
    cout << "Inserisci numero della posizione  > ";
    cin >> x;
    bool trv = false;
    do
    {
        if (x <= 9 && x > 0)
            trv = true;
        if (!trv)
        {
            cout << "Posizione NON valida!!\n";
            cout << "Inserisci numero della posizione  > ";
            cin >> x;
        }
    } while (!trv);
}

void giocatore(char segno, char tab[R][C], bool full)
{
    /**------------------------------------------------------------------------
     **                           GIOCATORE
     *?  Permette l'input ai giocatori 
     *@param segno, tab[][] char  
     *@param full bool  
     *@return void
     *------------------------------------------------------------------------**/
    int riga, colonna, n;
    bool trv = false;
    if (full)
        trv = true;
    cntrl_num(n);
    ps(n, riga, colonna);
    do
    {
        //! Controllo sulle posizoni
        if (tab[riga][colonna] >= '0' && tab[riga][colonna] <= '9') // segno_macchina
        {
            trv = true;
            tab[riga][colonna] = segno;
        }
        if (trv == false) //* Posizione occupata
        {
            cout << "\t!!\n";
            cout << "POSIZIONE OCCUPATA\n";
            cntrl_num(n);
            ps(n, riga, colonna);
        }
    } while (!trv);
}

void giocatore_macchina(char pc, char tab[R][C], bool full)
{
    int n, riga, colonna;
    bool trv = false, m = false;
    if (full)
        trv = true;
    n = num(10);
    ps(n, riga, colonna);
    do
    {
        //! Controllo sulle posizoni
        m = mosse(3, tab, pc, m);
        if (tab[riga][colonna] >= '0' && tab[riga][colonna] <= '9' && !m)
        {
            trv = true;
            tab[riga][colonna] = pc;
        }
        if (trv == false) //* Posizione occupata
        {
            n = num(10);
            ps(n, riga, colonna);
        }
    } while (!trv);
}

void stampa(int dim, char segno_umano, char tab[R][C], int n_color)
{
    //* Stampa della tabella

    const int n_v = 1; //? numero riga x la stampa

    cout << endl;

    for (int i = 0; i < dim; i++) //! Viene stampata in qst modo per un concetto di manetenimento del codice!
    {
        //* Se si vuole amumentare la capacita della tabella verra stampata correttamente!
        for (int j = 0; j < n_v; j++)
        {
            //! IL colore della tabella varia a seconda dell utente che deve giocare
            if (n_color == 0)
            {
#ifdef _WIN32
                set_console_color(WHITE);
                cout << setw(26) << "  ";
                set_console_color(GREEN);
                cout << "  |   ";
                cout << " |   " << endl;
                set_console_color(WHITE);
                cout << setw(25) << " " << tab[i][j];
                set_console_color(GREEN);
                cout << "  |  ";
                set_console_color(WHITE);
                cout << tab[i][j + 1];
                set_console_color(GREEN);
                cout << " |  ";
                set_console_color(WHITE);
                cout << tab[i][j + 2] << endl;
                set_console_color(WHITE);
#elif __APPLE__
                cout << setw(26) << " " << color(GREEN, "", "  |   ") << color(GREEN, "", " |   ") << endl;
                cout << setw(25) << " " << tab[i][j] << color(GREEN, "", "  |  ") << tab[i][j + 1] << color(GREEN, "", " |  ") << tab[i][j + 2] << endl;
#endif
            }
            else
            {
#ifdef _WIN32
                set_console_color(WHITE);
                cout << setw(26) << "  ";
                set_console_color(RED);
                cout << "  |   ";
                cout << " |   " << endl;
                set_console_color(WHITE);
                cout << setw(25) << " " << tab[i][j];
                set_console_color(RED);
                cout << "  |  ";
                set_console_color(WHITE);
                cout << tab[i][j + 1];
                set_console_color(RED);
                cout << " |  ";
                set_console_color(WHITE);
                cout << tab[i][j + 2] << endl;
                set_console_color(WHITE);
#elif __APPLE__
                cout << setw(26) << " " << color(RED, "", "  |   ") << color(RED, "", " |   ") << endl;
                cout << setw(25) << " " << tab[i][j] << color(RED, "", "  |  ") << tab[i][j + 1] << color(RED, "", " |  ") << tab[i][j + 2] << endl;
#endif
            }

            if (i == dim - 1) //? Si blocca per un fattore puramente estetico!!
                break;        //! BREAK utilizzato per arrestare il prgramma

            if (n_color == 0)
            {
#ifdef _WIN32
                set_console_color(GREEN);
                cout << setw(39) << "____|____|____\n";
                set_console_color(WHITE);
#elif __APPLE__
                cout << setw(49) << color(GREEN, "", "____|____|____\n");
#endif
            }
            else
            {
#ifdef _WIN32
                set_console_color(RED);
                cout << setw(39) << "____|____|____\n";
                set_console_color(WHITE);
#elif __APPLE__
                cout << setw(49) << color(RED, "", "____|____|____\n");
#endif
            }
        }
    }

    cout << endl;
}

/*================================ LOGICA ==============================*/

bool mosse(int dim, char tab[R][C], char segno_uomo, bool &win)
{
    /**------------------------------------------------------------------------
     **                           MOSSE
     *?  Controllo delle mosse da parte dei giocatori
     *@param dim int  
     *@param tab[][], segno_uomo char  
     *@param &win bool  
     *@return bool
     *------------------------------------------------------------------------**/
    bool m1 = false, m2 = false, m3 = false, m4 = false;
    bool r;
    m1 = orzizzontale(dim, tab, segno_uomo, win); // controllo mosse in linea orizzontale
    m2 = verticale(dim, tab, win, segno_uomo);    // controllo mosse in linea verticale
    m3 = diagonale(dim, tab, win, segno_uomo);    // controllo mosse nella diagonale principale
    m4 = diagonale2(dim, tab, win, segno_uomo);   // controllo mosse antidiagonale
    if (m1 || m2 || m3 || m4)
        r = true;
    else
        r = false;
    win = r;
    return r;
}

bool orzizzontale(int dim, char tab[R][C], char segno, bool &win)
{
    /**----------------------
     **   ORIZZONTALE
     *? Controllo linee orizontali
     *@param dim int  
     *@param tab[][], segno char  
     *@param &win bool  
     *@return bool
     *------------------------**/
    bool trv = false;
    int x;
    for (int i = 0; i < dim && !trv; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            if (tab[i][j] == segno && tab[i][j + 1] == segno && tab[i][j + 2] == segno)
            {
                trv = true;
                x = i;
            }
        }
    }
    win = trv;
    if (trv == true)
    {
        for (int j = 0; j < dim; j++)
        {
            tab[x][j] = '-';
        }
    }
    return trv;
}

bool verticale(int dim, char tab[R][C], bool &win, char segno_uomo)
{
    /**----------------------
     **   VERTICALE
     *? Controllo linee verticali
     *@param dim int  
     *@param tab[][], segno char  
     *@param &win bool  
     *@return bool
     *------------------------**/
    bool trv = false, fine = false;
    int conta = 0, i, j = 0, p;
    while (conta < dim && !fine)
    {
        p = 0;
        i = 0;
        for (int x = 0; x < dim; x++)
        {
            if (tab[i][j] == segno_uomo)
                p++;
            i++;
        }
        if (p == 3)
            fine = true;
        j++;
        conta++;
    }

    if (fine)
        trv = true;
    else
        trv = false;

    win = trv;
    if (win)
    {
        i--;
        j--;
        for (; i >= 0; i--)
        {
            tab[i][j] = '|';
        }
    }

    return trv;
}

bool diagonale(int dim, char tab[R][C], bool &win, char segno_uomo)
{
    /**----------------------
     **   DIAGONALE
     *? Controllo diagonale
     *@param dim int  
     *@param tab[][], segno char  
     *@param &win bool  
     *@return bool
     *------------------------**/
    bool trv = false;
    int i = 0, conta = 0, conta2 = 0;

    for (int j = 0; j < dim; j++)
    {
        if (tab[i][j] == segno_uomo)
            conta++;
        else
            conta2++;
        i++;
    }

    if (conta == 3)
        trv = true;
    else
        trv = false;

    win = trv;

    if (win)
    {
        i = 0;
        for (int j = 0; j < dim; j++)
        {
            tab[i][j] = '\\';
            i++;
        }
    }

    return trv;
}

bool diagonale2(int dim, char tab[R][C], bool &win, char segno_uomo)
{
    /**----------------------
     **   DIAGONALE2
     *? Controllo antidiagonale
     *@param dim int  
     *@param tab[][], segno char  
     *@param &win bool  
     *@return bool
     *------------------------**/
    bool trv = false;
    int i = dim - 1, conta = 0, conta2 = 0;

    for (int j = 0; j < dim; j++)
    {
        if (tab[i][j] == segno_uomo)
            conta++;
        else
            conta2++;
        i--;
    }

    if (conta == 3)
        trv = true;
    else
        trv = false;

    win = trv;

    if (win)
    {
        i = dim - 1;
        if (win)
        {
            for (int j = 0; j < dim; j++)
            {
                tab[i][j] = '/';
                i--;
            }
        }
    }
    return trv;
}

bool piena(int dim, char tab[R][C])
{
    /**----------------------
     **   PIENA
     *? Controllo tabella se piena
     *@param dim int  
     *@param tab[][] char
     *@return bool
     *------------------------**/
    bool trv = false;
    int conta = 0;

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (tab[i][j] == 'X' || tab[i][j] == 'O')
                conta++;
        }
    }

    if (conta >= 9)
        trv = true;
    else
        trv = false;

    return trv;
}

/*================================ GRAFICA ==============================*/

void banner()
{
/**----------------------
 **   BANNER
 *? Banner inziale
 *@return void
 *------------------------**/
#ifdef _WIN32
    cout << "\t\t\t\t__  __    _    ___    _   __  __\n";
    cout << "\t\t\t\t\\ \\/ /   | |  / _ \\  | |  \\ \\/ /\n";
    cout << "\t\t\t\t >  <    | | | (_) | | |   >  <\n";
    cout << "\t\t\t\t/_/\\_\\   | |  \\___/  | |  /_/\\_\\ \n";
    cout << "\t\t\t\t         |_|         |_|\n";
    cout << "\t\t\t\t ___  ___    ___  ___    ___  ___\n";
    cout << "\t\t\t\t|___||___|  |___||___|  |___||___|\n";
    cout << "\t\t\t\t__  __    _   __  __   _   __  __\n";
    cout << "\t\t\t\t\\ \\/ /   | |  \\ \\/ /  | |  \\ \\/ /\n";
    cout << "\t\t\t\t >  <    | |   >  <   | |   >  <\n";
    cout << "\t\t\t\t/_/\\_\\   | |  /_/\\_\\  | |  /_/\\_\\ \n";
    cout << "\t\t\t\t         |_|          |_|\n";
    cout << "\t\t\t\t ___  ___    ___  ___    ___  ___\n";
    cout << "\t\t\t\t|___||___|  |___||___|  |___||___|\n";
    cout << "\t\t\t\t__  __    _    ___    _   __  __\n";
    cout << "\t\t\t\t\\ \\/ /   | |  / _ \\  | |  \\ \\/ /\n";
    cout << "\t\t\t\t >  <    | | | (_) | | |   >  <\n";
    cout << "\t\t\t\t/_/\\_\\   | |  \\___/  | |  /_/\\_\\ \n";
    cout << "\t\t\t\t         |_|         |_|\n";
    cout << "\n";
    set_console_color(RED);
    cout << "\t\t\t _____ ____  ___ ____           __  _______  _____\n";
    cout << "\t\t\t|_   _|  _ \\|_ _/ ___|          \\ \\/ / _ \\ \\/ / _ \\ \n";
    set_console_color(GREEN);
    cout << "\t\t\t  | | | |_) || |\\___ \\   _____   \\  / | | \\  / | | |\n";
    set_console_color(YELLOW);
    cout << "\t\t\t  | | |  _ < | | ___) | |_____|  /  \\ |_| /  \\ |_| |\n";
    set_console_color(BLUE);
    cout << " \t\t\t  |_| |_| \\_\\___|____/          /_/\\_\\___/_/\\_\\___/\n";
    set_console_color(GREEN);
    cout << setw(40) << bold_on << "By:FRANCESCO PIO NOCERINO\n"
         << bold_off;
    set_console_color(WHITE);
    cout << endl
         << endl;
    /*=============================================*/

#elif __APPLE__
    cout << endl;
    cout << "\t\t\t\t   ▚▗▘   ▐   ▞▀▖  ▐  ▚▗▘\n";
    cout << "\t\t\t\t   ▗▚    ▐   ▌ ▌  ▐  ▗▚ \n";
    cout << "\t\t\t\t   ▘ ▘   ▐   ▝▀   ▐  ▘ ▘\n";
    cout << "\t\t\t\t   ▄▄▖▄▄▖▝ ▄▄▖▄▄▖ ▝ ▄▄▖▄▄▖\n\n";
    cout << "\t\t\t\t   ▚▗▘   ▐   ▚▗▘  ▐  ▚▗▘\n";
    cout << "\t\t\t\t   ▗▚    ▐   ▗▚   ▐  ▗▚ \n";
    cout << "\t\t\t\t   ▘ ▘   ▐   ▘ ▘  ▐  ▘ ▘\n\n";
    cout << "\t\t\t\t   ▄▄▖▄▄▖▝ ▄▄▖▄▄▖ ▝ ▄▄▖▄▄▖\n\n";
    cout << "\t\t\t\t   ▚▗▘   ▐   ▞▀▖  ▐  ▚▗▘\n";
    cout << "\t\t\t\t   ▗▚    ▐   ▌ ▌  ▐  ▗▚ \n";
    cout << "\t\t\t\t   ▘ ▘   ▐   ▝▀   ▐  ▘ ▘\n";
    cout << "\t\t\t\t         ▝        ▝ \n";
    cout << " \n";
    cout << color(RED, "", "\t\t\t\t▀▛▘▛▀▖▜▘▞▀▖      ▌ ▌ ▞▀▖ ▌ ▌ ▞▀▖ \n");
    cout << color(GREEN, "", "\t\t\t\t ▌ ▙▄▘▐ ▚▄   ▄▄▖ ▝▞  ▌ ▌ ▝▞  ▌ ▌\n");
    cout << color(YELLOW, "", "\t\t\t\t ▌ ▌▚ ▐ ▖ ▌      ▞▝▖ ▌ ▌ ▞▝▖ ▌ ▌\n");
    cout << color(BLUE, "", "\t\t\t\t ▘ ▘ ▘▀▘▝▀       ▘ ▘ ▝▀  ▘ ▘ ▝▀\n");
    cout << setw(72) << color(GREEN, BOLD, "By:FRANCESCO PIO NOCERINO\n");
#endif
    cout << " Premere un Tasto per inizare.....\n";
    cin.get();
    cl();
    guida();
}

void banner_vincita(char segno)
{
    /**----------------------
    **   SIZE_SHELL_MacOS
    *? Dato il segno del vincitore stampa il suo rispettivo banner Win
    *@param segno char  
    *@return void
    *------------------------**/
    switch (segno)
    {
    case 'X':
        banner_X();
        break;
    case 'O':
        banner_O();
        break;
    }
}

void banner_O()
{
    /**----------------------
    **   BANNER_O
    *? Banner per il simbolo 'O' 
    *@return void
    *------------------------**/

#ifdef _WIN32

    set_console_color(GREEN);
    cout << setw(51) << "__        _____ _   _       ";
    set_console_color(MAGENTA);
    cout << "___  _\n";
    set_console_color(GREEN);
    cout << setw(50) << "\\ \\      / /_ _| \\ | |     ";
    set_console_color(MAGENTA);
    cout << "/ _ \\| |\n";
    set_console_color(GREEN);
    cout << setw(49) << " \\ \\ /\\ / / | ||  \\| |    ";
    set_console_color(MAGENTA);
    cout << "| | | | |\n";
    set_console_color(GREEN);
    cout << setw(49) << "  \\ V  V /  | || |\\  |    ";
    set_console_color(MAGENTA);
    cout << "| |_| |_|\n";
    set_console_color(GREEN);
    cout << setw(50) << "   \\_/\\_/  |___|_| \\_|     ";
    set_console_color(MAGENTA);
    cout << "\\___/(_)\n";
    set_console_color(WHITE);
    /*=============================================*/

#elif __APPLE__

    cout << setw(80) << color(BLUE, "", " ▌ ▌▜▘▙ ▌") << color(MAGENTA, "", " ▞▀▖ ▐\n");
    cout << setw(82) << color(BLUE, "", " ▌▖▌▐ ▌▌▌") << color(MAGENTA, "", " ▌ ▌ ▐\n");
    cout << setw(82) << color(BLUE, "", " ▙▚▌▐ ▌▝▌") << color(MAGENTA, "", " ▝▀  ▝\n");
    cout << setw(80) << color(BLUE, "", " ▘ ▘▀▘▘ ▘") << color(MAGENTA, "", "     ▝\n");

#endif
}

void banner_X()
{
    /**----------------------
    **   BANNER_X
    *? Banner per il simbolo 'X' 
    *@return void
    *------------------------**/

#ifdef _WIN32

    set_console_color(GREEN);
    cout << setw(50) << "__        _____ _   _     ";
    set_console_color(MAGENTA);
    cout << "__  ___\n";
    set_console_color(GREEN);
    cout << setw(50) << "\\ \\      / /_ _| \\ | |    ";
    set_console_color(MAGENTA);
    cout << "\\ \\/ / |\n";
    set_console_color(GREEN);
    cout << setw(51) << " \\ \\ /\\ / / | ||  \\| |     ";
    set_console_color(MAGENTA);
    cout << "\\  /| |\n";
    set_console_color(GREEN);
    cout << setw(51) << "  \\ V  V /  | || |\\  |     ";
    set_console_color(MAGENTA);
    cout << "/  \\|_|\n";
    set_console_color(GREEN);
    cout << setw(50) << "   \\_/\\_/  |___|_| \\_|    ";
    set_console_color(MAGENTA);
    cout << "/_/\\_(_)\n";
    set_console_color(WHITE);
    /*=============================================*/

#elif __APPLE__

    cout << setw(80) << color(BLUE, "", " ▌ ▌▜▘▙ ▌") << color(MAGENTA, "", " ▚▗▘ ▐\n");
    cout << setw(82) << color(BLUE, "", " ▌▖▌▐ ▌▌▌") << color(MAGENTA, "", " ▗▚  ▐\n");
    cout << setw(82) << color(BLUE, "", " ▙▚▌▐ ▌▝▌") << color(MAGENTA, "", " ▘ ▘ ▝\n");
    cout << setw(80) << color(BLUE, "", " ▘ ▘▀▘▘ ▘") << color(MAGENTA, "", "     ▝\n");

#endif
}

void banner_parita()
{
/**----------------------
    **   BANNER_PARITA
    *? Banner pari
    *@return void
    *------------------------**/
#ifdef _WIN32
    set_console_color(YELLOW);
    cout << " ____   _    ____  ___ _______   ___\n";
    cout << "|  _ \\ / \\  |  _ \\|_ _|_   _\\ \\ / / |\n";
    cout << "| |_) / _ \\ | |_) || |  | |  \\ V /| |\n";
    cout << "|  __/ ___ \\|  _ < | |  | |   | | |_|\n";
    cout << "|_| /_/   \\_\\_| \\_\\___| |_|   |_| (_)\n";
    set_console_color(WHITE);
    /*=============================================*/

#elif __APPLE__
    cout << setw(110) << color(YELLOW, "", " ▛▀▖▞▀▖▛▀▖▜▘▀▛▘▌ ▌ ▐\n");
    cout << setw(105) << color(YELLOW, "", " ▙▄▘▙▄▌▙▄▘▐  ▌ ▝▞  ▐\n ");
    cout << setw(93) << color(YELLOW, "", "▌  ▌ ▌▌▚ ▐  ▌  ▌  ▝\n");
    cout << setw(96) << color(YELLOW, "", " ▘  ▘ ▘▘ ▘▀▘ ▘  ▘  ▝\n");

#endif
}

void guida()
{
    /**----------------------
    **   GUIDA
    *? Guida al gioco
    *@return void
    *------------------------**/
#ifdef _WIN32
    set_console_color(YELLOW);
    cout << setw(50) << bold_on << "GUIDA AL GIOCO!\n"
         << bold_off;
    set_console_color(WHITE);
    cout << bold_on << " Regole e svolgimento del gioco : \n"
         << bold_off;
#elif __APPLE__
    cout << setw(80) << color(YELLOW, BOLD, "GUIDA AL GIOCO!\n");
    cout << color(WHITE, BOLD, " Regole e svolgimento del gioco : \n");
#endif

    cout << endl;
    cout << " il tris viene giocato da due giocatori uno di essi giocherà utilizzando le crocette, l'altro dei piccoli cerchi(\'X\'-\'O\').\n";
    cout << endl;
    cout << " Si gioca su una griglia di 3X3 caselle (La griglia si colora in base all turno del giocatore). Il giocatore di turno occupa  \n";
    cout << endl;
    cout << " una delle caselle con il suo simbolo, poi cede la mossa all'avversario. Vince il primo giocatore che riesce ad ottenere un tris,\n";
    cout << endl;
    cout << " ovvero tre simboli adiacenti in riga, in colonna o lungo una delle diagonali. Se tutte e 9 le caselle vengono occupate \n";
    cout << endl;
    cout << " senza che uno dei giocatori abbia ottenuto un tris, la partita termina con un pareggio." << endl;
    cout << endl;
    cout << " BUON DIVERTIMETO!...";
    cin.get();
    cl();
}

/*========================================== FINE PROGRAMMA ==========================================*/
