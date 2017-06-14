#include "ThreadResponseKafka.h"
#include "AppManager.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>

#include <getopt.h>
#include <unistd.h>
#include "Log.h"

#include "StdPublic.h"
#include "PublicType.h"
#include "PacketBuffer.h"
#include "ConfigInfo.h"

#include <curl/curl.h>

#include "json/json.h"

#include "zmq.h"
#include "Function.h"


using namespace std;

static long curl_writer(void *data, int size, int nmemb, string *content)
{
    long sizes = size * nmemb;
    *content += string((char*)data, sizes);
    return sizes;
}


CThreadResponseKafka::CThreadResponseKafka()
{
    m_ctx = NULL;
    m_sc = NULL;
}

CThreadResponseKafka::~CThreadResponseKafka()
{
    int rc;
    if(m_ctx != NULL)
    {
        rc = zmq_ctx_term (m_ctx);
        assert (rc == 0);
    }

    if(m_sc != NULL)
    {
        rc = zmq_close (m_sc);
        assert (rc == 0);
    }
}

int CThreadResponseKafka::StartWork()
{
    m_ctx = zmq_ctx_new ();
    assert (m_ctx);

    m_sc = zmq_socket (m_ctx, ZMQ_PUB);
    assert (m_sc);
    char szBind[128] = {0};
    sprintf_s(szBind, "tcp://%s:5561", "127.0.0.1");
    CLog::Log("CThreadResponseKafka", CLog::TYPE_DEBUG, "BIND[%s]",szBind);
    int rc = zmq_bind (m_sc, szBind);
    assert (rc == 0);

    // 创建5个线程从管道中拉取数据
    //for(int32_t i = 0; i < 5; ++i)
    //{
        CreateThread(
            NULL,                       // default security attributes
            0,                          // use default stack size
            Proc,                       // thread function
            this,                       // argument to thread function
            0,                          // use default creation flags
            NULL);                      // returns the thread identifier
    //}

    return 0;
}

void* CThreadResponseKafka::Proc( LPVOID lpParam )
{
    CThreadResponseKafka* pThis = (CThreadResponseKafka*)lpParam;
    assert(pThis != NULL);

    //       char *pGbkReturn = "{\"code\":200,\"msg\":\"success\",\"data\":{\"orderDetail\":{\"ogNo\":\"201601CP05034909\",\"ogSeq\":\"201601CO05034909\",\"memGuid\":\"597AEEE5-69A1-9957-86AC-3FA9B4A81A20\",\"totalPay\":200.0,\"memberName\":\"测试\",\"memberCellphone\":\"15000301097\",\"insertDate\":\"2016-01-05 11:07:11\",\"payType\":\"支付宝\"},\"packageList\":[{\"packNo\":\"1\",\"packState\":1,\"packStateStr\":\"待付款\",\"type\":1,\"packQueryNo\":\"1\",\"freight\":0.0,\"itemList\":[{\"itno\":\"201505CG160000029\",\"sourceUrl\":\"/item/201505CM160000030\",\"picUrl\":\"/C/item/2015/0516/201505C160000016/201505C160000008_485901678.jpg\",\"productName\":\"android 规格品（有尺寸）\",\"qty\":1,\"isMall\":0,\"itSize\":\"2\",\"color\":\"蓝色\",\"price\":200.0,\"subTotalPrice\":200.0,\"coupons\":0.0,\"voucher\":0.0,\"shoppingCash\":0.0,\"shoppingCard\":0.0,\"pointsPrice\":0.0,\"promote\":0.0,\"freightNumber\":\"A0001\",\"kind\":\"1\",\"vvipPoints\":0.0,\"vvipDiscount\":0.0,\"useBalancePoints\":0.0,\"dividendDiscount\":0.0,\"scoreMallDiscount\":0.0}]}]}}";

    while(true)
    {
        char szmpInner[4096] = {0};
        int nLen;
        CAppManager::GetInstance().GetFifoKafkaMsg()->Pop((BYTE*)szmpInner, nLen);
        CLog::Log("CThreadResponseKafka", CLog::TYPE_DEBUG, "MsgBuffer %u",CAppManager::GetInstance().GetFifoKafkaMsg()->GetSize());

        // 获取到从kafka推送来的消息
        MsgPushInner mpInner;

        if(mpInner.ParseFromArray(szmpInner, nLen))
        {
            switch (mpInner.mptype())
            {
                case MP_NEWORDER:       // kafka新订单
                {
                    KfkNewOrder kfkneworder;
                    if(!kfkneworder.ParseFromArray(mpInner.mpdata().c_str(), mpInner.mpdata().length()))
                    {
                        CLog::Log("CThreadResponseKafka", CLog::TYPE_ERROR, "kafka new protobuf 反序列号失败");
                    }
                    else
                    {
                        CLog::Log("CThreadResponseKafka", CLog::TYPE_IMPORTANT, "kafka new protobuf 反序列号成功");
                        pThis->RecvKafkaNewOrder(kfkneworder);
                    }
                }
                break;
                case MP_PAYORDER:       // kafka已付款订单
                {
                    KfkPayOrder kfkpayorder;
                    if(!kfkpayorder.ParseFromArray(mpInner.mpdata().c_str(), mpInner.mpdata().length()))
                    {
                        CLog::Log("CThreadResponseKafka", CLog::TYPE_ERROR, "kafka pay protobuf 反序列号失败");
                    }
                    else
                    {
                        CLog::Log("CThreadResponseKafka", CLog::TYPE_IMPORTANT, "kafka pay protobuf 反序列号成功");
                        pThis->RecvKafkaPayOrder(kfkpayorder);
                    }
                }
                break;
                case MP_GOODSCOMMENT:       // kafka已评价商品
                {
                    KfkGoodsComment kfkgoodscomment;
                    if(!kfkgoodscomment.ParseFromArray(mpInner.mpdata().c_str(), mpInner.mpdata().length()))
                    {
                        CLog::Log("CThreadResponseKafka", CLog::TYPE_ERROR, "kafka goods comment protobuf 反序列号失败");
                    }
                    else
                    {
                        CLog::Log("CThreadResponseKafka", CLog::TYPE_IMPORTANT, "kafka goods comment protobuf 反序列号成功");
                        pThis->RecvKafkaGoodsComment(kfkgoodscomment);
                    }
                }
                break;
                case MP_RETURNORDER:       // kafka退货
                {
                    KfkReturnOrder kfkreturnorder;
                    if(!kfkreturnorder.ParseFromArray(mpInner.mpdata().c_str(), mpInner.mpdata().length()))
                    {
                        CLog::Log("CThreadResponseKafka", CLog::TYPE_ERROR, "kafka return order protobuf 反序列号失败");
                    }
                    else
                    {
                        CLog::Log("CThreadResponseKafka", CLog::TYPE_IMPORTANT, "kafka return order protobuf 反序列号成功");
                        pThis->RecvKafkaReturnOrder(kfkreturnorder);
                    }
                }
                break;
            }
        }
        else
        {
            CLog::Log("CThreadResponseKafka", CLog::TYPE_ERROR, "kafka protobuf 反序列号失败");
        }
    }

    return NULL;
}

