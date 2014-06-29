.SECONDARY:
.PHONY: clean

CC = clang
CXX = clang++

%.o: %.cxx
	$(CXX) -c $< $(CXXFLAGS)

geometry_test: geometry_test.o geometry.hpp
	$(CXX) -o geometry_test geometry_test.o

quadtree_test: quadtree_test.o quadtree.hpp geometry.hpp
	$(CXX) -o quadtree_test quadtree_test.o

clean:
	rm -rf *.o *_test *~

