#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stack>

using namespace std;

//sort points by angle from origin then distance from origin
bool wayToSort(point2D a, point2D b) {
  //check angle from origin is equal
  if (atan2(a.y, a.x) == atan2(b.y, b.x)){
    //check if y coordinate is equal
    if (a.y == b.y){
      return a.x < b.x;
    }
    return a.y < b.y;
  }
  return atan2(a.y, a.x) < atan2(b.y, b.x);
}

/* check for and delete duplicates in points vector */
vector<point2D> delete_duplicates(vector<point2D> p){
  //loop through vector and check if point and following point are equal
  int i = 0;
  while (i < (p.size() - 1)){
    if ((p[i].x == p[i+1].x) && (p[i].y == p[i+1].y)){
      //if equal, erase duplicate
      p.erase(p.begin()+(i+1));
    }
    else{
      i++;
    }
  }
  return p;
}


/* **************************************** */
/* returns the signed area of triangle abc. The area is positive if c
   is to the left of ab, and negative if c is to the right of ab
 */
int signed_area2D(point2D a, point2D b, point2D c) {
  int area = (((b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y))/2);
  //printf("area: %f \n", area);
  return area;
}


/* **************************************** */
/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2D p, point2D q, point2D r) {
  if (signed_area2D(p, q, r) == 0){
    return 1;
  }
  else{
    return 0;
  }
}

/* **************************************** */
/* return 1 if c is  strictly left of ab; 0 otherwise */
int left_strictly(point2D a, point2D b, point2D c) {
  if (signed_area2D(a, b, c) > 0){
    return 1;
  }
  else{
    return 0;
  }
}

/* return 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2D a, point2D b, point2D c) {
    if (left_strictly(a, b, c) || collinear(a, b, c)){
      return 1;
    }
    else{
      return 0;
    }
}

/* sort points by angle ccw from min_y */
vector<point2D> ccw_sort(vector<point2D> p) {
  //find lowest y point
  int min_y = 0;
  for (int i=1; i < p.size(); i++){
    if (p[i].y < p[min_y].y){
      min_y = i;
    }
  }

  //put lowest y point in first spot in vector of points
  point2D temp = p[0];
  p[0] = p[min_y];
  p[min_y] = temp;

  //modify so that points are located as if min_y is located at origin
  for (int i=1; i < p.size(); i++){
    p[i].y = p[i].y - p[0].y;
    p[i].x = p[i].x - p[0].x;
  }

  //sort by angle
   sort(p.begin() + 1, p.end(), wayToSort);

   //delete duplicates
   vector<point2D> points = delete_duplicates(p);

   //revert to original coordinates
   for (int i=1; i < points.size(); i++){
     points[i].y = points[i].y + points[0].y;
     points[i].x = points[i].x + points[0].x;
   }

   return points;
}

//return top element of stack
point2D first(vector<point2D> result){
  return result[result.size() - 1];
}

//return second element of stack
point2D second(vector<point2D> result){
  return result[result.size() - 2];
}

//return third element of stack
point2D third(vector<point2D> result){
  return result[result.size() - 3];
}

//find middle point of three collinear points
int remove_middle(point2D one, point2D two, point2D three){
  //sum x and y components
  int a_xy = abs(one.x + one.y);
  int b_xy = abs(two.x + two.y);
  int c_xy = abs(three.x + three.y);

  //if a is in the middle of b and c
  if(((a_xy >= b_xy) && (a_xy <= c_xy)) || ((a_xy <= b_xy) && (a_xy >= c_xy))) {
      return 1;
  }
  //if b is in the middle of a and c
  else if(((b_xy >= a_xy) && (b_xy <= c_xy)) || ((b_xy <= a_xy) && (b_xy >= c_xy))) {
      return 2;
  }
  //if c is in the middle of a and b
  else{
    return 3;
  }
}

/* compute the convex hull of the points in p; the points on the CH
   are returned as a vector
*/
vector<point2D> graham_scan(vector<point2D> p) {
  //sort points counterclockwise (and delete duplicates)
  vector<point2D> sorted = ccw_sort(p);

  printf("computing hull..");

  //add contents of stack to result
  vector<point2D> result;
  result.push_back(sorted[0]);
  result.push_back(sorted[1]);

  for (int i=2; i < sorted.size(); i++){
    //check that top of stack is convex
    if ((result.size() > 2) && (!left_on(third(result), second(result), first(result)))){
      result.pop_back();
    }

    //if point is left of or collinear, add to result
    if(left_on(second(result), first(result), sorted[i])){
      result.push_back(sorted[i]);

      //if top three points collinear, get rid of the point in the middle
      if(collinear(third(result), second(result), first(result))){
        int middle = remove_middle(first(result), second(result), third(result));
          result.erase(result.begin() + result.size() - middle);
      }
      //if top two points collinear with origin, remove middle point
      if ((collinear(sorted[0], second(result), first(result))) && (result.size() > 2)){
        int middle = remove_middle(first(result), second(result), sorted[0]);
          result.erase(result.begin() + result.size() - middle);
      }
    }

    else {
      //while new point is right of top points in stack, pop point from stack
      while(!left_on(second(result), first(result), sorted[i])){
          result.pop_back();
      }
      result.push_back(sorted[i]);

      //if top three points collinear, get rid of the point in the middle
      if(collinear(third(result), second(result), first(result))){
        int middle = remove_middle(first(result), second(result), third(result));
          result.erase(result.begin() + result.size() - middle);
      }
      //if top two points collinear with origin, remove middle point
      if ((collinear(sorted[0], second(result), first(result))) && (result.size() > 2)){
        int middle = remove_middle(first(result), second(result), sorted[0]);
          result.erase(result.begin() + result.size() - middle);
      }
    }
  }

  result.push_back(sorted[0]);
  printf("..done\n");
  return result;
}
