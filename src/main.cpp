#include <gl/GL.h>
#include "life.h"

//generate binary with OpenGL

int main() {
	int H = 50, W = 50, N = 50;
	life game(H, W, N, 8917534134);
	game.update(4);
	auto data= game.getData();

	return 0;

}