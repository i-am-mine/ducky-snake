#pragma once

#include <QObject>
#include <QList>
#include <QPoint>

// MODEL class
class GameModel : public QObject {
    Q_OBJECT

public:
    enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };

    explicit GameModel(QObject *parent = nullptr);

    void setupGame(); // Start or Restart game

    // Helper functions for VIEW
    const QList<QPoint>& getSnakeBody() const;
    QPoint getFruitPos() const;
    int getScore() const;
    bool isGameOver() const;
    int getWidth() const { return width; }
    int getHeight() const { return height; }

public slots:
    // Slot to Controller (QTimer) for snake move
    void advanceGame();
    // Slot to Controller (GameView) for envoking when keypress event
    void setDirection(eDirection newDir);

signals:
    // Signal to View (Observer) for re-drawing
    void stateChanged();
    // Signal to View for updating scores
    void scoreUpdated(int score);
    // Signal to View for ending
    void gameOver();

private:
    void spawnFruit(); // Randomize fruit localisation
    void checkCollision();

    // Game setting
    const int width = 10;
    const int height = 10;
    int m_score;
    bool m_gameOver;
    eDirection m_dir;
    QList<QPoint> m_snakeBody; // snake container
    QPoint m_fruitPos;
};
