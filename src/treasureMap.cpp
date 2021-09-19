#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{
    base = baseim;
    maze = mazeim;
    start = s;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2 * (pixel->r / 4);
    pixel->g = 2 * (pixel->g / 4);
    pixel->b = 2 * (pixel->b / 4);

}

void treasureMap::setRed(PNG & im, pair<int,int> loc){
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 255;
    pixel->g = 0;
    pixel->b = 0;

}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){
    d = d%64;
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);

    pixel->r = (pixel->r >> 2) * 4 + (d >> 4 & 3);
    pixel->g = (pixel->g >> 2) * 4 + ((d >> 2) & 3);
    pixel->b = (pixel->b >> 2) * 4 + (d & 3);
    


}

PNG treasureMap::renderMap(){
    PNG baseCopy = base;
    vector<vector<bool>> visited;

    visited.resize(baseCopy.width());
    for (unsigned int i = 0; i < visited.size(); i++) {
        for (unsigned int j = 0; j < baseCopy.height(); j++)
            visited[i].push_back(false);
    }
    vector<vector<int>> distances;
    distances.resize(baseCopy.width());
    for (unsigned int i = 0; i < distances.size(); i++) {
        distances[i].resize(baseCopy.height());
    }

    Queue<pair<int, int>> toVisit;

    visited[start.first][start.second] = true;
    distances[start.first][start.second] = 0;
    setLOB(baseCopy, start, distances[start.first][start.second]);
    toVisit.enqueue(start);

    while (!toVisit.isEmpty()) {
        pair<int, int> curr = toVisit.dequeue();
        int currDist = distances[curr.first][curr.second];
        vector<pair<int,int>> n = neighbors(curr);
        for (unsigned int i = 0; i < n.size(); i++) {
            pair<int, int> next = n[i];
            if (good(visited, curr, next)) {
                visited[next.first][next.second] = true;
                int nextDis = distances[next.first][next.second] = currDist + 1;
                setLOB(baseCopy, next, nextDis);
                toVisit.enqueue(next);
            }
        }
        currDist++;
    }
    return baseCopy;


}

PNG treasureMap::renderMaze(){
    PNG baseCopy = base;
    vector<vector<bool>> visited;

    visited.resize(baseCopy.width());
    for (unsigned int i = 0; i < visited.size(); i++) {
        for (unsigned int j = 0; j < baseCopy.height(); j++)
            visited[i].push_back(false);
    }

    Queue<pair<int, int>> toVisit;

    visited[start.first][start.second] = true;
    setRed(baseCopy, start);
    toVisit.enqueue(start);

    while (!toVisit.isEmpty()) {
        pair<int, int> curr = toVisit.dequeue();
        vector<pair<int,int>> n = neighbors(curr);
        for (unsigned int i = 0; i < n.size(); i++) {
            pair<int, int> next = n[i];
            if (good(visited, curr, next)) {
                setGrey(baseCopy, next);
                visited[next.first][next.second] = true;
                toVisit.enqueue(next);
            }
        }
    }
    for (int i = -3; i < 4; i++) {
        int col = start.first + i;
        for (int j = -3; j < 4; j++) {
            int row = start.second + j;
            if (row >= 0 && row < ((int) baseCopy.height()) && col >= 0 && col < ((int) baseCopy.width()))
                setRed(baseCopy, make_pair(col, row));
        }
    }
    return baseCopy;

}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
    bool inRange = (next.first < (int)maze.width()) && (next.second < (int)maze.height()) && (next.first >= 0) && (next.second >= 0);
    if (!inRange) return false;
    bool unvisited = !v[next.first][next.second];
    bool sameColour = *(maze.getPixel(curr.first, curr.second)) == *(maze.getPixel(next.first, next.second));

    return unvisited && sameColour;
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
    vector<pair<int,int>> neighbors;

    neighbors.push_back(make_pair(curr.first - 1, curr.second));  //left
    neighbors.push_back(make_pair(curr.first, curr.second + 1));  //below
    neighbors.push_back(make_pair(curr.first + 1, curr.second));  //right
    neighbors.push_back(make_pair(curr.first, curr.second - 1));  //above
    return neighbors;
}

