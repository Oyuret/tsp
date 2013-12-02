#include "Tsp.h"

float TSP::distance(const Node& a, const Node& b) {
  float x_diff = a.x - b.x;
  float y_diff = a.y - b.y;

  return sqrt(pow(x_diff, 2) + pow(y_diff, 2));
}

float TSP::distance(int a, int b) {
#ifdef USE_PRECOMPILED_DISTANCE_MATRIX
  return distances[a][b];
#else
  return distance(nodes[a],nodes[b]);
#endif
}

void TSP::init_distances() {
  distances.resize(nodes.size());
  for(size_t i=0; i<nodes.size(); ++i) {
    distances[i].resize(nodes.size());
    for(size_t j=0; j<nodes.size(); ++j) {
      distances[i][j] = distance(nodes[i],nodes[j]);
    }
  }
}

void TSP::init_buckets() {
  // DEM BUCKETS!
}

void TSP::solve() {

  #ifdef GREEDY
  greedy(0);
  #endif

}

void TSP::greedy(int start){

  tour.resize(nodes.size()+1);

  // use node 0
  nodes[start].used = true;
  tour[0] = start;

  // For every node, choose the closest one as the best next node.
  for(int i=1; i<nodes.size(); ++i) {
    int bestIndex = -1;
    float bestDist = float(INT_MAX);

    // We are using buckets
    #ifdef USE_BUCKETS
    //TODO USE BUCKETS
    #endif

    // We are not using buckets
    for(size_t j=0; j<nodes.size(); ++j) {
      if(nodes[j].used==false) {

        float testDist = distance(tour[i-1],j);
        if(testDist < bestDist) {
          bestIndex = j;
          bestDist = testDist;
        }

      }
    }

    nodes[bestIndex].used = true;
    tour[ i ] = bestIndex;
  }

  tour.back() = start;
}