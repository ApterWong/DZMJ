#ifndef MJ_REQUESTDATA_H
#define MJ_REQUESTDATA_H

#include "../pub/MJ_Base.h"

#define DataSize 512

class MJ_RequestData
{
public:
    enum Type{
        R_NoType,
        R_Hu = 0x01,
        R_Gang = 0x02,
        R_Peng = 0x04,
        R_Chi = 0x08,
        R_CanCel = 0x10,//ȡ������������--����ɺ������ԣ�����Ҳ���ִ�в���
        R_Init = 0x20,//��ʼ������Ϸ���֣� ����������client���ݳ���
        R_ChuPai = 0x40,//���ƣ�
        R_TuoGuan = 0x80,//�й�
        R_Exit = 0x100,
        R_HGPCList = 0x200,//���������б��ͻ�����Ҫ�ѷ����õ����ݴ�������ˣ��ɷ�������˶�
        R_AnGang = 0x400,//�����ƶ��������ϣ����԰���
        R_BuGang = 0x800,//������һ���ƣ�������֮ǰ�����ģ����Բ���
        R_Max
    };

    typedef unsigned int size_t;
public:
    MJ_RequestData(int id, Type type=R_NoType, MJ_Base::CARD c=MJ_Base::MJ_noCard);

    void setType(Type type);
    void addType(Type type);
    void setCard(MJ_Base::CARD c);
    void setHGPCList(MJ_Base::pCCARD h, MJ_Base::pCCARD g, MJ_Base::pCCARD p, MJ_Base::pCCARD c);
    void setChi(MJ_Base::CARD chi[3]);
    void setSenderID(int id);

    int getData(char *dst);//��bufλ����䱾�����壬����ֵ�����ĳ���

    int getType()const;
    int getHGPCList(MJ_Base::CARD *h, MJ_Base::CARD *g, MJ_Base::CARD *p, MJ_Base::CARD *c)const;
    int getChi(MJ_Base::CARD chi[3])const;
    MJ_Base::CARD getCard()const;
    int getSenderID()const;
public:

private:
    char data[DataSize];
    size_t len;

    size_t ver;
    int type;
    int senderID;//0-3

    MJ_Base::CARD cd;
    MJ_Base::CARD chiList[8];
    MJ_Base::CARD pengList[8];
    MJ_Base::CARD gangList[8];
    MJ_Base::CARD huList[8];

    MJ_Base::CARD chi[3];
    MJ_Base::CARD peng;
    MJ_Base::CARD gang;
    MJ_Base::CARD hu;
};

#endif // MJ_REQUESTDATA_H
