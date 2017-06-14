#ifndef  _BACKCTRL_H_
#define _BACKCTRL_H_

#define CMD_BACK_LOGIN  0x0001
#define CMD_BACK_LOGINACK 0x0002
#define CMD_BACK_GETUSERINFO 0x0003
#define CMD_BACK_LOGIN3 0x0004
#define CMD_BACK_LOGIN4 0x0005

static char g_backkey[17] = "abcdefghigkl1234";//没有用到结尾符
//static char g_globalip[16] = "221.238.248.100";
//static char g_connectip[16] = "221.238.248.99";

#endif

