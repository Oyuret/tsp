#include "Tsp.h"

void TSP::reset_nodes() {
  for(Node& node : nodes) {
    node.used = false;
    node.tour_index=0;
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

  greedy(0);
  
  two_opt();

  std::swap(tour,best_tour);
  reset_nodes();


  // We got less than 50 nodes. Test them all
  if(nodes.size() <= 275) {

    for(int i=0; i<nodes.size(); ++i) {

    reset_nodes();
#ifdef GREEDY
      greedy(i);
#endif
#ifdef GREEDY_TWO_OPT
      greedy(i);
      two_opt();

#endif
#ifdef GREEDY_TWO_POINT_FIVE_OPT
      greedy(i);
     twoFive_opt();

#endif

      float totalcost = compute_total_cost();
      if(totalcost < best) {
        best = totalcost;
        std::swap(tour, best_tour);
      }

      
    }

  }
  else {

    for(int i=0; i<nodes.size(); i+=nodes.size()/22) {

    reset_nodes();

#ifdef GREEDY
      greedy(i);
#endif
#ifdef GREEDY_TWO_OPT
      greedy(i);
      two_opt();
#endif
#ifdef GREEDY_TWO_POINT_FIVE_OPT
      greedy(i);
      twoFive_opt();
#endif

      float totalcost = compute_total_cost();
      if(totalcost < best) {
        best = totalcost;
        std::swap(tour, best_tour);
      }

      
    }

  }



#ifdef SHUFFLING
  for(int i=0; i< tour.size()-1; ++i) {
    nodes[tour[i]].tour_index = i;
  }

  for(int j=0; j<MAX_SHUFFLES; ++j) {
    shuffle(2);
    two_opt();
    float totalcost = compute_total_cost();
    if(totalcost < best) {
      best = totalcost;
      std::swap(tour, best_tour);
      for(int i=0; i< tour.size()-1; ++i) {
        nodes[tour[i]].tour_index = i;
      }
    }
  }
#endif

  std::swap(tour, best_tour);
}

void TSP::twoFive_opt(){
 bool improvement = true;

float curr;
float opt;
int max = nodes.size()-1;

int node2Index;
int node3Index;
int node4Index;

int n;
int i;
 while(improvement){

  improvement=false;
  for(i =0;i<max;i++){
   Node &node1 = nodes[tour[i]];
   Node &node2 = nodes[tour[i+1]];
   node2Index=tour[i+1];
    for(int neighbour : nodes[tour[i]].neighbourhood) {
     if(neighbour < i+1 )break;
     Node &node3 = nodes[neighbour];
     node3Index = node3.tour_index;
     Node &node4 = nodes[tour[node3Index+1]];
     node4Index=node4.tour_index;
     
     Node &node5 = nodes[tour[i+2]];

     if(node2.tour_index == node3Index || node2.tour_index == node4Index || node3Index==node4Index) break;
           
     curr = distance(tour[i],tour[i+1])+distance(neighbour, tour[node3Index+1])+distance(tour[i+1], tour[i+2]);
    opt= distance(tour[node3Index+1],tour[i+1])+distance(neighbour,tour[i+1])+distance(tour[i],tour[i+2]);
         
     if(opt < curr){
    if(node2.tour_index < node3.tour_index){
       for(n=node2.tour_index;n<node3Index;n++){
        tour[n]=tour[n+1];
        nodes[tour[n]].tour_index=n;
       }
       tour[node3Index]=node2Index;
       nodes[node2Index].tour_index=node3Index;

 }else{
    for(n=node2.tour_index;n>node4Index;n--){
       tour[n]=tour[n-1];
       nodes[tour[n]].tour_index=n;
      }
      tour[node4Index]=node2Index;
      nodes[node2Index].tour_index=node4Index; 
//printf("%i %i \n",tour[node4Index],nodes[node2Index].tour_index);
     }
     improvement = true;
     break;
    }
   }
   if(improvement){
    break;
   }
  }
 }
}

void TSP::greedy(int start) {

  tour.resize(nodes.size()+1);

  // use node 0
  nodes[start].used = true;
  tour[0] = start;
  int bestIndex;
  float bestDist;
  float testDist;

  // For every node, choose the closest one as the best next node.
  for(int i=1; i<nodes.size(); ++i) {
    bestIndex = -1;
    bestDist = float(INT_MAX);

    // We are using buckets
#ifdef USE_BUCKETS
    bool found=false;
    for(int neighbour : nodes[tour[i-1]].neighbourhood) {

      if(nodes[neighbour].used == false) {
        tour[i]=neighbour;
        nodes[neighbour].used=true;
        nodes[neighbour].tour_index = i;
        found=true;
        break;
      }
    }
    if(found) continue;
#endif

    // We are not using buckets
    for(size_t j=0; j<nodes.size(); ++j) {
      if(nodes[j].used==false) {

        testDist = distance(tour[i-1],j);
        if(testDist < bestDist) {
          bestIndex = j;
          bestDist = testDist;
        }

      }
    }

    nodes[bestIndex].used = true;
    nodes[bestIndex].tour_index = i;
    tour[ i ] = bestIndex;
  }

  tour.back() = start;
}

#ifndef USE_BUCKETS
void TSP::two_opt() {

  float gain;
  float bestGain = float(INT_MAX);
  int best_i, best_j;


  int a;
  int b;
  int c;
  int d;

  int t1;
  int t2;
  int t3;
  int t4;

  bool testA;
  bool testB;

  int j;
  int max;

  // Continue until no improvements are possible
  while (bestGain != float(0)) {

    bestGain = float(0);

    for (int i = 1; i < nodes.size()-1; i++) {

      j = i + 2;
      max = 0;

      for (; j < nodes.size(); j++) {

        // test
        a = tour[i  ];
        b = tour[i+1];
        c = tour[j  ];
        d = tour[j+1];

        t1 = a;
        t2 = b;
        t3 = d;
        t4 = c;


        testA = distance(t1,t2) > distance(t2,t3);
        testB = distance(t3,t4) > distance(t4,t1);

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
  int max_loops = 0;
  int a;
  int b;
  int c;
  int d;
  int end;    

  while(bestGain != float(0) && max_loops < MAX_2_OPT_LOOPS) {
    bestGain = float(0);

    // testA
    for(int i=1; i<nodes.size(); ++i) {

      a = tour[i];
      //auto aIter = std::find(tour.begin(), tour.end(), a);
      b = tour[i+1];

      for(int d : nodes[b].neighbourhood) {

        if(distance(b,a) > distance(b,d)) {
          //auto dIter = std::find(tour.begin(), tour.end(), d);

          //if(dIter == tour.begin()) continue;
          if(nodes[d].tour_index==0) continue;

          //dIter--;
          //int c = *dIter;
          c = tour[nodes[d].tour_index-1];

          if(nodes[c].tour_index < nodes[a].tour_index) continue;

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

   for(int i = 1; i < nodes.size(); i++) {


      c = tour[i  ];
      //auto cIter = std::find(tour.begin(), tour.end(), c);
      d = tour[i+1];

      for(int a : nodes[c].neighbourhood) {

        if(distance(c,d) > distance(c,a)) {

          //auto aIter = std::find(tour.begin(), tour.end(), a);
          //auto last = tour.end();
          //last--;

          //if(aIter == last) continue;
          if(nodes[a].tour_index == tour.size()-1) continue;

          //auto bIter = aIter+1;
          //int b = *bIter;
          b = tour[nodes[a].tour_index+1];

          if(nodes[c].tour_index < nodes[a].tour_index) continue;

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

      //auto best_iIter = std::find(tour.begin(), tour.end(), best_i);
      //auto best_jIter = std::find(tour.begin(), tour.end(), best_j);

      //best_iIter++;
      //best_jIter++;

      //std::reverse(best_iIter, best_jIter); // Reverse path

      std::reverse(&tour[nodes[best_i].tour_index + 1], &tour[nodes[best_j].tour_index+1]);


      end = nodes[best_j].tour_index+1;
      for(size_t i=nodes[best_i].tour_index + 1; i<end; ++i)
        nodes[tour[i]].tour_index = i;

      max_loops++;
    }
  }
}
#endif

#ifdef SHUFFLING
void TSP::shuffle(int n) {
  for(size_t i=0; i<n; ++i) {

    int tourA = 1 + rand() % (nodes.size()-2);
    int tourB = 1 + rand() % (nodes.size()-2);

    std::swap(tour[tourA],tour[tourB]);

    nodes[tour[tourA]].tour_index = tourA;
    nodes[tour[tourB]].tour_index = tourB;
  }
}
#endif
