#include "MJ_Player.h"
#include "MJ_AnalyResult.h"
#include <cstring>
#include <stack>
#include <QDebug>

#ifdef DEBUG

  #include <iostream>
  using namespace std;

#endif

bool MJ_Player::inPaiList(MJ_Base::CARD c)
{/*
    char *b = this->paiList;
    char *e = this->paiList + paiCount;
    char *m;// = this->paiList + paiCount/2;

    while(b < e)
    {
        m = b + (e-b) / 2;
        if(*m == c)
            return true;
        else if(*m > c)
        {
            b = m+1;
        }
        else
            e = m-1;
    }
    return false;*/
    for(int i=0; i<paiCount; i++)
    {
        if(paiList[i] == c)
            return true;
    }
    return false;
}

bool MJ_Player::hasSanLian(MJ_Base::CARD *li, MJ_Base::CARD c, CARD **res)
{
    if(isFENG(c))
        return false;

    CARD *it = li;
    int flg = 0;
    while(*it && flg<3)
    {
        if(*it == c)
        {
            res[flg++] = it;
            c--;
        }
        it++;
    }
    if(flg == 3)
    {
        return true;
    }
    return false;
}

bool MJ_Player::hasErLian(MJ_Base::CARD *li, MJ_Base::CARD c, CARD **res)
{
    if(isFENG(c))
        return false;

    CARD *it = li;
    int flg = 0;
    while(*it && flg<2)
    {
        if(*it == c)
        {
            res[flg++] = it;
            c--;
        }
        it++;
    }
    if(flg == 2)
    {
        return true;
    }
    return false;
}

bool MJ_Player::hasKanZhang(MJ_Base::CARD *li, MJ_Base::CARD c, CARD **res)
{
    if(isFENG(c))
        return false;

    CARD *it = li;
    int flg = 0;
    while(*it && flg<2)
    {
        if(*it == c)
        {
            res[flg++] = it;
            c-=2;
        }
        it++;
    }
    if(flg == 2)
    {
        return true;
    }
    return false;
}

MJ_Player::MJ_Player()
{
    MJ_Base::init();
}

void MJ_Player::init(MJ_Player::pCCARD _paiList, MJ_Base::CARD _wang)
{
    MJ_Base::init();
    memset(this->cHuList, 0, sizeof(this->cHuList));
    memset(this->cGangList, 0, sizeof(this->cGangList));
    memset(this->cPengList, 0, sizeof(this->cPengList));
    memset(this->cChiList, 0, sizeof(this->cChiList));

    this->wang = _wang;

    this->paiCount = strlen(_paiList);
    if(this->paiCount > 13)
        throw(0);
    memcpy(this->paiList, _paiList, this->paiCount);
    MJ_sort(this->paiList, this->paiCount);

#ifdef DEBUG
    cout << "*��ʼ����Ϸ- �� "  << this->paiList << "������:" << this->paiCount << endl;
#endif
}

void MJ_Player::setWang(MJ_Base::CARD wang)
{
    this->wang = wang;
}

MJ_Base::CARD MJ_Player::getWang() const
{
    return this->wang;
}

void MJ_Player::addCard(MJ_Base::CARD nc)
{
    auto i = this->paiCount;
    for(; i >0; i--)
    {
        if(this->paiList[i-1] < nc)
        {
           this->paiList[i] = this->paiList[i-1];
           continue;
        }
        break;
    }

    this->paiList[i] = nc;
    this->paiCount++;
#ifdef DEBUG
    cout << "*������:" << nc << ", " << this->paiList << "  " << this->paiCount << endl;
#endif
}

int MJ_Player::ChuPai(MJ_Base::CARD c)
{
    int ret;

    ret =  DelCard(c);
    if(ret == 0)
    {
        this->MJ_sort(this->paiList, this->paiCount+1);
//        this->paiRecList[this->paiRecCount++] = c;

//        analysis();
//        cChiCount = cChi();
//        cPengCount = cPeng();
//        cGangCount = cGang();
//        cHuCount = cHu();
    }

    return ret;
}

