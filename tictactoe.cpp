#include <bits/stdc++.h> 
#include <algorithm>
using namespace std;
int seed = time(NULL);

class Board
{
    public:
    int moves;
    vector<vector<int>> board;

    // constructor 
    Board(){
        for(int i=0; i<3; i++){
            this->board.resize(3, vector<int>(3));
            for(int j=0; j<3; j++){
                this->board[i][j] = -1;
            }
        }
        this->moves = 0;
    }
    // copy constructor
    Board(Board* b)
    {
        this->board = b->getBoardValues();
        this->moves = b->getMoves();
    }

    vector<pair<int, int> > getEmptyPos()
    {
        vector<pair<int, int> > positions;
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(this->board[i][j]==-1){
                    positions.push_back(make_pair(i, j));
                }

            }
        }
        return positions;
    }

    // function to check the status (a player one (return player), or in progress (return 2), or draw (return 3))
    int checkStatus()
    {
        vector<vector<int>> colwise(3, vector<int>(3));
        vector<int> diagonal1(3);
        vector<int> diagonal2(3);
        int p;
        // check row-wise
        for(int i=0; i<3; i++){
            colwise[0][i] = (this->board[i][0]);
            colwise[1][i] = (this->board[i][1]);
            colwise[2][i] = (this->board[i][2]);
            diagonal1[i] = (this->board[i][i]);
            diagonal2[i] = (this->board[i][2-i]);
            p = checkWin(this->board[i]);
            if(p!=-1){
                // player p has won
                return p;
            }

        }

        // check col-wise
        for(int i=0; i<3; i++){
            p = checkWin(colwise[i]);
            if(p!=-1){
                // player p has won
                return p;
            }
        }
        // check diagonal
        p = checkWin(diagonal1);
        if(p!=-1){
            // player p has won
            return p;
        }
        
        p = checkWin(diagonal2);
        if(p!=-1){
            // player p has won
            return p;
        }

        // in progress
        if(this->getEmptyPos().size()!=0){
            return 2;
        }

        // draw
        return 3;


    }

    // function to check for a win on the given row
    int checkWin(vector<int> row)
    {
        
        int value = row[0];
        for(int i=0; i<3-1; i++){
            if(row[i]!=row[i+1]){
                return -1;
            }
        }
        return row[0];
    }

    // function to display the board
    void display()
    {
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
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
        //cout<<"NEw move added by player"<<player<<" at "<<x<<y<<endl;
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
        this->board = board;
        this->player = 0;
        this->visitCount = 0;
        this->winScore = 0;
    }
    // constructor for a given board;
    State(Board *board){
        this->board.setBoardValues(board->getBoardValues());
        this->board.setMoves(board->getMoves());
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
        this->winScore = w;
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

            //cout<<"new possible state is "<<endl;
            //newState.getBoard()->display();
            cout<<endl<<endl;

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

        
        std::mt19937_64 random_engine(seed);
        std::uniform_int_distribution<int> dis(0, totalPos-1);
        int randnum = dis(random_engine);
        this->board.newMove(this->player, emptyPos[randnum].first, emptyPos[randnum].second);


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
        this->state = s;
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
    Node* getRandomChild()
    {
        int numChild = this->children.size();
        std::mt19937_64 random_engine(seed);
        std::uniform_int_distribution<int> dis(0, numChild-1);
        return &(this->children[dis(random_engine)]);
        
    }

    // function to calculate the ucb score
    double calcUCB(double winScore, int nodeVisit, int totalVisit)
    {
        if(nodeVisit == 0){
            return INT_MAX;
        }
        return (winScore/nodeVisit) + 1.41*sqrt(log(totalVisit)/nodeVisit);

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
            cout<<"ucb ="<<u<<endl;
            child->getState()->getBoard()->display();
            cout<<endl;

            

            if(u>max){
                max=u;
                bestChild = child;
            }
        }
        // return the child with max ucb score
        cout<<"Returning from findBestChild with : "<<endl;
        bestChild->getState()->getBoard()->display();
        cout<<endl<<endl;
        return bestChild ;

    }

    Node* getChildWithMaxScore()
    {
        int max = INT_MIN;
        Node* best = nullptr;
        // return child with highest visit score
        for(int i=0; i<this->children.size(); i++){
            Node* child = &(this->children[i]);
            cout<<"In getChildWithMaxScore, child "<<i<<" is"<<endl;
            child->getState()->getBoard()->display();
            cout<<" with vistis = "<<child->getState()->getVisitCount()<<endl;
            if((child->getState()->getVisitCount())>max){
                best = child;
                max = child->getState()->getVisitCount();
            }
            cout<<"max is"<<max<<endl;
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
    Node root;
    public:
    //constructor
    Tree()
    {
        Node r;
        this->root = r;
    }
    
    Node* getRoot()
    {
        return &(this->root);
    }
    void setRoot(Node* p)
    {
        this->root = *p;
    }

    void dispTree()
    {
        cout<<"root node : "<<endl;
        this->root.printNode();
        for(int i=0; i<this->root.getChildren()->size(); i++)
        {
            cout<<"child node :"<<endl;
            (*this->root.getChildren())[i].printNode();

        }
    }

};

// instances of every MCTS run that cycles through all 4 stages
class MCTS
{
    static int const WIN_SCORE = 100;

    public:
    Board findNextMove(Board* board, int player){
        int opponent = 1 - player;
        cout<<"opponent for this mcts object is "<<opponent<<endl;
        // create a new tree to find the next move
        Tree tree;
        Node* rootNode = tree.getRoot(); 

        //set the given board (current game state) as the root's state's board
        rootNode->getState()->setBoard(board);
        rootNode->getState()->setPlayer(opponent);
        cout<<"ROOT IS"<<endl;
        rootNode->getState()->getBoard()->display();
        cout<<endl<<endl;
        //run the 4 steps for a set number of iterations
        int nIter = 900;
        cout<<"starting iterations"<<endl;
        for(int i=0; i < nIter; i++)
        {
            cout<<"iteration number "<<i<<endl;
            Node* bestNode = rootNode;
            // SELECTION
            // select a path to leaf node with best UCB
            if(rootNode->getChildren()->empty()==false){
                cout<<"Root has"<<rootNode->getChildren()->size()<<" children for selection"<<endl;
                bestNode = selectNode(rootNode);
            }    
            
            cout<<"Selection done"<<endl;
            //bestNode->getState()->getBoard()->display();
            // check if selected node is not end game scenario
            // EXPAND
            

            // status of best node
            int temp = bestNode->getState()->getBoard()->checkStatus();
            cout<<"After selection, we selected a node with status = "<<temp<<endl<<endl;
            if(temp==2){
                expandNode(bestNode);
            }
            cout<<"Expansion done"<<endl;


            // SIMULATE
            Node* nodeExp = bestNode;
            // get a random child 
            if(bestNode->getChildren()->size() > 0){
                nodeExp = bestNode->getRandomChild();
            }
            cout<<"Got a random child done"<<endl;
            cout<<"The random Child is "<<endl;
            nodeExp->getState()->getBoard()->display();
            cout<<endl<<endl;

            // simulate a random playout of the random child
            int playoutRes = simulateRandomPlayout(nodeExp);
            cout<<"simulation random playout done"<<endl;


            // UPDATE VIA BACKPROP
            backProp(nodeExp, playoutRes);
            cout<<"BAckprop done"<<endl;

        }
        Node* temp = rootNode->getChildWithMaxScore();
        
        Node winner(temp);
        //tree.setRoot(temp);
        //tree.dispTree();
                cout<<"returned from getChildWithMAxScore and temp has: "<<endl;
                        temp->getState()->getBoard()->display();


        return *(winner.getState()->getBoard());




    }
    // Selection - Successively select the child node of root with highest UCB value (L). If L is not end game, then call Expansion for L.
    Node* selectNode(Node* rootNode)
    {
        Node* node = rootNode;
        int num = node->getChildren()->size();
        while(num!=0){
            // traverse the tree to get leaf with highest ucb value

            node = node->findBestChild();
            num = node->getChildren()->size();
            cout<<endl<<endl<<"returned selection child has "<< num <<" children"<<endl;

        }
        cout<<"returning from selection by selecting : "<<endl;
        node->getState()->getBoard()->display();
        cout<<endl<<endl;
        return node;
    }

    void expandNode(Node* node)
    {
        vector<State> possibleStates = node->getState()->getPossibleStates();
        // for each possible state, create a new child node
        cout<<"Number of possible states are"<<possibleStates.size()<<endl;
        for(int i =0; i<possibleStates.size(); i++){
            Node newNode(&possibleStates[i]);
            newNode.setParent(node);
            newNode.getState()->setPlayer(1-node->getState()->getPlayer());
            node->getChildren()->push_back(newNode);

            cout<<endl<<endl;
        }
        cout<<"after expanding, root has "<<node->getChildren()->size()<<" children"<<endl;
    }

    void backProp(Node* nodeExp, int player)
    {
        cout<<"In back prop with player"<<player<<endl;
        cout<<"Starting from node :"<<endl;
        nodeExp->getState()->getBoard()->display();
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
        int boardStatus = node->getState()->getBoard()->checkStatus();
        
        cout<<"In simulate random playout, board status is "<<boardStatus<<endl;
        if(boardStatus == 1 - node->getState()->getPlayer())
        {
            //opponent has won, end playout
            cout<<"opponent won"<<endl;
            Node *t = node->getParent();
            t->getState()->setWinScore((double)INT_MIN);
            cout<<"Returning from simulation"<<endl;
            return boardStatus;
        }

        // while board is in progress
        State* tempState = (node->getState());
        while(boardStatus == 2)
        {
            //cout<<"Board in prog"<<endl;
            tempState->togglePlayer();
            //cout<<"going to random play"<<endl;
            tempState->randomPlay();
            boardStatus = tempState->getBoard()->checkStatus();
        }

        return boardStatus;
    }
};


int main(){
    // create a board
    cout<<"Starting"<<endl;
    Board myboard;
    //myboard.display();
    MCTS mcts;
    int player = 0;
    int totalmoves = 9;
    for(int i=0; i<9; i++)
    {
        // for each move
        
            myboard  = (mcts.findNextMove(&myboard, player));
            cout<<"my board stat is "<<myboard.checkStatus()<<endl<<endl;
        
        cout<<"current state after chance "<<i<<endl;
        myboard.display();
        cout<<endl<<endl;
        if(myboard.checkStatus()!=2){
            // if not in progress
            cout<<"game stops"<<endl;
            break;
        }
        cout<<"game continues"<<endl;
        player = 1-player;

    }
    int winStat = myboard.checkStatus();
    cout<<"Winner stat is "<<winStat<<endl;


    return 0;

}

