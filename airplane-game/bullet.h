#ifndef BULLET_H
#define BULLET_H

#include <QRect>

class Bullet {
public:
    Bullet(int x, int y);
    
    void update();
    QRect getRect() const;
    bool isActive() const;
    void setActive(bool active);
    
private:
    int x;
    int y;
    bool active;
    static const int WIDTH = 5;
    static const int HEIGHT = 10;
    static const int SPEED = 10;
};

#endif // BULLET_H
