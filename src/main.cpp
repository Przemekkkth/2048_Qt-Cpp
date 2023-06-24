#include <QApplication>
#include <QIcon>
#include "view.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    View v;
    v.setWindowTitle("2048 Qt/C++");
    v.show();
    a.exec();
}
