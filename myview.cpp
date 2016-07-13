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


static const qreal INITSSPEED = 800;//游戏的初始化速度
static const QString SOUNDPATH = "./sounds/";

MyView::MyView(QWidget *parent) :
    QGraphicsView(parent)
{
    init_view();
}

void MyView::init_view() {

    setRenderHint(QPainter::Antialiasing);//使用抗锯齿的方式渲染
    setCacheMode(CacheBackground);//设置缓存背景，这样可以加快渲染速度
    setWindowTitle(tr("俄罗斯方块"));
    setWindowIcon(QIcon(":/images/icon.png"));//设置标题处的图标
    setMinimumSize(810, 510);  //2者设置成一样说明视图尺寸不能再更改
    setMaximumSize(810, 510);
    QGraphicsScene *scene = new QGraphicsScene;//新建场景指针
    scene->setSceneRect(5, 5, 800, 500);//场景大小
    scene->setBackgroundBrush(QPixmap(":/images/background.png"));
    setScene(scene);//设置场景

    //俄罗斯方块可移动区域外界的4条线,与外界预留3个像素是为了方便进行碰撞检测
    top_line = scene->addLine(197, 47, 403, 47);
    bottom_line = scene->addLine(197, 453, 403, 453);
    left_line = scene->addLine(197, 47, 197, 453);
    right_line = scene->addLine(403, 47, 403, 453);

    //添加当前方块组
    box_group = new BoxGroup;//通过新建BoxGroup对象间接达到调用box的2个类
    connect(box_group, SIGNAL(need_new_box()), this, SLOT(clear_full_rows()));
    connect(box_group, SIGNAL(game_finished()), this, SLOT(game_over()));
    scene->addItem(box_group);

    //添加提示方块组
    next_box_group = new BoxGroup;
    scene->addItem(next_box_group);

    game_score = new QGraphicsTextItem(0, scene);//文本的父item为对应的场景
    game_score->setFont(QFont("Times", 50, QFont::Bold));//为文本设置字体
    game_score->setPos(450, 350);//分数在场景中出现的位置

    game_level = new QGraphicsTextItem(0, scene);
    game_level->setFont(QFont("Times", 50, QFont::Bold));
    game_level->setPos(20, 150);

    //开始游戏
 //   start_game();
    //初始状态时不显示游戏区域，不显示游戏分数和游戏等级
    top_line->hide();
    bottom_line->hide();
    left_line->hide();
    right_line->hide();
    game_score->hide();
    game_level->hide();

    //添加黑色遮罩
    QWidget *mask = new QWidget;
    mask->setAutoFillBackground(true);
    mask->setPalette(QPalette(QColor(0, 0, 0, 50)));//alpha为不透明度
    mask->resize(900, 600);
    //addWidget()函数的返回值是QGraphicsProxyWidget，如果不添加相应的头文件，则此处会报错
    mask_widget = scene->addWidget(mask);
    mask_widget->setPos(-50, -50);
    mask_widget->setZValue(1);//该层薄纱放在原图的上面，这里有点类似于opengl中的3维绘图

    //选项面板
    QWidget *option = new QWidget;
    //将关闭按钮放在option上
    QPushButton *option_close_button = new QPushButton(tr("关  闭"), option);//第2个参数为按钮所在的widget
    //设置按钮的字体颜色是白色
    QPalette palette;
    palette.setColor(QPalette::ButtonText, Qt::black);//第一个参数调色版的role，这里指的是按钮字体颜色
    option_close_button->setPalette(palette);
    //设置关闭按钮的位置，和单击后的响应
    option_close_button->move(120, 300);
    connect(option_close_button, SIGNAL(clicked()), option, SLOT(hide()));//单击后消失

    option->setAutoFillBackground(true);
    option->setPalette(QPalette(QColor(0, 0, 0, 180)));
    option->resize(300, 400);
    QGraphicsWidget *option_widget = scene->addWidget(option);
    option_widget->setPos(250, 50);
    option_widget->setZValue(3);
    option_widget->hide();


    //帮助面板
    QWidget *help = new QWidget;
    QPushButton *help_close_button = new QPushButton(tr("帮  助"), help);
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

    //游戏欢迎文本
    game_welcome_text = new QGraphicsTextItem(0, scene);//第一个参数为文本内容，第二个参数为父item
    game_welcome_text->setHtml(tr("<font color=green>俄罗斯方块</font>"));
    game_welcome_text->setFont(QFont("Times", 40, QFont::Bold));
    game_welcome_text->setPos(300, 100);
    game_welcome_text->setZValue(2);//放在第2层

    //游戏暂停文本
    game_pause_text = new QGraphicsTextItem(0, scene);//第一个参数为文本内容，第二个参数为父item
    game_pause_text->setHtml(tr("<font color=green>游戏暂停中！</font>"));
    game_pause_text->setFont(QFont("Times", 40, QFont::Bold));
    game_pause_text->setPos(300, 100);
    game_pause_text->setZValue(2);//放在第2层
    game_pause_text->hide();

    //游戏结束文本
    game_over_text = new QGraphicsTextItem(0, scene);//第一个参数为文本内容，第二个参数为父item
    game_over_text->setHtml(tr("<font color=green>GAME　OVER！</font>"));
    game_over_text->setFont(QFont("Times", 40, QFont::Bold));
    game_over_text->setPos(300, 100);
    game_over_text->setZValue(2);//放在第2层
    game_over_text->hide();

    // 游戏中使用的按钮

    QPushButton *button1 = new QPushButton(tr("开    始"));
    QPushButton *button2 = new QPushButton(tr("选    项"));
    QPushButton *button3 = new QPushButton(tr("帮    助"));
    QPushButton *button4 = new QPushButton(tr("退    出"));
    QPushButton *button5 = new QPushButton(tr("重新开始"));
    QPushButton *button6 = new QPushButton(tr("暂    停"));
    QPushButton *button7 = new QPushButton(tr("主 菜 单"));
    QPushButton *button8 = new QPushButton(tr("返回游戏"));
    QPushButton *button9 = new QPushButton(tr("结束游戏"));

    connect(button1, SIGNAL(clicked()), this, SLOT(start_game()));
    connect(button2, SIGNAL(clicked()), option, SLOT(show()));
    connect(button3, SIGNAL(clicked()), help, SLOT(show()));
    connect(button4, SIGNAL(clicked()), qApp, SLOT(quit()));//此处槽函数的接收对象为应用程序本身
    connect(button5, SIGNAL(clicked()), this, SLOT(restart_game()));
    connect(button6, SIGNAL(clicked()), this, SLOT(pause_game()));
    connect(button7, SIGNAL(clicked()), this, SLOT(finish_game()));
    connect(button8, SIGNAL(clicked()), this, SLOT(return_game()));//返回主菜单
    connect(button9, SIGNAL(clicked()), this, SLOT(finish_game()));

    start_button = scene->addWidget(button1);//restart_button并不是QPushbutton类型，而是QGraphicsItem类型,后面的类似
    option_button = scene->addWidget(button2);
    help_button = scene->addWidget(button3);
    exit_button = scene->addWidget(button4);
    restart_button = scene->addWidget(button5);
    pause_button = scene->addWidget(button6);
    show_menu_button = scene->addWidget(button7);
    return_button = scene->addWidget(button8);
    finish_button = scene->addWidget(button9);

    //设置位置
    start_button->setPos(370, 200);
    option_button->setPos(370, 250);
    help_button->setPos(370, 300);
    exit_button->setPos(370, 350);
    restart_button->setPos(600, 150);
    pause_button->setPos(600, 200);
    show_menu_button->setPos(600, 250);
    return_button->setPos(370, 200);
    finish_button->setPos(370, 250);

    //将这些按钮都放在z方向的第二层
    start_button->setZValue(2);
    option_button->setZValue(2);
    help_button->setZValue(2);
    exit_button->setZValue(2);
    restart_button->setZValue(2);
    return_button->setZValue(2);
    finish_button->setZValue(2);

    //一部分按钮隐藏起来
    restart_button->hide();
    finish_button->hide();
    pause_button->hide();
    show_menu_button->hide();
    return_button->hide();


    //设置声音
    background_music = new Phonon::MediaObject(this);//背景音乐对象
    clearrow_sound = new Phonon::MediaObject(this);//消行声音对象
    //AudioOutput是一个将数据送到音频输出的设备
    Phonon::AudioOutput *audio1 = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::AudioOutput *audio2 = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(background_music, audio1);//
    Phonon::createPath(clearrow_sound, audio2);//绑定音源和音频输出控件

    Phonon::VolumeSlider *volume1 = new Phonon::VolumeSlider(audio1, option);//参数1为音频输出设备，参数2为对应的父widget
    Phonon::VolumeSlider *volume2 = new Phonon::VolumeSlider(audio2, option);
    QLabel *volume_label1 = new QLabel(tr("音乐声："), option);
    QLabel *volume_label2 = new QLabel(tr("音效声："), option);
    volume1->move(100, 100);
    volume2->move(100, 200);
    volume_label1->move(65, 105);
    volume_label2->move(60, 205);

    //音乐播放完后发射信号触发about_to_finish()槽函数
    connect(background_music, SIGNAL(aboutToFinish()), this, SLOT(about_to_finish()));
    // 因为播放完毕后会进入暂停状态，再调用play()将无法进行播放，需要在播放完毕后使其进入停止状态
    connect(clearrow_sound, SIGNAL(finished()), clearrow_sound, SLOT(stop()));
    background_music->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "background.mp3"));
    clearrow_sound->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "clearrow.mp3"));
    background_music->play();//初始化的时候直接播放背景音乐
}


