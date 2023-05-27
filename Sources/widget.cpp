#include "widget.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QtDebug>

Widget::Widget(QWidget *parent) : QWidget(parent) {
  qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime())); // 种下产生随机数的种子
  resize(1400, 800);
  array.append(position(160, 100));
  array.append(position(140, 100));
  array.append(position(120, 100));
  food = false;
  QTimer *time = new QTimer(this);
  time->start(600);
  connect(time, &QTimer::timeout, [=]() {
    if (key == Qt::Key_Up) {
      if (fruit.x * 20 == array[0].x && fruit.y * 20 == array[0].y - 20) {
          array.push_front(position(fruit.x * 20, fruit.y * 20));
          food = false;
      } else {     
      position tem;
      tem.x = array[0].x;
      tem.y = array[0].y - 20;
      array.push_front(tem);
      array.pop_back();
      }
    } else if (key == Qt::Key_Down) {
      if (fruit.x * 20 == array[0].x && fruit.y * 20 == array[0].y + 20) {
          array.push_front(position(fruit.x * 20, fruit.y * 20));
          food = false;
      } else {
        position tem;
        tem.x = array[0].x;
        tem.y = array[0].y + 20;
        array.push_front(tem);
        array.pop_back();
      }
    } else if (key == Qt::Key_Left) {
      if (fruit.x * 20 == array[0].x - 20 && fruit.y * 20 == array[0].y) {
          array.push_front(position(fruit.x * 20, fruit.y * 20));
          food = false;
      } else {
        position tem;
        tem.x = array[0].x - 20;
        tem.y = array[0].y;
        array.push_front(tem);
        array.pop_back();
      }
    } else {
      if (fruit.x * 20 == array[0].x + 20 && fruit.y * 20 == array[0].y) {
          array.push_front(position(fruit.x * 20, fruit.y * 20));
          food = false;
      } else {      
        position tem;
        tem.x = array[0].x + 20;
        tem.y = array[0].y;
        array.push_front(tem);
        array.pop_back();
      }
    }
    update();
  });

  connect(time, &QTimer::timeout, [=]() {
    if (food == false) {
      while (1) {
        fruit.x = qrand() % 50;
        fruit.y = qrand() % 40;
        for (int i = 0; i < array.size(); i++) {
          if (fruit.x == array[i].x && fruit.y == array[i].y) {
            continue;
          }
        }
        food = true;
        break;
      }
    }
  });
}

Widget::~Widget() {}

void Widget::keyPressEvent(QKeyEvent *event) {

  qDebug() << "这里被调用了";
  switch (event->key()) {

  case Qt::Key_Up:
    // 向上移动操作
    if (key == Qt::Key_Down) {
      qDebug() << "方向按错了";
    } else if (key == Qt::Key_Up) {
      qDebug() << "重复按下";
    } else {
      key = Qt::Key_Up; // 记录行进方向
    }
    break;

  case Qt::Key_Down:
    if (key == Qt::Key_Up) {
      qDebug() << "方向按错了";
    } else if (key == Qt::Key_Down) {
      qDebug() << "重复按下";
    } else {
      key = Qt::Key_Down; // 记录行进方向
    }
    break;

  case Qt::Key_Left:
    if (key == Qt::Key_Right) {
      qDebug() << "方向按错了";
    } else if (key == Qt::Key_Left) {
      qDebug() << "重复按下";
    } else {
      key = Qt::Key_Left; // 记录行进方向
    }
    break;
  case Qt::Key_Right:
    if (key == Qt::Key_Left) {
      qDebug() << "方向按错了";
    } else if (key == Qt::Key_Right) {
      qDebug() << "重复按下";
    } else {
      key = Qt::Key_Right; // 记录行进方向
    }
    break;
  }
}
// 记住是 paintEvent
void Widget::paintEvent(QPaintEvent *)

{
  QPainter painter(this);
  QPixmap pix;
  pix.load("../resourseFile/head.jpg");
  for (int i = 0; i < array.size(); i++) {
    qDebug() << array[i].x << ' ' << array[i].y;
    painter.drawPixmap(array[i].x, array[i].y, 20, 20, pix);
  }

  if (food == true) {
    qDebug() << "food: " << fruit.x * 20 << ' ' << fruit.y * 20;
    painter.drawPixmap(fruit.x * 20, fruit.y * 20, 20, 20, pix);
  }
}
