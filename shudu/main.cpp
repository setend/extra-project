#include <iostream>
#include "shudu.hpp"
#include <vector>

using namespace std;

int main()
{
    vector<vector<int>> a(9,vector<int>(9,0));
    a[0][0]=3;a[0][1]=9;a[0][3]=2;
    a[1][2]=7;a[1][7]=2;
    a[2][2]=5;a[2][3]=7;a[2][4]=1;a[2][6]=9;
    a[3][0]=6;a[3][4]=9;
    a[4][1]=8;a[4][3]=5;a[4][4]=6;a[4][5]=2;a[4][7]=1;
    a[5][4]=3;a[5][8]=9;
    a[6][2]=8;a[6][4]=2;a[6][5]=5;a[6][6]=3;
    a[7][1]=1;a[7][6]=2;
    a[8][5]=6;a[8][7]=7;a[8][8]=5;
    sol(a);
    return 0;
}

