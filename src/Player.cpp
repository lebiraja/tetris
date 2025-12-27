#include "Player.h"

Player::Player() : x(40), y(10), health(100), speed(2) {}

void Player::moveLeft() {
    if (x - speed >= 1) {
        x -= speed;
    }
}

void Player::moveRight() {
    if (x + speed <= 79) {
        x += speed;
    }
}

void Player::moveUp() {
    if (y - speed >= 1) {
        y -= speed;
    }
}

void Player::moveDown() {
    if (y + speed <= 20) {
        y += speed;
    }
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

void Player::heal(int amount) {
    health += amount;
    if (health > 100) {
        health = 100;
    }
}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}

int Player::getHealth() const {
    return health;
}
