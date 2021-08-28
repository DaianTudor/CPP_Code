#include <iostream>
using namespace std;
int main()
{
    while (0==0)
    {
    float v[10],rez=0;
    int i,x,op;
    cout << "COMPUTER_v1.0" << endl;
    cout << "Made by Daian Tudor" << endl;
    system("pause");
    cout << "Dati nr de termeni: ";
    cin >> x;
    for (i=1; i<=x; i++)
    {
        cout << "Dati termenul " << i << " : ";
        cin >> v[i];
        system("cls");
    }
    cout << "Menu" << endl << endl << "Alegeti o optiune :" << endl;
    cout << "1 - Aduna termenii" << endl << "2 - Scade Termenii" << endl;
    cout << "3 - Inmulteste termenii" << endl << "4 - Imparte termenii" << endl;
    cin >> op;
    switch (op)
    {
    case 1 :
        {
            for (i=1; i<=x; i++)
                rez=rez+v[i];
            break;
        }
    case 2 :
        {
            rez=v[1];
            for (i=2; i<=x; i++)
                rez=rez-v[i];
            break;
        }
    case 3 :
        {
            rez=1;
            for (i=1; i<=x; i++)
                rez=rez*v[i];
            break;
        }
    case 4 :
        {
            rez=v[1];
            for (i=2; i<=x; i++)
                rez=rez/v[i];
            break;
        }
    }
    cout << rez;
    system("pause");
    system("cls");
    }
}