int MJ_Player::DelCard(MJ_Base::CARD card)
{
    auto i = 0;
    bool flag = false;

    for (; i<this->paiCount; i++)
    {
        if(flag)
        {
            this->paiList[i-1] = this->paiList[i];
            continue;
        }
        if(this->paiList[i] == card)
        {
            flag = true;
        }
    }

    if (!flag)
    {
        return -1;
    }

    this->paiList[i-1] = MJ_noCard;
    this->paiCount -= 1;

    return 0;
}

/*  // �˺����ѷ���
int MJ_Player::DelCard(int offset)
{
    if (offset < 0 || offset >= this->paiCount)
    {
#ifdef DEBUG
        cout << "*ɾ�����ɹ�." << __FUNCTION__ << endl;
        cout << "*ɾ��һ����-��" << offset << " " <<  this->paiList << " " << this->paiCount << endl;
#endif
        return -1;
    }

    for(auto i=offset; i<this->paiCount-1; i++)
    {
        this->paiList[i] = this->paiList[i+1];
    }

    this->paiCount -= 1;
    this->paiList[this->paiCount] = MJ_noCard;

#ifdef DEBUG
        cout << "*ɾ��һ����-��" << offset << " " <<  this->paiList << " " << this->paiCount << endl;
#endif
    return 0;
}
*/

/***  *****  *****  *****  [����]  *****  *****  *****
 * ���ƹ��򣺷粻�ܳԣ�  �����ܳԣ�   �������з磩���Գԣ�ǰ���Ǳ��������Ƿ������£�,�����������ԭ��,   ���Գ����Աߵ���
 * �������ϵ�����������õ�  ? > �� > Ͳ > ��
 * �����������2�ֳԷ���5����ʽ
 *      ȥ�� -> ֻ����ɳԵ��Ʋ�������γ�
 *      ���� -> �ú��д���
 *      �к��У�
*/
int MJ_Player::cChi()
{
    CARD cards[16] = {0};
    int count = 0;

    memset(this->cChiList, 0, sizeof(this->cChiList));
    //  ȥ��
    for(auto i=0; i<this->paiCount; i++)
    {
        if(this->paiList[i] == this->paiList[i+1])
            continue;

        // �������к��У� ������
        if(this->paiList[i] == MJ_ZHONG)
        {
            for(auto i=0; i<count; i++)
            {
                if(cards[i] > this->wang)         // ��λ��
                    continue;
                else if(cards[i] == this->wang)     //������
                    break;
                else
                {
                    for(auto j=count; j>i; cards[j] = cards[j-1], j--);
                    cards[i] = this->wang;
                    count++;
                    break;
                }
            }
        }

        if(isFENG(this->paiList[i]))   // ���˵� ����
            continue;

        cards[count] = this->paiList[i];
        count++;
    }

    if(count <= 1)// �������Ų�ͬ���� ���Բ����ж���
    {
        //  ��Ҫ���Ǹ��µ��������
        memset(this->cChiList, 0, sizeof(this->cChiList));
        return 0;
    }

    int cLen = 0;
    for(auto i=0; i<count - 1; i++)
    {
        CARD tmp;
        cout << cards[i];
        if(cards[i] == cards[i+1] + 1)  // ����
        {
            tmp = cards[i] + 1;
            if(isMJCARD(tmp))
            {
                if(tmp == this->wang && !strchr(this->cChiList, MJ_ZHONG))   //�������ܳԣ� �ú��д���
                    this->cChiList[cLen++] = MJ_ZHONG;
                else if(!strchr(this->cChiList, tmp))
                    this->cChiList[cLen++] = tmp;
                else
                {;}
            }

            tmp = cards[i] - 2;
            if(isMJCARD(tmp))
            {
                if(tmp == this->wang && !strchr(this->cChiList, MJ_ZHONG))
                    this->cChiList[cLen++] = MJ_ZHONG;
                else if(!strchr(this->cChiList, tmp))
                    this->cChiList[cLen++] = tmp;
                else
                {
                    ;
                }
            }
        }
        else if (cards[i] == cards[i+1] + 2) // ����
        {
            tmp = cards[i] - 1;
            if(isMJCARD(tmp))
            {
                if(tmp == this->wang && !strchr(this->cChiList, MJ_ZHONG))
                    this->cChiList[cLen++] = MJ_ZHONG;
                else if(!strchr(this->cChiList, tmp))
                    this->cChiList[cLen++] = tmp;
            }
        }
        else
        {

        }

        if(i>0 && cards[i-1]-1 == cards[i] && cards[i] == cards[i+1]+1)   // ����һ�����Գ�
        {
            tmp = cards[i];
            if(tmp == this->wang && !strchr(this->cChiList, MJ_ZHONG))
                this->cChiList[cLen++] = MJ_ZHONG;
            else if(!strchr(this->cChiList, tmp))
                this->cChiList[cLen++] = tmp;
        }
    }

    this->cChiCount = cLen;
    return cLen;
}