// 收到kafka发来的一个新订单
void CThreadResponseKafka::RecvKafkaNewOrder(const KfkNewOrder &kfkneworder)
{
	// 获取买家信息
	char szMobileNo[64] = {0};
	char szEmail[128] = {0};
	char szNickName[128] = {0};
	char szHeadImgUrl[1024] = {0};
	if(!GetBuyerInfo(kfkneworder.memguid().c_str(), szMobileNo, szEmail, szNickName, szHeadImgUrl))
	{	// 获取买家信息失败
		CLog::Log("RecvKafkaNewOrder",CLog::TYPE_ERROR,"获取买家信息失败 guid:%s", kfkneworder.memguid().c_str());
		strcpy(szNickName, kfkneworder.memguid().c_str());
	}

    // 反序列号成功，调用获取订单详情的接口
    // 输入参数
    //data={"ogNo":"201510CP13100055","memGuid":"7CAD9F5F-5B03-E300-EC17-04DD297A3A54"}
    char szParam[128] = {0};
    sprintf_s(szParam, "data={\"ogNo\":\"%s\",\"memGuid\":\"%s\"}", kfkneworder.ogno().c_str(), kfkneworder.memguid().c_str());

    std::string strHttpReturn;

    //通过http接口获取买家信息
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig()->GetUrlOrderDetail());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szParam);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strHttpReturn);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);	
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    CURLcode res = curl_easy_perform(curl);

    //清理库接口
    curl_easy_cleanup(curl);

    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("RecvKafkaNewOrder",CLog::TYPE_ERROR,"获取订单详情失败。接口:%s 参数:%s",
                  CAppManager::GetInstance().GetConfig()->GetUrlOrderDetail(), szParam);
        return;
    }

    // utf8转gbk
    int nLength = strHttpReturn.length()*1.5;
    char *pGbkReturn = new char[nLength];
    memset(pGbkReturn, 0, nLength);
    utf2gbk((char*)strHttpReturn.c_str(),strHttpReturn.length(), pGbkReturn, nLength);

    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    if (reader.parse(pGbkReturn, recv))
    {
        CLog::Log("RecvKafkaNewOrder",CLog::TYPE_DEBUG,"%s", pGbkReturn);

        int ncode;
        if(recv.isMember("code"))   ncode = recv["code"].asInt();
        string strMsg;
        if(recv.isMember("msg"))    strMsg = recv["msg"].asString();
        if(ncode == 502 || ncode == 501 || ncode == 503)
        {
            CLog::Log("RecvKafkaNewOrder",CLog::TYPE_ERROR,"获取订单详情失败。code:%dmsg:%s",
                      ncode, strMsg.c_str());
        }
        else if(ncode == 200 && strMsg == "success" && recv.isMember("data") && !recv["data"].isNull())
        {
            Json::Value jvData = recv["data"];

            NewOrder neworder;
            neworder.set_buyername(szNickName);
            CLog::Log("RecvKafkaNewOrder",CLog::TYPE_IMPORTANT,"买家名字%s",szNickName);

            if(jvData.isMember("orderDetail") && !jvData["orderDetail"].isNull())
            {
                Json::Value jvDetail = jvData["orderDetail"];
                if(jvDetail.isMember("ogNo"))     neworder.set_ogno(jvDetail["ogNo"].asCString());
                if(jvDetail.isMember("ogSeq"))    neworder.set_ogseq(jvDetail["ogSeq"].asCString());
                if(jvDetail.isMember("memGuid"))  neworder.set_memguid(jvDetail["memGuid"].asCString());
                if(jvDetail.isMember("totalPay")) neworder.set_totalpay(jvDetail["totalPay"].asDouble());
                if(jvDetail.isMember("memberName"))   neworder.set_membername(jvDetail["memberName"].asCString());
                if(jvDetail.isMember("memberCellphone"))  neworder.set_membercellphone(jvDetail["memberCellphone"].asCString());
                if(jvDetail.isMember("insertDate"))   neworder.set_insertdate(jvDetail["insertDate"].asCString());
                if(jvDetail.isMember("payType"))  neworder.set_paytype(jvDetail["payType"].asCString());
            }
            if(jvData.isMember("packageList") && !jvData["packageList"].isNull())
            {
                Json::Value jvPL = jvData["packageList"];
                for(int i = 0; i < (int)jvPL.size(); ++i)
                {
                    double totalPay = 0;    // 包裹总金额

                    Json::Value jvP = jvPL[i];
                    if(jvP.isMember("packNo"))  neworder.set_packno(jvP["packNo"].asCString());
                    if(jvP.isMember("packState"))   neworder.set_packstate(jvP["packState"].asUInt());
                    if(jvP.isMember("packStateStr"))    neworder.set_packstatestr(jvP["packStateStr"].asCString());
                    if(jvP.isMember("type"))    neworder.set_type(jvP["type"].asUInt());
                    if(jvP.isMember("packQueryNo")) neworder.set_packqueryno(jvP["packQueryNo"].asCString());
                    if(jvP.isMember("freight")) neworder.set_freight(jvP["freight"].asDouble());

                    totalPay = neworder.freight();

                    if(neworder.type() != 2)
                    {
                        // 如果非商城订单，跳过循环
                        continue;
                    }

                    if(jvP.isMember("itemList") && !jvP["itemList"].isNull())
                    {
                        Json::Value jvGL = jvP["itemList"];
                        if(jvGL.size() == 0)
                        {
                            // 包裹列表中无商品
                            continue;
                        }
                        for(int j = 0; j < (int)jvGL.size(); ++j)
                        {
                            OrderGoodsInfo* goodinfo = neworder.add_goodslist();

                            Json::Value jvG = jvGL[j];
                            if(jvG.isMember("itno"))    goodinfo->set_itno(jvG["itno"].asCString());
                            if(jvG.isMember("sourcleUrl"))  goodinfo->set_sourceurl(jvG["sourcleUrl"].asCString());
                            if(jvG.isMember("picUrl"))  goodinfo->set_picurl(jvG["picUrl"].asCString());
                            if(jvG.isMember("productName")) goodinfo->set_productname(jvG["productName"].asCString());
                            if(jvG.isMember("qty")) goodinfo->set_qty(jvG["qty"].asUInt());
                            if(jvG.isMember("isMall"))  goodinfo->set_ismall(jvG["isMall"].asUInt());
                            if(jvG.isMember("price"))   goodinfo->set_price(jvG["price"].asDouble());
                            if(jvG.isMember("subTotalPrice"))   goodinfo->set_subtotalprice(jvG["subTotalPrice"].asDouble());
                            if(jvG.isMember("coupons")) goodinfo->set_coupons(jvG["coupons"].asDouble());
                            if(jvG.isMember("voucher")) goodinfo->set_voucher(jvG["voucher"].asDouble());
                            if(jvG.isMember("shoppingCash"))    goodinfo->set_shoppingcash(jvG["shoppingCash"].asDouble());
                            if(jvG.isMember("shoppingCard"))    goodinfo->set_shoppingcard(jvG["shoppingCard"].asDouble());
                            if(jvG.isMember("pointsPrice")) goodinfo->set_pointsprice(jvG["pointsPrice"].asDouble());
                            if(jvG.isMember("promote")) goodinfo->set_promote(jvG["promote"].asDouble());
                            if(jvG.isMember("freightNumber"))   goodinfo->set_freightnumber(jvG["freightNumber"].asCString());
                            if(jvG.isMember("kind"))    goodinfo->set_kind(jvG["kind"].asCString());
                            if(jvG.isMember("shippingTax")) goodinfo->set_shippingtax(jvG["shippingTax"].asDouble());
                            if(jvG.isMember("merchantId"))  neworder.set_merchantid(jvG["merchantId"].asCString());
                            if(jvG.isMember("skuId"))   goodinfo->set_skuid(jvG["skuId"].asCString());
                            if(jvG.isMember("color"))  goodinfo->set_color(jvG["color"].asCString());
                            if(jvG.isMember("salerprop"))   goodinfo->set_size(jvG["salerprop"].asCString());

                            totalPay += goodinfo->subtotalprice();
                        }

                        // 发送给ImServer
                        neworder.set_totalpay(totalPay);
                        SendNewOrderToImServer(neworder);

                        // 删除商品列表
                        neworder.clear_goodslist();
                    }
                }
            }
        }
    }

    delete[] pGbkReturn;
    pGbkReturn = NULL;
}

