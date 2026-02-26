#include <iostream>
using namespace std;

int main()
{
    int n = 5;

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= 6; j++)
        {

            if (j == 1 || j == 6)
                cout << "*";

            else if (i == 2 && (j == 2 || j == 5))
                cout << "*";

            else if (i == 3)   // slight change here (full middle row)
                cout << "*";

            else if (i == 4 && (j == 2 || j == 5))
                cout << "*";

            else
                cout << " ";
        }
        cout << endl;
    }
}