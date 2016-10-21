#include "MJ_LocalServer.h"

static MJ_Base::CARD cardSet[] = {
    'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',//��
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',//��
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',//Ͳ
    'A', 'B', 'C', 'D', 'E', 'F', 'G',          //��

    'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G',

    'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G',

    'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G'
};

MJ_LocalServer::MJ_LocalServer(QObject *parent) : QObject(parent)
{
    for(auto i=0; i<4; i++)
        this->member[i] = new MJ_Player();

    cur_id = 0;
    zhuang_id = -1;
    paiCount = 0;
    maxPaiCount = 136;

    memset(this->paiList, 0, this->paiCount);

    stat = this->SVR_normal;//����ģʽ
    tmOut = new QTimer(this);
    connect(this->tmOut, SIGNAL(timeout()), this, SLOT(tmOutSlot()), Qt::QueuedConnection);
}

MJ_LocalServer::~MJ_LocalServer()
{
    delete this->member[0];
    delete this->member[1];
    delete this->member[2];
    delete this->member[3];
}

void MJ_LocalServer::send(MJ_response &resp)
{
    emit responseSignal(resp);
}

//���֣�ѡ����ѡׯ�����ƣ���ʼ����13�ţ���
void MJ_LocalServer::start()
{
    MJ_Base::CARD cards[4][16] = {0};
    maxPaiCount = 136;
    BuGang_falg = false;
    f_HGPC_valid = false;
    wj_ready = 0;//��һ�û��������ϵ���

    srand(time(NULL));

    //if(zhuang_id < 0)  // ��һ��
    //    this->zhuang_id = rand() % 4; // ѡׯ
    this->zhuang_id = 0;
    cur_id = zhuang_id;//��ǰ������

    //ϴ��
    int cardCount[136] = {0};
    for(auto i=0; i<136; i++)
    {
        int r = rand() % 136;
        while(cardCount[r] != 0) r++, r %= 136;

        paiList[i] = cardSet[r];
        cardCount[r] = 1;//�����ѡ
    }
    //ѡ��
    shaizi1 = rand()%6+1;
    shaizi2 = rand()%6+1;

    maxPaiCount -= (shaizi1 + shaizi2) * 2;
    this->wang = paiList[maxPaiCount];
    switch(this->wang)
    {
    case MJ_Base::MJ_JIUWAN:
        this->wang = MJ_Base::MJ_YIWAN;
        break;
    case MJ_Base::MJ_JIUTONG:
        this->wang = MJ_Base::MJ_YITONG;
        break;
    case MJ_Base::MJ_JIUTIAO:
        this->wang = MJ_Base::MJ_YITIAO;
        break;
    case MJ_Base::MJ_BAN:
        this->wang = MJ_Base::MJ_DONG;
        break;
    default:
        this->wang += 1;
    }

    //����
    for(int i=0; i<13; i++)
    {
        cards[0][i] = this->paiList[paiCount++];
        cards[1][i] = this->paiList[paiCount++];
        cards[2][i] = this->paiList[paiCount++];
        cards[3][i] = this->paiList[paiCount++];
    }

    this->member[0]->init(cards[0], this->wang);
    this->member[1]->init(cards[0], this->wang);
    this->member[2]->init(cards[0], this->wang);
    this->member[3]->init(cards[0], this->wang);

    // ���Ϳ�����Ϣ
    MJ_response resp;
    resp.setCard(this->wang);
    resp.setPaiList(cards[0]);
    resp.setSendTo(0);
    resp.setType(MJ_response::T_Init);
    this->send(resp);

    resp.setCard(this->wang);
    resp.setPaiList(cards[1]);
    resp.setSendTo(1);
    resp.setType(MJ_response::T_Init);
    this->send(resp);

    resp.setCard(this->wang);
    resp.setPaiList(cards[2]);
    resp.setSendTo(2);
    resp.setType(MJ_response::T_Init);
    this->send(resp);

    resp.setCard(this->wang);
    resp.setPaiList(cards[3]);
    resp.setSendTo(3);
    resp.setType(MJ_response::T_Init);
    this->send(resp);
}

