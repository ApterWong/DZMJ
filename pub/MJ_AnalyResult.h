#ifndef MJ_ANALYRESULT_H
#define MJ_ANALYRESULT_H

#include <list>

#define DEBUG

class MJ_Player;
class MJ_AnalyResult
{
    friend class MJ_Player;
public:
    typedef char CARD;
    enum HU{
            //   �ܼ�26�к���
            //    1��
            HU_PingHu		= 0x00,					//  ƽ��
            HU_QiangGang 	= 0x01,					//	����
            HU_GangShangHua = 0x02,					//  ���Ͽ���
            //    2��
            HU_ZiMo			= 0x04,						//  ����
            HU_MenQianQing	= 0x08,						//  ����
            HU_XiaoDao	 	= 0x10,						//  С��
            HU_YouYiTao		= 0x20,						//  ��һ��
            HU_QiaoQiDui	= 0x40,						//  ���߶�
            HU_ShiSanLang	= 0x80,						//	ʮ����
            HU_YiTiaoLong	= 0x0100,					//  һ����
            HU_HunYiSe		= 0x0200,					//  ��һɫ
            HU_PengpengHu	= 0x0400,					//  ������
            HU_SiGuiYi		= 0x0800,					//  �Ĺ�һ
            HU_QuanQiuYi	= 0x1000,					//  ȫ��һ
            HU_DanWangZhua	= 0x2000,					//  ����ץ
            HU_WangGuiWei	= 0x4000,					//  ����λ
            //    4��
            HU_DaDao		= 0x8000,					//   ���
            HU_YouLiangTao	= 0x010000,					//   ������
            HU_QiFengHui	= 0x020000,					//	 �߷��
            HU_ShuangWangGuiWei = 0x040000,				//   ˫����λ
            HU_DanWangZhuaWang  = 0x080000,				//   ����ץ��
            HU_ShuangWangZhua	= 0x100000,				//   ˫��ץ
            HU_QingYiSe			= 0x200000,             // ��һɫ
            //    8��
            HU_ShuangWangZhuaWang = 0x400000,				//   ˫��ץ��
            HU_SanWangGuiWei	  = 0x800000,				//   ������λ
            //  16 ��
            //			���������ֺ���	*/
            HU_SiWangGuiWei	 	  = 0x01000000				//   ������λ
        };

    enum HU_FLAG{
        F_JiePao = 0,       // ����
        F_ZiMo = 1,         // ����
        F_QiangGang = 2,    // ���ܺ�
        F_GangShangHua = 4  // ���Ͽ���
    };

public:
    MJ_AnalyResult();
    MJ_AnalyResult(MJ_Player &);

    ~MJ_AnalyResult();

    int canHu(CARD *);

    int addHu(HU _HU);
    int calc_BeiShu(const MJ_Player &, CARD card, HU_FLAG flag/*���������ڡ����ܡ����ϻ���*/);//������ٷ�

    int H_ShiSanLang(CARD *);     // ʮ����
    int H_QiFengHui(CARD *);      // �߷��
    int H_QiaoQiDui(CARD *);      // ���ɶ�

    std::list<const char *> HU_names();
    int getFan();

private:
    void analy_sort(CARD *li, int len);

private:
    int st_count;
    int sl_count;
    int dz_count;
    int el_count;
    int kz_count;
    int dan_count;
    int w_count;
    int g_count;
    int p_count;
    int c_count;
    int flag;
    int fan;

    CARD _w;
    CARD gang[4];
    CARD peng[4];
    CARD chi[4];
    CARD st[4];
    CARD sl[4];
    CARD dz[8];
    CARD el[8];
    CARD kz[8];
    CARD dan[16];

    static const char *HuNames[32];

#ifdef DEBUG
public:
    void printResult();
#endif
};

#define forin_dz(dz, Count, into, cur, w) do{ \
    for(auto i=0; i<(Count); i++) \
    {\
        ((dz)[i]!=(w) && ((into)[(cur)++] = (dz)[i])) || ((into)[(cur)++] = (w)); \
    }}while(0);

#define forin_el(el, Count, into, cur, w) do{ \
    for(auto i=0; i<(Count); i++) \
    {\
        if(isMJCARD(el[i] + 1)) \
            (((el)[i]+1) != w && ((into)[(cur)++] = (el)[i]+1)) || (((into)[(cur)++] = w)); \
        if(isMJCARD(el[i] - 2)) \
            ((((el)[i]-2) != w) && ((into)[(cur)++] = (el)[i]-2)) || ((into)[(cur)++] = w); \
    }}while(0);

#define forin_kz(kz, Count, into, cur, w) forin_dz(kz, Count, into, cur, w)
#define forin_dan(dan, Count, into, cur, w) forin_dz(dan, Count, into, cur, w)

// �м���
#define hasYOUJITAO(li, len, result) do{\
    for(auto i=0; i<len-1; i++)\
    {\
        if(li[i] == li[i+1] || li[i] == li[i+1]+1)\
        {\
            *(result)+=1;\
            i++;\
        }\
    }\
}while(0)

#endif // MJ_ANALYRESULT_H