//开始游戏
void MyView::start_game() {

    game_welcome_text->hide();
    start_button->hide();
    option_button->hide();
    help_button->hide();
    exit_button->hide();
    mask_widget->hide();
    init_game();
}


//初始化游戏，为什么要分2个函数来写？
void MyView::init_game() {

    box_group->create_box(QPointF(300, 70)); //创建方块组,在中间位置处出现
    box_group->setFocus();//设置人机交互焦点，这样就可以使用键盘来控制它
    box_group->startTimer(INITSSPEED);//启动定时器
    game_speed = INITSSPEED;//游戏速度，暂停时需要用到
    next_box_group->create_box(QPoint(500, 70));//创建提示方块组

    scene()->setBackgroundBrush(QPixmap(":/images/background01.png"));
    game_score->setHtml(tr("<font color = red >0</font>"));
    game_level->setHtml(tr("<font color = white>第<br>一<br>关</font>"));//br为换行

    restart_button->show();
    pause_button->show();
    show_menu_button->show();
    game_score->show();
    game_level->show();
    top_line->show();
    bottom_line->show();
    left_line->show();
    right_line->show();
    // 可能以前返回主菜单时隐藏了boxGroup
    box_group->show();

    //设置游戏开始的背景音乐
    background_music->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "background01.mp3"));
    background_music->play();

}


