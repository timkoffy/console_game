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
    std::vector<std::pair<int, int>> dots;
    const int SPEED = 30;

public:
    void setup_game() {
        do {
            printf("введите высоту и ширину:\n");
            scanf("%i %i", &height, &width);
        } while (height<3 || width<3);

        system("stty -icanon");
        system("stty -echo");
        printf("\e[?25l");

        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

        draw_field(height,width,x,y,dots);
    }

    void draw_field(int height, int width, int x, int y, std::vector<std::pair<int, int>> dots) {
        system("clear");
        printf("x = %i, y = %i\nWASD - поворот, F - поставить точку, C - очистить поле\n\n", x,y);
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
                direction=1; break;
            case 100: case 178:
                direction=2; break;
            case 115: case 139:
                direction=3; break;
            case 97: case 132:
                direction=4; break;
            case 99:
                dots.clear(); break;
            case 102: case 176:
                int is_dot_exist=0;
                for (int i=0; i<dots.size(); i++) {
                    if (dots[i]==std::make_pair(x,y)) is_dot_exist=1;
                }
                if (!is_dot_exist) {
                    dots.emplace_back(x,y);
                } break;
        }
    }

    void move() {
        switch (direction) {
            case 1: if (y-1>0) y--; break;
            case 2: if (x+2<width) x++; break;
            case 3: if (y+2<height) y++; break;
            case 4: if (x-1>0) x--; break;
        }
    }


    void playtime() {
        setup_game();
        while (1) {
            int ch = getchar();
            if (ch != EOF) key_input(ch);
            move();
            draw_field(height,width,x,y,dots);
            std::this_thread::sleep_for(std::chrono::milliseconds(SPEED));
        }
    }
};

int main() {
    Game game;
    game.playtime();
    return 0;
}