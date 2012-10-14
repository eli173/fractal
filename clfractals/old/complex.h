//header for complex.c

#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct {
  double re;
  double im;
} complex;

//functions

complex set_complex(double re, double im);
complex add_complex(complex c1, complex c2);
complex multiply_complex(complex c1, complex c2); 
double abs_complex(complex c1);
#endif
