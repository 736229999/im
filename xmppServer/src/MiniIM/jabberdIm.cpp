#include "jabberdIm.h"
#include <curl/curl.h>
#include "Log.h"
#include <stringprep.h>
#include "md5.h"
#include "xtpacket.h"
#include "Function.h"
#include "json/json.h"
#include "improtocol.pb.h"

extern map<uint32,sess_t> sessManager;


sig_atomic_t JabberdIm::c2s_shutdown = 0;
sig_atomic_t JabberdIm::c2s_logrotate = 0;
sig_atomic_t JabberdIm::c2s_sighup = 0;
//sig_atomic_t c2s_lost_router = 0;

extern IDNAPI const Stringprep_profile stringprep_nameprep[];
#undef stringprep_nameprep

#define stringprep_nameprep(in, maxlen)                        \
  stringprep(in, maxlen, (Stringprep_profile_flags)0, stringprep_nameprep)


const char* JabberdIm::TransPwd(const char* pwd)
{
//	CLog::Log("CBusiLogin::TranPwd", CLog::TYPE_DEBUG, "PWD:%s",pwd);
	
	string symbol_space = "+";
	string symbol_split = "&";
	string symbol_special = "%";

	string url_space = "%2B";
	string url_split = "%26";
	string url_special = "%25";

	string UrlPwd(pwd);

	string::size_type nPos = 0;

	nPos = UrlPwd.find(symbol_special,0);
	while(nPos != string::npos)
	{
		UrlPwd.replace(nPos,symbol_special.length(),url_special);

		nPos = UrlPwd.find(symbol_special,nPos + 1);
	}

	nPos = UrlPwd.find(symbol_split,0);
	while(nPos != string::npos)
	{
		UrlPwd.replace(nPos,symbol_split.length(),url_split);

		nPos = UrlPwd.find(symbol_split,nPos + 1);
	}
	
	nPos = UrlPwd.find(symbol_space,0);
	while(nPos != string::npos)
	{
		UrlPwd.replace(nPos,symbol_space.length(),url_space);

		nPos = UrlPwd.find(symbol_space,nPos + 1);
	}

//	CLog::Log("CBusiLogin::TranPwd", CLog::TYPE_DEBUG, "Tran PWD:%s",UrlPwd.c_str());

	return UrlPwd.c_str();
}

static long curl_writer(void *data, int size, int nmemb, string *content)
{
    long sizes = size * nmemb;
    *content += string((char*)data, sizes);
    return sizes;
}

bool JabberdIm::CheckBuyer(const char* user, const char* password, char* guid,  char * szUserName,const char *ip)
{
	CConfigInfo m_Config;
	//默认使用用户名登陆
	std::string sPostParam = "type=3";
	if (strlen(user) == 11 && user[0] == '1' && atoi(user) != 0) {	//使用手机号登陆
		sPostParam = "type=2";
	} else if (CheckEmail(user)) {	//使用Email登陆
		sPostParam = "type=1";
	}
	/*
	
	username	string	Y	用户名
	password	string	Y	密码
	token		string	Y	token
	ip			string	Y	登录者ip
	referer 	string	Y	登录来源: 
							pc是登录来源链接;
							PC IM是"moumou" ;无线是"app"
	channel 	int 	Y	登录平台  
							1:	pc	
							2：触屏 	  
							3：android 
							4:	ios
	channelFrom int 	Y	平台登录来源 
							0： 飞牛网用户 
	isFromReg	Int 	Y	是否是注册后登录
							0：不是从注册后登录，1是注册后登录
	
	*/

	//加入用户名
	sPostParam += "&username=";
	sPostParam += user;
	//加入密码
	sPostParam += "&password=";
	sPostParam += Base64Encode(password);
	//加入token
	sPostParam += "&token=1";
	//加入登陆者IP
	sPostParam += "&ip=";
	sPostParam += ip;
	//加入referer
	sPostParam += "&referer=moumou";	
	//加入channel
	sPostParam += "&channel=3";
	//加入channelFrom
	sPostParam += "&channelFrom=0";
	//加入isFromReg
	sPostParam += "&isFromReg=0";
		
	std::string data;

	//通过http接口获取买家信息
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, m_Config.GetBuyerURL());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sPostParam.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	CURLcode res = curl_easy_perform(curl);

	//清理库接口
	curl_easy_cleanup(curl);

	//判断获取是否成功
	if (res != CURLE_OK) 
	{
		CLog::Log("CheckBuyer",CLog::TYPE_ERROR,"Get buyer infomation error!");
		return false;
	}

	//解析json
	Json::Reader reader;
	Json::Value recv;
	if (reader.parse(data, recv)) {
		//解析Header中的Status
		uint32 code = -1;
		string status;
		if(recv["code"].isUInt())
		{
			code = recv["code"].asUInt();
			if(code != 200)
			{
				//没找到子串
				CLog::Log("CheckBuyer",CLog::TYPE_ERROR," code is not right %d!",code);
				return false;			
			}
		}
		
		if(recv["msg"].isString())
		{
			status = recv["msg"].asString();
		}
		//判断是否有子串012002100
		if(status.find("ok") != std::string::npos)
		{
			string strGuid = recv["data"]["user"]["MEM_GUID"].asString();
			string strUserName = recv["data"]["user"]["MEM_USERNAME"].asString();
	
			strcpy(guid,strGuid.c_str());
			strcpy(szUserName, strUserName.c_str());
			//找到了子串
			return true;
		}
	}

	//没找到子串
	CLog::Log("CheckBuyer",CLog::TYPE_ERROR," Not found userinfo!");
	return false;
}



bool JabberdIm::CheckSeller(char* user, const char* pwd, uint32& uid, uint32& merchant_id, uint32& state)
{
	
	CConfigInfo m_Config;
	//特殊符号转义
	merchant_id = 0;
	uid = 0;
	state = 0;
	const char* password = TransPwd(pwd);
	if(password == NULL)
	{
		CLog::Log("CBusiLogin::CheckSeller", CLog::TYPE_ERROR, "password:%s trans is NULL",pwd);
		return false;
	}	

	//初始化http　get请求
	std::string sPostParam = R"(version=1.0&method=feiniu.member.loginForMoumou&params={"loginName": ")";
	//加入用户名
	sPostParam += user;
	sPostParam += R"(", "password": ")";
	//加入密码
	sPostParam += password;
	sPostParam += R"("})";

	char utfStr[512];
	//首先先将gbk编码转换为unicode编码
	if(NULL==setlocale(LC_ALL,"zh_CN.gbk"))//设置转换为unicode前的码,当前为gbk编码
	{
		CLog::Log("CBusiLogin",CLog::TYPE_ERROR,"setlocale Bad Parameter");
		return false;
	}

	int unicodeLen=mbstowcs(NULL,sPostParam.c_str(),0);//计算转换后的长度
	if(unicodeLen<=0)
	{
		CLog::Log("CBusiLogin",CLog::TYPE_ERROR,"mbstowcs Can not Transfer!!!");
		return false;
	}
	wchar_t *unicodeStr=(wchar_t *)calloc(sizeof(wchar_t),unicodeLen+1);
	mbstowcs(unicodeStr,sPostParam.c_str(),sPostParam.size());//将gbk转换为unicode

	//将unicode编码转换为utf8编码
	if(NULL==setlocale(LC_ALL,"zh_CN.utf8"))//设置unicode转换后的码,当前为utf8
	{
		CLog::Log("CBusiLogin", CLog::TYPE_ERROR, "setlocale Bad Parameter");
		return false;
	}
	int utfLen=wcstombs(NULL,unicodeStr,0);//计算转换后的长度
	if(utfLen<=0)
	{
		CLog::Log("CBusiLogin", CLog::TYPE_ERROR, "wcstombs Can not Transfer!!!");
		return false;
	}
	else if(utfLen>=512)//判断空间是否足够
	{
		CLog::Log("CBusiLogin", CLog::TYPE_ERROR, "Dst Str memory not enough: %d", utfLen);
		return false;
	}
	wcstombs(utfStr,unicodeStr,utfLen);
	utfStr[utfLen]=0;//添加结束符
	free(unicodeStr);


	std::string data;

	//通过HTTP接口获取卖家信息
	CURL *curl = curl_easy_init();
	if(curl == NULL)
	{		
		CLog::Log("CBusiLogin", CLog::TYPE_ERROR, "curl_easy_init error");
	}
	curl_easy_setopt(curl, CURLOPT_URL, m_Config.GetSellerURL());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, utfStr);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
	CURLcode res = curl_easy_perform(curl);

	//清理库内存
	curl_easy_cleanup(curl);

	//判断是否获取成
	if (res != CURLE_OK) {		
		CLog::Log("CheckSeller", CLog::TYPE_ERROR, "Get seller infomation error!");
		return false;
	}

	//解析json字符创
	Json::Reader reader;
	Json::Value recv;
	if (reader.parse(data, recv)) {
		//获取result字段
		std::string result = recv["result"].asString();
		printf("%s\n", result.c_str());
		if(result != "null")
		{
			//解析result字段
			Json::Value jvResult;
			if(reader.parse(result.c_str(),jvResult))
			{
				//获取auth中的memberId字段
				uid = jvResult["auth"]["memberId"].asInt();
				
				//获取auth中的merchantId字段
				merchant_id = jvResult["auth"]["merchantId"].asInt();
				state = jvResult["auth"]["state"].asInt();
				string username = jvResult["auth"]["username"].asString();
				//strcpy(user, username.c_str());
				int ret = utf2gbk((char *)username.c_str(),username.length(),(char *)user,MAX_USERNAME_LEN);		 	
				if(ret == -1)
				{
					CLog::Log("CheckSeller",CLog::TYPE_ERROR,"user:%s, data:%s, utf8 to gbk changed failed!",user, data.c_str());							
					return false;
				}
				return true;
			}
		}
	}
	CLog::Log("CheckSeller",CLog::TYPE_ERROR,"user:%s, data:%s, parse failed!",user, data.c_str());
	return false;
}


