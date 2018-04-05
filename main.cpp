#include <iostream>
#include <fstream>
#include <cfloat>
#include <vector>
#include <queue>
#include <cmath>
#include <sstream>

using namespace std;

#define X 30
#define Y 30
#define Z 30

class node
{
public:
    int x, y, z;
    bool isObs;
    node* parent;
    double f, g, h;
    node();
    node(int _x, int _y, int _z, bool _isObs);
};

node::node(int _x, int _y, int _z, bool _isObs) {
    x = _x;
    y = _y;
    z = _z;
    isObs = _isObs;
    parent = nullptr;
    f = FLT_MAX;
    g = FLT_MAX;
    h = FLT_MAX;
}

node::node() = default;

void map_init(vector<vector<vector<node>>> &map);
void Astarsearch(vector<vector<vector<node>>> &map, node* S_node, node* G_node);

int main() {
    vector<vector<vector<node>>> map(X,vector<vector<node>>(Y,vector<node>(Z)));
    fstream myfile;
    myfile.open("map.txt", fstream::out);
    myfile << "x\ty\tz\tvalue"<<endl;
    int i, j , k;
    for(i = 0; i < X; i++){
        for(j = 0; j < Y; j++){
            for(k = 0; k < Z; k++)
            {
                if(i>20 && i<28 && j>20 && j<28)
                {
                    myfile<<i<<"\t"<<j<<"\t"<<k<<"\t"<<1<<endl;
                }
                else
                {
                    myfile<<i<<"\t"<<j<<"\t"<<k<<"\t"<<0<<endl;
                }
            }
        }
    }
    myfile.close();

    map_init(map);
    node* S_node = &map[0][0][0];
    node* G_node = &map[X-1][Y-1][Z-1];
    Astarsearch(map, S_node, G_node);

    return 0;
};

void map_init(vector<vector<vector<node>>> &map)
{
    int i, j, k, value;
    fstream myfile;
    myfile.open("map.txt");
    string temp;
    getline(myfile,temp);
    while(getline(myfile,temp)){
        stringstream tempStream(temp);
        tempStream>>i>>j>>k>>value;
        map[i][j][k] = node(i, j, k, static_cast<bool>(value));
    }
    myfile.close();
}

class node_comp
{
public:
    bool operator()(node* n1, node* n2)
    {
        return n1->h <= n2->h;
    }
};

bool isDest(node* temp, node* G_node);
bool isValid(int i, int j, int k, vector<vector<vector<node>>> &map);
bool isOnClosed_list(vector<node*> Closed_list, vector<vector<vector<node>>> &map, node* temp);
void tracePath(node* temp, node* S_node);
bool judgeupdate(double hnew, int i, int j, int k, vector<vector<vector<node>>> &map);

void Astarsearch(vector<vector<vector<node>>> &map, node* S_node, node* G_node)
{
    priority_queue<node*, vector<node*>, node_comp> Open_list;
    vector<node*> Closed_list;
    S_node -> f = 0;
    S_node -> g = 0;
    S_node -> h = 0;
    S_node -> parent = S_node;
    Open_list.push(S_node);

    int i, j, k;
    int di, dj, dk;

    while(!Open_list.empty())
    {
        cout<<Open_list.size()<<endl;
        node* temp = Open_list.top();
        Open_list.pop();
        if(isDest(temp, G_node))
        {
            tracePath(temp, S_node);
            break;
        }
        i = temp->x;
        j = temp->y;
        k = temp->z;
        double gnew, fnew, hnew;
        for(di = -1; di < 2; di++)
        {
            for(dj = -1; dj < 2; dj++)
            {
                for(dk = -1; dk < 2; dk++)
                {
                    if(isValid(i+di, j+dj, k+dk, map))
                    {
                        if(!(di == 0 && dj == 0 && dk == 0))
                        {
                            if((!isOnClosed_list(Closed_list, map, temp))&&(!map[i+di][j+dj][k+dk].isObs))
                            {
                                gnew = sqrt((i+di-G_node->x)^2+(j+dj-G_node->y)^2+(k+dk-G_node->z)^2);
                                fnew = temp->f + sqrt(di^2 + dj^2 + dk^2);
                                hnew = gnew + fnew;
                                if(judgeupdate(hnew, i+di, j+dj, k+dk, map) == true)
                                {
                                    map[i+di][j+dj][k+dk].parent = temp;
                                    map[i+di][j+dj][k+dk].g = sqrt((i+di-G_node->x)^2+(j+dj-G_node->y)^2+(k+dk-G_node->z)^2);
                                    map[i+di][j+dj][k+dk].f = temp->f + sqrt(di^2 + dj^2 + dk^2);
                                    map[i+di][j+dj][k+dk].h = map[i+di][j+dj][k+dk].g + map[i+di][j+dj][k+dk].f;
                                    Open_list.push(&map[i+di][j+dj][k+dk]);
                                }
                            }
                        }
                    }
                }
            }
        }
        Closed_list.push_back(temp);

    }
}

bool judgeupdate(double hnew, int i, int j, int k, vector<vector<vector<node>>> &map)
{
    if(map[i][j][k].h == FLT_MAX)
    {
        return true;
    }
    else if(map[i][j][k].h > hnew)
    {
        return true;
    }
    else{
        return false;
    }
}

bool isDest(node* temp, node* G_node)
{
    if((temp->x == G_node->x)&&(temp->y == G_node->y)&&(temp->z == G_node->z))
    {
        return true;
    }
    else{
        return false;
    }
}

bool isOnClosed_list(vector<node*> Closed_list, vector<vector<vector<node>>> &map, node* temp)
{
    int i, j;
    bool re_value = false;
    j = Closed_list.size();
    for(i = 0; i < j; i++)
    {
        if((Closed_list[i]->x == temp -> x) && (Closed_list[i]->y == temp -> y) && (Closed_list[i]->z == temp->z))
        {
            return !((temp -> x == 0) && (temp -> y == 0) && (temp -> z == 0));
        }
    }
    return re_value;
}

bool isValid(int i, int j, int k, vector<vector<vector<node>>> &map)
{
    return i >= 0 && i < X && j >= 0 && j < Y && k >= 0 && k < Z;
}

void tracePath(node* temp, node* S_node)
{
    vector<node> Path;
    Path.push_back(*temp);
    node* parent_temp;
    parent_temp = temp;
    fstream myfile;
    myfile.open("A_star_path.txt", fstream::out);
    myfile << "x\ty\tz"<<endl;
    while(!(parent_temp->x == S_node->x && parent_temp->y == S_node->y && parent_temp->z == S_node->z))
    {
        Path.push_back(*parent_temp);
        parent_temp = parent_temp->parent;
    }
    int i;
    for(i = 0; i < Path.size(); i++)
    {
        printf("(%d, %d, %d)", Path[i].x, Path[i].y, Path[i].z);
        myfile<<Path[i].x<<"\t"<<Path[i].y<<"\t"<<Path[i].z<<endl;
    }
    myfile.close();
}