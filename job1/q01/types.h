#ifndef TIPOS
#define TIPOS

// Define a point struct to hold 2 integers
typedef struct {
	double x;
	double y;
} point;

// Define a quadrilateral struct to hold data input
typedef struct {
	point a;
	point b;
	point c;
  point d;
} quadrilateral;

typedef struct {
	point a;
	point b;
	point c;
} triangle;

#endif
