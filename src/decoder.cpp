#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {
      vector<vector<bool>> v; //visited or not
      vector<vector<pair<int, int>>> last; //the originated p lead to curr
      Queue<pair<int, int>> toVisit;
      vector<vector<int>> d;

      v.resize(mapImg.width());
      for (unsigned int i = 0; i < v.size(); i++) {
         for (unsigned int j = 0; j < mapImg.height(); j++)
            v[i].push_back(false);
      }

      last.resize(mapImg.width());
      for (unsigned int i = 0; i < last.size(); i++) {
        last[i].resize(mapImg.height());
      }
      
      d.resize(mapImg.width());
      for (unsigned int i = 0; i < d.size(); i++) {
         for (unsigned int j = 0; j < mapImg.height(); j++)
            d[i].push_back(0);
      }
      v[start.first][start.second] = true;
      toVisit.enqueue(start);
      while (!toVisit.isEmpty()) {
         pair<int, int> curr = toVisit.dequeue();
         int currDist = d[curr.first][curr.second];
         vector<pair<int,int>> n = neighbors(curr);
         for (unsigned int i = 0; i < n.size(); i++) {
            pair<int, int> next = n[i];
            if (good(v, d, curr, next)) {
               v[next.first][next.second] = true;
               last[next.first][next.second] = curr;
               d[next.first][next.second] = currDist + 1;
               toVisit.enqueue(next);
            } 
        }
      }
      

      pair<int, int> t = findTreasure(d);
      pair<int, int> curr = t;      //set treasue as first point
      Stack<pair<int, int>> st;  //store the path backwards
      while ((curr.first != start.first) || (curr.second != start.second)) {
         st.push(curr);
         curr = last[curr.first][curr.second]; //set curr to the originated p of curr
      }
      st.push(start); //include 
      while (!st.isEmpty()) pathPts.push_back(st.pop());
    
}

pair<int,int> decoder::findTreasure(vector<vector<int>> d) {
   int max = 0;
   pair<int, int> t;
   for (unsigned int i = 0; i < d.size(); i++) {
      for (unsigned int j = 0; j < d[i].size(); j++) {
         if (d[i][j] != 0) {
            if (d[i][j] >= max) {
               max = d[i][j];
               t = make_pair(i, j);
            }
         }
      }
   }
   return t;
}

PNG decoder::renderSolution(){
   PNG solution = mapImg;
   for (unsigned int i = 0; i < pathPts.size(); i++) {
      pair<int, int> curr = pathPts[i];
      RGBAPixel *p = solution.getPixel(curr.first, curr.second);
      p->r = 255;
      p->g = 0;
      p->b = 0;
   }
   return solution;
}

PNG decoder::renderMaze(){
   PNG mazeImg = mapImg;
   vector<vector<bool>> v;
   vector<vector<int>> d;

   v.resize(mazeImg.width());
   for (unsigned int i = 0; i < v.size(); i++) {
      for (unsigned int j = 0; j < mazeImg.height(); j++)
         v[i].push_back(false);
   }

   d.resize(mazeImg.width());
   for (unsigned int i = 0; i < d.size(); i++) {
      for (unsigned int j = 0; j < mazeImg.height(); j++)
         d[i].resize(mazeImg.height());
   }

   Queue<pair<int, int>> toVisit;

   v[start.first][start.second] = true;
   d[start.first][start.second] = 0;
   setRed(mazeImg, start);
   toVisit.enqueue(start);

   while (!toVisit.isEmpty()) {
      pair<int, int> curr = toVisit.dequeue();
      int currDist = d[curr.first][curr.second];
      vector<pair<int,int>> n = neighbors(curr);
      for (unsigned int i = 0; i < n.size(); i++) {
         pair<int, int> next = n[i];
         if (good(v, d, curr, next)) {
            setGrey(mazeImg, next);
            v[next.first][next.second] = true;
            d[next.first][next.second] = currDist + 1;
            toVisit.enqueue(next);
         }
      }
   }
   for (int i = -3; i < 4; i++) {
        int col = start.first + i;
        for (int j = -3; j < 4; j++) {
            int row = start.second + j;
            if (row >= 0 && row < ((int) mazeImg.height()) && col >= 0 && col < ((int) mazeImg.width()))
                setRed(mazeImg, make_pair(col, row));
        }
    }
   return mazeImg;


}

void decoder::setGrey(PNG & im, pair<int,int> loc){
   RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2 * (pixel->r / 4);
    pixel->g = 2 * (pixel->g / 4);
    pixel->b = 2 * (pixel->b / 4);

}

void decoder::setRed(PNG & im, pair<int,int> loc){
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 255;
    pixel->g = 0;
    pixel->b = 0;

}

pair<int,int> decoder::findSpot(){
   return pathPts[pathPts.size() - 1];

}

int decoder::pathLength(){
   return pathPts.size();
}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){
   if ((next.first >= (int)mapImg.width()) || (next.second >= (int)mapImg.height()) || (next.first < 0) || (next.second < 0))
      return false;
   bool unvisited = !v[next.first][next.second];
   int currD = d[curr.first][curr.second];
   RGBAPixel *pixel = mapImg.getPixel(next.first, next.second);
   bool sameColour = compare(*pixel, currD);
   return unvisited && sameColour;

}

bool decoder::inRange(pair<int,int> loc){
   if ((loc.first >= (int)mapImg.width()) || (loc.second >= (int)mapImg.height()) || (loc.first < 0) || (loc.second < 0))
      return false;
   return true;
}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {
   vector<pair<int,int>> neighbors;

    neighbors.push_back(make_pair(curr.first - 1, curr.second));  //left
    neighbors.push_back(make_pair(curr.first, curr.second + 1));  //below
    neighbors.push_back(make_pair(curr.first + 1, curr.second));  //right
    neighbors.push_back(make_pair(curr.first, curr.second - 1));  //above
    return neighbors;
}


bool decoder::compare(RGBAPixel p, int d){
   int red = p.r;
   int green = p.g;
   int blue = p.b;
   int v = (red % 4) * 16 + (green % 4) * 4 + blue % 4;
   return v == (d + 1) % 64;

}