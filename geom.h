#ifndef __geom_h
#define __geom_h

#include <vector>
#include <math.h>

using namespace std;

typedef struct _point2d {
  int x,y;
} point2D;

//sorts by angle. If angle is equal, sorts by distance from origin point
bool wayToSort(point2D a, point2D b);

//deletes duplicates from points vector
vector<point2D> delete_duplicates(vector<point2D> p);

/* returns 2 times the signed area of triangle abc. The area is
   positive if c is to the left of ab, 0 if a,b,c are collinear and
   negative if c is to the right of ab
 */
int signed_area2D(point2D a, point2D b, point2D c);

/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2D p, point2D q, point2D r);

/* return 1 if c is  strictly left of ab; 0 otherwise */
int left_strictly (point2D a, point2D b, point2D c);

/* return 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2D a, point2D b, point2D c);

/* sort points in a 2D vector in counterclockwise order from a given point*/
vector<point2D> ccw_sort(vector<point2D> p);

//return top element of stack
point2D first(vector<point2D> result);

//return second element of stack
point2D second(vector<point2D> result);

//return third element of stack
point2D third(vector<point2D> result);

/* compute the convex hull of the points in p; the points on the CH
   are returned as a list or vector
*/
vector<point2D> graham_scan(vector<point2D>);

#endif
