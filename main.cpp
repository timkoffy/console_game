#include <cstdlib>
#include <stdio.h>
#include <vector>

void drawField(int height, int width, int x, int y, std::vector<std::pair<int, int>> dots) {
    system("clear");
    printf("x = %i, y = %i\npress [f] to dot\n\n", x,y);
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
                    if (!is_dot) printf(". ");
                }
            }
        }
        printf("\n");
    }
}

int main() {
    int height,width;
    int x=1;
    int y=1;
    unsigned int direction;
    std::vector<std::pair<int, int>> dots;

    do {
        printf("введите высоту и ширину:\n");
        scanf("%i %i", &height, &width);
    } while (height<3 || width<3);

    system("stty -icanon");
    system("stty -echo");
    printf("\e[?25l");

    drawField(height,width,x,y,dots);

    while (1) {
        direction = getchar();
        switch (direction) {
            case 119:
            case 134:
                if (y-1>0) y--; break;
            case 100:
            case 178:
                if (x+2<width) x++; break;
            case 115:
            case 139:
                if (y+2<height) y++; break;
            case 97:
            case 132:
                if (x-1>0) x--; break;
            case 102:
                int flag=1;
                for (int q=0; q<dots.size(); q++) {
                    if (dots[q]==std::make_pair(x,y)) flag=0;
                }
                if (flag) dots.push_back(std::make_pair(x,y));


        }
        drawField(height,width,x,y,dots);
    }
}