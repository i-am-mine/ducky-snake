#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsTextItem>
#include "gamemodel.h"

class GameView : public QGraphicsView {
    Q_OBJECT

public:
    // View gets the pointer to Model when initialization
    explicit GameView(GameModel *model, QWidget *parent = nullptr);

protected:
    // --- Controller Part ---
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    // --- View Part (Slots cho Observer) ---
    void updateView();  // Call when Model sends a signal stateChanged()
    void updateScoreDisplay(int score);  // Call when Model sends a signal scoreUpdated()
    void showGameOverScreen();  // Call when Model sends a signal gameOver()

private:
    void setupView(); // Scene setup

    GameModel *m_model; // Model pointer
    QGraphicsScene *m_scene; // Scene
    QTimer *m_gameTimer; // Game controller

    // Graphic objects
    QGraphicsRectItem *m_fruitItem;
    QList<QGraphicsRectItem*> m_snakeItems;
    QGraphicsTextItem *m_scoreText;

    // GameOver objects
    QGraphicsRectItem *m_gameOverOverlay;
    QGraphicsTextItem *m_gameOverText;

    const int TILE_SIZE = 30; // Pixel size
};
