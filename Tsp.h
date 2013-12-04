#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

#if __GNUC__
    #include <climits>
    #include <stdio.h>
#endif

#ifndef TSP_AVALG
#define TSP_AVALG

//----------------------------------------------------
// Debug macros
//----------------------------------------------------

#define KATTIS          // Outcomment to run in debug mode
#define VISUAL_STUDIO   // This is to get rid of annoying scanf warnings

// --------------------------------------------------
// Not running on KATTIS. DO lots of testing
// --------------------------------------------------
#ifndef KATTIS
  #define OPEN_GL
  #define DRAW_NODE_NUMBERS
#endif

// --------------------------------------------------
// Running on VS. Do some stuff
//--------------------------------------------------
#ifndef __GNUC__
  #define scanf(...) scanf_s(__VA_ARGS__)
  #define WINDOWS
#endif

//-----------------------------------------------------
// Run macros
//-----------------------------------------------------

#define MAX_2_OPT_LOOPS 30
#define USE_PRECOMPILED_DISTANCE_MATRIX     // Outcomment to disable distance matrix
#define USE_BUCKETS                     // Outcomment to disable neighbourhood buckets

// Pick solving method
//#define GREEDY
//#define TWO_OPT
#define GREEDY_TWO_OPT


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
    int tour_index;

    #ifdef USE_BUCKETS
    std::vector<int> neighbourhood;
    #endif

    // constructors
    Node() : used(false), tour_index(0) {}
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

    float compute_total_cost();


  private:

    std::vector<Node> nodes;
    std::vector<int> tour;

    float distance(const Node& a, const Node& b);
    float distance(int a, int b);

    float compute_gain(int i, int j);
    float compute_gain(int a, int b, int c, int d);

    void reset_nodes();

    void two_opt();
    void three_opt();
    void greedy(int start);

    #ifdef USE_PRECOMPILED_DISTANCE_MATRIX
    std::vector<std::vector<float>> distances;
    #endif

};





#endif
