#include "widget.h"
#include<QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Widget W;
    W.show();
    return app.exec();
}
