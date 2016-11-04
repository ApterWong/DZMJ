#include "MJ_Desktop.h"
#include "ui_MJ_Desktop.h"
#include "../pub/MJ_AnalyResult.h"

#include <QDesktopWidget>
#include <QDebug>

MJ_Desktop::MJ_Desktop(MJ_Cli::GF_Flag flag, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MJ_Desktop)
{
    ui->setupUi(this);
    this->resize(800, 600);
    QDesktopWidget * desktopWidget = QApplication::desktop();
    this->setGeometry((desktopWidget->width()-this->width())/2,
                      (desktopWidget->height()-this->height())/2, this->width(), this->height());

    this->player[0] = new MJ_Player();
    this->player[1] = new MJ_otherPlayer();
    this->player[2] = new MJ_otherPlayer();
    this->player[3] = new MJ_otherPlayer();
    this->self = dynamic_cast<MJ_Player*> (this->player[0]);
    this->self->init("", MJ_Base::MJ_noCard);
    //this->player[0]->init();
    this->player[1]->init();
    this->player[2]->init();
    this->player[3]->init();

    if(flag == MJ_Cli::GF_Local)
    {
        this->request = new MJ_RequestLocal();
        this->server = MJ_LocalServer::getInstance();
        this->zhuang = 0;
        this->self_offset = 0;//ׯ�ң����Ҷ���0�±�λ��
        this->ID = 0;

        this->computer1 = new MJ_Computer(this);
        this->computer1->init();
        this->computer1->setID(1);
        this->computer2 = new MJ_Computer(this);
        this->computer2->init();
        this->computer2->setID(2);
        this->computer3 = new MJ_Computer(this);
        this->computer3->init();
        this->computer3->setID(3);

        connect(this->server, SIGNAL(responseSignal(MJ_response)),
                this->computer1, SLOT(responseSlot(MJ_response)), Qt::QueuedConnection);
        connect(this->server, SIGNAL(responseSignal(MJ_response)),
                        this->computer2, SLOT(responseSlot(MJ_response)), Qt::QueuedConnection);
        connect(this->server, SIGNAL(responseSignal(MJ_response)),
                        this->computer3, SLOT(responseSlot(MJ_response)), Qt::QueuedConnection);

        // bind svr --->   this
//        QObject::connect(this->request, SIGNAL(RequestSignal(MJ_RequestData)),
//                this->server, SLOT(RecvSlot(MJ_RequestData)));
        QObject::connect(this->server, SIGNAL(responseSignal(MJ_response)),
                         this, SLOT(responseSlot(MJ_response)), Qt::QueuedConnection);
    }
    else if(flag == MJ_Cli::GF_OnLine)
    {
        // ......
    }

    this->init_widgets();
}

MJ_Desktop::~MJ_Desktop()
{
    delete ui;

    delete this->player[0];
    delete this->player[1];
    delete this->player[2];
    delete this->player[3];

    delete this->request;
    delete this->gameOverWidget;
}

void MJ_Desktop::preStart()
{
    this->startButton->show();
}

