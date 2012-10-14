
unsigned char iterate(double x, double y
	     )
{
  double2 point = (double2)(x,y);
  double2 value = (double2)(x,y);
  for(int i=0;i<127;i++) {
    value = (double2)((value.x*value.x-value.y*value.y),(2*value.x*value.y))+point;
    if (length((float2)(value.x,value.y))>2) {
      return i;
    }
  }
  return 255;
}

double2 px_to_vec(int x, int y,
		 double xmin, double ymin,
		 double x_per_px,
		 double y_per_px
		 )
{
  return (double2)((xmin+x_per_px*x),(ymin+y_per_px*y));
}

int from_id_x(int idno, int x) {
  return idno%x;
}

int from_id_y(int idno, int x) {
  return idno/x;
}

__kernel void Mandelbrot(__global unsigned char* mandelbrot_set,
			 const int x, const int y,
			 const double xmin, const double xmax,
			 const double ymin, const double ymax
			 )
{
  const int idno = get_global_id(0);
  if (idno<(x*y)) {
    double x_per_px = ((xmax-xmin)/x);
    double y_per_px = ((ymax-ymin)/y);
    mandelbrot_set[idno] = iterate( (px_to_vec(from_id_x(idno,x),from_id_y(idno,x),xmin,ymin,x_per_px,y_per_px)).x, (px_to_vec(from_id_x(idno,x),from_id_y(idno,x),xmin,ymin,x_per_px,y_per_px)).y);
  }
}
/**/
//