// 将新订单传给imserver
void CThreadResponseKafka::SendNewOrderToImServer(const NewOrder &neworder)
{
    // 转换为二进制流
    uint32_t msg_size = neworder.ByteSize();
    char* szData = new char[msg_size];
    neworder.SerializeToArray(szData, msg_size);

    MsgPushInd mpInd;
    mpInd.set_mptype(MP_NEWORDER);
    mpInd.set_mpdata(szData, msg_size);

    delete szData;
    szData = NULL;

    msg_size = mpInd.ByteSize();
    szData = new char[msg_size];
    mpInd.SerializeToArray(szData, msg_size);

    // 组包
    DataPacket dp;
    XT_HEAD_PB *pHead = dp.mutable_head();
    pHead->set_cmd(SS_CID_MSG_PUSH);
    dp.set_data(szData, msg_size);

    delete szData;
    szData = NULL;

    msg_size = dp.ByteSize();
    szData = new char[msg_size];
    if (!dp.SerializeToArray(szData, msg_size))
    {
        CLog::Log("SendNewOrderToImServer", CLog::TYPE_ERROR, "新订单 protobuf 序列号失败");
    }
    else
    {
        CLog::Log("SendNewOrderToImServer", CLog::TYPE_DEBUG, "新订单 protobuf 序列号成功。二进制长度:%d", msg_size);
    }

    int rc = zmq_send(m_sc, (void*)szData, msg_size, 0);
    if (rc < 0)
    {
        CLog::Log("SendNewOrderToImServer", CLog::TYPE_ERROR, "zmq 发送消息失败");
    }
    else
    {
        CLog::Log("SendNewOrderToImServer", CLog::TYPE_DEBUG, "zmq 发送消息成功");
    }

    delete szData;
    szData = NULL;
}

