#ifndef MJ_WIDGETBASE_H
#define MJ_WIDGETBASE_H
#include "../../pub/MJ_Base.h"

#include <QPixmap>

/***  ***  ***  ***  ***  ***  ***
 *      �������ڼ����������Ͻ�Ҫ��ʾ��
 *  ��������������������������ֻ����һ��
 *  ʵ��
 */
class MJ_widgetBase
{
public:
    enum BG_CARD{
        C_top,
        C_bottom,
        C_left,
        C_right,

        C_bg,//�������Ƶı���ͼ

        C_bg_top,
        C_bg_bottom,
        C_bg_left,
        C_bg_right
    };

public:
    static MJ_widgetBase *getInstance();

    ~MJ_widgetBase();

    const QPixmap *operator[](MJ_Base::CARD cd);
    const QPixmap *operator[](MJ_widgetBase::BG_CARD cd);

private:
    MJ_widgetBase();

private:
    static MJ_widgetBase *instance;

    QPixmap *cards[128];

    QPixmap *bottom;
    QPixmap *top;
    QPixmap *left;
    QPixmap *right;

    QPixmap *bg_bottom;
    QPixmap *bg_top;
    QPixmap *bg_left;
    QPixmap *bg_right;

    QPixmap *bg;

    QPixmap *None;
};

#endif // MJ_WIDGETBASE_H
