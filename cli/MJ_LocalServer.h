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
    explicit MJ_LocalServer(QObject *parent = 0);
public:
    static MJ_LocalServer *Instance;
    static MJ_LocalServer *getInstance();
    ~MJ_LocalServer();

    void send(MJ_response &);

    void start();//����
    void faPai_NoCard();
    void faPai();
    void resl_chuPai(MJ_RequestData &);
    void resl_HGPCList(MJ_RequestData &);
    void resl_Hu(MJ_RequestData &);
    void resl_Gang(MJ_RequestData &);
    void resl_Peng(MJ_RequestData &);
    void resl_Chi(MJ_RequestData &);
    void resl_Cancel(MJ_RequestData &);
    void resl_AnGang(MJ_RequestData &);
    void resl_BuGang(MJ_RequestData &);

private:
    void _FaPaiRequest();

signals:
    void responseSignal(MJ_response);

    void _postToRecvSlot(MJ_RequestData request);  // ���Լ����ͣ����⺯��ֱ�ӵ��ô����Ļ���

public slots:
    void RecvSlot(MJ_RequestData request);
    void tmOutSlot();
    void tmChuPaiSlot();

public:
    enum SVR_STAT{
        SVR_normal,
        SVR_vote
    };

    enum Policy{
        P_None = 0x0,
        P_Chi = 0x01,
        P_Peng = 0x10,
        P_Gang = 0x100,
        P_Hu = 0x1000,
        P_Max
    };

private:
    MJ_Player *player[4];

    int mem_policy[4];//���ÿ����Һ���������Ϣ
    int current_policy;//Policy����,���浱ǰ������ߵģ���>��[��]>�ԣ�
    int current_policy_ID;
    bool f_HGPC_valid;//���� �������� ��Ч����ʱ���п�����Ӧ���ӳ����⵼���¼��Ѿ������ˣ��ϼҲ�����������Ϣ
    bool gang_falg;
    MJ_Base::CARD chi[4];

    int cur_id;
    int zhuang_id;
    int winner_id;

    MJ_Base::CARD wang;
    MJ_Base::CARD card;//�������һ����
    MJ_Base::CARD tmOutCard;//�������Գ�ʱ������

    int shaizi1, shaizi2;//ɫ��
    int wj_ready;

    int paiCount;//��ǰ������������maxPaiCount������Ϸ����
    int maxPaiCount;
    MJ_Base::CARD paiList[136];//���е���

    SVR_STAT stat;
    QTimer *tmOut;
    QTimer *tmChuPai;
};

#define resp_unsuccessful(who, to) do{\
    MJ_response resp;\
    resp.setType(MJ_response::T_UnSucc);\
    resp.setWho((who));\
    resp.setSendTo((to));\
    send(resp);\
}while(0)

#endif // MJ_LOCALSERVER_H