void JabberdIm::MD5Password(char* pPassword)
{
	unsigned char digest[16];

	char password[255] = {0};
	sprintf_s(password,"%s",pPassword);

	MD5_CTX_IM md5T;
	md5T.MD5Init();
	md5T.MD5Update ((unsigned char*)password,(uint32)strlen(password) );
	md5T.MD5Final (digest);

	string sHex=ToHexString(digest,16);

	strcpy(pPassword,sHex.c_str());

}


JabberdIm::JabberdIm()
{
    //c2s_lost_router = 0;

	
}

void JabberdIm::_c2s_signal(int signum)
{
    c2s_shutdown = 1;
//    c2s_lost_router = 0;
}

void JabberdIm::_c2s_signal_hup(int signum)
{
    c2s_logrotate = 1;
    c2s_sighup = 1;
}

void JabberdIm::_c2s_signal_usr1(int signum)
{
    set_debug_flag(0);
}

void JabberdIm::_c2s_signal_usr2(int signum)
{
    set_debug_flag(1);
}

/** store the process id */
void JabberdIm::_c2s_pidfile(c2s_t c2s)
{
    const char *pidfile;
    FILE *f;
    pid_t pid;

    pidfile = config_get_one(c2s->config, "pidfile", 0);
    if(pidfile == NULL)
        return;

    pid = getpid();

    if((f = fopen(pidfile, "w+")) == NULL)
    {
        log_write(c2s->log, LOG_ERR, "couldn't open %s for writing: %s", pidfile, strerror(errno));
        return;
    }

    if(fprintf(f, "%d", pid) < 0)
    {
        log_write(c2s->log, LOG_ERR, "couldn't write to %s: %s", pidfile, strerror(errno));
        fclose(f);
        return;
    }

    fclose(f);

    log_write(c2s->log, LOG_INFO, "process id is %d, written to %s", pid, pidfile);
}
/** pull values out of the config file */
void JabberdIm::_c2s_config_expand(c2s_t c2s)
{
    const char *str, *ip, *mask;
    char *req_domain, *to_address, *to_port;
    config_elem_t elem;
    int i;
    stream_redirect_t sr;

    set_debug_log_from_config(c2s->config);

    c2s->id = config_get_one(c2s->config, "id", 0);
    if(c2s->id == NULL)
        c2s->id = "c2s";

    c2s->router_ip = config_get_one(c2s->config, "router.ip", 0);
    if(c2s->router_ip == NULL)
        c2s->router_ip = "127.0.0.1";

    c2s->router_port = j_atoi(config_get_one(c2s->config, "router.port", 0), 5347);

    c2s->router_user = config_get_one(c2s->config, "router.user", 0);
    if(c2s->router_user == NULL)
        c2s->router_user = "jabberd";
    c2s->router_pass = config_get_one(c2s->config, "router.pass", 0);
    if(c2s->router_pass == NULL)
        c2s->router_pass = "secret";

    c2s->router_pemfile = config_get_one(c2s->config, "router.pemfile", 0);

    c2s->router_cachain = config_get_one(c2s->config, "router.cachain", 0);

    c2s->router_private_key_password = config_get_one(c2s->config, "router.private_key_password", 0);

    c2s->retry_init = j_atoi(config_get_one(c2s->config, "router.retry.init", 0), 3);
    c2s->retry_lost = j_atoi(config_get_one(c2s->config, "router.retry.lost", 0), 3);
    if((c2s->retry_sleep = j_atoi(config_get_one(c2s->config, "router.retry.sleep", 0), 2)) < 1)
        c2s->retry_sleep = 1;

    c2s->log_type = log_STDOUT;
    if(config_get(c2s->config, "log") != NULL)
    {
        if((str = config_get_attr(c2s->config, "log", 0, "type")) != NULL)
        {
            if(strcmp(str, "file") == 0)
                c2s->log_type = log_FILE;
            else if(strcmp(str, "syslog") == 0)
                c2s->log_type = log_SYSLOG;
        }
    }

    if(c2s->log_type == log_SYSLOG)
    {
        c2s->log_facility = config_get_one(c2s->config, "log.facility", 0);
        c2s->log_ident = config_get_one(c2s->config, "log.ident", 0);
        if(c2s->log_ident == NULL)
            c2s->log_ident = "jabberd/c2s";
    }
    else if(c2s->log_type == log_FILE)
        c2s->log_ident = config_get_one(c2s->config, "log.file", 0);

    c2s->packet_stats = config_get_one(c2s->config, "stats.packet", 0);

    c2s->local_ip = config_get_one(c2s->config, "local.ip", 0);
    if(c2s->local_ip == NULL)
        c2s->local_ip = "0.0.0.0";

    c2s->local_port = j_atoi(config_get_one(c2s->config, "local.port", 0), 0);

    c2s->local_pemfile = config_get_one(c2s->config, "local.pemfile", 0);

    c2s->local_cachain = config_get_one(c2s->config, "local.cachain", 0);

    c2s->local_private_key_password = config_get_one(c2s->config, "local.private_key_password", 0);

    c2s->local_verify_mode = j_atoi(config_get_one(c2s->config, "local.verify-mode", 0), 0);

    c2s->local_ssl_port = j_atoi(config_get_one(c2s->config, "local.ssl-port", 0), 0);

    c2s->http_forward = config_get_one(c2s->config, "local.httpforward", 0);

    c2s->io_max_fds = j_atoi(config_get_one(c2s->config, "io.max_fds", 0), 1024);

    c2s->compression = (config_get(c2s->config, "io.compression") != NULL);

    c2s->io_check_interval = j_atoi(config_get_one(c2s->config, "io.check.interval", 0), 0);
    c2s->io_check_idle = j_atoi(config_get_one(c2s->config, "io.check.idle", 0), 0);
    c2s->io_check_keepalive = j_atoi(config_get_one(c2s->config, "io.check.keepalive", 0), 0);

    c2s->pbx_pipe = config_get_one(c2s->config, "pbx.pipe", 0);

    elem = config_get(c2s->config, "stream_redirect.redirect");
    if(elem != NULL)
    {
        for(i = 0; i < elem->nvalues; i++)
        {
            sr = (stream_redirect_t) pmalloco(xhash_pool(c2s->stream_redirects), sizeof(struct stream_redirect_st));
            if(!sr)
            {
                log_write(c2s->log, LOG_ERR, "cannot allocate memory for new stream redirection record, aborting");
                exit(1);
            }
            req_domain = j_attr((const char **) elem->attrs[i], "requested_domain");
            to_address = j_attr((const char **) elem->attrs[i], "to_address");
            to_port = j_attr((const char **) elem->attrs[i], "to_port");

            if(req_domain == NULL || to_address == NULL || to_port == NULL)
            {
                log_write(c2s->log, LOG_ERR, "Error reading a stream_redirect.redirect element from file, skipping");
                continue;
            }

            // Note that to_address should be RFC 3986 compliant
            sr->to_address = to_address;
            sr->to_port = to_port;

            xhash_put(c2s->stream_redirects, pstrdup(xhash_pool(c2s->stream_redirects), req_domain), sr);
        }
    }

    c2s->ar_module_name = config_get_one(c2s->config, "authreg.module", 0);

    if(config_get(c2s->config, "authreg.mechanisms.traditional.plain") != NULL) c2s->ar_mechanisms |= AR_MECH_TRAD_PLAIN;
    if(config_get(c2s->config, "authreg.mechanisms.traditional.digest") != NULL) c2s->ar_mechanisms |= AR_MECH_TRAD_DIGEST;
    if(config_get(c2s->config, "authreg.mechanisms.traditional.cram-md5") != NULL) c2s->ar_mechanisms |= AR_MECH_TRAD_CRAMMD5;

    if(config_get(c2s->config, "authreg.ssl-mechanisms.traditional.plain") != NULL) c2s->ar_ssl_mechanisms |= AR_MECH_TRAD_PLAIN;
    if(config_get(c2s->config, "authreg.ssl-mechanisms.traditional.digest") != NULL) c2s->ar_ssl_mechanisms |= AR_MECH_TRAD_DIGEST;
    if(config_get(c2s->config, "authreg.ssl-mechanisms.traditional.cram-md5") != NULL) c2s->ar_ssl_mechanisms |= AR_MECH_TRAD_CRAMMD5;

    elem = config_get(c2s->config, "io.limits.bytes");
    if(elem != NULL)
    {
        c2s->byte_rate_total = j_atoi(elem->values[0], 0);
        if(c2s->byte_rate_total != 0)
        {
            c2s->byte_rate_seconds = j_atoi(j_attr((const char **) elem->attrs[0], "seconds"), 1);
            c2s->byte_rate_wait = j_atoi(j_attr((const char **) elem->attrs[0], "throttle"), 5);
        }
    }

    elem = config_get(c2s->config, "io.limits.stanzas");
    if(elem != NULL)
    {
        c2s->stanza_rate_total = j_atoi(elem->values[0], 0);
        if(c2s->stanza_rate_total != 0)
        {
            c2s->stanza_rate_seconds = j_atoi(j_attr((const char **) elem->attrs[0], "seconds"), 1);
            c2s->stanza_rate_wait = j_atoi(j_attr((const char **) elem->attrs[0], "throttle"), 5);
        }
    }

    elem = config_get(c2s->config, "io.limits.connects");
    if(elem != NULL)
    {
        c2s->conn_rate_total = j_atoi(elem->values[0], 0);
        if(c2s->conn_rate_total != 0)
        {
            c2s->conn_rate_seconds = j_atoi(j_attr((const char **) elem->attrs[0], "seconds"), 5);
            c2s->conn_rate_wait = j_atoi(j_attr((const char **) elem->attrs[0], "throttle"), 5);
        }
    }

    c2s->stanza_size_limit = j_atoi(config_get_one(c2s->config, "io.limits.stanzasize", 0), 0);

    /* tweak timed checks with rate times */
    if(c2s->io_check_interval == 0)
    {
        if(c2s->byte_rate_total != 0)
            c2s->io_check_interval = c2s->byte_rate_wait;

        if(c2s->stanza_rate_total != 0 && c2s->io_check_interval > c2s->stanza_rate_wait)
            c2s->io_check_interval = c2s->stanza_rate_wait;
    }

    str = config_get_one(c2s->config, "io.access.order", 0);
    if(str == NULL || strcmp(str, "deny,allow") != 0)
        c2s->access = access_new(0);
    else
        c2s->access = access_new(1);

    elem = config_get(c2s->config, "io.access.allow");
    if(elem != NULL)
    {
        for(i = 0; i < elem->nvalues; i++)
        {
            ip = j_attr((const char **) elem->attrs[i], "ip");
            mask = j_attr((const char **) elem->attrs[i], "mask");

            if(ip == NULL)
                continue;

            if(mask == NULL)
                mask = "255.255.255.255";

            access_allow(c2s->access, ip, mask);
        }
    }

    elem = config_get(c2s->config, "io.access.deny");
    if(elem != NULL)
    {
        for(i = 0; i < elem->nvalues; i++)
        {
            ip = j_attr((const char **) elem->attrs[i], "ip");
            mask = j_attr((const char **) elem->attrs[i], "mask");

            if(ip == NULL)
                continue;

            if(mask == NULL)
                mask = "255.255.255.255";

            access_deny(c2s->access, ip, mask);
        }
    }
}

