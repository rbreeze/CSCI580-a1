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
    cout << x << ", " << y;
  }

  int x; 
  int y; 
  int manhattan_distance; 
  int g_score; 
  int f_score; 
  int cost; 
  bool open = false; 
  bool closed = false; 
  bool printed = false; 
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
  dist += goal->y - current->y; 
  return dist; 
}

// prints the grid
void printGrid(Node*** grid) {
  for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
      cout << *(grid[i][j]);
    }
    cout << endl;
  }
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
  Node*** grid = new Node** [WIDTH]; 
  for (int i = 0; i < WIDTH; i++)
    grid[i] = new Node* [HEIGHT];

  // declare character to store input
  char input;

  // get the grid from std input
  for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
      cin >> input; 

      // created Node* with initial G and F scores of infinity
      Node* n = new Node(i, j, input, INF, INF);

      // store Node* in grid 
      grid[i][j] = n;
    }
  }

  // define pointers to start and goal nodes
  Node* start_node = grid[start_x][start_y]; 
  Node* goal_node = grid[goal_x][goal_y];

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

    // make pointers to all neighbors
    Node* up = y > 0 ? grid[x][y-1] : nullptr;
    Node* down = y < HEIGHT-1 ? grid[x][y+1] : nullptr; 
    Node* right = x < WIDTH-1 ? grid[x+1][y] : nullptr;
    Node* left = x > 0 ? grid[x-1][y] : nullptr;

    // define array of neighbors to iterate over
    Node* neighbors[4] = { left, down, up, right };

    // iterate through current node's neighbors
    for (int i = 0; i < 4; i++) {

      // define current neighbor
      Node* neighbor = neighbors[i];

      // check if neighbor is null, closed, or impassable
      if (neighbor == nullptr || neighbor->closed || neighbor->terrain == 'x')
        continue;

      int tentative_g = current->g_score + get_weight(neighbor);

      if (!neighbor->open) {
        open.push(neighbor);
        neighbor->open = true;
      } else if (tentative_g >= neighbor->g_score) continue; 

      // if this is the currently best available path, save it
      cameFrom[neighbor] = current;
      neighbor->g_score = tentative_g; 
      neighbor->f_score  = neighbor->g_score + get_manhattan_distance(neighbor, goal_node);
    }

  }

  // draw path

   while (cameFrom[current] && !current->printed) {

    Node* prev = cameFrom[current];

    if (prev->y < current->y) 
      prev->terrain = 'v';
    else if (prev->y > current->y)
      prev->terrain = '^';
    else if (prev->x < current->x) 
      prev->terrain = '>';
    else if (prev->x > current->x) 
      prev->terrain = '<';

    current->printed = true;

    current = prev;

  } 

  // set start and goal node characters
  start_node->terrain = 'S'; 
  goal_node->terrain = 'G';

  // print the grid
  printGrid(grid);

  // De-Allocate memory
  for (int i = 0; i < WIDTH; ++i) delete [] grid[i];
  delete [] grid;

  return 0;
}