void MJ_LocalServer::faPai()
{
    MJ_response resp;
    if(paiCount == maxPaiCount)
    {
        resp.setType(MJ_response::T_GMOver);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        send(resp);
        return ;
    }

    this->card = this->paiList[paiCount++];

    resp.setSendTo(cur_id);
    resp.setType(MJ_response::T_FaPai);
    resp.setCard(this->card);

    send(resp);

    this->tmChuPai->start(15000);//����15��
}

void MJ_LocalServer::resl_chuPai(MJ_RequestData &req)
{
    int senderID = req.getSenderID();
    if(senderID >= 4 || senderID < 0)
        return ;

    MJ_Base::CARD cd = req.getCard();
    MJ_response resp;
    resp.setType(MJ_response::T_Ok);
    resp.setSendTo(senderID);
    send(resp);

    cur_id = senderID;
    mem_policy[senderID] = P_None;

    int j = senderID + 1;
    j %= 4;//��λ���¼�
    mem_policy[j] = P_None;
    if(member[j]->testHu(cd))
        mem_policy[j] |= (int)P_Hu;
    if(member[j]->testGang(cd))
        mem_policy[j] |= P_Gang;
    if(member[j]->testPeng(cd))
        mem_policy[j] |= P_Peng;
    if(member[j]->testChi(cd))
        mem_policy[j] |= P_Chi;
    if(mem_policy[j] != P_None) // ��ѡ��
    {
        this->stat = SVR_vote;//�ȴ�����
    }

    j += 1; j %= 4;
    mem_policy[j] = P_None;
    if(member[j]->testHu(cd))
        mem_policy[j] |= P_Hu;
    if(member[j]->testGang(cd))
        mem_policy[j] |= P_Gang;
    if(member[j]->testPeng(cd))
        mem_policy[j] |= P_Peng;
    if(mem_policy[j] != P_None) // ��ѡ��
    {
        this->stat = SVR_vote;//�ȴ�����
    }

    j += 1; j %= 4;
    mem_policy[j] = P_None;
    if(member[j]->testHu(cd))
        mem_policy[j] |= P_Hu;
    if(member[j]->testGang(cd))
        mem_policy[j] |= P_Gang;
    if(member[j]->testPeng(cd))
        mem_policy[j] |= P_Peng;
    if(mem_policy[j] != P_None) // ��ѡ��
    {
        this->stat = SVR_vote;//�ȴ�����
    }

    //֪ͨ������ң�ĳ��ҳ���һ����
    resp.setType(MJ_response::T_ChuPai);
    resp.setCard(cd);
    resp.setWho(senderID);
    resp.setSendTo(MJ_response::SDT_Broadcast);
    send(resp);

    if(this->stat == SVR_vote) // ��������ҿ��� ��������
    {
        current_policy = P_None;
        resp.setType(MJ_response::T_Wait);
        resp.setWho(senderID);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        send(resp);

        this->tmOut->start(10000);//10sѡ��ʱ��
        f_HGPC_valid = true;
    }
    else if(this->stat == SVR_normal) // ��������
    {
        cur_id ++;
        cur_id %= 4;
        this->faPai();
    }
    else   // �����ˣ�
    {
        // do print err log
    }
}

void MJ_LocalServer::resl_HGPCList(MJ_RequestData &request)
{
    int senderID = request.getSenderID();
    if(senderID >= 4 || senderID < 0)
        return ;

    if((wj_ready & 0x0f) != 0x0f)
    {
        wj_ready |= senderID + 1;
    }

    MJ_Base::CARD hu[8] = {0};
    MJ_Base::CARD g[8] = {0};
    MJ_Base::CARD p[8] = {0};
    MJ_Base::CARD c[8] = {0};

    request.getHGPCList(hu, g, p, c);

    member[senderID]->setcChiList(c);
    member[senderID]->setcPengList(p);
    member[senderID]->setcGangList(g);
    member[senderID]->setcHuList(hu);
}