void JabberdIm::_c2s_hosts_expand(c2s_t c2s)
{
    char *realm;
    config_elem_t elem;
    char id[1024];
    int i;

    elem = config_get(c2s->config, "local.id");
    if(!elem)
    {
        log_write(c2s->log, LOG_NOTICE, "no local.id configured - skipping local domains configuration");
        return;
    }
    for(i = 0; i < elem->nvalues; i++)
    {
        host_t host = (host_t) pmalloco(xhash_pool(c2s->hosts), sizeof(struct host_st));
        if(!host)
        {
            log_write(c2s->log, LOG_ERR, "cannot allocate memory for new host, aborting");
            exit(1);
        }

        realm = j_attr((const char **) elem->attrs[i], "realm");

        /* stringprep ids (domain names) so that they are in canonical form */
        strncpy(id, elem->values[i], 1024);
        id[1023] = '\0';
        if (stringprep_nameprep(id, (Stringprep_profile_flags)1024) != 0)
        {
            log_write(c2s->log, LOG_ERR, "cannot stringprep id %s, aborting", id);
            exit(1);
        }

        host->realm = (realm != NULL) ? realm : pstrdup(xhash_pool(c2s->hosts), id);

        host->host_pemfile = j_attr((const char **) elem->attrs[i], "pemfile");

        host->host_cachain = j_attr((const char **) elem->attrs[i], "cachain");

        host->host_verify_mode = j_atoi(j_attr((const char **) elem->attrs[i], "verify-mode"), 0);

        host->host_private_key_password = j_attr((const char **) elem->attrs[i], "private-key-password");

#ifdef HAVE_SSL
        if(host->host_pemfile != NULL)
        {
            if(c2s->sx_ssl == NULL)
            {
                c2s->sx_ssl = sx_env_plugin(c2s->sx_env, sx_ssl_init, host->realm, host->host_pemfile, host->host_cachain, host->host_verify_mode, host->host_private_key_password);
                if(c2s->sx_ssl == NULL)
                {
                    log_write(c2s->log, LOG_ERR, "failed to load %s SSL pemfile", host->realm);
                    host->host_pemfile = NULL;
                }
            }
            else
            {
                if(sx_ssl_server_addcert(c2s->sx_ssl, host->realm, host->host_pemfile, host->host_cachain, host->host_verify_mode, host->host_private_key_password) != 0)
                {
                    log_write(c2s->log, LOG_ERR, "failed to load %s SSL pemfile", host->realm);
                    host->host_pemfile = NULL;
                }
            }
        }
#endif

        host->host_require_starttls = (j_attr((const char **) elem->attrs[i], "require-starttls") != NULL);

        host->ar_register_enable = (j_attr((const char **) elem->attrs[i], "register-enable") != NULL);
        host->ar_register_oob = j_attr((const char **) elem->attrs[i], "register-oob");
        if(host->ar_register_enable || host->ar_register_oob)
        {
            host->ar_register_instructions = j_attr((const char **) elem->attrs[i], "instructions");
            if(host->ar_register_instructions == NULL)
            {
                if(host->ar_register_oob)
                    host->ar_register_instructions = "Only web based registration is possible with this server.";
                else
                    host->ar_register_instructions = "Enter a username and password to register with this server.";
            }
        }
        else
            host->ar_register_password = (j_attr((const char **) elem->attrs[i], "password-change") != NULL);

        /* check for empty <id/> CDATA - XXX this "1" is VERY config.c dependant !!! */
        if(! strcmp(id, "1"))
        {
            /* remove the realm even if set */
            host->realm = NULL;

            /* skip if vHost already configured */
            if(! c2s->vhost)
                c2s->vhost = host;

            /* add meaningful log "id" */
            strcpy(id, "default vHost");
        }
        else
        {
            /* insert into vHosts xhash */
            xhash_put(c2s->hosts, pstrdup(xhash_pool(c2s->hosts), id), host);
        }

        log_write(c2s->log, LOG_NOTICE, "[%s] configured; realm=%s, registration %s, using PEM:%s",
                  id, (host->realm != NULL ? host->realm : "no realm set"), (host->ar_register_enable ? "enabled" : "disabled"),
                  (host->host_pemfile ? host->host_pemfile : "Default"));
    }
}