void MJ_Desktop::init_widgets()
{
    this->HGPC_widget = new MJ_HGPCWidget(QSize(100, 30), this);
    this->clock_wdiget= new MJ_CLockWidget(0, QSize(80, 60), this);
    this->disCard_Widget[0] = new MJ_DiscardWidget(this);
    this->disCard_Widget[1] = new MJ_DiscardWidget(this);
    this->disCard_Widget[2] = new MJ_DiscardWidget(this);
    this->disCard_Widget[3] = new MJ_DiscardWidget(this);

    this->self_widget       = new MJ_selfWidget(this);
    this->DuiMen_widget     = new MJ_DuiMenWidget(this);
    this->ShangJia_widget   = new MJ_ShangJiaWidget(this);
    this->XiaJia_widget     = new MJ_XiaJiaWidget(this);
    this->gameOverWidget    = new MJ_gameOverWidget(this);

    wangAndRemainCardCount = new MJ_wangAndRemainCardCount(this);

    connect(self_widget, SIGNAL(retClicked(MJ_Base::CARD)),
            this, SLOT(selfWidgetSlot(MJ_Base::CARD)), Qt::QueuedConnection);
    connect(HGPC_widget, SIGNAL(hgpc_finished()),
            this, SLOT(HGPCWidgetSlot()), Qt::QueuedConnection);

    this->self_widget->setModel(this->self);
    this->XiaJia_widget->setModel(dynamic_cast<MJ_otherPlayer*>(this->player[1]));
    this->DuiMen_widget->setModel(dynamic_cast<MJ_otherPlayer*>(this->player[2]));
    this->ShangJia_widget->setModel(dynamic_cast<MJ_otherPlayer*>(this->player[3]));

    int x = this->size().width();
    int y = this->size().height();

    this->self_widget->setSize(QSize(x*0.6, y/6));
    this->self_widget->move((x-this->self_widget->width()) / 2,
                            y - this->self_widget->height());
    qDebug() << this->self_widget->geometry();

    this->DuiMen_widget->setSize(QSize(x*0.6, y/6));
    this->DuiMen_widget->move((x-this->self_widget->width())/2, 0);
    qDebug() << this->DuiMen_widget->geometry();

    this->XiaJia_widget->setSize(QSize(x/6, y*0.6));
    this->XiaJia_widget->move(x - this->XiaJia_widget->width() + this->clock_wdiget->width(),
                              (y - this->XiaJia_widget->height()) / 2);
    qDebug() << this->XiaJia_widget->geometry();

    this->ShangJia_widget->setSize(QSize(x/6, y*0.6));
    this->ShangJia_widget->move(0, (this->height()-this->ShangJia_widget->height()) / 2);
    qDebug() << this->ShangJia_widget->geometry();

    this->disCard_Widget[0]->setId(this->ID);
    this->disCard_Widget[1]->setId(1);
    this->disCard_Widget[2]->setId(2);
    this->disCard_Widget[3]->setId(3);

    x = this->width()*5/6;
    y = this->height()*5/6;

    this->disCard_Widget[0]->resize((this->width())/3.3, (this->height() - 80)/3 - this->self_widget->height()/3);
    this->disCard_Widget[0]->move(this->width()/2.8,
                                         /*this->height()-this->self_widget->height()-(this->height() - 80)/3);*/
                                  this->height()/2+40);
    this->disCard_Widget[1]->resize((this->width() - this->XiaJia_widget->width()-this->ShangJia_widget->width())/3,
                                    this->height()/3);
    this->disCard_Widget[1]->move(this->width()/2+120, this->height()/3);

    this->disCard_Widget[2]->resize(this->width()/3.3, (this->height()-80)/3 - this->DuiMen_widget->height()/3);
    this->disCard_Widget[2]->move(this->width()/3, this->height()/2-40-this->disCard_Widget[2]->height());
    this->disCard_Widget[3]->resize((this->width() - this->ShangJia_widget->width()-this->ShangJia_widget->width())/3,
                                    this->height()/3);
    this->disCard_Widget[3]->move(this->ShangJia_widget->width()-40, this->height()/3);

    //  ��ʼ��ť
    this->startButton = new QPushButton(QString::fromLocal8Bit("��ʼ"),this);
    this->startButton->setObjectName("StartButton");
    this->startButton->move((this->width()-this->startButton->width())/2, this->height()*3/4);
    connect(this->startButton, SIGNAL(clicked(bool)), this, SLOT(startButtonClicked(bool)), Qt::QueuedConnection);

    this->HGPC_widget->move(this->width()*2/3, this->height()-this->self_widget->height()-60);
    this->HGPC_widget->hide();

    wangAndRemainCardCount->setGeometry(30, 30, this->width()/12, this->height()/7);

    this->gameOverWidget->move((this->width()-this->gameOverWidget->width())/2,
                               120);
    this->gameOverWidget->hide();
}