void MJ_LocalServer::resl_Hu(MJ_RequestData &req)
{
    int senderID = req.getSenderID();

    if(!f_HGPC_valid)
    {
        resp_unsuccessful(senderID, senderID);
        return;
    }

    if(this->mem_policy[senderID] & P_Hu)
    {
        this->stat = SVR_normal;
        if(this->tmOut->isActive())
            this->tmOut->stop();

        MJ_response resp;
        resp.setType(MJ_response::T_Ok);
        resp.setWho(senderID);
        resp.setSendTo(senderID);
        send(resp);

        resp.setType(MJ_response::T_Hu);
        resp.setCard(req.getCard());
        resp.setWho(senderID);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        send(resp);

        //���ֽ���
        resp.setType(MJ_response::T_GMOver);
        resp.setWho(senderID);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        send(resp);
    }
    else
    {
        resp_unsuccessful(senderID, senderID);
    }
}

void MJ_LocalServer::resl_Gang(MJ_RequestData &req)
{
     int senderID = req.getSenderID();

     if(!f_HGPC_valid)
     {
         resp_unsuccessful(senderID, senderID);
         return;
     }

     if(current_policy > P_Gang)
     {
         resp_unsuccessful(senderID, senderID);
         return ;
     }

     if(mem_policy[senderID] & P_Gang)
     {
         MJ_Base::CARD cd = req.getCard();
         current_policy = P_Gang;
         current_policy_ID = senderID;
         mem_policy[current_policy_ID] = P_None;//ȡ����״̬

         MJ_response resp;
         resp.setType(MJ_response::T_Ok);
         resp.setWho(senderID);
         resp.setSendTo(senderID);
         send(resp);

         resp.setType(MJ_response::T_Gang);
         resp.setCard(cd);
         resp.setSendTo(MJ_response::SDT_Broadcast);
         resp.setWho(senderID);
         send(resp);

         if(mem_policy[0] <= P_Gang && mem_policy[1] <= P_Gang &&
                 mem_policy[2] <= P_Gang && mem_policy[3] <= P_Gang)
         {
             this->stat = SVR_normal;
             if(this->tmOut->isActive())
                 this->tmOut->stop();

             // ����
             cur_id = current_policy_ID;
             this->faPai();

             resp.setType(MJ_response::T_ChuPai);
             resp.setSendTo(MJ_response::SDT_Broadcast);
             resp.setWho(senderID);

             send(resp);
         }
     }
     else
         resp_unsuccessful(senderID, senderID);

     return ;
}

void MJ_LocalServer::resl_Peng(MJ_RequestData &req)
{
    int senderID = req.getSenderID();

    if(!f_HGPC_valid)
    {
        resp_unsuccessful(senderID, senderID);
        return;
    }

    if(current_policy > P_Peng)
    {
        resp_unsuccessful(senderID, senderID);
        return ;
    }

    if(mem_policy[senderID] & P_Peng)
    {
        MJ_Base::CARD cd = req.getCard();
        current_policy = P_Peng;
        current_policy_ID = senderID;
        mem_policy[current_policy_ID] = P_None;//ȡ����״̬

        MJ_response resp;
        resp.setType(MJ_response::T_Ok);
        resp.setWho(senderID);
        resp.setSendTo(senderID);
        send(resp);

        resp.setType(MJ_response::T_Peng);
        resp.setCard(cd);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        resp.setWho(senderID);
        send(resp);

        if(mem_policy[0] <= P_Peng && mem_policy[1] <= P_Peng &&
                mem_policy[2] <= P_Peng && mem_policy[3] <= P_Peng)
        {
            this->stat = SVR_normal;
            if(this->tmOut->isActive())
                this->tmOut->stop();

            resp.setType(MJ_response::T_ChuPai);
            resp.setSendTo(MJ_response::SDT_Broadcast);
            resp.setWho(senderID);

            send(resp);
        }
    }
    else
    {
        resp_unsuccessful(senderID, senderID);
    }
    return;
}

