#include "myview.h"
#include "box.h"
#include <QIcon>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#include <QTimer>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QApplication>
#include <QLabel>
#include <QFileInfo>


static const qreal INITSSPEED = 800;//��Ϸ�ĳ�ʼ���ٶ�
static const QString SOUNDPATH = "./sounds/";

MyView::MyView(QWidget *parent) :
    QGraphicsView(parent)
{
    init_view();
}

void MyView::init_view() {

    setRenderHint(QPainter::Antialiasing);//ʹ�ÿ���ݵķ�ʽ��Ⱦ
    setCacheMode(CacheBackground);//���û��汳�����������Լӿ���Ⱦ�ٶ�
    setWindowTitle(tr("����˹����"));
    setWindowIcon(QIcon(":/images/icon.png"));//���ñ��⴦��ͼ��
    setMinimumSize(810, 510);  //2�����ó�һ��˵����ͼ�ߴ粻���ٸ���
    setMaximumSize(810, 510);
    QGraphicsScene *scene = new QGraphicsScene;//�½�����ָ��
    scene->setSceneRect(5, 5, 800, 500);//������С
    scene->setBackgroundBrush(QPixmap(":/images/background.png"));
    setScene(scene);//���ó���

    //����˹������ƶ���������4����,�����Ԥ��3��������Ϊ�˷��������ײ���
    top_line = scene->addLine(197, 47, 403, 47);
    bottom_line = scene->addLine(197, 453, 403, 453);
    left_line = scene->addLine(197, 47, 197, 453);
    right_line = scene->addLine(403, 47, 403, 453);

    //��ӵ�ǰ������
    box_group = new BoxGroup;//ͨ���½�BoxGroup�����Ӵﵽ����box��2����
    connect(box_group, SIGNAL(need_new_box()), this, SLOT(clear_full_rows()));
    connect(box_group, SIGNAL(game_finished()), this, SLOT(game_over()));
    scene->addItem(box_group);

    //�����ʾ������
    next_box_group = new BoxGroup;
    scene->addItem(next_box_group);

    game_score = new QGraphicsTextItem(0, scene);//�ı��ĸ�itemΪ��Ӧ�ĳ���
    game_score->setFont(QFont("Times", 50, QFont::Bold));//Ϊ�ı���������
    game_score->setPos(450, 350);//�����ڳ����г��ֵ�λ��

    game_level = new QGraphicsTextItem(0, scene);
    game_level->setFont(QFont("Times", 50, QFont::Bold));
    game_level->setPos(20, 150);

    //��ʼ��Ϸ
 //   start_game();
    //��ʼ״̬ʱ����ʾ��Ϸ���򣬲���ʾ��Ϸ��������Ϸ�ȼ�
    top_line->hide();
    bottom_line->hide();
    left_line->hide();
    right_line->hide();
    game_score->hide();
    game_level->hide();

    //��Ӻ�ɫ����
    QWidget *mask = new QWidget;
    mask->setAutoFillBackground(true);
    mask->setPalette(QPalette(QColor(0, 0, 0, 50)));//alphaΪ��͸����
    mask->resize(900, 600);
    //addWidget()�����ķ���ֵ��QGraphicsProxyWidget������������Ӧ��ͷ�ļ�����˴��ᱨ��
    mask_widget = scene->addWidget(mask);
    mask_widget->setPos(-50, -50);
    mask_widget->setZValue(1);//�ò㱡ɴ����ԭͼ�����棬�����е�������opengl�е�3ά��ͼ

    //ѡ�����
    QWidget *option = new QWidget;
    //���رհ�ť����option��
    QPushButton *option_close_button = new QPushButton(tr("��  ��"), option);//��2������Ϊ��ť���ڵ�widget
    //���ð�ť��������ɫ�ǰ�ɫ
    QPalette palette;
    palette.setColor(QPalette::ButtonText, Qt::black);//��һ��������ɫ���role������ָ���ǰ�ť������ɫ
    option_close_button->setPalette(palette);
    //���ùرհ�ť��λ�ã��͵��������Ӧ
    option_close_button->move(120, 300);
    connect(option_close_button, SIGNAL(clicked()), option, SLOT(hide()));//��������ʧ

    option->setAutoFillBackground(true);
    option->setPalette(QPalette(QColor(0, 0, 0, 180)));
    option->resize(300, 400);
    QGraphicsWidget *option_widget = scene->addWidget(option);
    option_widget->setPos(250, 50);
    option_widget->setZValue(3);
    option_widget->hide();


    //�������
    QWidget *help = new QWidget;
    QPushButton *help_close_button = new QPushButton(tr("��  ��"), help);
    help_close_button->setPalette(palette);
    help_close_button->move(120, 300);
    connect(help_close_button, SIGNAL(clicked()), help, SLOT(hide()));

    help->setAutoFillBackground(true);
    help->setPalette(QPalette(QColor(0, 0, 0, 180)));
    help->resize(300, 400);
    QGraphicsWidget *help_widget = scene->addWidget(help);
    help_widget->setPos(250, 50);
    help_widget->setZValue(3);
    help_widget->hide();

    //��Ϸ��ӭ�ı�
    game_welcome_text = new QGraphicsTextItem(0, scene);//��һ������Ϊ�ı����ݣ��ڶ�������Ϊ��item
    game_welcome_text->setHtml(tr("<font color=green>����˹����</font>"));
    game_welcome_text->setFont(QFont("Times", 40, QFont::Bold));
    game_welcome_text->setPos(300, 100);
    game_welcome_text->setZValue(2);//���ڵ�2��

    //��Ϸ��ͣ�ı�
    game_pause_text = new QGraphicsTextItem(0, scene);//��һ������Ϊ�ı����ݣ��ڶ�������Ϊ��item
    game_pause_text->setHtml(tr("<font color=green>��Ϸ��ͣ�У�</font>"));
    game_pause_text->setFont(QFont("Times", 40, QFont::Bold));
    game_pause_text->setPos(300, 100);
    game_pause_text->setZValue(2);//���ڵ�2��
    game_pause_text->hide();

    //��Ϸ�����ı�
    game_over_text = new QGraphicsTextItem(0, scene);//��һ������Ϊ�ı����ݣ��ڶ�������Ϊ��item
    game_over_text->setHtml(tr("<font color=green>GAME��OVER��</font>"));
    game_over_text->setFont(QFont("Times", 40, QFont::Bold));
    game_over_text->setPos(300, 100);
    game_over_text->setZValue(2);//���ڵ�2��
    game_over_text->hide();

    // ��Ϸ��ʹ�õİ�ť

    QPushButton *button1 = new QPushButton(tr("��    ʼ"));
    QPushButton *button2 = new QPushButton(tr("ѡ    ��"));
    QPushButton *button3 = new QPushButton(tr("��    ��"));
    QPushButton *button4 = new QPushButton(tr("��    ��"));
    QPushButton *button5 = new QPushButton(tr("���¿�ʼ"));
    QPushButton *button6 = new QPushButton(tr("��    ͣ"));
    QPushButton *button7 = new QPushButton(tr("�� �� ��"));
    QPushButton *button8 = new QPushButton(tr("������Ϸ"));
    QPushButton *button9 = new QPushButton(tr("������Ϸ"));

    connect(button1, SIGNAL(clicked()), this, SLOT(start_game()));
    connect(button2, SIGNAL(clicked()), option, SLOT(show()));
    connect(button3, SIGNAL(clicked()), help, SLOT(show()));
    connect(button4, SIGNAL(clicked()), qApp, SLOT(quit()));//�˴��ۺ����Ľ��ն���ΪӦ�ó�����
    connect(button5, SIGNAL(clicked()), this, SLOT(restart_game()));
    connect(button6, SIGNAL(clicked()), this, SLOT(pause_game()));
    connect(button7, SIGNAL(clicked()), this, SLOT(finish_game()));
    connect(button8, SIGNAL(clicked()), this, SLOT(return_game()));//�������˵�
    connect(button9, SIGNAL(clicked()), this, SLOT(finish_game()));

    start_button = scene->addWidget(button1);//restart_button������QPushbutton���ͣ�����QGraphicsItem����,���������
    option_button = scene->addWidget(button2);
    help_button = scene->addWidget(button3);
    exit_button = scene->addWidget(button4);
    restart_button = scene->addWidget(button5);
    pause_button = scene->addWidget(button6);
    show_menu_button = scene->addWidget(button7);
    return_button = scene->addWidget(button8);
    finish_button = scene->addWidget(button9);

    //����λ��
    start_button->setPos(370, 200);
    option_button->setPos(370, 250);
    help_button->setPos(370, 300);
    exit_button->setPos(370, 350);
    restart_button->setPos(600, 150);
    pause_button->setPos(600, 200);
    show_menu_button->setPos(600, 250);
    return_button->setPos(370, 200);
    finish_button->setPos(370, 250);

    //����Щ��ť������z����ĵڶ���
    start_button->setZValue(2);
    option_button->setZValue(2);
    help_button->setZValue(2);
    exit_button->setZValue(2);
    restart_button->setZValue(2);
    return_button->setZValue(2);
    finish_button->setZValue(2);

    //һ���ְ�ť��������
    restart_button->hide();
    finish_button->hide();
    pause_button->hide();
    show_menu_button->hide();
    return_button->hide();


    //��������
    background_music = new Phonon::MediaObject(this);//�������ֶ���
    clearrow_sound = new Phonon::MediaObject(this);//������������
    //AudioOutput��һ���������͵���Ƶ������豸
    Phonon::AudioOutput *audio1 = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::AudioOutput *audio2 = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(background_music, audio1);//
    Phonon::createPath(clearrow_sound, audio2);//����Դ����Ƶ����ؼ�

    Phonon::VolumeSlider *volume1 = new Phonon::VolumeSlider(audio1, option);//����1Ϊ��Ƶ����豸������2Ϊ��Ӧ�ĸ�widget
    Phonon::VolumeSlider *volume2 = new Phonon::VolumeSlider(audio2, option);
    QLabel *volume_label1 = new QLabel(tr("��������"), option);
    QLabel *volume_label2 = new QLabel(tr("��Ч����"), option);
    volume1->move(100, 100);
    volume2->move(100, 200);
    volume_label1->move(65, 105);
    volume_label2->move(60, 205);

    //���ֲ���������źŴ���about_to_finish()�ۺ���
    connect(background_music, SIGNAL(aboutToFinish()), this, SLOT(about_to_finish()));
    // ��Ϊ������Ϻ�������ͣ״̬���ٵ���play()���޷����в��ţ���Ҫ�ڲ�����Ϻ�ʹ�����ֹͣ״̬
    connect(clearrow_sound, SIGNAL(finished()), clearrow_sound, SLOT(stop()));
    background_music->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "background.mp3"));
    clearrow_sound->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "clearrow.mp3"));
    background_music->play();//��ʼ����ʱ��ֱ�Ӳ��ű�������
}