int MJ_Player::cPeng()
{
    int count = 0;
    for(auto i=0; i<this->paiCount - 1; i++)
    {
        if(this->paiList[i] == this->paiList[i+1])  // ��ͬ��������
        {
            if(paiList[i] != this->wang)
                this->cPengList[count++] = this->paiList[i];

            if(this->paiList[i] == this->paiList[i + 2])    //�п���������ͬ��  ����������ͬ��
            {
                i += 2;
            }
            else if(this->paiList[i] == this->paiList[i + 3])
            {
                i += 3;
            }
            else
                i++;
        }
    }

    this->cPengCount = count;
    return count;
}

int MJ_Player::cGang()
{
    int count = 0;

//    for(int i=0; i<this->_p; i++)
//    {
//        this->cGangList[count++] = this->peng[i];
//    }

    for(auto i=0; i<this->paiCount-2; i++)
    {
        if(this->paiList[i] == this->paiList[i+1] &&
                this->paiList[i] == this->paiList[i+2])
        {
            if(paiList[i] != this->wang)
                this->cGangList[count++] = this->paiList[i];
            if(this->paiList[i] == this->paiList[i+3])
                i += 3;
            else
                i += 2;
        }
    }

    this->cGangCount = count;
    return count;
}

int MJ_Player::cHu()
{
    cHuCount = 0;
    for(auto it : this->AnalyResults)
    {
        this->cHuList[cHuCount++] = it.first;
    }
    return cHuCount;
}

