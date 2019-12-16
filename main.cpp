#include <QApplication>
#include <QDataStream>
#include "window.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    Window window;

    window.show();
    return app.exec();
}
