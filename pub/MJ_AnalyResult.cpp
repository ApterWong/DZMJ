#include "MJ_AnalyResult.h"
#include "MJC_CardSet.h"
#include <cstring>

#ifdef DEBUG
#include <iostream>
#endif

MJ_AnalyResult::MJ_AnalyResult()
{
    memset(this, 0, sizeof(MJ_AnalyResult));
}

MJ_AnalyResult::~MJ_AnalyResult()
{

}

int MJ_AnalyResult::canHu(MJC_CardSet::CARD *ting)
{
    if(ting == NULL)
        throw(0);

    int tCount = 0;
    /***  *****  *****  *****   *****
     *      �����ĸ������࣬ �������������������Щ
     *      ������ץ��
     * *****
     *   0: (dz>=1 && dan==0) || dan==1
     *   1: kz+dz+el==0&&dan==0  ||                  dan==0  (����ץ)
     *      dan==1&&kz+dz+el==1
     *   2: dz+el+kz==1 && dan==0 ||                 dz+el+kz==1 &&dan=0(����ץ)
     *      dz+el+kz == 2 && dan=1 ||
     *      dz>1 && dan==0   ||
     *      dz==1 && dan==0        -dz
     *   3: dan==1 && dz+el+kz==0 ||                 ˫��ץ
     *      dan==0&&kz+dz+el==2 ||                        (����ץ)
     *      kz+el+dz==1 && dan==2                         kz+dz+el == 2&&dan==0 (����ץ)
     *      kz+dz+el==3&&dan==1
     *   4: dan==0 && dz+el+kz==0         ������λ
     *      kz+dz+el==1 && dan==1         ˫��ץ
     *      kz+dz+el==0 && dan==3
     *      kz+dz+el==3 && dan==0          ����ץ
     *      dz+el+kz==4 && dan==1
     */
            switch(w_count)
            {
            case 0:
                if((dz_count >= 1 && dan_count==0) || dan_count==1)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount,  _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                break;

            case 1:// ������һ���������������
                if(kz_count+dz_count+el_count==0 && dan_count==0)  // ��ץ
                {
                    ting[tCount++] = MJC_CardSet::MJ_WANG;
                }
                else if(dan_count==1 && kz_count+dz_count+el_count == 1)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                else
                {
                    ;
                }
                break;

            case 2:
//                dz+el+kz==1 && dan==0 ||       dz+el+kz==1 &&dan=0(����ץ)
//                dz+el+kz == 2 && dan=1 ||
//                dz==2&&el+kz==2 && dan==0   ||
//                dz==1 && kz+el==3&& dan==0        -dz
                if(dz_count + el_count + kz_count == 1 && dan_count==0)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)

                    ting[tCount++] = MJC_CardSet::MJ_WANG;
                }
                else if(dz_count+el_count+kz_count == 2 && dan_count == 1)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                else if(dz_count==2 && el_count+kz_count==2 && dan_count==0)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                }
                else if(dz_count==1 && kz_count+el_count==3 && dan_count==0)
                {
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                }
                else
                {

                }

                break;

            case 3:
//                dan==1 && dz+el+kz==0 ||                 ˫��ץ
//                dan==0&&kz+dz+el==2 ||                        (����ץ)
//                dz==1 && kz+el==0 && dan==2                         kz+dz+el == 2&&dan==0 (����ץ)
//                kz+dz+el==3&&dan==1
                if(dan_count==0 && dz_count+el_count+kz_count==0)
                {
                    ting[tCount++] = MJC_CardSet::MJ_WANG;//
                }
                else if(dan_count==0&&kz_count+el_count+dz_count == 2)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                }
                else if(dan_count==2 && dz_count == 1 && kz_count+el_count==0)
                {
                    forin_dan(dan, dan_count, ting, tCount, _w)
                    forin_dz(dz, dz_count, ting, tCount, _w);
                }
                else if(dan_count==1 && dz_count+kz_count+el_count==3)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                break;

            case 4:
//                *   4: dan==0 && dz+el+kz==0         ������λ
//                *      kz+dz+el==1 && dan==1         ˫��ץ
//                *      kz+dz+el==0 && dan==3
//                *      kz+dz+el==3 && dan==0          ����ץ
//                *      dz+el+kz==4 && dan==1
                if(dan_count==0 && dz_count+el_count+kz_count)
                {
                    ting[tCount++] = MJC_CardSet::MJ_WANG;//
                }
                else if(dz_count+kz_count+el_count==1 && dan_count==1)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                else if(kz_count+dz_count+el_count==3 && dan_count==0)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    ting[tCount++] = MJC_CardSet::MJ_WANG;//
                }
                else if(dz_count+el_count+kz_count==4 && dan_count==1)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                break;
            }
     return tCount;
}


#ifdef DEBUG
void MJ_AnalyResult::printResult()
{
    std::cout << "��ͬ��" << st_count << " | " <<  st << std::endl;
    std::cout << "������" << sl_count << " | " <<  sl << std::endl;
    std::cout << "���ӣ�" << dz_count << " | " <<  dz << std::endl;
    std::cout << "���ţ�" << kz_count << " | " <<  kz << std::endl;
    std::cout << "������" << el_count << " | " << el << std::endl;
    std::cout << "���ţ�" << dan_count << " | " <<  dan << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
}

#endif
