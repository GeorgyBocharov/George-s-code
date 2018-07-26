//
// Created by george on 22.05.18.
//

#ifndef GAME_DLC_H
#define GAME_DLC_H


const float PI = 3.141592;
const int INF_MASS = -1;

#define BACK_X 4536
#define BACK_Y 3146
#define WIN_X 1280
#define WIN_Y 720




#define DELTA_V 1
#define COL_DMG 10
#define ALIVE 2
#define BONUS_SIZE 50





#define BONUS_AM 10


#define BOT 3
#define BONUS -10
#define SPACECRAFT 5


#define BONUS_FIRST 1
#define BONUS_SECOND 2

#define SUP_HEALTH 200

#define SUP_MASS 2000
#define SUP_DAMAGE 50

#define pure = 0

#define BOT_AMOUNT 10
#define MET_AM 20

#define Gde printf("Line %d, Function %s\n", __LINE__, __PRETTY_FUNCTION__)



#define DEFAULT 0
#define INTERACTED 1


int chance() {
    return (rand() > RAND_MAX / 2);
}

int sgn(double x) {
    if (x > 0) return 1;
    else if (x < 0) return -1;
    else return 0;
}

float check(float var, float val, float edge) {
    if (val < var)
        return var;
    else
        return edge - var;
}

float speedVar(float val, int sign) {
    int likely;
    if (sign == -1)
        likely = 3;
    else if (sign == 1)
        likely = 2;
    else if (sign == 2)
    {
        likely = 3;
        sign = 1;
    }
    else {
        std::cout << "wrong value of sign\n";
        exit(EXIT_FAILURE);
    }
    return val = val + sign * sgn(val) * sgn(rand() - RAND_MAX / likely) * DELTA_V;
}

float length (sf::Vector2f vector) {
    return sqrtf(powf(vector.x, 2) + powf(vector.y, 2));
}

float dot(sf::Vector2f a, sf::Vector2f b){
    return a.x * b.x + a.y * b.y;
}

#endif //GAME_DLC_H
