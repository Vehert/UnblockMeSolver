#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node {
  int depth;
  struct node *parent;
  struct node *prev;
  struct node *next;
  char move[6][6];
};

// FIFO
int fifo_size;
struct node *first_node = NULL;
struct node *current_node = NULL;
struct node *last_node = NULL;
struct node *solution_node = NULL;


int new_moves = 0;
int prev_depth = 0;

char level40[6][6] = {
  {'-', '-', ' ', '|', '|', '|'},
  {'-', '-', ' ', '|', '|', '|'},
  {'|', '|', '*', '*', '|', ' '},
  {'|', '|', '|', ' ', ' ', ' '},
  {'|', ' ', '|', '-', '-', ' '},
  {'|', ' ', ' ', '-', '-', ' '}
};

char level26[6][6] = {
  {'|', '-', '-', '-', '-', ' '},
  {'|', ' ', ' ', '-', '-', '-'},
  {'*', '*', ' ', '|', ' ', '|'},
  {'-', '-', '-', '|', '|', '|'},
  {'-', '-', '-', '-', '|', '|'},
  {' ', ' ', ' ', ' ', ' ', ' '}
};

char level20[6][6] = {
  {'-', '-', '-', '-', ' ', '|'},
  {'|', '-', '-', '-', '-', '|'},
  {'|', '|', '|', '*', '*', '|'},
  {'|', '|', '|', '|', ' ', ' '},
  {'|', '|', '|', '|', '-', '-'},
  {' ', ' ', '|', '-', '-', '-'}
};

char level3[6][6] = {
  {'|', '-', '-', '-', '-', ' '},
  {'|', ' ', '|', ' ', ' ', ' '},
  {'*', '*', '|', '|', '|', ' '},
  {' ', ' ', ' ', '|', '|', ' '},
  {'-', '-', '-', '|', '|', ' '},
  {' ', ' ', ' ', ' ', ' ', ' '}
};

char level1[6][6] = {
  {'-', '-', '-', ' ', ' ', '|'},
  {' ', ' ', '|', ' ', ' ', '|'},
  {'*', '*', '|', ' ', ' ', '|'},
  {'|', ' ', '|', ' ', '-', '-'},
  {'|', ' ', ' ', ' ', '|', ' '},
  {'-', '-', '-', ' ', '|', ' '}
};

char test[6][6] = {
  {' ', ' ', '-', '-', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' '}
};

char empty[6][6] = {
  {' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' '}
};


void next_moves();
void add(char move[][6]);
int equals(char a[][6], char b[][6]);
void printm(char move[][6]);


int main(int argc, char *argv[]) {
  struct node root;

  // Init
  root.depth = 0;
  root.parent = NULL;
  root.prev = NULL;
  root.next = NULL;
  memcpy(root.move, level26, sizeof(char) * 36);
  first_node = current_node = last_node = &root;  
  fifo_size = 1;

  // Search
  next_moves();
  //printf("Depth: %d, new moves: %d, fifo size: %d\n", last_node->depth, new_moves, fifo_size);
  new_moves = 0;
  while (current_node->next != NULL && solution_node == NULL) {
    current_node = current_node->next;
    next_moves();
    //printf("Depth: %d, new moves: %d, fifo size: %d\n", last_node->depth, new_moves, fifo_size);
    /*if (prev_depth < last_node->depth) {
      printf("No solution in less than %d moves.\n", current_node->depth);
      prev_depth = last_node->depth;
    }*/
    new_moves = 0;
  }

  if (solution_node != NULL) {
    printf("Move %d:\n", solution_node->depth);
    printm(solution_node->move);

    current_node = solution_node;
    while (current_node->parent != NULL) {
      current_node = current_node->parent;
      printf("Move %d:\n", current_node->depth);
      printm(current_node->move);
    }
    printf("Solution found in %d moves.\n", solution_node->depth);
  } else {
    printf("No solution found.\n");
  }

  return 0;
}

int equals(char a[][6], char b[][6]) {
  short i, j;
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 6; j++) {
      if (a[i][j] != b[i][j])
	return 0;
    }
  }
  return 1;
}