int JabberdIm::_c2s_sx_sasl_callback(int cb, void *arg, void **res, sx_t s, void *cbarg)
{
    c2s_t c2s = (c2s_t) cbarg;
    const char *my_realm, *mech;
    sx_sasl_creds_t creds;
    static char buf[3072];
    char mechbuf[256];
    struct jid_st jid;
    jid_static_buf jid_buf;
    int i, r;
    sess_t sess;
    char skey[44];
	uint32 merchant_id = -1;
	uint32 uid = -1;
	uint32 state = -1;
	char guid[64];
	char username[128];

    /* init static jid */
    jid_static(&jid,&jid_buf);

    /* retrieve our session */
    assert(s != NULL);
    sprintf(skey, "%d", s->tag);

    /*
     * Retrieve the session, note that depending on the operation,
     * session may be null.
     */
    sess = (sess_t)xhash_get(c2s->sessions, skey);

    switch(cb)
    {
        case sx_sasl_cb_GET_REALM:

            if(s->req_to == NULL)   /* this shouldn't happen */
                my_realm = "";

            else
            {
                host_t host;
                /* get host for request */
                host = (host_t)xhash_get(c2s->hosts, s->req_to);
                if(host == NULL)
                {
                    log_write(c2s->log, LOG_ERR, "SASL callback for non-existing host: %s", s->req_to);
                    *res = (void *)NULL;
                    return sx_sasl_ret_FAIL;
                }

                my_realm = host->realm;
                if(my_realm == NULL)
                    my_realm = s->req_to;
            }

            strncpy(buf, my_realm, 256);
            *res = (void *)buf;

            log_debug(ZONE, "sx sasl callback: get realm: realm is '%s'", buf);
            return sx_sasl_ret_OK;
            break;

        case sx_sasl_cb_GET_PASS:
            assert(sess != NULL);
            creds = (sx_sasl_creds_t) arg;

            log_debug(ZONE, "sx sasl callback: get pass (authnid=%s, realm=%s)", creds->authnid, creds->realm);
#if 0
            if(c2s->ar->get_password && (c2s->ar->get_password)(
                   c2s->ar, sess, (char *)creds->authnid, (creds->realm != NULL) ? (char *)creds->realm: "", buf) == 0)
            {
                *res = buf;
                return sx_sasl_ret_OK;
            }
#endif
            return sx_sasl_ret_FAIL;

        case sx_sasl_cb_CHECK_PASS:
            assert(sess != NULL);
            creds = (sx_sasl_creds_t) arg;

            log_debug(ZONE, "sx sasl callback: check pass (authnid=%s, realm=%s passwd=%s)",
				creds->authnid, creds->realm,creds->pass);
#if 0
            if(c2s->ar->check_password != NULL)
            {
                if ((c2s->ar->check_password)(
                        c2s->ar, sess, (char *)creds->authnid, (creds->realm != NULL) ? (char *)creds->realm : "", (char *)creds->pass) == 0)
                    return sx_sasl_ret_OK;
                else
                    return sx_sasl_ret_FAIL;
            }

            if(c2s->ar->get_password != NULL)
            {
                if ((c2s->ar->get_password)(c2s->ar, sess, (char *)creds->authnid, (creds->realm != NULL) ? (char *)creds->realm : "", buf) != 0)
                    return sx_sasl_ret_FAIL;

                if (strcmp(creds->pass, buf)==0)
                    return sx_sasl_ret_OK;
            }
#endif	

			if(CheckBuyer(creds->authnid,creds->pass,guid,username,sess->ip))
			{
				 log_debug(ZONE, "@#####sasl  plaintext auth (check) succeded username %s pwd %s  ip %s port %d guid %s ",
	              creds->authnid,creds->pass,sess->ip,sess->port);
				 	char cPass[40 + 1];
					unsigned char digest[16];
					
					strcpy(cPass, creds->pass);
					MD5Password(cPass);

					//判断登陆的是卖家还是买家
					uint32 m_uid;
					char szGuid[MAX_GUID_LEN] = "";
					char szUserName[MAX_USERNAME_LEN + 1] = "";
				    char szMobileNo[MAX_PHONE_LEN + 1]="";
				    char szEmail[MAX_EMAIL_LEN+1]="";
					char szNickName[61]="";
					char szHeadImgUrl[MAX_HEADURL_LEN+1] ="";
				 //验证通过,存储帐号
				 if(strlen(guid)== 36 )
				 {				 	
					strncpy(szNickName,username,MAX_USERNAME_LEN);
					 if(strlen(username) == 0)
					 {
						 //sprintf_s(m_szError, "%s", "用户名错误");
						 CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Buyer: %s GetUserName Failed!",creds->authnid);
						 return sx_sasl_ret_FAIL;
					 }
					 else if(((CDataBaseWeb *)c2s->m_databaseWeb)->SaveAccountBuyer(username, cPass, guid, m_uid, szMobileNo, szEmail, szNickName, szHeadImgUrl) != 0)
					 {
						 //sprintf_s(m_szError, "%s", "您的用户名或密码错误");
						 CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Buyer:%s Save Account Failed!",creds->authnid);
						 return sx_sasl_ret_FAIL;
					 }
				 }
				 else
				 {
					 //sprintf_s(m_szError, "%s", "您用户名或密码错误");
					 CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Buyer:%s, GUID:%s error!",creds->authnid,guid);
					 return sx_sasl_ret_FAIL;
				 }		

				// 登录前先删除原有的会话信息
				map<uint32,sess_t>::iterator sessold = sessManager.find(m_uid);
    			if(sessold != sessManager.end())
    			{
        			log_debug(ZONE, "map erase old fd");
        			mio_close(sessold->second->c2s->mio, sessold->second->fd);
        			sessold->second->fd = NULL;
        			sessManager.erase(sess->m_selfID);
   				}
				 
				 sess->m_selfID = m_uid;
				 strcpy(sess->m_username,creds->authnid);				 
			    RecvDataStruct packets;
		//(uint32 fd,char *username,char *pwd,uint32 unsigned src_ip, uint32 src_port,uint32 cmd,RecvDataStruct *RecvData)
			    log_debug(ZONE, "generatepkt   fd:%d", c2s->server_fd->fd);
			    uint32 ipaddr = inet_addr(sess->ip);
			    ((generatepkt)c2s->m_generatepkt)((uint32)sess->fd->fd,(char *)creds->authnid,(char *)creds->pass,ipaddr,sess->port,(uint32)0x211,0,0,&packets,NULL);
			    ((pfnjabberdCallback) c2s->m_pfnUserCallback)(c2s->m_pUserKey,NE_RECVDATA,&packets);				
				//sleep(1);
				return sx_sasl_ret_OK;
			}
			else
			{
				return sx_sasl_ret_FAIL;
			}
           
            break;

        case sx_sasl_cb_CHECK_AUTHZID:
            assert(sess != NULL);
            creds = (sx_sasl_creds_t) arg;

            /* we need authzid to validate */
            if(creds->authzid == NULL || creds->authzid[0] == '\0')
                return sx_sasl_ret_FAIL;

            /* authzid must be a valid jid */
            if(jid_reset(&jid, creds->authzid, -1) == NULL)
                return sx_sasl_ret_FAIL;

            /* and have domain == stream to addr */
            if(!s->req_to || (strcmp(jid.domain, s->req_to) != 0))
                return sx_sasl_ret_FAIL;

            /* and have no resource */
            if(jid.resource[0] != '\0')
                return sx_sasl_ret_FAIL;
#if 0
            /* and user has right to authorize as */
            if (c2s->ar->user_authz_allowed)
            {
                if (c2s->ar->user_authz_allowed(c2s->ar, sess, (char *)creds->authnid, (char *)creds->realm, (char *)creds->authzid))
                    return sx_sasl_ret_OK;
            }
            else
            {
                if (strcmp(creds->authnid, jid.node) == 0 &&
                    (c2s->ar->user_exists)(c2s->ar, sess, jid.node, jid.domain))
                    return sx_sasl_ret_OK;
            }
#endif
            return sx_sasl_ret_FAIL;

        case sx_sasl_cb_GEN_AUTHZID:
            /* generate a jid for SASL ANONYMOUS */
            jid_reset(&jid, s->req_to, -1);

            /* make node a random string */
            jid_random_part(&jid, jid_NODE);

            strcpy(buf, jid.node);

            *res = (void *)buf;

            return sx_sasl_ret_OK;
            break;

        case sx_sasl_cb_CHECK_MECH:
            mech = (char *)arg;

            i=0;
            while(i<sizeof(mechbuf) && mech[i]!='\0')
            {
                mechbuf[i]=tolower(mech[i]);
                i++;
            }
            mechbuf[i]='\0';

            /* Determine if our configuration will let us use this mechanism.
             * We support different mechanisms for both SSL and normal use */

            if (strcmp(mechbuf, "digest-md5") == 0)
            {
                /* digest-md5 requires that our authreg support get_password */
#if 0
                if (c2s->ar->get_password == NULL)
#endif
                    return sx_sasl_ret_FAIL;
            }
            else if (strcmp(mechbuf, "plain") == 0)
            {
                /* plain requires either get_password or check_password */
#if 0
                if (c2s->ar->get_password == NULL && c2s->ar->check_password == NULL)
#endif
                    return sx_sasl_ret_FAIL;
            }

            /* Using SSF is potentially dangerous, as SASL can also set the
             * SSF of the connection. However, SASL shouldn't do so until after
             * we've finished mechanism establishment
             */
            if (s->ssf>0)
            {
                r = snprintf(buf, sizeof(buf), "authreg.ssl-mechanisms.sasl.%s",mechbuf);
                if (r < -1 || r > sizeof(buf))
                    return sx_sasl_ret_FAIL;
                if(config_get(c2s->config,buf) != NULL)
                    return sx_sasl_ret_OK;
            }

            r = snprintf(buf, sizeof(buf), "authreg.mechanisms.sasl.%s",mechbuf);
            if (r < -1 || r > sizeof(buf))
                return sx_sasl_ret_FAIL;

            /* Work out if our configuration will let us use this mechanism */
            if(config_get(c2s->config,buf) != NULL)
                return sx_sasl_ret_OK;
            else
                return sx_sasl_ret_FAIL;
        default:
            break;
    }

    return sx_sasl_ret_FAIL;
}
void JabberdIm::_c2s_time_checks(c2s_t c2s)
{
    sess_t sess;
    time_t now;
    union xhashv xhv;

    now = time(NULL);

    if(xhash_iter_first(c2s->sessions))
        do
        {
            xhv.sess_val = &sess;
            xhash_iter_get(c2s->sessions, NULL, NULL, xhv.val);

            if(c2s->io_check_idle > 0 && sess->s && now > sess->last_activity + c2s->io_check_idle)
            {
                log_write(c2s->log, LOG_NOTICE, "[%d] [%s, port=%d] timed out", sess->fd->fd, sess->ip, sess->port);

                sx_error(sess->s, stream_err_HOST_GONE, "connection timed out");
                sx_close(sess->s);

                continue;
            }

            if(c2s->io_check_keepalive > 0 && now > sess->last_activity + c2s->io_check_keepalive && sess->s->state >= state_STREAM)
            {
                log_debug(ZONE, "sending keepalive for %d", sess->fd->fd);

                sx_raw_write(sess->s, " ", 1);
            }

            if(sess->rate != NULL && sess->rate->bad != 0 && rate_check(sess->rate) != 0)
            {
                /* read the pending bytes when rate limit is no longer in effect */
                log_debug(ZONE, "reading throttled %d", sess->fd->fd);
                sess->s->want_read = 1;
                sx_can_read(sess->s);
            }

        }
        while(xhash_iter_next(c2s->sessions));
}

