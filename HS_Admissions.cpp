#include <fstream>
//#include <iostream>
#include <algorithm>
#include <iomanip>
#include <stack>

// Dăian Tudor, 26-29 iunie 2024, din lipsă de alte chestii de făcut la orele nopții :)))

using namespace std;

ifstream cin("file.in");
ofstream cout("file.out");

struct elev {
    // Date primite
    string codificare, nume, prenume, initiale;
    string proba_matern_bilingv; // - dacă nu a susținut sau nu a echivalat cu un atestat lingvistic; X dacă a susținut sau a echivalat cu un atestat lingvistic, pe limba
    string proba_vocationala; /// - dacă nu a susținut probă vocațională; ARTISTIC_.../SPORTIV_.../FILOLOGIE-WALDORF/TEOLOGIE_.../MILITAR/OSP
    short optiuniSpecializari[ 255 ], nrOptiuneRepartizata;
    float medieGimnaziu, notaMatematica, notaLbMaterna, notaLbRomana;
    
    // Date prelucrate
    float medieEvaluareNationala;
    short optiuneAdmisa;
};

struct specializare {
    // Date primite
    string numeUnitateInvatamant, numeSpecializare, vocational, bilingv_matern;
    short numarLocuri;
    
    // Date prelucrate
    float ultimaMedie;
    short numarLocuriOcupate;
    stack<elev>eleviAdmisi;
};

elev A[ 200000 ];
specializare B[ 1000 ];
int numA, numB, Bst = 1001, Bdr = -1, medieMatematica = 0, medieLbRom = 0, medieLbMat = 0, materni = 0;

void read()
{
    cin >> numA >> numB;
    for ( int i = 1; i <= numA; ++i )
    {
        cin >> A[ i ].codificare;
        cin >> A[ i ].nume >> A[ i ].initiale >> A[ i ].prenume;
        cin >> A[ i ].medieGimnaziu >> A[ i ].notaLbRomana >> A[ i ].notaLbMaterna >> A[ i ].notaMatematica;
        if ( A[ i ].notaLbMaterna == 0 )
                A[ i ].medieEvaluareNationala = ( A[ i ].notaMatematica + A[ i ].notaLbRomana ) / 2;
        else
                A[ i ].medieEvaluareNationala = max( ( A[ i ].notaLbMaterna + A[ i ].notaLbRomana + A[ i ].notaMatematica ) / 3, ( A[ i ].notaMatematica + A[ i ].notaLbMaterna ) / 2 );
        cin >> A[ i ].proba_matern_bilingv >> A[ i ].proba_vocationala;
        int x; cin >> x; // nr specializări
        for ( int j = 1; j <= x; ++j )
            cin >> A[ i ].optiuniSpecializari[ j ];
        medieMatematica += A[ i ].notaMatematica;
        medieLbMat += A[ i ].notaLbMaterna;
        if ( A[ i ].notaLbMaterna != 0 )
            materni++;
        medieLbRom += A[ i ].notaLbRomana;
    }
    for ( int i = 1; i <= numB; ++i )
    {
        int codificare;
        cin >> codificare;
        cin >> B[ codificare ].numeSpecializare;
        cin >> B[ codificare ].numeUnitateInvatamant;
        cin >> B[ codificare ].bilingv_matern;
        cin >> B[ codificare ].vocational;
        cin >> B[ codificare ].numarLocuri;
        B[ codificare ].numarLocuriOcupate = 0;
        Bdr = max( Bdr, codificare );
        Bst = min( Bst, codificare );
    }
}

bool comparatorElevi( elev x, elev y )
{
    if ( x.medieEvaluareNationala > y.medieEvaluareNationala )
        return true;
    if ( x.medieEvaluareNationala < y.medieEvaluareNationala )
        return false;
    
    if ( x.medieGimnaziu > y.medieGimnaziu )
        return true;
    if ( x.medieGimnaziu < y.medieGimnaziu )
        return false;
    
    if ( x.notaLbRomana > y.notaLbRomana )
        return true;
    else
        return false;
}

