#ifndef ENEMY_H
#define ENEMY_H

#include <QRect>
#include <QColor>

class Enemy {
public:
    enum Type {
        RED,    // Slow, 10 points
        YELLOW, // Medium, 20 points
        PURPLE  // Fast, 30 points
    };
    
    Enemy(int x, int y, Type type);
    
    void update();
    QRect getRect() const;
    bool isActive() const;
    void setActive(bool active);
    Type getType() const;
    int getScore() const;
    QColor getColor() const;
    
private:
    int x;
    int y;
    Type type;
    bool active;
    int speed;
    int score;
    static constexpr int WIDTH = 40;
    static constexpr int HEIGHT = 40;
};

#endif // ENEMY_H
