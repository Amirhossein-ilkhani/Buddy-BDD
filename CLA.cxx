/**
  FILE:  CLA.cc
  DESCR: BDD implementation of an N bit CLA adder
  AUTH:  Amirhossein ilkhani
  DATE:  2023
*************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <iostream>
#include "bdd.h"
using namespace std;

int N;

bdd *ainp;
bdd *binp;
bdd *c;
bdd *s;
bdd *p;
bdd *g;


/*************************************************************************
  Adder
*************************************************************************/


void build_adder(void)
{
   int n;
   for (n=0 ; n<N ; n++){
  	 p[n] = ainp[n] ^ binp[n];
     	 g[n] = ainp[n] & binp[n];
   }


   for (n=0 ; n<N ; n++) 
	c[n+1] = g[n]|(p[n]&c[n]);

   for (n=0 ; n<N ; n++)
	s[n] = p[n] ^ c[n];

}




int main(int argc, char **argv)
{
   using namespace std ;
   int method=BDD_REORDER_NONE;
   int n;
   
   if(argc < 2 || argc > 3)
   {
      cout << "usage: adder N R\n";
      cout << "       N number of bits\n";
      cout << "       R -> enable reordering if R is win2,win2ite,win3,win3ite,sift,siftite\n";
      cout << "            in this case 'adder' starts with a worst case ordering\n";
      exit(1);
   }
   
   N = atoi(argv[1]);
   if (N <= 0)
   {
      cout << "The number of bits must be more than zero\n";
      exit(2);
   }

   if (argc == 3)
   {
      if (strcmp(argv[2], "win2") == 0)
	 method = BDD_REORDER_WIN2;
      else
      if (strcmp(argv[2], "win2ite") == 0)
	 method = BDD_REORDER_WIN2ITE;
      else
      if (strcmp(argv[2], "win3") == 0)
	 method = BDD_REORDER_WIN3;
      else
      if (strcmp(argv[2], "win3ite") == 0)
	 method = BDD_REORDER_WIN3ITE;
      else
      if (strcmp(argv[2], "sift") == 0)
	 method = BDD_REORDER_SIFT;
      else
      if (strcmp(argv[2], "siftite") == 0)
	 method = BDD_REORDER_SIFTITE;
      else
      if (strcmp(argv[2], "rand") == 0)
	 method = BDD_REORDER_RANDOM;
   }
   
   bdd_init(500,1000);
   bdd_setvarnum(2*N);
      
   ainp = new bdd[N];
   binp = new bdd[N];
   c = new bdd[N+1];
   s = new bdd[N];
   p = new bdd[N];
   g = new bdd[N];
   
   for (n=0 ; n<N ; n++)
   {
      if (method == BDD_REORDER_NONE)
      {
	 ainp[n] = bdd_ithvar(2*n);
	 binp[n] = bdd_ithvar(2*n+1);
      }
      else
      {
	 ainp[n] = bdd_ithvar(n);
	 binp[n] = bdd_ithvar(N+n);
      }
   }

   for (n=0 ; n<N ; n++)
   {
      bdd_addvarblock(ainp[n],1);
      bdd_addvarblock(binp[n],1);
   }

   //bdd_autoreorder(method);
   //bdd_reorder_verbose(2);
   
   std::clock_t start = std::clock();

   build_adder();
   
   std::clock_t end = std::clock();

   long double time_elapsed = 1000.0 * (end - start) / CLOCKS_PER_SEC;


   cout << "Time elapsed by function is :  " << time_elapsed << " mil_sec" << endl;
   //-----------------------
   
   if (method != BDD_REORDER_NONE)
   {
      cout << "Sizes before reordering:\n";
      for (n=0 ; n<N+1 ; n++)
	 cout << "c[" << n << "]: " << bdd_nodecount(c[n]) << " nodes\n";

      bdd_reorder(method);
      cout << "Sizes after reordering:\n";
   }
   else
      cout << "Sizes:\n";

   for (n=0 ; n<N+1 ; n++)
      cout << "c[" << n << "]: " << bdd_nodecount(c[n]) << " nodes\n";

        FILE *fid;
	    fid = fopen ("CLA.dot", "w");
            bdd_fprintdot(fid, c[N]);
	    fclose(fid); 

}



/*=== DEBUGING =========================================================*/
/*
bdd setval(int val, int v)
{
   bdd x = bddtrue;
   
   for (int n=0 ; n<N ; n++)
   {
      if (val & 1)
      {
	 if (v == 0)
	    x &= ainp[n];
	 else
	    x &= binp[n];
      }
      else
      {
	 if (v == 0)
	    x &= !ainp[n];
	 else
	    x &= !binp[n];
      }

      val = val >> 1;
   }

   return x;
}


int test_vector(bdd av, bdd bv, int a, int b)
{
   int res = a+b;

   for (int n=0 ; n<N ; n++)
   {
      bdd resv = av & bv & xout[n];
      
      if (resv == bddfalse && (res & 1) || resv != bddfalse && !(res & 1))
	 return 0;

      res = res >> 1;
   }

   return 1;
}


int test_adder(void)
{
   int m = 1 << N;
   
   for (int a=0 ; a<m ; a++)
   {
      for (int b=0 ; b<m ; b++)
      {
	 bdd av = setval(a,0);
	 bdd bv = setval(b,1);

	 if (test_vector(av,bv,a,b) == 0)
	    return 0;
      }
   }

   return 1;
}
*/

/* EOF */