int MJ_Player::analysis()
{
    if(!(this->paiCount != 1 || this->paiCount != 4 ||
            this->paiCount != 10 || this->paiCount != 13))
        return -1;

    AnalyResults.clear();
/**** ��ǰ�ж�  ������� --  ʮ����  ���߶�   �߷��
 *   ����  ������� 13��
*/
    if(this->paiCount == 13)
    {
        CARD list[16] = {0};
        MJ_AnalyResult analy;
        int ret = 0;
        analy.dan_count = this->paiCount;
        memcpy(analy.dan, this->paiList, 16);

        MJ_sort(analy.dan, analy.dan_count);
        ret = analy.H_QiFengHui(list);
        for(auto i=0; i<ret; i++)
        {
            AnalyResults[list[i]] = analy;
        }
        if(ret)
            return 0;

        analy.dan_count = this->paiCount;
        memcpy(analy.dan, this->paiList, 16);

        MJ_sort(analy.dan, analy.dan_count);
        ret = analy.H_ShiSanLang(list);
        for(auto i=0; i<ret; i++)
        {
            AnalyResults[list[i]] = analy;
        }
        if(ret)
            return 0;

        analy.dan_count = this->paiCount;
        memcpy(analy.dan, this->paiList, 16);

        MJ_sort(analy.dan, analy.dan_count);
        //���߶� ��Ҫ�� ����ȡ����
        for(auto i=0; i<this->paiCount; i++)
        {
            if(analy.dan[i] == this->wang)
            {
                analy.dan[i] = MJ_Player::NewCard;
                analy.w_count++;
            }
        }
        MJ_sort(analy.dan, analy.dan_count);
        analy.dan_count -= analy.w_count;
        ret = analy.H_QiaoQiDui(list);

        for(auto i=0; i<ret; i++)
        {
            AnalyResults[list[i]] = analy;
        }
        if(ret)
            return 0;
    }

    typedef struct _node{
        typedef enum{
            W_ST, W_SL, W_DZ, W_EL, W_KZ, W_DAN
        }WHO;

        typename MJ_Base::CARD pai[16];    //
        int count;  // ����
        int pos;    // ����λ��
        WHO who;    // ѹջ��
    }Node;

    MJ_AnalyResult analy;

    CARD pai[16];//ѭ�����õ�����
    int pCount;//ѭ�����õ����Ƴ���

    Node node;
    Node::WHO whoFlg;

    std::stack<Node> STACK;

    //ǰ��׼��
    //AnalyResults.clear();
    memset(&node, 0, sizeof(node));
    memcpy(node.pai, this->paiList, this->paiCount);
    node.count = this->paiCount;
    node.pos = 0;
    node.who = Node::W_ST;
    analy._w = this->wang;
    analy.st_count=1;

    for(auto i=0; i<node.count; i++)
    {
        if(node.pai[i] == this->wang)   // ��ȡ���ĸ���
        {
            analy.w_count++;
            node.pai[i] = MJ_noCard;
        }
        else if(node.pai[i] == MJ_ZHONG)    // ������������ԭ��
        {
            node.pai[i] = this->wang;
        }
    }
    MJ_sort(node.pai, node.count);
    node.count -= analy.w_count;
    STACK.push(node);

    while(!STACK.empty())
    {
        Node n = STACK.top(); STACK.pop();
        memcpy(pai, n.pai, 16);
        pCount = n.count;
        whoFlg = n.who;
        auto i=n.pos;

        switch(whoFlg)      //
        {
        case Node::W_ST:
            analy.st_count--;
            analy.st[analy.st_count] = 0;
            break;
        case Node::W_SL:
            analy.sl_count--;
            analy.sl[analy.sl_count] = 0;
            break;
        case Node::W_DZ:
            analy.dz_count--;
            analy.dz[analy.dz_count] = 0;
            break;
        case Node::W_EL:
            analy.el_count--;
            analy.el[analy.el_count] = 0;
            break;
        case Node::W_KZ:
            analy.kz_count--;
            analy.kz[analy.kz_count] = 0;
            break;
        case Node::W_DAN:
            break;
        }

        /****************************************************
         *      ѡ��������ͬ���� ���
         *      ÿѡһ�飬��ԭ��¼ѹջ������ԭ�ȵ�������λ�ã��´�����ԭ��¼
         *   ������
         */
        for(; whoFlg==Node::W_ST && i<pCount-2; i++)
        {
            if(hasSanTong(pai + i))
            {
                Node tmp;
                memcpy(tmp.pai, pai, 16);
                tmp.count = pCount;
                tmp.pos = i+3;          // �ı� pos  ����
                tmp.who = Node::W_ST;
                STACK.push(tmp);

                analy.st[analy.st_count++] = pai[i];

                for(auto j=i; j<pCount; pai[j] = pai[j+3],j++);
                pCount -= 3;

                i = -1;
                continue;
            }
        }
        if(whoFlg == Node::W_ST && i >= pCount-2)//���ﲢ���Ǳ�ʾpai��û����ͬ�������Ƶ����
                                        //  ��ʱ����п�������ͬ����ϲ�ֳ���������� ��õĺ�����������
        {
            i = 0;
            whoFlg = Node::W_SL;    //
        }

        /****************************************************
         *   �ҳ��������
         */
        for(; whoFlg==Node::W_SL && i<pCount-2; i++)
        {
            CARD *res[3] = {0};
            if(hasSanLian(pai, pai[i], res))
            {
                Node tmp;
                memcpy(tmp.pai, pai, 16);
                tmp.count = pCount;
                tmp.pos = i+3;          // �ı� pos  ����
                tmp.who = Node::W_SL;
                STACK.push(tmp);

                analy.sl[analy.sl_count++] = pai[i];

                *res[0] = MJ_noCard;
                *res[1] = MJ_noCard;
                *res[2] = MJ_noCard;
                this->MJ_sort(pai, pCount);
                pCount -= 3;

                i = -1;
                continue;
            }
        }
        if(whoFlg == Node::W_SL && i >= pCount-2)
        {
            i = 0;
            whoFlg = Node::W_DZ;
        }

        /***
         *      �����������ˣ��� <= 1�� && ʣ�µ��ƻ�����5��
         *                 �� == 2�� && ʣ�µ��ƻ�����10��
         *                 �� == 3�� && ʣ�µ��ƻ�����10��
         *      ���������ǲ�������ˣ�
         */
        if((analy.w_count<=1 && pCount>=5) ||
                (analy.w_count==2 && pCount>10) ||
                (analy.w_count==3 && pCount>10))
            continue;

        /*****
         *  �Ҷ���
         */
        for(; whoFlg==Node::W_DZ && i<pCount-1; i++)
        {
            if(hasDuiZi(pai+i))
            {
                Node tmp;
                memcpy(tmp.pai, pai, 16);
                tmp.count = pCount;
                tmp.pos = i+2;          // �ı� pos  ����
                tmp.who = Node::W_DZ;
                STACK.push(tmp);

                analy.dz[analy.dz_count++] = pai[i];

                for(auto j=i; j<pCount; pai[j] = pai[j+2],j++);
                pCount -= 2;

                i = -1;
                continue;
            }
        }
        if(whoFlg == Node::W_DZ && i >= pCount-1)
        {
            i = 0;
            whoFlg = Node::W_KZ;
        }

        /*****
         *   �ҿ���
         */
        for(; whoFlg==Node::W_KZ && i<pCount-1; i++)
        {
            CARD *res[2] = {NULL};
            if(hasKanZhang(pai, pai[i], res))
            {
                Node tmp;
                memcpy(tmp.pai, pai, 16);
                tmp.count = pCount;
                tmp.pos = i+2;          // �ı� pos  ����
                tmp.who = Node::W_KZ;
                STACK.push(tmp);

                analy.kz[analy.kz_count++] = pai[i] - 1;

                *res[0] = MJ_noCard;
                *res[1] = MJ_noCard;
                this->MJ_sort(pai, pCount);
                pCount -= 2;

                i = -1;
                continue;
            }
        }
        if(whoFlg == Node::W_KZ && i >= pCount-1)
        {
            i = 0;
            whoFlg = Node::W_EL;
        }

        /*****
         *
         */
        for(; whoFlg==Node::W_EL && i<pCount-1; i++)
        {
            CARD *res[2] = {NULL};
            if(hasErLian(pai, pai[i], res))
            {
                Node tmp;
                memcpy(tmp.pai, pai, 16);
                tmp.count = pCount;
                tmp.pos = i+2;          // �ı� pos  ����
                tmp.who = Node::W_EL;
                STACK.push(tmp);

                analy.el[analy.el_count++] = pai[i];

                *res[0] = MJ_noCard;
                *res[1] = MJ_noCard;
                this->MJ_sort(pai, pCount);
                pCount -= 2;

                i = -1;
                continue;
            }
        }

        /****
         *  ʣ�µľ��ǵ�����
         */
        analy.dan_count = pCount;
        analy.dan_count && memcpy(analy.dan, pai, pCount);
        analy.dan_count || memset(analy.dan, 0, 8);

        CARD allTing[16] = {0};
        int tingCount = 0;
        if(tingCount = analy.canHu(allTing))
        {
            for(auto i=0; i<tingCount; i++)
            {
                qDebug() << __FUNCTION__;
                qDebug() << "\tAllting:" << allTing;
                CARD tmp = allTing[i];
                if(tmp != this->wang)  //����֮ǰ���滻�ɺ���
                {
                    AnalyResults[tmp] = analy;
                }
                else{
                    AnalyResults[MJ_ZHONG] = analy;
                }
            }
        }

    }

    return 0;
}

