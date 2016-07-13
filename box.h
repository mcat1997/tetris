#ifndef BOX_H
#define BOX_H

#include <QGraphicsObject>
#include <QGraphicsItemGroup>
//#include <QTimer>

class OneBox : public QGraphicsObject
{
public:
    OneBox(const QColor &color = Qt::red);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

private:
    QColor brushColor;

};


class BoxGroup : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    enum BoxShape {IShape, JShape, LShape, OShape, SShape, TShape, ZShape, RandomShape};//8�ж���˹������״
    BoxGroup();
    QRectF boundingRect() const;//�ں�������const��ʾ���ܸı���ĳ�Ա
    void clear_box_group(bool destroy_box = false);
    void create_box(const QPointF &point, BoxShape shape = RandomShape);//�ں��������������Խ������趨ΪĬ��ֵ�����崦����Ҫ
    bool isColliding();
    BoxShape getCurrentShape() {return current_shape;}//��õ�ǰ����˹�������״


protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void need_new_box();
    void game_finished();

public slots:
    void move_one_step();
    void startTimer(int interval);
    void stop_timer();

private:
    BoxShape current_shape;
    QTransform old_transform;
    QTimer *timer;

};

#endif // BOX_H