int main()
{
    ios_base::sync_with_stdio(NULL);
    cin.tie(0);
    cout.tie(0);
    
    int nrEleviRep1Opt = 0, nrEleviRep2Opt = 0, nrEleviRep3Opt = 0;
    cout << "Program de repartizare computerizată a elevilor absolvenți ai claselor a VIII-a\n\n";
    read();
    cout << "Începem să repartizăm elevii. \n\n";
    sort( A + 1, A + 1 + numA, comparatorElevi );
    for ( int i = 1; i <= numA; ++i )
    {
        cout << "Repartizam elevul " << A[ i ].nume << " " << A[ i ].prenume << ", cu codul " << A[ i ].codificare <<".\n";
        cout << "Medie la EN : " << A[ i ].medieEvaluareNationala << " , Medie Gimnaziu : " << A[ i ].medieGimnaziu << ".\n";
        for ( int j = 1; j <= 255; ++j )
        {
            if ( B[ A[ i ].optiuniSpecializari[ j ] ].bilingv_matern != A[ i ].proba_matern_bilingv && B[ A[ i ].optiuniSpecializari[ j ] ].bilingv_matern != "-" )
                continue;
            if ( B[ A[ i ].optiuniSpecializari[ j ] ].vocational != A[ i ].proba_vocationala && B[ A[ i ].optiuniSpecializari[ j ] ].vocational != "-" )
                continue;
            if ( A[ i ].optiuniSpecializari[ j ] == 0 ) // S-au terminat optiunile, elevul nu poate fi repartizat
            {
                cout << "Elevul "<< A[ i ].nume << " " << A[ i ].prenume << ", cu codul " << A[ i ].codificare << " nu putut fi repartizat.\n";
                A[ i ].optiuneAdmisa = -1;
                break;
            }
            cout << "Suntem la specializarea " << j << ", la " << B[ A[ i ].optiuniSpecializari[ j ] ].numeUnitateInvatamant << ", cu codul specializării " << A[ i ].optiuniSpecializari[ j ] << "\n";
            if ( B[ A[ i ].optiuniSpecializari[ j ] ].numarLocuriOcupate < B[ A[ i ].optiuniSpecializari[ j ] ].numarLocuri )
            {
                // Va fi admis la a j-a specializare
                cout << "Elevul " << A[ i ].nume << " " << A[ i ].prenume << ", cu codul " << A[ i ].codificare << " a fost repartizat la specializarea " << A[ i ].optiuniSpecializari[ j ] << " de la " << B[ A[ i ].optiuniSpecializari[ j ] ].numeUnitateInvatamant << ".\n";
                A[ i ].optiuneAdmisa = A[ i ].optiuniSpecializari[ j ];
                A[ i ].nrOptiuneRepartizata = j;
                B[ A[ i ].optiuniSpecializari[ j ] ].numarLocuriOcupate++;
                B[ A[ i ].optiuniSpecializari[ j ] ].eleviAdmisi.push( A[ i ] );
                if ( B[ A[ i ].optiuniSpecializari[ j ] ].numarLocuriOcupate == B[ A[ i ].optiuniSpecializari[ j ] ].numarLocuri )
                {
                    cout << "El a fost ultimul elev admis la specializarea " << A[ i ].optiuniSpecializari[ j ] << " de la " << B[ A[ i ].optiuniSpecializari[ j ] ].numeUnitateInvatamant << ". Ultima medie de admitere este " << A[ i ].medieEvaluareNationala << ".\n";
                    B[ A[ i ].optiuniSpecializari[ j ] ].ultimaMedie = A[ i ].medieEvaluareNationala;
                }
                if ( j == 1 ) nrEleviRep1Opt++;
                if ( j == 2 ) nrEleviRep2Opt++;
                if ( j == 3 ) nrEleviRep3Opt++;
                break;
            }
        }
        cout << "Trecem la următorul elev.\n\n";
    }
    cout << "Am terminat repartizarea elevilor. Trecem la afișarea statisticilor\n\n\n";
    for ( int i = Bst; i <= Bdr; ++i )
    {
        if ( B[ i ].numarLocuriOcupate < B[ i ].numarLocuri && !B[ i ].eleviAdmisi.empty())
            B[ i ].ultimaMedie = B[ i ].eleviAdmisi.top().medieEvaluareNationala;
        cout << "\nSpecializarea : " << i << "; " << B[ i ].numeUnitateInvatamant << "\n" <<"Din " << B[ i ].numarLocuri << " locuri au fost ocupate " << B[ i ].numarLocuriOcupate << " locuri \n" << "Ultima medie de admitere a fost " << B[ i ].ultimaMedie << ". Următorii elevi au fost admiși : \n";
        stack<elev>temp;
        temp = B[ i ].eleviAdmisi;
        while ( !temp.empty() )
        {
            cout << "> "<< temp.top().nume << " " << temp.top().prenume << "(" << temp.top().codificare << "), media " << temp.top().medieEvaluareNationala << ", la a " << temp.top().nrOptiuneRepartizata << "-a optiune.\n";
            temp.pop();
        }
    }
    cout << "\n\n";
    int nrRepartizati = 0;
    for ( int i = 1; i <= numA; ++i )
    {
        cout << A[ i ].codificare << " (" << A[ i ].nume << " " << A[ i ].prenume << ") ";
        if ( A[ i ].optiuneAdmisa == -1 )
            cout << "nu a putut fi admis în urma etapei de repartizare computerizată.\n";
        else
        {
            cout << "a fost admis la specializarea " << A[ i ].optiuneAdmisa << " (" << B[ A[ i ].optiuneAdmisa ].numeSpecializare << ") de la " << B[ A[ i ].optiuneAdmisa ].numeUnitateInvatamant << ".\n";
            nrRepartizati++;
        }
    }
    float a = nrRepartizati, b = numA, cm = medieMatematica, clm = medieLbMat, clr = medieLbRom, mt = materni;
    cout << "\n\nAu fost repartizați la prima specializare, " << nrEleviRep1Opt << ", " << nrEleviRep2Opt << " la a doua specializare și " << nrEleviRep3Opt << " la cea de-a treia specializare, din totalul de "<< numA <<" elevi.\n";
    cout << setprecision( 4  ) <<"\nÎn total, au fost repartizați " << nrRepartizati << " și au rămas nerepartizați " << numA - nrRepartizati << ". Procentul elevilor repartizați este de " << a / b * 100 << "%.\nMedia națională la Matematică este " << cm / b << " cea la limba și literatura română este " << clr / b << ", iar cea la limba și literatura maternă este " << clm / mt << ".\n";
    return 0;
}