void MJ_Desktop::resl_init(MJ_response &resp)
{
    //��ʼ������
    // ���ͷ������
    int recverID = resp.getSendTo();
    MJ_Base::CARD lst[16] = {0};
    resp.getPaiList(lst);

    qDebug() << "_Desktop  Init:" << lst << " " << resp.getCard()
             << "recvid = " << recverID;

    if(recverID == this->ID)
    {
        this->wangAndRemainCardCount->mj_setPaiCount(resp.getPaiCount());
        this->wangAndRemainCardCount->mj_setCard(resp.getCard());
        this->self_widget->setWang(resp.getCard());

        this->self->init(lst, resp.getCard()); // ��ʼ������
        MJ_Base::CARD h[16] = {0};
        MJ_Base::CARD g[16] = {0};
        MJ_Base::CARD p[16] = {0};
        MJ_Base::CARD c[16] = {0};

        this->self->AnalysisHGPC();
        this->self->getCanHuList(h, 16);
        this->self->getCanGangList(g, 16);
        this->self->getCanPengList(p, 16);
        this->self->getCanChiList(c, 16);        

        MJ_RequestData req(this->ID);
        req.setType(MJ_RequestData::R_HGPCList);
        req.setHGPCList(h, g, p, c);
        this->request->req_send(req);

        this->self_widget->draw_NewCard(false);
        this->self_widget->draw_PaiList();
    }
    else
    {
        this->player[recverID]->init(lst, resp.getCard());
    }

    this->disCard_Widget[0]->clean();
    this->disCard_Widget[1]->clean();
    this->disCard_Widget[2]->clean();
    this->disCard_Widget[3]->clean();

    /****
       *  TEST    */

    update();
   /**/
}

// ���ֶ���ҿ����ܳԵ������wait��Ϣֻ���յ�һ�Σ���
// �յ�wait��Ϣ��Ҫ��¼����ĳ��������������Ȩ����ߵ�һ�������˭�յ�������Ϣ��˭�������ʤ����
void MJ_Desktop::resl_wait(MJ_response &resp)
{
    //�ж��Լ����Ժ������ԣ�
    int who = resp.getWho();//who��˭������
    //int recverId = resp.getSendTo();//recverIDͨ����4��MJ_response::SDT_Broadcast
    MJ_Base::CARD card = resp.getCard();//�ղ�[who]������


    this->s_card = card;
    this->s_stat = S_None;
    this->s_hgpc = false;
    this->s_id = who;   // ����˭�����ƣ���[������]ʱ������������

    //  ��ʱ 6s ����
    this->clock_wdiget->clockStart(0, 6, false);

    //  �Լ��ľ�����
    if(who == this->ID)
    {
        qDebug() << "Desktop::resl_wait:  SELF" << "return";
        return;
    }

    int self_hgpcStat = 0;
    if((who+1)%4 == this->ID && this->self->testChi(card))
        self_hgpcStat |= S_CHI;
    if(this->self->testPeng(card))
        self_hgpcStat |= S_PENG;
    if(this->self->testGang(card))
        self_hgpcStat |= S_GANG;
    if(this->self->testHu(card))
        self_hgpcStat |= S_HU;
    /***************
       *  ���ú�������ѡ�񴰿�
    *****************/
    //if(self_hgpcStat & S_CHI == (int)S_CHI) //err:
    if((self_hgpcStat & S_CHI) == S_CHI)
    {
        MJ_Base::CARD chilist[8][4] = {0};
        int ret = this->self->getCChiList(card, chilist);
        this->HGPC_widget->setChiList(chilist, ret);
        qDebug() << __FUNCTION__ << __LINE__ << "****chilist***" << chilist[0] << " "
                 << chilist[1] << " " << chilist[2];
    }

    //�Լ��ܺ������� �͵���
    if(self_hgpcStat)
        this->HGPC_widget->hgpc_show(self_hgpcStat);

    qDebug() << "Desktop::resl_wait: " << this->self->getCanChiList(nullptr, 0);
    qDebug() << "Desktop::resl_wait: " << (S_HGPC)self_hgpcStat << endl;
}

//  ����� �����ܺ������յ��Ķ�����
//  ͨ���ж���˭������������Ӧ�Ľ������
//  Ҳ�п������Լ��ģ���Ϊ�Լ�����������Ҫ�õ�MJ_LocalServer����֤
//  �յ�����Ϣ������ͳɹ��ˣ���Ҫ�ȴ����� ���� ��Ϣ��������Ϣ����˭��˭��������ִ�гɹ���
void MJ_Desktop::resl_Chi(MJ_response &resp)
{
    int who = resp.getWho();

    MJ_Base::CARD _chi[4] = {0};
    resp.getChi(_chi);

    if(who == this->ID)
    {
        this->self->Chi(resp.getCard(), _chi);
        this->HGPC_widget->hide();
    }
    else
        this->player[who]->Chi(resp.getCard(), _chi);

    // ������һ�εĶ��������γ��� ��ҵĳ��ƣ�
    this->disCard_Widget[this->s_id]->undo();

    this->s_stat = S_CHI;
    this->s_hgpc = true;
    this->s_id   = who;

    /*********
     *    ���½���
     */
}