//��ʼ��Ϸ
void MyView::start_game() {

    game_welcome_text->hide();
    start_button->hide();
    option_button->hide();
    help_button->hide();
    exit_button->hide();
    mask_widget->hide();
    init_game();
}


//��ʼ����Ϸ��ΪʲôҪ��2��������д��
void MyView::init_game() {

    box_group->create_box(QPointF(300, 70)); //����������,���м�λ�ô�����
    box_group->setFocus();//�����˻��������㣬�����Ϳ���ʹ�ü�����������
    box_group->startTimer(INITSSPEED);//������ʱ��
    game_speed = INITSSPEED;//��Ϸ�ٶȣ���ͣʱ��Ҫ�õ�
    next_box_group->create_box(QPoint(500, 70));//������ʾ������

    scene()->setBackgroundBrush(QPixmap(":/images/background01.png"));
    game_score->setHtml(tr("<font color = red >0</font>"));
    game_level->setHtml(tr("<font color = white>��<br>һ<br>��</font>"));//brΪ����

    restart_button->show();
    pause_button->show();
    show_menu_button->show();
    game_score->show();
    game_level->show();
    top_line->show();
    bottom_line->show();
    left_line->show();
    right_line->show();
    // ������ǰ�������˵�ʱ������boxGroup
    box_group->show();

    //������Ϸ��ʼ�ı�������
    background_music->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "background01.mp3"));
    background_music->play();

}


