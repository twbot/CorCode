 
 #include <stdint.h>

 #include "Magnometer.h"

Magnometer::GetAxis(int buffer){
	int buffer = (buffer << 2 | buffer)
	x.GetAxis();
	y.GetAxis();
	z.GetAxis();
}