void MJ_Player::AnalysisHGPC()
{
    analysis();
    cChi();
    cPeng();
    cGang();
    cHu();

    qDebug() << __FUNCTION__ << __LINE__ << " :";
    qDebug() << "\tcChiCount:" << cChiCount << this->cChiList << endl
             << "\tcPengCount:" << cPengCount << this->cPengList << endl
             << "\tcGangCount:" << cGangCount << this->cGangList << endl
             << "\tcHuCount:" << cHuCount << this->cHuList;
}

// ���ط����õ� �������� ���
int MJ_Player::getCanChiList(CARD *lst, int c) const
{
    if(lst == nullptr)
        return this->cChiCount;
    memcpy(lst, this->cChiList, c);

    return this->cChiCount;
}

int MJ_Player::getCanPengList(CARD *lst, int c) const
{
    if(lst == nullptr)
        return this->cPengCount;
    memcpy(lst, this->cPengList, c);

    return this->cPengCount;
}

int MJ_Player::getCanGangList(CARD *lst, int c) const
{
    if(lst == nullptr)
        return this->cGangCount;
    memcpy(lst, this->cGangList, c);

    return this->cGangCount;
}

int MJ_Player::getCanHuList(CARD *lst, int c) const
{
    if(lst == nullptr)
        return this->cHuCount;
    memcpy(lst, this->cHuList, c);

    return this->cHuCount;
}

