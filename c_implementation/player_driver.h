#ifndef PLAYER_DRIVER_H
#define PLAYER_DRIVER_H


int is_same_move(int current) {
    static int previous = 0;
    int result = previous == current;
    previous = current;
    return result;
}


#endif // PLAYER_DRIVER_H
