#include "enemy.h"

Enemy::Enemy(int x, int y, Type type) 
    : x(x), y(y), type(type), active(true) {
    switch (type) {
        case RED:
            speed = 2;
            score = 10;
            break;
        case YELLOW:
            speed = 4;
            score = 20;
            break;
        case PURPLE:
            speed = 6;
            score = 30;
            break;
    }
}

void Enemy::update() {
    if (active) {
        y += speed;
        if (y > 600) {
            active = false;
        }
    }
}

QRect Enemy::getRect() const {
    return QRect(x, y, WIDTH, HEIGHT);
}

bool Enemy::isActive() const {
    return active;
}

void Enemy::setActive(bool active) {
    this->active = active;
}

Enemy::Type Enemy::getType() const {
    return type;
}

int Enemy::getScore() const {
    return score;
}

QColor Enemy::getColor() const {
    switch (type) {
        case RED:
            return QColor(255, 0, 0);
        case YELLOW:
            return QColor(255, 255, 0);
        case PURPLE:
            return QColor(128, 0, 128);
        default:
            return QColor(255, 0, 0);
    }
}
