#include <chrono>
#include <cstdlib>
#include <stdio.h>
#include <thread>
#include <vector>
#include <fcntl.h>


class Game {
private:
    int height,width;
    int x=1;
    int y=1;
    int direction;
    std::vector<std::pair<int, int>> tail;
    const int SPEED = 150;
    const int TAIL_LENGHT=10;

public:
    void setup(int h, int w) {
        system("stty -icanon");
        system("stty -echo");
        printf("\e[?25l");
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

        height=h; width=w;
        draw_field(height,width,x,y,tail);
        tail.emplace_back(x,y);
    }

    void draw_field(int height, int width, int x, int y, std::vector<std::pair<int, int>> dots) {
        system("clear");
        for (int i=0;i<height;i++) {
            if ( i == 0 || i == height-1 ) {
                for (int j=0;j<width;j++) {
                    printf("* ");
                }
            } else {
                for (int j=0;j<width;j++) {
                    if ( i==y && j==x ) {
                        printf("& ");
                    } else if ( j == 0 || j == width-1 ) {
                        printf("* ");
                    } else {
                        int is_dot=0;
                        for (int q=0; q<dots.size(); q++) {
                            if (i==dots[q].second && j==dots[q].first) {
                                printf("0 ");
                                is_dot=1;
                            }
                        }
                        if (!is_dot) printf("  ");
                    }
                }
            }
            printf("\n");
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
            // case 102: case 176:
            //     int is_dot_exist=0;
            //     for (int i=0; i<dots.size(); i++) {
            //         if (dots[i]==std::make_pair(x,y)) is_dot_exist=1;
            //     }
            //     if (!is_dot_exist) {
            //         dots.emplace_back(x,y);
            //     } break;
        }
    }

    void move() {
        switch (direction) {
            case 1: y--; break;
            case 2: x++; break;
            case 3: y++; break;
            case 4: x--; break;
        }
        if (x==0) x=width-1;
        if (y==0) y=height-1;
        if (x==width+1) x=1;
        if (y==height+1) y=1;
    }

    void tail_spawn() {
        int exists=0;
        for (int i=0; i<tail.size(); i++) {
            if (tail[i]==std::make_pair(x,y)) exists=1;
        }
        if (!exists) {
            tail.emplace_back(x,y);
        }
    }

    void tail_update() {
        tail.emplace_back(x,y);
        tail.erase(tail.begin());
    }

    void tail_collapsed() {

    }

    void playtime() {
        while (1) {
            int ch = getchar();
            if (ch != EOF) {
                key_input(ch);
            }

            if (tail.size()<TAIL_LENGHT) tail_spawn();
            else tail_update();

            move();

            draw_field(height,width,x,y,tail);
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

    if (h<3 || w<3) {
        printf("height and width must be more than 3");
        return 1;
    }

    Game game;
    game.setup(h,w);
    game.playtime();
    return 0;
}