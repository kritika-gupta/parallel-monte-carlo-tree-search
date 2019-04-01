#include <bits/stdc++.h> 
#include <algorithm>
#include <omp.h>
#include <time.h>
#include"tictactoe_serial_header.h"
using namespace std;
#define CLK CLOCK_MONOTONIC



int main(int argc, char* argv[]){
    
    int n = atoi(argv[1]);

    // create a board
    Board myboard;
    //myboard.display();
    MCTS mcts(n);
    int player = 0;
    int totalmoves = 9;
    double start = omp_get_wtime();
    for(int i=0; i<9; i++)
    {
        int x, y;
        // for each move
        // if(player == 0){
        //     cin>>x>>y;
        //     myboard.newMove(player, x, y);
        // }
        // else{
            //cout<<"Player is "<<player<<endl;
            myboard  = *((mcts.findNextMove(&myboard, player)).getState()->getBoard());
       // }
        //cout<<"my board stat is "<<myboard.checkStatus()<<endl<<endl;
        
        //cout<<"current state after chance "<<i<<endl;
        //myboard.display();
        
        if(myboard.checkStatus()!=2){
            // if not in progress
            //cout<<"game stops"<<endl;
            break;
        }
        //cout<<"game continues"<<endl;
        player = 1-player;

        

    }
    double serial_time = omp_get_wtime() - start;
    int winStat = myboard.checkStatus();
    cout<<winStat<<" "<<serial_time<<endl;
    return 0;

}

