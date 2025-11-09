#include "bullet.h"

Bullet::Bullet(int x, int y) : x(x), y(y), active(true) {}

void Bullet::update() {
    if (active) {
        y -= SPEED;
        if (y < 0) {
            active = false;
        }
    }
}

QRect Bullet::getRect() const {
    return QRect(x, y, WIDTH, HEIGHT);
}

bool Bullet::isActive() const {
    return active;
}

void Bullet::setActive(bool active) {
    this->active = active;
}
