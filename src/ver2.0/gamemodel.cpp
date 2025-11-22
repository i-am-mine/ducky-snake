// gamemodel.cpp
#include "gamemodel.h"
#include <QRandomGenerator>

GameModel::GameModel(QObject *parent) : QObject(parent) {
    // srand(time(NULL)) không cần thiết, QRandomGenerator tốt hơn
    setupGame();
}

void GameModel::setupGame() {
    m_gameOver = false;
    m_dir = STOP;
    m_score = 0;
    m_snakeBody.clear();

    // Initialization
    QPoint head(width / 2, height / 2); // 1st pos is at the middle of gameframe
    m_snakeBody.push_back(head);

    spawnFruit();

    // Inform to View for score and state updates
    emit scoreUpdated(m_score);
    emit stateChanged();
}

void GameModel::spawnFruit() {
    do {
        m_fruitPos.setX(QRandomGenerator::global()->bounded(width));
        m_fruitPos.setY(QRandomGenerator::global()->bounded(height));
    } while (m_snakeBody.contains(m_fruitPos)); // check if fruit is not in the snake pos
}

void GameModel::advanceGame() {
    if (m_gameOver || m_dir == STOP) return;

    QPoint newHead = m_snakeBody.first();
    switch (m_dir) {
        case LEFT:  newHead.setX(newHead.x() - 1); break;
        case RIGHT: newHead.setX(newHead.x() + 1); break;
        case UP:    newHead.setY(newHead.y() - 1); break;
        case DOWN:  newHead.setY(newHead.y() + 1); break;
        case STOP:  return;
    }

    // 1. Check wall collision
    if (newHead.x() < 0 || newHead.x() >= width || newHead.y() < 0 || newHead.y() >= height) {
        m_gameOver = true;
        emit gameOver(); // Raise signal to game ending
        return;
    }

    // 2. Check snake-body collision (not including the snake-tail since of continuous snake move)
    for (int i = 0; i < m_snakeBody.size() - 1; ++i) {
        if (m_snakeBody.at(i) == newHead) {
            m_gameOver = true;
            emit gameOver();
            return;
        }
    }

    // 3. Move
    m_snakeBody.prepend(newHead);

    // 4. Check if fruit eating
    if (newHead == m_fruitPos) {
        m_score += 10;
        spawnFruit();
        emit scoreUpdated(m_score);
    } else {
        m_snakeBody.pop_back(); // Delete if not fruit eating
    }

    // 5. Signal to View for state change, then need to re-draw
    emit stateChanged();
}

void GameModel::setDirection(eDirection newDir) {
    // Add logic: cannot move backward
    if (m_dir == LEFT && newDir == RIGHT) return;
    if (m_dir == RIGHT && newDir == LEFT) return;
    if (m_dir == UP && newDir == DOWN) return;
    if (m_dir == DOWN && newDir == UP) return;

    m_dir = newDir;
}

// --- Getter functions ---
const QList<QPoint>& GameModel::getSnakeBody() const { return m_snakeBody; }
QPoint GameModel::getFruitPos() const { return m_fruitPos; }
int GameModel::getScore() const { return m_score; }
bool GameModel::isGameOver() const { return m_gameOver; }
