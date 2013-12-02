#include <iostream>
#include <vector>
#include <math.h>

#ifndef TSP_AVALG
#define TSP_AVALG

//----------------------------------------------------
// Debug macros
//----------------------------------------------------

//#define KATTIS        // Outcomment to run in debug mode

// --------------------------------------------------
// Visual Studio crap
// --------------------------------------------------
#ifndef KATTIS
  #define OPEN_GL
  #define DRAW_NODE_NUMBERS
  #define scanf(...) scanf_s(__VA_ARGS__)
#endif

//-----------------------------------------------------
// Run macros
//-----------------------------------------------------

#define MAX_2_OPT_LOOPS INT_MAX
#define USE_PRECOMPILED_DISTANCE_MATRIX     // Outcomment to disable distance matrix
//#define USE_BUCKETS                     // Outcomment to disable neighbourhood buckets

// Pick solving method
#define GREEDY
//#define TWO_OPT
//#define GREEDY_TWO_OPT


//-----------------------------------------------------
// More macros
//-----------------------------------------------------
#ifdef USE_BUCKETS // Change these to alter behaviour of buckets
  #define NEIGHBOURHOOD_LIMIT 100
#endif


//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
// NO MORE MACROS
//----------------------------------------------------------------------------
//----------------------------------------------------------


// ---------------------------------------------------
// The Node Class
// ---------------------------------------------------

class Node {
  public:
    float x, y;
    bool used;

    #ifdef USE_BUCKETS
    int neighbourhood[NEIGHBOURHOOD_LIMIT];
    #endif

    // constructors
    Node() : used(false) {}
    Node(float x, float y) : x(x), y(y), used(false) {}
 
};

// --------------------------------------------------------
// The TSP class
// --------------------------------------------------------

class TSP {

  public:
    // constructor
    TSP() { }

    // stuff
    void solve();
    std::vector<Node>& get_nodes() { return nodes; }
    std::vector<int>& get_tour() { return tour; }
    void init_buckets();
    void init_distances();


  private:

    std::vector<Node> nodes;
    std::vector<int> tour;

    float distance(const Node& a, const Node& b);
    float distance(int a, int b);

    void two_opt();
    void three_opt();
    void greedy(int start);

    #ifdef USE_PRECOMPILED_DISTANCE_MATRIX
    std::vector<std::vector<float>> distances;
    #endif

    #ifdef USE_BUCKETS
    // BUCKETS!
    #endif
};





#endif