void JabberdIm::InitHead(LPXT_HEAD lpHead,uint16 cmd,uint16 len,uint32 sid,uint32 did,uint8 dtype , uint8 flag)
{
    lpHead->cmd = cmd;
    lpHead->did = did;
    lpHead->dtype = dtype;
    lpHead->flag = XT_HEAD::FLAG_XMPPREQ;
    lpHead->from = 0x9999;  //版本号
    lpHead->len = len;
    lpHead->seq = 0;
    lpHead->sid = sid;
}

void JabberdIm::InitHeadSasl(LPXT_HEAD lpHead,uint16 cmd,uint16 len,uint32 sid,uint32 did,uint8 dtype , uint8 flag)
{
    lpHead->cmd = cmd;
    lpHead->did = did;
    lpHead->dtype = dtype;
    lpHead->flag = XT_HEAD::FLAG_XMPPSASL;
    lpHead->from = 0x9999;  //版本号
    lpHead->len = len;
    lpHead->seq = 0;
    lpHead->sid = sid;
}


int JabberdIm::InitPkt(uint32 fd,uint32 src_ip, uint16 src_port,RecvDataStruct *RecvData)
{
    RecvData->src_ip = src_ip;
    RecvData->port = src_port;
    RecvData->src_type = 1;
    RecvData->vlink = fd;
}


#if 0
int JabberdIm::InitData()
{


}


int JabberdIm::Encrypt()
{


}

int JabberdIm::SendP2PMsg(uint32 fd,char *username,char *pwd,uint32 src_ip, uint16 src_port,uint16 cmd,
                          uint32 fromid,uint32 toid,RecvDataStruct *RecvData)
{

    RecvData->src_ip = src_ip;
    RecvData->port = src_port;

    RecvData->src_type = 1;
    //RecvData->data_len = 1;
    RecvData->vlink = fd;

    XT_HEAD xtHead;
    InitHead(head,CMD_P2PMSG_SEND,req.to_id,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
    InitHead(&head,(uint16)CMD_P2PMSG_SEND,sendPkt.GetSize(),0,0,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);
    InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),0,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);

    CSendPacket sendPkt((char *)RecvData->data);

    XT_HEAD xtHead;

    InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),0,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);

    sendPkt.SetXtHead(xtHead);

    sendPacket.SetXtHead(head);

    char crypt[MAX_SEND_BUFFERS];
    long crypt_len;
    uint8 recv_flag = type;

    char random_key[16] = "123456";
    CSendPacket cp(crypt);

    {
        //通过服务器中转
        cp  <<req.from_id
            <<req.to_id
            <<req.data_type
            <<req.ver
            <<req.send_time
            //<<req.ms
            <<recv_flag;
        if (req.data_type==XT_MSG::IM_MSG
            || req.data_type==XT_MSG::EVALUA_ACK
            || req.data_type==XT_MSG::OPEN_P2PSENDMSG_DLG
            || req.data_type==XT_MSG::CUST_SER_TRAN_REQ
            || req.data_type==XT_MSG::AUTO_REPLY
            || req.data_type==XT_MSG::IM_MSG_INSERT)
        {
            cp<<req.fontSize<<req.fontColor<<req.fontStyle;
            cp<<req.fontName;
        }
        // 写入聊天信息内容
        cp.WriteData(req.data,req.data_len);
        if ( req.data_type==XT_MSG::IM_MSG
             || req.data_type==XT_MSG::EVALUA_ACK
             || req.data_type==XT_MSG::OPEN_P2PSENDMSG_DLG
             || req.data_type==XT_MSG::CUST_SER_TRAN_REQ
             || req.data_type==XT_MSG::AUTO_REPLY
             || req.data_type==XT_MSG::IM_MSG_INSERT)
            cp<<req.from_nickname;
        // 加密
        TEAEncrypt(crypt,cp.GetSize(),
                   (char *)random_key,
                   crypt,&crypt_len);
        sendPacket.WritePacket(crypt,crypt_len);
    }

    sendData.data_len = sendPacket.GetSize();
    return m_pUdpComm->SendDataTo(sendData);
}
#endif


