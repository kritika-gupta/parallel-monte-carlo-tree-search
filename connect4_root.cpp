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

    void setLastPos(int r, int c)
    {
        this->lastcol = c;
        this->lastrow = r;
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

        int lc = this->getLastCol();
        int lr = this->getLastRow();

        if(lc==-1 || lr ==-1)
        {
            // game not started yet, return in_progress
            return 2;
        }
        
        
        

        // only need to check around the last position dropped
        int player = this->board[lr][lc];

        // horizontal connect 4
        int right = 0, left = 0;
        for(int c = lc-1; c>=0 && this->board[lr][c]==player; --c){
            left++;
        }
        for(int c = lc+1; c<7 && this->board[lr][c]==player; ++c){
            right++;
        }
        if(left+right+1 >= 4){
            //cout<<"horizontal win by "<<player<<endl;
            return player;
        }

        // vertical connect 4
        int up = 0, down = 0;
        for(int r = lr-1; r>=0 && this->board[r][lc]==player; --r){
            up++;
        }
        for(int r = lr+1; r<6 && this->board[r][lc]==player; ++r){
            down++;
        }
        if(up+down+1 >= 4){
            //cout<<"vertical win by "<<player<<endl;

            return player;
        }

        // right diagonal connect 4
        up = 0, down = 0;
        for(int r = lr-1, c = lc-1 ; r>=0 && c>=0 && this->board[r][c]==player; --r, --c){
            up++;
        }
        for(int r = lr+1, c = lc+1; r<6 &&c<7 && this->board[r][c]==player; ++r, ++c){
            down++;
        }
        if(up+down+1 >= 4){
            //cout<<"right diagonal win by "<<player<<endl;

            return player;
        }
        
        // left diagonal connect 4
        up = 0, down = 0;
        for(int r = lr+1, c = lc-1 ; r<6 && c>=0 && this->board[r][c]==player; ++r, --c){
            up++;
        }
        for(int r = lr-1, c = lc+1; r>=0 &&c<7 && this->board[r][c]==player; --r, ++c){
            down++;
        }
        if(up+down+1 >= 4){
            //cout<<"left diagonal win by "<<player<<endl;

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

class State
{
    Board board;
    int player;
    int visitCount;
    double winScore;

    public:

    // constructor
    State(){
        Board board;
        this->board.setBoardValues(board.getBoardValues());
        this->board.setMoves(board.getMoves());
        this->board.setLastPos(board.getLastRow(), board.getLastCol());

        this->player = 0;
        this->visitCount = 0;
        this->winScore = 0;
    }
    // constructor for a given board;
    State(Board *board){
        this->board.setBoardValues(board->getBoardValues());
        this->board.setMoves(board->getMoves());
        this->board.setLastPos(board->getLastRow(), board->getLastCol());
    }
    // copy constructor
    State(State* s)
    {
        this->board = new Board(s->getBoard());
        this->setPlayer(s->getPlayer());
        this->setVisitCount(s->getVisitCount());
        this->setWinScore(s->getWinScore());
    }
    //setter functions
    void setBoard(Board *board)
    {
        this->board.setBoardValues(board->getBoardValues());

        this->board.setMoves(board->getMoves());
        this->board.setLastPos(board->getLastRow(), board->getLastCol());
    }

    void setPlayer(int n)
    {
        this->player = n;
    }

    void setWinScore(double s)
    {
        this->winScore = s;
    }
    void setVisitCount(int n)
    {
        this->visitCount = n;
    }
    void incrementVisit()
    {
        this->visitCount++;
    }
    void addScore(double w)
    {
        if(this->winScore!=(double)INT_MIN){
            this->winScore += w;

        }
    }
    // getter functions
    Board* getBoard()
    {
        return &(this->board);
    }

    int getVisitCount()
    {
        return this->visitCount;
    }
    double getWinScore()
    {
        return this->winScore;
    }
    int getPlayer(){
        return this->player;
    }

    vector<State> getPossibleStates()
    {
        vector<State> possibleStates;

        // playing at each empty position generates a new state
        vector<pair<int, int> > emptyPositions = this->board.getEmptyPos();

        for(int i=0; i<emptyPositions.size(); i++){
            State newState;
            newState.setBoard(&(this->board));
            newState.setPlayer(1-this->player);
            newState.getBoard()->newMove(newState.getPlayer(), emptyPositions[i].first, emptyPositions[i].second);

            possibleStates.push_back(newState);
        }
        return possibleStates;
    }

    void togglePlayer()
    {
        this->player = 1-this->player;
    }
    
    void randomPlay()
    {
        
        vector<pair<int, int> > emptyPos  = this->board.getEmptyPos();
        int totalPos = emptyPos.size();
        if(totalPos==0) return;

        
        clock_gettime(CLK, &curr_time);
        
        //cout<<"seed is "<<curr_time.tv_nsec<<endl;
        mt19937_64 random_engine((curr_time.tv_nsec)*omp_get_thread_num());

        uniform_int_distribution<int> dis(0, totalPos-1);
        int randnum = dis(random_engine);
        this->board.newMove(this->player, emptyPos[randnum].first, emptyPos[randnum].second);
        //cout<<"returning from random play"<<endl;

    }
    
};

class Node
{
    State state;
    Node* parent;
    vector<Node>children;
    double ucb;

    public:
    //constructor
    Node()
    {
        State s;
        this->state.setBoard(s.getBoard());
        this->state.setPlayer(s.getPlayer());
        this->state.setWinScore(s.getWinScore());
        this->state.setVisitCount(s.getVisitCount());

        this->parent=nullptr;
        vector<Node> children(0);
        this->children = children;
        this->ucb = 0;    // **

    }

    Node(State* s)
    {
        this->state.setBoard(s->getBoard());
        this->state.setPlayer(s->getPlayer());
        this->state.setWinScore(s->getWinScore());
        this->state.setVisitCount(s->getVisitCount());
        this->parent=nullptr;

        vector<Node> children(0);

        this->children = children;
        this->ucb = 0;    // **
    }

    Node(State* s, Node* p, vector<Node> *c, double u)
    {
        this->state.setBoard(s->getBoard());
        this->state.setPlayer(s->getPlayer());
        this->state.setWinScore(s->getWinScore());
        this->state.setVisitCount(s->getVisitCount());
        *this->parent = Node(p->getState(), p->getParent(), p->getChildren(), p->getUCB());
        this->children = *c;
        this->ucb = u;    // **
    }

    Node(Node * n)
    {
        this->state = new State(n->getState());
        this->setParent(n->getParent());
        if(n->getChildren()->size()==0){
            this->children.resize(0);

        }
        for(int i=0; i<n->getChildren()->size(); i++)
        {
            // copy esch node child
            Node newNode((*n->getChildren())[i]);
            this->children.push_back(newNode);
        }
        this->setUCB(n->getUCB());

    }

    // setters
    void setParent(Node* p)
    {
        this->parent = p;
    }
    void setState(State* s)
    {
        this->state.setBoard(s->getBoard());
        this->state.setPlayer(s->getPlayer());
        this->state.setWinScore(s->getWinScore());
        this->state.setVisitCount(s->getVisitCount());
    }
    void setUCB(double u)
    {
        this->ucb = u;
    }

    //getters
    double getUCB()
    {
        return this->ucb;
    }
    Node* getParent()
    {
        return this->parent;
    }
    State* getState()
    {
        return &(this->state);
    }

    vector<Node>* getChildren()
    {
        return &(this->children);
    }

    // generate a random child for simulation phase
    Node* getRandomChild(int i)
    {
        int numChild = this->children.size();
        
        clock_gettime(CLK, &curr_time);
        
        //cout<<"seed is "<<curr_time.tv_nsec<<endl;
        std::mt19937_64 random_engine(curr_time.tv_nsec*omp_get_thread_num());
        std::uniform_int_distribution<int> dis(0, numChild-1);
        return &(this->children[dis(random_engine)]);
        
    }

    // function to calculate the ucb score
    double calcUCB(double winScore, int nodeVisit, int totalVisit)
    {
        if(nodeVisit == 0){
            return (double)INT_MAX;
        }
        return (winScore/(double)nodeVisit) + 1.41*sqrt(log((double)totalVisit)/(double)nodeVisit);

    }

    Node* findBestChild()
    {
        double max = INT_MIN;
        Node* bestChild = nullptr;
        // calculate ucb scores for each child
        
        for(int i=0; i<this->getChildren()->size(); i++){
            Node* child = &(*this->getChildren())[i];
            double u = calcUCB(child->getState()->getWinScore(), child->getState()->getVisitCount(), this->getState()->getVisitCount());
            child->setUCB(u);
            if(verbose) {
                cout<<"ucb = "<<u<<endl;
                child->getState()->getBoard()->display();
                cout<<endl;
            }

            

            if(u>max){
                max=u;
                bestChild = child;
            }
        }
        
        // return the child with max ucb score
        if(verbose) {
            cout<<"Returning from findBestChild with : "<<endl;
            bestChild->getState()->getBoard()->display();
        }
        return bestChild ;

    }

    Node* getChildWithMaxScore()
    {
        int max = INT_MIN;
        Node* best = nullptr;
        // return child with highest visit score
        for(int i=0; i<this->children.size(); i++){
            Node* child = &(this->children[i]);
            if(verbose) {
                cout<<"In getChildWithMaxScore, child "<<i<<" is"<<endl;
                child->getState()->getBoard()->display();
                cout<<" with vistis = "<<child->getState()->getVisitCount()<<endl;
            }
            if((child->getState()->getVisitCount())>max){
                best = child;
                max = child->getState()->getVisitCount();
            }
            if(verbose) {
                cout<<"max is"<<max<<endl;
            }
        }
        return best;

    }

    void printNode()
    {
        this->getState()->getBoard()->display();
    }
};

class Tree
{
    Node* root;
    public:
    //constructor
    Tree()
    {        
        this->root = new Node();
    }

    //copy constructor
    Tree(Tree *tree)
    {
        this->root = new Node(tree->getRoot());
        
    }

    Node* getRoot()
    {
        return (this->root);
    }
    void setRoot(Node* p)
    {
        this->root = p;
    }

    void dispTree()
    {
        cout<<"root node : "<<endl;
        this->root->printNode();
        for(int i=0; i<this->root->getChildren()->size(); i++)
        {
            cout<<"child node :"<<endl;
            (*this->root->getChildren())[i].printNode();
            cout<<"score = "<<(*this->root->getChildren())[i].getState()->getWinScore()<<endl<<endl;
            cout<<"visits = "<<(*this->root->getChildren())[i].getState()->getVisitCount()<<endl<<endl;
        }
    }

};

// instances of every MCTS run that cycles through all 4 stages
class MCTS
{
    static int const WIN_SCORE = 10;
    Tree* tree;
    int level;
    int opponent;
    int n;
    int p;


    public:
    MCTS(int num, int threads)
    {
        this->tree = new Tree();
        this->level = 0;
        this->n = num;
        this->p = threads;
    }


    
    Node findNextMove(Board* board, int player){
        this->opponent = 1 - player;
        //cout<<"opponent for this mcts object is "<<opponent<<endl;
        // create a new tree to find the next move
        Node* rootNode = this->tree->getRoot(); 

        //set the given board (current game state) as the root's state's board
        rootNode->getState()->setBoard(board);
        rootNode->getState()->setPlayer(opponent);

        Node* bestNode, *nodeExp;
        int tempstatus, playoutRes, size;
        //run the 4 steps for a set number of iterations

        vector<vector<int>> allChildren(this->p);
        omp_set_num_threads(this->p);
        #pragma omp parallel private(bestNode, nodeExp, tempstatus, playoutRes) shared(rootNode)
        {
            Tree threadTree(this->tree);
            Node* myRoot = threadTree.getRoot();
            // cout<<"myRoot points to  "<<myRoot<<endl;
            // cout<<"Children = "<<myRoot->getChildren()->size()<<endl;
            for(int i=0; i <this->n; i++)
            {
                //Node* bestNode = rootNode;
                // SELECTION
                // select a path to leaf node with best UCB
                //if(rootNode->getChildren()->empty()==false){
                    

                    bestNode = selectNode(myRoot);
                    
                    
                //}    
                
                //cout<<"Selection done"<<endl;

                //bestNode->getState()->getBoard()->display();
                // check if selected node is not end game scenario
                // EXPAND
                
                    
                // status of best node
                tempstatus = bestNode->getState()->getBoard()->checkStatus();

                //cout<<"After selection, we selected a node with status = "<<temp<<endl<<endl;
                if(tempstatus==2){
                    expandNode(bestNode);
                }
               

                //cout<<"Expansion done"<<endl;

                // SIMULATE
                nodeExp = bestNode;
                // get a random child 
                if(bestNode->getChildren()->size() > 0){
                    nodeExp = bestNode->getRandomChild(i);
                }
                
                //cout<<"Got a random child done"<<endl;

                //cout<<"The random Child is "<<endl;
                //nodeExp->getState()->getBoard()->display();
                //cout<<endl<<endl;
                // simulate a random playout of the random child
                playoutRes = simulateRandomPlayout(nodeExp);

                // UPDATE VIA BACKPROP
                backProp(nodeExp, playoutRes);
                //cout<<"BAckprop done"<<endl;
                int myID = omp_get_thread_num();
                size = myRoot->getChildren()->size();
                allChildren[myID].resize(size);
                for(int i=0; i<size; i++){
                    allChildren[myID][i] = (*myRoot->getChildren())[i].getState()->getVisitCount();

                }
                

            }
        }

        // merge the scores of the clones
        expandNode(rootNode);
        size = rootNode->getChildren()->size();
        vector<int> finalScores(size, 0);
        
        int max = INT_MIN, max_id = 0;
        #pragma omp parallel for
        for(int j=0; j<size; j++){
            for(int i=0; i<this->p; i++)
            {
                finalScores[j] += allChildren[i][j];
            }
            if(finalScores[j]>max)
            {
                max = finalScores[j];
                max_id = j;
            }
                
            //(*rootNode->getChildren())[j].getState()->setVisitCount(finalScores[j]);
        }
        //Node* temp = rootNode->getChildWithMaxScore();
        Node* temp = &(*rootNode->getChildren())[max_id];
        Node winner(temp);
        //cout<<"LEVEL = "<<this->level<<endl<<endl;
        //this->tree->dispTree();
        this->tree->setRoot(temp);
        this->level++;

        return winner;

    }
    // Selection - Successively select the child node of root with highest UCB value (L). If L is not end game, then call Expansion for L.
    Node* selectNode(Node* rootNode)
    {
        Node* node = rootNode;
        int num = rootNode->getChildren()->size();

        while(num!=0){
            // traverse the tree to get leaf with highest ucb value

            node = node->findBestChild();
            num = node->getChildren()->size();

        }
        //cout<<"returning from selection by selecting : "<<endl;

        //node->getState()->getBoard()->display();
        //cout<<endl<<endl;
        return node;
    }

    void expandNode(Node* node)
    {

        vector<State> possibleStates = node->getState()->getPossibleStates();

        // for each possible state, create a new child node
        //cout<<"Number of possible states are"<<possibleStates.size()<<endl;
        for(int i =0; i<possibleStates.size(); i++){
            Node newNode(&possibleStates[i]);

            newNode.setParent(node);
            
            newNode.getState()->setPlayer(1-node->getState()->getPlayer());

            node->getChildren()->push_back(newNode);
                                    
            
        }

    }

    void backProp(Node* nodeExp, int player)
    {
        // cout<<"In back prop with player"<<player<<endl;
        // cout<<"Starting from node :"<<endl;
        // nodeExp->getState()->getBoard()->display();
        Node *tempNode = nodeExp;
        while(tempNode!=nullptr)
        {
            // go back up
            //tempNode->getState()->getBoard()->display();
            tempNode->getState()->incrementVisit();
            if(tempNode->getState()->getPlayer()==player)
            {
                tempNode->getState()->addScore(WIN_SCORE);
            }
            tempNode = tempNode->getParent();
        }
    }

    int simulateRandomPlayout(Node* node)
    {
        Node tempNode(node);  // copy of node to be played out
        State tempState(tempNode.getState());
        int boardStatus = tempState.getBoard()->checkStatus();
        
        //cout<<"In simulate random playout, board status is "<<boardStatus<<endl;
        if(boardStatus == this->opponent)
        {
            //opponent has won, end playout
            //cout<<"opponent won"<<endl;
            tempNode.getParent()->getState()->setWinScore((double)INT_MIN);
            //cout<<"Returning from simulation"<<endl;
            return boardStatus;
        }

        // while board is in progress
        while(boardStatus == 2)
        {
            //cout<<"Board in prog"<<endl;
            tempState.togglePlayer();
            //cout<<"going to random play"<<endl;
            tempState.randomPlay();
            
            boardStatus = tempState.getBoard()->checkStatus();
        }

        return boardStatus;
    }
};


int main(int argc, char* argv[]){
    
    int n = atoi(argv[1]);
    int p = atoi(argv[2]);

    // create a board
    Board myboard;
    //myboard.display();
    MCTS mcts(n, p);
    int player = 0;
    int totalmoves = 42;
    double start = omp_get_wtime();
    for(int i=0; i<totalmoves; i++)
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
    double root_time = omp_get_wtime() - start;
    int winStat = myboard.checkStatus();
    cout<<winStat<<" "<<root_time<<endl;
    return 0;

}

