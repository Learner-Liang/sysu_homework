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
    static constexpr int WIDTH = 5;
    static constexpr int HEIGHT = 10;
    static constexpr int SPEED = 10;
};

#endif // BULLET_H
