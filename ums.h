// Tree node
struct node {
  int depth;
  struct node *parent;
  struct node *prev;
  struct node *next;
  char move[6][6];
  unsigned short piece_moved_i, piece_moved_j;
};

// FIFO
struct node *first_node = NULL;
struct node *current_node = NULL;
struct node *last_node = NULL;

struct node *solution_node = NULL;

// Perfs and debugging
unsigned int new_moves = 0;  // new possible moves at each round 
unsigned int fifo_size = 0;  // the total number of tested moves
unsigned int prev_depth = 0;

void next_moves();
void add(char move[][6], unsigned short i, unsigned short j);
int equals(char a[][6], char b[][6]);
void printm(char move[][6]);
