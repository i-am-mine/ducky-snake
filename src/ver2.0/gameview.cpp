// gameview.cpp
#include "gameview.h"
#include <QKeyEvent>
#include <QBrush>
#include <QPen>

GameView::GameView(GameModel *model, QWidget *parent)
    : QGraphicsView(parent), m_model(model) {

    m_scene = new QGraphicsScene(this);
    setScene(m_scene);

    // Controller: Timer cho game loop
    m_gameTimer = new QTimer(this);
    m_gameTimer->start(500);    // Game speed (ms)

    setupView();

    // --- MVC connection ---

    // 1. Controller -> Model: Timer calls logic game
    connect(m_gameTimer, &QTimer::timeout, m_model, &GameModel::advanceGame);

    // 2. Model -> View (Observer): Model is changed, View is then updated
    connect(m_model, &GameModel::stateChanged, this, &GameView::updateView);
    connect(m_model, &GameModel::scoreUpdated, this, &GameView::updateScoreDisplay);
    connect(m_model, &GameModel::gameOver, this, &GameView::showGameOverScreen);
}

void GameView::setupView() {
    // Scene size initialization (based on Model)
    int w = m_model->getWidth() * TILE_SIZE;
    int h = m_model->getHeight() * TILE_SIZE;
    m_scene->setSceneRect(0, 0, w, h);

    // View setup
    setRenderHint(QPainter::Antialiasing);
    setBackgroundBrush(QBrush(QColor(140, 190, 70))); // Ground color (grass)
    setMinimumSize(w + 2, h + 2); // Min windows size

    // Create fruit
    m_fruitItem = new QGraphicsRectItem(0, 0, TILE_SIZE, TILE_SIZE);
    m_fruitItem->setBrush(Qt::red); // Fruit color
    m_fruitItem->setPen(Qt::NoPen);
    m_scene->addItem(m_fruitItem);

    // Create score display
    m_scoreText = new QGraphicsTextItem();
    m_scene->addItem(m_scoreText);
    m_scoreText->setPos(5, 5);
    m_scoreText->setDefaultTextColor(Qt::white);
    m_scoreText->setFont(QFont("Arial", 16, QFont::Bold));
    updateScoreDisplay(m_model->getScore());

    // Create GameOver scene (hidden)
    m_gameOverOverlay = new QGraphicsRectItem(0, 0, w, h);
    m_gameOverOverlay->setBrush(QColor(0, 0, 0, 150)); // Matte black coating
    m_gameOverText = new QGraphicsTextItem("GAME OVER! 🦆\nPress 'R' to Restart"); //
    m_gameOverText->setDefaultTextColor(Qt::red);
    m_gameOverText->setFont(QFont("Arial", 24, QFont::Bold));
    m_gameOverText->setPos((w - m_gameOverText->boundingRect().width()) / 2, (h - m_gameOverText->boundingRect().height()) / 2);

    m_scene->addItem(m_gameOverOverlay);
    m_scene->addItem(m_gameOverText);
    m_gameOverOverlay->hide();
    m_gameOverText->hide();

    updateView(); // Draw initial scene
}

// Slot: Update state
void GameView::updateView() {
    // 1. Delete the old snake Xóa các ô rắn cũ
    for(auto item : m_snakeItems) {
        m_scene->removeItem(item);
        delete item;
    }
    m_snakeItems.clear();

    // 2. Draw a new snake from Model data
    const auto& snakeBody = m_model->getSnakeBody();
    if (snakeBody.isEmpty()) return;

    // Draw DuckHead Vẽ đầu (Mother Duck 🦆)
    QPoint headPos = snakeBody.first();
    QGraphicsRectItem *headItem = new QGraphicsRectItem(headPos.x() * TILE_SIZE, headPos.y() * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    headItem->setBrush(Qt::yellow); // DuckHead color
    headItem->setPen(Qt::NoPen);
    m_scene->addItem(headItem);
    m_snakeItems.append(headItem);

    // Draw snake-body (Ducklings 🐥)
    for (int i = 1; i < snakeBody.size(); ++i) {
        QPoint bodyPos = snakeBody.at(i);
        QGraphicsRectItem *bodyItem = new QGraphicsRectItem(bodyPos.x() * TILE_SIZE, bodyPos.y() * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        bodyItem->setBrush(QColor(255, 255, 150)); // Ducklings color
        bodyItem->setPen(Qt::NoPen);
        m_scene->addItem(bodyItem);
        m_snakeItems.append(bodyItem);
    }

    // 3. Update new fruit pos
    QPoint fruitPos = m_model->getFruitPos();
    m_fruitItem->setPos(fruitPos.x() * TILE_SIZE, fruitPos.y() * TILE_SIZE);
}

// Slot: Update score
void GameView::updateScoreDisplay(int score) {
    m_scoreText->setPlainText(QString("Score: %1").arg(score));
}

// Slot: Display Game Over
void GameView::showGameOverScreen() {
    m_gameTimer->stop(); // End game
    m_gameOverOverlay->show();
    m_gameOverText->show();
    m_gameOverOverlay->setZValue(1); // Make sure the coating is on top
    m_gameOverText->setZValue(2);    // Display text on the very top layer
}

// --- Controller: handle events ---
void GameView::keyPressEvent(QKeyEvent *event) {
    // If GameOver, only get R or Q keypress
    if (m_model->isGameOver()) {
        if (event->key() == Qt::Key_R) {
            m_gameOverOverlay->hide();
            m_gameOverText->hide();
            m_model->setupGame(); // Restart Model
            m_gameTimer->start(); // Reset Timer
        } else if (event->key() == Qt::Key_Q) {
            close();
        }
        return;
    }

    // ControlKey: W, A, S, D
    switch (event->key()) {
        case Qt::Key_W:
        case Qt::Key_Up:
            m_model->setDirection(GameModel::UP);
            break;
        case Qt::Key_A:
        case Qt::Key_Left:
            m_model->setDirection(GameModel::LEFT);
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            m_model->setDirection(GameModel::DOWN);
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            m_model->setDirection(GameModel::RIGHT);
            break;
        case Qt::Key_X: // X for exit
            close();
            break;
        default:
            QGraphicsView::keyPressEvent(event);
    }
}

// Make sure the scene is fit with window
void GameView::resizeEvent(QResizeEvent *event) {
    fitInView(sceneRect(), Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(event);
}
