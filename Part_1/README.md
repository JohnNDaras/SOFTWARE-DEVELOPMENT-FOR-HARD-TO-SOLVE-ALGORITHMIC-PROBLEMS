**<p align=center>Algorithmic Approaches to Polygon Formation: Comparative Study of Incremental, Convex Hull, and Onion Methods**

Konstantinos Kitsos - 1900082

Ioannis Daras - sdi1800040

Georgios Tzathas - sdi1600168

Compilation: 

The files are in a directory, so the following commands are sufficient:

     cmake -DCGAL_DIR=/usr/lib/CGAL .
     make

The main.cpp is the main program that handles everything. Initially, it parses the command line arguments and stores them in variables. It reads from the input file, calls one of the three algorithms that return a polygon, and finally prints the results to the output file. Note that the area calculation function with the pick algorithm is commented out at the bottom of the main function to avoid interfering with the checks of the others.

Incremental Algorithm:

Consists of files utils.inc.hpp, utils.inc.cpp, and incremental.cpp. In the utils and its header file, there are helper functions:

    ascx, descx, ascy, descy are used as the third argument in sort() to sort the points for the initialization of the incremental according to the four criteria.

    find_vertex returns the position in the polygon where a vertex is located in the form of an integer index.

    is_visible checks the visibility of an edge from a point. It involves drawing two line segments seg1 and seg2 at the ends of the desired edge and counting the number of intersections with edges of the polygon. The implementation does not always find truly 'visible' edges but ensures that their ends are visible, allowing for potential replacement.

    select_edge implements the other criterion of the incremental algorithm, forming triangles and selecting the one with minimum or maximum area. With option 1, it returns a randomly selected edge from the candidates.

The incremental() function in incremental.cpp implements the incremental algorithm as follows:

    Sorts the points (criterion 1).
    Initializes the polygon and convex hull as the triangle of the first 3 points and ensures that each polygon is counter-clockwise (compatible with convex_hull_2() of CGAL, which always returns in counter-clockwise order).
    For each remaining point, it finds red edges in the convex hull using the is_visible() function and adds them to the red_edges vector. Then, it searches 'behind' these red edges by calling find_vertex for each red edge and finds where these vertices (source and target) lie on the polygonal line. It then searches for visible edges within this range using is_visible and stores them in the visible_edges vector. Finally, it selects an edge to replace based on the second criterion, adds the new point with an insert before the edge's target, calculates the new convex hull, and continues.

Observations and Conclusions:
The initialization, although it may have some differences in the final area (more in images than in uniform with a small sampling), these differences appear quite unpredictable and without a specific pattern. In contrast, the choice of a visible edge consistently produces predictable results. Selecting maximum area at each step gives a large final area, selecting minimum gives a small final area, while random selection tends to be somewhere in between with very rare exceptions, as expected.

Polygonization Based on Convex Hull:

(a) Initially calculates the convex hull and initializes the polygonal line A in the find_convex_hull function. The points of the polygonal line are stored in the chull vector, while the remaining points are stored in the diff vector.

(b) For each edge of A, it finds the nearest internal point in the nearest_point1 function. It calculates the distances between the line segment A and the points in the diff vector (i.e., points not belonging to the polygonal line) and stores them in the mylist vector. It then sorts the vector based on distance to find the nearest point on A. In a vector of structs named nearest, it places the two endpoints of edge A and the area of the triangle formed by these two points and the nearest point on A.

(c) Based on the user-selected criterion, either minimum area, maximum area, or random point is chosen for edge extension in the add_max_area, add_min_area, or add_random functions, respectively. The struct in the nearest vector corresponding to the chosen edge is removed, and two new structs with the new points of the edge are inserted. The points of the nearest vector are then copied to the chull vector, and the nearest vector is cleared.

These steps (a, b, c) are repeated until the diff vector is empty, and all points have moved to the chull vector. Finally, the points are stored in the polygon and printed.

Observations and Conclusions:
Selecting maximum area at each step gives a large final area, selecting minimum gives a small final area, while random selection tends to be somewhere in between with very rare exceptions, as expected. Based on sampling, the algorithm seems to give the largest area while taking the most time compared to the other two algorithms.

Onion:

Consists of files utils_onion.hpp, utils_onion.cpp (with helper functions), and onion.cpp (with the main algorithm).

    Functions from utils_onion.cpp:

    visible(p, edge, polygon): Finds a visible edge, same as incremental→is_visible().
    is_edge_visible: Checks if m or m+1 sees an edge (obviously containing k). Assumes the other point sees k as the closest.
    nearest_point_index: Based on squared_distance (ready-made function), returns the position in p2 of the point closest to the edge we want to delete (k).

    onion.cpp:

Polygon_2 onion(Points S, int onion_initialization):

General logic (briefly explained with comments, actual code includes more detailed comments):

    Significant point is the change in the orientation of the inner polygon (p2).
    Changing the orientation: Ensures that in the main loop (initially), the inner polygon (p2) has a different orientation than it had in the previous iteration. It reverses (using reverse_orientation()) only in even depths (related to starting with depth = 0 but adding the polygon 'depth 1' as stated in the assignment).
    Update Index: After finding the visible edge (visible_edge) in p2, finds the index of the visible_edge.target(). To maintain order, connects the edge1.source() of the outer p1 to visible_edge.target() of p2 (because the orientation in p2 is now reversed).
    index = index % p2.vertices().size();: In the loop for adding new points, ensures that when reaching the last point, the next one is the first, reading the polygon cyclically.
    The rest of the steps are covered in the comments.

Conclusion:
All three algorithms seem to perform well and provide results according to expectations. The onion algorithm, although more complex, produces polygons with larger areas compared to the other two algorithms. The incremental and convex hull-based algorithms offer simplicity and efficiency, with the convex hull-based algorithm providing a good balance between simplicity and area optimization.

Instructions for Running:
To run the program, execute the following commands in the terminal:

cmake -DCGAL_DIR=/usr/lib/CGAL .
make
./main -i input.txt -o output.txt -a <algorithm_type>

Replace <algorithm_type> with one of the following options:

    incremental
    convex_hull
    onion
