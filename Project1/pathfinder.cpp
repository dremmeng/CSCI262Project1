/*
    pathfinder.cpp
        
    Method implementations for the pathfinder class.
    
    assignment: CSCI 262 Project - Pathfinder        

    author: <your name here>
*/

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
using namespace std;
#include "pathfinder.h"

using namespace std;

// constructor - optional, in case you need to initialize anything
pathfinder::pathfinder() { ; }

// public run() method - invokes private methods to do everything;
// returns the total elevation change cost of the best optimal path.
int pathfinder::run(string data_file_name, string save_file_name, bool use_recursion) {
    // suggested implementation - modify as desired!
    _use_recursion = use_recursion;
    if (!_read_data(data_file_name)) {
        cout << "Error reading map data from \"" << data_file_name << "\"!" << endl;
        return -1;
    }

    _draw_map();

    int best_cost = _draw_paths();

    _save_image(save_file_name);

    return best_cost;
}

/*******************/
/* PRIVATE METHODS */
/*******************/

// _read_data - read and store width, height, and elevation data from the
// provided data file; return false if something goes wrong
bool pathfinder::_read_data(string data_file_name) {
    // open input file
    ifstream file(data_file_name);
    string str = " ";
    vector<vector<int>> foo;
    // read in and store width and height
    file >> str;
    file >> _width;
    file >> str;
    file >> _height;
    _elevations = vector<vector<int>>(_height, vector<int>(_width));
    // read in and store elevation data
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            file >> _elevations[i][j];
        }
    }
    // close input file
    file.close();
    // return true if everything worked
    return true;
}

// _draw_map - draw the elevation data as grayscale values on our Picture
// object.
void pathfinder::_draw_map() {
    // TODO: determine min/max elevation values in map
    _min = _elevations[0][0];
    _max = _elevations[0][0];
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            if (_elevations[i][j] < _min)
            {
                _min = _elevations[i][j];
            }
            if (_elevations[i][j] > _max)
            {
                _max = _elevations[i][j];
            }
        }
    }
    // TODO: from min, max, determine a scaling factor for data to shift
    //       elevations into 0 - 255 range
    float factor =  (_max - _min)/255;
    // TODO: for each elevation point, determine its grayscale value and set the Picture point
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            int r, g, b;
            r = g = b = (_elevations[i][j] - _min) / factor;
            _image.set(i, j, r, g, b);
        }
    }
}

// _draw_paths() - find and draw optimal paths from west to east using either recursion or dynamic programming
// return the best optimal path cost
int pathfinder::_draw_paths() {
    // TODO: based on the _use_recursion field, compute minimum cost for every point on map using either
    //  recursion or dynamic programming. Keep track of next move necessary to attain min cost.
    //  This will require additional data structures!
    int lowest_cost = _max;
    for (int i = 0; i < _height-1; i++)
    {
        int cost = costtoeast(i,0);
        cout << cost << endl;
        if (cost < lowest_cost)
        {
            lowest_cost = cost;
        }

    }
    cout << lowest_cost;
    return 0;
}

int pathfinder::costtoeast(int y, int x)
{
    int cost1=0, cost2=0, cost3=0;
    if (x < _width-1)
    {
        if (y > 0)
        {
            cost1 = (((_elevations[x][y] - _elevations[x + 1][y - 1])) ^ 2) ^ (1 / 2) + costtoeast(y - 1, x + 1);
        }
        else
        {
            cost1 = 100000;
        }
        if (y < _height-2)
        {
            cost2 = (((_elevations[x][y] - _elevations[x + 1][y + 1])) ^ 2) ^ (1 / 2) + costtoeast(y + 1, x + 1);
        }
        else
        {
            cost2 = 100000;
        }
        cost3 = (((_elevations[x][y] - _elevations[x + 1][y])) ^ 2) ^ (1 / 2) + costtoeast(y, x + 1);
    }
    else if (x == _width-1)
    {
        cost1 = cost2 = cost3 = 0;
    }
    
    if ((cost1 < cost2) && (cost1 < cost3)) 
    {
        return cost1;
    }
    else if ((cost2 < cost1) && (cost2 < cost3))
    {
        return cost2;
    }
    else 
    {
        return cost3;
    }
}

// _save_image - we'll do this one for you :)
void pathfinder::_save_image(string save_file_name) {
    _image.save(save_file_name);
}

