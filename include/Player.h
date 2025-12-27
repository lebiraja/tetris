#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
    int x;
    int y;
    int health;
    int speed;

public:
    Player();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void takeDamage(int damage);
    void heal(int amount);
    
    int getX() const;
    int getY() const;
    int getHealth() const;
};

#endif
