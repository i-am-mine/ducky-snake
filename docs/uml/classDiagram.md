```mermaid
classDiagram
    direction TB
    class main {
        +main(argc, argv)
    }
    class QApplication {
        note right: External (Qt)
    }
    class GameModel {
        note right: QObject subclass (Qt)
        - m_snakeBody: QList<QPoint>
        - m_fruitPos: QPoint
        - m_score: int
        - m_dir: eDirection
        - m_gameOver: bool
        + stateChanged()
        + scoreUpdated(int)
        + gameOver()
        + advanceGame()
        + setDirection(eDirection)
        + setupGame()
    }
    class GameView {
        note right: QGraphicsView subclass (Qt)
        - m_model: GameModel*
        - m_scene: QGraphicsScene*
        - m_gameTimer: QTimer*
        - updateView()
        - updateScoreDisplay(int)
        - showGameOverScreen()
        # keyPressEvent(QKeyEvent*)
    }
    class QObject
    class QGraphicsView
    class QGraphicsScene
    class QTimer
    QObject <|-- GameModel : inherits
    QGraphicsView <|-- GameView : inherits
    GameView o-- GameModel : "m_model (holds reference)"
    GameView *-- QGraphicsScene : "m_scene (owns)"
    GameView *-- QTimer : "m_gameTimer (owns)"
    main ..> QApplication : "creates"
    main ..> GameModel : "creates"
    main ..> GameView : "creates"
    QTimer ..> GameModel : "Flow 1 (Controller) connect(timeout(), advanceGame())"
    GameModel ..> GameView : "Flow 1 (View) connect(stateChanged(), updateView())"
    GameView ..> GameModel : "Flow 2 (Input) keyPressEvent() calls setDirection()"
