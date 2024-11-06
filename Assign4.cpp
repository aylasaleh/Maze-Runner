#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <thread>

using namespace std; 

class Graph {
private:
    struct vertex { //struct if vertex storing:
        int y; //y coordinate (i)
        int x; //x coordinate (j)
        vertex (int y, int x): y(y), x(x) {}; 
    };

    int V; // Number of vertices
    vector<vector<int> > adjList; // Adjacency list
    vector <vertex> vertexCoordinates ; //stores coordintates of each vertex in relation to adjList

public:
    int uncrossable; 

    Graph ()
    {
        V= 0; 
    }

    int findVertex(int y, int x)
    {
        for (int i = 0; i < vertexCoordinates.size(); i++)
        {    
            if (vertexCoordinates[i].y == y && vertexCoordinates[i].x == x)
                return i;   //return index of coordinates
        }
        return -1; 
    }

    int y(int i)
    {
        return vertexCoordinates[i].y; 
    }

    int x(int i)
    {
        return vertexCoordinates[i].x; 
    }

    void addVertex(int y, int x){
        vertexCoordinates.push_back(vertex(y,x)); 
        adjList.resize(V+1);                  //add space for new vector
        V++; 
    }

    // Function to add an edge to the graph
    void addEdge(int y, int x, int i, int j) {    //add them to each others destination
        int a = findVertex(y,x); //find indicies of both verticies
        int b = findVertex(i,j);
        if (a!= -1 && b!= -1) //a and b are both found
        {   
            adjList[a].push_back(b); 
            adjList[b].push_back(a); 
        }
        else
            cout << "one coordinate not found" << endl; 
    }

    vector<int> dijkstra (int y, int x, int i, int j)
    {
        int s = findVertex(y,x);    //get index of start vertex
        int e = findVertex(i,j);    //get index of end vertex

        vector <int> d(V,999);               //vector to store distance from v fro all other verticies 
        vector<bool> visited(V, false);     // vector to mark visited vertices
        vector <int> prev (V); 
        queue <int> que;                  //will be using a que to determine order of traversing vertex as to maintain adjacency order

        d[s] = 0; //distance between vertex inputted and itseld is always 0
        prev[s] = s; 

        que.push(s); 

        while (!que.empty()){

            int v = que.front();    
            que.pop(); 

            visited[v] = true; //mark as visited

            //go to all adjacent nodes
            for (int i = 0; i < adjList[v].size(); i++){
                int adj = adjList[v][i]; 
                if (v != uncrossable || (i !=19 && j!=19)){ //either it's endpoint is x (its the player) and it has to be uncrossasble or its endpoint is the player (its the gaurd) and uncrossable does not matter
                    if (!visited[adj])                  //to avoid looping over already visited verticies
                        que.push(adj);                  //add adjacent verticies to que

                    if (d[adj]> d[v]+1)      //if new path distance is less than original distance 
                    {    d[adj] = d[v] +1;  //update it to shorter dis: distance between any vertex and it's adjacent vertex is 1 added
                        prev[adj] = v;
                    }
                }else
                {    d[v] = 999; //make dis infinity for future refrence
                    break; 
                }
            }
        }


        vector<int> path;
        int n = 0; 
        int currentVertex = e;
        path.push_back(currentVertex);
        while (currentVertex != s && n < V) {
            currentVertex = prev[currentVertex]; 
            path.push_back(currentVertex); 
            n++; 
        }

        if (n == V) //if it tried to push more verticies than the maximum that means the path is incorrect and the gaurd has blocked off all exits for the player
        {    vector <int> b;
            return b; //return an empty vector
        }

        return path; //otherwise return the path

    }
    

};

class maze{
private:

