#include <iostream>
#include <fstream> 
using namespace std;

const int MAX = 50;                 // the largest number of rows and columns in the maze

char grid[MAX][MAX];                 // two dimensional array to hold the maze 

bool explored[MAX][MAX];            // array to check if the node was explored before

int parentRow[MAX][MAX];            //this 2D array is used to trace the path from the goal back to the start index like linked list
int parentCol[MAX][MAX];


//variables to hold the length and width of the maze
int height = 0;
int width = 0;

int startRow, startCol;              // index of the start position
int goalRow, goalCol;                // index of the goal position

void loadMaze(const char* FileName)
{
    ifstream file(FileName);

    char line[MAX];                  // array of chars to read each row in the maze

    int startFlag = 0, goalFlag = 0;  //we are going to use them to make sure that there is start and goal positions

    //reading the file content
    while (file.getline(line, MAX))
    {
        //the while loop reads rows' elements till we reach new line 
        int col = 0;
        while (line[col] != '\0')
        {
            grid[height][col] = line[col];

            // finding the start position
            if (line[col] == 'A')
            {
                startFlag++;
                startRow = height;
                startCol = col;
            }

            // finding the goal position
            if (line[col] == 'B')
            {
                goalFlag++;
                goalRow = height;
                goalCol = col;
            }

            col++;
        }
        height++;
        width = col;
    }

    if (!(startFlag && goalFlag))
    {
        cout << "unvalid maze" << endl;
    }

}



void printMaze()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cout << grid[i][j];
        }
        cout << endl;
    }

}



//we need a function to check if the node is a wall or part of the maze path

bool isValid(int r, int c)
{
    if (r<0 || r>height-1) return false;
    if (c<0 || c>width-1) return false;
    if (grid[r][c] == '#') return false;
    if (explored[r][c] == true) return false;

    return true;
}


void solve()
{
    //the solve mechanism is BFS. it puts nodes in a queue where we explore nodes first input first ouput


    //in the queue we put new nodes in the array and explore nodes from the start of the array so we need two indices one for the last node in the queue
    //and the other one is for the the array start position to take a node to explore this is not memory friendly but easy to implement and it is better
    //to use dynamic memory allocation but anyway it is simpler.


    // the max number of elements needed is N^2 in case the grid has no obstacles and where the start is in the most bottom left and 
    // the goal is in the most up right
    //frontier for exploring nodes
    int queueRow[MAX * MAX];       
    int queueCol[MAX * MAX];

    

    int queueFront = 0;
    int queueRear = 0;
    //add the start node position to the queue
    queueRow[queueRear] = startRow;
    queueCol[queueRear] = startCol;

    explored[startRow][startCol] = true;
    queueRear++;

    while (queueFront < queueRear)
    {
        //take the first node index in the queue
        int r = queueRow[queueFront];
        int c = queueCol[queueFront];

        queueFront++;

        //check if the node is the goal
        if (r == goalRow && c == goalCol)
        {
            break;
        }

        //it is not the goal then now add the neighboring possible nodes to the queue (1,0) (-1,0) (0,1) (0,-1)
        int dirRow[4] = { -1,1,0,0 };
        int dirCol[4] = { 0,0,-1,1 };

        for (int i = 0; i < 4; i++)
        {
            int neibourRow = r + dirRow[i];
            int neibourCol = c + dirCol[i];


            if (isValid(neibourRow, neibourCol))
            {
                queueRow[queueRear] = neibourRow;
                queueCol[queueRear] = neibourCol;
                queueRear++;

            
                explored[neibourRow][neibourCol] = true; 



                parentRow[neibourRow][neibourCol] = r;
                parentCol[neibourRow][neibourCol] = c;
            }
        }

    }

    //now we have reached the goal and we need to get the path from start to goal
    //each explored node has a parent node so we can trace back from the goal to the start by tracing parent nodes

    int traceRow = goalRow;
    int traceCol = goalCol;

    int traceRow_container; 
    int traceCol_container;
    //loop till we reach the start position
    while (!(traceRow == startRow && traceCol == startCol))
    {

        traceRow_container = parentRow[traceRow][traceCol];
        traceCol_container = parentCol[traceRow][traceCol];

        if (traceRow_container == startRow && traceCol_container == startCol)
        {
            break;
        }
        grid[traceRow_container][traceCol_container] = '*';
        

        traceRow = traceRow_container;    
        traceCol = traceCol_container;

        

        /*you may say why not we do the following traceRow = parentRow[traceRow][traceCol]; 
        traceCol = parentCol[traceRow][traceCol]; and this is because traceRow will change first then you will not
        get the right traceCol for example assume you are in 2 0 then first line of code makes trace row 3 so the
        second line will deal with 3 not 2 parentCol[3][0]
        */
    }


}


int main() {

    loadMaze("maze2.txt");
    printMaze();
    cout << endl << endl;

    solve();
    printMaze();

    
    return 0;
}