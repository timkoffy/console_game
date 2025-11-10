#include <chrono>
#include <cstdlib>
#include <stdio.h>
#include <thread>
#include <vector>
#include <fcntl.h>
#include <iostream>

class Game {
    int SPEED = 100;
    int TAIL_LENGHT = 10;

    int height,width;

    int x_player=1;
    int y_player=1;

    int x_fruit;
    int y_fruit;

    std::vector<std::pair<int, int>> tail;
    std::vector<std::pair<int, int>> fruits;

    bool GAME_STARTS = false;
    bool GAME_OVER = false;

    enum keys {
        W_KEY_ENG = 119,
        W_KEY_RUS = 134,
        D_KEY_ENG = 100,
        D_KEY_RUS = 178,
        S_KEY_ENG = 115,
        S_KEY_RUS = 139,
        A_KEY_ENG = 97,
        A_KEY_RUS = 132
    };

    enum directions {
        UP = 0,
        RIGHT = 1,
        DOWN = 2,
        LEFT = 3
    };
    directions direction;

public:
    void setup(int h, int w) {
        height=h; width=w;
        fruit_spawn();
        draw_field();
    }

    void draw_field() {
        system("clear");

        std::cout << "score: " << TAIL_LENGHT << std::endl;
        std::cout << x_fruit << " " << y_fruit << "\n" << std::endl;

        for (int y = 0; y < height; y++) {
            if ( y == 0 || y == height-1 ) {
                for (int x = 0; x < width; x++) {
                    std::cout << "# ";
                } std::cout << "\n"; continue;
            }
            for (int x = 0; x < width; x++) {
                if ( x == x_player && y == y_player) {
                    std::cout << "& "; continue;
                } if ( x == 0 || x == width-1) {
                    std::cout << "# "; continue;
                } if ( x == x_fruit && y == y_fruit ) {
                    std::cout << "% "; continue;
                }
                bool exists = false;
                for (int tail_index = 0; tail_index < tail.size(); tail_index++) {
                    if (x == tail[tail_index].first &&
                        y == tail[tail_index].second) {
                        std::cout << "* ";
                        exists = true;
                    } if (GAME_STARTS && x_player == tail[tail_index].first &&
                        y_player == tail[tail_index].second) {
                        GAME_OVER = true;
                    }
                }
                if (!exists) std::cout << "  ";
            } std::cout << "\n";
        }
    }

    void key_input(int key) {
        switch (key) {
            case W_KEY_ENG: case W_KEY_RUS:
                if (direction != DOWN) direction = UP; break;
            case D_KEY_ENG: case D_KEY_RUS:
                if (direction != LEFT) direction = RIGHT; break;
            case S_KEY_ENG: case S_KEY_RUS:
                if (direction != UP) direction = DOWN; break;
            case A_KEY_ENG: case A_KEY_RUS:
                if (direction != RIGHT) direction = LEFT; break;
        }
    }

    void move() {
        switch (direction) {
            case UP: y_player--; break;
            case RIGHT: x_player++; break;
            case DOWN: y_player++; break;
            case LEFT: x_player--; break;
        }
        if (x_player == 0) x_player = width - 2;
        if (y_player == 0) y_player = height - 2;
        if (x_player == width-1) x_player = 1;
        if (y_player == height-1) y_player = 1;
    }

    void tail_spawn() {
        int exists=0;
        for (int i=0; i<tail.size(); i++) {
            if (tail[i]==std::make_pair(x_player,y_player)) exists=1;
        }
        if (!exists) tail.emplace_back(x_player,y_player);
    }

    void tail_update() {
        tail.emplace_back(x_player,y_player);
        tail.erase(tail.begin());
        GAME_STARTS = true;
    }

    void fruit_spawn() {
        srand(static_cast<unsigned int>(time(nullptr)));
        x_fruit = rand() % (width-2) + 1;
        y_fruit = rand() / width % (height-2) + 1;
    }

    void fruit_collected() {
        TAIL_LENGHT++;
        system("echo -e '\a'");
        fruit_spawn();
    }

    void run() {
        while (!GAME_OVER) {
            int ch = getchar();
            if (ch != EOF) key_input(ch);

            if (tail.size()<TAIL_LENGHT) tail_spawn();
            else tail_update();

            move();

            if ( x_player == x_fruit && y_player == y_fruit ) fruit_collected();

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