// 收到kafka发来的一个已付款订单
void CThreadResponseKafka::RecvKafkaPayOrder(const KfkPayOrder &kfkpayorder)
{
	// 获取买家信息
	char szMobileNo[64] = {0};
	char szEmail[128] = {0};
	char szNickName[128] = {0};
	char szHeadImgUrl[1024] = {0};
	if(!GetBuyerInfo(kfkpayorder.memguid().c_str(), szMobileNo, szEmail, szNickName, szHeadImgUrl))
	{	// 获取买家信息失败
		CLog::Log("RecvKafkaPayOrder",CLog::TYPE_ERROR,"获取买家信息失败 guid:%s", kfkpayorder.memguid().c_str());
		strcpy(szNickName, kfkpayorder.memguid().c_str());
	}

	// 获取当前时间作为下单时间
	time_t t = time(0); 
    char szTime[64] = {0};
    strftime( szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S",localtime(&t) ); 
    
	/*time_t now;
	time(&now);
    struct tm *tm_now;
    tm_now = localtime(&now);
    char szTime[64] = {0};
    sprintf_s(szTime, "%d-%d-%d %d:%d:%d", tm_now->tm_year, tm_now->tm_mon, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
    */CLog::Log("RecvKafkaPayOrder",CLog::TYPE_IMPORTANT, "下单时间:%s", szTime);

    // 反序列号成功，调用获取订单详情的接口
    // 输入参数
    //data={"ogNo":"201510CP13100055","memGuid":"7CAD9F5F-5B03-E300-EC17-04DD297A3A54"}
    char szParam[128] = {0};
    sprintf_s(szParam, "data={\"ogNo\":\"%s\",\"memGuid\":\"%s\"}", kfkpayorder.ogno().c_str(), kfkpayorder.memguid().c_str());

    std::string strHttpReturn;

    //通过http接口获取买家信息
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig()->GetUrlOrderDetail());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szParam);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strHttpReturn);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);	
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    CURLcode res = curl_easy_perform(curl);

    //清理库接口
    curl_easy_cleanup(curl);

    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("RecvKafkaPayOrder",CLog::TYPE_ERROR,"获取订单详情失败。接口:%s 参数:%s",
                  CAppManager::GetInstance().GetConfig()->GetUrlOrderDetail(), szParam);
        return;
    }

    // utf8转gbk
    int nLength = strHttpReturn.length()*1.5;
    char *pGbkReturn = new char[nLength];
    memset(pGbkReturn, 0, nLength);
    utf2gbk((char*)strHttpReturn.c_str(),strHttpReturn.length(), pGbkReturn, nLength);

    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    if (reader.parse(pGbkReturn, recv))
    {
        CLog::Log("RecvKafkaPayOrder",CLog::TYPE_DEBUG,"%s", pGbkReturn);

        int ncode;
        if(recv.isMember("code"))   ncode = recv["code"].asInt();
        string strMsg;
        if(recv.isMember("msg"))    strMsg = recv["msg"].asString();
        if(ncode == 502 || ncode == 501 || ncode == 503)
        {
            CLog::Log("RecvKafkaPayOrder",CLog::TYPE_ERROR,"获取订单详情失败。code:%dmsg:%s",
                      ncode, strMsg.c_str());
        }
        else if(ncode == 200 && strMsg == "success" && recv.isMember("data") && !recv["data"].isNull())
        {
            Json::Value jvData = recv["data"];

            PayOrder payorder;
            payorder.set_buyername(szNickName);
            payorder.set_paydate(szTime);
            CLog::Log("RecvKafkaPayOrder",CLog::TYPE_IMPORTANT,"买家名字%s",szNickName);

            if(jvData.isMember("orderDetail") && !jvData["orderDetail"].isNull())
            {
                Json::Value jvDetail = jvData["orderDetail"];
                if(jvDetail.isMember("ogNo"))     payorder.set_ogno(jvDetail["ogNo"].asCString());
                if(jvDetail.isMember("ogSeq"))    payorder.set_ogseq(jvDetail["ogSeq"].asCString());
                if(jvDetail.isMember("memGuid"))  payorder.set_memguid(jvDetail["memGuid"].asCString());
                if(jvDetail.isMember("totalPay")) payorder.set_totalpay(jvDetail["totalPay"].asDouble());
                if(jvDetail.isMember("memberName"))   payorder.set_membername(jvDetail["memberName"].asCString());
                if(jvDetail.isMember("memberCellphone"))  payorder.set_membercellphone(jvDetail["memberCellphone"].asCString());
                if(jvDetail.isMember("insertDate"))   payorder.set_insertdate(jvDetail["insertDate"].asCString());
                if(jvDetail.isMember("payType"))  payorder.set_paytype(jvDetail["payType"].asCString());
            }
            if(jvData.isMember("packageList") && !jvData["packageList"].isNull())
            {
                Json::Value jvPL = jvData["packageList"];
                for(int i = 0; i < (int)jvPL.size(); ++i)
                {
                    double totalPay = 0;    // 包裹总金额

                    Json::Value jvP = jvPL[i];
                    if(jvP.isMember("packNo"))  payorder.set_packno(jvP["packNo"].asCString());
                    if(jvP.isMember("packState"))   payorder.set_packstate(jvP["packState"].asUInt());
                    if(jvP.isMember("packStateStr"))    payorder.set_packstatestr(jvP["packStateStr"].asCString());
                    if(jvP.isMember("type"))    payorder.set_type(jvP["type"].asUInt());
                    if(jvP.isMember("packQueryNo")) payorder.set_packqueryno(jvP["packQueryNo"].asCString());
                    if(jvP.isMember("freight")) payorder.set_freight(jvP["freight"].asDouble());

                    totalPay = payorder.freight();

                    if(payorder.type() != 2)
                    {
                        // 如果非商城订单，跳过循环
                        continue;
                    }

                    if(jvP.isMember("itemList") && !jvP["itemList"].isNull())
                    {
                        Json::Value jvGL = jvP["itemList"];
                        if(jvGL.size() == 0)
                        {
                            // 包裹列表中无商品
                            continue;
                        }
                        for(int j = 0; j < (int)jvGL.size(); ++j)
                        {
                            OrderGoodsInfo* goodinfo = payorder.add_goodslist();

                            Json::Value jvG = jvGL[j];
                            if(jvG.isMember("itno"))    goodinfo->set_itno(jvG["itno"].asCString());
                            if(jvG.isMember("sourcleUrl"))  goodinfo->set_sourceurl(jvG["sourcleUrl"].asCString());
                            if(jvG.isMember("picUrl"))  goodinfo->set_picurl(jvG["picUrl"].asCString());
                            if(jvG.isMember("productName")) goodinfo->set_productname(jvG["productName"].asCString());
                            if(jvG.isMember("qty")) goodinfo->set_qty(jvG["qty"].asUInt());
                            if(jvG.isMember("isMall"))  goodinfo->set_ismall(jvG["isMall"].asUInt());
                            if(jvG.isMember("price"))   goodinfo->set_price(jvG["price"].asDouble());
                            if(jvG.isMember("subTotalPrice"))   goodinfo->set_subtotalprice(jvG["subTotalPrice"].asDouble());
                            if(jvG.isMember("coupons")) goodinfo->set_coupons(jvG["coupons"].asDouble());
                            if(jvG.isMember("voucher")) goodinfo->set_voucher(jvG["voucher"].asDouble());
                            if(jvG.isMember("shoppingCash"))    goodinfo->set_shoppingcash(jvG["shoppingCash"].asDouble());
                            if(jvG.isMember("shoppingCard"))    goodinfo->set_shoppingcard(jvG["shoppingCard"].asDouble());
                            if(jvG.isMember("pointsPrice")) goodinfo->set_pointsprice(jvG["pointsPrice"].asDouble());
                            if(jvG.isMember("promote")) goodinfo->set_promote(jvG["promote"].asDouble());
                            if(jvG.isMember("freightNumber"))   goodinfo->set_freightnumber(jvG["freightNumber"].asCString());
                            if(jvG.isMember("kind"))    goodinfo->set_kind(jvG["kind"].asCString());
                            if(jvG.isMember("shippingTax")) goodinfo->set_shippingtax(jvG["shippingTax"].asDouble());
                            if(jvG.isMember("merchantId"))  payorder.set_merchantid(jvG["merchantId"].asCString());
                            if(jvG.isMember("skuId"))   goodinfo->set_skuid(jvG["skuId"].asCString());
                            if(jvG.isMember("color"))  goodinfo->set_color(jvG["color"].asCString());
                            if(jvG.isMember("salerprop"))   goodinfo->set_size(jvG["salerprop"].asCString());

                            totalPay += goodinfo->subtotalprice();
                        }

                        // 发送给ImServer
                        payorder.set_totalpay(totalPay);
                        SendPayOrderToImServer(payorder);

                        // 删除商品列表
                        payorder.clear_goodslist();
                    }
                }
            }
        }
    }

    delete[] pGbkReturn;
    pGbkReturn = NULL;
}

