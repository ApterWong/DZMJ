#ifndef MJ_PLAYER_H
#define MJ_PLAYER_H

#include <map>
#include <string>
#include "MJ_AnalyResult.h"
#include "MJ_Base.h"

#define DEBUG

class MJ_Player : public MJ_Base
{
    friend class MJ_AnalyResult;

private:

    CARD HuList[16];
    CARD cChiList[16];
    CARD cGangList[8];
    CARD cPengList[8];

    CARD wang;

    _L cPengCount;
    _L cGangCount;
    _L cChiCount;

    std::map<CARD, MJ_AnalyResult> AnalyResults;
private:
    bool hasSanLian(CARD*li, CARD c, CARD **res);
    bool hasErLian(CARD*li, CARD c, CARD **res);
    bool hasKanZhang(CARD*li, CARD c, CARD **res);

    void MJ_sort(typename MJ_Player::CARD *a, int len);

public:

    MJ_Player();

    // ���ֳ�ʼ�� �� ���� ���Ҿ���
    void init(pCCARD _paiList, CARD _wang) ;

    // ��ץ�����Ʒ�������
    void addCard(CARD newCard) ;

    int ChuPai(CARD c) override;
    // ɾ��һ���ƣ����ƣ���-3�� ��-3�� ��-3��
    int DelCard(CARD card);
    int DelCard(int offset);

    // ������ �����ܺ�
    // ����ֵ��>=0ƥ�������� <0����������
    int cChi() ;
    int cPeng() ;
    int cGang() ;
    int analysis();

    int Hu(CARD c, pCCARD ll) override;
    int Gang(CARD c) override;
    int Peng(CARD c) override;
    int Chi(CARD c, pCCARD ll) override;


    //����һ���� ��ȡ�����ԳԵ�����ϡ�
    //����ֵ��-1  ������card����0��ʾû�п��ԳԵģ�������������ʾres���ж�����
    //res��Ž�������̰߳�ȫ
    int getCChiList(CARD card, CARD (*res)[3]);
    bool testHu(CARD c);
    bool testPeng(CARD c);
    bool testGang(CARD c);
    bool testChi(CARD c);

    int copy_chiList(MJ_AnalyResult *);
    int copy_pengList(MJ_AnalyResult *);
    int copy_gangList(MJ_AnalyResult *);


#ifdef DEBUG
    void printTest();
    void printHu();
#endif
};

#define isFENG(card) ((card) <= MJ_Player::MJ_DONG && (card) >= MJ_Player::MJ_BAN)
#define isTONG(card) ((card) >= MJ_Player::MJ_JIUTONG && (card) <= MJ_Player::MJ_YITONG)
#define isTIAO(card) ((card) >= MJ_Player::MJ_JIUTIAO && (card) <= MJ_Player::MJ_YITIAO)
#define isWAN(card) ((card) >= MJ_Player::MJ_JIUWAN && (card) <= MJ_Player::MJ_YIWAN)

//  �ж���һ�źϷ�����
#define isMJCARD(card) (isFENG(card) || isTONG(card) || isTIAO(card) || isWAN(card))

//  ������ͬ����
#define hasSanTong(plist) ((plist)[0] == (plist)[1] && (plist)[0] == (plist)[2])
//  ����
//#define hasSanLian(plist) (!isFENG((plist)[0])) && strchr((plist), (plist)[0]-1) && strchr((plist), (plist)[0]-2)
//  ����
#define hasDuiZi(plist) ((plist)[0] == (plist)[1])
//  ����
//#define hasErLian(plist) (!isFENG((plist)[0]) && (strchr((plist), (plist)[0]-1))
//  ����
//#define hasKanZhang(plist) (!isFENG((plist)[0]) && (strchr((plist),(plist)[0]-2)))

#endif // MJ_Player_H
