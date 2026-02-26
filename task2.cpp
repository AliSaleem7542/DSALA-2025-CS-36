#include<iostream>
using namespace std;

int main()
{
    int array[4][5] = {0};
    int x = 1;

    // Top row
    for(int j = 0; j < 5; j++)
        array[0][j] = x++;

    // Right column
    for(int i = 1; i < 4; i++)
        array[i][4] = x++;

    // Bottom row
    for(int j = 3; j >= 0; j--)
        array[3][j] = x++;

    // Left column
    for(int i = 2; i > 0; i--)
        array[i][0] = x++;

    // Inner top row
    for(int j = 1; j < 4; j++)
        array[1][j] = x++;

    // Inner bottom row
    for(int j = 3; j > 0; j--)
        array[2][j] = x++;

    // full array 
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(array[i][j] == 0)
                cout << "    ";
            else
                cout << array[i][j] << "   ";
        }
        cout << endl;
    }
}