// 将已付款订单传给imserver
void CThreadResponseKafka::SendPayOrderToImServer(const PayOrder &payorder)
{
    // 转换为二进制流
    uint32_t msg_size = payorder.ByteSize();
    char* szData = new char[msg_size];
    payorder.SerializeToArray(szData, msg_size);

    MsgPushInd mpInd;
    mpInd.set_mptype(MP_PAYORDER);
    mpInd.set_mpdata(szData, msg_size);

    delete szData;
    szData = NULL;

    msg_size = mpInd.ByteSize();
    szData = new char[msg_size];
    mpInd.SerializeToArray(szData, msg_size);

    // 组包
    DataPacket dp;
    XT_HEAD_PB *pHead = dp.mutable_head();
    pHead->set_cmd(SS_CID_MSG_PUSH);
    dp.set_data(szData, msg_size);

    delete szData;
    szData = NULL;

    msg_size = dp.ByteSize();
    szData = new char[msg_size];
    if (!dp.SerializeToArray(szData, msg_size))
    {
        CLog::Log("SendPayOrderToImServer", CLog::TYPE_ERROR, "已付款订单 protobuf 序列号失败");
    }
    else
    {
        CLog::Log("SendPayOrderToImServer", CLog::TYPE_DEBUG, "已付款订单 protobuf 序列号成功。二进制长度:%d", msg_size);
    }

    int rc = zmq_send(m_sc, (void*)szData, msg_size, 0);
    if (rc < 0)
    {
        CLog::Log("SendPayOrderToImServer", CLog::TYPE_ERROR, "zmq 发送消息失败");
    }
    else
    {
        CLog::Log("SendPayOrderToImServer", CLog::TYPE_DEBUG, "zmq 发送消息成功");
    }

    delete szData;
    szData = NULL;
}

