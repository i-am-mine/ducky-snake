#include <QApplication>
#include "gamemodel.h"
#include "gameview.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // MODEL initialization
    GameModel model;

    // VIEW initialization
    GameView view(&model);

    // Screen setup
    view.setWindowTitle("Ducky Snake 🦆");

    // Start
    view.show();

    return app.exec();
}
