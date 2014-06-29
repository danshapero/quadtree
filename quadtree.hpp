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
        root = NULL;

        num_points = P.size();
        X = &P;
    }


     ////////////
    // Accessors

    int getNumPoints() {
        return num_points;
    }

    Point<DIM> getPoint(int k) {
        return (*X)[k];
    }
};
