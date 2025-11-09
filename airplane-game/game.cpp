#include "game.h"
#include <algorithm>
#include <random>

Game::Game(int width, int height) 
    : width(width), height(height), score(0), level(1), 
      frameCount(0), enemySpawnRate(60) {
    player = std::make_unique<Player>(width / 2 - 20, height - 80);
}

void Game::update() {
    if (isGameOver()) {
        return;
    }
    
    frameCount++;
    
    // Spawn enemies
    if (frameCount % enemySpawnRate == 0) {
        spawnEnemy();
    }
    
    // Update player
    // Player updates are handled by input
    
    // Update bullets
    for (auto& bullet : bullets) {
        bullet->update();
    }
    
    // Update enemies
    for (auto& enemy : enemies) {
        enemy->update();
    }
    
    // Remove inactive bullets
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const std::unique_ptr<Bullet>& b) { return !b->isActive(); }),
        bullets.end()
    );
    
    // Remove inactive enemies
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& e) { return !e->isActive(); }),
        enemies.end()
    );
    
    // Check collisions
    checkCollisions();
    
    // Update level
    updateLevel();
}

void Game::shoot() {
    if (!isGameOver()) {
        int playerX = player->getX();
        int playerY = player->getY();
        bullets.push_back(std::make_unique<Bullet>(playerX + 17, playerY));
    }
}

void Game::movePlayerLeft() {
    if (!isGameOver()) {
        player->moveLeft();
    }
}

void Game::movePlayerRight() {
    if (!isGameOver()) {
        player->moveRight();
    }
}

void Game::setPlayerX(int x) {
    if (!isGameOver()) {
        player->setX(x - 20);  // Center the player on the mouse
    }
}

Player* Game::getPlayer() {
    return player.get();
}

const std::vector<std::unique_ptr<Enemy>>& Game::getEnemies() const {
    return enemies;
}

const std::vector<std::unique_ptr<Bullet>>& Game::getBullets() const {
    return bullets;
}

int Game::getScore() const {
    return score;
}

int Game::getLevel() const {
    return level;
}

bool Game::isGameOver() const {
    return !player->isAlive();
}

void Game::spawnEnemy() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(0, width - 40);
    std::uniform_int_distribution<> typeDist(0, 2);
    
    int x = xDist(gen);
    Enemy::Type type = static_cast<Enemy::Type>(typeDist(gen));
    
    enemies.push_back(std::make_unique<Enemy>(x, 0, type));
}

void Game::checkCollisions() {
    // Check bullet-enemy collisions
    for (auto& bullet : bullets) {
        if (!bullet->isActive()) continue;
        
        for (auto& enemy : enemies) {
            if (!enemy->isActive()) continue;
            
            if (bullet->getRect().intersects(enemy->getRect())) {
                bullet->setActive(false);
                enemy->setActive(false);
                score += enemy->getScore();
                break;
            }
        }
    }
    
    // Check player-enemy collisions
    QRect playerRect = player->getRect();
    for (auto& enemy : enemies) {
        if (!enemy->isActive()) continue;
        
        if (playerRect.intersects(enemy->getRect())) {
            enemy->setActive(false);
            player->takeDamage(10);
        }
    }
}

void Game::updateLevel() {
    int newLevel = 1 + score / 100;
    if (newLevel > level) {
        level = newLevel;
        // Increase difficulty by spawning enemies more frequently
        enemySpawnRate = std::max(20, 60 - (level - 1) * 5);
    }
}
