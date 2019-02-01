/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 

#include "math.h" 
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>


static int within(float p, float q, float r)
{
    return (p <= q && q <= r) || (r <= q && q <= p);
}


int line_intercepts_circle(float cx, float cy, float r, float x0, float y0, float x1, float y1, float* rx, float* ry)
{
	float del, m, d, ix0, ix1, iy0, iy1, a, b, c, swap;

	/* insert pertubation for handle inf.. in this case, will not get problems */
	if(fabsf(x1 - x0) < 1.0)
	{
		if(x0 < x1)
		{	
			x0 -= 1.01 - fabsf(x1 - x0);
		}
		else
		{
			x1 -= 1.01 - fabsf(x0 - x1);
		}
	}

	/* line equation */
	m = (y1 - y0) / (x1 - x0);
	d = - (x1 * (y1 - y0)) / (x1 - x0) + y1;

	/* circle equation */
	a = -2*cx;
	b = -2*cy;
	c = cx*cx + cy*cy - r*r;


	del = (2*m*d + a + b*m)*(2*m*d + a + b*m) - 4*(1+m*m)*(d*d + b*d + c);

	if(del < 0)
		return 0;

	ix0 = (-(2*m*d + a + b*m) + sqrtf(del)) / (2*(1 + m*m));
	ix1 = (-(2*m*d + a + b*m) - sqrtf(del)) / (2*(1 + m*m));

	del = (a*m - 2*d + m*m*b)*(a*m - 2*d + m*m*b) - 4*(1+m*m)*(d*d - a*m*d + m*m*c);

	if(del < 0)
		return 0;

	iy0 = (2*d - a*m - b*m*m + sqrtf(del)) / (2*(1 + m*m));
	iy1 = (2*d - a*m - b*m*m - sqrtf(del)) / (2*(1 + m*m));


	if(m < 0)
	{
		swap = iy1;
		iy1 = iy0;
		iy0 = swap;
	}

	if(pythagorean_c(ix0 - x0, iy0 - y0) < pythagorean_c(ix1 - x0, iy1 - y0))
	{
		*rx = ix0;
		*ry = iy0;
	}
	else
	{
		*rx = ix1;
		*ry = iy1;	
	}


	if(x0 != x1)
	{
		if(!within(x0, *rx, x1))
			return 0;
	}
	else if(!within(y0, *ry, y1))
	{
		return 0;
	}


	return 1;
}


int point_inside_circle(float cx, float cy, float r, float x, float y)
{
	return pythagorean_c(x - cx, y - cy) <= r;
}

