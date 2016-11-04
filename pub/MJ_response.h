#ifndef MJ_RESPONSE_H
#define MJ_RESPONSE_H
#include <string>
#include <cstring>

#include "../pub/MJ_Base.h"

class MJ_response
{
public:
    enum Type{
        T_Ok = 0x01, //ack����Ӧ�˴β�������Ч��
        T_confirm, //����ȷ��   /*���ѷ�����*/
        T_Wait,
        T_Init,
        T_ChuPai,//��֪client�ó����ˣ�Ҳ��ʾ
        T_FaPai,
        T_Hu,
        T_Gang,
        T_Peng,
        T_Chi,
        T_BuGang,  // ����
        T_AnGang,  // ����
        T_GMOver,  //game over
        T_UnSucc = 0x400,  // ���ɹ��Ĳ���,��ʱ��������ʱ���µĲ�һ�£�������Ѿ���ʱ����Ĭ�϶�����
                           // �����Ѿ��յ������ȼ��Ķ����� ���յ� client�����󣨻�����ȼ�������
                           // ���ش���Ϣ
        T_Max
    };

    enum VER{
        v1 = 100
    };

    enum SendToWho{
        SDT_Broadcast = 4
    };

    enum HuType{
        HU_JiePaio,
        HU_ZiMo,
        HU_QiangGang,
        HU_GangShangHua
    };

    typedef unsigned int size_t;

public:
    MJ_response(int sendTo=0, VER v = v1);

    void addType(Type t);
    void setType(Type t);
    void setVer(VER v);
    void setHuType(HuType hu);
    void setSendTo(int sendto);
    void setWho(int w);
    void setChi(MJ_Base::CARD chi[4]);
    void setgpc(MJ_Base::CARD *g, MJ_Base::CARD *p, MJ_Base::CARD *c);

    void setCard(MJ_Base::CARD c);
    void setInitType(std::string name, int xid, int pid, int fpri, int score);
    void setPaiList(MJ_Base::pCCARD lst);
    void setPaiCount(int count);

    Type getType()const;
    HuType getHuType()const;
    VER getVer()const;
    int getSendTo()const;
    int getWho()const;
    MJ_Base::CARD getCard()const;
    void getInitType(std::string &name, int &xid, int &pid, int &fpri, int &score);
    int getPaiList(MJ_Base::CARD *lst);
    void getChi(MJ_Base::CARD chi[3]);
    void getgpc(MJ_Base::CARD *g, MJ_Base::CARD *p, MJ_Base::CARD *c);
    int getPaiCount();

public:
private:
    VER ver;
    Type type;
    HuType huType;
    int SendTo;// a id, recver's id, range 0-4
    int who;//��ʾ�ϴζ���˭�������ģ����磺���ƣ�֪ͨ�������ʱ��־˭�����ƣ�
    size_t len;//  msg length
    int paiCount;

    MJ_Base::CARD card;
    MJ_Base::CARD paiList[16];
    MJ_Base::CARD chi[4];
    MJ_Base::CARD g[8];
    MJ_Base::CARD p[8];
    MJ_Base::CARD c[16];
    int P_ID;//���ID
    int X_ID;//��λ��
    int f_pri;//ׯ��&�м�
    std::string p_name;//����ǳ�
    int score;//��ҽ�Ǯ��
};

#endif // MJ_RESPONSE_H
