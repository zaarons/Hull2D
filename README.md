Zoe Aarons (4/17/18):  https://github.com/zaarons/Hull2D

This program takes a user input (number of points) and uses Graham scan to find the convex hull of this set of points. 
Points are sorted by angle with respect to the point with the minimum y value (using maximum x as the tie breaker). Points are
then looped through and added to the convex hull. If they fall to the left or are collinear with the previous two points, the new
point is added to the vector. Else points are popped from the vector until the new point is to the left of or collinear with the two
last points in the vector.

Once the convex hull is calculated, it is drawn for multiple initializers that account for many degenerate cases. The number of
points can be changed (user input) and the initializers can be viewed successively by pressing 'i'.

There are two current bugs. There is one incorrect point on the star initializer for n=100000 and for the butterfly at n=1000000.