//  �������Բ�����
//  ��дMJ_Base::
int MJ_Player::Chi(MJ_Base::CARD card, pCCARD ll)
{
    qDebug() << "Player::Chi:" << ll << "card = " << card;
    for(int i=0; i<3; i++)
    {
        if(card != ll[i])
            this->DelCard(ll[i]);

        this->chi[_c++] = ll[i];
    }
    this->gpc_seq[this->gpc_seq_count++] = SEQ_CHI;
//    this->MJ_sort(this->paiList, this->paiCount);

    return 0;
}

int MJ_Player::Peng(MJ_Base::CARD card)
{
    this->DelCard(card);
    this->DelCard(card);

    this->peng[_p++] = card;
    this->gpc_seq[this->gpc_seq_count++] = SEQ_PENG;

//    this->MJ_sort(this->paiList, this->paiCount);

    return 0;
}

int MJ_Player::Gang(MJ_Base::CARD card)
{
    for(int i=0; i<this->_p; i++)
    {
        if(card == this->peng[i]) // ת��
        {
            this->UndoPeng(card);
            this->gang[_g++] = card;
            this->gpc_seq[this->gpc_seq_count++] = SEQ_GANG;
            this->DelCard(card);
            return 0;
        }
    }

    this->DelCard(card);
    this->DelCard(card);
    this->DelCard(card);

    this->gang[_g++] = card;
    this->gpc_seq[this->gpc_seq_count++] = SEQ_GANG;

//    this->MJ_sort(this->paiList, this->paiCount);

    return 0;
}

int MJ_Player::Hu(CARD , pCCARD )
{
    return 0;
}

