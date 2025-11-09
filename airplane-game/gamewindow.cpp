#include "gamewindow.h"
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent), leftPressed(false), rightPressed(false), spacePressed(false) {
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowTitle("Airplane Game");
    setMouseTracking(true);
    
    game = std::make_unique<Game>(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::gameLoop);
    timer->start(1000 / FPS);
}

GameWindow::~GameWindow() {
    // QTimer will be automatically deleted as it's a child of this widget
}

void GameWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    
    // Draw background
    painter.fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Qt::black);
    
    if (game->isGameOver()) {
        // Draw game over screen
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 30));
        painter.drawText(rect(), Qt::AlignCenter, "GAME OVER");
        
        painter.setFont(QFont("Arial", 20));
        painter.drawText(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 50, 
                        QString("Final Score: %1").arg(game->getScore()));
        return;
    }
    
    // Draw player (green square)
    Player* player = game->getPlayer();
    if (player) {
        painter.fillRect(player->getRect(), Qt::green);
    }
    
    // Draw enemies (colored squares)
    for (const auto& enemy : game->getEnemies()) {
        if (enemy->isActive()) {
            painter.fillRect(enemy->getRect(), enemy->getColor());
        }
    }
    
    // Draw bullets (white rectangles)
    for (const auto& bullet : game->getBullets()) {
        if (bullet->isActive()) {
            painter.fillRect(bullet->getRect(), Qt::white);
        }
    }
    
    // Draw HUD
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14));
    painter.drawText(10, 20, QString("Score: %1").arg(game->getScore()));
    painter.drawText(10, 40, QString("Level: %1").arg(game->getLevel()));
    painter.drawText(10, 60, QString("Health: %1").arg(player->getHealth()));
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Left) {
        leftPressed = true;
    } else if (event->key() == Qt::Key_Right) {
        rightPressed = true;
    } else if (event->key() == Qt::Key_Space && !event->isAutoRepeat()) {
        spacePressed = true;
        game->shoot();
    }
}

void GameWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Left) {
        leftPressed = false;
    } else if (event->key() == Qt::Key_Right) {
        rightPressed = false;
    } else if (event->key() == Qt::Key_Space && !event->isAutoRepeat()) {
        spacePressed = false;
    }
}

void GameWindow::mouseMoveEvent(QMouseEvent *event) {
    if (!game->isGameOver()) {
        game->setPlayerX(event->pos().x());
    }
}

void GameWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        game->shoot();
    }
}

void GameWindow::gameLoop() {
    // Handle continuous key presses
    if (leftPressed) {
        game->movePlayerLeft();
    }
    if (rightPressed) {
        game->movePlayerRight();
    }
    
    // Update game state
    game->update();
    
    // Repaint
    update();
}