// 收到kafka发来的一个已评论商品
void CThreadResponseKafka::RecvKafkaGoodsComment(const KfkGoodsComment &kfkgoodscomment)
{
	CLog::Log("RecvKafkaGoodsComment",CLog::TYPE_DEBUG,"kfk 商品ID%s 评价等级%f 评价内容%s",
                  kfkgoodscomment.skuid().c_str(),
                  kfkgoodscomment.evaluatelevel(),
                  kfkgoodscomment.evaluatecontext().c_str());

    // 获取当前时间作为下单时间
	time_t t = time(0); 
    char szTime[64] = {0};
    strftime( szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S",localtime(&t) );               

    // 反序列号成功，调用获取订单详情的接口
    // 输入参数
    //data={"ogNo":"201510CP13100055","memGuid":"7CAD9F5F-5B03-E300-EC17-04DD297A3A54"}
    char szParam[128] = {0};
    sprintf_s(szParam, "data={\"ogNo\":\"%s\",\"memGuid\":\"%s\"}", kfkgoodscomment.ogno().c_str(), kfkgoodscomment.memguid().c_str());

    std::string strHttpReturn;

    //通过http接口获取买家信息
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig()->GetUrlOrderDetail());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szParam);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strHttpReturn);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);	
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    CURLcode res = curl_easy_perform(curl);

    //清理库接口
    curl_easy_cleanup(curl);

    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("RecvKafkaGoodsComment",CLog::TYPE_ERROR,"获取订单详情失败。接口:%s 参数:%s",
                  CAppManager::GetInstance().GetConfig()->GetUrlOrderDetail(), szParam);
        return;
    }

    // utf8转gbk
    int nLength = strHttpReturn.length()*1.5;
    char *pGbkReturn = new char[nLength];
    memset(pGbkReturn, 0, nLength);
    utf2gbk((char*)strHttpReturn.c_str(),strHttpReturn.length(), pGbkReturn, nLength);

    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    if (reader.parse(pGbkReturn, recv))
    {
        CLog::Log("RecvKafkaGoodsComment",CLog::TYPE_DEBUG,"%s", pGbkReturn);

        int ncode;
        if(recv.isMember("code"))   ncode = recv["code"].asInt();
        string strMsg;
        if(recv.isMember("msg"))    strMsg = recv["msg"].asString();
        if(ncode == 502 || ncode == 501 || ncode == 503)
        {
            CLog::Log("RecvKafkaGoodsComment",CLog::TYPE_ERROR,"获取订单详情失败。code:%dmsg:%s",
                      ncode, strMsg.c_str());
        }
        else if(ncode == 200 && strMsg == "success" && recv.isMember("data") && !recv["data"].isNull())
        {
            Json::Value jvData = recv["data"];

            GoodsComment goodscomment;
            goodscomment.set_bugername(kfkgoodscomment.bugername().c_str());
            goodscomment.set_evaluatelevel(kfkgoodscomment.evaluatelevel());
            goodscomment.set_evaluatecontext(kfkgoodscomment.evaluatecontext().c_str());
            goodscomment.set_commentdate(szTime);

            if(jvData.isMember("orderDetail") && !jvData["orderDetail"].isNull())
            {
                Json::Value jvDetail = jvData["orderDetail"];
                if(jvDetail.isMember("ogNo"))     goodscomment.set_ogno(jvDetail["ogNo"].asCString());
                if(jvDetail.isMember("ogSeq"))    goodscomment.set_ogseq(jvDetail["ogSeq"].asCString());
                if(jvDetail.isMember("memGuid"))  goodscomment.set_memguid(jvDetail["memGuid"].asCString());
                if(jvDetail.isMember("memberName"))   goodscomment.set_membername(jvDetail["memberName"].asCString());
                if(jvDetail.isMember("memberCellphone"))  goodscomment.set_membercellphone(jvDetail["memberCellphone"].asCString());
                if(jvDetail.isMember("insertDate"))   goodscomment.set_insertdate(jvDetail["insertDate"].asCString());
                if(jvDetail.isMember("payType"))  goodscomment.set_paytype(jvDetail["payType"].asCString());
            }
            if(jvData.isMember("packageList") && !jvData["packageList"].isNull())
            {
                Json::Value jvPL = jvData["packageList"];
                for(int i = 0; i < (int)jvPL.size(); ++i)
                {
                    Json::Value jvP = jvPL[i];

                    if(jvP["type"].asUInt() != 2)
                    {
                        // 如果非商城订单，跳过循环
                        continue;
                    }

                    if(jvP.isMember("itemList") && !jvP["itemList"].isNull())
                    {
                        Json::Value jvGL = jvP["itemList"];
                        if(jvGL.size() == 0)
                        {
                            // 包裹列表中无商品
                            continue;
                        }
                        for(int j = 0; j < (int)jvGL.size(); ++j)
                        {
                            Json::Value jvG = jvGL[j];
                            if(jvG["skuId"].asString() == kfkgoodscomment.skuid())
                            {
								OrderGoodsInfo* goodinfo = goodscomment.mutable_goods();
                            
                                if(jvG.isMember("itno"))    goodinfo->set_itno(jvG["itno"].asCString());
                                if(jvG.isMember("sourcleUrl"))  goodinfo->set_sourceurl(jvG["sourcleUrl"].asCString());
                                if(jvG.isMember("picUrl"))  goodinfo->set_picurl(jvG["picUrl"].asCString());
                                if(jvG.isMember("productName")) goodinfo->set_productname(jvG["productName"].asCString());
                                if(jvG.isMember("qty")) goodinfo->set_qty(jvG["qty"].asUInt());
                                if(jvG.isMember("isMall"))  goodinfo->set_ismall(jvG["isMall"].asUInt());
                                if(jvG.isMember("price"))   goodinfo->set_price(jvG["price"].asDouble());
                                if(jvG.isMember("subTotalPrice"))   goodinfo->set_subtotalprice(jvG["subTotalPrice"].asDouble());
                                if(jvG.isMember("coupons")) goodinfo->set_coupons(jvG["coupons"].asDouble());
                                if(jvG.isMember("voucher")) goodinfo->set_voucher(jvG["voucher"].asDouble());
                                if(jvG.isMember("shoppingCash"))    goodinfo->set_shoppingcash(jvG["shoppingCash"].asDouble());
                                if(jvG.isMember("shoppingCard"))    goodinfo->set_shoppingcard(jvG["shoppingCard"].asDouble());
                                if(jvG.isMember("pointsPrice")) goodinfo->set_pointsprice(jvG["pointsPrice"].asDouble());
                                if(jvG.isMember("promote")) goodinfo->set_promote(jvG["promote"].asDouble());
                                if(jvG.isMember("freightNumber"))   goodinfo->set_freightnumber(jvG["freightNumber"].asCString());
                                if(jvG.isMember("kind"))    goodinfo->set_kind(jvG["kind"].asCString());
                                if(jvG.isMember("shippingTax")) goodinfo->set_shippingtax(jvG["shippingTax"].asDouble());
                                if(jvG.isMember("merchantId"))  goodscomment.set_merchantid(jvG["merchantId"].asCString());
                                if(jvG.isMember("skuId"))   goodinfo->set_skuid(jvG["skuId"].asCString());
                                if(jvG.isMember("color"))  goodinfo->set_color(jvG["color"].asCString());
                                if(jvG.isMember("salerprop"))   goodinfo->set_size(jvG["salerprop"].asCString());

                                // 发送给ImServer
                        		SendGoodsCommentToImServer(goodscomment);

                        		delete[] pGbkReturn;
    							pGbkReturn = NULL;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    delete[] pGbkReturn;
    pGbkReturn = NULL;
}

// 将已评价商品传给imserver
void CThreadResponseKafka::SendGoodsCommentToImServer(const GoodsComment &goodscomment)
{
    // 转换为二进制流
    uint32_t msg_size = goodscomment.ByteSize();
    char* szData = new char[msg_size];
    goodscomment.SerializeToArray(szData, msg_size);

    MsgPushInd mpInd;
    mpInd.set_mptype(MP_GOODSCOMMENT);
    mpInd.set_mpdata(szData, msg_size);

    delete szData;
    szData = NULL;

    msg_size = mpInd.ByteSize();
    szData = new char[msg_size];
    mpInd.SerializeToArray(szData, msg_size);

    // 组包
    DataPacket dp;
    XT_HEAD_PB *pHead = dp.mutable_head();
    pHead->set_cmd(SS_CID_MSG_PUSH);
    dp.set_data(szData, msg_size);

    delete szData;
    szData = NULL;

    msg_size = dp.ByteSize();
    szData = new char[msg_size];
    if (!dp.SerializeToArray(szData, msg_size))
    {
        CLog::Log("SendGoodsCommentToImServer", CLog::TYPE_ERROR, "已评价商品 protobuf 序列号失败");
    }
    else
    {
        CLog::Log("SendGoodsCommentToImServer", CLog::TYPE_DEBUG, "已评价商品 protobuf 序列号成功。二进制长度:%d", msg_size);
    }

    int rc = zmq_send(m_sc, (void*)szData, msg_size, 0);
    if (rc < 0)
    {
        CLog::Log("SendGoodsCommentToImServer", CLog::TYPE_ERROR, "zmq 发送消息失败");
    }
    else
    {
        CLog::Log("SendGoodsCommentToImServer", CLog::TYPE_DEBUG, "zmq 发送消息成功");
    }

    delete szData;
    szData = NULL;
}

// 获取买家信息
bool CThreadResponseKafka::GetBuyerInfo(const char* guid, char* szMobileNo, char* szEmail, char* szNickName,char* szHeadImgUrl)
{
    if(strcmp(guid, "") == 0)
    {
        CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"guid为空");
        return false;
    }

    std::string sPostParam = "type=5";

    //加入用户名
    sPostParam += "&name=";
    sPostParam += guid;
    sPostParam += "&returnType=1";
    std::string data;

    //通过http接口获取买家信息
    CURL *curl = curl_easy_init();
    string strHttp = CAppManager::GetInstance().GetConfig()->GetBuyerInfoURL();
    CLog::Log("GetBuyerInfo",CLog::TYPE_DEBUG,"http:%s, param:%s", strHttp.c_str(), sPostParam.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, strHttp.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sPostParam.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);   
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    CURLcode res = curl_easy_perform(curl);

    //清理库接口
    curl_easy_cleanup(curl);

    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"guid:%s curlFaild", guid);
        return false;
    }

	
	// utf8转gbk
	int nLength = 4096;;
	char pGbkReturn[4096] = {0};
	utf2gbk((char*)data.c_str(),data.length(), pGbkReturn, nLength);

    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("GetBuyerInfo",CLog::TYPE_DEBUG,"JSON:%s",data.c_str());
    if (reader.parse(pGbkReturn, recv))
    {
        if(!recv.isMember("msg"))
        {
            return false;
        }
        string strMsg = recv["msg"].asString();
        if(strMsg != "success")
        {
            //用户不存在
            CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"guid:%s, notExist", guid);
            return false;
        }
        else
        {
            string strMobileNo= recv["data"]["MEM_CELL_PHONE"].asString();
            string strEmail = recv["data"]["MEM_EMAIL"].asString();
            string strNickName =recv["data"]["NICKNAME"].asString();
            string strHeadImgFile =recv["data"]["HEAD_PORTRAIT"].asString();
            string strUserName =recv["data"]["MEM_USERNAME"].asString();
            if(strNickName == "")
            {
            	strNickName = strUserName;
            }

            strncpy(szMobileNo, strMobileNo.c_str(), MAX_PHONE_LEN);
            strncpy(szEmail, strEmail.c_str(), MAX_EMAIL_LEN);
            strncpy(szNickName, strNickName.c_str(), strNickName.length());

            if(strHeadImgFile.length()!=0)
            {
                char szHeadImgFile[MAX_HEADURL_LEN-30]="";
                char preHeadImgURL[MAX_HEADURL_LEN-70] = {0};
                strcpy(preHeadImgURL,CAppManager::GetInstance().GetConfig()->GetHeadImgURL());

                strncpy(szHeadImgFile,strHeadImgFile.c_str(),MAX_HEADURL_LEN-30 );
                CLog::Log("GetBuyerInfo",CLog::TYPE_DEBUG,"preHeadImgURL:%s",preHeadImgURL);

                sprintf(szHeadImgUrl,"%s%s",preHeadImgURL,szHeadImgFile);
            }
            else
            {
                strncpy(szHeadImgUrl,strHeadImgFile.c_str(),MAX_HEADURL_LEN );
            }
            CLog::Log("GetBuyerInfo",CLog::TYPE_DEBUG,"guid:%s, mobileNo:%s, email:%s, nickName:%s, szHeadImgFile:%s",
                      guid, szMobileNo, szEmail, szNickName, szHeadImgUrl);
            //找到了子串
            return true;
        }
    }
    else
    {
        //json串解析失败
        CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "guid:%s, data:%s ,jsonPraseFail", guid, data.c_str());
        return false;
    }
}

// 收到kafka发来的一个退换货商品
void CThreadResponseKafka::RecvKafkaReturnOrder(const KfkReturnOrder &kfkreturnorder)
{
	CLog::Log("RecvKafkaReturnOrder",CLog::TYPE_DEBUG,"kfk 退货响应");

	// 获取买家信息
	char szMobileNo[64] = {0};
	char szEmail[128] = {0};
	char szNickName[128] = {0};
	char szHeadImgUrl[1024] = {0};
	if(!GetBuyerInfo(kfkreturnorder.memguid().c_str(), szMobileNo, szEmail, szNickName, szHeadImgUrl))
	{	// 获取买家信息失败
		CLog::Log("RecvKafkaReturnOrder",CLog::TYPE_ERROR,"获取买家信息失败 guid:%s", kfkreturnorder.memguid().c_str());
		strcpy(szNickName, kfkreturnorder.memguid().c_str());
	}

    // 反序列号成功，调用获取订单详情的接口
    // 输入参数
    //data={"ogSeq":"201601CP15037023","fdlSeq":"","memGuid":"F1962196-3DB1-4449-8897-9A716E30A193","isMall":"2"}
    char szParam[128] = {0};
    sprintf_s(szParam, "data={\"ogSeq\":\"%s\",\"fdlSeq\":\"\",\"memGuid\":\"%s\",\"isMall\":\"2\"}", kfkreturnorder.ogseq().c_str(), kfkreturnorder.memguid().c_str());

    std::string strHttpReturn;

    //通过http接口获取买家信息
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig()->GetReturnInfoUrl());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szParam);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strHttpReturn);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);	
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    CURLcode res = curl_easy_perform(curl);

    //清理库接口
    curl_easy_cleanup(curl);

    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("RecvKafkaReturnOrder",CLog::TYPE_ERROR,"获取退货详情失败。接口:%s 参数:%s",
                  CAppManager::GetInstance().GetConfig()->GetReturnInfoUrl(), szParam);
        return;
    }

    // utf8转gbk
    int nLength = strHttpReturn.length()*1.5;
    char *pGbkReturn = new char[nLength];
    memset(pGbkReturn, 0, nLength);
    utf2gbk((char*)strHttpReturn.c_str(),strHttpReturn.length(), pGbkReturn, nLength);

    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    if (reader.parse(pGbkReturn, recv))
    {
        CLog::Log("RecvKafkaReturnOrder",CLog::TYPE_DEBUG,"%s", pGbkReturn);

        int ncode = 0;
        if(recv.isMember("code"))   ncode = recv["code"].asInt();
        if(ncode != 200 || ncode == 501 || ncode == 503)
        {
            CLog::Log("RecvKafkaReturnOrder",CLog::TYPE_ERROR,"获取订单详情失败。code:%d", ncode);
            delete[] pGbkReturn;
    		pGbkReturn = NULL;
    		return;
        }

        if(!recv.isMember("data") || recv["data"].isNull())
        {
        	delete[] pGbkReturn;
    		pGbkReturn = NULL;
    		return;
        }
        
        Json::Value jvData = recv["data"];
        if(!jvData.isMember("returnInfo") || jvData["returnInfo"].isNull())
        {
        	delete[] pGbkReturn;
    		pGbkReturn = NULL;
    		return;
        }
        Json::Value jvReturnInfoArr =  jvData["returnInfo"];
        
		for(int i = 0; i < (int)jvReturnInfoArr.size(); ++i)
		{
			ReturnOrder returninfo;
			returninfo.set_memguid(kfkreturnorder.memguid().c_str());
			returninfo.set_buyername(szNickName);
			Json::Value jvReturnInfo = jvReturnInfoArr[i];
			if(jvReturnInfo.isMember("rsSeq"))     returninfo.set_rsseq(jvReturnInfo["rsSeq"].asCString());
			if(returninfo.rsseq() != kfkreturnorder.rsseq())
			{
				continue;
			}
			if(jvReturnInfo.isMember("ogNo"))     returninfo.set_ogno(jvReturnInfo["ogNo"].asCString());
			if(jvReturnInfo.isMember("ogsSeq"))     returninfo.set_ogsseq(jvReturnInfo["ogsSeq"].asCString());
			if(jvReturnInfo.isMember("merchantId"))     returninfo.set_merchantid(jvReturnInfo["merchantId"].asCString());
			if(jvReturnInfo.isMember("applyDt")) 
			{
				uint32 applydate = (uint32)(jvReturnInfo["applyDt"].asDouble()/1000);
				time_t t = applydate; 
    			char szTime[64] = {0};
    			strftime( szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S",localtime(&t) );   
				CLog::Log("RecvKafkaReturnOrder",CLog::TYPE_IMPORTANT," 退货时间:%u %s", applydate, szTime);
				returninfo.set_applydate(szTime);    			
			}
			if(jvReturnInfo.isMember("applyPrice"))     returninfo.set_applyprice(jvReturnInfo["applyPrice"].asDouble());
			
			if(!jvReturnInfo.isMember("productInfos") || jvReturnInfo["productInfos"].isNull())
			{	
				continue;
			}
			Json::Value jvGoodsArr =  jvReturnInfo["productInfos"];
			if((int)jvGoodsArr.size() == 0)
			{
				continue;
			}
			for(int j = 0; j < (int)jvGoodsArr.size(); ++j)
			{
				ReturnGoodsInfo* goodinfo = returninfo.add_goodslist();
				Json::Value jvGoods = jvGoodsArr[j];

				if(jvGoods.isMember("skuId") && !jvGoods["skuId"].isNull())     goodinfo->set_skuid(jvGoods["skuId"].asCString());
				if(jvGoods.isMember("productPrice") && !jvGoods["productPrice"].isNull())     goodinfo->set_productprice(jvGoods["productPrice"].asDouble());
				if(jvGoods.isMember("productQty") && !jvGoods["productQty"].isNull())     goodinfo->set_productqty(jvGoods["productQty"].asInt());
				if(jvGoods.isMember("returnQty") && !jvGoods["returnQty"].isNull())     goodinfo->set_returnqty(jvGoods["returnQty"].asInt());
				if(jvGoods.isMember("productMainUrl") && !jvGoods["productMainUrl"].isNull())     goodinfo->set_productmainurl(jvGoods["productMainUrl"].asCString());
				if(jvGoods.isMember("sourceUrl") && !jvGoods["sourceUrl"].isNull())     goodinfo->set_sourceurl(jvGoods["sourceUrl"].asCString());
				if(jvGoods.isMember("color") && !jvGoods["color"].isNull())     goodinfo->set_color(jvGoods["color"].asCString());
				if(jvGoods.isMember("salerprop") && !jvGoods["salerprop"].isNull())     goodinfo->set_size(jvGoods["salerprop"].asCString());
				if(jvGoods.isMember("reason") && !jvGoods["reason"].isNull())     goodinfo->set_reason(jvGoods["reason"].asCString());
				if(jvGoods.isMember("aprnPromote") && !jvGoods["aprnPromote"].isNull())     goodinfo->set_aprnpromote(jvGoods["aprnPromote"].asDouble());
				if(jvGoods.isMember("aprnVoucher") && !jvGoods["aprnVoucher"].isNull())     goodinfo->set_aprnvoucher(jvGoods["aprnVoucher"].asDouble());
				if(jvGoods.isMember("shippingTax") && !jvGoods["shippingTax"].isNull())     goodinfo->set_shippingtax(jvGoods["shippingTax"].asDouble());
				if(jvGoods.isMember("origPrice") && !jvGoods["origPrice"].isNull())     goodinfo->set_origprice(jvGoods["origPrice"].asDouble());
				if(jvGoods.isMember("aprnPoint") && !jvGoods["aprnPoint"].isNull())     goodinfo->set_aprnpoint(jvGoods["aprnPoint"].asDouble());
				if(jvGoods.isMember("aprnPointPrice") && !jvGoods["aprnPointPrice"].isNull())     goodinfo->set_aprnpointprice(jvGoods["aprnPointPrice"].asDouble());
				if(jvGoods.isMember("aprnCash") && !jvGoods["aprnCash"].isNull())     goodinfo->set_aprncash(jvGoods["aprnCash"].asDouble());
				if(jvGoods.isMember("refundablePrice") && !jvGoods["refundablePrice"].isNull())     goodinfo->set_refundableprice(jvGoods["refundablePrice"].asDouble());
				if(jvGoods.isMember("productName") && !jvGoods["productName"].isNull())     goodinfo->set_productname(jvGoods["productName"].asCString());
				if(jvGoods.isMember("rssSeq") && !jvGoods["rssSeq"].isNull())     goodinfo->set_rssseq(jvGoods["rssSeq"].asCString());
			}

			// 发送给ImServer
            SendReturnOrderToImServer(returninfo);
		}
    }

    delete[] pGbkReturn;
    pGbkReturn = NULL;
}

