#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#define LEN(x) (sizeof x)/(sizeof *x)

typedef struct {
	int x;
	int y;
} Coord;

typedef struct {
	Coord start;
	Coord end;
	char * colour;
} Line;

typedef struct {
	Coord top_left;
	Coord bottom_right;
} Viewbox;

typedef enum {
	Left,
	Right
} Direction;

Coord complex2coord(complex x) {
	Coord coord = {
		.x = (int) creal(x),
		.y = (int) cimag(x),
	};
	return coord;
}

double deg2rad(double x) {
	return (x * M_PI) / 180.0;
}

complex rotate(Direction branch_direction, complex direction, double angle) {
	switch (branch_direction) {
		case Left:
			direction = direction * CMPLX(cos(deg2rad(angle)), sin(deg2rad(angle)));
			break;
		case Right:
			direction = direction * CMPLX(cos(deg2rad(-angle)), sin(deg2rad(-angle)));
			break;
	}
	return direction;
}

Viewbox update_viewbox(Viewbox vbx, complex position) {
		Coord pos = complex2coord(position);
		vbx.top_left.x     = (pos.x < vbx.top_left.x)     ? pos.x : vbx.top_left.x;
		vbx.top_left.y     = (pos.y < vbx.top_left.y)     ? pos.y : vbx.top_left.y;
		vbx.bottom_right.x = (pos.x > vbx.bottom_right.x) ? pos.x : vbx.bottom_right.x;
		vbx.bottom_right.y = (pos.y > vbx.bottom_right.y) ? pos.y : vbx.bottom_right.y;
		return vbx;
}

void print_complex(complex arg) {
	printf("%.2f + %.2fi\n", creal(arg), cimag(arg));
}

Viewbox tree(Line * lines, int depth, double angle, double scale_factor) {
	static int line_no = 0;
	static complex position = CMPLX(0,0);
	static Viewbox vbx = {
		.top_left = { .x = 0, .y = 0 },
		.bottom_right = { .x = 0, .y = 0 }
	};
	static complex direction = CMPLX(0, -LENGTH);
	int no_colours = LEN(colours);

	Line line = {
		.start = complex2coord(position),
		.end = complex2coord(position + direction),
		.colour = colours[depth % no_colours]
	};
	lines[line_no++] = line;

	vbx = update_viewbox(vbx, position + direction);

	if (depth > 0) {

		/* save starting position and direction */
		complex start_position  = position,
						start_direction = direction;

		position = start_position + direction;

		direction = rotate(Left, start_direction, angle) * scale_factor;
		vbx = update_viewbox(vbx, position);
		tree(lines, depth - 1, angle, scale_factor);

		direction = rotate(Right, start_direction, angle) * scale_factor;
		vbx = update_viewbox(vbx, position);
		tree(lines, depth - 1, angle, scale_factor);

		position = start_position;
		direction = start_direction;
	}

	return vbx;
}

int main(void) {
	size_t num_lines = (1 << (depth + 1)) - 1;
	Line * lines = calloc(1 << (depth + 1), sizeof(Line));

	FILE * fp;
	fp = fopen("tree.svg", "w");

	/* record xml metadata */
	fputs("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n", fp);

	Viewbox vbx = tree(lines, depth, angle, scale_factor);

	int width  = (vbx.bottom_right.x - vbx.top_left.x),
	    height = (vbx.bottom_right.y - vbx.top_left.y),
			min_x  = vbx.top_left.x,
			min_y  = vbx.top_left.y;

	fprintf(fp, "<svg width=\"%d\" height=\"%d\" viewBox=\"%d %d %d %d\" xmlns=\"http://www.w3.org/2000/svg\">\n",
					width, height,
					min_x, min_y, width, height);
	        

	fprintf(fp, "<rect width=\"100%%\" height=\"100%%\" x=\"%d\" y=\"%d\" fill=\"%s\" />\n",
	        vbx.top_left.x, vbx.top_left.y, background);

	for (size_t i = 0; i < num_lines; i++) {
		fprintf(fp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"%s\" stroke-width=\"%d\" />\n",
		        lines[i].start.x,
		        lines[i].start.y,
		        lines[i].end.x,
		        lines[i].end.y,
						lines[i].colour,
						WIDTH);
	}

	fputs("</svg>\n", fp);

	fclose(fp);
}