void add(char move[][6]) {
  // Do not add a previous move
  struct node *n = last_node;
  if (equals(n->move, move)) {
    return;
  }
  while (n->prev != NULL) {
    n = n->prev;
    if (equals(n->move, move)) {
      return;
    }
  }

  struct node *new_node = malloc(sizeof(struct node));
  memcpy(new_node->move, move, sizeof(char) * 36);

  new_node->depth = current_node->depth + 1;
  new_node->parent = current_node;
  new_node->prev = last_node;
  new_node->next = NULL;

  last_node->next = new_node;
  last_node = new_node;

  //printm(new_node->move);
  new_moves++;
  fifo_size++;
}

void next_moves() {
  char working_move[6][6];
  char next_move[6][6];
  short i, j, k, start, end;

  memcpy(working_move, current_node->move, sizeof(char) * 36);

  for (i = 0; i < 6; i++) {
    for (j = 0; j < 6; j++) {
      
      if (working_move[i][j] == '|') {
	// Vertical pieces

	// Locate the piece
	start = i;
	while (start - 1 >= 0 && working_move[start - 1][j] == '|')
	  start--;
	end = i;
	while (end + 1 <= 5 && working_move[end + 1][j] == '|')
	  end++;
	// If the piece's length is > 3 then there is in fact 2 pieces
	if (end - start >= 3)
	  end = start + 1;
	//printf("Piece starts at %d,%d and ends at %d,%d\n", start, j, end, j);
	
	// Move up
	k = start;
	memcpy(next_move, current_node->move, sizeof(char) * 36);
	while (k - 1 >= 0 && current_node->move[k - 1][j] == ' ') {
	  next_move[k - 1][j] = '|';
	  next_move[end - (start - k)][j] = ' ';
	  //printm(next_move);
	  add(next_move);
	  k--;
	}

	// Move down
	k = end;
	memcpy(next_move, current_node->move, sizeof(char) * 36);
	while (k + 1 <= 5 && current_node->move[k + 1][j] == ' ') {
	  next_move[k + 1][j] = '|';
	  next_move[start + (k - end)][j] = ' ';
	  //printm(next_move);
	  add(next_move);
	  k++;
	}
       
	// Mark the piece as dealt
	for (k = start; k <= end; k++)
	  working_move[k][j] = 'x';

      } else if (working_move[i][j] == '-' || working_move[i][j] == '*') {
	// Horizontal pieces
	char piece = working_move[i][j];

	// Locate the piece
	start = j;
	while (start - 1 >= 0 && working_move[i][start - 1] == piece)
	  start--;
	end = j;
	while (end + 1 <= 5 && working_move[i][end + 1] == piece)
	  end++;
	// If the piece's length is > 3 then there is in fact 2 pieces
	if (end - start >= 3)
	  end = start + 1;
	//printf("Piece starts at %d,%d and ends at %d,%d\n", i, start, i, end);

	// Move left
	k = start;
	memcpy(next_move, current_node->move, sizeof(char) * 36);
	while (k - 1 >= 0 && current_node->move[i][k - 1] == ' ') {
	  next_move[i][k - 1] = piece;
	  next_move[i][end - (start - k)] = ' ';
	  //printm(next_move);
	  add(next_move);
	  k--;
	}

	// Move right
	k = end;
	memcpy(next_move, current_node->move, sizeof(char) * 36);
	while (k + 1 <= 5 && current_node->move[i][k + 1] == ' ') {
	  next_move[i][k + 1] = piece;
	  next_move[i][start + (k - end)] = ' ';
	  //printm(next_move);
	  add(next_move);
	  if (piece == '*' && k + 1 == 5) {
	    solution_node = last_node;
	  }
	  k++;
	}
       
	// Mark the piece as dealt
	for (k = start; k <= end; k++)
	  working_move[i][k] = 'x';

      }

    }
  }

} // end next_moves();

void printm(char move[][6]) {
  short i, j;
  printf("++++++++\n");
  for (i = 0; i < 6; i++) {
    printf("+");
    for (j = 0; j < 6; j++) {
      printf("%c", move[i][j]);
    }
    if (i != 2)
      printf("+");
    printf("\n");
  }
  printf("++++++++\n");
}