// �ϼҳ����ƿ��Գԣ�������ô�ԣ����سԷ�
/**** BUG: �з�ĳԷ� *****/
int MJ_Player::getCChiList(MJ_Base::CARD card, MJ_Base::CARD (*res)[4])
{
    if(res == nullptr)
        return -1;

    CARD a, b, cd;
    int count = 0;

    if(card == MJ_Base::MJ_ZHONG)
        cd = this->wang;
    else
        cd = card;

    a = cd + 2;
    b = cd + 1;
    // abc   789
    if(isMJCARD(a) && isMJCARD(b))//�ǺϷ�����
    {
        if(inPaiList(a) && inPaiList(b))//������������������
        {
            res[count][0] = card;
            res[count][1] = b;
            res[count][2] = a;
            count++;
        }
        if(inPaiList(b) && (a == this->wang) && inPaiList(MJ_Base::MJ_ZHONG))
        {
            res[count][0] = card;
            res[count][1] = b;
            res[count][2] = MJ_ZHONG;
            count++;
        }
        if(inPaiList(a) && (b == this->wang) && inPaiList(MJ_ZHONG))
        {
            res[count][0] = card;
            res[count][1] = MJ_ZHONG;
            res[count][2] = a;
            count++;
        }
    }

    a = card - 1;
    b = card + 1;
    if(isMJCARD(a) && isMJCARD(b))
    {
        if(inPaiList(a) && inPaiList(b))
        {
            res[count][1] = card;
            res[count][0] = a;
            res[count][2] = b;
            count++;
        }
        if(inPaiList(b) && (a == this->wang) && inPaiList(MJ_Base::MJ_ZHONG))
        {
            res[count][1] = card;
            res[count][0] = MJ_ZHONG;
            res[count][2] = b;
            count++;
        }
        if(inPaiList(a) && (b == this->wang) && inPaiList(MJ_ZHONG))
        {
            res[count][1] = card;
            res[count][0] = a;
            res[count][2] = MJ_ZHONG;
            count++;
        }
    }

    a = card - 2;
    b = card - 1;
    if(isMJCARD(a) && isMJCARD(b))
    {
        if(inPaiList(a) && inPaiList(b))
        {
            res[count][2] = card;
            res[count][0] = a;
            res[count][1] = b;
            count++;
        }
        if(inPaiList(b) && (a == this->wang) && inPaiList(MJ_Base::MJ_ZHONG))
        {
            res[count][2] = card;
            res[count][0] = MJ_ZHONG;
            res[count][1] = b;
            count++;
        }
        if(inPaiList(a) && (b == this->wang) && inPaiList(MJ_ZHONG))
        {
            res[count][2] = card;
            res[count][0] = a;
            res[count][1] = MJ_ZHONG;
            count++;
        }
    }


    return count;
}

bool MJ_Player::testHu(MJ_Base::CARD c)
{
    if(c == MJ_noCard)
        return false;

    for(int i=0; i<cHuCount; i++)
    {
        if(c == this->cHuList[i])
            return true;
    }

    return false;
}

bool MJ_Player::testPeng(MJ_Base::CARD c)
{
    if(c == MJ_noCard)
        return false;

    for(auto i=0; i<this->cPengCount; i++)
    {
        if(c == this->cPengList[i])
            return true;
    }

    return false;
}

bool MJ_Player::testGang(MJ_Base::CARD c)
{
    if(c == MJ_noCard)
        return false;

    for(auto i=0; i<this->cGangCount; i++)
    {
        if(c == this->cGangList[i])
            return true;
    }

    return false;
}

bool MJ_Player::testChi(MJ_Base::CARD c)
{
    if(c == MJ_noCard)
        return false;

    qDebug() << __FUNCTION__ << __LINE__ << "test card:" << c;
    qDebug() << "\tcCount = " << cChiCount << ", chilist:" << this->cChiList;
    for(auto i=0; i<this->cChiCount; i++)
    {
        if(c == this->cChiList[i])
            return true;
    }

    return false;
}

// ����
MJ_Base::CARD MJ_Player::testAnGang()
{
    for (int i=0; i<this->paiCount; i++)
    {
        if( this->paiList[i] != this->wang &&
                paiList[i] == paiList[i+1] &&
                paiList[i] == paiList[i+2] &&
                paiList[i] == paiList[i+3] &&
                paiList[i] == paiList[i+4])
        {
            return paiList[i];
        }
    }

    return MJ_noCard;
}

