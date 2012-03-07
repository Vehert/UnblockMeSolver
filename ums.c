#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ums.h"

#include "puzzles/empty.h"
#include "puzzles/test.h"
#include "puzzles/beginner.h"
#include "puzzles/intermediate.h"
#include "puzzles/advanced.h"


int main(int argc, char *argv[]) {
  struct node root;

  // Init
  root.depth = 0;
  root.parent = NULL;
  root.prev = NULL;
  root.next = NULL;
  memcpy(root.move, beginner_puzzle40, sizeof(char) * 36);
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
    printf("Solution found in %u moves (%u tested moves).\n", solution_node->depth, fifo_size);
  } else {
    printf("No solution found.\n");
  }

  return 0;
}


int equals(char a[][6], char b[][6]) {
  short i, j;
  for (j = 0; j < 6; j++) {
    for (i = 0; i < 6; i++) {
      if (a[i][j] != b[i][j])
	return 0;
    }
  }
  return 1;
}


void add(char move[][6], unsigned short i, unsigned short j) {
  // Do not add a previous move
  struct node *n = last_node;
  do {
    if (equals(n->move, move))
      return;
  } while (((n = n->prev) != NULL) && (current_node->depth - n->depth < 3));

  struct node *new_node = malloc(sizeof(struct node));
  memcpy(new_node->move, move, sizeof(char) * 36);

  new_node->depth = current_node->depth + 1;
  new_node->parent = current_node;
  new_node->prev = last_node;
  new_node->next = NULL;
  new_node->piece_moved_i = i;
  new_node->piece_moved_j = j;

  last_node->next = new_node;
  last_node = new_node;

  //printm(new_node->move);
  new_moves++;
  fifo_size++;
}


void next_moves() {
  char working_move[6][6];
  char next_move[6][6];
  short i, j, k;
  char piece;

  memcpy(working_move, current_node->move, sizeof(char) * 36);

  for (j = 0; j < 6; j++) {
    for (i = 0; i < 6; i++) {
    
      piece = working_move[i][j];
      if (piece == ' ')
	continue;

      if (piece == '-' || piece == '*') {
	// Horizontal pieces of size 2

	if (! (current_node->piece_moved_i == i && current_node->piece_moved_j == j)) {

	  // Move right
	  memcpy(next_move, current_node->move, sizeof(char) * 36);
	  for (k = j + 2; k < 6 && current_node->move[i][k] == ' '; k++) {
	    if (k == 5 && piece == '*') {
	      solution_node = last_node;
	      return;
	    }
	    next_move[i][k] = piece;
	    next_move[i][k - 2] = ' ';
	    add(next_move, i, k - 1);
	  }

	  // Move left
	  memcpy(next_move, current_node->move, sizeof(char) * 36);
	  for (k = j - 1; k >= 0 && current_node->move[i][k] == ' '; k--) {
	    next_move[i][k] = piece;
	    next_move[i][k + 2] = ' ';
	    add(next_move, i, k);
	  }
	
	}
	// Mark the piece as dealt
	working_move[i][j + 1] = 'x';

      } else if (piece == '|') {
	// Vertical pieces of size 2

	if (! (current_node->piece_moved_i == i && current_node->piece_moved_j == j)) {

	  // Move up
	  memcpy(next_move, current_node->move, sizeof(char) * 36);
	  for (k = i - 1; k >= 0 && current_node->move[k][j] == ' '; k--) {
	    next_move[k][j] = '|';
	    next_move[k + 2][j] = ' ';
	    add(next_move, k, j);
	  }

	  // Move down
	  memcpy(next_move, current_node->move, sizeof(char) * 36);
	  for (k = i + 2; k < 6 && current_node->move[k][j] == ' '; k++) {
	    next_move[k - 2][j] = ' ';
	    next_move[k][j] = '|';
	    add(next_move, k - 1, j);
	  }

	}

	// Mark the piece as dealt
	working_move[i + 1][j] = 'x';

      } else if (piece == 'I') {
	// Vertical pieces of size 3

	if (! (current_node->piece_moved_i == i && current_node->piece_moved_j == j)) {

	  // Move up
	  memcpy(next_move, current_node->move, sizeof(char) * 36);
	  for (k = i - 1; k >= 0 && current_node->move[k][j] == ' '; k--) {
	    next_move[k][j] = 'I';
	    next_move[k + 3][j] = ' ';
	    add(next_move, k, j);
	  }

	  // Move down
	  memcpy(next_move, current_node->move, sizeof(char) * 36);
	  for (k = i + 3; k < 6 && current_node->move[k][j] == ' '; k++) {
	    next_move[k][j] = 'I';
	    next_move[k - 3][j] = ' ';
	    add(next_move, k - 2, j);
	  }
       
	}

	// Mark the piece as dealt
	working_move[i + 1][j] = 'x';
	working_move[i + 2][j] = 'x';

      } else if (working_move[i][j] == '=') {
	// Horizontal pieces of size 3

	if (! (current_node->piece_moved_i == i && current_node->piece_moved_j == j)) {

	  // Move left
	  memcpy(next_move, current_node->move, sizeof(char) * 36);
	  for (k = j - 1; k >= 0 && current_node->move[i][k] == ' '; k--) {
	    next_move[i][k] = '=';
	    next_move[i][k + 3] = ' ';
	    add(next_move, i, k);
	  }

	  // Move right
	  memcpy(next_move, current_node->move, sizeof(char) * 36);
	  for (k = j + 3; k < 6 && current_node->move[i][k] == ' '; k++) {
	    next_move[i][k] = '=';
	    next_move[i][k - 3] = ' ';
	    add(next_move, i, k - 2);
	  }
       
	}

	// Mark the piece as dealt
	working_move[i][j + 1] = 'x';
	working_move[i][j + 2] = 'x';

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