void MyView::clear_full_rows() {

    for(int y = 429; y > 50; y -= 20) {
        //每隔20行取一个item出来，括号里面的参数不能弄错，否则没有方块消失的效果
        QList<QGraphicsItem *> list = scene()->items(199, y, 202, 22, Qt::ContainsItemShape);//返回指定区域内所有可见的item
        if(list.count() == 10) {   //如果一行已满，则销毁该行的所有小方块
            foreach(QGraphicsItem *item, list) {
                OneBox *box = (OneBox *) item;
          //      box->deleteLater();
                /*采用动态效果消失方块行*/
                QGraphicsBlurEffect *blur_effect = new QGraphicsBlurEffect;//创建模糊效果对象
                box->setGraphicsEffect(blur_effect);//给每个小方块添加模糊设置
                QPropertyAnimation *animation = new QPropertyAnimation(box, "scale");//添加动态效果，尺寸变换效果
                animation->setEasingCurve(QEasingCurve::OutBounce);//为动态效果设置缓冲取曲线，是用来插值的
                animation->setDuration(250);//持续250毫秒
                animation->setStartValue(4);//其实尺寸
                animation->setEndValue(0.25);//结束尺寸
                animation->start(QAbstractAnimation::DeleteWhenStopped);//动画结束后删除该类
                connect(animation, SIGNAL(finished()), box, SLOT(deleteLater()));//动画结束完后才调用小方块销毁函数
       //         clearrow_sound->play();

            }
            rows << y;//将满行的行号保存到rows中
        }
    }

    //如果满行，则下移上面的方块
    if(rows.count()>0) {
        clearrow_sound->play();
        QTimer::singleShot(400, this, SLOT(move_box()));//只执行一次定时器，等动态效果完后再下移上面的方块
    //    clearrow_sound->play();
    }
    else {
        //没有满行，则新出现提示方块，且提示方块出更新新的提示方块
        box_group->create_box(QPointF(300, 70), next_box_group->getCurrentShape());
        next_box_group->clear_box_group(true);
        next_box_group->create_box(QPointF(500, 70));//
    }

}


void MyView::move_box() {

    for(int i = rows.count(); i > 0; --i) {
        int row = rows.at(i-1);//取出满行的行号,从最上面的位置开始
        //取出从区域上边界到当前满行之间所形成的矩形区域
        foreach(QGraphicsItem *item, scene()->items(199, 49, 202, row-47, Qt::ContainsItemShape)) {
            item->moveBy(0, 20);
        }
    }
    //更新分数
    update_score(rows.count());
    //出现新的方块组
    rows.clear();
    box_group->create_box(QPointF(300, 70), next_box_group->getCurrentShape());
    next_box_group->clear_box_group(true);
    next_box_group->create_box(QPoint(500, 70));
}


void MyView::update_score(const int full_row_num) {

    long score = full_row_num*10;//每消一行的十分，比较霸气
    int current_score = game_score->toPlainText().toInt();
    current_score += score;
    game_score->setHtml(tr("<font color = red>%1</font>").arg(current_score));
    if(current_score >= 100) {
        game_level->setHtml(tr("<font color = white>第<br>二<br>关</font>"));
        scene()->setBackgroundBrush(QPixmap(":/images/background02.png"));
        game_speed = 300;
        box_group->stop_timer();//重新设置定时器参数
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

    //销毁当前方块组和当前方块中的所有小方块
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

    //游戏结束时更该背景音乐
    background_music->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "background.mp3"));
    background_music->play();

}

void MyView::pause_game()
{
    box_group->stop_timer();//中断游戏最主要的是停止方块下移的定时器工作
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
    background_music->enqueue(background_music->currentSource());//设置后续的播放序列为当前的音频文件，达到重复播放的目的
}

void MyView::keyPressEvent(QKeyEvent *event) {
    if(pause_button->isVisible())//当屏幕中可以看到暂停按钮时，代表游戏还在运行，这时候的焦点给下降中的方块组
        box_group->setFocus();
    else
        box_group->clearFocus();//如果游戏暂停，则方块组不能获得焦点
    QGraphicsView::keyPressEvent(event);
}
