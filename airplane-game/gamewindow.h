#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <memory>
#include "game.h"

class GameWindow : public QWidget {
    Q_OBJECT
    
public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    
private slots:
    void gameLoop();
    
private:
    std::unique_ptr<Game> game;
    QTimer *timer;
    
    bool leftPressed;
    bool rightPressed;
    bool spacePressed;
    
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr int FPS = 60;
};

#endif // GAMEWINDOW_H