// 将退换货订单传给imserver
void CThreadResponseKafka::SendReturnOrderToImServer(const ReturnOrder &returnorder)
{
    // 转换为二进制流
    uint32_t msg_size = returnorder.ByteSize();
    char* szData = new char[msg_size];
    returnorder.SerializeToArray(szData, msg_size);

    MsgPushInd mpInd;
    mpInd.set_mptype(MP_RETURNORDER);
    mpInd.set_mpdata(szData, msg_size);

    delete szData;
    szData = NULL;

    msg_size = mpInd.ByteSize();
    szData = new char[msg_size];
    mpInd.SerializeToArray(szData, msg_size);

    // 组包
    DataPacket dp;
    XT_HEAD_PB *pHead = dp.mutable_head();
    pHead->set_cmd(SS_CID_MSG_PUSH);
    dp.set_data(szData, msg_size);

    delete szData;
    szData = NULL;

    msg_size = dp.ByteSize();
    szData = new char[msg_size];
    if (!dp.SerializeToArray(szData, msg_size))
    {
        CLog::Log("SendReturnOrderToImServer", CLog::TYPE_ERROR, "退货商品 protobuf 序列号失败");
    }
    else
    {
        CLog::Log("SendReturnOrderToImServer", CLog::TYPE_DEBUG, "退货商品 protobuf 序列号成功。二进制长度:%d", msg_size);
    }

    int rc = zmq_send(m_sc, (void*)szData, msg_size, 0);
    if (rc < 0)
    {
        CLog::Log("SendReturnOrderToImServer", CLog::TYPE_ERROR, "zmq 发送消息失败");
    }
    else
    {
        CLog::Log("SendReturnOrderToImServer", CLog::TYPE_DEBUG, "zmq 发送消息成功");
    }

    delete szData;
    szData = NULL;
}
