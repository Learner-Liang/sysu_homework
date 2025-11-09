#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include "player.h"
#include "enemy.h"
#include "bullet.h"

class Game {
public:
    Game(int width, int height);
    
    void update();
    void shoot();
    void movePlayerLeft();
    void movePlayerRight();
    void setPlayerX(int x);
    
    Player* getPlayer();
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const;
    const std::vector<std::unique_ptr<Bullet>>& getBullets() const;
    
    int getScore() const;
    int getLevel() const;
    bool isGameOver() const;
    
private:
    void spawnEnemy();
    void checkCollisions();
    void updateLevel();
    
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Bullet>> bullets;
    
    int width;
    int height;
    int score;
    int level;
    int frameCount;
    int enemySpawnRate;
};

#endif // GAME_H
