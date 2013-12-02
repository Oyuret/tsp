#include "Tsp.h"

int main ( int argc, char** argv ) {

  // initialize the tsp
  TSP tsp;


  int nodeCount;
  scanf("%d",&nodeCount);
  tsp.get_nodes().resize(nodeCount);
 
  for(Node& node : tsp.get_nodes()) {
    scanf("%f%f", &node.x, &node.y);
  }
  
  #ifdef USE_BUCKETS
  tsp.init_buckets();
  #endif

  #ifdef USE_PRECOMPILED_DISTANCE_MATRIX
  tsp.init_distances();
  #endif
  
  tsp.solve();

  int counter=0;
  int last = tsp.get_tour().size()-1;
  for(int node : tsp.get_tour()) {
    if(counter == last)
      break;
    printf("%d\n",node);
    counter++;
  }

  #ifndef KATTIS
  system("pause");
  #endif
  

  return 0;
}