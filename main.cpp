#include "Tsp.h"
#ifdef OPEN_GL
  #include "visualizer.h"
#endif

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

  // print a la Kattis
  int counter=0;
  int last = tsp.get_tour().size()-1;
  for(int node : tsp.get_tour()) {
    if(counter == last)
      break;
    printf("%d\n",node);
    counter++;
  }

  // We are debugging. Let's visualize it
  #ifndef KATTIS
  printf("Total cost: %f\n", tsp.compute_total_cost());
  use_opengl(argc, argv, &tsp);
  system("pause");
  #endif

  // Are we on VS
  #ifdef VISUAL_STUDIO
    system("pause");
  #endif
  

  return 0;
}