/*************************************************************************
  FILE:  MUX.cc
  DESCR: BDD of 3stage mux.
  AUTH:  Amirhossein ilkhani
  DATE:  2023
*************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <iostream>
#include "bdd.h"
using namespace std;
using namespace std::chrono;

int N;

bdd *ainp;
bdd *binp;
bdd *o;


/*************************************************************************
  Adder
*************************************************************************/

bdd mux(bdd a, bdd b, bdd s){
	return (!s&a)|(s&b);
}


void build_mux(void)
{
   o[0] = mux(ainp[0],ainp[1],binp[0]);
   o[1] = mux(ainp[2],ainp[3],binp[0]);
   o[2] = mux(ainp[4],ainp[5],binp[0]);
   o[3] = mux(ainp[6],ainp[7],binp[0]);

   o[4] = mux(o[0],o[1],binp[1]);
   o[5] = mux(o[2],o[3],binp[1]);

   o[6] = mux(o[4],o[5],binp[2]);

}




int main(int argc, char **argv)
{
   using namespace std ;
   int method=BDD_REORDER_NONE;
   int n;
   
   if(argc < 1 || argc > 2)
   {
      cout << "usage: adder N R\n";
      cout << "       R -> enable reordering if R is win2,win2ite,win3,win3ite,sift,siftite\n";
      cout << "            in this case 'adder' starts with a worst case ordering\n";
      exit(1);
   }
   

   if (argc == 2)
   {
      if (strcmp(argv[1], "win2") == 0)
	 method = BDD_REORDER_WIN2;
      else
      if (strcmp(argv[1], "win2ite") == 0)
	 method = BDD_REORDER_WIN2ITE;
      else
      if (strcmp(argv[1], "win3") == 0)
	 method = BDD_REORDER_WIN3;
      else
      if (strcmp(argv[1], "win3ite") == 0)
	 method = BDD_REORDER_WIN3ITE;
      else
      if (strcmp(argv[1], "sift") == 0)
	 method = BDD_REORDER_SIFT;
      else
      if (strcmp(argv[1], "siftite") == 0)
	 method = BDD_REORDER_SIFTITE;
      else
      if (strcmp(argv[1], "rand") == 0)
	 method = BDD_REORDER_RANDOM;
   }
   
   bdd_init(500,1000);
   bdd_setvarnum(11);
      
   ainp = new bdd[8];
   binp = new bdd[3];
   o = new bdd[7];

   for(n=0; n<3; n++)
      binp[n] = bdd_ithvar(n);   

   for (n=3 ; n<11 ; n++)
   {
     ainp[n-3] = bdd_ithvar(n);
   }	 

   for(n=0 ; n<8 ; n++)
   {
      bdd_addvarblock(ainp[n],1);
   }
   
   for(n=0; n<3; n++){
      bdd_addvarblock(binp[n],1);
   }
  
   build_mux();
  
   //-----------------------
   
   if (method != BDD_REORDER_NONE)
   {
      cout << "Sizes before reordering:\n";
      cout << "out: " << bdd_nodecount(o[6]) << " nodes\n";
      bdd_reorder(method);
      cout << "Sizes after reordering:\n";
   }
   else
      cout << "Sizes:\n";
      cout << "out: " << bdd_nodecount(o[6]) << " nodes\n";

        FILE *fid;
	    fid = fopen ("mux.dot", "w");
            bdd_fprintdot(fid, o[6]);
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
