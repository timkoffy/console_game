#include <chrono>
#include <cstdlib>
#include <stdio.h>
#include <thread>
#include <vector>
#include <fcntl.h>
#include <iostream>

class Game {
    const int SPEED = 150;
    const int TAIL_LENGHT=10;
    int height,width;
    int x_player=1;
    int y_player=1;
    int direction;
    std::vector<std::pair<int, int>> tail;

public:
    void setup(int h, int w) {
        height=h; width=w;
        draw_field();
        tail.emplace_back(x_player,y_player);
    }

    void draw_field() {
        system("clear");
        for (int y = 0; y < height; y++) {
            if ( y == 0 || y == height-1 ) {
                for (int x = 0; x < width; x++) {
                    std::cout << "# ";
                } std::cout << "\n"; continue;
            }
            for (int x = 0; x < width; x++) {
                if ( y == y_player && x == x_player ) {
                    std::cout << "& "; continue;
                } if ( x == 0 || x == width-1 ) {
                    std::cout << "# "; continue;
                }
                bool exists = false;
                for (int tail_index = 0; tail_index < tail.size(); tail_index++) {
                    if (x == tail[tail_index].first &&
                        y == tail[tail_index].second) {
                        std::cout << "* ";
                        exists = true;
                    }
                } if (!exists) std::cout << "  ";
            } std::cout << "\n";
        }
    }

    void key_input(int key) {
        switch (key) {
            case 119: case 134:
                if (direction!=3) direction=1; break;
            case 100: case 178:
                if (direction!=4) direction=2; break;
            case 115: case 139:
                if (direction!=1) direction=3; break;
            case 97: case 132:
                if (direction!=2) direction=4; break;
        }
    }

    void move() {
        switch (direction) {
            case 1: y_player--; break;
            case 2: x_player++; break;
            case 3: y_player++; break;
            case 4: x_player--; break;
        }
        if (x_player == 0)
            x_player = width-2;
        if (y_player == 0)
            y_player = height-2;
        if (x_player == width-1)
            x_player = 1;
        if (y_player == height-1)
            y_player = 1;
    }

    void tail_spawn() {
        int exists=0;
        for (int i=0; i<tail.size(); i++) {
            if (tail[i]==std::make_pair(x_player,y_player)) exists=1;
        }
        if (!exists) {
            tail.emplace_back(x_player,y_player);
        }
    }

    void tail_update() {
        tail.emplace_back(x_player,y_player);
        tail.erase(tail.begin());
    }

    void tail_collapsed() {

    }

    void run() {
        while (1) {
            int ch = getchar();
            if (ch != EOF) {
                key_input(ch);
            }

            if (tail.size()<TAIL_LENGHT) tail_spawn();
            else tail_update();

            move();

            draw_field();
            std::this_thread::sleep_for(std::chrono::milliseconds(SPEED));
        }
    }
};

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