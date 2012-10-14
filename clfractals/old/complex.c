//My implementation of a complex # library, easily portable(?) to opencl, for fractals et.al

#include <math.h>
#include "complex.h"

complex set_complex(double re, double im) {
  complex val = {re, im};
  return val;
}

complex add_complex(complex c1, complex c2) {
  complex val = {c1.re+c2.re, c1.im+c2.im};
  return val;
}

complex multiply_complex(complex c1, complex c2) {
  complex val = { ((c1.re * c2.re) - (c1.im * c2.im)), ((c1.re * c2.im) + (c2.re * c1.im)) };
  return val;
}

double abs_complex(complex c1) {
  return sqrt((c1.re * c1.re) + (c1.im *c1.im));
}
