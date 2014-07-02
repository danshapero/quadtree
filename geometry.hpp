/* This file defines point and rectangle objects */

#include <algorithm>

////////////////////////////////////////////////////////////////////////////
template<unsigned int DIM> class Point {                                  //
////////////////////////////////////////////////////////////////////////////
    double coords[DIM];

    public:

    // Default constructors
    Point() {
        for (int i=0; i < DIM; i++) {
            coords[i] = 0.0;
        }
    }

    // Copy constructor
    Point(const Point& p) {
        for (int i=0; i < DIM; i++) {
            coords[i] = p[i];
        }
    }

    // Overload the array access operator
    inline double& operator[] (int i) {
        return coords[i];
    }

    inline const double& operator[] (int i) const {
        return coords[i];
    }

    /* Given an input point p, returns an int specifying which quadrant
       p is in relative to the this point. Bit #k of the returned int is 1 
       if p is below this point along dimension k. */
    inline unsigned int quadrant( const Point<DIM>& p ) {
        unsigned int quad = 0;
        for (int i = 0; i < DIM; i++) {
            quad = quad + ((p[i] <= coords[i]) ? (1<<i) : 0);
        }
        return quad;
    }

};


// Overloaded comparison operators for point data type
template<unsigned int DIM>
inline bool operator== (const Point<DIM>& p, const Point<DIM>& q) {
    bool ret = true;
    for (int i = 0; i < DIM; i++) {
        ret = ret and (p[i] == q[i]);
    }
    return ret;
}


template<unsigned int DIM>
inline bool operator!= (const Point<DIM>& p, const Point<DIM>& q) {
    return !(p==q);
}




////////////////////////////////////////////////////////////////////////////
template<unsigned int DIM> class Rectangle {                              //
////////////////////////////////////////////////////////////////////////////
    Point<DIM> x;
    double L[DIM];

    public:

     ///////////////
    // Constructors

    Rectangle() {
        for (int i = 0; i < DIM; i++) {
            L[i] = 0.0;
            x[i] = 0.0;
        }
    }

    Rectangle( const Point<DIM>& center, double half_width ) {
        for (int i = 0; i < DIM; i++) {
            x[i] = center[i];
            L[i] = half_width;
        }
    }

    Rectangle( const Point<DIM>& center, double half_widths[DIM]) {
        for (int i = 0; i < DIM; i++) {
            x[i] = center[i];
            L[i] = half_widths[i];
        }
    }


     ////////////
    // Accessors

    // Returns the center of the rectangle
    Point<DIM> getCenter() {
        return x;
    }

    // Return the half-width along axis i of the rectangle
    double getHalfWidth(int i) {
        return L[i];
    }

    // Returns true if all the half-widths are > 0
    bool empty() {
        for (int i=0; i < DIM; i++) {
            if (L[i]==0) {
                return true;
            }
        }
        return false;
    }

    // Returns true if a point is inside the rectangle
    /* NOTE: For our purposes, we're considering half-closed rectangles.
       If a point is on the boundary of a rectangle, then it is considered
       to be inside the rectangle if it's on the upper boundary but not
       inside the rectangle if it's on the lower boundary. This is to avoid
       annoying edge cases because computational geometry is hard. */
    inline bool inRectangle( Point<DIM>& p ) {
        int i;
        bool ret = true;
        for (i=0; i < DIM; i++) {
            ret = ret and (p[i] <= x[i]+L[i]) and (p[i] > x[i]-L[i]);
        }
        return ret;
    }

};


// Overload the & operator for rectangles to return the intersection
template<unsigned int DIM>
Rectangle<DIM> operator& ( Rectangle<DIM>& R1, Rectangle<DIM>& R2 ) {
    int i;
    double A, B, z1, z2, L[DIM];

    Point<DIM> p1 = R1.getCenter();
    Point<DIM> p2 = R2.getCenter();
    Point<DIM> q;

    for (i = 0; i < DIM; i++) {
        z1 = p1[i]-R1.getHalfWidth(i);
        z2 = p2[i]-R2.getHalfWidth(i);
        A = std::max( z1, z2 );

        z1 = p1[i]+R1.getHalfWidth(i);
        z2 = p2[i]+R2.getHalfWidth(i);
        B = std::min( z1, z2 );

        q[i] = (A+B)/2;
        L[i] = std::max( (B-A)/2, 0.0 );
    }

    Rectangle<DIM> S(q, L);
    return S;
}


// Overloaded comparison operators for rectangle data type
template<unsigned int DIM>
bool operator== ( Rectangle<DIM>& R1, Rectangle<DIM>& R2) {
    if ( R1.getCenter() != R2.getCenter() ) {
        return false;
    }

    for (int i = 0; i < DIM; i++) {
        if (R1.getHalfWidth(i) != R2.getHalfWidth(i)) {
            return false;
        }
    }

    return true;
}


template<unsigned int DIM>
bool operator!= ( Rectangle<DIM>& R1, Rectangle<DIM>& R2) {
    return !(R1 == R2);
}
