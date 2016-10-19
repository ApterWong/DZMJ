#ifndef MJ_LOCALSERVER_H
#define MJ_LOCALSERVER_H

#include <QObject>
#include <QTimer>
#include <random>
#include <ctime>

#include "../pub/MJ_RequestData.h"
#include "../pub/MJ_response.h"
#include "../pub/MJ_Player.h"
#include "MJ_RequestLocal.h"

class MJ_LocalServer : public QObject
{
    Q_OBJECT
public:
    explicit MJ_LocalServer(QObject *parent = 0);
    ~MJ_LocalServer();

    void send(MJ_response &);

    void start();//����
    void faPai();
    void resl_chuPai(MJ_RequestData &);
    void resl_HGPCList(MJ_RequestData &);
    void resl_Hu(MJ_RequestData &);
    void resl_Gang(MJ_RequestData &);
    void resl_Peng(MJ_RequestData &);
    void resl_Chi(MJ_RequestData &);
    void resl_Cancel(MJ_RequestData &);

signals:
    void responseSignal(MJ_response);

public slots:
    void RecvSlot(MJ_RequestData request);
    void tmOutSlot();

public:
    enum SVR_STAT{
        SVR_normal,
        SVR_vote
    };

    enum Policy{
        P_None = 0x0,
        P_Chi = 0x01,
        P_Peng = 0x02,
        P_Gang = 0x04,
        P_Hu = 0x08,
        P_Max
    };

private:
    MJ_Player *member[4];

    int mem_policy[4];//���ÿ����Һ���������Ϣ
//    Policy max_policy;
    int current_policy;
    int current_policy_ID;
    bool f_HGPC_valid;
    MJ_Base::CARD chi[3];

    int cur_id;
    int zhuang_id;
    MJ_Base::CARD wang;
    MJ_Base::CARD card;//�������һ����
    int shaizi1, shaizi2;//ɫ��
    int wj_ready;

    int paiCount;//��ǰ������������maxPaiCount������Ϸ����
    int maxPaiCount;
    MJ_Base::CARD paiList[136];//���е���

    SVR_STAT stat;
    QTimer *tmOut;
};

#endif // MJ_LOCALSERVER_H