void MyView::clear_full_rows() {

    for(int y = 429; y > 50; y -= 20) {
        //ÿ��20��ȡһ��item��������������Ĳ�������Ū������û�з�����ʧ��Ч��
        QList<QGraphicsItem *> list = scene()->items(199, y, 202, 22, Qt::ContainsItemShape);//����ָ�����������пɼ���item
        if(list.count() == 10) {   //���һ�������������ٸ��е�����С����
            foreach(QGraphicsItem *item, list) {
                OneBox *box = (OneBox *) item;
          //      box->deleteLater();
                /*���ö�̬Ч����ʧ������*/
                QGraphicsBlurEffect *blur_effect = new QGraphicsBlurEffect;//����ģ��Ч������
                box->setGraphicsEffect(blur_effect);//��ÿ��С�������ģ������
                QPropertyAnimation *animation = new QPropertyAnimation(box, "scale");//��Ӷ�̬Ч�����ߴ�任Ч��
                animation->setEasingCurve(QEasingCurve::OutBounce);//Ϊ��̬Ч�����û���ȡ���ߣ���������ֵ��
                animation->setDuration(250);//����250����
                animation->setStartValue(4);//��ʵ�ߴ�
                animation->setEndValue(0.25);//�����ߴ�
                animation->start(QAbstractAnimation::DeleteWhenStopped);//����������ɾ������
                connect(animation, SIGNAL(finished()), box, SLOT(deleteLater()));//�����������ŵ���С�������ٺ���
       //         clearrow_sound->play();

            }
            rows << y;//�����е��кű��浽rows��
        }
    }

    //������У�����������ķ���
    if(rows.count()>0) {
        clearrow_sound->play();
        QTimer::singleShot(400, this, SLOT(move_box()));//ִֻ��һ�ζ�ʱ�����ȶ�̬Ч���������������ķ���
    //    clearrow_sound->play();
    }
    else {
        //û�����У����³�����ʾ���飬����ʾ����������µ���ʾ����
        box_group->create_box(QPointF(300, 70), next_box_group->getCurrentShape());
        next_box_group->clear_box_group(true);
        next_box_group->create_box(QPointF(500, 70));//
    }

}