    int m [20][20] = {
    {2,1,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0},//0
    {0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1},//1
    {0,0,0,1,0,1,0,1,0,1,0,0,1,1,0,1,0,1,1,0},//2  
    {1,1,1,1,0,1,0,0,0,1,1,0,1,1,0,1,0,0,1,1},//3
    {0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,1,1,0,1,0},//4
    {0,1,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,0,0,0},//5
    {0,1,0,1,1,1,0,1,0,1,0,0,0,1,0,0,0,1,1,0},//6
    {0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,0,1,0},//7
    {0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,1,0,0},//8
    {1,0,1,1,0,1,1,0,0,1,0,1,0,1,1,0,0,1,1,0},//9
    {0,0,0,0,0,0,1,1,1,1,0,1,0,1,0,1,0,0,0,0},//10
    {0,1,1,1,1,1,1,0,0,1,0,1,0,1,0,1,0,1,0,1},//11
    {0,0,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0},//12
    {1,0,0,0,1,1,1,1,1,0,1,1,0,1,0,1,0,0,1,0},//13
    {1,1,0,1,1,0,0,0,0,0,0,0,0,1,0,1,1,0,1,0},//14
    {0,0,0,0,0,0,1,1,1,1,1,1,0,1,0,0,1,0,1,0},//15
    {1,1,0,1,1,1,1,0,0,0,1,1,0,1,1,0,1,0,1,0},//16
    {0,0,0,1,0,0,0,0,1,0,1,1,0,1,1,0,0,0,1,1},//17
    {0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0},//18
    {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,3}};//19  //maze 2D array
   //0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9

    int x; 
    int y; 

    int ox;
    int oy; 

    Graph graph;

    vector<int> oldPath ; 

public:
    int endgame; 

    void generateMazeGraph()
    {
        //initalize position variables of player and guard
        x = 0; 
        y = 0; 
        ox = 19; 
        oy = 0; 

        for(int i = 0; i < 20; ++i) {   
            for(int j = 0; j < 20; ++j) {   //go through each element in the array
                if (m[i][j]!=1) //if clear path then add vertex
                {    graph.addVertex(i,j); 
                
                    //check for edges upwards (vertex we have already added)
                    if (i > 0 && m[i-1][j] != 1)
                        graph.addEdge(i,j,i-1,j); 
                    
                    //check for edges to the left (vertex we have already added)
                    if (j > 0 && m[i][j-1] != 1)
                        graph.addEdge(i,j,i,j-1);

                    //we wont check for verticies on the direct right or down since we haven't created them yet, leave them untill we rach them in the for loop
                }
            }
        } 
    }

void guardMove()
    {
        vector<int> path = graph.dijkstra(oy,ox,y,x);    //find path from gaurd "O" to player "P" 
        path.pop_back();                                //to take away starting point
        int step = path.back(); 
        path.pop_back(); 

        m[oy][ox] = 0;          //erasing old guard
        oy = graph.y(step);     //updating coordinates
        ox = graph.x(step); 
        m[oy][ox] = 5;          //putting new guard
        if (oy == y && ox == x) //check for endgame conditions
            endgame = 2; 
    }

    void setPath()
    {
        while (!oldPath.empty()){   //erase old path
            int a = oldPath.back();
            oldPath.pop_back(); 
            if(m[graph.y(a)][graph.x(a)] != 5) //if there is a gaurd do not erase him 
                m[graph.y(a)][graph.x(a)] = 0;  
        }

        vector<int> path = graph.dijkstra(y,x,19,19); // find path to X

        if (path.empty())//gaurd cut path from player and x we returned an empty path-> end game
        {   
            endgame = 2; //end game 
            return ; 
        } 

        oldPath = path;                         //for next run make old path = new path
        while (!path.empty()){                  //give coordinates of path a value of 4 for displayMaze()
            int a = path.back();
            path.pop_back(); 
            if (m[graph.y(a)][graph.x(a)] == 5) //if theres a gaurd in out path 
            {
                graph.uncrossable = a;          //mark vertex as uncrossable
                setPath();                      //setPath from the begening (erase old (aka this one) and create a new based on uncrossable)
                return; 
            }else if (m[graph.y(a)][graph.x(a)] == 0)
               m[graph.y(a)][graph.x(a)] = 4; 
        }

        m[y][x] = 2; //place player again
        m[19][19] = 3; //place X again
    }