bool MJ_Player::testBuGang(MJ_Base::CARD c)
{
    if(c == MJ_noCard)
        return false;

    for(int i=0; i< this->_p; i++)
    {
        if(c == peng[i])
            return true;
    }
    return false;
}

bool MJ_Player::testZiMo(MJ_Base::CARD c)
{
    if(c == MJ_noCard)
        return false;

    for(int i=0; i<cHuCount; i++)
    {
        if(cHuList[i] == MJ_WANG) // ��ץ
            return true;
        if(c == cHuList[i])//����
            return true;
    }
    return false;
}

int MJ_Player::setcHuList(MJ_Base::CARD _h[])
{
    if(_h == nullptr)
       return -1;
    int len = strlen(_h);
    memcpy(this->cHuList, _h, 16);
    this->cHuCount = len;
    return 0;
}

int MJ_Player::setcGangList(MJ_Base::CARD _g[])
{
    if(_g == nullptr)
        return -1;
    int len = strlen(_g);
    memcpy(this->cGangList, _g, 8);
    this->cGangCount = len;
    return 0;
}

int MJ_Player::setcPengList(MJ_Base::CARD _p[])
{
    if(_p == nullptr)
            return -1;
    int len = strlen(_p);
    memcpy(this->cPengList, _p, 8);
    this->cPengCount = len;
    return 0;
}

int MJ_Player::setcChiList(MJ_Base::CARD _c[])
{
    if(_c == nullptr)
        return -1;
    int len = strlen(_c);
    memcpy(this->cChiList, _c, 8);
    this->cChiCount = len;
    return 0;
}

int MJ_Player::copy_chiList(MJ_AnalyResult *to)
{
    to->c_count = this->_c;
    memcpy(to->chi, this->chi, this->_c * 3);
    return this->_c;
}

int MJ_Player::copy_pengList(MJ_AnalyResult *to)
{
    to->p_count = this->_p;
    memcpy(to->peng, this->peng, this->_p);
    return this->_p;
}

int MJ_Player::copy_gangList(MJ_AnalyResult *to)
{
    to->g_count = this->_g;
    memcpy(to->gang, this->gang, this->_g);
    return this->_g;
}

MJ_AnalyResult MJ_Player::getAnalyResult(MJ_Base::CARD c) const
{
    for(auto it : AnalyResults)
    {
        if(it.first == c)
        {
            return it.second;
        }
    }
    return MJ_AnalyResult();
}

void MJ_Player::MJ_sort(typename MJ_Base::CARD *li, int len)
{
    for(int i=0; i<len-1; i++)
    {
        for (int j=0; j<len-1-i; j++)
        {
            if(li[j] < li[j+1])
            {
                CARD t = li[j];
                li[j] = li[j+1];
                li[j+1] = t;
            }
        }
    }
}

#ifdef DEBUG
void MJ_Player::printTest()
{
    cout << "**********************************************" << endl;
    cout << "* ���ϵ���: " << this->paiList << "�� " << this->paiCount << " ��" << endl;
    cout << "* ���Գԣ�" << this->cChiList << endl;
    cout << "* ��������" << this->cPengList << endl;
    cout << "* ���Ըܣ�" << this->cGangList << endl;
    cout << "* ���Ժ���" << this->cHuList << endl;
    cout << "* ���Ժ���";
    for(auto it : AnalyResults) {
        it.second.calc_BeiShu(*this, it.first, MJ_AnalyResult::F_JiePao);
        cout << it.first << ": " << it.second.getFan() << "  ";
        list<const char *> lst = it.second.HU_names();

        for(list<const char *>::iterator x = lst.begin(); x !=lst.end(); ++x)
        {
            cout << *x << "��";
        }
        cout << endl;
    }

    cout << "\n**********************************************" << endl;
}

void MJ_Player::printHu()
{

}
#endif