void MyView::move_box() {

    for(int i = rows.count(); i > 0; --i) {
        int row = rows.at(i-1);//ȡ�����е��к�,���������λ�ÿ�ʼ
        //ȡ���������ϱ߽絽��ǰ����֮�����γɵľ�������
        foreach(QGraphicsItem *item, scene()->items(199, 49, 202, row-47, Qt::ContainsItemShape)) {
            item->moveBy(0, 20);
        }
    }
    //���·���
    update_score(rows.count());
    //�����µķ�����
    rows.clear();
    box_group->create_box(QPointF(300, 70), next_box_group->getCurrentShape());
    next_box_group->clear_box_group(true);
    next_box_group->create_box(QPoint(500, 70));
}


void MyView::update_score(const int full_row_num) {

    long score = full_row_num*10;//ÿ��һ�е�ʮ�֣��Ƚϰ���
    int current_score = game_score->toPlainText().toInt();
    current_score += score;
    game_score->setHtml(tr("<font color = red>%1</font>").arg(current_score));
    if(current_score >= 100) {
        game_level->setHtml(tr("<font color = white>��<br>��<br>��</font>"));
        scene()->setBackgroundBrush(QPixmap(":/images/background02.png"));
        game_speed = 300;
        box_group->stop_timer();//�������ö�ʱ������
        box_group->startTimer(game_speed);

        if (QFileInfo(background_music->currentSource().fileName()).baseName() != "background02") {
             background_music->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "background02.mp3"));
             background_music->play();
        }
    }
}


void MyView::game_over() {

    pause_button->hide();
    show_menu_button->hide();
    mask_widget->show();
    game_over_text->show();
    restart_button->setPos(370, 200);
    finish_button->show();

}

void MyView::restart_game()
{
    mask_widget->hide();
    game_over_text->hide();
    finish_button->hide();
    restart_button->setPos(600, 150);

    //���ٵ�ǰ������͵�ǰ�����е�����С����
    next_box_group->clear_box_group(true);
    box_group->clear_box_group();
    box_group->hide();
    foreach(QGraphicsItem *item, scene()->items(199, 49, 202, 402,Qt::ContainsItemBoundingRect)) {
        scene()->removeItem(item);
        OneBox *box = (OneBox*)item;
        box->deleteLater();
    }
    init_game();
}


void MyView::finish_game()
{
    game_over_text->hide();
    finish_button->hide();
    restart_button->setPos(600, 150);
    restart_button->hide();
    pause_button->hide();
    show_menu_button->hide();
    game_score->hide();
    game_level->hide();
    top_line->hide();
    bottom_line->hide();
    left_line->hide();
    right_line->hide();

    next_box_group->clear_box_group(true);
    box_group->clear_box_group();
    box_group->hide();
    foreach(QGraphicsItem *item, scene()->items(199, 49, 202, 402,Qt::ContainsItemBoundingRect)) {
        scene()->removeItem(item);
        OneBox *box = (OneBox*)item;
        box->deleteLater();
    }

    mask_widget->show();
    game_welcome_text->show();
    start_button->show();
    option_button->show();
    help_button->show();
    exit_button->show();
    scene()->setBackgroundBrush(QPixmap(":/images/background.png"));

    //��Ϸ����ʱ���ñ�������
    background_music->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "background.mp3"));
    background_music->play();

}

void MyView::pause_game()
{
    box_group->stop_timer();//�ж���Ϸ����Ҫ����ֹͣ�������ƵĶ�ʱ������
    restart_button->hide();
    pause_button->hide();
    show_menu_button->hide();
    mask_widget->show();
    game_pause_text->show();
    return_button->show();
}

void MyView::return_game()
{
    return_button->hide();
    game_pause_text->hide();
    mask_widget->hide();
    restart_button->show();
    pause_button->show();
    show_menu_button->show();
    box_group->startTimer(game_speed);
}

void MyView::about_to_finish()
{
    background_music->enqueue(background_music->currentSource());//���ú����Ĳ�������Ϊ��ǰ����Ƶ�ļ����ﵽ�ظ����ŵ�Ŀ��
}

void MyView::keyPressEvent(QKeyEvent *event) {
    if(pause_button->isVisible())//����Ļ�п��Կ�����ͣ��ťʱ��������Ϸ�������У���ʱ��Ľ�����½��еķ�����
        box_group->setFocus();
    else
        box_group->clearFocus();//�����Ϸ��ͣ���򷽿��鲻�ܻ�ý���
    QGraphicsView::keyPressEvent(event);
}
