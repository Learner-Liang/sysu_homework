#include "player.h"
#include <algorithm>

Player::Player(int x, int y) 
    : x(x), y(y), health(MAX_HEALTH) {}

void Player::moveLeft() {
    x -= SPEED;
    if (x < 0) {
        x = 0;
    }
}

void Player::moveRight() {
    x += SPEED;
    if (x > 760) {  // 800 - WIDTH
        x = 760;
    }
}

void Player::setX(int x) {
    this->x = std::max(0, std::min(x, 760));
}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}

QRect Player::getRect() const {
    return QRect(x, y, WIDTH, HEIGHT);
}

int Player::getHealth() const {
    return health;
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool Player::isAlive() const {
    return health > 0;
}