void JabberdIm::generate_pkt(uint32 fd,char *username,char *pwd,uint32 src_ip, uint16 src_port,uint16 cmd,
                             uint32 fromid,uint32 toid,RecvDataStruct *RecvData,void *input)
{
	printf("generate fd %d username %s password %s src_ip %u src_port %d  cmd %#x \r\n",
           fd,username,pwd,src_ip, src_port,cmd );
    char random_key[16] = "123456";
    char cryptData[MAX_SEND_BUFFERS];
    CSendPacket cryptPacket(cryptData);
    //CSendPacket cp(cryptData);
    CSendPacket sendPkt((char *)RecvData->data);
    XT_HEAD xtHead;
    int crypt_len;
    uint8 req_flag = 0;
    uint8 data_type = XT_MSG::IM_MSG;
    XT_MSG *req = (XT_MSG *)input;
    XT_USERINFO_GET_REQ *userinfo_req = (XT_USERINFO_GET_REQ *)input;
    XT_GET_SUB_ACCOUNT_REQ *subaccount_req = (XT_GET_SUB_ACCOUNT_REQ *)input;
    XT_DIRECTORY_REQ *directory_req = (XT_DIRECTORY_REQ *)input;
	XT_WEB_BUSI_LIST_REQ *busilist_req = (XT_WEB_BUSI_LIST_REQ *)input;
	XT_LOGOUT *logout_req = (XT_LOGOUT *)input;
	MsgListReq *msglist_req = (MsgListReq*)input;
    switch(cmd)
    {
        case 0x211:

            InitPkt(fd,src_ip,src_port,RecvData);

            InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),0,0,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);

            sendPkt.SetXtHead(xtHead);

            req_flag |= XT_LOGIN_REQ::LOGINBYNAME;
            req_flag |= (XT_LOGIN_REQ::BUYER << 2);
            sendPkt<< req_flag <<username;

            sendPkt.WritePacket((char *)random_key,sizeof(random_key));

            // 写验证码
            //cryptPacket.WritePacket(crypt,16);
            //cryptPacket.WritePacket(random_key,sizeof(random_key));

            cryptPacket.WritePacket(pwd,24);

            cryptPacket<<(uint8)0<<src_ip<<(uint16)src_port;

            xTEAEncryptWithKey(cryptPacket.GetData(),cryptPacket.GetSize(),
                               (char *)random_key,
                               cryptData,&crypt_len);

            sendPkt.WriteData(cryptData,crypt_len);
            RecvData->data_len =sendPkt.GetSize();

            return;

        case 0x60b:
            InitPkt(fd,src_ip,src_port,RecvData);

            //InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);
            InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),req->from_id,req->to_id,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);

            sendPkt.SetXtHead(xtHead);





            {
                //通过服务器中转
                cryptPacket <<req->from_id
                            <<req->to_id
                            <<req->data_type
                            <<(uint16) 0x999
                            <<(uint32) req->send_time
                            //<<req.ms
                            <<req->recv_flag;
                if (req->data_type==XT_MSG::IM_MSG
                    //|| req->data_type==XT_MSG::EVALUA_ACK
                    || req->data_type==XT_MSG::CUST_SER_TRAN_REQ
                    || req->data_type==XT_MSG::AUTO_REPLY
                    /*|| req->data_type==XT_MSG::IM_MSG_INSERT*/)
                {
                    cryptPacket<<req->fontSize<<req->fontColor<<req->fontStyle;
                    cryptPacket<<req->fontName;
                }
                // 写入聊天信息内容
                cryptPacket.WriteData(req->data,req->data_len);
                if ( req->data_type==XT_MSG::IM_MSG
                     /*|| req->data_type==XT_MSG::EVALUA_ACK
                     || req->data_type==XT_MSG::OPEN_P2PSENDMSG_DLG
                     || req->data_type==XT_MSG::CUST_SER_TRAN_REQ*/
                     || req->data_type==XT_MSG::AUTO_REPLY
                     /*|| req->data_type==XT_MSG::IM_MSG_INSERT*/)
                    cryptPacket<<req->from_nickname;

                cryptPacket<<req->uuid;    
                // 加密
#if 0
                xTEAEncryptWithKey(cryptData,cryptPacket.GetSize(),
                                   (char *)random_key,
                                   cryptData,&crypt_len);
                sendPkt.WritePacket(cryptData,crypt_len);
#endif

                sendPkt.WritePacket(cryptData,cryptPacket.GetSize());
                RecvData->data_len = sendPkt.GetSize();

                printf("##sendPkt.GetSize() %d RecvData:data_len  %d\r\n",sendPkt.GetSize(),RecvData->data_len  );
            }


            return;


        case 0x415:

            InitPkt(fd,src_ip,src_port,RecvData);

            //InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);
            InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);

            sendPkt.SetXtHead(xtHead);


            {
                //通过服务器中转
                cryptPacket <<userinfo_req->fid;


                sendPkt.WritePacket(cryptData,cryptPacket.GetSize());
                RecvData->data_len = sendPkt.GetSize();

                printf("##sendPkt.GetSize() %d RecvData:data_len  %d\r\n",sendPkt.GetSize(),RecvData->data_len  );
            }

            return;

        case    CMD_GET_SUB_ACCOUNT_REQ:

            InitPkt(fd,src_ip,src_port,RecvData);

            //InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);
            InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);

            sendPkt.SetXtHead(xtHead);


            {
                //通过服务器中转
                cryptPacket <<subaccount_req->merchant_id
                            <<subaccount_req->group_id
                            <<subaccount_req->type;


                sendPkt.WritePacket(cryptData,cryptPacket.GetSize());
                RecvData->data_len = sendPkt.GetSize();

                printf("##sendPkt.GetSize() %d RecvData:data_len  %d\r\n",sendPkt.GetSize(),RecvData->data_len);
            }
            break;

        case CMD_DIRECTORY_REQ:
        {
            InitPkt(fd,src_ip,src_port,RecvData);

            //InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);
            InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);

            sendPkt.SetXtHead(xtHead);

            cryptPacket <<directory_req->id<<directory_req->local_ip<<directory_req->local_port;

            sendPkt.WritePacket(cryptData,cryptPacket.GetSize());
            RecvData->data_len = sendPkt.GetSize();

            printf("##sendPkt.GetSize() %d RecvData:data_len  %d\r\n",sendPkt.GetSize(),RecvData->data_len  );
        }
        break;

		case CMD_XMPP_BUSI_LIST_REQ:
		{
	        InitPkt(fd,src_ip,src_port,RecvData);
            //InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);
            InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);
            sendPkt.SetXtHead(xtHead);
            {
                //通过服务器中转
                cryptPacket <<busilist_req->id
                            <<busilist_req->msgnum;
				
                sendPkt.WritePacket(cryptData,cryptPacket.GetSize());
                RecvData->data_len = sendPkt.GetSize();

                printf("##sendPkt.GetSize() %d RecvData:data_len  %d\r\n",sendPkt.GetSize(),RecvData->data_len);
            }
    	}
		break;

		case CMD_HISTORY_MSG_REQ:
		{
			InitPkt(fd,src_ip,src_port,RecvData);
            //InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);
            InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);
            sendPkt.SetXtHead(xtHead);
            {
				uint32_t msg_size = msglist_req->ByteSize();
    			char* szData = new char[msg_size];
    			printf("CMD_HISTORY_MSG_REQ msg_size:%d\r\n",msg_size);
    			if (!msglist_req->SerializeToArray(szData, msg_size))
    			{
        			printf("CMD_HISTORY_MSG_REQ 序列号失败\r\n");
    			}
    			else
    			{
    				sendPkt.WriteData(szData, msg_size);
                	RecvData->data_len = sendPkt.GetSize();
    			}
    			delete[] szData;
    			szData = NULL;	
            }
		}
		break;

		case CMD_LOGOUT:
		{
			InitPkt(fd,src_ip,src_port,RecvData);
            //InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);
            InitHead(&xtHead,(uint16)cmd,sendPkt.GetSize(),fromid,toid,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);

			
            sendPkt.SetXtHead(xtHead);
			cryptPacket << logout_req->id
						<< (uint8)logout_req->authData;
			sendPkt.WritePacket(cryptData,cryptPacket.GetSize());
            RecvData->data_len = sendPkt.GetSize();
		}	
		break;
        default:
            break;
    }
}


