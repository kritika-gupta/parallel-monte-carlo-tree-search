#include <bits/stdc++.h> 
#include <algorithm>
using namespace std;
int seed = 5;
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
    }
    // constructor for a given board;
    State(Board board){
        this->board = board;
    }

    //setter functions
    void setBoard(Board board)
    {
        this->board = board;
    }

    void setPlayer(int n)
    {
        this->player = n;
    }

    // getter functions
    Board getBoard()
    {
        return this->board;
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
        for(int i=0; i<emptyPositions.size; i++){
            State newState(this->board);
            newState.setPlayer(1-this->player);
            newState.getBoard().newMove(newState.getPlayer(), emptyPositions[i].first, emptyPositions[i].second);
            possibleStates.push_back(newState);

        }
        return possibleStates;
    }
    
    
};

class Node
{
    State state;
    Node parent();
    vector<Node>children;
    double ucb;

    public:
    //constructor
    Node()
    {
        State s;
        this->state = s;
        vector<Node> children;
        this->children = children;
        this->ucb = 0;    // **

    }

    Node(State s)
    {
        this->state = s;
        vector<Node> children;
        this->children = children;
        this->ucb = 0;    // **
    }

    Node(State s, Node p, vector<Node> c)
    {
        this->state = s;
        this->parent = p;
        this->children = c;
        this->ucb = 0;    // **
    }

    // setters
    void setParent(Node p)
    {
        this->parent = p;
    }
    void setState(State s)
    {
        this->state = s;
    }

    //getters
    Node getParent()
    {
        return this->parent;
    }
    State getState()
    {
        return this->state;
    }

    vector<Node> getChildren()
    {
        return this->children;
    }

    // generate a random child for simulation phase
    Node getRandomChild()
    {
        int numChild = this->children.size;
        std::mt19937_64 random_engine(seed);
    }

    // function to calculate the ucb score
    double calcUCB(double winScore, int nodeVisit, int totalVisit)
    {
        if(nodeVisit == 0){
            return INT_MAX;
        }
        return (winScore/nodeVisit) + 1.41*sqrt(log(totalVisit)/nodeVisit);

    }

    Node findBestChild()
    {
        // calculate ucb scores for each child
        for(auto child: this->children){
            this->ucb = calcUCB(child.getState().getWinScore(), child.getState().getVisitCount(), this->getState().getVisitCount());
        }
        // return the child with max ucb score
        return *std::max_element(children.begin(), children.end(), [](Node* a, Node* b){return a->ucb, b->ucb;});

    }
};

class Tree
{
    Node root;

    public:
    Node getRoot()
    {
        return this->root;
    }

};

// instances of every MCTS run that cycles through all 4 stages
class MCTS
{
    public:
    Board findNextMove(Board board, int player){
        int opponent = 1 - player;

        // create a new tree to find the next move
        Tree tree;
        Node rootNode = tree.getRoot();

        //set the given board (current game state) as the root's state's board
        rootNode.getState().setBoard(board);
        rootNode.getState().setPlayer(player);

        //run the 4 steps for a set number of iterations
        int nIter = 100;
        for(int i=0; i < nIter; i++)
        {
            // SELECTION    
            Node bestNode = selectNode(rootNode);

            // check if selected node is not end game scenario
            // EXPAND
            if(bestNode.getState().getBoard().checkStatus()==2){
                expandNode(bestNode);
            }

            // SIMULATE
            Node nodeExp = bestNode;
            // get a random child 
            if(bestNode.getChildren().size > 0){
                nodeExp = bestNode.
            }

        }

    }

};


// Selection - Successively select the child node of root with highest UCB value (L). If L is not end game, then call Expansion for L.
Node selectNode(Node rootNode)
{
    Node node = rootNode;
    while(node.getChildren().empty==false){
        // traverse the tree to get leaf with highest ucb value
        node = node.findBestChild();
    }
    return node;
}

void expandNode(Node node)
{
    vector<State> possibleStates = node.getState().getPossibleStates();
    // for each possible state, create a new child node
    for(int i =0; i<possibleStates.size; i++){
        Node newNode(possibleStates[i]);
        newNode.setParent(node);
        newNode.getState().setPlayer(1-node.getState().getPlayer());
        node.getChildren().push_back(newNode);
    }
}

void backProp(Node nodeExp, int player)
{
    
}

int simulateRandomPlayout(Node node)
{

}

class Board
{
    public:
    int moves;
    vector<vector<int>> board;

    // constructor 
    Board(){
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                board[i][j] = -1;
            }
        }
        moves = 0;
    }

    vector<pair<int, int> > getEmptyPos()
    {
        vector<pair<int, int> > positions;
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(this->board[i][j]==-1) positions.push_back(make_pair(i, j));

            }
        }
    }

    // function to check the status (a player one (return player), or in progress (return 2), or draw (return 3))
    int checkStatus()
    {
        vector<vector<int>> colwise;
        vector<int> diagonal1;
        vector<int> diagonal2;
        int p;
        // check row-wise
        for(int i=0; i<3; i++){
            colwise[0].push_back(this->board[i][0]);
            colwise[1].push_back(this->board[i][1]);
            colwise[2].push_back(this->board[i][2]);
            diagonal1.push_back(this->board[i][i]);
            diagonal2.push_back(this->board[i][2-i]);
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
        if(this->getEmptyPos().size!=0) return 2;

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
        board[x][y] = player;   // set '1' or 'O' as the value on board 
    }

    // getters and setters
    vector<vector<int> > getBoardValues()
    {
        return this->board;
    }

    void setBoardValues(vector<vector<int> > board)
    {
        this->board = board;
    }


    
};


int main(){
    // create a board
    Board myboard;
    myboard.display();

    return 0;

}

