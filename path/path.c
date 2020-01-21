#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum directions {
  North,
  South,
  East,
  West
};

enum directions current_direction = North;

typedef struct {
  int x;
  int y;
} Coord;

typedef struct node {
  Coord coord;
  struct node * next;
} Node;

Node * coord_list = NULL;
Coord curr_pos = {.x=0, .y=0};

Node * create_node(Coord coord) {
  Node * node = malloc(sizeof(node));
  node->coord = coord;
  node->next = NULL;

  return node;
}

void append(Coord coord) {
  static Node * tail;
  if (coord_list->next == NULL) {
    coord_list->next = create_node(coord);
    tail = coord_list->next;
  } else {
    tail->next = create_node(coord);
    tail = tail->next;
  }
}

void print_coord(Coord c) {
  printf("(%d, %d)", c.x, c.y);
}

void print_list(void) {
  printf("[");
  for (Node * n = coord_list; n != NULL; n = n->next) {
    print_coord(n->coord);
    if (n->next) printf(", ");
    else printf("]\n");
  }
}

void L(void) {
  switch (current_direction) {
    case North:
      current_direction = West;
      break;

    case West:
      current_direction = South;
      break;

    case South:
      current_direction = East;
      break;

    case East:
      current_direction = North;
      break;
  }
}

void R(void) {
  switch (current_direction) {
    case North:
      current_direction = East;
      break;

    case East:
      current_direction = South;
      break;
      
    case South:
      current_direction = West;
      break;

    case West:
      current_direction = North;
      break;
  }
}

void F(void) {
  Coord mid_pos = curr_pos;
  switch (current_direction) {
    case (North):
      mid_pos.y++;
      curr_pos.y += 2;
      break;

    case (South):
      mid_pos.y--;
      curr_pos.y -= 2;
      break;

    case (East):
      mid_pos.x++;
      curr_pos.x += 2;
      break;

    case (West):
      mid_pos.x--;
      curr_pos.x -= 2;
      break;
  }
  append(mid_pos);
  append(curr_pos);
}

void p(int n) {
  if (n == 0) {
    F();
  } else {
    p(n-1);
    L();
    p(n-1);
    R();
  }
}

void write_ff(char * fname) {
  int max_x = 0,
      min_x = 0,
      max_y = 0,
      min_y = 0;
  for (Node * n = coord_list; n != NULL; n = n->next) {
    max_x = (n->coord.x > max_x) ? n->coord.x : max_x;
    min_x = (n->coord.x < min_x) ? n->coord.x : min_x;
    max_y = (n->coord.y > max_y) ? n->coord.y : max_y;
    min_y = (n->coord.y < min_y) ? n->coord.y : min_y;
  }
  uint16_t width = (max_x - min_x) + 1;
  uint16_t height = (max_y - min_y) + 1;

  FILE * fp;
  fp = fopen(fname, "wb");
  if (!fp) {
    printf("Failed to open file %s for saving the image to.\n", fname);
    exit(EXIT_FAILURE);
  }
  
  fwrite("farbfeld", sizeof(char), 8, fp);
  uint32_t geom[2] = { htonl(width), htonl(height) };
  fwrite(geom, sizeof(uint32_t), 2, fp);

  uint16_t * pixels = calloc(width * height * 4, sizeof(uint16_t));

  for (int i = 3; i < width * height * 4; i += 4) {
    pixels[i] = 0xFFFF;
  }

  int x, y;
  for (Node * n = coord_list; n != NULL; n = n->next) {
    x = n->coord.x - min_x;
    y = n->coord.y - min_y;
    memset(pixels + (y * width * 4) + (x * 4), 0xFF, 8);
  }

  fwrite(pixels, sizeof(uint16_t), width * height * 4, fp);

  fclose(fp);
}

int main(int argc, char ** argv) {
  if (argc != 2) { 
		printf("Usage: %s <depth>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
  int n;
  sscanf(argv[1], "%d", &n);
  coord_list = create_node(curr_pos);
  p(n);
  write_ff("path.ff");
	return 0;
}
