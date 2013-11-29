#include "Tsp.h"

void TSP::reset_nodes() {
  for(Node& node : nodes) {
    node.used = false;
  }
}

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

float TSP::compute_total_cost() {
        float total = float(0);
        for(size_t i=0; i<tour.size()-1; ++i)
                total += distance(tour[i],tour[i+1]);

        total += distance(tour[tour.size()-1],tour[0]);
        return total;
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

  float best = float(INT_MAX);
  std::vector<int> best_tour;

  for(int i=0; i<nodes.size(); ++i) {

  #ifdef GREEDY
    greedy(i);
  #endif
  #ifdef GREEDY_TWO_OPT
    greedy(i);
    two_opt();
  #endif

    reset_nodes();

    if(compute_total_cost() < best) {
      best = compute_total_cost();
      std::swap(tour, best_tour);
    }
  }

  std::swap(tour, best_tour);

}

void TSP::greedy(int start) {

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

#ifndef USE_BUCKETS
void TSP::two_opt() {

  float gain;
  float bestGain = float(INT_MAX);
  int best_i, best_j;

  // Continue until no improvements are possible
  while (bestGain != float(0)) {

    bestGain = float(0);

    for (int i = 1; i < nodes.size()-1; i++) {

      int j = i + 2;

      for (; j < nodes.size(); j++) {

        // test
        int a = tour[i  ];
        int b = tour[i+1];
        int c = tour[j  ];
        int d = tour[j+1];

        int t1 = a;
        int t2 = b;
        int t3 = d;
        int t4 = c;


        bool testA = distance(t1,t2) > distance(t2,t3);
        bool testB = distance(t3,t4) > distance(t4,t1);

        if(!testA && !testB)
          continue;

        gain = compute_gain(i, j);

        // Search for the biggest gain
        if (gain < bestGain) {
          bestGain = gain;
          best_i = i;
          best_j = j;
        }
      }
    }

    if (bestGain != float(0)) {
      std::reverse(&tour[best_i + 1], &tour[best_j+1]); // Reverse path
    }
  }
}
#endif

float TSP::compute_gain(int i, int j) {
        int a, b, c, d;
 
        a = tour[i];
        b = tour[i + 1];
        c = tour[j];
        d = tour[j + 1];
 
        return (distance(a,c) + distance(b,d)) - (distance(c,d) + distance(a,b));
}