void MJ_LocalServer::resl_Chi(MJ_RequestData &req)
{
    int senderID = req.getSenderID();

    if(!f_HGPC_valid)
    {
        resp_unsuccessful(senderID, senderID);
        return;
    }

    if(current_policy > P_Chi)
    {
        resp_unsuccessful(senderID, senderID);
        return;
    }

    // ֻ�ܳ��ϼҵ���
    if(current_policy == P_None && (cur_id+1)%4 == senderID)
    {
        req.getChi(this->chi);

        current_policy_ID = senderID;
        current_policy = P_Chi;
        mem_policy[current_policy_ID] = P_None;//ȡ����״̬

        //�㲥�����ҵĳԲ���
        MJ_response resp;
        resp.setType(MJ_response::T_Ok);
        resp.setWho(senderID);
        resp.setSendTo(senderID);
        send(resp);

        resp.setType(MJ_response::T_Chi);
        resp.setWho(senderID);
        resp.setChi(this->chi);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        send(resp);

        // ���û�бȳԸ������ȼ��Ĳ������ڣ������ҳ���
        if(mem_policy[0] <= P_Chi && mem_policy[1] <= P_Chi &&
                mem_policy[2] <= P_Chi && mem_policy[3] <= P_Chi)
        {
            this->stat = SVR_normal;
            if(this->tmOut->isActive())
                this->tmOut->stop();

            resp.setType(MJ_response::T_ChuPai);
            resp.setSendTo(MJ_response::SDT_Broadcast);
            resp.setWho(senderID);

            send(resp);
        }
    }
    else // ��Ӧ���ɹ�
    {
        resp_unsuccessful(senderID, senderID);
    }
    return ;
}

void MJ_LocalServer::resl_Cancel(MJ_RequestData &req)
{
    int senderID = req.getSenderID();

    this->mem_policy[senderID] = P_None;//ȡ���������
    MJ_response resp;

    // �������Ҳ���ȡ����֮ǰ�����ѡ��Ĳ����ǵ�ǰ���ֵʱ
    if(mem_policy[0] <= current_policy && mem_policy[1] <= current_policy &&
            mem_policy[2] <= current_policy && mem_policy[3] <= current_policy)
    {
        //  û�в�������ִ�������֧�Ļ������Է��Ƹ��¼���
        if(current_policy == P_None)
        {
            cur_id++;
            cur_id %= 4;
            this->faPai();
        }

        this->stat = SVR_normal;
        if(this->tmOut->isActive())
            this->tmOut->stop();

        if(current_policy == P_Gang)  // ���Ǹ��������
        {
            // ����
            cur_id = current_policy_ID;
            this->faPai();

            resp.setType(MJ_response::T_ChuPai);
            resp.setSendTo(MJ_response::SDT_Broadcast);
            resp.setWho(senderID);

            send(resp);
        }
        else
        {
            resp.setType(MJ_response::T_ChuPai);
            resp.setSendTo(MJ_response::SDT_Broadcast);
            resp.setWho(this->current_policy_ID);//

            send(resp);
        }
    }
    //  ��������ȴ�
}

void MJ_LocalServer::resl_AnGang(MJ_RequestData &req)
{
    int senderID = req.getSenderID();

    MJ_response resp;
    resp.setType(MJ_response::T_Ok);
    resp.setWho(senderID);
    resp.setSendTo(senderID);
    send(resp);

    resp.setType(MJ_response::T_Gang);
    resp.setSendTo(MJ_response::SDT_Broadcast);
    resp.setWho(senderID);
    send(resp);

    cur_id = senderID;
    this->faPai();

    resp.setType(MJ_response::T_ChuPai);
    resp.setSendTo(MJ_response::SDT_Broadcast);
    resp.setWho(senderID);

    send(resp);
}

