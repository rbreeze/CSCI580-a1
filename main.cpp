// main.cpp
// Breeze, Remington
// rbreeze

#include <iostream>
#include <cstdlib>
#include <limits.h> 
#include <queue> 
#include <vector> 
#include <map> 

#define INF INT_MAX

using std::cout; 
using std::cin; 
using std::endl;
using std::ostream;
using std::vector;
using std::priority_queue;
using std::map;
using std::string;
using std::to_string;

int WIDTH; int HEIGHT; 

struct Node {
  // Node constructor
  Node(int x_prime, int y_prime, char t_prime, int g_prime, int f_prime) {
    x = x_prime; 
    y = y_prime; 
    terrain = t_prime;
    g_score = g_prime; 
    f_score = f_prime;
  }
  // Node constructor with only coordinates
  Node(int x_prime, int y_prime) {
    x = x_prime; 
    y = y_prime; 
  }

  void printXY() {
    cout << x << ", " << y << endl;
  }

  int x; 
  int y; 
  int manhattan_distance; 
  int g_score; 
  int f_score; 
  int cost; 
  bool open = false; 
  bool closed = false; 
  char terrain;
}; 

// Node comparison structure for priority queue based on F score
struct NodeFComparison {
  bool operator()(Node* a, Node* b) {
    return a->f_score > b->f_score; 
  }
};

// Node comparison structure for x and y coordinates
struct NodeXYComparison {
  bool operator() (const Node* lhs, const Node* rhs) const {
    string l = to_string(lhs->x) + to_string(lhs->y); 
    string r = to_string(rhs->x) + to_string(rhs->y); 
    return l < r;
  } 
};

// overloaded std output for Node
ostream& operator<<(ostream& s, const Node& n) {
  s << n.terrain; 
  return s; 
}

// Heuristic function
int get_manhattan_distance(Node* current, Node* goal) {
  // x component of heuristic
  int dist = goal->x - current->x; 
  // add y component
  dist+= goal->y - current->y; 
  return dist; 
}

int get_weight(Node* n) {
  switch (n->terrain){
    case '.': 
      return 1; 
    case '*': 
      return 3; 
    case 'o': 
      return 5; 
    case '~': 
      return 7; 
    case 'x': 
      return INF; 
    default: 
      return 0; 
  }
}

int main(int argc, char** argv) {

  // make sure program is being used correctly
  if (argc < 3) {
    cout << "Incorrect usage." << endl;
    return 1; 
  } 

  // define global width and height
  ::WIDTH = atoi(argv[1]); 
  ::HEIGHT = atoi(argv[2]);

  // set start coordinates
  int start_x = argc > 3 ? atoi(argv[3]) : 0; 
  int start_y = argc > 4  ? atoi(argv[4]) : 0;

  // set goal coordinates
  int goal_x = argc > 5 ? atoi(argv[5]) : (WIDTH - 1);
  int goal_y = argc > 6 ? atoi(argv[6]) : (HEIGHT - 1);

  // allocate memory for grid of Node*
  Node*** grid = new Node** [ HEIGHT ]; 
  for (int i = 0; i < HEIGHT; ++i)
    grid[i] = new Node* [WIDTH];

  // declare character to store input
  char input;

  // get the grid from std input
  for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
      cin >> input; 
      // store Node* in grid with initial G and F scores of infinity
      grid[i][j] = new Node(i, j, input, INF, INF);
    }
  }

  // define pointers to start and goal nodes
  Node* start_node = grid[start_x][start_y]; 
  Node* goal_node = grid[goal_x][goal_y];

  cout << "Start: " << start_node->x << ", " << start_node->y << endl;
  cout << "Goal: " << goal_node->x << ", " << goal_node->y << endl;

  // set start node's f score to its manhattan distance from the goal
  start_node->f_score = get_manhattan_distance(start_node, goal_node);

  // set start node's g score to zero
  start_node->g_score = 0; 

  // declare a priority queue to hold open nodes and push start node to it
  priority_queue<Node*, vector<Node*>, NodeFComparison> open; 
  open.push(start_node);

  // declare map for most efficient path
  map<Node*,Node*,NodeXYComparison> cameFrom;

  // cycle through queue 
  Node* current; 
  while (!open.empty()) {
    // current node is the one with the lowest f_score
    current = open.top(); 

    // if we've reached our goal, stop the loop
    if (current == goal_node) break; 

    // remove current from open and add it to closed
    open.pop();
    current->open = false;
    current->closed = true; 

    // set local variables for more concise syntax
    int x = current->x; int y = current->y; 

    // define minimums for x and y directions
    int min_x = x > 0 ? x - 1 : x;
    int min_y = y > 0 ? y - 1 : y;  
    
    // define caps for x and y directions
    int max_x = x >= WIDTH-1 ? WIDTH-1 : x+1; 
    int max_y = y >= HEIGHT-1 ? HEIGHT-1 : y+1; 

    // iterate through current node's neighbors
    for (int i = min_x; i <= max_x; i++) {
      for (int j = min_y; j <= max_y; j++) {
        if (i == x || j == y) {

          // define current neighbor
          Node* neighbor = grid[i][j];

          // only continue if the neighbor isn't closed
          if (!neighbor->closed) {

            int tentative_g = current->g_score + get_weight(neighbor);

            if (!neighbor->open) {
              open.push(neighbor);
              neighbor->open = true;
            }
            if (tentative_g <= neighbor->g_score) {
              cameFrom[neighbor] = current;
              neighbor->g_score = tentative_g; 
              neighbor->f_score  = neighbor->g_score + get_manhattan_distance(neighbor, goal_node);
            }
          }
        }
      }
    }

  }

  // reconstruct path
  vector<Node*> path;
  path.push_back(current);

  while (cameFrom[current]) {
    current = cameFrom[current];
    path.push_back(current); 
  }

  // set terrain of path
  for (int i = 0; i < path.size(); i++) {
    path[i]->terrain = '1';
  }

  // print the grid
  for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
      cout << *(grid[i][j]);
    }
    cout << endl;
  }

  cout << endl;

  // De-Allocate memory
  for (int i = 0; i < HEIGHT; ++i) delete [] grid[i];
  delete [] grid;

  return 0;
}
