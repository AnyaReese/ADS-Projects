#include <iostream>
#include <vector>

using namespace std;
// Define a structure to represent a connector and its possible fence connections.
struct Connector {
    int degree;
    int row;
    int col;
    int up, down, left, right;
};

struct Site {
    int x;
    int y;
};
struct Path {
    int valid;
    Site begin;
    Site end;
};


struct Garden {
    int connector_index;
    int degree;
};

int ConnectorNum = 0;
int PathNum;
int TotalPathnum = 0;
bool solution = false;

// Function to restore the garden diagram.
void restoreGarden(int PathIndex, int ValidPathNum, const vector<vector<Garden> > &garden, vector<Path> &paths,
                   vector<Connector> &connectors) {
    // Involve backtracking and checking the validity of the fence placement.
    if (PathIndex > TotalPathnum || solution || ValidPathNum > PathNum) return;

    int Connector1_index;
    int Connector2_index;

    // Check the validity of the fence placement
    if (ValidPathNum == PathNum) {
        
        for (int i = 0; i < ConnectorNum; ++i) {
            if (connectors[i].degree != garden[connectors[i].row][connectors[i].col].degree) {
                // cout<<"fail 2";
                return;
            }
        }
        solution = true;
        for (int i = 0; i < TotalPathnum; i++) {
            if (paths[i].valid == 1) {
                if (paths[i].begin.y < paths[i].end.y) { // →
                    Connector1_index = garden[paths[i].begin.x][paths[i].begin.y].connector_index; // The Connector index of the begin of the path
                    Connector2_index = garden[paths[i].end.x][paths[i].end.y].connector_index;     // The Connector index of the end of the path
                    connectors[Connector1_index].right = 1;
                    connectors[Connector2_index].left = 1;
                } else if (paths[i].begin.x < paths[i].end.x) { // ↓
                    Connector1_index = garden[paths[i].begin.x][paths[i].begin.y].connector_index;
                    Connector2_index = garden[paths[i].end.x][paths[i].end.y].connector_index;
                    connectors[Connector1_index].down = 1;
                    connectors[Connector2_index].up = 1;
                }
            }
        }
    }


    // Reverse the all potential paths

    Connector1_index = garden[paths[PathIndex].begin.x][paths[PathIndex].begin.y].connector_index; // The Connector index of the begin of the path
    Connector2_index = garden[paths[PathIndex].end.x][paths[PathIndex].end.y].connector_index;     // The Connector index of the end of the path

    if( connectors[Connector1_index].degree+1 <= garden[paths[PathIndex].begin.x][paths[PathIndex].begin.y].degree &&
        connectors[Connector2_index].degree+1 <= garden[paths[PathIndex].end.x][paths[PathIndex].end.y].degree){
        paths[PathIndex].valid = 1;
        connectors[Connector1_index].degree++;
        connectors[Connector2_index].degree++;
        restoreGarden(PathIndex + 1, ValidPathNum + 1, garden, paths, connectors);
        connectors[Connector1_index].degree--;
        connectors[Connector2_index].degree--;
    }


    paths[PathIndex].valid = 0;
    restoreGarden(PathIndex + 1, ValidPathNum, garden, paths, connectors);

    return;

}


int main() {
    int n, m; // Number of rows and columns in the garden.
    vector<vector<Garden> > garden; // 2D array to store the garden layout.
    vector<Connector> connectors(3000); // Vector to store all connectors.
    vector<Path> paths(3000);
    int degree;
    int totaldegree = 0;
    cin >> n >> m;

/*---------------------------------------------------Create the garden graph--------------------------------------------------*/

    garden.resize(n + 1); // initialize the garden
    for (int i = 0; i <= n; ++i) {
        garden[i].resize(m + 1);
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            cin >> degree;
            garden[i][j].degree = degree;
            if (degree > 0) {
                garden[i][j].connector_index = ConnectorNum;
                connectors[ConnectorNum].row = i;
                connectors[ConnectorNum].col = j;
                connectors[ConnectorNum++].degree = 0;
                totaldegree += degree;
            }
        }
    }
    if (totaldegree % 2 == 1){
        cout << "No Solution" << endl;
        return 0;
    }
    PathNum = totaldegree / 2; // Calculate the total number of paths based on the total degree


/*---------------------------------------------------Find potential paths-------------------------------------------------------*/

    int pre_degree;
    int pre_pos;
    for (int i = 1; i <= n; ++i) {  // find the horizontal path
        pre_degree = garden[i][1].degree;
        pre_pos = 1;
        for (int j = 2; j <= m; ++j) {
            // if(garden[i][j].prefixsum==garden[i][m].prefixsum) break; // if there is no connector after j, then break

            if (pre_degree == 0 && garden[i][j].degree != 0) {
                pre_degree = garden[i][j].degree;
                pre_pos = j;
            } // Initialize pre
            else if (pre_degree != 0 && garden[i][j].degree != 0) {
                paths[TotalPathnum].begin.x = i;
                paths[TotalPathnum].begin.y = pre_pos;
                paths[TotalPathnum].end.x = i;
                paths[TotalPathnum++].end.y = j;
                pre_pos = j;
            }

        }
    }

    for (int j = 1; j <= m; ++j) {  // find the vertical path
        pre_degree = garden[1][j].degree;
        pre_pos = 1;
        for (int i = 2; i <= n; ++i) {
            // if(garden[i][j].prefixsum==garden[n][j].prefixsum) break; // if there is no connector after j, then break

            if (pre_degree == 0 && garden[i][j].degree != 0) {
                pre_degree = garden[i][j].degree;
                pre_pos = i;
            } // Initialize pre
            else if (garden[i][j].degree != 0 && pre_degree != 0) {
                paths[TotalPathnum].begin.x = pre_pos;
                paths[TotalPathnum].begin.y = j;
                paths[TotalPathnum].end.x = i;
                paths[TotalPathnum++].end.y = j;
                pre_pos = i;
            }
        }
    }

/*---------------------------------------------------Restore the garden---------------------------------------------------------*/

    restoreGarden(0, 0, garden, paths, connectors); // Restore the garden.

/*----------------------------------------------------------Output--------------------------------------------------------------*/
    if (!solution) {
        cout << "No Solution" << endl;
    } else {
        for (int i = 0; i < ConnectorNum; ++i) {
            cout << connectors[i].row << " " << connectors[i].col << " "
                 << connectors[i].up << " " << connectors[i].down << " "
                 << connectors[i].left << " " << connectors[i].right << "\n";
        }
    }

    return 0;
}
