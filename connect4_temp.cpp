#include <bits/stdc++.h> 
#include <algorithm>
#include <omp.h>
#include <time.h>

using namespace std;
#define CLK CLOCK_MONOTONIC
struct timespec curr_time;
double totalTime = 0;

bool verbose = false;

class Board
{
    public:
    int moves;
    vector<vector<int>> board;
    int lastcol, lastrow;

    // constructor 
    Board(){
        for(int i=0; i<6; i++){
            this->board.resize(6, vector<int>(7));
            for(int j=0; j<7; j++){
                this->board[i][j] = -1;
            }
        }
        this->moves = 0;
        this->lastcol = -1;
        this->lastrow = -1;
    }
    // copy constructor
    Board(Board* b)
    {
        this->board = b->getBoardValues();
        this->moves = b->getMoves();
        this->lastcol = b->getLastCol();
        this->lastrow = b->getLastRow();
    }

    int getLastRow()
    {
        return this->lastrow;
    }

    int getLastCol()
    {
        return this->lastcol;
    }

    // fn to return a list of empty positions on the board
    vector<pair<int, int> > getEmptyPos()
    {
        vector<pair<int, int> > positions;

        //for every col, get the deepest empty row
        for(int col=0; col<7; col++)
        {
            for(int row = 5; row>=0; row--)
            // start row count from bottommost row 
            {
                if(this->board[row][col]==-1){
                    positions.push_back(make_pair(row, col));
                    break;
                }
            }
        }
        
        return positions;
    }

    // function to check the status (a player one (return player), or in progress (return 2), or draw (return 3))
    int checkStatus()
    {

        // draw
        if(this->getEmptyPos().size()==0){
            return 3;
        }

        int lc = this->lastcol;
        int lr = this->lastrow;
        cout<<"last col, last row is "<<lc<<", "<<lr<<endl;


        if(lc==-1 || lr ==-1)
        {
            // game not started yet, return in_progress
            return 2;
        }
        
        
        

        // only need to check around the last position dropped
        int player = this->board[lr][lc];

        // horizontal connect 4
        int right = 0, left = 0;
        for(int c = lc-1; c>=0 && this->board[lr][lc]==player; --c){
            left++;
        }
        for(int c = lc+1; c<7 && this->board[lr][lc]==player; ++c){
            right++;
        }
        if(left+right+1 >= 4){
            return player;
        }

        // vertical connect 4
        int up = 0, down = 0;
        for(int r = lr-1; r>=0 && this->board[lr][lc]==player; --r){
            up++;
        }
        for(int r = lr+1; r<6 && this->board[lr][lc]==player; ++r){
            down++;
        }
        if(left+right+1 >= 4){
            return player;
        }

        // right diagonal connect 4
        up = 0, down = 0;
        for(int r = lr-1, c = lc-1 ; r>=0 && c>=0 && this->board[lr][lc]==player; --r, --c){
            up++;
        }
        for(int r = lr+1, c = lc+1; r<6 &&c<7 && this->board[lr][lc]==player; ++r, ++c){
            down++;
        }
        if(left+right+1 >= 4){
            return player;
        }
        
        // left diagonal connect 4
        up = 0, down = 0;
        for(int r = lr+1, c = lc-1 ; r<6 && c>=0 && this->board[lr][lc]==player; ++r, --c){
            up++;
        }
        for(int r = lr-1, c = lc+1; r>=0 &&c<7 && this->board[lr][lc]==player; --r, ++c){
            down++;
        }
        if(left+right+1 >= 4){
            return player;
        }

        // in progress
        if(this->getEmptyPos().size()!=0){
            return 2;
        }



    }

    // function to check for a win on the given row
    // int checkWin(vector<int> row)
    // {
        
    //     int value = row[0];
    //     for(int i=0; i<3-1; i++){
    //         if(row[i]!=row[i+1]){
    //             return -1;
    //         }
    //     }
    //     return row[0];
    // }

    // function to display the board
    void display()
    {
        for(int i=0; i<6; i++){
            for(int j=0; j<7; j++){
                cout<<this->board[i][j]<< " ";
            }
            cout<<endl;
        }
    }

    // function to perform new move
    void newMove(int player, int x, int y)
    {
        this->moves++;  // increment number of moves
        this->board[x][y] = player;   // set '1' or 'O' as the value on board 
        if(verbose) {
            this->display();
        }
        //cout<<"NEw move added by player"<<player<<" at "<<x<<y<<endl;
        this->lastcol = y;
        this->lastrow = x;

    }

    // getters and setters
    vector<vector<int> > getBoardValues()
    {
        return this->board;
    }

    int getMoves()
    {
        return this->moves;
    }

    void setBoardValues(vector<vector<int> > board)
    {
        this->board = board;
    }
    void setMoves(int n)
    {
        this->moves = n;
    }

    
};

int main(){
    Board b;
    b.newMove(0, 5, 0);
    b.newMove(0, 5, 1);
    b.newMove(0, 5, 2);
    b.newMove(0, 5, 3);
    b.display();
    int stat = b.checkStatus();
    cout<<"status is : "<<stat<<endl;
    return 0;


}