void MJ_Desktop::resl_Peng(MJ_response &resp)
{
    int who = resp.getWho();

    //  �������ҳԹ������ƣ���Ҫ���˲���
    if(this->s_hgpc && this->s_stat == S_CHI)
    {
        this->player[this->s_id]->UndoChi();
    }
    else
    {
        this->disCard_Widget[this->s_id]->undo();
    }

    if(who == this->ID)
    {
        this->self->Peng(resp.getCard());
        this->HGPC_widget->hide();
    }
    else
        this->player[who]->Peng(resp.getCard());


    this->s_stat = S_PENG;
    this->s_hgpc = true;
    this->s_id   = who;

    /*********
     *    ���½���
     */
}

void MJ_Desktop::resl_Gang(MJ_response &resp)
{
    int who = resp.getWho();

    //  �������ҳԹ������ƣ���Ҫ���˲���
    if(this->s_hgpc && this->s_stat == S_CHI)
    {
        this->player[this->s_id]->UndoChi();
    }
    //  �����������������ƣ�err
    else if(this->s_hgpc && this->s_stat == S_PENG)
    {

    }
    else
    {
        this->disCard_Widget[this->s_id]->undo();
    }

    this->s_stat = S_GANG;
    this->s_hgpc = true;
    this->s_id   = who;
    if(who == this->ID)
    {
        this->self->Gang(resp.getCard());
        this->HGPC_widget->hide();
    }
    else
        this->player[who]->Gang(resp.getCard());
    /*********
     *    ���½���
     */
}

void MJ_Desktop::resl_Hu(MJ_response &resp)
{
    int who = resp.getWho();

    //  �������ҳ�[����]�������ƣ���Ҫ���˲���
    if(this->s_hgpc && this->s_stat == S_CHI)
    {
        this->player[this->s_id]->UndoChi();
    }
    else if(this->s_hgpc && this->s_stat == S_PENG)
    {
        this->player[this->s_id]->UndoPeng();
    }
    else if(this->s_hgpc && this->s_stat == S_GANG)
    {
        this->player[this->s_id]->UndoGang();
    }
    else
    {
        this->disCard_Widget[this->s_id]->undo();
    }

    this->s_stat = S_HU;
    this->s_hgpc = true;
    this->s_id   = who;

    MJ_Base::CARD pailist[16] = {0};
    MJ_Base::CARD g[8]={0}, p[8]={0}, c[16]={0};
    resp.getPaiList(pailist);
    resp.getgpc(g, p, c);

    qDebug() << QString::fromLocal8Bit("���%1ʤ��:").arg(who) << "pailist:" << pailist;
    qDebug() << "\tgang:" << g << "  p:" << p << " c:" << c;

    MJ_Player win = MJ_Player();
    win.setPengList(p);
    win.setGangList(g);
    win.setChiList(c);
    win.setPaiList(pailist);
    win.setWang(this->self->getWang());
    win.AnalysisHGPC();
    if(win.testHu(resp.getCard()))
    {
        MJ_AnalyResult analy(win);
        analy.calc_BeiShu(win, resp.getCard(), MJ_AnalyResult::F_JiePao);
        int fan = analy.getFan();
        auto list = analy.HU_names();

        this->gameOverWidget->setPai(g, p, c, pailist, resp.getCard());
        this->gameOverWidget->setFan(list, fan);
        this->gameOverWidget->show();
    }

    if(who == this->ID)
    {
        this->self->Hu(resp.getCard(), pailist);
        this->HGPC_widget->hide();
    }
    else
        this->player[who]->Hu(resp.getCard(), pailist);
    /*********
     *    ���½���
     */
}

