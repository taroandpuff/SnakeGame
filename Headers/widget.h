#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QVector>
#include <QWidget>
#include <QMessageBox>
#include <QApplication>

class position {
public:
    position(int x = 0, int y = 0) {
        this->x = x;
        this->y = y;
    }

    int x;
    int y;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QVector<position> array;

    void keyPressEvent(QKeyEvent *event);//键盘按下事件
    void  paintEvent(QPaintEvent *);//自动调用
    Qt::Key key;//蛇行进方向
    int food; // 食物
    position fruit;
};
#endif // MAINWIDGET_H
