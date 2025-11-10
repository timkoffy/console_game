#ifndef CONSOLE_GAME_GAME_H
#define CONSOLE_GAME_GAME_H

#include <vector>

class Game {
private:
    int SPEED = 150;
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
        W_KEY_ENG = 119, W_KEY_RUS = 134,
        D_KEY_ENG = 100, D_KEY_RUS = 178,
        S_KEY_ENG = 115, S_KEY_RUS = 139,
        A_KEY_ENG = 97, A_KEY_RUS = 132
    };

    enum directions { UP = 0, RIGHT, DOWN, LEFT };
    directions direction;

    void move();
    void tail_spawn();
    void tail_update();
    void fruit_spawn();
    void fruit_collected();
    void key_input(int key);
    void draw_field();

public:
    void setup(int h, int w);
    void run();
};


#endif