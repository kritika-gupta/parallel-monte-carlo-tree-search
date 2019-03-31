#include <bits/stdc++.h> 
#include <algorithm>
#include <omp.h>
using namespace std;
int main(){

    vector<vector<int>> allChildren(4);
    for(int i=0; i<4; i++)
    {
        allChildren[i] = {0, 4*i, 8*i};
    }

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<allChildren[i].size(); j++){
            cout<<allChildren[i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}