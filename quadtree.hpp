#include <vector>
#include "geometry.hpp"


////////////////////////////////////////////////////////////////////////////
template< unsigned int DIM, unsigned int CAPACITY = 4 >                   //
class QuadTreeCell {                                                      //
////////////////////////////////////////////////////////////////////////////
    protected:
    // Rectangle which bounds the entire cell
    Rectangle<DIM> R;

    /* Child cells of this cell. In dimension d, there are 2^d children of
       each cell. */
    QuadTreeCell<DIM>* child[ 1 << DIM ];

    // Points contained in this cell
    unsigned int point_count;
    unsigned int indices[CAPACITY];


    public:


     ///////////////
    // Constructors

    // Construct a quadtree cell with a given bounding box
    QuadTreeCell( Rectangle<DIM>& boundingBox) {
        R = boundingBox;
        for (int index = 0; index < (1<<DIM); index++) {
            child[index] = NULL;
        }

        point_count = 0;
    }

    QuadTreeCell( Point<DIM>& p, double L ) {
        Rectangle<DIM> boundingBox( p, L );
        R = boundingBox;

        for (int index=0; index < (1<<DIM); index++) {
            child[index] = NULL;
        }

        point_count = 0;
    }


     //////////////
    // Destructors

    ~QuadTreeCell() {
        point_count = 0;
        for (int index = 0; index < (1<<DIM); index++) {
            delete child[index];
        }
    }


     ////////////
    // Accessors

    // Get the center point of the cell
    Point<DIM> getCenter() {
        return R.getCenter();
    }

    // Return the width of the cell
    double getHalfWidth(int i) {
        return R.getHalfWidth(i);
    }

    double getHalfWidth() {
        return R.getHalfWidth(0);
    }

    Rectangle<DIM> getBoundingBox() {
        return R;
    }

    void rangeSearch(std::vector<int>& I,
                        std::vector< Point<DIM> >& P, Rectangle<DIM>& T) {
        int i, k;

        // Check all the children
        Rectangle<DIM> S;
        for (k = 0; k < (1<<DIM); k++) {
            if (child[k]) {
                /* Find the intersection of the search rectangle with the
                   child cell's bounding box */
                S = child[k]->getBoundingBox();
                S = S & T;

                /* If the intersection isn't empty, recurse the range
                   search to the child cell */
                if ( ! S.empty() ) {
                    child[k]->rangeSearch(I, P, S);
                }
            }
        }

        Point<DIM> x;
        // Check each of the points stored locally
        for (k = 0; k < point_count; k++) {
            i = indices[k];
            x = P[i];
            if ( T.inRectangle(x) ) {
                I.push_back(i);
            }
        }
    }


     ///////////
    // Mutators

    // Set one of the cell's children
    void setChild(int index, QuadTreeCell<DIM>& cell) {
        child[index] = &cell;
    }

    /* Insert a new point `p` into this cell if it has room or into a child
       if it does not. Note that we do not store the actual point at the
       cell, just an integer `index` that can be used to identify it in a
       list of points later. */
    bool insertPoint(int index, Point<DIM>& p) {
        if ( ! R.inRectangle(p) ) {
            return false;
        }

        /* If there's enough space in this cell's internal storage, store
           it here */
        if ( point_count < CAPACITY ) {
            indices[point_count++] = index;
            return true;
        }

        // Otherwise, find the quadrant to which the point belongs
        Point<DIM> q = getCenter();
        int k = q.quadrant(p);

        // If the corresponding child cell is not empty, recurse to it
        if ( child[k] ) {
            return child[k]->insertPoint(index, p);

        // Otherwise, make a new cell
        } else {
            int t;
            double L = getHalfWidth()/2;

            for (int i = 0; i < DIM; i++) {
                // t == -1 if the i-th bit of index is 1, 0 otherwise
                t = 1 - 2*((k & (1<<i)) != 0);

                /* Choose q[i] = this.center[i] +/- this.width/2 according
                   to which quadrant it belongs to. */
                q[i] = q[i] + t*L;
            }

            // Initialize the appropriate child of this cell
            child[k] = new QuadTreeCell<DIM>(q, L);

            // Insert X[index] into the child cell
            return child[k]->insertPoint(index, p);
        }

        return false;
    }
};



////////////////////////////////////////////////////////////////////////////
template< unsigned int DIM, unsigned int CAPACITY = 4 >                   //
class QuadTree {                                                          //
////////////////////////////////////////////////////////////////////////////
    protected:
    // bounding box for the entire QuadTree
    Rectangle<DIM> R;

    // root cell of the QuadTree
    QuadTreeCell<DIM>* root;

    // number of points contained in the QuadTree
    unsigned int num_points;

    // pointer to the list of Points in the QuadTree
    std::vector< Point<DIM> >* X;


    public:

     ///////////////
    // Constructors

    QuadTree(Rectangle<DIM>& boundingBox, std::vector< Point<DIM> >& P) {
        R = boundingBox;
        root = new QuadTreeCell<2>(boundingBox);

        num_points = P.size();
        X = &P;

        for (int index = 0; index < num_points; index++) {
            root->insertPoint(index, P[index]);
        }
    }


     //////////////
    // Destructors

    ~QuadTree() {
        delete root;
        X = NULL;
    }


     ////////////
    // Accessors

    int getNumPoints() {
        return num_points;
    }

    Point<DIM> getPoint(int k) {
        return (*X)[k];
    }

    void rangeSearch(std::vector<int>& I, Rectangle<DIM>& T) {
        root->rangeSearch(I, *X, T);
    }


     ///////////
    // Mutators

    /*bool insertPoint( Point<DIM>& p ) {
        QuadTreeCell<2>* cell = root;

        return false;
    }*/

};
