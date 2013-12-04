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

#ifdef USE_PRECOMPILED_DISTANCE_MATRIX
void TSP::init_distances() {
  distances.resize(nodes.size());
  for(size_t i=0; i<nodes.size(); ++i) {
    distances[i].resize(nodes.size());
    for(size_t j=0; j<nodes.size(); ++j) {
      distances[i][j] = distance(nodes[i],nodes[j]);
    }
  }
}
#endif

#ifdef USE_BUCKETS
void TSP::init_buckets() {
  std::vector<int> tmpList;

  for(int j=0; j< nodes.size(); ++j) {
      tmpList.push_back(j);
  }
  // for each node copy the contents from distance to each other node.
  for(int i=0; i< nodes.size(); ++i) {

    // order that list
    std::sort(tmpList.begin(), tmpList.end(), [=](int a, int b){return distance(i,a)<distance(i,b);});

    // put the NEIGHBOURHOOD_LIMIT ones to it's neighbourhood
    for(int j=1; j<tmpList.size() && j<NEIGHBOURHOOD_LIMIT; ++j) {
      nodes[i].neighbourhood.push_back(tmpList[j]);
    }

  }
}
#endif

void TSP::solve() {

  float best = float(INT_MAX);
  std::vector<int> best_tour(nodes.size()+1);

  for(int i=0; i<nodes.size(); ++i) {

#ifdef GREEDY
    greedy(i);
#endif
#ifdef GREEDY_TWO_OPT
    greedy(i);
    two_opt();
#endif

    reset_nodes();
    
    float totalcost = compute_total_cost();
    if(totalcost < best) {
      best = totalcost;
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
    bool found=false;
    for(int neighbour : nodes[tour[i-1]].neighbourhood) {

      if(nodes[neighbour].used == false) {
        tour[i]=neighbour;
        nodes[neighbour].used=true;
        found=true;
        break;
      }
    }
    if(found) continue;
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
      int max = 0;

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

float TSP::compute_gain(int a, int b, int c, int d) {
  return (distance(a,c) + distance(b,d)) - (distance(c,d) + distance(a,b));
}

#ifdef USE_BUCKETS
void TSP::two_opt() {
  float gain;
  float bestGain = float(INT_MAX);
  int best_i, best_j;

  while(bestGain != float(0)) {
    bestGain = float(0);

    // testA
    for(int i=1; i<nodes.size(); ++i) {

      int a = tour[i];
      auto aIter = std::find(tour.begin(), tour.end(), a);
      int b = tour[i+1];

      for(int d : nodes[b].neighbourhood) {

        if(distance(b,a) > distance(b,d)) {
          auto dIter = std::find(tour.begin(), tour.end(), d);

          if(dIter == tour.begin()) continue;
            
          dIter--;
          int c = *dIter;

          if(dIter < aIter) continue;

          gain = compute_gain(a,b,c,d);

          if (gain < bestGain) {
            bestGain = gain;
            best_i = a;
            best_j = c;
            break;
          }
          
        } else {
          break;
        }
      }
    }


    // testB
    for(int i = 1; i < nodes.size(); i++) {
      
      
      int c = tour[i  ];
      auto cIter = std::find(tour.begin(), tour.end(), c);
      int d = tour[i+1];

      for(int a : nodes[c].neighbourhood) {

        if(distance(c,d) > distance(c,a)) {

          auto aIter = std::find(tour.begin(), tour.end(), a);
          auto last = tour.end();
          last--;

          if(aIter == last) continue;
          
          auto bIter = aIter+1;
          int b = *bIter;

          if(cIter < aIter) continue;

          gain = compute_gain(a,b,c,d);

          if (gain < bestGain) {
             bestGain = gain;
             best_i = a;
             best_j = c;
             break;
          }
          
        } else {
          break;
        }
      }
    }

    if(bestGain != float(0)) {

      auto best_iIter = std::find(tour.begin(), tour.end(), best_i);
      auto best_jIter = std::find(tour.begin(), tour.end(), best_j);

      best_iIter++;
      best_jIter++;

      std::reverse(best_iIter, best_jIter); // Reverse path
    }
  }
}
#endif