int JabberdIm::StartWork()
{


    CreateThread(
        NULL,                       // default security attributes
        0,                          // use default stack size
        jabberd,                // thread function
        this,                       // argument to thread function
        0,                          // use default creation flags
        NULL);                      // returns the thread identifier

    return RESULT_SUCCESS;
}


void *JabberdIm::jabberd(void *params)
{


	//setDataBaseWeb(&DatabaseWeb);
//	m_pDataBaseWeb = &DatabaseWeb;
	//初始化数据库
	CConfigInfo		dbConfig;
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	dbConfig.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);

	CDataBaseWeb DatabaseWeb;
	DatabaseWeb.SetConnectDB(miniuid, minipwd, minisid);
	if( !DatabaseWeb.ConnectDB() )
	{
		CLog::Log("jabberd",CLog::TYPE_ERROR,"DatabaseWeb.ConnectDB() failed!");
		return (void *)RESULT_FAILED;
	}
	//setDataBaseWeb(&DatabaseWeb);

    JabberdIm *d_Jbd = (JabberdIm *)params;
	d_Jbd->m_pDataBaseWeb = &DatabaseWeb;

    c2s_t c2s;

    char *config_file;
    int optchar;
    int mio_timeout;
    sess_t sess;
    bres_t res;
    union xhashv xhv;
    time_t check_time = 0;
    const char *cli_id = 0;

#ifdef HAVE_UMASK
    umask((mode_t) 0027);
#endif

    srand(time(NULL));

#ifdef HAVE_WINSOCK2_H
    /* get winsock running */
    {
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;

        wVersionRequested = MAKEWORD( 2, 2 );

        err = WSAStartup( wVersionRequested, &wsaData );
        if ( err != 0 )
        {
            /* !!! tell user that we couldn't find a usable winsock dll */
            return 0;
        }
    }
#endif
#if 0
    jabber_signal(SIGINT, _c2s_signal);
    jabber_signal(SIGTERM, _c2s_signal);
#ifdef SIGHUP
    jabber_signal(SIGHUP, _c2s_signal_hup);
#endif
#ifdef SIGPIPE
    jabber_signal(SIGPIPE, SIG_IGN);
#endif
    jabber_signal(SIGUSR1, _c2s_signal_usr1);
    jabber_signal(SIGUSR2, _c2s_signal_usr2);
#endif

    c2s = (c2s_t) calloc(1, sizeof(struct c2s_st));

    /* load our config */
    c2s->config = config_new();

    config_file = CONFIG_DIR "/c2s.xml";
    set_debug_flag(1);

    /* cmdline parsing */

    if(config_load_with_id(c2s->config, config_file, cli_id) != 0)
    {
        fputs("c2s: couldn't load config, aborting\n", stderr);
        config_free(c2s->config);
        free(c2s);
        return NULL;
    }

    c2s->stream_redirects = xhash_new(523);

    _c2s_config_expand(c2s);

    c2s->log = log_new(c2s->log_type, c2s->log_ident, c2s->log_facility);
    log_write(c2s->log, LOG_NOTICE, "starting up");

    _c2s_pidfile(c2s);
#if 0
    if(c2s->ar_module_name == NULL)
    {
        log_write(c2s->log, LOG_NOTICE, "no authreg module specified in config file");
    }
    else if((c2s->ar = authreg_init(c2s, c2s->ar_module_name)) == NULL)
    {
        access_free(c2s->access);
        config_free(c2s->config);
        log_free(c2s->log);
        free(c2s);
        exit(1);
    }
#endif
    c2s->sessions = xhash_new(1023);

    c2s->conn_rates = xhash_new(101);

    c2s->dead = jqueue_new();

    c2s->dead_sess = jqueue_new();

    c2s->sx_env = sx_env_new();

#ifdef HAVE_SSL
    /* get the ssl context up and running */
    if(c2s->local_pemfile != NULL)
    {
        c2s->sx_ssl = sx_env_plugin(c2s->sx_env, sx_ssl_init, NULL, c2s->local_pemfile, c2s->local_cachain, c2s->local_verify_mode, c2s->local_private_key_password);
        if(c2s->sx_ssl == NULL)
        {
            log_write(c2s->log, LOG_ERR, "failed to load local SSL pemfile, SSL will not be available to clients");
            c2s->local_pemfile = NULL;
        }
    }

    /* try and get something online, so at least we can encrypt to the router */
    if(c2s->sx_ssl == NULL && c2s->router_pemfile != NULL)
    {
        c2s->sx_ssl = sx_env_plugin(c2s->sx_env, sx_ssl_init, NULL, c2s->router_pemfile, c2s->router_cachain, NULL, c2s->router_private_key_password);
        if(c2s->sx_ssl == NULL)
        {
            log_write(c2s->log, LOG_ERR, "failed to load router SSL pemfile, channel to router will not be SSL encrypted");
            c2s->router_pemfile = NULL;
        }
    }
#endif

#ifdef HAVE_LIBZ
    /* get compression up and running */
    if(c2s->compression)
        sx_env_plugin(c2s->sx_env, sx_compress_init);
#endif

    /* get stanza ack up */
    sx_env_plugin(c2s->sx_env, sx_ack_init);

    /* and user IP address plugin */
    sx_env_plugin(c2s->sx_env, address_init);

    /* get sasl online */
    c2s->sx_sasl = sx_env_plugin(c2s->sx_env, sx_sasl_init, "xmpp", _c2s_sx_sasl_callback, (void *) c2s);
    if(c2s->sx_sasl == NULL)
    {
        log_write(c2s->log, LOG_ERR, "failed to initialise SASL context, aborting");
        exit(1);
    }

    /* get bind up */
    sx_env_plugin(c2s->sx_env, bind_init, c2s);

    c2s->mio = mio_new(c2s->io_max_fds);
    if(c2s->mio == NULL)
    {
        log_write(c2s->log, LOG_ERR, "failed to create MIO, aborting");
        exit(1);
    }

    /* hosts mapping */
    c2s->hosts = xhash_new(1021);
    _c2s_hosts_expand(c2s);
    c2s->sm_avail = xhash_new(1021);

    c2s->retry_left = c2s->retry_init;
//    _c2s_router_connect(c2s);
    //listen port 5222 socket
    {

        if(c2s->local_port != 0)
        {
            c2s->server_fd = mio_listen(c2s->mio, c2s->local_port, c2s->local_ip, _c2s_client_mio_callback, (void *) c2s);
            if(c2s->server_fd == NULL)
                log_write(c2s->log, LOG_ERR, "[%s, port=%d] failed to listen", c2s->local_ip, c2s->local_port);
            else
                log_write(c2s->log, LOG_NOTICE, "[%s, port=%d] listening for connections", c2s->local_ip, c2s->local_port);
        }
        else
            c2s->server_fd = NULL;

#ifdef HAVE_SSL
        if(c2s->local_ssl_port != 0 && c2s->local_pemfile != NULL)
        {
            c2s->server_ssl_fd = mio_listen(c2s->mio, c2s->local_ssl_port, c2s->local_ip, _c2s_client_mio_callback, (void *) c2s);
            if(c2s->server_ssl_fd == NULL)
                log_write(c2s->log, LOG_ERR, "[%s, port=%d] failed to listen", c2s->local_ip, c2s->local_ssl_port);
            else
                log_write(c2s->log, LOG_NOTICE, "[%s, port=%d] listening for SSL connections", c2s->local_ip, c2s->local_ssl_port);
        }
        else
            c2s->server_ssl_fd = NULL;
#endif
    }




    c2s->m_pUserKey = (void *)d_Jbd->m_pUserKey;
    c2s->m_pfnUserCallback = (void *)d_Jbd->m_pfnUserCallback;
    c2s->m_generatepkt =(void *) d_Jbd->generate_pkt;
	c2s->m_databaseWeb = (void *)d_Jbd->m_pDataBaseWeb;
