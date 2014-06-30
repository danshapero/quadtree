#include <iostream>
#include <vector>

#include "quadtree.hpp"

using namespace std;

int main() {

    Point<2> p;
    p[0] = 0.0;
    p[1] = 0.0;


    ////////////////////////////////////////////////////////////////////////
    // Tests for quadtree cells                                           //
    ////////////////////////////////////////////////////////////////////////

    // Make a QuadTreeCell centered at 0 with half-width 1.0
    QuadTreeCell<2>* cell;
    cell = new QuadTreeCell<2>(p, 1.0);

    // Check that the half-width and cell center were set properly
    double L = cell->getHalfWidth();
    if ( L != 1.0 ) {
        cout << "Setting or getting quad tree cell half-width " << endl;
        cout << "failed. Should return 1.0, result found: " << L << endl;
        return 1;
    }

    Point<2> q = cell->getCenter();
    if ( q!=p ) {
        cout << "Setting or getting quad tree cell center failed." << endl;
        cout << "Should return the point (" << p[0] << ", " << p[1] 
                << ")" << endl;
        cout << "Point found: (" << q[0] << ", " << q[1] << ")." << endl;
    }

    delete cell;



    ////////////////////////////////////////////////////////////////////////
    // Tests for quadtrees                                                //
    ////////////////////////////////////////////////////////////////////////

    p[0] = 0.0;
    p[1] = 0.0;
    L = 10.0;
    Rectangle<2> R(p, L);

    vector< Point<2> > X;

    for (int i = 0; i < 100; i++) {
        q[0] = 20.0 * double(rand()) / RAND_MAX - 10.0;
        q[1] = 20.0 * double(rand()) / RAND_MAX - 10.0;

        X.push_back(q);
    }

    QuadTree<2> QT(R, X);

    vector<int> I;

    QT.rangeSearch(I, R);

    cout << I.size() << endl;
    


    cout << "Done with tests! Your quadtree works!" << endl;

    return 0;
}
