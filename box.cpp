#include "box.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>


//OneBox�Ǵ�QGraphicsObject�̳ж�����
OneBox::OneBox(const QColor &color) : brushColor(color) {

}


//�ú���Ϊָ������Ļ�ͼ�������߿�
QRectF OneBox::boundingRect() const {

    qreal pen_width = 1;
    //С����ı߳�Ϊ20.5����
    return QRectF(-10-pen_width/2, -10-pen_width/2, 20+pen_width, 20+pen_width);

}

void OneBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    //��ͼ�����������ʸ�,���򵥶�����ɫȥ������о���Щ������ɫ�ܵ�һ
    painter->drawPixmap(-10, -10, 20, 20, QPixmap(":/images/box.gif"));
    painter->setBrush(brushColor);//���û�ˢ��ɫ
    QColor penColor = brushColor;
    penColor.setAlpha(20);//����ɫ��͸���ȼ�С��ʹ����߽�����ɫֱ�������ֿ�
    painter->setPen(penColor);//ɫ���ƻ���
    //���ﻭ���ο򣬿�����䲿���û�ˢ�������������û��ʻ�
    painter->drawRect(-10, -10, 20, 20);//�����ο�
}


//�ھֲ�������Ϸ���item��shape,���Ǻ���û�������ط������˸ú���
QPainterPath OneBox::shape() const{

    //QPainterPath��һ����ͼ����������
    QPainterPath path;
    path.addRect(-9.5, -9.5, 19, 19);
    return path;
}


//BoxGroup�Ǵ�QGraphicsItemGroup��QObject�̳ж�����
BoxGroup::BoxGroup() {

    setFlags(QGraphicsItem::ItemIsFocusable);//�����������뽹��
    old_transform = transform();//���ص�ǰitem�ı任����,��BoxGroup������ת�󣬿���ʹ���������лָ�
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move_one_step()));
    current_shape = RandomShape;
}


QRectF BoxGroup::boundingRect() const {

    qreal pen_width = 1;
    return QRectF(-40-pen_width/2, -40-pen_width/2, 80+pen_width, 80+pen_width);//2*2��С�������һ��С������
}


void BoxGroup::keyPressEvent(QKeyEvent *event) {

    switch(event->key())
    {
        //���¼�λ׹���
        case Qt::Key_Down:
            moveBy(0, 20);//moveBy��ϵͳ�Դ��ĺ���������Ҫ�����Լ�ȥʵ��
            while(!isColliding()) {
                moveBy(0, 20);
            }
            moveBy(0, -20);//������
            clear_box_group();//����ײ���ͽ���ǰ�������4��item�Ƴ�,�����ٷ�����
            emit need_new_box();//�����źţ���MyView�н���
            break;
        case Qt::Key_Left:
            moveBy(-20, 0);
            if(isColliding()) {
                moveBy(20, 0);
            }
            break;
        case Qt::Key_Right:
            moveBy(20, 0);
            if(isColliding()) {
                moveBy(-20, 0);
            }
            break;
         //ʵ��С���������
         case Qt::Key_Space:
            rotate(90);//���������תҲ����Ҫ�Լ�ʵ�֣�Qt�����Դ��÷���
            if(isColliding())
                rotate(-90);//���κ���ײ�ˣ���������λ�ȥ
            break;
    }
}


//����Ƿ�����ײ
bool BoxGroup::isColliding() {

    QList<QGraphicsItem *> item_list = childItems();//������item�б�
    QGraphicsItem *item;
    foreach(item, item_list) {
        if(item->collidingItems().count()>1)//collidingItems�����뵱ǰitem��ײ����item�б�
            return true;//����������һ��item��������ײ
    }
    return false;
}


//�����������ͼ���Ƴ������������Ҫ(������Ϊtrue�������)�����ٵ�
//�䱾���ǽ����е�С����ӷ��������Ƴ������ﵽ����ͼ�н��������Ƴ���Ŀ��
void BoxGroup::clear_box_group(bool destroy_box) {

    QList<QGraphicsItem *> item_list = childItems();
    QGraphicsItem *item;
    foreach(item, item_list) {
        removeFromGroup(item);//��item�ӷ��������Ƴ���
        if(destroy_box) {
            OneBox *box = (OneBox*)item;
            box->deleteLater();//�����Ʒ��ص��¼�ѭ��ʱ����Ŀ�걻ɾ����������
        }
    }
}