#if 0

    RecvDataStruct packets;
    generate_pkt(0,&packets);

#if 0
    packets.src_ip = 111111111;
    packets.port = 7788;

    packets.src_type = 1;
    packets.data_len = 1;
    packets.vlink = 2;
#endif

    CLog::Log("CAppManager::init",CLog::TYPE_ERROR,"READY TO START JABBERD");

    while(1)
    {

        c2s->m_pfnUserCallback(c2s->m_pUserKey,NE_RECVDATA,&packets);

        CLog::Log("CAppManager::init",CLog::TYPE_ERROR,"we are here ! hahahaha \r\n");
        log_write(c2s->log, LOG_ERR, "we are here ! hahahaha \r\n");
        sleep(1);

    }

#endif


    log_write(c2s->log, LOG_NOTICE, "We are here,  server_fd:%d", c2s->server_fd->fd);

    mio_timeout = ((c2s->io_check_interval != 0 && c2s->io_check_interval < 5) ?
                   c2s->io_check_interval : 5);

    while(!d_Jbd->c2s_shutdown)
    {
        mio_run(c2s->mio, mio_timeout);

        if(d_Jbd->c2s_logrotate)
        {
            set_debug_log_from_config(c2s->config);

            log_write(c2s->log, LOG_NOTICE, "reopening log ...");
            log_free(c2s->log);
            c2s->log = log_new(c2s->log_type, c2s->log_ident, c2s->log_facility);
            log_write(c2s->log, LOG_NOTICE, "log started");

            d_Jbd->c2s_logrotate = 0;
        }

        if(d_Jbd->c2s_sighup)
        {
            log_write(c2s->log, LOG_NOTICE, "reloading some configuration items ...");
            config_t conf;
            conf = config_new();
            if (conf && config_load(conf, config_file) == 0)
            {
                xhash_free(c2s->stream_redirects);
                c2s->stream_redirects = xhash_new(523);

                char *req_domain, *to_address, *to_port;
                config_elem_t elem;
                int i;
                stream_redirect_t sr;

                elem = config_get(conf, "stream_redirect.redirect");
                if(elem != NULL)
                {
                    for(i = 0; i < elem->nvalues; i++)
                    {
                        sr = (stream_redirect_t) pmalloco(xhash_pool(c2s->stream_redirects), sizeof(struct stream_redirect_st));
                        if(!sr)
                        {
                            log_write(c2s->log, LOG_ERR, "cannot allocate memory for new stream redirection record, aborting");
                            exit(1);
                        }
                        req_domain = j_attr((const char **) elem->attrs[i], "requested_domain");
                        to_address = j_attr((const char **) elem->attrs[i], "to_address");
                        to_port = j_attr((const char **) elem->attrs[i], "to_port");

                        if(req_domain == NULL || to_address == NULL || to_port == NULL)
                        {
                            log_write(c2s->log, LOG_ERR, "Error reading a stream_redirect.redirect element from file, skipping");
                            continue;
                        }

                        // Note that to_address should be RFC 3986 compliant
                        sr->to_address = to_address;
                        sr->to_port = to_port;

                        xhash_put(c2s->stream_redirects, pstrdup(xhash_pool(c2s->stream_redirects), req_domain), sr);
                    }
                }
                config_free(conf);
            }
            else
            {
                log_write(c2s->log, LOG_WARNING, "couldn't reload config (%s)", config_file);
                if (conf) config_free(conf);
            }
            d_Jbd->c2s_sighup = 0;
        }
#if 0
        if(c2s_lost_router)
        {
            if(c2s->retry_left < 0)
            {
                log_write(c2s->log, LOG_NOTICE, "attempting reconnect");
                sleep(c2s->retry_sleep);
                c2s_lost_router = 0;
                if (c2s->router) sx_free(c2s->router);
                _c2s_router_connect(c2s);
            }

            else if(c2s->retry_left == 0)
            {
                c2s_shutdown = 1;
            }

            else
            {
                log_write(c2s->log, LOG_NOTICE, "attempting reconnect (%d left)", c2s->retry_left);
                c2s->retry_left--;
                sleep(c2s->retry_sleep);
                c2s_lost_router = 0;
                if (c2s->router) sx_free(c2s->router);
                _c2s_router_connect(c2s);
            }
        }
#endif
        /* cleanup dead sess (before sx_t as sess->result uses sx_t nad cache) */
        while(jqueue_size(c2s->dead_sess) > 0)
        {
            sess = (sess_t) jqueue_pull(c2s->dead_sess);

            /* free sess data */
            if(sess->ip != NULL) free((void*)sess->ip);
            if(sess->smcomp != NULL) free((void*)sess->smcomp);
            if(sess->result != NULL) nad_free(sess->result);
            if(sess->resources != NULL)
                for(res = sess->resources; res != NULL;)
                {
                    bres_t tmp = res->next;
                    jid_free(res->jid);
                    free(res);
                    res = tmp;
                }
            if(sess->rate != NULL) rate_free(sess->rate);
            if(sess->stanza_rate != NULL) rate_free(sess->stanza_rate);

            free(sess);
        }

        /* cleanup dead sx_ts */
        while(jqueue_size(c2s->dead) > 0)
            sx_free((sx_t) jqueue_pull(c2s->dead));

        /* time checks */
        if(c2s->io_check_interval > 0 && time(NULL) >= c2s->next_check)
        {
            log_debug(ZONE, "running time checks");

            _c2s_time_checks(c2s);

            c2s->next_check = time(NULL) + c2s->io_check_interval;
            log_debug(ZONE, "next time check at %d", c2s->next_check);
        }

        if(time(NULL) > check_time + 60)
        {
#ifdef POOL_DEBUG
            pool_stat(1);
#endif
            if(c2s->packet_stats != NULL)
            {
                int fd = open(c2s->packet_stats, O_TRUNC | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP);
                if (fd >= 0)
                {
                    char buf[100];
                    int len = snprintf(buf, 100, "%lld\n", c2s->packet_count);
                    if (write(fd, buf, len) != len)
                    {
                        close(fd);
                        fd = -1;
                    }
                    else close(fd);
                }
                if (fd < 0)
                {
                    log_write(c2s->log, LOG_ERR, "failed to write packet statistics to: %s", c2s->packet_stats);
                    d_Jbd->c2s_shutdown = 1;
                }
            }

            check_time = time(NULL);
        }
    }

    log_write(c2s->log, LOG_NOTICE, "shutting down");

    if(xhash_iter_first(c2s->sessions))
        do
        {
            xhv.sess_val = &sess;
            xhash_iter_get(c2s->sessions, NULL, NULL, xhv.val);

            if(sess->active && sess->s)
                sx_close(sess->s);

        }
        while(xhash_iter_next(c2s->sessions));

    /* cleanup dead sess */
    while(jqueue_size(c2s->dead_sess) > 0)
    {
        sess = (sess_t) jqueue_pull(c2s->dead_sess);

        /* free sess data */
        if(sess->ip != NULL) free((void*)sess->ip);
        if(sess->result != NULL) nad_free(sess->result);
        if(sess->resources != NULL)
            for(res = sess->resources; res != NULL;)
            {
                bres_t tmp = res->next;
                jid_free(res->jid);
                free(res);
                res = tmp;
            }

        free(sess);
    }

    while(jqueue_size(c2s->dead) > 0)
        sx_free((sx_t) jqueue_pull(c2s->dead));

    if (c2s->fd != NULL) mio_close(c2s->mio, c2s->fd);
    sx_free(c2s->router);

    sx_env_free(c2s->sx_env);

    mio_free(c2s->mio);

    xhash_free(c2s->sessions);

//   authreg_free(c2s->ar);

    xhash_free(c2s->conn_rates);

    xhash_free(c2s->stream_redirects);

    xhash_free(c2s->sm_avail);

    xhash_free(c2s->hosts);

    jqueue_free(c2s->dead);

    jqueue_free(c2s->dead_sess);

    access_free(c2s->access);

    log_free(c2s->log);

    config_free(c2s->config);

    free(c2s);

#ifdef POOL_DEBUG
    pool_stat(1);
#endif

#ifdef HAVE_WINSOCK2_H
    WSACleanup();
#endif

    return 0;
}

