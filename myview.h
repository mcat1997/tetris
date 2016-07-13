#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include <phonon>

class BoxGroup;

class MyView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyView(QWidget *parent = 0);//�ؼ���explicit��Ϊ�˷�ֹ��ʽ����ת��

public slots:
    void start_game();
    void clear_full_rows();
    void move_box();
    void game_over();
    void restart_game();
    void finish_game();
    void pause_game();
    void return_game();
    void about_to_finish();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QGraphicsLineItem *top_line;
    QGraphicsLineItem *bottom_line;
    QGraphicsLineItem *left_line;
    QGraphicsLineItem *right_line;
    BoxGroup *box_group;
    BoxGroup *next_box_group;
    qreal game_speed;
    QList<int> rows;
    QGraphicsTextItem *game_score;//�����ı�ͼ�ζ���
    QGraphicsTextItem *game_level;//��Ϸ�ȼ��ı�ͼ�ζ���
    QGraphicsWidget *mask_widget;
    //��ҳ����Ϸ����Ҫ�õ��ĸ��ְ�ť
    QGraphicsWidget *start_button;
    QGraphicsWidget *finish_button;
    QGraphicsWidget *restart_button;
    QGraphicsWidget *pause_button;
    QGraphicsWidget *option_button;
    QGraphicsWidget *return_button;
    QGraphicsWidget *help_button;
    QGraphicsWidget *exit_button;
    QGraphicsWidget *show_menu_button;

    //��ʾ�˻��������ı���Ϣ
    QGraphicsTextItem *game_welcome_text;
    QGraphicsTextItem *game_pause_text;
    QGraphicsTextItem *game_over_text;

    //��ӱ������ֺ���������
    Phonon::MediaObject *background_music;
    Phonon::MediaObject *clearrow_sound;

    void init_view();
    void init_game();
    void update_score(const int full_row_num = 0);

signals:

    
};

#endif // MYVIEW_H
