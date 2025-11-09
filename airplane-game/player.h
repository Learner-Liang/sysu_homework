#ifndef PLAYER_H
#define PLAYER_H

#include <QRect>

class Player {
public:
    Player(int x, int y);
    
    void moveLeft();
    void moveRight();
    void setX(int x);
    int getX() const;
    int getY() const;
    QRect getRect() const;
    
    int getHealth() const;
    void takeDamage(int damage);
    bool isAlive() const;
    
private:
    int x;
    int y;
    int health;
    static const int WIDTH = 40;
    static const int HEIGHT = 40;
    static const int SPEED = 10;
    static const int MAX_HEALTH = 100;
};

#endif // PLAYER_H
