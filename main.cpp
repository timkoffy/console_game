#include "Game.h"
#include <chrono>
#include <cstdlib>
#include <stdio.h>
#include <thread>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("must be 2 arguments");
        return 1;
    }

    int h=std::atoi(argv[1]);
    int w=std::atoi(argv[2]);

    if (h<5 || w<5) {
        printf("height and width must be more than 5");
        return 1;
    }

    system("stty -icanon");
    system("stty -echo");
    printf("\e[?25l");
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    Game game;
    game.setup(h,w);
    game.run();
    return 0;
}
