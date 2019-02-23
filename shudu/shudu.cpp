#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include "shudu.hpp"
using namespace std;

static bool helper(vector<vector<int>> &Map,int row, int col);

static bool check(vector<vector<int>> &Map, int row, int ,int);
void sol(vector<vector<int>> &shudu)
{
    helper(shudu,0,0);
    for(int i=0; i< 9; ++i){
        for(int j=0; j<9; ++j)
            cout << shudu[i][j] << ' ';
        cout << endl;
    }

}

bool helper(vector<vector<int>> &Map,int row,int col)
{
    if(row > 8) return true;
    int next_row = row + col/8;
    int next_col = (col + 1)%9;
    if(Map[row][col] > 0) return helper(Map,next_row,next_col);
    for(int i = 1 ; i <= 9; ++i)
    {
        Map[row][col] = i;
        if(check(Map,row,col,i) && 
           helper(Map,next_row,next_col))
            return true;
        Map[row][col] = 0;
    }
    return false;
}

bool check( vector<vector<int>>& m,int row,int col,int t)
{
    for(int i=0 ;i<9; ++i)
    {
        if(i!=row && m[i][col] == t) return false;
        if(i != col && m[row][i] == t) return false;
    }
    int startx = row/3*3;
    int starty = col/3*3;
    for(int i=0;i<3;++i)
        for(int j=0; j<3; ++j)
        {
            if((startx+i != row || starty+j != col) && m[startx+i][starty+j] == t)
                return false;
        }
    return true;
}


   