    void displayMaze()
    {
        cout << "_________________________________________" << endl; 
        for(int i = 0; i < 20; ++i) {   //y-axis
            cout << "|" ;
            for(int j = 0; j < 20; ++j) {   //x-axis
                if (m[i][j] == 0)
                    cout << "  "; 
                else if (m[i][j] == 1)
                    cout << "# ";
                else if (m[i][j] == 2)
                    cout << "P "; 
                else if (m[i][j] == 3)
                    cout << "X ";
                else if (m[i][j] == 4)
                    cout << ". "; 
                else if (m[i][j] == 5)
                    cout << "O "; 
            }
            cout << "|" << endl; 
        }
        cout << "_________________________________________" << endl; 
        cout << "Press\t A for left \t D for right\t W for up\t S for down then PRESS ENTER:"<<endl; 
    }

    void move(char key)
    {
        if (key =='w' && y!=0 && m[y-1][x] != 1)    //going up
        {   m[y][x] = 0;
            y--; 
            m[y][x] = 2;}
        if (key =='s' && y!=19 && m[y+1][x] != 1)   //going down
        {  m[y][x] = 0;
            y++; 
            m[y][x] = 2;}
        if (key =='d'&& x!=19 && m[y][x+1] != 1)    //going right
        {   m[y][x] = 0;
            x++; 
            m[y][x] = 2;}
        if (key =='a' && x!=0 && m[y][x-1] != 1)    //going left
        {   m[y][x] = 0;
            x--; 
            m[y][x] = 2;}

        if (y == oy && x == ox) //check for endgame conditions
           endgame = 2; 
        if (y == 19 && x == 19)
           endgame = 1; 
    }

    void setUpMaze()
    {
        generateMazeGraph();
        graph.uncrossable = -1; 
        setPath(); 
        m[y][x] = 2; //place player 
        m[19][19] = 3; //place X 
        m[oy][ox] = 5; //place gaurd
        displayMaze(); 
    }

};  

int main ()
{
    //give instructions: 
    for (int i = 0; i < 7; i++)
            cout << "_________________________________________" << endl; 
    cout << "         WELCOME TO MAZE RUNNER" <<endl; 
    cout << "The goal of the game is to reach the \"X\""<< endl; 
    cout << "   You can move by the A, W, S, D keys "<< endl; 
    cout << "  BUT BE CAREFUL: while you're trying to" << endl; 
    cout << "follow the path to \"X\" a gaurd \"O\" can see"<< endl; 
    cout << "      you and is trying to catch you"<< endl; 
    cout << "So take detours, the path will automatically"<< endl; 
    cout << "readjust to the shortest path to \"X\" but" << endl ;
    cout << "   don't trap yourself in any dead ends" << endl; 
    cout << "     PRESS ANY KEY AND ENTER TO START" << endl; 
    for (int i = 0; i < 6; i++)
            cout << "_________________________________________" << endl; 

    char x;     //input any keyboard stroke to continue
    cin>> x; 

    maze m;
    m.setUpMaze(); 

    m.endgame = false; 

    while (!m.endgame)
    { 
        char k; 
        cin >> k;
        m.guardMove();
        m.move(k);
        m.setPath();  
        if (!m.endgame) //check endgame conditions before printing(skip if yes and exit while)
           m.displayMaze();
    }

    //print Game Over messagea
    if (m.endgame==1){  //Won
        for (int i = 0; i < 10; i++)
            cout << "_________________________________________" << endl; 
        cout << "\t      Game Over!" << endl;
        cout << "\t   YOU'VE REACHED X"<< endl; 
        for (int i = 0; i < 10; i++)
            cout << "_________________________________________" << endl; 
    }else if (m.endgame==2){
        for (int i = 0; i < 10; i++)
            cout << "_________________________________________" << endl; 
        cout << "\t      Game Over!" << endl;
        cout << "\t THE GAURD CATCHED YOU!"<< endl; 
        cout << "\t better luck next time"<< endl; 
        for (int i = 0; i < 9; i++)
            cout << "_________________________________________" << endl; 
    }

}