void MJ_Desktop::resl_FaPai(MJ_response &resp)
{
    int who = resp.getWho();
    this->self_newCard = resp.getCard();

    cur_zhuapai = who;//��ǵ�ǰҪ���Ƶ����
    if(who == this->ID)
    {
        if(self_newCard != MJ_Base::MJ_noCard)
            this->self_widget->draw_NewCard(true);
        this->XiaJia_widget->draw_NewCard(false);
        this->ShangJia_widget->draw_NewCard(false);
        this->DuiMen_widget->draw_NewCard(false);

        this->HGPC_widget->hide();


        this->self->setNewCard(resp.getCard());
        this->self_widget->draw_PaiList();

        int hg_stat = 0;
        if(this->self->testGang(self_newCard))//����
        {
            anGangCard = this->self_newCard;
            hg_stat |= S_GANG;
        }
        else if((anGangCard = this->self->testAnGang()) != MJ_Base::MJ_noCard)
        {
            hg_stat |= S_GANG;
        }
        if(this->self->testHu(self_newCard))
            hg_stat |= S_HU;
        if(this->self->testBuGang(self_newCard))//  ���ܣ����ܿɱ����ܺ�
            hg_stat |= S_BuGang;
        if(this->self->testZiMo(self_newCard))
            hg_stat |= S_ZiMo;

        if(hg_stat != S_None)
        {
            /***
             *  ��ʾ�ɺ��ƣ����ƴ���
             */
            this->s_card  = self_newCard;
            this->HGPC_widget->hgpc_show(hg_stat);
        }
    }
    else
    {
        if(who == 1)
        {
            this->XiaJia_widget->draw_NewCard(true);
            this->self_widget->draw_NewCard(false);
            this->ShangJia_widget->draw_NewCard(false);
            this->DuiMen_widget->draw_NewCard(false);
        }
        else if(who == 2)
        {
            this->DuiMen_widget->draw_NewCard(true);
            this->self_widget->draw_NewCard(false);
            this->XiaJia_widget->draw_NewCard(false);
            this->ShangJia_widget->draw_NewCard(false);
        }
        else if(who == 3)
        {
            this->ShangJia_widget->draw_NewCard(true);
            this->self_widget->draw_NewCard(false);
            this->DuiMen_widget->draw_NewCard(false);
            this->XiaJia_widget->draw_NewCard(false);
        }
        else
        {
            qDebug() << "_Desktop::resl_FaPai err: who = " << who;
        }
    }

    this->clock_wdiget->clockStart(who, 8);

    if(self_newCard != MJ_Base::MJ_noCard)
        this->wangAndRemainCardCount->mj_setPaiCount(resp.getPaiCount());

    update();

    qDebug() << "_Desktop::resl_FaPai" << endl;
}

void MJ_Desktop::resl_ChuPai(MJ_response &resp)
{
    qDebug() << "_Desktop::resl_ChuPai:" << resp.getCard() << endl;

    int who = resp.getWho();
    MJ_Base::CARD card = resp.getCard();

    if(who < 0 || who >=4)
        throw("err:");

    if(who == this->ID)
    {
        if(card == this->self_newCard)
        {
            this->self_widget->draw_NewCard(false);
        }
        else
        {
            if(self_newCard != MJ_Base::MJ_noCard)
                this->self->addCard(this->self_newCard);
            this->self->ChuPai(card);

            // �����·����Լ��ĺ������� ���͸�svr
            MJ_Base::CARD h[16] = {0};
            MJ_Base::CARD g[16] = {0};
            MJ_Base::CARD p[16] = {0};
            MJ_Base::CARD c[16] = {0};

            qDebug() << __FUNCTION__ << __LINE__ << "  ";
            this->self->AnalysisHGPC();
            qDebug() << this->self->getCanHuList(h, 16);
            qDebug() << this->self->getCanGangList(g, 16);
            qDebug() << this->self->getCanPengList(p, 16);
            qDebug() << this->self->getCanChiList(c, 16);

            qDebug() << __FUNCTION__ << __LINE__ << "  " << h << g << p << c;

            MJ_RequestData req(this->ID);
            req.setType(MJ_RequestData::R_HGPCList);
            req.setHGPCList(h, g, p, c);
            this->request->req_send(req);
        }
        this->self_widget->draw_PaiList();
    }
    // ��ӵ��������� ����
    this->disCard_Widget[who]->addCard(card);
    this->HGPC_widget->hide();
    update();
    qDebug() << "_Desktop::resl_ChuPai:" << " endl" << endl;
}

void MJ_Desktop::resl_OK(MJ_response &)
{

}

void MJ_Desktop::resl_Unsucc(MJ_response &resp)
{
    int recverID = resp.getSendTo();

    if(recverID == this->ID)
    {
        /****
         * ͨ���� �������� ������Ҫ����
         */

    }
}

void MJ_Desktop::resl_Over(MJ_response &)
{

    this->clock_wdiget->clockStart(0, 0, false);
    this->preStart();
}

