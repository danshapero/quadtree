#include <iostream>
#include "geometry.hpp"

using namespace std;

int main() {

    ////////////////////////////////////////////////////////////////////////
    // Tests for points                                                   //
    ////////////////////////////////////////////////////////////////////////

    // Make two points q = (2.0, 2.0),  p = (2.0, 2.0)
    Point<2> p, q;
    q[0] = 2.0;
    q[1] = 2.0;

    p[0] = 2.0;
    p[1] = 2.0;

    // Test that the equality operator works
    if ( !(q==p) ) {
        cout << "Equality comparison operator for points fails, " << endl;
        cout << "reports that two identical points are not equal." << endl;
        return(1);
    }

    // Make p a point with coordinates all less than those of q
    p[0] = 1.0;
    p[1] = 1.0;

    // Check that q correctly reports the quadrant code for p, which
    // should be 3 since every coordinate of p is less than that of q
    int quad = q.quadrant(p);
    if (quad != 3) {
        cout << "Reporting quadrant of one point relative to " << endl;
        cout << "another point failed." << endl;
        cout << "Relative to the point (" << q[0] << "," << q[1] << ")," 
            << endl;
        cout << "The point (" << p[0] << "," << p[1] << ") should" << endl;
        cout << "be in quadrant 3. Quadrant found: " << quad << endl;
        return 1;
    }

    quad = p.quadrant(q);
    if (quad != 0) {
        cout << quad << endl;
        return 1;
    }



    ////////////////////////////////////////////////////////////////////////
    // Tests for rectangles                                               //
    ////////////////////////////////////////////////////////////////////////

    // Make an empty rectangle centered at q
    Rectangle<2> R;

    if ( ! R.empty() ) {
        cout << "Checking whether rectangle is empty failed." << endl;
        cout << "Empty rectangle reported as non-empty." << endl;
        return 1;
    }

    // Make a rectangle centered at q of half-width 4.0
    double L = 4.0;
    R = Rectangle<2>( q, L );

    // Check that the rectangle is correctly reported as non-empty
    if ( R.empty() ) {
        cout << "Checking whether rectangle is empty failed." << endl;
        cout << "Non-empty rectangle reported as empty." << endl;
        return 1;
    }

    // Now make p the center of the rectangle,
    p = R.getCenter();

    // and check that it has been set properly.
    if ( p[0]!=q[0] or p[1]!=q[1] ) {
        cout << "Getting center of rectangle failed." << endl;
        cout << "Rectangle was initialized with center point " << endl;
        cout << "    (" << q[0] << "," << q[1] << ")" << endl;
        cout << "but a query for the center returned the point " << endl;
        cout << "    (" << p[0] << "," << p[1] << ")" << endl;
        return 1;
    }


    // Make p a point contained in the rectangle
    p[0] = 1.0;
    p[1] = 4.0;

    // and check that the rectangle recognizes that it's inside
    if ( ! R.inRectangle(p) ) {
        cout << "Testing for point in rectangle failed." << endl;
        cout << "Point (" << p[0] << "," << p[1] << ") is inside" << endl;
        cout << "rectangle [" << q[0] << " +/- " << L << ", " << q[1] <<
            " +/- " << L << "]" << endl;
        cout << "but was reported as outside rectangle." << endl;
        return 1;
    }

    // Make p a point not contained in the rectangle
    p[0] = 10.0;
    p[1] = 10.0;

    // and check that the rectangle recognizes that it's outside
    if ( R.inRectangle(p) ) {
        cout << "Testing for point in rectangle failed." << endl;
        cout << "Point (" << p[0] << "," << p[1] << ") is not inside" << endl;
        cout << "rectangle [" << q[0] << " +/- " << L << ", " << q[1] <<
            " +/- " << L << "]" << endl;
        cout << "but was reported as inside rectangle." << endl;
        return 1;
    }

    // Make a rectangle S which is equal to R and check that they're equal
    Rectangle<2> S(q, L);

    if ( R!=S ) {
        cout << "Comparison of rectangles failed; two identical " << endl;
        cout << "rectangles reported as not equal." << endl;
        return 1;
    }

    // Make a rectangle S around p with half-width 1
    S = Rectangle<2>(p, 1.0);

    // Check that the intersection of R and S is empty
    Rectangle<2> T = R & S;

    if ( ! T.empty() ) {
        cout << "Rectangle intersection failed. Intersection of " << endl;
        cout << "two disjoint rectangles returned a non-empty " << endl;
        cout << "rectangle." << endl;
        return 1;
    }

    /* Make a new triangle S around p with half-width 5 and check that it
       has the right intersection with R */
    S = Rectangle<2>(p, 5.0);
    T = R & S;

    Point<2> z;
    z[0] = 5.5;
    z[1] = 5.5;
    Rectangle<2> TC(z, 0.5);

    if ( T != TC ) {
        cout << "Rectangle intersection computed wrong result." << endl;
        return 1;
    }


    cout << "All geometry tests completed!" << endl;

    return 0;
}
