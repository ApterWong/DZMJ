#ifndef MJ_PLAYER_H
#define MJ_PLAYER_H

#include <map>
#include <string>
#include "MJ_AnalyResult.h"
#include "MJ_Base.h"

class MJ_Player : public MJ_Base
{
    friend class MJ_AnalyResult;

private:

    //  ���������  �� �������� ��
    CARD cHuList[16];
    CARD cChiList[16];
    CARD cGangList[8];
    CARD cPengList[8];

    CARD wang;

    _L cHuCount;
    _L cPengCount;
    _L cGangCount;
    _L cChiCount;

    std::map<CARD, MJ_AnalyResult> AnalyResults;
private:
    bool inPaiList(CARD c);

    bool hasSanLian(CARD*li, CARD c, CARD **res);
    bool hasErLian(CARD*li, CARD c, CARD **res);
    bool hasKanZhang(CARD*li, CARD c, CARD **res);

    void MJ_sort(typename MJ_Player::CARD *a, int len);

    // ������ �����ܺ�
    // ����ֵ��>=0ƥ�������� <0����������
    int cChi() ;
    int cPeng() ;
    int cGang() ;
    int cHu();
    int analysis();

public:

    MJ_Player();

    // ���ֳ�ʼ�� �� ���� ���Ҿ���
    void init(pCCARD _paiList, CARD _wang) override;

    void setWang(CARD wang);
    CARD getWang()const;

    // ��ץ�����Ʒ�������
    void addCard(CARD newCard);

    int ChuPai(CARD c) override;
    // ɾ��һ���ƣ�����-1����-2�� ��-2�� ��-3��
    int DelCard(CARD card);
    //int DelCard(int offset);

    void AnalysisHGPC();

    int getCanChiList(CARD *lst, int c)const;
    int getCanPengList(CARD *lst, int c)const;
    int getCanGangList(CARD *lst, int c)const;
    int getCanHuList(CARD *lst, int c)const;

    int Hu(CARD, pCCARD) override;
    int Gang(CARD c) override;
    int Peng(CARD c) override;
    int Chi(CARD c, pCCARD ll) override;

    //����һ���� ��ȡ�����ԳԵ�����ϡ�
    //����ֵ��-1  ������card����0��ʾû�п��ԳԵģ�������������ʾres���ж�����
    //res��Ž�������̰߳�ȫ
    int getCChiList(CARD card, CARD (*res)[4]);
    bool testHu(CARD c);
    bool testPeng(CARD c);
    bool testGang(CARD c);
    bool testChi(CARD c);

    // ����Ƿ�ɲ���
    bool testBuGang(CARD c);
    bool testZiMo(CARD c);
    CARD testAnGang();

    //svr ���õ���ֱ��������ҷ����Ľ����������
    //����ֵ����Щ����������֤���ܣ�ͨ�����أ�list����������������-1
    int setcHuList(CARD _h[]);
    int setcGangList(CARD _g[]);
    int setcPengList(CARD _p[]);
    int setcChiList(CARD _c[]);

    int copy_chiList(MJ_AnalyResult *);
    int copy_pengList(MJ_AnalyResult *);
    int copy_gangList(MJ_AnalyResult *);

    MJ_AnalyResult getAnalyResult(CARD c)const;

#ifdef DEBUG
    void printTest();
    void printHu();
#endif
};


#endif // MJ_Player_H
