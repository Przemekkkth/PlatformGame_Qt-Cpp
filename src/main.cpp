#include <QApplication>
#include "view.h"
int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    View v;
    v.showFullScreen();
    v.setWindowTitle("Platform Game");
    a.exec();
}