//��������˹�����飬������״����ѡ�񷽿������ɫ����״
void BoxGroup::create_box(const QPointF &point, BoxShape shape) {

    static const QColor color_table[7] = {
        QColor(200, 0, 0, 100), QColor(255, 200, 0, 100), QColor(0, 0, 200, 100),
        QColor(0, 200, 0, 100), QColor(0, 200, 255, 100), QColor(200, 0, 255, 100),
        QColor(150, 100, 100, 100)
    };
    int shape_id = shape; //Box_Shape��ö���ͣ���ʵҲ�����ͣ���Ϊ���൱�����͵ĺ궨��
    if(shape == RandomShape) {
        shape_id = qrand()%7;//���ȡһ����ɫ
    }
    QColor color = color_table[shape_id];//����idѡ��ɫ
    QList<OneBox *> list;
    setTransform(old_transform);//�ָ�������ǰ�ı任����
    for(int i = 0; i < 4; ++i) { //4��С�������һ��������
        OneBox *temp  = new OneBox(color);
        list << temp;//��С�������list�б�
        addToGroup(temp);
    }
    switch(shape_id) {
        case IShape:
        current_shape = IShape;//���ŵ�һ��
        list.at(0)->setPos(-30, -10);
        list.at(1)->setPos(-10, -10);
        list.at(2)->setPos(10, -10);
        list.at(3)->setPos(30, -10);
        break;
        case JShape:
        current_shape = JShape;//J��
        list.at(0)->setPos(10, -10);
        list.at(1)->setPos(10, 10);
        list.at(2)->setPos(10, 30);
        list.at(3)->setPos(-10, 30);
        break;
        case LShape:
        current_shape = LShape;//L�͵ķ�����
        list.at(0)->setPos(-10, -10);
        list.at(1)->setPos(-10, 10);
        list.at(2)->setPos(-10, 30);
        list.at(3)->setPos(10, 30);
        break;
        case OShape://������
        current_shape = OShape;
        list.at(0)->setPos(-10, -10);
        list.at(1)->setPos(10, -10);
        list.at(2)->setPos(-10, 10);
        list.at(3)->setPos(10, 10);
        break;
        case SShape://S��
        current_shape = SShape;
        list.at(0)->setPos(10, -10);
        list.at(1)->setPos(30, -10);
        list.at(2)->setPos(-10, 10);
        list.at(3)->setPos(10, 10);
        break;
        case TShape: //������
        current_shape = TShape;
        list.at(0)->setPos(-10, -10);
        list.at(1)->setPos(10, -10);
        list.at(2)->setPos(30, -10);
        list.at(3)->setPos(10, 10);
        break;
        case ZShape://Z����
        current_shape = ZShape;
        list.at(0)->setPos(-10, -10);
        list.at(1)->setPos(10, -10);
        list.at(2)->setPos(10, 10);
        list.at(3)->setPos(30, 10);
        break;
        default: break;
    }
    setPos(point);//��׼���õĶ���˹�������ָ����λ�ã�Ȼ�������ײ���
    if(isColliding()) {
        //�������˹����һ���ֺ�ͷ�������ײ����Ϊ���Ǵ��м�����ģ�����һ��ʼ�����������������߷�����ײ��
        //ֻ������������ײ����������������ײ��˵����Ϸ�Ѿ��������Ϳ��Է�����Ϸ�����ź��ˣ��Ҷ�ʱ��ֹͣ��
        stop_timer();
        emit game_finished();
    }
}


//�����ϵͳ��ĺ�������������������������������
//��ʵ�Ǹ������е�timeEvent()�������õ�
void BoxGroup::startTimer(int interval) {
    timer->start(interval);//������ʱ���������ö�ʱ�������Ȼ����BoxGroup()�Ĺ��캯���������˸ö�ʱ�����ź���ۺ���
}


//ÿ����ʱ����ʱ���ˣ�С�������������һ��
void BoxGroup::move_one_step() {
    moveBy(0, 20);//�ú����Ǹ���ĺ���������ָ�����ƶ�һ����λ����Ϊ����Ϊ������
    if(isColliding()) {//������ײ�������
        moveBy(0, -20);
        clear_box_group();//���������Ƴ���ͼ
        emit need_new_box();//�����ź�֪ͨ������Ҫ�µķ��������
    }
}



void BoxGroup::stop_timer() {

    timer->stop();//��ʱ��ֹͣ
}


