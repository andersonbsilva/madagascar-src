/* Signal component separation using plane-wave destruction. 

The program works with 2-D data.
*/
/*
  Copyright (C) 2004 University of Texas at Austin
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <rsf.h>
    
#include "copyk.h"
#include "predk.h"

int main(int argc, char* argv[])
{
    int i, n1, n2, n12, n3, n123, niter;
    float eps, *d, *s, ***pp;
    bool verb;
    sf_file in, out, dips;

    sf_init (argc,argv);
    in = sf_input("in");
    dips = sf_input("dips");
    out = sf_output("out");

    if (!sf_histint(in,"n1",&n1)) sf_error("No n1= in input");
    if (!sf_histint(in,"n2",&n2)) sf_error("No n2= in input");

    if (!sf_histint(dips,"n1",&i) || i != n1) sf_error("Wrong n1= in dips");
    if (!sf_histint(dips,"n2",&i) || i != n2) sf_error("Wrong n2= in dips");
    if (!sf_histint(dips,"n3",&n3)) sf_error("No n3= in dips");

    n12 = n1*n2;
    n123 = n12*n3;
    sf_putint (out,"n3",n3);

    if (!sf_getint ("niter",&niter)) niter=50;
    /* maximum number of iterations */

    if (!sf_getfloat ("eps",&eps)) eps=0.;
    /* regularization parameter */

    if (!sf_getbool("verb",&verb)) verb = false;
    /* verbosity flag */

    s = sf_floatalloc(n123);
    d = sf_floatalloc(n12);
    pp = sf_floatalloc3(n1,n2,n3);

    sf_floatread (d,n12,in);
    sf_floatread (pp[0][0],n123,dips);
    
    predk_init(n3,n1,n2,0.0001,pp);
    copyk_init(n3,n12);

    sf_solver_prec (copyk_lop,sf_cgstep,predk_lop,
		    n123,n123,n12,s,d,niter,eps,"verb",verb,"end");

    sf_floatwrite(s,n123,out);

    exit(0);
}