void MJ_Desktop::responseSlot(MJ_response response)
{
    int type = response.getType();

    qDebug() << "_Desktop::responseSlot: Send to" << response.getSendTo();
    qDebug() << "_Desktop::responseSlot: who" << response.getWho();

    switch(type)
    {
    case MJ_response::T_Init:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Init";
        this->resl_init(response);
        break;
    case MJ_response::T_Wait:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Wait";
        this->resl_wait(response);
        break;
    case MJ_response::T_Chi:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Chi";
        this->resl_Chi(response);
        break;
    case MJ_response::T_Peng:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Peng";
        this->resl_Peng(response);
        break;
    case MJ_response::T_Gang:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Gang";
        this->resl_Gang(response);
        break;
    case MJ_response::T_Hu:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Hu";
        this->resl_Hu(response);
        break;
    case MJ_response::T_FaPai:
        qDebug() << "_Desktop::responseSlot: type:" << "T_FaPai";
        this->resl_FaPai(response);
        break;
    case MJ_response::T_ChuPai:
        qDebug() << "_Desktop::responseSlot: type:" << "T_ChuPai";
        this->resl_ChuPai(response);
        break;
    case MJ_response::T_Ok:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Ok";
        this->resl_OK(response);
        break;
    case MJ_response::T_UnSucc:
        qDebug() << "_Desktop::responseSlot: type:" << "T_UnSucc";
        this->resl_Unsucc(response);
        break;
    case MJ_response::T_GMOver:
        qDebug() << "_Desktop::responseSlot: type:" << "T_GMOver";
        this->resl_Over(response);
        break;
    default:
        break;
    }
}

void MJ_Desktop::selfWidgetSlot(MJ_Base::CARD cd)
{
    if(cur_zhuapai == this->ID) //
    {
        qDebug() << QString::fromLocal8Bit("���ƣ�") << cd;

        this->HGPC_widget->hide();
        this->cur_zhuapai = -1;//�޸��ظ�����bug

        MJ_RequestData req(this->ID);
        req.setType(MJ_RequestData::R_ChuPai);
        req.setCard(cd);
        req.setSenderID(this->ID);

        request->req_send(req);
    }
}

void MJ_Desktop::HGPCWidgetSlot()
{
    int result = HGPC_widget->getResult();
    qDebug() << "_Desktop::HGPCWidgetSlot:" << result;

    MJ_RequestData req(this->ID);
    switch(result)
    {
    case MJ_HGPCWidget::RES_HU:
        req.setType(MJ_RequestData::R_Hu);
        qDebug() << QString::fromLocal8Bit(" �ҷ����ƣ�") << this->s_card;
        break;
    case MJ_HGPCWidget::RES_GANG:
        if(this->anGangCard != MJ_Base::MJ_noCard) // ���԰��ܾ��Ȱ���
        {
            req.setType(MJ_RequestData::R_AnGang);
            this->s_card = this->anGangCard;
            qDebug() << QString::fromLocal8Bit(" �ҷ����ܣ�") << this->s_card;
        }
        else
        {
            req.setType(MJ_RequestData::R_Gang);
            qDebug() << QString::fromLocal8Bit(" �ҷ����ܣ�") << this->s_card;
        }
        break;
    case MJ_HGPCWidget::RES_PENG:
        req.setType(MJ_RequestData::R_Peng);
        qDebug() << QString::fromLocal8Bit(" �ҷ����ƣ�") << this->s_card;
        break;
    case MJ_HGPCWidget::RES_CHI:
        {
            req.setType(MJ_RequestData::R_Chi);
            MJ_Base::CARD cds[4] = {0};
            this->HGPC_widget->getChiSelected(cds);
            req.setChi(cds);
            qDebug() << QString::fromLocal8Bit(" �ҷ����ƣ�") << cds;
        }
        break;
    //case MJ_HGPCWidget::RES_CANCEL:
    default:
        req.setType(MJ_RequestData::R_CanCel);
        qDebug() << QString::fromLocal8Bit(" �ҷ�����") << this->s_card;
        break;
    }
    req.setCard(this->s_card);
    req.setSenderID(this->ID);

    this->request->req_send(req);
}

void MJ_Desktop::startButtonClicked(bool)
{
    MJ_RequestData req(this->ID);

    req.setType(MJ_RequestData::R_Init);
    req.setSenderID(this->ID);

    this->request->req_send(req);
    this->startButton->hide();
    this->gameOverWidget->hide();
}