void MJ_LocalServer::resl_BuGang(MJ_RequestData &req)
{
    int senderID = req.getSenderID();
    MJ_Base::CARD cd = req.getCard();

    MJ_response resp;
    resp.setType(MJ_response::T_Ok);
    resp.setWho(senderID);
    resp.setSendTo(senderID);
    send(resp);

    resp.setType(MJ_response::T_Gang);
    resp.setSendTo(MJ_response::SDT_Broadcast);
    resp.setWho(senderID);
    send(resp);

    cur_id = senderID;
    int j = senderID;
    this->mem_policy[j] = P_None;//
    j++;
    j%=4;
    for(auto i=0; i<3; i++)
    {
        if(this->member[j]->testHu(cd))
        {
            this->mem_policy[j] = P_Hu;
            this->stat = SVR_vote;
        }
        else {
            this->mem_policy[j] = P_None;//
        }
        j++;
        j%=4;//��λ���¸����
    }

    if(this->stat == SVR_vote)
    {
        current_policy = P_None;
        resp.setType(MJ_response::T_Wait);
        resp.setWho(senderID);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        send(resp);

        this->tmOut->start(10000);//10sѡ��ʱ��
        f_HGPC_valid = true;
        BuGang_falg = true;//�˱�־�����ж��Ƿ����ܺ�
    }
    else
    {
        cur_id = senderID;
        this->faPai();

        resp.setType(MJ_response::T_ChuPai);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        resp.setWho(senderID);

        send(resp);
    }
}

void MJ_LocalServer::RecvSlot(MJ_RequestData request)
{
    int type = request.getType();
//    int senderID = request.getSenderID();

    switch(type)
    {
    case MJ_RequestData::R_Init:
        this->start();
        break;

    case MJ_RequestData::R_HGPCList:
        this->resl_HGPCList(request);
        if((wj_ready & 0x0f) == 0x0f)
            this->faPai();
        break;

    case MJ_RequestData::R_ChuPai:
        this->resl_chuPai(request);
        break;

    case MJ_RequestData::R_Hu:
        this->resl_Hu(request);
        break;

    case MJ_RequestData::R_Gang:
        this->resl_Gang(request);
        break;

    case MJ_RequestData::R_Peng:
        this->resl_Peng(request);
        break;

    case MJ_RequestData::R_Chi:
        this->resl_Chi(request);
        break;

    case MJ_RequestData::R_CanCel:
        this->resl_Cancel(request);
        break;

    case MJ_RequestData::R_AnGang:
        this->resl_AnGang(request);
        break;

    case MJ_RequestData::R_BuGang:
        this->resl_BuGang(request);
        break;

    default: // �����ˣ�
        break;
    }
}

// �������� ѡ��ʱ
void MJ_LocalServer::tmOutSlot()
{
    f_HGPC_valid = false;

    // �����������ѡ���Ҫ�����
    if(this->current_policy != P_None)
    {
        this->current_policy = P_None;
        MJ_response resp;
        resp.setType(MJ_response::T_ChuPai);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        resp.setWho(this->current_policy_ID);
        send(resp);
    }
    else {  // ��û����ѡ��, ���Ƹ��¼�
        this->cur_id++;
        this->cur_id %= 4;
        this->faPai();
    }
}

void MJ_LocalServer::tmChuPaiSlot()
{
    this->member[cur_id]->DelCard(this->card);//  Ĭ�ϳ���

    MJ_RequestData req(cur_id);
    req.setType(MJ_RequestData::R_ChuPai);
    req.setSenderID(cur_id);
    req.setCard(this->card);

    this->resl_chuPai(req);
}






