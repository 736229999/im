
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[18];
};
static const struct sqlcxp sqlfpn =
{
    17,
    "./DatabaseMini.pc"
};


static unsigned int sqlctx = 6818979;


static struct sqlexd {
   unsigned long  sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
      const short *cud;
   unsigned char  *sqlest;
      const char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
   unsigned char  **sqphsv;
   unsigned long  *sqphsl;
            int   *sqphss;
            short **sqpind;
            int   *sqpins;
   unsigned long  *sqparm;
   unsigned long  **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
            int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
   unsigned char  *sqhstv[15];
   unsigned long  sqhstl[15];
            int   sqhsts[15];
            short *sqindv[15];
            int   sqinds[15];
   unsigned long  sqharm[15];
   unsigned long  *sqharc[15];
   unsigned short  sqadto[15];
   unsigned short  sqtdso[15];
} sqlstm = {12,15};

// Prototypes
extern "C" {
  void sqlcxt (void **, unsigned int *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlcx2t(void **, unsigned int *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlbuft(void **, char *);
  void sqlgs2t(void **, char *);
  void sqlorat(void **, unsigned int *, void *);
}

// Forms Interface
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern "C" { void sqliem(unsigned char *, signed int *); }

 static const char *sq0120 = 
"select MsgId ,SENDER ,SENDTIME ,MSGTYPE ,MSGLEN ,MSG  from IM_CLUSTERTALKMS\
G where (ClusterId=:b1 and MsgId>:b2) order by MsgId            ";

 static const char *sq0122 = 
"select MsgId ,SendTime ,MsgType ,MsgLen ,MSG  from IM_CLUSTERSYSMSG where C\
lusterId=:b1           ";

 static const char *sq0127 = 
"select Sender ,((((cast((cast(SendTime as  timestamp ) at time zone 'gmt') \
as date )-to_date('19700101 00:00:00','yyyymmdd hh24:mi:ss'))* 24)* 60)* 60) \
SendTime ,MsgType ,MsgLen ,MSG  from IM_OFFLINETALKMSG where Receiver=:b1 ord\
er by SendTime            ";

 static const char *sq0129 = 
"select Sender ,SendTime ,MsgType ,MsgLen ,MSG  from IM_OFFLINESYSMSG where \
Receiver=:b1           ";

 static const char *sq0133 = 
"select Msgid ,msg ,TO_CHAR(PubTime,'YYYY/MM/DD HH24:MI:SS')  from IM_SYSTEM\
MSG where (Msgid>:b1 and ExpireTime>=sysdate) order by MSGID            ";

 static const char *sq0142 = 
"select t1.MsgId ,t1.Receiver ,t1.Sender ,((((cast((cast(t1.SendTime as  tim\
estamp ) at time zone 'gmt') as date )-to_date('19700101 00:00:00','yyyymmdd \
hh24:mi:ss'))* 24)* 60)* 60) SendTime ,t1.MsgType ,t1.MsgLen ,t1.Msg ,t1.Send\
erName ,t1.Text  from IM_OFFLINETALKMSG t1 ,IM_USERONLINE t2 where (((t2.Serv\
erNo=:b1 and t2.status=1) and t1.Receiver=t2.UserId) and rownum<=:b2) order b\
y t1.Receiver,t1.MsgId            ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4138,852,0,0,
5,0,0,0,0,0,568,99,0,0,1,1,0,1,0,3,102,0,0,
24,0,0,1,91,0,1542,103,0,0,4,4,0,0,128,3,68,0,0,3,102,0,0,3,3,0,0,3,68,0,0,1,9,
0,0,
59,0,0,0,0,0,527,112,0,0,1,1,0,1,0,1,102,0,0,
78,0,0,0,0,0,527,128,0,0,1,1,0,1,0,1,102,0,0,
97,0,0,0,0,0,590,129,0,0,1,1,0,1,0,3,102,0,0,
116,0,0,0,0,0,525,136,0,0,4,1,0,1,0,1,102,0,0,2,3,0,0,2,3,0,0,2,9,0,0,
147,0,0,0,0,0,527,163,0,0,1,1,0,1,0,1,102,0,0,
166,0,0,0,0,0,590,164,0,0,1,1,0,1,0,3,102,0,0,
185,0,0,2,233,0,1542,211,0,0,14,14,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,68,0,0,
3,68,0,0,3,68,0,0,3,68,0,0,3,68,0,0,3,68,0,0,3,68,0,0,3,9,0,0,3,9,0,0,3,9,0,0,
3,68,0,0,1,9,0,0,
260,0,0,3,102,0,1542,262,0,0,5,5,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,68,0,0,3,
3,0,0,1,9,0,0,
299,0,0,4,103,0,1542,313,0,0,5,5,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,68,0,0,3,
3,0,0,1,9,0,0,
338,0,0,0,0,0,568,368,0,0,1,1,0,1,0,3,102,0,0,
357,0,0,5,94,0,1542,369,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,102,0,0,3,3,0,0,1,
9,0,0,
392,0,0,0,0,0,527,378,0,0,1,1,0,1,0,1,102,0,0,
411,0,0,0,0,0,527,393,0,0,1,1,0,1,0,1,102,0,0,
430,0,0,0,0,0,590,394,0,0,1,1,0,1,0,3,102,0,0,
449,0,0,0,0,0,525,400,0,0,6,1,0,1,0,1,102,0,0,2,68,0,0,2,68,0,0,2,9,0,0,2,9,0,
0,2,9,0,0,
488,0,0,0,0,0,527,429,0,0,1,1,0,1,0,1,102,0,0,
507,0,0,0,0,0,590,430,0,0,1,1,0,1,0,3,102,0,0,
526,0,0,0,0,0,568,466,0,0,1,1,0,1,0,3,102,0,0,
545,0,0,6,111,0,1542,467,0,0,7,7,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,68,0,0,3,
68,0,0,3,102,0,0,3,3,0,0,1,9,0,0,
592,0,0,0,0,0,527,476,0,0,1,1,0,1,0,1,102,0,0,
611,0,0,0,0,0,527,491,0,0,1,1,0,1,0,1,102,0,0,
630,0,0,0,0,0,590,492,0,0,1,1,0,1,0,3,102,0,0,
649,0,0,0,0,0,525,498,0,0,10,1,0,1,0,1,102,0,0,2,68,0,0,2,68,0,0,2,68,0,0,2,68,
0,0,2,68,0,0,2,9,0,0,2,9,0,0,2,9,0,0,2,68,0,0,
704,0,0,0,0,0,527,535,0,0,1,1,0,1,0,1,102,0,0,
723,0,0,0,0,0,590,536,0,0,1,1,0,1,0,3,102,0,0,
742,0,0,0,0,0,568,583,0,0,1,1,0,1,0,3,102,0,0,
761,0,0,7,124,0,1542,584,0,0,7,7,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,68,0,0,3,
9,0,0,3,102,0,0,3,3,0,0,1,9,0,0,
808,0,0,0,0,0,527,593,0,0,1,1,0,1,0,1,102,0,0,
827,0,0,0,0,0,527,608,0,0,1,1,0,1,0,1,102,0,0,
846,0,0,0,0,0,590,609,0,0,1,1,0,1,0,3,102,0,0,
865,0,0,0,0,0,525,615,0,0,10,1,0,1,0,1,102,0,0,2,68,0,0,2,68,0,0,2,68,0,0,2,68,
0,0,2,68,0,0,2,9,0,0,2,9,0,0,2,9,0,0,2,68,0,0,
920,0,0,0,0,0,527,652,0,0,1,1,0,1,0,1,102,0,0,
939,0,0,0,0,0,590,653,0,0,1,1,0,1,0,3,102,0,0,
958,0,0,8,97,0,1542,693,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,3,0,0,1,9,
0,0,
993,0,0,0,0,0,1605,778,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1016,0,0,9,63,0,1540,780,0,0,1,0,0,0,128,2,68,0,0,1,9,0,0,
1039,0,0,10,145,0,1539,781,0,0,6,6,0,0,128,1,68,0,0,1,68,0,0,1,68,0,0,1,68,0,0,
1,3,0,0,1,3,0,0,1,9,0,0,
1082,0,0,11,0,0,1567,800,0,0,0,0,0,0,128,1,9,0,0,
1101,0,0,0,0,0,1613,801,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1124,0,0,12,0,0,1565,805,0,0,0,0,0,0,128,1,9,0,0,
1143,0,0,13,70,0,1540,807,0,0,2,1,0,0,128,2,113,0,0,1,68,0,0,1,9,0,0,
1170,0,0,14,0,0,1567,827,0,0,0,0,0,0,128,1,9,0,0,
1189,0,0,0,0,0,1613,828,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1212,0,0,0,0,0,1648,835,0,0,5,5,0,0,128,3,3,0,0,1,95,0,0,1,3,0,0,3,113,0,0,1,
68,0,0,1,9,0,0,
1251,0,0,15,0,0,1567,855,0,0,0,0,0,0,128,1,9,0,0,
1270,0,0,0,0,0,1613,856,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1293,0,0,16,52,0,1541,862,0,0,2,2,0,0,128,1,113,0,0,1,68,0,0,1,9,0,0,
1320,0,0,17,0,0,1567,881,0,0,0,0,0,0,128,1,9,0,0,
1339,0,0,0,0,0,1613,882,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1362,0,0,18,0,0,1565,886,0,0,0,0,0,0,128,1,9,0,0,
1381,0,0,0,0,0,1613,887,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1404,0,0,19,0,0,1565,888,0,0,0,0,0,0,128,1,9,0,0,
1423,0,0,0,0,0,1605,959,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1446,0,0,20,71,0,1540,962,0,0,2,1,0,0,128,2,68,0,0,1,68,0,0,1,9,0,0,
1473,0,0,0,0,0,1613,981,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1496,0,0,21,106,0,1538,990,0,0,1,1,0,0,128,1,68,0,0,1,9,0,0,
1519,0,0,0,0,0,1613,1008,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1542,0,0,22,0,0,1567,1009,0,0,0,0,0,0,128,1,9,0,0,
1561,0,0,23,0,0,1565,1012,0,0,0,0,0,0,128,1,9,0,0,
1580,0,0,24,59,0,1540,1015,0,0,2,1,0,0,128,2,9,0,0,1,68,0,0,1,9,0,0,
1607,0,0,25,59,0,1540,1016,0,0,2,1,0,0,128,2,9,0,0,1,68,0,0,1,9,0,0,
1634,0,0,26,64,0,1540,1017,0,0,1,0,0,0,128,2,68,0,0,1,9,0,0,
1657,0,0,27,195,0,1539,1019,0,0,10,10,0,0,128,1,68,0,0,1,68,0,0,1,68,0,0,1,68,
0,0,1,3,0,0,1,3,0,0,1,68,0,0,1,9,0,0,1,9,0,0,1,9,0,0,1,9,0,0,
1716,0,0,0,0,0,1613,1040,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1739,0,0,28,0,0,1567,1041,0,0,0,0,0,0,128,1,9,0,0,
1758,0,0,29,0,0,1565,1044,0,0,0,0,0,0,128,1,9,0,0,
1777,0,0,30,71,0,1540,1046,0,0,2,1,0,0,128,2,113,0,0,1,68,0,0,1,9,0,0,
1804,0,0,0,0,0,1613,1065,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1827,0,0,31,0,0,1567,1066,0,0,0,0,0,0,128,1,9,0,0,
1846,0,0,32,70,0,1540,1072,0,0,2,1,0,0,128,2,68,0,0,1,68,0,0,1,9,0,0,
1873,0,0,0,0,0,1613,1091,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1896,0,0,33,104,0,1538,1096,0,0,1,1,0,0,128,1,68,0,0,1,9,0,0,
1919,0,0,0,0,0,1613,1114,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
1942,0,0,34,0,0,1567,1115,0,0,0,0,0,0,128,1,9,0,0,
1961,0,0,35,0,0,1565,1118,0,0,0,0,0,0,128,1,9,0,0,
1980,0,0,36,63,0,1540,1121,0,0,1,0,0,0,128,2,68,0,0,1,9,0,0,
2003,0,0,37,148,0,1539,1123,0,0,8,8,0,0,128,1,68,0,0,1,68,0,0,1,68,0,0,1,68,0,
0,1,3,0,0,1,68,0,0,1,3,0,0,1,68,0,0,1,9,0,0,
2054,0,0,0,0,0,1613,1142,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
2077,0,0,38,0,0,1567,1143,0,0,0,0,0,0,128,1,9,0,0,
2096,0,0,39,0,0,1565,1146,0,0,0,0,0,0,128,1,9,0,0,
2115,0,0,40,70,0,1540,1148,0,0,2,1,0,0,128,2,113,0,0,1,68,0,0,1,9,0,0,
2142,0,0,0,0,0,1613,1167,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
2165,0,0,41,0,0,1567,1168,0,0,0,0,0,0,128,1,9,0,0,
2184,0,0,0,0,0,1648,1176,0,0,5,5,0,0,128,3,68,0,0,1,95,0,0,1,68,0,0,3,113,0,0,
1,68,0,0,1,9,0,0,
2223,0,0,0,0,0,1613,1195,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
2246,0,0,42,0,0,1567,1196,0,0,0,0,0,0,128,1,9,0,0,
2265,0,0,43,53,0,1541,1204,0,0,2,2,0,0,128,1,113,0,0,1,68,0,0,1,9,0,0,
2292,0,0,0,0,0,1613,1223,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
2315,0,0,44,0,0,1567,1224,0,0,0,0,0,0,128,1,9,0,0,
2334,0,0,45,0,0,1565,1227,0,0,0,0,0,0,128,1,9,0,0,
2353,0,0,46,52,0,1541,1231,0,0,2,2,0,0,128,1,113,0,0,1,68,0,0,1,9,0,0,
2380,0,0,0,0,0,1613,1250,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
2403,0,0,47,0,0,1567,1251,0,0,0,0,0,0,128,1,9,0,0,
2422,0,0,48,0,0,1565,1256,0,0,0,0,0,0,128,1,9,0,0,
2441,0,0,0,0,0,1613,1257,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
2464,0,0,49,0,0,1565,1258,0,0,0,0,0,0,128,1,9,0,0,
2483,0,0,50,72,0,1542,1294,0,0,2,2,0,0,128,3,68,0,0,3,3,0,0,1,9,0,0,
2510,0,0,51,90,0,1542,1346,0,0,3,3,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,1,9,0,0,
2541,0,0,0,0,0,1605,1428,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
2564,0,0,52,72,0,1540,1429,0,0,2,1,0,0,128,2,68,0,0,1,68,0,0,1,9,0,0,
2591,0,0,0,0,0,1613,1448,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
2614,0,0,53,70,0,1540,1455,0,0,2,1,0,0,128,2,68,0,0,1,68,0,0,1,9,0,0,
2641,0,0,0,0,0,1613,1460,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
2664,0,0,54,70,0,1540,1466,0,0,2,1,0,0,128,2,68,0,0,1,68,0,0,1,9,0,0,
2691,0,0,0,0,0,1613,1471,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
2714,0,0,55,150,0,1539,1479,0,0,8,8,0,0,128,1,68,0,0,1,68,0,0,1,68,0,0,1,68,0,
0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,9,0,0,
2765,0,0,56,0,0,1567,1498,0,0,0,0,0,0,128,1,9,0,0,
2784,0,0,0,0,0,1613,1499,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
2807,0,0,57,0,0,1565,1502,0,0,0,0,0,0,128,1,9,0,0,
2826,0,0,58,161,0,1541,1506,0,0,9,9,0,0,128,1,68,0,0,1,68,0,0,1,68,0,0,1,3,0,0,
1,3,0,0,1,3,0,0,1,3,0,0,1,68,0,0,1,68,0,0,1,9,0,0,
2881,0,0,59,0,0,1567,1526,0,0,0,0,0,0,128,1,9,0,0,
2900,0,0,0,0,0,1613,1527,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
2923,0,0,60,0,0,1565,1530,0,0,0,0,0,0,128,1,9,0,0,
2942,0,0,61,91,0,1540,1534,0,0,3,2,0,0,128,2,113,0,0,1,68,0,0,1,68,0,0,1,9,0,0,
2973,0,0,62,0,0,1567,1553,0,0,0,0,0,0,128,1,9,0,0,
2992,0,0,0,0,0,1613,1554,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
3015,0,0,63,0,0,1567,1557,0,0,0,0,0,0,128,1,9,0,0,
3034,0,0,64,143,0,1538,1558,0,0,0,0,0,0,128,1,9,0,0,
3053,0,0,65,91,0,1540,1560,0,0,3,2,0,0,128,2,113,0,0,1,68,0,0,1,68,0,0,1,9,0,0,
3084,0,0,0,0,0,1648,1569,0,0,5,5,0,0,128,3,3,0,0,1,95,0,0,1,3,0,0,3,113,0,0,1,
68,0,0,1,9,0,0,
3123,0,0,66,0,0,1567,1588,0,0,0,0,0,0,128,1,9,0,0,
3142,0,0,0,0,0,1613,1589,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
3165,0,0,67,73,0,1541,1596,0,0,3,3,0,0,128,1,113,0,0,1,68,0,0,1,68,0,0,1,9,0,0,
3196,0,0,68,0,0,1567,1615,0,0,0,0,0,0,128,1,9,0,0,
3215,0,0,0,0,0,1613,1616,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
3238,0,0,69,0,0,1565,1620,0,0,0,0,0,0,128,1,9,0,0,
3257,0,0,0,0,0,1613,1621,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
3280,0,0,70,0,0,1565,1622,0,0,0,0,0,0,128,1,9,0,0,
3299,0,0,71,83,0,1542,1649,0,0,3,3,0,0,128,3,68,0,0,3,4,0,0,3,3,0,0,1,9,0,0,
3330,0,0,72,97,0,1542,1724,0,0,5,5,0,0,128,3,68,0,0,3,3,0,0,3,9,0,0,3,3,0,0,3,
3,0,0,1,9,0,0,
3369,0,0,73,92,0,1542,1791,0,0,4,4,0,0,128,3,68,0,0,3,9,0,0,3,9,0,0,3,3,0,0,1,
9,0,0,
3404,0,0,74,108,0,1542,1860,0,0,5,5,0,0,128,3,68,0,0,3,9,0,0,3,9,0,0,3,9,0,0,3,
3,0,0,1,9,0,0,
3443,0,0,75,105,0,1542,1924,0,0,5,5,0,0,128,3,68,0,0,3,9,0,0,3,9,0,0,3,9,0,0,3,
3,0,0,1,9,0,0,
3482,0,0,76,97,0,1542,1996,0,0,5,5,0,0,128,3,68,0,0,3,9,0,0,3,9,0,0,3,68,0,0,3,
3,0,0,1,9,0,0,
3521,0,0,0,0,0,568,2064,0,0,1,1,0,1,0,3,102,0,0,
3540,0,0,77,145,0,1542,2065,0,0,9,9,0,0,128,3,68,0,0,3,9,0,0,3,9,0,0,3,68,0,0,
3,68,0,0,3,68,0,0,3,68,0,0,3,102,0,0,3,3,0,0,1,9,0,0,
3595,0,0,0,0,0,527,2074,0,0,1,1,0,1,0,1,102,0,0,
3614,0,0,0,0,0,527,2090,0,0,1,1,0,1,0,1,102,0,0,
3633,0,0,0,0,0,590,2091,0,0,1,1,0,1,0,3,102,0,0,
3652,0,0,0,0,0,525,2097,0,0,5,1,0,1,0,1,102,0,0,2,9,0,0,2,9,0,0,2,9,0,0,2,9,0,
0,
3687,0,0,0,0,0,527,2126,0,0,1,1,0,1,0,1,102,0,0,
3706,0,0,0,0,0,590,2127,0,0,1,1,0,1,0,3,102,0,0,
3725,0,0,78,91,0,1542,2159,0,0,4,4,0,0,128,3,68,0,0,3,3,0,0,3,9,0,0,3,3,0,0,1,
9,0,0,
3760,0,0,79,74,0,1542,2214,0,0,3,3,0,0,128,3,68,0,0,3,3,0,0,3,3,0,0,1,9,0,0,
3791,0,0,80,108,0,1542,2280,0,0,5,5,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,3,3,0,0,
3,3,0,0,1,9,0,0,
3830,0,0,81,70,0,1542,2333,0,0,2,2,0,0,128,3,68,0,0,3,3,0,0,1,9,0,0,
3857,0,0,82,73,0,1542,2384,0,0,2,2,0,0,128,3,3,0,0,3,3,0,0,1,9,0,0,
3884,0,0,83,97,0,1542,2444,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,3,3,0,0,1,
9,0,0,
3919,0,0,84,83,0,1542,2512,0,0,3,3,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,1,9,0,0,
3950,0,0,85,87,0,1542,2564,0,0,3,3,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,1,9,0,0,
3981,0,0,86,0,0,1565,2589,0,0,0,0,0,0,128,1,9,0,0,
4000,0,0,87,89,0,1542,2619,0,0,3,3,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,1,9,0,0,
4031,0,0,88,0,0,1565,2643,0,0,0,0,0,0,128,1,9,0,0,
4050,0,0,89,98,0,1540,2685,0,0,4,1,0,0,128,2,68,0,0,2,3,0,0,2,4,0,0,1,68,0,0,1,
9,0,0,
4085,0,0,90,156,0,1539,2713,0,0,6,6,0,0,128,1,68,0,0,1,68,0,0,1,68,0,0,1,4,0,0,
1,3,0,0,1,4,0,0,1,9,0,0,
4128,0,0,91,0,0,1565,2736,0,0,0,0,0,0,128,1,9,0,0,
4147,0,0,92,129,0,1539,2741,0,0,5,5,0,0,128,1,68,0,0,1,68,0,0,1,68,0,0,1,4,0,0,
1,4,0,0,1,9,0,0,
4186,0,0,93,0,0,1565,2749,0,0,0,0,0,0,128,1,9,0,0,
4205,0,0,94,209,0,1541,2774,0,0,6,6,0,0,128,1,68,0,0,1,68,0,0,1,4,0,0,1,3,0,0,
1,4,0,0,1,68,0,0,1,9,0,0,
4248,0,0,95,0,0,1565,2783,0,0,0,0,0,0,128,1,9,0,0,
4267,0,0,96,181,0,1541,2788,0,0,5,5,0,0,128,1,68,0,0,1,68,0,0,1,4,0,0,1,4,0,0,
1,68,0,0,1,9,0,0,
4306,0,0,97,0,0,1565,2796,0,0,0,0,0,0,128,1,9,0,0,
4325,0,0,98,169,0,1541,2801,0,0,5,5,0,0,128,1,68,0,0,1,68,0,0,1,4,0,0,1,4,0,0,
1,68,0,0,1,9,0,0,
4364,0,0,99,0,0,1565,2809,0,0,0,0,0,0,128,1,9,0,0,
4383,0,0,100,193,0,1541,2820,0,0,6,6,0,0,128,1,68,0,0,1,68,0,0,1,4,0,0,1,3,0,0,
1,4,0,0,1,68,0,0,1,9,0,0,
4426,0,0,101,0,0,1565,2829,0,0,0,0,0,0,128,1,9,0,0,
4445,0,0,102,165,0,1541,2834,0,0,5,5,0,0,128,1,68,0,0,1,68,0,0,1,4,0,0,1,4,0,0,
1,68,0,0,1,9,0,0,
4484,0,0,103,0,0,1565,2842,0,0,0,0,0,0,128,1,9,0,0,
4503,0,0,104,68,0,1541,2862,0,0,2,2,0,0,128,1,3,0,0,1,68,0,0,1,9,0,0,
4530,0,0,105,0,0,1565,2869,0,0,0,0,0,0,128,1,9,0,0,
4549,0,0,106,68,0,1541,2875,0,0,2,2,0,0,128,1,3,0,0,1,68,0,0,1,9,0,0,
4576,0,0,107,0,0,1565,2882,0,0,0,0,0,0,128,1,9,0,0,
4595,0,0,108,68,0,1541,2896,0,0,2,2,0,0,128,1,3,0,0,1,68,0,0,1,9,0,0,
4622,0,0,109,0,0,1565,2903,0,0,0,0,0,0,128,1,9,0,0,
4641,0,0,110,68,0,1541,2917,0,0,2,2,0,0,128,1,3,0,0,1,68,0,0,1,9,0,0,
4668,0,0,111,0,0,1565,2924,0,0,0,0,0,0,128,1,9,0,0,
4687,0,0,112,193,0,1541,2931,0,0,6,6,0,0,128,1,68,0,0,1,68,0,0,1,4,0,0,1,3,0,0,
1,4,0,0,1,68,0,0,1,9,0,0,
4730,0,0,113,0,0,1565,2940,0,0,0,0,0,0,128,1,9,0,0,
4749,0,0,114,165,0,1541,2945,0,0,5,5,0,0,128,1,68,0,0,1,68,0,0,1,4,0,0,1,4,0,0,
1,68,0,0,1,9,0,0,
4788,0,0,115,0,0,1565,2953,0,0,0,0,0,0,128,1,9,0,0,
4807,0,0,116,99,0,1542,2983,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,3,0,0,
1,9,0,0,
4842,0,0,117,94,0,1542,3032,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,3,0,0,
1,9,0,0,
4877,0,0,118,71,0,1542,3084,0,0,2,2,0,0,128,3,68,0,0,3,3,0,0,1,9,0,0,
4904,0,0,119,88,0,1540,3140,0,0,3,2,0,0,128,2,68,0,0,1,68,0,0,1,68,0,0,1,9,0,0,
4935,0,0,120,139,0,1545,3170,0,0,2,2,0,0,128,1,68,0,0,1,68,0,0,1,9,0,0,
4962,0,0,120,0,0,1551,3174,0,0,0,0,0,0,128,1,9,0,0,
4981,0,0,120,0,0,1551,3190,0,0,0,0,0,0,128,1,9,0,0,
5000,0,0,0,0,0,1605,3194,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
5023,0,0,120,0,0,1551,3198,0,0,0,0,0,0,128,1,9,0,0,
5042,0,0,0,0,0,1613,3213,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
5065,0,0,120,0,0,1551,3214,0,0,0,0,0,0,128,1,9,0,0,
5084,0,0,120,0,0,1549,3226,0,0,6,0,0,0,128,2,68,0,0,2,68,0,0,2,68,0,0,2,3,0,0,
2,68,0,0,2,113,0,0,1,9,0,0,
5127,0,0,120,0,0,1551,3230,0,0,0,0,0,0,128,1,9,0,0,
5146,0,0,0,0,0,1613,3247,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
5169,0,0,120,0,0,1551,3248,0,0,0,0,0,0,128,1,9,0,0,
5188,0,0,0,0,0,1646,3263,0,0,5,5,0,0,128,3,68,0,0,1,113,0,0,1,68,0,0,3,95,0,0,
1,68,0,0,1,9,0,0,
5227,0,0,0,0,0,1613,3273,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
5250,0,0,120,0,0,1551,3274,0,0,0,0,0,0,128,1,9,0,0,
5269,0,0,121,71,0,1540,3308,0,0,2,1,0,0,128,2,68,0,0,1,68,0,0,1,9,0,0,
5296,0,0,122,98,0,1545,3337,0,0,1,1,0,0,128,1,68,0,0,1,9,0,0,
5319,0,0,122,0,0,1551,3341,0,0,0,0,0,0,128,1,9,0,0,
5338,0,0,122,0,0,1551,3356,0,0,0,0,0,0,128,1,9,0,0,
5357,0,0,0,0,0,1605,3360,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
5380,0,0,122,0,0,1551,3364,0,0,0,0,0,0,128,1,9,0,0,
5399,0,0,0,0,0,1613,3380,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
5422,0,0,122,0,0,1551,3381,0,0,0,0,0,0,128,1,9,0,0,
5441,0,0,122,0,0,1549,3392,0,0,5,0,0,0,128,2,68,0,0,2,68,0,0,2,3,0,0,2,68,0,0,
2,113,0,0,1,9,0,0,
5480,0,0,122,0,0,1551,3396,0,0,0,0,0,0,128,1,9,0,0,
5499,0,0,0,0,0,1613,3412,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
5522,0,0,122,0,0,1551,3414,0,0,0,0,0,0,128,1,9,0,0,
5541,0,0,123,50,0,1538,3417,0,0,1,1,0,0,128,1,68,0,0,1,9,0,0,
5564,0,0,124,0,0,1565,3418,0,0,0,0,0,0,128,1,9,0,0,
5583,0,0,0,0,0,1646,3431,0,0,5,5,0,0,128,3,68,0,0,1,113,0,0,1,68,0,0,3,95,0,0,
1,68,0,0,1,9,0,0,
5622,0,0,125,50,0,1538,3439,0,0,1,1,0,0,128,1,68,0,0,1,9,0,0,
5645,0,0,0,0,0,1613,3442,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
5668,0,0,122,0,0,1551,3443,0,0,0,0,0,0,128,1,9,0,0,
5687,0,0,126,72,0,1540,3477,0,0,2,1,0,0,128,2,68,0,0,1,68,0,0,1,9,0,0,
5714,0,0,127,255,0,1545,3507,0,0,1,1,0,0,128,1,68,0,0,1,9,0,0,
5737,0,0,122,0,0,1551,3511,0,0,0,0,0,0,128,1,9,0,0,
5756,0,0,122,0,0,1551,3526,0,0,0,0,0,0,128,1,9,0,0,
5775,0,0,0,0,0,1605,3530,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
5798,0,0,122,0,0,1551,3534,0,0,0,0,0,0,128,1,9,0,0,
5817,0,0,0,0,0,1613,3549,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
5840,0,0,122,0,0,1551,3550,0,0,0,0,0,0,128,1,9,0,0,
5859,0,0,127,0,0,1549,3563,0,0,5,0,0,0,128,2,68,0,0,2,68,0,0,2,3,0,0,2,68,0,0,
2,113,0,0,1,9,0,0,
5898,0,0,127,0,0,1551,3567,0,0,0,0,0,0,128,1,9,0,0,
5917,0,0,0,0,0,1613,3583,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
5940,0,0,127,0,0,1551,3584,0,0,0,0,0,0,128,1,9,0,0,
5959,0,0,0,0,0,1646,3597,0,0,5,5,0,0,128,3,68,0,0,1,113,0,0,1,68,0,0,3,95,0,0,
1,68,0,0,1,9,0,0,
5998,0,0,0,0,0,1613,3606,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
6021,0,0,127,0,0,1551,3607,0,0,0,0,0,0,128,1,9,0,0,
6040,0,0,128,71,0,1540,3642,0,0,2,1,0,0,128,2,68,0,0,1,68,0,0,1,9,0,0,
6067,0,0,129,98,0,1545,3670,0,0,1,1,0,0,128,1,68,0,0,1,9,0,0,
6090,0,0,122,0,0,1551,3674,0,0,0,0,0,0,128,1,9,0,0,
6109,0,0,122,0,0,1551,3689,0,0,0,0,0,0,128,1,9,0,0,
6128,0,0,0,0,0,1605,3693,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
6151,0,0,122,0,0,1551,3697,0,0,0,0,0,0,128,1,9,0,0,
6170,0,0,0,0,0,1613,3713,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
6193,0,0,122,0,0,1551,3714,0,0,0,0,0,0,128,1,9,0,0,
6212,0,0,129,0,0,1549,3726,0,0,5,0,0,0,128,2,68,0,0,2,68,0,0,2,3,0,0,2,68,0,0,
2,113,0,0,1,9,0,0,
6251,0,0,129,0,0,1551,3731,0,0,0,0,0,0,128,1,9,0,0,
6270,0,0,0,0,0,1613,3747,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
6293,0,0,129,0,0,1551,3748,0,0,0,0,0,0,128,1,9,0,0,
6312,0,0,0,0,0,1646,3761,0,0,5,5,0,0,128,3,68,0,0,1,113,0,0,1,68,0,0,3,95,0,0,
1,68,0,0,1,9,0,0,
6351,0,0,0,0,0,1613,3769,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
6374,0,0,129,0,0,1551,3770,0,0,0,0,0,0,128,1,9,0,0,
6393,0,0,130,56,0,1540,3806,0,0,2,1,0,0,128,2,68,0,0,1,68,0,0,1,9,0,0,
6420,0,0,131,90,0,1539,3834,0,0,4,4,0,0,128,1,68,0,0,1,68,0,0,1,68,0,0,1,68,0,
0,1,9,0,0,
6455,0,0,132,0,0,1565,3855,0,0,0,0,0,0,128,1,9,0,0,
6474,0,0,133,147,0,1545,3861,0,0,1,1,0,0,128,1,68,0,0,1,9,0,0,
6497,0,0,133,0,0,1549,3865,0,0,3,0,0,0,128,2,68,0,0,2,9,0,0,2,9,0,0,1,9,0,0,
6528,0,0,133,0,0,1551,3869,0,0,0,0,0,0,128,1,9,0,0,
6547,0,0,134,50,0,1541,3872,0,0,2,2,0,0,128,1,68,0,0,1,68,0,0,1,9,0,0,
6574,0,0,135,0,0,1565,3873,0,0,0,0,0,0,128,1,9,0,0,
6593,0,0,133,0,0,1551,3881,0,0,0,0,0,0,128,1,9,0,0,
6612,0,0,133,0,0,1551,3903,0,0,0,0,0,0,128,1,9,0,0,
6631,0,0,136,71,0,1542,3928,0,0,2,2,0,0,128,3,68,0,0,3,3,0,0,1,9,0,0,
6658,0,0,0,0,0,1605,4009,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
6681,0,0,0,0,0,1648,4015,0,0,5,5,0,0,128,3,3,0,0,1,95,0,0,1,3,0,0,3,113,0,0,1,
68,0,0,1,9,0,0,
6720,0,0,137,0,0,1567,4034,0,0,0,0,0,0,128,1,9,0,0,
6739,0,0,0,0,0,1613,4035,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
6762,0,0,138,145,0,1542,4042,0,0,8,8,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,68,0,
0,3,3,0,0,3,3,0,0,3,113,0,0,3,3,0,0,1,9,0,0,
6813,0,0,0,0,0,1613,4048,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
6836,0,0,139,0,0,1565,4049,0,0,0,0,0,0,128,1,9,0,0,
6855,0,0,140,87,0,1542,4077,0,0,4,4,0,0,128,3,68,0,0,3,3,0,0,3,68,0,0,3,3,0,0,
1,9,0,0,
6890,0,0,141,154,0,1542,4123,0,0,8,8,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,68,0,
0,3,68,0,0,3,68,0,0,3,68,0,0,3,3,0,0,1,9,0,0,
6941,0,0,0,0,0,1605,4188,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
6964,0,0,142,417,0,1545,4200,0,0,2,2,0,0,128,1,3,0,0,1,68,0,0,1,9,0,0,
6991,0,0,142,0,0,1551,4205,0,0,0,0,0,0,128,1,9,0,0,
7010,0,0,142,0,0,1551,4220,0,0,0,0,0,0,128,1,9,0,0,
7029,0,0,142,0,0,1551,4227,0,0,0,0,0,0,128,1,9,0,0,
7048,0,0,0,0,0,1613,4242,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
7071,0,0,142,0,0,1551,4243,0,0,0,0,0,0,128,1,9,0,0,
7090,0,0,142,0,0,1549,4259,0,0,9,0,0,0,128,2,68,0,0,2,68,0,0,2,68,0,0,2,68,0,0,
2,3,0,0,2,68,0,0,2,113,0,0,2,9,0,0,2,9,0,0,1,9,0,0,
7145,0,0,0,0,0,1613,4267,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
7168,0,0,142,0,0,1551,4268,0,0,0,0,0,0,128,1,9,0,0,
7187,0,0,143,0,0,1565,4270,0,0,0,0,0,0,128,1,9,0,0,
7206,0,0,0,0,0,1646,4286,0,0,5,5,0,0,128,3,68,0,0,1,113,0,0,1,68,0,0,3,95,0,0,
1,68,0,0,1,9,0,0,
7245,0,0,144,306,0,1539,4294,0,0,1,1,0,0,128,1,68,0,0,1,9,0,0,
7268,0,0,145,47,0,1538,4302,0,0,1,1,0,0,128,1,68,0,0,1,9,0,0,
7291,0,0,146,51,0,1541,4306,0,0,1,1,0,0,128,1,68,0,0,1,9,0,0,
7314,0,0,0,0,0,1613,4310,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
7337,0,0,142,0,0,1551,4311,0,0,0,0,0,0,128,1,9,0,0,
7356,0,0,0,0,0,1613,4317,0,0,1,1,0,0,128,3,113,0,0,1,9,0,0,
7379,0,0,142,0,0,1551,4318,0,0,0,0,0,0,128,1,9,0,0,
7398,0,0,147,0,0,1565,4320,0,0,0,0,0,0,128,1,9,0,0,
};


#include "stdafx.h"
#define SQLCA_NONE 
#define ORACA_NONE
#include <sqlcpr.h>
// extern struct sqlca sqlca;
// extern struct oraca oraca;

/*#include <oci.h>*/
/*使用到的连接名与上下文
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);
	EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid];
*/

#include "DatabaseMini.h"
#include <oci.h>

/* EXEC SQL INCLUDE SQLCA;
 */ 
/*
 * $Header: sqlca.h 24-apr-2003.12:50:58 mkandarp Exp $ sqlca.h 
 */

/* Copyright (c) 1985, 2003, Oracle Corporation.  All rights reserved.  */
 
/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:
 
    #define SQLCA_STORAGE_CLASS extern
 
  will define the SQLCA as an extern.
 
  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.
 
MODIFIED
    lvbcheng   07/31/98 -  long to int
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   08/11/92 -  No sqlca var if SQLCA_NONE macro set 
  Clare      12/06/84 - Ch SQLCA to not be an extern.
  Clare      10/21/85 - Add initialization.
  Bradbury   01/05/86 - Only initialize when SQLCA_INIT set
  Clare      06/12/86 - Add SQLCA_STORAGE_CLASS option.
*/
 
#ifndef SQLCA
#define SQLCA 1
 
struct   sqlca
         {
         /* ub1 */ char    sqlcaid[8];
         /* b4  */ int     sqlabc;
         /* b4  */ int     sqlcode;
         struct
           {
           /* ub2 */ unsigned short sqlerrml;
           /* ub1 */ char           sqlerrmc[70];
           } sqlerrm;
         /* ub1 */ char    sqlerrp[8];
         /* b4  */ int     sqlerrd[6];
         /* ub1 */ char    sqlwarn[8];
         /* ub1 */ char    sqlext[8];
         };

#ifndef SQLCA_NONE 
#ifdef   SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
         struct sqlca sqlca
#endif
 
#ifdef  SQLCA_INIT
         = {
         {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
         sizeof(struct sqlca),
         0,
         { 0, {0}},
         {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
         }
#endif
         ;
#endif
 
#endif
 
/* end SQLCA */
/* EXEC SQL INCLUDE ORACA;
 */ 
/*
 * $Header: oraca.h 24-apr-2003.12:50:59 mkandarp Exp $ oraca.h 
 */

/* Copyright (c) 1985, 2003, Oracle Corporation.  All rights reserved.  */
 
/*
NAME
  ORACA : Oracle Communications Area.
FUNCTION
  Contains no code. Provides supplementary communications to/from
  Oracle (in addition to standard SQLCA).
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  oracchf : Check cursor cache consistency flag. If set AND oradbgf
            is set, then directs SQLLIB to perform cursor cache
            consistency checks before every cursor operation
            (OPEN, FETCH, SELECT, INSERT, etc.).
  oradbgf : Master DEBUG flag. Used to turn all DEBUG options
            on or off.
  orahchf : Check Heap consistency flag. If set AND oradbgf is set,
            then directs SQLLIB to perform heap consistency checks
            everytime memory is dynamically allocated/free'd via
            sqlalc/sqlfre/sqlrlc. MUST BE SET BEFORE 1ST CONNECT
            and once set cannot be cleared (subsequent requests
            to change it are ignored).
  orastxtf: Save SQL stmt text flag. If set, then directs SQLLIB
            to save the text of the current SQL stmt in orastxt
            (in VARCHAR format).
  orastxt : Saved len and text of current SQL stmt (in VARCHAR
            format).
  orasfnm : Saved len and text of filename containing current SQL
            stmt (in VARCHAR format).
  oraslnr : Saved line nr within orasfnm of current SQL stmt.
 
  Cursor cache statistics. Set after COMMIT or ROLLBACK. Each
  CONNECT'd DATABASE has its own set of statistics.
 
  orahoc  : Highest Max Open OraCursors requested. Highest value
            for MAXOPENCURSORS by any CONNECT to this DATABASE.
  oramoc  : Max Open OraCursors required. Specifies the max nr
            of OraCursors required to run this pgm. Can be higher
            than orahoc if working set (MAXOPENCURSORS) was set
            too low, thus forcing the PCC to expand the cache.
  oracoc  : Current nr of OraCursors used.
  oranor  : Nr of OraCursor cache reassignments. Can show the
            degree of "thrashing" in the cache. Optimally, this
            nr should be kept as low as possible (time vs space
            optimization).
  oranpr  : Nr of SQL stmt "parses".
  oranex  : Nr of SQL stmt "executes". Optimally, the relation-
            ship of oranex to oranpr should be kept as high as
            possible.
 
 
  If the symbol ORACA_NONE is defined, then there will be no ORACA
  *variable*, although there will still be a struct defined.  This
  macro should not normally be defined in application code.

  If the symbol ORACA_INIT is defined, then the ORACA will be
  statically initialized. Although this is not necessary in order
  to use the ORACA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the ORACA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then ORACA_INIT should be left undefined --
  all others can define ORACA_INIT if they wish.
 
OWNER
  Clare
DATE
  10/19/85
MODIFIED
    apopat     05/08/02  - [2362423] MVS PE to make lines shorter than 79
    apopat     07/31/99 -  [707588] TAB to blanks for OCCS
    lvbcheng   10/27/98 -  change long to int for oraca
    pccint     10/03/96 -  Add IS_OSD for linting
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   09/04/92 -  Make oraca variable optional 
    Osborne    05/24/90 - Add ORACA_STORAGE_CLASS construct
  Clare      02/20/86 - PCC [10101l] Feature: Heap consistency check.
  Clare      03/04/86 - PCC [10101r] Port: ORACA init ifdef.
  Clare      03/12/86 - PCC [10101ab] Feature: ORACA cuc statistics.
*/
/* IS_OSD */ 
#ifndef  ORACA
#define  ORACA     1
 
struct   oraca
         {
    /* text */ char oracaid[8];      /* Reserved                            */
    /* ub4  */ int oracabc;          /* Reserved                            */
 
    /*       Flags which are setable by User. */
 
   /* ub4 */ int  oracchf;           /* <> 0 if "check cur cache consistncy"*/
   /* ub4 */ int  oradbgf;           /* <> 0 if "do DEBUG mode checking"    */
   /* ub4 */ int  orahchf;           /* <> 0 if "do Heap consistency check" */
   /* ub4 */ int  orastxtf;          /* SQL stmt text flag                  */
#define  ORASTFNON 0                 /* = don't save text of SQL stmt       */
#define  ORASTFERR 1                 /* = only save on SQLERROR             */
#define  ORASTFWRN 2                 /* = only save on SQLWARNING/SQLERROR  */
#define  ORASTFANY 3                 /* = always save                       */
         struct
           {
  /* ub2  */ unsigned short orastxtl;
  /* text */ char  orastxtc[70];
           } orastxt;                /* text of last SQL stmt               */
         struct
           {
  /* ub2  */   unsigned short orasfnml;
  /* text */   char       orasfnmc[70];
           } orasfnm;                /* name of file containing SQL stmt    */
  /* ub4 */ int   oraslnr;           /* line nr-within-file of SQL stmt     */

  /* ub4 */ int   orahoc;            /* highest max open OraCurs requested  */
  /* ub4 */ int   oramoc;            /* max open OraCursors required        */
  /* ub4 */ int   oracoc;            /* current OraCursors open             */
  /* ub4 */ int   oranor;            /* nr of OraCursor re-assignments      */
  /* ub4 */ int   oranpr;            /* nr of parses                        */
  /* ub4 */ int   oranex;            /* nr of executes                      */
         };

#ifndef ORACA_NONE

#ifdef ORACA_STORAGE_CLASS
ORACA_STORAGE_CLASS struct oraca oraca
#else
struct oraca oraca
#endif
#ifdef ORACA_INIT
         =
         {
         {'O','R','A','C','A',' ',' ',' '},
         sizeof(struct oraca),
         0,0,0,0,
         {0,{0}},
         {0,{0}},
         0,
         0,0,0,0,0,0
         }
#endif
         ;

#endif

#endif
/* end oraca.h */


/* EXEC ORACLE OPTION (RELEASE_CURSOR = YES); */ 
  
/* EXEC ORACLE OPTION (ORACA=YES); */ 


extern struct oraca oraca;
extern void sqlgls(char*,size_t*,size_t *);

 
typedef struct long_varraw {
  unsigned int len;
  char buf[1];
} long_varraw;
/* EXEC SQL TYPE long_varraw IS LONG VARRAW REFERENCE; */ 


//线程运行上下文
/* EXEC SQL BEGIN DECLARE SECTION; */ 

	extern sql_context m_ctx[ 200 ];
/* EXEC SQL END DECLARE SECTION; */ 

	

CDatabaseMini::CDatabaseMini(void)
{

}

CDatabaseMini::~CDatabaseMini(void)
{

}

void CDatabaseMini::SetConnectDB(char* uid, char* pwd, char* sid)
{
	strncpy(miniuid, uid,32);
	strncpy(minipwd,pwd,32);
	strncpy(minisid, sid,32);
}

bool CDatabaseMini::ConnectDB()
{
	return ConnectMini();
}

/*================================================================ 
*
* 函 数 名：Get_User_GroupInfo 
** 根据用户名分组信息
* 参 数： 
* const uint32 [[IN]] : 用户ID
* XT_GROUPINFO_GET_ACK::tagGroupInfo* pGroupInfo [IN,OUT]: 用户分组信息
* int &nGroupNum [IN,OUT] : 所要获取的分组数和返回的分组数，如果输入为0，则输出存在的数目。
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 数据库错误:DB_ERROR;
* 
================================================================*/ 
int CDatabaseMini::Get_User_GroupInfo(const uint32 c_iUserID, XT_GROUPINFO_GET_ACK::tagGroupInfo* pGroupInfo, uint8 &nGroupNum, uint8 flag)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR var_groupname[21]; */ 
struct { unsigned short len; unsigned char arr[21]; } var_groupname;

		unsigned int     i_UserId,i_flag;
		int				g_pid,g_id,ret_count;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;	
		SQL_CURSOR cur_group;
	/* EXEC SQL END DECLARE SECTION; */ 

 

	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);

	SET_INIT_NULL(var_groupname);
	i_UserId = c_iUserID ;
	i_flag = flag;
	
	ret_count = 0;
	int db_ret;
	int iReconnTimes;
	bool bReconn;	

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	/* EXEC SQL ALLOCATE :cur_group; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_group;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



start:	
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.GetGroupInfo(:i_UserId,:cur_group,:retcode,:i_flag);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 5;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . GetGroupInfo ( :i_UserId , :cur_grou\
p , :retcode , :i_flag ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )24;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&i_UserId;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&cur_group;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&i_flag;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[4] = (unsigned long )34;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		/* EXEC SQL CLOSE :cur_group; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )59;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_group;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 )
	{
		sql_error(sqlca, oraca);
		nGroupNum = 0;
		/* EXEC SQL CLOSE :cur_group; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )78;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_group;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL FREE  :cur_group; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )97;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_group;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}
	
	for(;;)
	{  
	 
		/* EXEC SQL FETCH :cur_group INTO :g_pid,:g_id,:var_groupname; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )116;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_group;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&g_pid;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&g_id;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&var_groupname;
  sqlstm.sqhstl[3] = (unsigned long )23;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (db_ret == 1403 )
		{			
			nGroupNum = ret_count; 
			break;
		}
		
		if( ret_count >= nGroupNum )//指定获取的数目
		{
			break;
		}
		
		if( db_ret < 0 )
		{
			nGroupNum = ret_count; 
			break;
		}
		SET_END_NULL(var_groupname);

		pGroupInfo[ret_count].nGroupPid = g_pid;
		pGroupInfo[ret_count].nGroupID = g_id;
		strncpy(pGroupInfo[ret_count].szGroupName, (char*)var_groupname.arr, MAX_GROUPNAME_LEN);
	
		++ret_count;
	}
	
	/* EXEC SQL CLOSE :cur_group; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 5;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )147;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_group;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL FREE  :cur_group; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 5;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )166;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_group;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	
	return  DB_SUCCESS;

}

int CDatabaseMini::WebMsgSave(XT_TALK* msg,uint32 logintime)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int from_id,to_id,data_type,version,send_time,recv_flag,fontSize,fontColor,fontStyle,recv_logintime,retcode;
		/* VARCHAR var_fontName[50],var_nickname[41],var_msg[1315],db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[50]; } var_fontName;

struct { unsigned short len; unsigned char arr[41]; } var_nickname;

struct { unsigned short len; unsigned char arr[1315]; } var_msg;

struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	from_id = msg->from_id;
	to_id = msg->to_id;
	data_type = msg->data_type;
	version = msg->ver;
	send_time = msg->send_time;
	recv_flag = msg->recv_flag;
	fontSize = msg->fontSize;
	fontColor = msg->fontColor;
	fontStyle = msg->fontStyle;
	recv_logintime =logintime;
	
	SET_INIT_NULL(var_fontName);
	SET_INIT_NULL(var_nickname);
	SET_INIT_NULL(var_msg);
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	strncpy((char*)var_nickname.arr,msg->from_nickname,MAX_NICKNAME_LEN+1);/*4*/
	var_nickname.len = (short)strlen((char *)var_nickname.arr);

	strncpy((char*)var_fontName.arr,msg->fontName,50);
	var_fontName.len = (short)strlen((char *)var_fontName.arr);
	
	strncpy((char*)var_msg.arr,msg->data,1315);
	var_msg.len = (short)strlen((char *)var_msg.arr);

	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;

	start:	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.WebIMMsgSave_20150708(:from_id,:to_id,:data_type,:version,:send_time,:recv_flag,:fontSize,:fontColor,:fontStyle,:recv_logintime,:var_fontName,:var_nickname,:var_msg,:retcode);
			end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . WebIMMsgSave_20150708 ( :from_id , :t\
o_id , :data_type , :version , :send_time , :recv_flag , :fontSize , :fontCol\
or , :fontStyle , :recv_logintime , :var_fontName , :var_nickname , :var_msg \
, :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )185;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&from_id;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&to_id;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&data_type;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&version;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&send_time;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&recv_flag;
 sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (unsigned char  *)&fontSize;
 sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         short *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned long )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (unsigned char  *)&fontColor;
 sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         short *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned long )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (unsigned char  *)&fontStyle;
 sqlstm.sqhstl[8] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         short *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned long )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqhstv[9] = (unsigned char  *)&recv_logintime;
 sqlstm.sqhstl[9] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[9] = (         int  )0;
 sqlstm.sqindv[9] = (         short *)0;
 sqlstm.sqinds[9] = (         int  )0;
 sqlstm.sqharm[9] = (unsigned long )0;
 sqlstm.sqadto[9] = (unsigned short )0;
 sqlstm.sqtdso[9] = (unsigned short )0;
 sqlstm.sqhstv[10] = (unsigned char  *)&var_fontName;
 sqlstm.sqhstl[10] = (unsigned long )52;
 sqlstm.sqhsts[10] = (         int  )0;
 sqlstm.sqindv[10] = (         short *)0;
 sqlstm.sqinds[10] = (         int  )0;
 sqlstm.sqharm[10] = (unsigned long )0;
 sqlstm.sqadto[10] = (unsigned short )0;
 sqlstm.sqtdso[10] = (unsigned short )0;
 sqlstm.sqhstv[11] = (unsigned char  *)&var_nickname;
 sqlstm.sqhstl[11] = (unsigned long )43;
 sqlstm.sqhsts[11] = (         int  )0;
 sqlstm.sqindv[11] = (         short *)0;
 sqlstm.sqinds[11] = (         int  )0;
 sqlstm.sqharm[11] = (unsigned long )0;
 sqlstm.sqadto[11] = (unsigned short )0;
 sqlstm.sqtdso[11] = (unsigned short )0;
 sqlstm.sqhstv[12] = (unsigned char  *)&var_msg;
 sqlstm.sqhstl[12] = (unsigned long )1317;
 sqlstm.sqhsts[12] = (         int  )0;
 sqlstm.sqindv[12] = (         short *)0;
 sqlstm.sqinds[12] = (         int  )0;
 sqlstm.sqharm[12] = (unsigned long )0;
 sqlstm.sqadto[12] = (unsigned short )0;
 sqlstm.sqtdso[12] = (unsigned short )0;
 sqlstm.sqhstv[13] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[13] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[13] = (         int  )0;
 sqlstm.sqindv[13] = (         short *)0;
 sqlstm.sqinds[13] = (         int  )0;
 sqlstm.sqharm[13] = (unsigned long )0;
 sqlstm.sqadto[13] = (unsigned short )0;
 sqlstm.sqtdso[13] = (unsigned short )0;
 sqlstm.sqhstv[14] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[14] = (unsigned long )34;
 sqlstm.sqhsts[14] = (         int  )0;
 sqlstm.sqindv[14] = (         short *)0;
 sqlstm.sqinds[14] = (         int  )0;
 sqlstm.sqharm[14] = (unsigned long )0;
 sqlstm.sqadto[14] = (unsigned short )0;
 sqlstm.sqtdso[14] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
		
	if (retcode == 1)
	{
		return DB_ERROR;
	}
	
	return DB_SUCCESS;	
}

int CDatabaseMini::WebRecommSave(uint32 fid,uint32 did,uint16 recomm,uint32 time)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int from_id,to_id,recomment,send_time;
		int	retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	from_id = fid;
	to_id = did;
	recomment = recomm;
	send_time = time;
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;

	start:	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.WebRecommSave(:from_id,:to_id,:recomment,:send_time,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . WebRecommSave ( :from_id , :to_id , :\
recomment , :send_time , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )260;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&from_id;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&to_id;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&recomment;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&send_time;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[5] = (unsigned long )34;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
		
	if (retcode == 1 || retcode == 1403)
	{
		return DB_ERROR;
	}
	
	return DB_SUCCESS;	
}

int CDatabaseMini::WebServiceSave(uint32 fid,uint32 did,uint16 recomm,uint32 time)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int from_id,to_id,recomment,send_time;
		int	retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	from_id = fid;
	to_id = did;
	recomment = recomm;
	send_time = time;
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;

	start:	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.WebServiceSave(:from_id,:to_id,:recomment,:send_time,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . WebServiceSave ( :from_id , :to_id , \
:recomment , :send_time , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )299;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&from_id;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&to_id;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&recomment;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&send_time;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[5] = (unsigned long )34;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
		
	if (retcode == 1)
	{
		return DB_ERROR;
	}
	
	return DB_SUCCESS;	
}

int CDatabaseMini::WebBusiListReq(XT_WEB_BUSI_LIST_REQ* BusiListReq,XT_WEB_BUSI_LIST_ACK::BusiList* pListInfo,uint32& nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR	db_link_name[32],var_nickname[41],var_headImg[101],var_usersign[250]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

struct { unsigned short len; unsigned char arr[41]; } var_nickname;

struct { unsigned short len; unsigned char arr[101]; } var_headImg;

struct { unsigned short len; unsigned char arr[250]; } var_usersign;

		unsigned int id,msgnum,list_id,c_time;
		int	retcode;
		SQL_CURSOR cur_listInfo;
	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);
	
	id=BusiListReq->id;
	msgnum=BusiListReq->msgnum;
	
	SET_INIT_NULL(var_nickname);
	SET_INIT_NULL(var_headImg);
	SET_INIT_NULL(var_usersign);
  
	int db_ret, iReconnTimes;
	uint32 iNum = 0;	
	bool bReconn;	
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
  

start:  
		/* EXEC SQL ALLOCATE :cur_listInfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )338;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_listInfo;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name EXECUTE
			begin
				Mini_User_Pkg.WebListReq_20151020(:id,:msgnum,:cur_listInfo,:retcode);
			end;
		END-EXEC; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "begin Mini_User_Pkg . WebListReq_20151020 ( :id , :msgnum \
, :cur_listInfo , :retcode ) ; end ;";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )357;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&id;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&msgnum;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&cur_listInfo;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&retcode;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[4] = (unsigned long )34;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			/* EXEC SQL CLOSE :cur_listInfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )392;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_listInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if(db_ret < 0 && db_ret != -1405) 
		{
			sql_error(sqlca, oraca);		
			/* EXEC SQL CLOSE :cur_listInfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )411;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_listInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL FREE  :cur_listInfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )430;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_listInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;
		}

		for(;;)
		{  
			/* EXEC SQL FETCH :cur_listInfo INTO :list_id,:c_time,:var_nickname,:var_headImg,:var_usersign; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )449;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqfoff = (         int )0;
   sqlstm.sqfmod = (unsigned int )2;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_listInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&list_id;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&c_time;
   sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&var_nickname;
   sqlstm.sqhstl[3] = (unsigned long )43;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)&var_headImg;
   sqlstm.sqhstl[4] = (unsigned long )103;
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (unsigned char  *)&var_usersign;
   sqlstm.sqhstl[5] = (unsigned long )252;
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         short *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned long )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			db_ret = sqlca.sqlcode;
			if (db_ret == 1403 )
			{
				nRetCount = iNum;
				break;
			}
			if (iNum >= nRetCount)
			{
				break;
			}	
			if( db_ret < 0 && db_ret != -1405 )
			{
				sql_error(sqlca, oraca);
				nRetCount = iNum;
				break;
			}		
			pListInfo[iNum].id = list_id;
			pListInfo[iNum].time = c_time;
			
			SET_END_NULL(var_nickname);
			SET_END_NULL(var_headImg);
			SET_END_NULL(var_usersign);
			strncpy(pListInfo[iNum].nickname,(char*)var_nickname.arr,MAX_NICKNAME_LEN+1);	
			strncpy(pListInfo[iNum].headImgUrl,(char*)var_headImg.arr,MAX_HEADURL_LEN+1);
			strncpy(pListInfo[iNum].usersign,(char*)var_usersign.arr,MAX_USERSIGN_LEN+1);

			++iNum;
		}
		/* EXEC SQL CLOSE :cur_listInfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )488;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_listInfo;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL FREE  :cur_listInfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )507;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_listInfo;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_SUCCESS;		
}

int CDatabaseMini::WebMsgReq(XT_WEB_MORE_MSG_REQ* msgReq,XT_WEB_MORE_MSG_ACK::MsgInfo* msgInfo,uint32& nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR	db_link_name[32],var_fontName[50],var_nickName[41],var_data[1315]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

struct { unsigned short len; unsigned char arr[50]; } var_fontName;

struct { unsigned short len; unsigned char arr[41]; } var_nickName;

struct { unsigned short len; unsigned char arr[1315]; } var_data;

		unsigned int sid,did,msgtime,msgnum,msgid;
		unsigned int id,send_time,fontSize,fontColor,fontStyle;
		int	retcode;
		SQL_CURSOR cur_msgInfo;
	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);
	
	sid=msgReq->sid;
	did=msgReq->did;
	msgtime=msgReq->msgtime;
	msgnum=msgReq->msgnum;
	msgid=msgReq->msgid;
	
	SET_INIT_NULL(var_fontName);
	SET_INIT_NULL(var_nickName);
	SET_INIT_NULL(var_data);

  
	int db_ret, iReconnTimes;
	uint32 iNum = 0;	
	bool bReconn;	
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
  

start:  
		/* EXEC SQL ALLOCATE :cur_msgInfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )526;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name EXECUTE
			begin
				Mini_User_Pkg.WebMsgReq(:sid,:did,:msgtime,:msgid,:msgnum,:cur_msgInfo,:retcode);
			end;
		END-EXEC; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "begin Mini_User_Pkg . WebMsgReq ( :sid , :did , :msgtime ,\
 :msgid , :msgnum , :cur_msgInfo , :retcode ) ; end ;";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )545;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&sid;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&did;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&msgtime;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&msgid;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&msgnum;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&cur_msgInfo;
  sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&retcode;
  sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         short *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned long )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[7] = (unsigned long )34;
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         short *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned long )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			/* EXEC SQL CLOSE :cur_msgInfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )592;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if(db_ret < 0 && db_ret != -1405) 
		{
			sql_error(sqlca, oraca);		
			/* EXEC SQL CLOSE :cur_msgInfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )611;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL FREE  :cur_msgInfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )630;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;
		}

		for(;;)
		{  
			/* EXEC SQL FETCH :cur_msgInfo INTO :id,:send_time,:fontSize,:fontColor,:fontStyle,:var_fontName,:var_nickName,:var_data,:msgid; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )649;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqfoff = (         int )0;
   sqlstm.sqfmod = (unsigned int )2;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&id;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&send_time;
   sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&fontSize;
   sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)&fontColor;
   sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (unsigned char  *)&fontStyle;
   sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         short *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned long )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqhstv[6] = (unsigned char  *)&var_fontName;
   sqlstm.sqhstl[6] = (unsigned long )52;
   sqlstm.sqhsts[6] = (         int  )0;
   sqlstm.sqindv[6] = (         short *)0;
   sqlstm.sqinds[6] = (         int  )0;
   sqlstm.sqharm[6] = (unsigned long )0;
   sqlstm.sqadto[6] = (unsigned short )0;
   sqlstm.sqtdso[6] = (unsigned short )0;
   sqlstm.sqhstv[7] = (unsigned char  *)&var_nickName;
   sqlstm.sqhstl[7] = (unsigned long )43;
   sqlstm.sqhsts[7] = (         int  )0;
   sqlstm.sqindv[7] = (         short *)0;
   sqlstm.sqinds[7] = (         int  )0;
   sqlstm.sqharm[7] = (unsigned long )0;
   sqlstm.sqadto[7] = (unsigned short )0;
   sqlstm.sqtdso[7] = (unsigned short )0;
   sqlstm.sqhstv[8] = (unsigned char  *)&var_data;
   sqlstm.sqhstl[8] = (unsigned long )1317;
   sqlstm.sqhsts[8] = (         int  )0;
   sqlstm.sqindv[8] = (         short *)0;
   sqlstm.sqinds[8] = (         int  )0;
   sqlstm.sqharm[8] = (unsigned long )0;
   sqlstm.sqadto[8] = (unsigned short )0;
   sqlstm.sqtdso[8] = (unsigned short )0;
   sqlstm.sqhstv[9] = (unsigned char  *)&msgid;
   sqlstm.sqhstl[9] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[9] = (         int  )0;
   sqlstm.sqindv[9] = (         short *)0;
   sqlstm.sqinds[9] = (         int  )0;
   sqlstm.sqharm[9] = (unsigned long )0;
   sqlstm.sqadto[9] = (unsigned short )0;
   sqlstm.sqtdso[9] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			db_ret = sqlca.sqlcode;
			if (db_ret == 1403 )
			{
				nRetCount = iNum;
				break;
			}
			if (iNum >= nRetCount)
			{
				break;
			}	
			if( db_ret < 0 && db_ret != -1405 )
			{
				sql_error(sqlca, oraca);
				nRetCount = iNum;
				break;
			}		
			msgInfo[iNum].id = id;
			msgInfo[iNum].send_time = send_time;
			msgInfo[iNum].fontSize = fontSize;
			msgInfo[iNum].fontColor = fontColor;
			msgInfo[iNum].fontStyle = fontStyle;
			msgInfo[iNum].msgid = msgid;
			
			SET_END_NULL(var_fontName);
			SET_END_NULL(var_nickName);
			SET_END_NULL(var_data);

			
			strncpy(msgInfo[iNum].fontName,(char*)var_fontName.arr,50);	
			strncpy(msgInfo[iNum].nickName,(char*)var_nickName.arr,MAX_NICKNAME_LEN+1);
			strncpy(msgInfo[iNum].data,(char*)var_data.arr,1315);	
	
		
			
			++iNum;
		}
		/* EXEC SQL CLOSE :cur_msgInfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )704;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL FREE  :cur_msgInfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )723;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_SUCCESS;		
}

int CDatabaseMini::WebMsgMoreReq(XT_WEB_MORE_MSG_REQ* msgReq,XT_WEB_MORE_MSG_ACK::MsgInfo* msgInfo,std::vector<uint32_t> &ids, uint32& nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR	db_link_name[32],var_fontName[50],var_nickName[41],var_data[1315]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

struct { unsigned short len; unsigned char arr[50]; } var_fontName;

struct { unsigned short len; unsigned char arr[41]; } var_nickName;

struct { unsigned short len; unsigned char arr[1315]; } var_data;

		/* VARCHAR merchant_ids[1024]; */ 
struct { unsigned short len; unsigned char arr[1024]; } merchant_ids;

		unsigned int sid,msgtime,msgnum,msgid;
		unsigned int id,send_time,fontSize,fontColor,fontStyle;
		int	retcode;
		SQL_CURSOR cur_msgInfo;
	/* EXEC SQL END DECLARE SECTION; */ 


	SET_INIT_NULL(merchant_ids);
	for(unsigned int i = 0; i < ids.size();i++)
	{
		char buffer[64] = {0};
		strcat((char *)merchant_ids.arr,itoa(ids[i],buffer,10));
		strcat((char *)merchant_ids.arr,",");
	}	
	merchant_ids.arr[(short)strlen((char*)merchant_ids.arr)-1] = '\0';
	merchant_ids.len = (short)strlen((char*)merchant_ids.arr);	
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);
	
	sid=msgReq->sid;
//	did=msgReq->did;
	msgtime=msgReq->msgtime;
	msgnum=msgReq->msgnum;
	msgid=msgReq->msgid;
	
	SET_INIT_NULL(var_fontName);
	SET_INIT_NULL(var_nickName);
	SET_INIT_NULL(var_data);

  
	int db_ret, iReconnTimes;
	uint32 iNum = 0;	
	bool bReconn;	
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
  

start:  
		/* EXEC SQL ALLOCATE :cur_msgInfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )742;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name EXECUTE
			begin
				Mini_User_Pkg.WebMsgMoreReq(:sid,:msgtime,:msgid,:msgnum,:merchant_ids,:cur_msgInfo,:retcode);
			end;
		END-EXEC; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "begin Mini_User_Pkg . WebMsgMoreReq ( :sid , :msgtime , :m\
sgid , :msgnum , :merchant_ids , :cur_msgInfo , :retcode ) ; end ;";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )761;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&sid;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&msgtime;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&msgid;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&msgnum;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&merchant_ids;
  sqlstm.sqhstl[4] = (unsigned long )1026;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&cur_msgInfo;
  sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&retcode;
  sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         short *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned long )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[7] = (unsigned long )34;
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         short *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned long )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			/* EXEC SQL CLOSE :cur_msgInfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )808;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if(db_ret < 0 && db_ret != -1405) 
		{
			sql_error(sqlca, oraca);		
			/* EXEC SQL CLOSE :cur_msgInfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )827;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL FREE  :cur_msgInfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )846;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;
		}

		for(;;)
		{  
			/* EXEC SQL FETCH :cur_msgInfo INTO :id,:send_time,:fontSize,:fontColor,:fontStyle,:var_fontName,:var_nickName,:var_data,:msgid; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )865;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqfoff = (         int )0;
   sqlstm.sqfmod = (unsigned int )2;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&id;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&send_time;
   sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&fontSize;
   sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)&fontColor;
   sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (unsigned char  *)&fontStyle;
   sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         short *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned long )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqhstv[6] = (unsigned char  *)&var_fontName;
   sqlstm.sqhstl[6] = (unsigned long )52;
   sqlstm.sqhsts[6] = (         int  )0;
   sqlstm.sqindv[6] = (         short *)0;
   sqlstm.sqinds[6] = (         int  )0;
   sqlstm.sqharm[6] = (unsigned long )0;
   sqlstm.sqadto[6] = (unsigned short )0;
   sqlstm.sqtdso[6] = (unsigned short )0;
   sqlstm.sqhstv[7] = (unsigned char  *)&var_nickName;
   sqlstm.sqhstl[7] = (unsigned long )43;
   sqlstm.sqhsts[7] = (         int  )0;
   sqlstm.sqindv[7] = (         short *)0;
   sqlstm.sqinds[7] = (         int  )0;
   sqlstm.sqharm[7] = (unsigned long )0;
   sqlstm.sqadto[7] = (unsigned short )0;
   sqlstm.sqtdso[7] = (unsigned short )0;
   sqlstm.sqhstv[8] = (unsigned char  *)&var_data;
   sqlstm.sqhstl[8] = (unsigned long )1317;
   sqlstm.sqhsts[8] = (         int  )0;
   sqlstm.sqindv[8] = (         short *)0;
   sqlstm.sqinds[8] = (         int  )0;
   sqlstm.sqharm[8] = (unsigned long )0;
   sqlstm.sqadto[8] = (unsigned short )0;
   sqlstm.sqtdso[8] = (unsigned short )0;
   sqlstm.sqhstv[9] = (unsigned char  *)&msgid;
   sqlstm.sqhstl[9] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[9] = (         int  )0;
   sqlstm.sqindv[9] = (         short *)0;
   sqlstm.sqinds[9] = (         int  )0;
   sqlstm.sqharm[9] = (unsigned long )0;
   sqlstm.sqadto[9] = (unsigned short )0;
   sqlstm.sqtdso[9] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			db_ret = sqlca.sqlcode;
			if (db_ret == 1403 )
			{
				nRetCount = iNum;
				break;
			}
			if (iNum >= nRetCount)
			{
				break;
			}	
			if( db_ret < 0 && db_ret != -1405 )
			{
				sql_error(sqlca, oraca);
				nRetCount = iNum;
				break;
			}		
			msgInfo[iNum].id = id;
			msgInfo[iNum].send_time = send_time;
			msgInfo[iNum].fontSize = fontSize;
			msgInfo[iNum].fontColor = fontColor;
			msgInfo[iNum].fontStyle = fontStyle;
			msgInfo[iNum].msgid = msgid;
			
			SET_END_NULL(var_fontName);
			SET_END_NULL(var_nickName);
			SET_END_NULL(var_data);

			
			strncpy(msgInfo[iNum].fontName,(char*)var_fontName.arr,50);	
			strncpy(msgInfo[iNum].nickName,(char*)var_nickName.arr,MAX_NICKNAME_LEN+1);
			strncpy(msgInfo[iNum].data,(char*)var_data.arr,1315);	
	
		
			
			++iNum;
		}
		/* EXEC SQL CLOSE :cur_msgInfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )920;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL FREE  :cur_msgInfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )939;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_msgInfo;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_SUCCESS;		
}

/*================================================================ 
*
* 函 数 名：Get_Family_MaxMsgID 
** 根据家族ID获取当前家族最大和最小消息ID
* 参 数： 
* uint32 c_iFamilyID [[IN]] : 家族ID
* uint32 & nMsgID [IN,OUT]: 家族当前消息ID
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 数据库错误:DB_ERROR;  如果家族创建后无任何消息，nMsgID = 0;
* 
================================================================*/ 
int  CDatabaseMini::Get_Family_MaxMsgID(const uint32 c_iFamilyID, uint32 & nMaxMsgID, uint32 &nMinMsgID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int      nFamilyID;
		unsigned int      nmaxmsgid,nminmsgid;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	nMaxMsgID = 0;
	nFamilyID = c_iFamilyID; 
	nmaxmsgid = 0;
	nminmsgid = 0;	 
	 
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);

	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:	
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.GetFamilyMsgID(:nFamilyID,:nmaxmsgid,:nminmsgid,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . GetFamilyMsgID ( :nFamilyID , :nmaxm\
sgid , :nminmsgid , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )958;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nFamilyID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&nmaxmsgid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nminmsgid;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[4] = (unsigned long )34;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

    
	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 && db_ret != -1405) //数据库发生错误
	{
		sql_error(sqlca, oraca);
		CLog::Log("Get_Family_MaxMsgID",CLog::TYPE_ERROR,"获取家族最大消息ID失败,nFamilyID:%d, db_ret:%d",nFamilyID,db_ret);
		return DB_ERROR;
	}
	
	if( db_ret >= 0 && db_ret != 1403)
	{
		nMaxMsgID = nmaxmsgid;
		nMinMsgID = nminmsgid;
	}
	else	//初始ID从1开始
	{
		nMaxMsgID = 1;
		nMinMsgID = 0;	
	}	

	return DB_SUCCESS;
}

/*================================================================ 
*
* 函 数 名：Save_Family_RequestMsg 
** 保存家族请求消息ID
* 参 数： 
* uint32 c_iFamilyID	[IN] : 家族ID
* uint32 iSenderID		[IN] : 发送者ID
* uint32 iSendTime		[IN] : 请求时间
* char   *strMsg		[IN] : 请求内容
* int	 iMsgLen		[IN] : 内容长度
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 数据库错误:DB_ERROR; 
* 
================================================================*/ 
int CDatabaseMini::Save_Family_RequestMsg(const uint32 c_iFamilyID, uint32 iSenderID,uint32 iSendTime,const char *strMsg, int iMsgLen)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		long_varraw * lvr;
		unsigned int     nFamilyID, nSenderID, nSendTime, nMsgSeq;
		int nMsgLen, nMsgType;
		char*   pmsg;
		OCIBlobLocator *blob;
		unsigned int offset = 1; /* attention: typedef ub4 unsigned int */
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);

	
	nFamilyID = c_iFamilyID;
	nSenderID = iSenderID;
	nSendTime = iSendTime;
	nMsgLen = iMsgLen;
	nMsgType = CMD_CLUSTER_AUTH_REQ;
	pmsg = (char*)strMsg;
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;	

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:   	
	/* EXEC SQL AT :db_link_name ALLOCATE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )993;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	
	/* EXEC SQL AT :db_link_name SELECT IM_CLUSTERSYSMSG_SEQ_MSGID.nextval INTO :nMsgSeq FROM DUAL; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select IM_CLUSTERSYSMSG_SEQ_MSGID.nextval  into :b1  from D\
UAL ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1016;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nMsgSeq;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL AT :db_link_name INSERT INTO IM_CLUSTERSYSMSG(MSGID, CLUSTERID,SENDER,SRCIP,SRCPORT,SENDTIME,MSGTYPE,MSGLEN,MSG)
		VALUES(:nMsgSeq, :nFamilyID,:nSenderID,0,0,:nSendTime,:nMsgType,:nMsgLen,empty_blob()); */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "insert into IM_CLUSTERSYSMSG (MSGID,CLUSTERID,SENDER,SRCIP,\
SRCPORT,SENDTIME,MSGTYPE,MSGLEN,MSG) values (:b1,:b2,:b3,0,0,:b4,:b5,:b6,empt\
y_blob())";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1039;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nMsgSeq;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&nFamilyID;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nSenderID;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&nSendTime;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&nMsgType;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&nMsgLen;
 sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[6] = (unsigned long )34;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         short *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned long )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 || db_ret == 1403 )
	{
		sql_error(sqlca, oraca);	
		/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1082;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1101;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	} 
	
	/* EXEC SQL AT :db_link_name COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1124;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 

	/* EXEC SQL AT :db_link_name SELECT MSG INTO :blob  FROM IM_CLUSTERSYSMSG 
		WHERE MSGID = :nMsgSeq FOR UPDATE; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select MSG into :b1  from IM_CLUSTERSYSMSG where MSGID=:b2 \
for update ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1143;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&nMsgSeq;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 

	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 || db_ret == 1403 )
	{
		sql_error(sqlca, oraca);	
		/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1170;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1189;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	} 
  
	lvr = (long_varraw *)malloc(sizeof(ub4) + nMsgLen);
	lvr->len = nMsgLen;
	memcpy(lvr->buf,pmsg,nMsgLen);
	/* EXEC SQL AT :db_link_name LOB WRITE ONE :nMsgLen 
			FROM :lvr WITH LENGTH :nMsgLen INTO :blob AT :offset; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1212;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nMsgLen;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)lvr;
 sqlstm.sqhstl[1] = (unsigned long )4;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nMsgLen;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&blob;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&offset;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[5] = (unsigned long )34;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 || db_ret == 1403 )
	{
		sql_error(sqlca, oraca);	
		/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1251;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1270;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		free(lvr);
		return DB_ERROR;
	} 
	free(lvr);

	/* EXEC SQL AT :db_link_name UPDATE IM_CLUSTERSYSMSG SET MSG = :blob 
		WHERE MSGID = :nMsgSeq; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update IM_CLUSTERSYSMSG  set MSG=:b1 where MSGID=:b2";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1293;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&nMsgSeq;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 || db_ret == 1403 )
	{
		sql_error(sqlca, oraca);	
		/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1320;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1339;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	} 
  
	/* EXEC SQL AT :db_link_name COMMIT WORK; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1362;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1381;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL AT :db_link_name COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1404;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	
	return DB_SUCCESS;
}

/*================================================================ 
*
* 函 数 名：Save_User_Msg 
** 保存用户的消息，包括系统消息
* 参 数： 
* uint32 iRecvID	[IN] : 家族ID
* uint32 iSendID		[IN] : 发送者ID
* uint32 iNatIP,uint16 iNatPort,uint32 iTime [IN] 
* uint16 iMsgType		[IN] : 消息类型
* char   *strMsg		[IN] : 消息内容
* uint16	 iMsgLen		[IN] : 内容长度
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 数据库错误:DB_ERROR; 
* 
================================================================*/ 
int CDatabaseMini::Save_User_Msg(uint32 iRecvID,uint32 iSendID,uint32 iNatIP,uint16 iNatPort,uint32 iTime,uint16 iMsgType,const char *strMsg, uint16 iMsgLen,const char *strText)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		long_varraw * lvr;
		unsigned int    nRecvID,nSendID, nTime,nMsgLen;
		unsigned int	int_tmp,natip;
		short   natport,nMsgType;
		char*   pmsg;
		OCIBlobLocator *blob;
		unsigned int offset = 1;  /* attention: typedef ub4 unsigned int */
		/* VARCHAR db_link_name[32], var_text[1315]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

struct { unsigned short len; unsigned char arr[1315]; } var_text;

		/* VARCHAR var_fromname[41], var_toname[41]; */ 
struct { unsigned short len; unsigned char arr[41]; } var_fromname;

struct { unsigned short len; unsigned char arr[41]; } var_toname;

	/* EXEC SQL END DECLARE SECTION; */ 

 	CLog::Log("Save_User_Msg",CLog::TYPE_DEBUG,"iRecvID is %d,iSendID is %d,iNatIP is %d,iNatPort is %d,iTime is %d,iMsgType is %d,strMsg is %s,iMsgLen is %d,strText is %s",
	iRecvID,iSendID,iNatIP,iNatPort,iTime,iMsgType,strMsg,iMsgLen,strText);	
	if( !strMsg )
	{
		CLog::Log("Save_User_Msg",CLog::TYPE_DEBUG,"strMsg is NULL");
		return DB_PARAM;
	}
	if(strlen(strText) == 0 )
	{
         CLog::Log("Save_User_Msg",CLog::TYPE_DEBUG,"strText len is 0");
         return DB_PARAM;
    }
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);

	SET_INIT_NULL(var_text);
	SET_INIT_NULL(var_fromname);
	SET_INIT_NULL(var_toname);

	nRecvID = iRecvID;
	nSendID = iSendID;
	nTime = iTime;
	nMsgLen = iMsgLen;
	nMsgType = iMsgType;
	pmsg = (char*)strMsg;
	strncpy((char *)var_text.arr,strText,1315);
	var_text.len=(unsigned short)strlen(strText);
	natport = iNatPort;
	natip = iNatIP;
	//delline = 3;
		
	int db_ret;
	int iReconnTimes;
	bool bReconn;
 
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start: 
	/* EXEC SQL AT :db_link_name ALLOCATE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1423;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	if( nMsgType == CMD_P2PMSG_SEND )
	{
		/* EXEC SQL AT :db_link_name SELECT COUNT(MSGID) INTO :int_tmp FROM IM_OFFLINETALKMSG 
			WHERE RECEIVER=:nRecvID; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select count(MSGID) into :b1  from IM_OFFLINETALKMSG where\
 RECEIVER=:b2";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1446;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&nRecvID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( db_ret < 0 )
		{
			sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1473;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;
		}

		if (db_ret == 1403)
			int_tmp = 0;
		
		if( int_tmp >= OFFLINE_MAX_MSGNUM  && nRecvID != MANAGER_ID) 
		{
			/* EXEC SQL AT :db_link_name DELETE FROM IM_OFFLINETALKMSG WHERE  MSGID=(SELECT Min(MSGID) FROM IM_OFFLINETALKMSG WHERE RECEIVER=:nRecvID); */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "delete  from IM_OFFLINETALKMSG  where MSGID=(select min(M\
SGID)  from IM_OFFLINETALKMSG where RECEIVER=:b1)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1496;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&nRecvID;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			db_ret = sqlca.sqlcode;
			if (conn_error(db_ret))
			{
				iReconnTimes=0;
				bReconn=ConnectDB();
				while(!bReconn)
				{
					iReconnTimes++;
					sleep(iReconnTimes%10);
					bReconn=ConnectDB();
				}
				goto start;				
			}
			
			if( db_ret < 0  )
			{
				sql_error(sqlca, oraca);
				/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )1519;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&blob;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[1] = (unsigned long )34;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )1542;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[0] = (unsigned long )34;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				return DB_ERROR;			
			}
			/* EXEC SQL AT :db_link_name COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1561;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 
		}
		
		/* EXEC SQL AT :db_link_name select nickname into :var_fromname from im_userinfo where userid=:nSendID; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select nickname into :b1  from im_userinfo where userid=:b\
2";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1580;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&var_fromname;
  sqlstm.sqhstl[0] = (unsigned long )43;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&nSendID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name select nickname into :var_toname from im_userinfo where userid=:nRecvID; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select nickname into :b1  from im_userinfo where userid=:b\
2";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1607;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&var_toname;
  sqlstm.sqhstl[0] = (unsigned long )43;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&nRecvID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name SELECT IM_OFFLINETALKMSG_SEQ_MSGID.nextval into :int_tmp FROM DUAL; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select IM_OFFLINETALKMSG_SEQ_MSGID.nextval  into :b1  from\
 DUAL ";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1634;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* 如果要用IM服务器时间，则 sendtime=cast(from_tz(to_timestamp ('19700101000000', 'yyyymmddhh24miss')+ numtodsinterval(:nTime, 'second'), 'gmt') at time zone sessiontimezone as date) */
		/* EXEC SQL AT :db_link_name INSERT INTO IM_OFFLINETALKMSG(MSGID, RECEIVER,SENDER,SRCIP,SRCPORT,SENDTIME,
			MSGTYPE,MSGLEN,MSG,TEXT,SenderName,ReceiverName)
			VALUES(:int_tmp, :nRecvID,:nSendID,:natip,:natport,sysdate,
			:nMsgType,:nMsgLen,empty_blob(),:var_text,:var_fromname,:var_toname); */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "insert into IM_OFFLINETALKMSG (MSGID,RECEIVER,SENDER,SRCIP\
,SRCPORT,SENDTIME,MSGTYPE,MSGLEN,MSG,TEXT,SenderName,ReceiverName) values (:b\
1,:b2,:b3,:b4,:b5,sysdate,:b6,:b7,empty_blob(),:b8,:b9,:b10)";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1657;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&nRecvID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&nSendID;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&natip;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&natport;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&nMsgType;
  sqlstm.sqhstl[5] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&nMsgLen;
  sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         short *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned long )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (unsigned char  *)&var_text;
  sqlstm.sqhstl[7] = (unsigned long )1317;
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         short *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned long )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (unsigned char  *)&var_fromname;
  sqlstm.sqhstl[8] = (unsigned long )43;
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         short *)0;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned long )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqhstv[9] = (unsigned char  *)&var_toname;
  sqlstm.sqhstl[9] = (unsigned long )43;
  sqlstm.sqhsts[9] = (         int  )0;
  sqlstm.sqindv[9] = (         short *)0;
  sqlstm.sqinds[9] = (         int  )0;
  sqlstm.sqharm[9] = (unsigned long )0;
  sqlstm.sqadto[9] = (unsigned short )0;
  sqlstm.sqtdso[9] = (unsigned short )0;
  sqlstm.sqhstv[10] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[10] = (unsigned long )34;
  sqlstm.sqhsts[10] = (         int  )0;
  sqlstm.sqindv[10] = (         short *)0;
  sqlstm.sqinds[10] = (         int  )0;
  sqlstm.sqharm[10] = (unsigned long )0;
  sqlstm.sqadto[10] = (unsigned short )0;
  sqlstm.sqtdso[10] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( db_ret < 0 || db_ret == 1403 )
		{
			sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1716;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1739;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;			
		}
		/* EXEC SQL AT :db_link_name COMMIT; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1758;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 

		/* EXEC SQL AT :db_link_name SELECT MSG INTO :blob  FROM IM_OFFLINETALKMSG 
			WHERE MSGID = :int_tmp FOR UPDATE; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select MSG into :b1  from IM_OFFLINETALKMSG where MSGID=:b\
2 for update ";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1777;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( sqlca.sqlcode < 0 || sqlca.sqlcode == 1403 )
		{
			sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1804;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1827;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;			
		} 
	}
	else
	{
		/* EXEC SQL AT :db_link_name SELECT COUNT(MSGID) INTO :int_tmp FROM IM_OFFLINESYSMSG 
			WHERE RECEIVER=:nRecvID; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select count(MSGID) into :b1  from IM_OFFLINESYSMSG where \
RECEIVER=:b2";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1846;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&nRecvID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( db_ret < 0 )
		{
			sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1873;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;			
		}
		if( int_tmp >= OFFLINE_MAX_MSGNUM ) 
		{
			/* EXEC SQL AT :db_link_name DELETE FROM IM_OFFLINESYSMSG WHERE  MSGID=(SELECT Min(MSGID) FROM IM_OFFLINESYSMSG WHERE RECEIVER=:nRecvID); */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "delete  from IM_OFFLINESYSMSG  where MSGID=(select min(MS\
GID)  from IM_OFFLINESYSMSG where RECEIVER=:b1)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1896;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&nRecvID;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			db_ret = sqlca.sqlcode;
			if (conn_error(db_ret))
			{
				iReconnTimes=0;
				bReconn=ConnectDB();
				while(!bReconn)
				{
					iReconnTimes++;
					sleep(iReconnTimes%10);
					bReconn=ConnectDB();
				}
				goto start;				
			}
			
			if( db_ret < 0 )
			{
				sql_error(sqlca, oraca);
				/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )1919;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&blob;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[1] = (unsigned long )34;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )1942;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[0] = (unsigned long )34;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				return DB_ERROR;			
			}
			/* EXEC SQL AT :db_link_name COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1961;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 
		} 
		
		/* EXEC SQL AT :db_link_name SELECT IM_OFFLINESYSMSG_SEQ_MSGID.nextval into :int_tmp FROM DUAL; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select IM_OFFLINESYSMSG_SEQ_MSGID.nextval  into :b1  from \
DUAL ";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1980;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		
		/* EXEC SQL AT :db_link_name INSERT INTO IM_OFFLINESYSMSG(MSGID, RECEIVER,SENDER,SRCIP,SRCPORT,SENDTIME,MSGTYPE,MSGLEN,MSG)
			VALUES(:int_tmp, :nRecvID,:nSendID,:natip,:natport,:nTime,:nMsgType,:nMsgLen,empty_blob()); */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "insert into IM_OFFLINESYSMSG (MSGID,RECEIVER,SENDER,SRCIP,\
SRCPORT,SENDTIME,MSGTYPE,MSGLEN,MSG) values (:b1,:b2,:b3,:b4,:b5,:b6,:b7,:b8,\
empty_blob())";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2003;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&nRecvID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&nSendID;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&natip;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&natport;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&nTime;
  sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&nMsgType;
  sqlstm.sqhstl[6] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         short *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned long )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (unsigned char  *)&nMsgLen;
  sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         short *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned long )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[8] = (unsigned long )34;
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         short *)0;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned long )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( db_ret < 0 || db_ret == 1403 )
		{
			sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2054;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2077;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;			
		}
		/* EXEC SQL AT :db_link_name COMMIT; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2096;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 

		/* EXEC SQL AT :db_link_name SELECT MSG INTO :blob  FROM IM_OFFLINESYSMSG 
			WHERE MSGID = :int_tmp FOR UPDATE; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select MSG into :b1  from IM_OFFLINESYSMSG where MSGID=:b2\
 for update ";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2115;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 
		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( db_ret < 0 || db_ret == 1403 )
		{
			sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2142;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2165;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;			
		} 
	}

	lvr = (long_varraw *)malloc(sizeof(ub4) + nMsgLen);
	lvr->len = nMsgLen;
	memcpy(lvr->buf,pmsg,nMsgLen);
	/* EXEC SQL AT :db_link_name LOB WRITE ONE :nMsgLen
			FROM :lvr WITH LENGTH :nMsgLen INTO :blob AT :offset; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2184;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nMsgLen;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)lvr;
 sqlstm.sqhstl[1] = (unsigned long )4;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nMsgLen;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&blob;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&offset;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[5] = (unsigned long )34;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 || db_ret == 1403 )
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2223;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2246;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		free(lvr);
		return DB_ERROR;			
	} 
	free(lvr);

	if( nMsgType == CMD_P2PMSG_SEND )
	{
		/* EXEC SQL AT :db_link_name UPDATE IM_OFFLINETALKMSG SET MSG = :blob 
			WHERE MSGID = :int_tmp; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "update IM_OFFLINETALKMSG  set MSG=:b1 where MSGID=:b2";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2265;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( db_ret < 0 || db_ret == 1403 )
		{
			sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2292;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2315;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;			
		} 
		/* EXEC SQL AT :db_link_name COMMIT; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2334;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 
	}
	else
	{
		/* EXEC SQL AT :db_link_name UPDATE IM_OFFLINESYSMSG SET MSG = :blob 
			WHERE MSGID = :int_tmp; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "update IM_OFFLINESYSMSG  set MSG=:b1 where MSGID=:b2";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2353;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( db_ret < 0 || db_ret == 1403 )
		{
			sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2380;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2403;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;			
		} 
	}

	/* EXEC SQL AT :db_link_name COMMIT WORK; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2422;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2441;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL AT :db_link_name COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2464;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



	return DB_SUCCESS;
}

/*================================================================ 
*
* 函 数 名：Delete_Family_Msg 
** 删除家族的消息
* 参 数：
const uint32 iFamilyID	[IN] : 删除家族
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 操作失败: DB_ERROR;
* 
================================================================*/
int CDatabaseMini::Delete_Family_Msg(const uint32 iFamilyID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int      nFamilyID;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nFamilyID		= iFamilyID;
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.DeleteFamilyMsg(:nFamilyID,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . DeleteFamilyMsg ( :nFamilyID , :retc\
ode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2483;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nFamilyID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	    
	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 )
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}   

	return DB_SUCCESS;

}
int CDatabaseMini::Delete_FamilyMem_RecvMsg(const uint32 iFamilyID, uint32 iUserID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int      nFamilyID, nUserID;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nFamilyID		= iFamilyID;
	nUserID			= iUserID;
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.DeleteFamilyMemRecvMsg(:nFamilyID,:nUserID,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . DeleteFamilyMemRecvMsg ( :nFamilyID \
, :nUserID , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2510;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nFamilyID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&nUserID;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[3] = (unsigned long )34;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 )
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	} 
	return DB_SUCCESS;
}

/*================================================================ 
*
* 函 数 名：Save_Family_Msg 
** 保存家族请求消息ID获取当前家族最大消息ID
* 参 数： 
* uint32 familyMsgID	[IN] : 家族消息ID
* uint32 familyID		[IN] : 家族ID
* uint32 senderID		[IN] : 发送者ID
* uint32 tm				[IN] : 发送时间
* int	 msgType		[IN] : 消息类型
* char   *msg			[IN] : 消息内容
* int	 msgLen			[IN] : 内容长度
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 数据库错误:DB_ERROR; 
* 
================================================================*/ 
int CDatabaseMini::Save_Family_Msg(uint32 familyMsgID,uint32 familyID,uint32 senderID, int tm, int msgType,int msgLen,char *msg)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		long_varraw * lvr;
		unsigned int    cid,sno;
		int				time,msglen;
		unsigned int int_tmp, cmid, minmsgid;
		unsigned int natip;
		short   natport,msgtype;
		char*   pmsg;
		OCIBlobLocator *blob;
		unsigned int offset = 1;  /* attention: typedef ub4 unsigned int */
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 

  
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	int_tmp = 0;
	
	cmid = familyMsgID;
	cid = familyID;
	sno = senderID;
	natip = 0;
	natport = 0;
	time = tm;
	msglen = msgLen;
	msgtype = msgType;
	pmsg = msg;
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;	

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	
start:	

	/* EXEC SQL AT :db_link_name ALLOCATE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2541;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL AT :db_link_name SELECT COUNT(MSGID) INTO :int_tmp FROM IM_CLUSTERTALKMSG 
		WHERE CLUSTERID=:cid; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select count(MSGID) into :b1  from IM_CLUSTERTALKMSG where \
CLUSTERID=:b2";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2564;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&int_tmp;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&cid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( sqlca.sqlcode < 0 )
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2591;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}

	bool isupdate = false;
	if( int_tmp >= FAMILE_MAX_MSGNUM ) 
	{
		/* EXEC SQL AT :db_link_name SELECT MIN(MSGID) INTO :minmsgid FROM IM_CLUSTERTALKMSG 
			WHERE CLUSTERID=:cid; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select min(MSGID) into :b1  from IM_CLUSTERTALKMSG where C\
LUSTERID=:b2";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2614;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&minmsgid;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&cid;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (db_ret < 0)
		{
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2641;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;
		}
		isupdate = true;
	}
	
	/* EXEC SQL AT :db_link_name SELECT MAX(MSGID) INTO :int_tmp FROM IM_CLUSTERTALKMSG 
		WHERE CLUSTERID=:cid; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select max(MSGID) into :b1  from IM_CLUSTERTALKMSG where CL\
USTERID=:b2";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2664;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&int_tmp;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&cid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if ((db_ret < 0 && db_ret != -1405) || db_ret == 1403)
	{
		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2691;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}
	if (db_ret == -1405)
		int_tmp = 1;
	
	if (isupdate == false)
	{
		/* EXEC SQL AT :db_link_name INSERT INTO IM_CLUSTERTALKMSG(MSGID,CLUSTERID,SENDER,SRCIP,SRCPORT,SENDTIME,MSGTYPE,MSGLEN,MSG)
			VALUES(:cmid,:cid,:sno,:natip,:natport,:time,:msgtype,:msglen,empty_blob()); */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "insert into IM_CLUSTERTALKMSG (MSGID,CLUSTERID,SENDER,SRCI\
P,SRCPORT,SENDTIME,MSGTYPE,MSGLEN,MSG) values (:b1,:b2,:b3,:b4,:b5,:b6,:b7,:b\
8,empty_blob())";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2714;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cmid;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&cid;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&sno;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&natip;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&natport;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&time;
  sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&msgtype;
  sqlstm.sqhstl[6] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         short *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned long )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (unsigned char  *)&msglen;
  sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         short *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned long )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[8] = (unsigned long )34;
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         short *)0;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned long )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( sqlca.sqlcode < 0 || sqlca.sqlcode == 1403 )
		{
			sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2765;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2784;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;
		} 
		/* EXEC SQL AT :db_link_name COMMIT; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2807;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 
	}
	else
	{
		/* EXEC SQL AT :db_link_name UPDATE IM_CLUSTERTALKMSG 
				SET MSGID=:cmid,SENDER=:sno,SRCIP=:natip,SRCPORT=:natport,SENDTIME=:time,MSGTYPE=:msgtype,MSGLEN=:msglen,MSG=empty_blob() 
				where MSGID = :minmsgid and CLUSTERID = :cid; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "update IM_CLUSTERTALKMSG  set MSGID=:b1,SENDER=:b2,SRCIP=:\
b3,SRCPORT=:b4,SENDTIME=:b5,MSGTYPE=:b6,MSGLEN=:b7,MSG=empty_blob() where (MS\
GID=:b8 and CLUSTERID=:b9)";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2826;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cmid;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&sno;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&natip;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&natport;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&time;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&msgtype;
  sqlstm.sqhstl[5] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&msglen;
  sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         short *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned long )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (unsigned char  *)&minmsgid;
  sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         short *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned long )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (unsigned char  *)&cid;
  sqlstm.sqhstl[8] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         short *)0;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned long )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqhstv[9] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[9] = (unsigned long )34;
  sqlstm.sqhsts[9] = (         int  )0;
  sqlstm.sqindv[9] = (         short *)0;
  sqlstm.sqinds[9] = (         int  )0;
  sqlstm.sqharm[9] = (unsigned long )0;
  sqlstm.sqadto[9] = (unsigned short )0;
  sqlstm.sqtdso[9] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( sqlca.sqlcode < 0 || sqlca.sqlcode == 1403 )
		{
			sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2881;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2900;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;
		} 
		/* EXEC SQL AT :db_link_name COMMIT; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2923;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 
		
	}

	/* EXEC SQL AT :db_link_name SELECT MSG INTO :blob  FROM IM_CLUSTERTALKMSG 
		WHERE MSGID=:cmid AND CLUSTERID=:cid FOR UPDATE; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select MSG into :b1  from IM_CLUSTERTALKMSG where (MSGID=:b\
2 and CLUSTERID=:b3) for update ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2942;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&cmid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&cid;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[3] = (unsigned long )34;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( sqlca.sqlcode < 0 || sqlca.sqlcode == 1403 )
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2973;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2992;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		if( db_ret == -2112)
		{
			/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )3015;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL AT :db_link_name delete IM_CLUSTERTALKMSG t where rowid > (
				select min(rowid) from IM_CLUSTERTALKMSG y where t.MSGID=y.msgid and t.CLUSTERID=y.clusterid ); */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "delete  from IM_CLUSTERTALKMSG t  where rowid>(select min\
(rowid)  from IM_CLUSTERTALKMSG y where (t.MSGID=y.msgid and t.CLUSTERID=y.cl\
usterid))";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )3034;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL AT :db_link_name SELECT MSG INTO :blob  FROM IM_CLUSTERTALKMSG 
				WHERE MSGID=:cmid AND CLUSTERID=:cid FOR UPDATE; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select MSG into :b1  from IM_CLUSTERTALKMSG where (MSGID=\
:b2 and CLUSTERID=:b3) for update ";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )3053;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&cmid;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&cid;
   sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[3] = (unsigned long )34;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

  
		}
		else
			return DB_ERROR;
	} 
  
	lvr = (long_varraw *)malloc(sizeof(ub4) + msglen);
	lvr->len = msglen;  memcpy(lvr->buf,pmsg,msglen);
	/* EXEC SQL AT :db_link_name LOB WRITE ONE :msglen
			FROM :lvr WITH LENGTH :msglen INTO :blob AT :offset; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3084;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&msglen;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)lvr;
 sqlstm.sqhstl[1] = (unsigned long )4;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&msglen;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&blob;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&offset;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[5] = (unsigned long )34;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( sqlca.sqlcode < 0 || sqlca.sqlcode == 1403 )
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3123;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3142;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		free(lvr);
		return DB_ERROR;
	} 

	free(lvr);

	/* EXEC SQL AT :db_link_name UPDATE IM_CLUSTERTALKMSG SET MSG = :blob 
		WHERE MSGID=:cmid AND CLUSTERID=:cid; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update IM_CLUSTERTALKMSG  set MSG=:b1 where (MSGID=:b2 and \
CLUSTERID=:b3)";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3165;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&cmid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&cid;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[3] = (unsigned long )34;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( sqlca.sqlcode < 0 || sqlca.sqlcode == 1403 )
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3196;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3215;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	} 
  
	/* EXEC SQL AT :db_link_name COMMIT WORK; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3238;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3257;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL AT :db_link_name COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3280;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



	return DB_SUCCESS;
}

int CDatabaseMini::Get_User_OnlineTime(uint32 iUserID,uint32 &OnlineTime)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int	nUserID;
		double			timelen;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	nUserID = iUserID;
	timelen = 0;
	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.GetUserOnlineTime(:nUserID,:timelen,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . GetUserOnlineTime ( :nUserID , :time\
len , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3299;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nUserID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&timelen;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(double);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[3] = (unsigned long )34;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if ((db_ret < 0  && db_ret != -1405)) //出错
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}

	OnlineTime = (uint32)timelen;
	return DB_SUCCESS;
}

/*================================================================ 
*
* 函 数 名：Add_Group 
** 添加分组
* 参 数： 
* uint32 iUserID [[IN]] : 用户ID
* uint8 &iGroupID [IN,OUT]: 组ID，输入指定的ID。输出数据库中存储的ID
* char * strGroupName	[IN] : 组名
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 数据库错误:DB_ERROR;  用所能创建的组已满：DB_GROUP_FULL; 组名已经存在:DB_GROUP_EXIST;
* 
================================================================*/ 
int CDatabaseMini::Add_Group(uint32 iUserID, uint8 &iGroupID, char * strGroupName, uint8 &iGroupPid)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR var_groupname[21]; */ 
struct { unsigned short len; unsigned char arr[21]; } var_groupname;

		unsigned int nUserID;
		int     g_id;
		int     g_pid;
		int		retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	SET_INIT_NULL(var_groupname);
	nUserID = iUserID;
	g_id = iGroupID;
	g_pid = iGroupPid;
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	if( strlen(strGroupName) > 20 )
		return DB_PARAM;
	
	strncpy((char*)var_groupname.arr, strGroupName,MAX_GROUPNAME_LEN);
	var_groupname.len = (unsigned short)strlen(strGroupName);
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:  
	
	/* EXEC SQL AT :db_link_name EXECUTE
	begin
		Mini_Local_Pkg.AddGroup(:nUserID,:g_id,:var_groupname,:g_pid,:retcode);
	end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . AddGroup ( :nUserID , :g_id , :var_g\
roupname , :g_pid , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3330;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nUserID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&g_id;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_groupname;
 sqlstm.sqhstl[2] = (unsigned long )23;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&g_pid;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[5] = (unsigned long )34;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
		
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if ( db_ret < 0)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;	
	}    
	
	if( retcode == DB_GROUP_FULL )
		return DB_GROUP_FULL;
	
	if( retcode == DB_GROUP_EXIST )
		return DB_GROUP_EXIST;
		
	return DB_SUCCESS;
}

int CDatabaseMini::Mod_Cust_Info(uint32 id, char* account, char* time)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR var_account[40]; */ 
struct { unsigned short len; unsigned char arr[40]; } var_account;

		/* VARCHAR var_time[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } var_time;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		unsigned int nID;
		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	SET_INIT_NULL(var_account);
	SET_INIT_NULL(var_time);
	nID = id;

	var_account.len = (unsigned short)strlen(account);
	strncpy((char*)var_account.arr,account,MAX_CUST_ACCOUNT_LEN);

	var_time.len = (unsigned short)strlen(time);
	strncpy((char*)var_time.arr,time,MAX_CUST_TIME_LEN);
	 
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	int	db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.ModifyCustInfo(:nID,:var_account,:var_time,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . ModifyCustInfo ( :nID , :var_account\
 , :var_time , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3369;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&var_account;
 sqlstm.sqhstl[1] = (unsigned long )42;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_time;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[4] = (unsigned long )34;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if(db_ret == 1403 || db_ret < 0)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
	if (retcode == 1)
		return DB_ERROR;

	return DB_SUCCESS;
}


int CDatabaseMini::Mod_Cust_Remark(uint32 id, char* account, char* time,char* remark)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR var_account[40]; */ 
struct { unsigned short len; unsigned char arr[40]; } var_account;

		/* VARCHAR var_time[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } var_time;

		/* VARCHAR var_remark[201]; */ 
struct { unsigned short len; unsigned char arr[201]; } var_remark;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		unsigned int nID;
		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	SET_INIT_NULL(var_account);
	SET_INIT_NULL(var_time);
	SET_INIT_NULL(var_remark);
	
	nID = id;

	var_account.len = (unsigned short)strlen(account);
	strncpy((char*)var_account.arr,account,MAX_CUST_ACCOUNT_LEN);

	var_time.len = (unsigned short)strlen(time);
	strncpy((char*)var_time.arr,time,MAX_CUST_TIME_LEN);

	var_remark.len = (unsigned short)strlen(remark);
	strncpy((char*)var_remark.arr,remark,MAX_CUST_REMAKE_LEN);
	 
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	int	db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.ModifyCustRemark(:nID,:var_account,:var_time,:var_remark,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . ModifyCustRemark ( :nID , :var_accou\
nt , :var_time , :var_remark , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3404;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&var_account;
 sqlstm.sqhstl[1] = (unsigned long )42;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_time;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&var_remark;
 sqlstm.sqhstl[3] = (unsigned long )203;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[5] = (unsigned long )34;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	CLog::Log("CBusinessModCustRemark", CLog::TYPE_DEBUG, "id:%d, db_ret:%d, retcode:%d", id, db_ret, retcode);
	
	if(db_ret == 1403 || db_ret < 0)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
	
	if (retcode == 1)
		return DB_ERROR;

	return DB_SUCCESS;
}




int CDatabaseMini::Get_Cust_Remark(uint32 id, char* account, char* time, char* remark)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		/* VARCHAR var_account[40]; */ 
struct { unsigned short len; unsigned char arr[40]; } var_account;

		/* VARCHAR var_time[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } var_time;

		/* VARCHAR var_remark[201]; */ 
struct { unsigned short len; unsigned char arr[201]; } var_remark;

		unsigned int nID;
		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	strncpy((char*)var_account.arr, account,MAX_CUST_ACCOUNT_LEN);
	var_account.len = (short)strlen(account);
	
	nID=id;

	SET_INIT_NULL(var_time);
	SET_INIT_NULL(var_remark);
	
	int	db_ret, iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.GetCustRemark(:nID,:var_account,:var_time,:var_remark,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . GetCustRemark ( :nID , :var_account \
, :var_time , :var_remark , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3443;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&var_account;
 sqlstm.sqhstl[1] = (unsigned long )42;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_time;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&var_remark;
 sqlstm.sqhstl[3] = (unsigned long )203;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[5] = (unsigned long )34;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}




	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	CLog::Log("CDatabaseMini::Get_Cust_Remark", CLog::TYPE_DEBUG, "id:%d, db_ret:%d, retcode:%d",id, db_ret, retcode);

		if( db_ret < 0 && db_ret != -1405) 
	{	
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
	
	if (retcode == 1)
	{
		return DB_ERROR;
	}
	
	strncpy(time,(char*)var_time.arr,32);	
	strncpy(remark,(char*)var_remark.arr,MAX_CUST_REMAKE_LEN);
	
	return DB_SUCCESS;
}




int CDatabaseMini::Get_Cust_Num(uint32 id, char* btime, char* etime, uint32& nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		/* VARCHAR var_btime[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } var_btime;

		/* VARCHAR var_etime[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } var_etime;

		unsigned int nID, nCount;
		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	 
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	strncpy((char*)var_btime.arr, btime,32);
	var_btime.len = (short)strlen(btime);
	
	strncpy((char*)var_etime.arr, etime,32);
	var_etime.len = (short)strlen(etime);

	nID=id;
	nCount=nRetCount;
	
	int	db_ret, iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.GetCustNum(:nID,:var_btime,:var_etime,:nCount,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . GetCustNum ( :nID , :var_btime , :va\
r_etime , :nCount , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3482;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&var_btime;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_etime;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&nCount;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[5] = (unsigned long )34;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if(db_ret == 1403 || db_ret < 0)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
	
	if (retcode == 1)
		return DB_ERROR;
		
	nRetCount=nCount;
	return DB_SUCCESS;
}

int CDatabaseMini::Get_Cust_Info(uint32 id, char* btime, char* etime, uint32 count, uint16 start_pos, uint16 iNeedCustCount, uint8 bDirect, XT_GET_NEAR_COMM_CUST_ACK::TagCommInfo* CliInfo, uint32& nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR	db_link_name[32],var_btime[32],var_etime[32],var_account[40],var_conntime[32],var_orderid[64],var_comment[128]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

struct { unsigned short len; unsigned char arr[32]; } var_btime;

struct { unsigned short len; unsigned char arr[32]; } var_etime;

struct { unsigned short len; unsigned char arr[40]; } var_account;

struct { unsigned short len; unsigned char arr[32]; } var_conntime;

struct { unsigned short len; unsigned char arr[64]; } var_orderid;

struct { unsigned short len; unsigned char arr[128]; } var_comment;

		unsigned int nID,nMaxCount,nPos,nCount,nDirect;
		int	retcode;
		SQL_CURSOR cur_cliInfo;
	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);
	
	strncpy((char*)var_btime.arr, btime,32);
	var_btime.len = (short)strlen(btime);
	
	strncpy((char*)var_etime.arr, etime,32);
	var_etime.len = (short)strlen(etime);

	nID=id;
	nMaxCount=count;
	nPos=start_pos;
	nCount=iNeedCustCount;
	nDirect=bDirect;
	SET_INIT_NULL(var_account);
	SET_INIT_NULL(var_conntime);
	SET_INIT_NULL(var_orderid);
	SET_INIT_NULL(var_comment);
  
	int db_ret, iReconnTimes;
	uint32 iNum = 0;	
	bool bReconn;	
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
  

start:  
		/* EXEC SQL ALLOCATE :cur_cliInfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3521;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_cliInfo;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name EXECUTE
			begin
				Mini_Local_Pkg.GetCustInfo(:nID,:var_btime,:var_etime,:nMaxCount,:nPos,:nCount,:nDirect,:cur_cliInfo,:retcode);
			end;
		END-EXEC; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "begin Mini_Local_Pkg . GetCustInfo ( :nID , :var_btime , :\
var_etime , :nMaxCount , :nPos , :nCount , :nDirect , :cur_cliInfo , :retcode\
 ) ; end ;";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3540;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&nID;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&var_btime;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&var_etime;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&nMaxCount;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&nPos;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&nCount;
  sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&nDirect;
  sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         short *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned long )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (unsigned char  *)&cur_cliInfo;
  sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         short *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned long )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (unsigned char  *)&retcode;
  sqlstm.sqhstl[8] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         short *)0;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned long )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqhstv[9] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[9] = (unsigned long )34;
  sqlstm.sqhsts[9] = (         int  )0;
  sqlstm.sqindv[9] = (         short *)0;
  sqlstm.sqinds[9] = (         int  )0;
  sqlstm.sqharm[9] = (unsigned long )0;
  sqlstm.sqadto[9] = (unsigned short )0;
  sqlstm.sqtdso[9] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			/* EXEC SQL CLOSE :cur_cliInfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )3595;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_cliInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		CLog::Log("Get_Cust_Info",CLog::TYPE_DEBUG,"id:%d, db_ret:%d, retcode:%d ",id, db_ret, retcode);
		
		if(db_ret < 0 && db_ret != -1405) 
		{
			sql_error(sqlca, oraca);		
			/* EXEC SQL CLOSE :cur_cliInfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )3614;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_cliInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL FREE  :cur_cliInfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )3633;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_cliInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			return DB_ERROR;
		}

		for(;;)
		{  
			/* EXEC SQL FETCH :cur_cliInfo INTO :var_account,:var_conntime,:var_orderid,:var_comment; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )3652;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqfoff = (         int )0;
   sqlstm.sqfmod = (unsigned int )2;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_cliInfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&var_account;
   sqlstm.sqhstl[1] = (unsigned long )42;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&var_conntime;
   sqlstm.sqhstl[2] = (unsigned long )34;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&var_orderid;
   sqlstm.sqhstl[3] = (unsigned long )66;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)&var_comment;
   sqlstm.sqhstl[4] = (unsigned long )130;
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			db_ret = sqlca.sqlcode;
			if (db_ret == 1403 )
			{
				nRetCount = iNum;
				break;
			}
			if (iNum >= nRetCount)
			{
				break;
			}	
			if( db_ret < 0 && db_ret != -1405 )
			{
				sql_error(sqlca, oraca);
				nRetCount = iNum;
				break;
			}		
			SET_END_NULL(var_account);
			SET_END_NULL(var_conntime);
			SET_END_NULL(var_orderid);
			SET_END_NULL(var_comment);
			
			strncpy(CliInfo[iNum].szCommAccount,(char*)var_account.arr,MAX_CUST_ACCOUNT_LEN);					
			strncpy(CliInfo[iNum].szCommTime,(char*)var_conntime.arr,MAX_CUST_TIME_LEN);						
			strncpy(CliInfo[iNum].szTransInfo,(char*)var_orderid.arr,MAX_CUST_ORDER_LEN);	
			strncpy(CliInfo[iNum].szRemarks,(char*)var_comment.arr,MAX_CUST_REMAKE_LEN);

			++iNum;
		}
		/* EXEC SQL CLOSE :cur_cliInfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3687;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_cliInfo;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL FREE  :cur_cliInfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3706;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_cliInfo;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_SUCCESS;		
}

int CDatabaseMini::Modify_Group(uint32 iUserID, uint8 iGroupID, char *strGroupName)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR var_groupname[21]; */ 
struct { unsigned short len; unsigned char arr[21]; } var_groupname;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		unsigned int     nUserID;
		int		g_id;
		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	SET_INIT_NULL(var_groupname);
	nUserID = iUserID;
	g_id = iGroupID;
	if( strlen(strGroupName) > 20 )
		return DB_PARAM;
	var_groupname.len = (unsigned short)strlen(strGroupName);
	strncpy((char*)var_groupname.arr,strGroupName,MAX_GROUPNAME_LEN);
	 
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	int	db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.ModifyGroup(:nUserID,:g_id,:var_groupname,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . ModifyGroup ( :nUserID , :g_id , :va\
r_groupname , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3725;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nUserID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&g_id;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_groupname;
 sqlstm.sqhstl[2] = (unsigned long )23;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[4] = (unsigned long )34;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if(db_ret == 1403 || db_ret < 0)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
	
	if (retcode == DB_GROUP_EXIST)
		return DB_GROUP_EXIST;

	return DB_SUCCESS;
}

int CDatabaseMini::Delete_Group(uint32 iUserID, uint8 iGroupID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     nUserID;
		int g_id;
		int		 retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	nUserID = iUserID;
	g_id = iGroupID;
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.DeleteGroup(:nUserID,:g_id,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . DeleteGroup ( :nUserID , :g_id , :re\
tcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3760;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nUserID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&g_id;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[3] = (unsigned long )34;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if(db_ret < 0)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}  
	return DB_SUCCESS;
}

int CDatabaseMini::Save_User_Login(uint32 userid,uint32 userip, uint16 userport, int serverno)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int clientno;
		unsigned int clientip;
		int thissrvno;
		short clientport;
		int		 retcode;
		/* VARCHAR nowdate[24]; */ 
struct { unsigned short len; unsigned char arr[24]; } nowdate;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	SET_INIT_NULL(nowdate);
	
	if (userid ==0)
		return DB_PARAM;
	clientno = userid;
	clientip = userip;
	thissrvno = serverno;
	char strTime[20];
	time_t ntm;
	time(&ntm);
	struct tm *newtime;
	newtime = localtime(&ntm);
	sprintf(strTime, "%d-%d-%d", newtime->tm_year + 1900, newtime->tm_mon + 1, newtime->tm_mday);
	strncpy((char*)nowdate.arr, strTime,19);
	nowdate.len = (short)strlen((char*)nowdate.arr);	
	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.SaveUserLogin(:clientno,:clientip,:clientport,:thissrvno,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . SaveUserLogin ( :clientno , :clienti\
p , :clientport , :thissrvno , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3791;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&clientno;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&clientip;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&clientport;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(short);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&thissrvno;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[5] = (unsigned long )34;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 )
	{		
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
	
	
	return DB_SUCCESS;
}

int CDatabaseMini::Save_User_Logout(uint32 userid)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int clientno;
		int		 retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	if (userid ==0)
		return DB_PARAM;
	clientno = userid;
	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.SaveUserLogout(:clientno,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . SaveUserLogout ( :clientno , :retcod\
e ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3830;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&clientno;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 )
	{		
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
	
	return DB_SUCCESS;
}

int	CDatabaseMini::Update_All_Logout(int serverno)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		int	v_serverno;
		int	retcode;
		/* VARCHAR	db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	SET_INIT_NULL(db_link_name);
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	v_serverno = serverno;
	int	db_ret;
	int	iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:

	/* EXEC SQL AT	:db_link_name EXECUTE
		begin
			Mini_Local_Pkg.UpdateAllLogout(:v_serverno,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . UpdateAllLogout ( :v_serverno , :ret\
code ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3857;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&v_serverno;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;
	}

	if(	db_ret < 0 )
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}

	return DB_SUCCESS;
}

int CDatabaseMini::Save_User_Action(uint32 userid,XT_USER_ACTION_AUDIT_REQ::Action *action,uint16 count)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int clientno,actionno;
		int actioncount;
		int		 retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	if (userid <=0)
		return 1;

	clientno = userid;
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;	
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	
start:
	
	for(int i = 0; i < count; ++i)
	{
		actionno = action[i].OpeId;
		actioncount = action[i].count;

		/* EXEC SQL AT :db_link_name EXECUTE
			begin
				Mini_Local_Pkg.SaveUserAction(:clientno,:actionno,:actioncount,:retcode);
			end;
		END-EXEC; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "begin Mini_Local_Pkg . SaveUserAction ( :clientno , :actio\
nno , :actioncount , :retcode ) ; end ;";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3884;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&clientno;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&actionno;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&actioncount;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&retcode;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[4] = (unsigned long )34;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( db_ret < 0 )
		{		
			sql_error(sqlca, oraca);
			continue;
		}
	}
	
	return DB_SUCCESS;
}

int CDatabaseMini::Save_User_Timelen(uint32 userid, int logintm, int logouttm)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int		clientno,onlinetm;
		int		 retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	clientno = userid;
	
	CTime intm(logintm);
	CTime outtm(logouttm);

	onlinetm = 0;
	
	if ( intm.GetDay() == outtm.GetDay())
	{	
		onlinetm = (unsigned int)(logouttm - logintm)/60;	//按分钟记时
	}
	else
	{
		onlinetm = outtm.GetHour() * 60 + outtm.GetMinute();
		onlinetm = onlinetm + (24*60-intm.GetHour()*60-intm.GetMinute());
	}
	if(onlinetm ==0)
		return DB_SUCCESS;
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.SaveUserTimelen(:clientno,:onlinetm,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . SaveUserTimelen ( :clientno , :onlin\
etm , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3919;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&clientno;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&onlinetm;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[3] = (unsigned long )34;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if (db_ret < 0 && db_ret != -1405) //出错
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}

	return DB_SUCCESS;
}

int CDatabaseMini::Save_Merchant_Login(uint32 userid,int logintm)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int merchantid,thisintm;
		int		 retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	merchantid = userid;
	thisintm = logintm;
	if (userid ==0)
		return DB_PARAM;
	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.SaveMerchantLogin(:merchantid,:thisintm,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . SaveMerchantLogin ( :merchantid , :t\
hisintm , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3950;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&merchantid;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&thisintm;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[3] = (unsigned long )34;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 )
	{		
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
	/* EXEC SQL AT :db_link_name COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3981;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	return DB_SUCCESS;
}



int CDatabaseMini::Save_Merchant_Logout(uint32 userid,int logouttm)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int merchantid,thisouttm;
		int		 retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	merchantid = userid;
	thisouttm = logouttm;
	if (userid ==0)
		return DB_PARAM;
	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.SaveMerchantLogout(:merchantid,:thisouttm,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . SaveMerchantLogout ( :merchantid , :\
thisouttm , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )4000;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&merchantid;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&thisouttm;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[3] = (unsigned long )34;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	if( db_ret < 0 )
	{		
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
	/* EXEC SQL AT :db_link_name COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )4031;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	return DB_SUCCESS;
}
int CDatabaseMini::Save_User_OnlineTime(uint32 clientno,int logintm,int logouttm, int &money)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int		userid,lstouttm, thisintm, thisouttm;
		int					sendmoney, daymoney;
		int					countFlag;
		float				onlinetime, todayonlinetm;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	userid = clientno;
	thisintm = logintm;
	thisouttm = logouttm;
	sendmoney = DAY_SEND_MONEY;
	todayonlinetm = 0;
	countFlag = 0;
	money = 0;
	lstouttm = 0;
	todayonlinetm = 0;

	unsigned int onlinetm = (unsigned int)(logouttm - logintm);
	char buf[10];
	sprintf(buf, "%.1f", float(onlinetm)/60.0/60);	//六分钟记时
	float onlinetimelen = (float)atof(buf);
	if (onlinetimelen < 0.1)	//小时
		return DB_SUCCESS;
	onlinetime = onlinetimelen;
		
	int db_ret;
	int iReconnTimes;
	bool bReconn;	
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	/* EXEC SQL AT :db_link_name 
		SELECT LstLogoutTime, CountFlag, TodayLineTime INTO :lstouttm, :countFlag, :todayonlinetm 
		FROM IM_USERSTAT
		WHERE UserId=:userid; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select LstLogoutTime ,CountFlag ,TodayLineTime into :b1,:b2\
,:b3  from IM_USERSTAT where UserId=:b4";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )4050;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&lstouttm;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&countFlag;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&todayonlinetm;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(float);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&userid;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[4] = (unsigned long )34;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if (db_ret < 0 && db_ret != -1405) //出错
	{
		sql_error(sqlca, oraca);
		money = 0;
		return DB_ERROR;
	}
	if (db_ret == 1403)//没有该用户
	{
		if (onlinetimelen >= 2.0)
		{
			/* EXEC SQL AT :db_link_name INSERT INTO IM_USERSTAT (UserId,LstLoginTime,LstLogoutTime,TotalLineTime,
						 LoginCount, CountFlag,TotalMoney, TodayLineTime)
				values(:userid, :thisintm,:thisouttm,:onlinetime,1, 1, :sendmoney, :onlinetime); */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "insert into IM_USERSTAT (UserId,LstLoginTime,LstLogoutTim\
e,TotalLineTime,LoginCount,CountFlag,TotalMoney,TodayLineTime) values (:b1,:b\
2,:b3,:b4,1,1,:b5,:b4)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )4085;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&userid;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&thisintm;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&thisouttm;
   sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&onlinetime;
   sqlstm.sqhstl[3] = (unsigned long )sizeof(float);
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)&sendmoney;
   sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (unsigned char  *)&onlinetime;
   sqlstm.sqhstl[5] = (unsigned long )sizeof(float);
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         short *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned long )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqhstv[6] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[6] = (unsigned long )34;
   sqlstm.sqhsts[6] = (         int  )0;
   sqlstm.sqindv[6] = (         short *)0;
   sqlstm.sqinds[6] = (         int  )0;
   sqlstm.sqharm[6] = (unsigned long )0;
   sqlstm.sqadto[6] = (unsigned short )0;
   sqlstm.sqtdso[6] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			db_ret = sqlca.sqlcode;
			if (conn_error(db_ret))
			{
				iReconnTimes=0;
				bReconn=ConnectDB();
				while(!bReconn)
				{
					iReconnTimes++;
					sleep(iReconnTimes%10);
					bReconn=ConnectDB();
				}
				goto start;				
			}
			
			if (db_ret < 0 ) //出错
			{
				sql_error(sqlca, oraca);
				money = 0;
				return DB_ERROR;
			}
			/* EXEC SQL AT :db_link_name COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )4128;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			money = DAY_SEND_MONEY;		
		}
		else
		{		
			/* EXEC SQL AT :db_link_name INSERT INTO  IM_USERSTAT (UserId,LstLoginTime,LstLogoutTime,TotalLineTime,
						 LoginCount, TodayLineTime)
				values(:userid, :thisintm,:thisouttm,:onlinetime,1, :onlinetime); */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "insert into IM_USERSTAT (UserId,LstLoginTime,LstLogoutTim\
e,TotalLineTime,LoginCount,TodayLineTime) values (:b1,:b2,:b3,:b4,1,:b4)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )4147;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&userid;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&thisintm;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&thisouttm;
   sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&onlinetime;
   sqlstm.sqhstl[3] = (unsigned long )sizeof(float);
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)&onlinetime;
   sqlstm.sqhstl[4] = (unsigned long )sizeof(float);
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[5] = (unsigned long )34;
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         short *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned long )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			db_ret = sqlca.sqlcode;
			if (db_ret < 0 ) //出错
			{
				return DB_ERROR;
			}
			/* EXEC SQL AT :db_link_name COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )4186;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		}

		return DB_SUCCESS;	
	}

	if (lstouttm == 0)
		lstouttm = logouttm - 24*60*60;
	
	//用户登录过
	//上次登出时间与本次登出时间比较

	CTime lst(lstouttm);	//上次登出时间
	CTime logout(logouttm);	//本次登出时间
	CTime login(logintm);	//本次登录时间


	//当天非第一次登出
	if (logout.GetYear() == lst.GetYear() && logout.GetMonth() == lst.GetMonth() && logout.GetDay() == lst.GetDay())//年月日相同
	{
		if (countFlag == 0)//当日未充金币
		{
			float tmlen = todayonlinetm + onlinetimelen;
			if ( tmlen >= 2.0)//当日在线时长超过两小时
			{
				/* EXEC SQL AT :db_link_name UPDATE IM_USERSTAT SET lstLoginTime=:thisintm,LstLogoutTime=:thisouttm,
					TotalLineTime = TotalLineTime+:onlinetime, LoginCount=LoginCount+1, 
					CountFlag = 1,TotalMoney= TotalMoney + :sendmoney, TodayLineTime = TodayLineTime + :onlinetime 
					WHERE UserId=:userid; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "update IM_USERSTAT  set lstLoginTime=:b1,LstLogoutTime=:\
b2,TotalLineTime=(TotalLineTime+:b3),LoginCount=(LoginCount+1),CountFlag=1,To\
talMoney=(TotalMoney+:b4),TodayLineTime=(TodayLineTime+:b3) where UserId=:b6";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4205;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&thisintm;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&thisouttm;
    sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (unsigned char  *)&onlinetime;
    sqlstm.sqhstl[2] = (unsigned long )sizeof(float);
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         short *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned long )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (unsigned char  *)&sendmoney;
    sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         short *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned long )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (unsigned char  *)&onlinetime;
    sqlstm.sqhstl[4] = (unsigned long )sizeof(float);
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         short *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned long )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqhstv[5] = (unsigned char  *)&userid;
    sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[5] = (         int  )0;
    sqlstm.sqindv[5] = (         short *)0;
    sqlstm.sqinds[5] = (         int  )0;
    sqlstm.sqharm[5] = (unsigned long )0;
    sqlstm.sqadto[5] = (unsigned short )0;
    sqlstm.sqtdso[5] = (unsigned short )0;
    sqlstm.sqhstv[6] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[6] = (unsigned long )34;
    sqlstm.sqhsts[6] = (         int  )0;
    sqlstm.sqindv[6] = (         short *)0;
    sqlstm.sqinds[6] = (         int  )0;
    sqlstm.sqharm[6] = (unsigned long )0;
    sqlstm.sqadto[6] = (unsigned short )0;
    sqlstm.sqtdso[6] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				db_ret = sqlca.sqlcode;
				if (db_ret < 0 ) //出错
				{
					return DB_ERROR;
				}
				/* EXEC SQL AT :db_link_name COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4248;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[0] = (unsigned long )34;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

				
				money = DAY_SEND_MONEY;	
			}
			else//不足两小时
			{
				/* EXEC SQL AT :db_link_name UPDATE IM_USERSTAT SET lstLoginTime=:thisintm,LstLogoutTime=:thisouttm,
					TotalLineTime = TotalLineTime+:onlinetime, LoginCount=LoginCount+1, 
					CountFlag = 0, TodayLineTime = TodayLineTime + :onlinetime WHERE UserId=:userid; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "update IM_USERSTAT  set lstLoginTime=:b1,LstLogoutTime=:\
b2,TotalLineTime=(TotalLineTime+:b3),LoginCount=(LoginCount+1),CountFlag=0,To\
dayLineTime=(TodayLineTime+:b3) where UserId=:b5";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4267;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&thisintm;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&thisouttm;
    sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (unsigned char  *)&onlinetime;
    sqlstm.sqhstl[2] = (unsigned long )sizeof(float);
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         short *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned long )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (unsigned char  *)&onlinetime;
    sqlstm.sqhstl[3] = (unsigned long )sizeof(float);
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         short *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned long )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (unsigned char  *)&userid;
    sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         short *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned long )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[5] = (unsigned long )34;
    sqlstm.sqhsts[5] = (         int  )0;
    sqlstm.sqindv[5] = (         short *)0;
    sqlstm.sqinds[5] = (         int  )0;
    sqlstm.sqharm[5] = (unsigned long )0;
    sqlstm.sqadto[5] = (unsigned short )0;
    sqlstm.sqtdso[5] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				db_ret = sqlca.sqlcode;
				if (db_ret < 0 ) //出错
				{
					return DB_ERROR;
				}
				/* EXEC SQL AT :db_link_name COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4306;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[0] = (unsigned long )34;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			}			
		}
		else//已充金币
		{
			/* EXEC SQL AT :db_link_name UPDATE IM_USERSTAT SET lstLoginTime=:thisintm,LstLogoutTime=:thisouttm,
				TotalLineTime = TotalLineTime+:onlinetime, LoginCount=LoginCount+1, 
				TodayLineTime = TodayLineTime + :onlinetime WHERE UserId=:userid; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "update IM_USERSTAT  set lstLoginTime=:b1,LstLogoutTime=:b\
2,TotalLineTime=(TotalLineTime+:b3),LoginCount=(LoginCount+1),TodayLineTime=(\
TodayLineTime+:b3) where UserId=:b5";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )4325;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&thisintm;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&thisouttm;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&onlinetime;
   sqlstm.sqhstl[2] = (unsigned long )sizeof(float);
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&onlinetime;
   sqlstm.sqhstl[3] = (unsigned long )sizeof(float);
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)&userid;
   sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[5] = (unsigned long )34;
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         short *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned long )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			db_ret = sqlca.sqlcode;
			if (db_ret < 0 ) //出错
			{
				return DB_ERROR;
			}
			/* EXEC SQL AT :db_link_name COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )4364;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

				
		}
		return DB_SUCCESS;
	
	}
	else//与上次登出年月日有一个不同 当天第一次登出。
	{
		if (logout.GetDay() == login.GetDay())//同一天登录登出,表示当天第一次上线离线(丢弃在线一个月)
		{
			if ( onlinetimelen >= 2.0)//本次在线时长超过两小时
			{
				/* EXEC SQL AT :db_link_name UPDATE IM_USERSTAT SET lstLoginTime=:thisintm,LstLogoutTime=:thisouttm,
					TotalLineTime = TotalLineTime+:onlinetime, LoginCount=LoginCount+1, 
					CountFlag = 1,TotalMoney= TotalMoney + :sendmoney, TodayLineTime =  :onlinetime 
					WHERE UserId=:userid; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "update IM_USERSTAT  set lstLoginTime=:b1,LstLogoutTime=:\
b2,TotalLineTime=(TotalLineTime+:b3),LoginCount=(LoginCount+1),CountFlag=1,To\
talMoney=(TotalMoney+:b4),TodayLineTime=:b3 where UserId=:b6";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4383;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&thisintm;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&thisouttm;
    sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (unsigned char  *)&onlinetime;
    sqlstm.sqhstl[2] = (unsigned long )sizeof(float);
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         short *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned long )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (unsigned char  *)&sendmoney;
    sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         short *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned long )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (unsigned char  *)&onlinetime;
    sqlstm.sqhstl[4] = (unsigned long )sizeof(float);
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         short *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned long )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqhstv[5] = (unsigned char  *)&userid;
    sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[5] = (         int  )0;
    sqlstm.sqindv[5] = (         short *)0;
    sqlstm.sqinds[5] = (         int  )0;
    sqlstm.sqharm[5] = (unsigned long )0;
    sqlstm.sqadto[5] = (unsigned short )0;
    sqlstm.sqtdso[5] = (unsigned short )0;
    sqlstm.sqhstv[6] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[6] = (unsigned long )34;
    sqlstm.sqhsts[6] = (         int  )0;
    sqlstm.sqindv[6] = (         short *)0;
    sqlstm.sqinds[6] = (         int  )0;
    sqlstm.sqharm[6] = (unsigned long )0;
    sqlstm.sqadto[6] = (unsigned short )0;
    sqlstm.sqtdso[6] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				db_ret = sqlca.sqlcode;
				if (db_ret < 0 ) //出错
				{
					return DB_ERROR;
				}
				/* EXEC SQL AT :db_link_name COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4426;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[0] = (unsigned long )34;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

				
				money = DAY_SEND_MONEY;		
			}			
			else//不足两小时
			{
				/* EXEC SQL AT :db_link_name UPDATE IM_USERSTAT SET lstLoginTime=:thisintm,LstLogoutTime=:thisouttm,
					TotalLineTime = TotalLineTime+:onlinetime, LoginCount=LoginCount+1, 
					CountFlag = 0, TodayLineTime = :onlinetime WHERE UserId=:userid; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "update IM_USERSTAT  set lstLoginTime=:b1,LstLogoutTime=:\
b2,TotalLineTime=(TotalLineTime+:b3),LoginCount=(LoginCount+1),CountFlag=0,To\
dayLineTime=:b3 where UserId=:b5";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4445;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&thisintm;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&thisouttm;
    sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (unsigned char  *)&onlinetime;
    sqlstm.sqhstl[2] = (unsigned long )sizeof(float);
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         short *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned long )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (unsigned char  *)&onlinetime;
    sqlstm.sqhstl[3] = (unsigned long )sizeof(float);
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         short *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned long )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (unsigned char  *)&userid;
    sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         short *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned long )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[5] = (unsigned long )34;
    sqlstm.sqhsts[5] = (         int  )0;
    sqlstm.sqindv[5] = (         short *)0;
    sqlstm.sqinds[5] = (         int  )0;
    sqlstm.sqharm[5] = (unsigned long )0;
    sqlstm.sqadto[5] = (unsigned short )0;
    sqlstm.sqtdso[5] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				db_ret = sqlca.sqlcode;
				if (db_ret < 0 ) //出错
				{
					return DB_ERROR;
				}
				/* EXEC SQL AT :db_link_name COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4484;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[0] = (unsigned long )34;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

				
			}
			return DB_SUCCESS;
		}
		else//上下线不在同一天。
		{
			char buf[10];
			sprintf(buf, "%.1f", login.GetMinute()/60.0);
			float fintm = (float)atof(buf);
			fintm = float(24.0 -(float)login.GetHour()-fintm);//登录当天在线时长
			sprintf(buf, "%.1f", logout.GetMinute()/60.0);
			float fouttm = (float)atof(buf);
			fouttm = (float)logout.GetHour()+fouttm;//登出当天在线时长
			
			if (fintm >= 2.0)//登录当天在线时长超过两小时
			{
				if (login.GetYear() == lst.GetYear() && login.GetMonth() == lst.GetMonth() && login.GetDay() == lst.GetDay())//当天有登出过
				{
					if (countFlag == 0)//前一天未充金币
					{				
						/* EXEC SQL AT :db_link_name UPDATE IM_USERSTAT SET TotalMoney= TotalMoney + :sendmoney 
						WHERE UserId=:userid; */ 

{
      struct sqlexd sqlstm;
      sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 15;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "update IM_USERSTAT  set TotalMoney=(TotalMoney+:b1) wh\
ere UserId=:b2";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )4503;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)4352;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (unsigned char  *)&sendmoney;
      sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         short *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned long )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (unsigned char  *)&userid;
      sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         short *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned long )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
      sqlstm.sqhstl[2] = (unsigned long )34;
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         short *)0;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned long )0;
      sqlstm.sqadto[2] = (unsigned short )0;
      sqlstm.sqtdso[2] = (unsigned short )0;
      sqlstm.sqphsv = sqlstm.sqhstv;
      sqlstm.sqphsl = sqlstm.sqhstl;
      sqlstm.sqphss = sqlstm.sqhsts;
      sqlstm.sqpind = sqlstm.sqindv;
      sqlstm.sqpins = sqlstm.sqinds;
      sqlstm.sqparm = sqlstm.sqharm;
      sqlstm.sqparc = sqlstm.sqharc;
      sqlstm.sqpadto = sqlstm.sqadto;
      sqlstm.sqptdso = sqlstm.sqtdso;
      sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


						db_ret = sqlca.sqlcode;
						if (db_ret < 0 ) //出错
						{
							return DB_ERROR;
						}
						/* EXEC SQL AT :db_link_name COMMIT; */ 

{
      struct sqlexd sqlstm;
      sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 15;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )4530;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)4352;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
      sqlstm.sqhstl[0] = (unsigned long )34;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         short *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned long )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqphsv = sqlstm.sqhstv;
      sqlstm.sqphsl = sqlstm.sqhstl;
      sqlstm.sqphss = sqlstm.sqhsts;
      sqlstm.sqpind = sqlstm.sqindv;
      sqlstm.sqpins = sqlstm.sqinds;
      sqlstm.sqparm = sqlstm.sqharm;
      sqlstm.sqparc = sqlstm.sqharc;
      sqlstm.sqpadto = sqlstm.sqadto;
      sqlstm.sqptdso = sqlstm.sqtdso;
      sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


						money = DAY_SEND_MONEY;
					}
				}
				else
				{
					/* EXEC SQL AT :db_link_name UPDATE IM_USERSTAT SET TotalMoney= TotalMoney + :sendmoney 
					WHERE UserId=:userid; */ 

{
     struct sqlexd sqlstm;
     sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
     sqlstm.sqlvsn = 12;
     sqlstm.arrsiz = 15;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.stmt = "update IM_USERSTAT  set TotalMoney=(TotalMoney+:b1) whe\
re UserId=:b2";
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )4549;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)4352;
     sqlstm.occurs = (unsigned int  )0;
     sqlstm.sqhstv[0] = (unsigned char  *)&sendmoney;
     sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
     sqlstm.sqhsts[0] = (         int  )0;
     sqlstm.sqindv[0] = (         short *)0;
     sqlstm.sqinds[0] = (         int  )0;
     sqlstm.sqharm[0] = (unsigned long )0;
     sqlstm.sqadto[0] = (unsigned short )0;
     sqlstm.sqtdso[0] = (unsigned short )0;
     sqlstm.sqhstv[1] = (unsigned char  *)&userid;
     sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
     sqlstm.sqhsts[1] = (         int  )0;
     sqlstm.sqindv[1] = (         short *)0;
     sqlstm.sqinds[1] = (         int  )0;
     sqlstm.sqharm[1] = (unsigned long )0;
     sqlstm.sqadto[1] = (unsigned short )0;
     sqlstm.sqtdso[1] = (unsigned short )0;
     sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
     sqlstm.sqhstl[2] = (unsigned long )34;
     sqlstm.sqhsts[2] = (         int  )0;
     sqlstm.sqindv[2] = (         short *)0;
     sqlstm.sqinds[2] = (         int  )0;
     sqlstm.sqharm[2] = (unsigned long )0;
     sqlstm.sqadto[2] = (unsigned short )0;
     sqlstm.sqtdso[2] = (unsigned short )0;
     sqlstm.sqphsv = sqlstm.sqhstv;
     sqlstm.sqphsl = sqlstm.sqhstl;
     sqlstm.sqphss = sqlstm.sqhsts;
     sqlstm.sqpind = sqlstm.sqindv;
     sqlstm.sqpins = sqlstm.sqinds;
     sqlstm.sqparm = sqlstm.sqharm;
     sqlstm.sqparc = sqlstm.sqharc;
     sqlstm.sqpadto = sqlstm.sqadto;
     sqlstm.sqptdso = sqlstm.sqtdso;
     sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


					db_ret = sqlca.sqlcode;
					if (db_ret < 0 ) //出错
					{
						return DB_ERROR;
					}
					/* EXEC SQL AT :db_link_name COMMIT; */ 

{
     struct sqlexd sqlstm;
     sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
     sqlstm.sqlvsn = 12;
     sqlstm.arrsiz = 15;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )4576;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)4352;
     sqlstm.occurs = (unsigned int  )0;
     sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
     sqlstm.sqhstl[0] = (unsigned long )34;
     sqlstm.sqhsts[0] = (         int  )0;
     sqlstm.sqindv[0] = (         short *)0;
     sqlstm.sqinds[0] = (         int  )0;
     sqlstm.sqharm[0] = (unsigned long )0;
     sqlstm.sqadto[0] = (unsigned short )0;
     sqlstm.sqtdso[0] = (unsigned short )0;
     sqlstm.sqphsv = sqlstm.sqhstv;
     sqlstm.sqphsl = sqlstm.sqhstl;
     sqlstm.sqphss = sqlstm.sqhsts;
     sqlstm.sqpind = sqlstm.sqindv;
     sqlstm.sqpins = sqlstm.sqinds;
     sqlstm.sqparm = sqlstm.sqharm;
     sqlstm.sqparc = sqlstm.sqharc;
     sqlstm.sqpadto = sqlstm.sqadto;
     sqlstm.sqptdso = sqlstm.sqtdso;
     sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


					money = DAY_SEND_MONEY;

				}
			}
			else//22点以后登录
			{
				if (login.GetYear() == lst.GetYear() && login.GetMonth() == lst.GetMonth() && login.GetDay() == lst.GetDay())//前一天有登出过
				{
					if (countFlag == 0)//前一天未充金币
					{
						float tmlen = todayonlinetm + fintm;
						if ( tmlen >= 2.0)//前一天在线时长累计超过两小时
						{
							/* EXEC SQL AT :db_link_name UPDATE IM_USERSTAT SET TotalMoney= TotalMoney + :sendmoney 
							WHERE UserId=:userid; */ 

{
       struct sqlexd sqlstm;
       sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 15;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "update IM_USERSTAT  set TotalMoney=(TotalMoney+:b1) w\
here UserId=:b2";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )4595;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)4352;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (unsigned char  *)&sendmoney;
       sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         short *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned long )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (unsigned char  *)&userid;
       sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[1] = (         int  )0;
       sqlstm.sqindv[1] = (         short *)0;
       sqlstm.sqinds[1] = (         int  )0;
       sqlstm.sqharm[1] = (unsigned long )0;
       sqlstm.sqadto[1] = (unsigned short )0;
       sqlstm.sqtdso[1] = (unsigned short )0;
       sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
       sqlstm.sqhstl[2] = (unsigned long )34;
       sqlstm.sqhsts[2] = (         int  )0;
       sqlstm.sqindv[2] = (         short *)0;
       sqlstm.sqinds[2] = (         int  )0;
       sqlstm.sqharm[2] = (unsigned long )0;
       sqlstm.sqadto[2] = (unsigned short )0;
       sqlstm.sqtdso[2] = (unsigned short )0;
       sqlstm.sqphsv = sqlstm.sqhstv;
       sqlstm.sqphsl = sqlstm.sqhstl;
       sqlstm.sqphss = sqlstm.sqhsts;
       sqlstm.sqpind = sqlstm.sqindv;
       sqlstm.sqpins = sqlstm.sqinds;
       sqlstm.sqparm = sqlstm.sqharm;
       sqlstm.sqparc = sqlstm.sqharc;
       sqlstm.sqpadto = sqlstm.sqadto;
       sqlstm.sqptdso = sqlstm.sqtdso;
       sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


							db_ret = sqlca.sqlcode;
							if (db_ret < 0 ) //出错
							{
								return DB_ERROR;
							}
							/* EXEC SQL AT :db_link_name COMMIT; */ 

{
       struct sqlexd sqlstm;
       sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 15;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )4622;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)4352;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
       sqlstm.sqhstl[0] = (unsigned long )34;
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         short *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned long )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqphsv = sqlstm.sqhstv;
       sqlstm.sqphsl = sqlstm.sqhstl;
       sqlstm.sqphss = sqlstm.sqhsts;
       sqlstm.sqpind = sqlstm.sqindv;
       sqlstm.sqpins = sqlstm.sqinds;
       sqlstm.sqparm = sqlstm.sqharm;
       sqlstm.sqparc = sqlstm.sqharc;
       sqlstm.sqpadto = sqlstm.sqadto;
       sqlstm.sqptdso = sqlstm.sqtdso;
       sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


							money = DAY_SEND_MONEY;//登入当天发送的金币										
						}					
					}						
				}
			}


			float tmlen = onlinetimelen-fintm-fouttm;//除登入登出天在线时间
			
			int day = int(tmlen)/24;//登录登出间隔天数
			if (day != 0)
			{
				daymoney = sendmoney * day;
				/* EXEC SQL AT :db_link_name UPDATE IM_USERSTAT SET TotalMoney= TotalMoney + :daymoney 
				WHERE UserId=:userid; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "update IM_USERSTAT  set TotalMoney=(TotalMoney+:b1) wher\
e UserId=:b2";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4641;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&daymoney;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&userid;
    sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[2] = (unsigned long )34;
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         short *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned long )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				db_ret = sqlca.sqlcode;
				if (db_ret < 0 ) //出错
				{
					return DB_SUCCESS;
				}
				/* EXEC SQL AT :db_link_name COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4668;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[0] = (unsigned long )34;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				money = money  + daymoney;
				
			}
			onlinetime = fouttm;
			if( fouttm >= 2.0)//登出当天在线时长超过两小时
			{
				/* EXEC SQL AT :db_link_name UPDATE IM_USERSTAT SET lstLoginTime=:thisintm,LstLogoutTime=:thisouttm,
					TotalLineTime = TotalLineTime+:onlinetime, LoginCount=LoginCount+1, 
					CountFlag = 1,TotalMoney= TotalMoney + :sendmoney, TodayLineTime =  :onlinetime 
					WHERE UserId=:userid; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "update IM_USERSTAT  set lstLoginTime=:b1,LstLogoutTime=:\
b2,TotalLineTime=(TotalLineTime+:b3),LoginCount=(LoginCount+1),CountFlag=1,To\
talMoney=(TotalMoney+:b4),TodayLineTime=:b3 where UserId=:b6";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4687;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&thisintm;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&thisouttm;
    sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (unsigned char  *)&onlinetime;
    sqlstm.sqhstl[2] = (unsigned long )sizeof(float);
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         short *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned long )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (unsigned char  *)&sendmoney;
    sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         short *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned long )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (unsigned char  *)&onlinetime;
    sqlstm.sqhstl[4] = (unsigned long )sizeof(float);
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         short *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned long )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqhstv[5] = (unsigned char  *)&userid;
    sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[5] = (         int  )0;
    sqlstm.sqindv[5] = (         short *)0;
    sqlstm.sqinds[5] = (         int  )0;
    sqlstm.sqharm[5] = (unsigned long )0;
    sqlstm.sqadto[5] = (unsigned short )0;
    sqlstm.sqtdso[5] = (unsigned short )0;
    sqlstm.sqhstv[6] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[6] = (unsigned long )34;
    sqlstm.sqhsts[6] = (         int  )0;
    sqlstm.sqindv[6] = (         short *)0;
    sqlstm.sqinds[6] = (         int  )0;
    sqlstm.sqharm[6] = (unsigned long )0;
    sqlstm.sqadto[6] = (unsigned short )0;
    sqlstm.sqtdso[6] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				db_ret = sqlca.sqlcode;
				if (db_ret < 0 ) //出错
				{
					return DB_SUCCESS;
				}
				/* EXEC SQL AT :db_link_name COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4730;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[0] = (unsigned long )34;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

				
				money = money  + DAY_SEND_MONEY;
			}			
			else//不足两小时
			{
				/* EXEC SQL AT :db_link_name UPDATE IM_USERSTAT SET lstLoginTime=:thisintm,LstLogoutTime=:thisouttm,
					TotalLineTime = TotalLineTime+:onlinetime, LoginCount=LoginCount+1, 
					CountFlag = 0, TodayLineTime = :onlinetime WHERE UserId=:userid; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "update IM_USERSTAT  set lstLoginTime=:b1,LstLogoutTime=:\
b2,TotalLineTime=(TotalLineTime+:b3),LoginCount=(LoginCount+1),CountFlag=0,To\
dayLineTime=:b3 where UserId=:b5";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4749;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&thisintm;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&thisouttm;
    sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (unsigned char  *)&onlinetime;
    sqlstm.sqhstl[2] = (unsigned long )sizeof(float);
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         short *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned long )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (unsigned char  *)&onlinetime;
    sqlstm.sqhstl[3] = (unsigned long )sizeof(float);
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         short *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned long )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (unsigned char  *)&userid;
    sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         short *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned long )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[5] = (unsigned long )34;
    sqlstm.sqhsts[5] = (         int  )0;
    sqlstm.sqindv[5] = (         short *)0;
    sqlstm.sqinds[5] = (         int  )0;
    sqlstm.sqharm[5] = (unsigned long )0;
    sqlstm.sqadto[5] = (unsigned short )0;
    sqlstm.sqtdso[5] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				db_ret = sqlca.sqlcode;
				if (db_ret < 0 ) //出错
				{
					return DB_SUCCESS;
				}
				/* EXEC SQL AT :db_link_name COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )4788;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[0] = (unsigned long )34;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

				
			}
		}
	}
	
	return DB_SUCCESS;	
}

int CDatabaseMini::Save_User_RecvFamilyMsgID(uint32 uid, uint32 cid, uint32 mid)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int  clusterid,clientid,msgid;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

  
	clusterid = cid;
	clientid=uid;
	msgid = mid;
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	int db_ret;
	int iReconnTimes;
	bool bReconn;	
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	

start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.SaveUserRecvFamilyMsg(:clusterid,:clientid,:msgid,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . SaveUserRecvFamilyMsg ( :clusterid ,\
 :clientid , :msgid , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )4807;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&clusterid;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&clientid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&msgid;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[4] = (unsigned long )34;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	

	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 )
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}

	return DB_SUCCESS;
}

   
int CDatabaseMini::Get_User_FamilyMsgID(uint32 userID, uint32 familyID, uint32 &msgID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int      clusterid,mid,clientno;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int	 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 


	clientno=userID;
	clusterid = familyID;   
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.GetUserFamilyMsgID(:clientno,:clusterid,:mid,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . GetUserFamilyMsgID ( :clientno , :cl\
usterid , :mid , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )4842;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&clientno;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&clusterid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&mid;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[4] = (unsigned long )34;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	

	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 && db_ret != -1405)
	{
		mid = 0;
		sql_error(sqlca, oraca);
	} 
	
	if (db_ret == 1403 || retcode == 1403 || db_ret == -1405)
		mid = 0;
		
	msgID = mid;
	return DB_SUCCESS;
}

int CDatabaseMini::Delete_All_Offline_Msg(uint32 userID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     recv;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	recv = userID;
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.DeleteAllOfflineMsg(:recv,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . DeleteAllOfflineMsg ( :recv , :retco\
de ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )4877;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&recv;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 && db_ret != -1405)
	{
		sql_error(sqlca, oraca);
	} 
	return DB_SUCCESS;
}

int CDatabaseMini::Get_Family_Offline_Msg(uint32 userID, uint32 familyID, uint32 &msgID, ST_OFFLINE_MSG *pFamilyMsg, uint8 &nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		long_varraw *lvr;
		unsigned int     origin,time,msglen,msgid,int_tmp;
		short   msgtype;
		OCIBlobLocator *blob;
		unsigned int amt,offset,bufsize,clusid,msgno;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);	
		
	amt = 0; offset=1;bufsize = MAX_MSG_LEN;
	clusid = familyID;
	msgid = msgID;
	unsigned int nowmid=msgid;
	int db_ret;
	msgID = 0;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	

start: 
	if (nRetCount == 0)
	{
		/* EXEC SQL AT :db_link_name SELECT COUNT(MsgId) INTO :int_tmp
			FROM  IM_CLUSTERTALKMSG 
			WHERE ClusterId= :clusid AND MsgId > :msgid; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select count(MsgId) into :b1  from IM_CLUSTERTALKMSG where\
 (ClusterId=:b2 and MsgId>:b3)";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )4904;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&clusid;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&msgid;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[3] = (unsigned long )34;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( db_ret < 0 )
		{
			sql_error(sqlca, oraca);
			return DB_ERROR;
		}
		nRetCount = int_tmp;
		return DB_SUCCESS;
	}	
	
	/* EXEC SQL AT :db_link_name DECLARE cur_getfamilymsg CURSOR FOR
		SELECT MsgId,SENDER,SENDTIME,MSGTYPE,MSGLEN,MSG 
		FROM  IM_CLUSTERTALKMSG WHERE ClusterId= :clusid AND MsgId > :msgid ORDER BY MsgId; */ 

	
	/* EXEC SQL OPEN cur_getfamilymsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0120;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )4935;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&clusid;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&msgid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		/* EXEC SQL CLOSE cur_getfamilymsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )4962;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0  && db_ret != -1405)
	{
		
		sql_error(sqlca, oraca);
		/* EXEC SQL CLOSE cur_getfamilymsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )4981;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}
  
	/* EXEC SQL AT :db_link_name ALLOCATE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5000;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		/* EXEC SQL CLOSE cur_getfamilymsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5023;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if (db_ret == 1403 || (db_ret < 0  && db_ret != -1405 ))
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5042;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL CLOSE cur_getfamilymsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5065;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}
 
	lvr = (long_varraw *)malloc(sizeof(unsigned int) + bufsize);
	lvr->len = bufsize;
	
	int iNum = 0;
	for(;;)
	{  
		msglen=0;
		amt = 0; offset=1;bufsize = MAX_MSG_LEN;
		/* EXEC SQL FETCH cur_getfamilymsg INTO :msgno,:origin,:time,:msgtype,:msglen,:blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5084;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&msgno;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&origin;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&time;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&msgtype;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&msglen;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&blob;
  sqlstm.sqhstl[5] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[6] = (unsigned long )34;
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         short *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned long )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			/* EXEC SQL CLOSE cur_getfamilymsg; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5127;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if ((db_ret < 0 && db_ret != -1405)|| db_ret == 1403 || iNum >= nRetCount)
		{
			if (db_ret != 1403)
				sql_error(sqlca, oraca);
			
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5146;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL CLOSE cur_getfamilymsg; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5169;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			free(lvr);
			msgID = nowmid;
			nRetCount = iNum;
			return DB_SUCCESS;
		} 

		pFamilyMsg[iNum].msgid		= msgno;
		pFamilyMsg[iNum].recvID		= userID;
		pFamilyMsg[iNum].sendID		= origin;
		pFamilyMsg[iNum].tm			= (unsigned int)time;
		pFamilyMsg[iNum].msgtype	= (unsigned short)msgtype;  
		pFamilyMsg[iNum].msglen		= msglen + 1;

 
		/* EXEC SQL AT :db_link_name LOB READ :amt FROM :blob AT :offset INTO :lvr WITH LENGTH :msglen; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5188;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&amt;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&blob;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&offset;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)lvr;
  sqlstm.sqhstl[3] = (unsigned long )4;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&msglen;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[5] = (unsigned long )34;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		if (sqlca.sqlcode == 1403 )
		{
			memcpy((char*)pFamilyMsg[iNum].msg,lvr->buf,msglen + 1);
			memset(lvr->buf, 0, msglen);
			nowmid = msgno;
		}
		++iNum;
	}

	/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5227;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL CLOSE cur_getfamilymsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5250;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	free(lvr);
	return DB_SUCCESS;
}

int CDatabaseMini::Get_Family_Offline_SysMsg(uint32 userID, uint32 familyID,ST_OFFLINE_MSG *pFamilyMsg, uint8 &nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		long_varraw *lvr;
		unsigned int     sendno,time,msglen, int_tmp;
		short   msgtype;
		OCIBlobLocator *blob;
		unsigned int amt,offset,bufsize;
		unsigned int clusid;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
		
	//unsigned int nowmid=0;
	amt = 0; offset=1;
	bufsize = MAX_MSG_LEN;
	clusid = familyID;
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:  
	if (nRetCount == 0)
	{
		/* EXEC SQL AT :db_link_name SELECT COUNT(MsgId) INTO :int_tmp 
			FROM IM_CLUSTERSYSMSG
			WHERE ClusterId= :clusid; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select count(MsgId) into :b1  from IM_CLUSTERSYSMSG where \
ClusterId=:b2";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5269;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&clusid;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( db_ret < 0 )
		{
			sql_error(sqlca, oraca);
			return DB_ERROR;
		}
		nRetCount = int_tmp;
		return DB_SUCCESS;
	}			

	/* EXEC SQL AT :db_link_name DECLARE cur_familysysmsg CURSOR FOR
		SELECT MsgId,SendTime,MsgType,MsgLen,MSG 
		FROM   IM_CLUSTERSYSMSG WHERE ClusterId= :clusid; */ 

	/* EXEC SQL OPEN cur_familysysmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0122;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5296;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&clusid;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5319;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 )
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5338;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}
	
	/* EXEC SQL AT :db_link_name ALLOCATE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5357;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5380;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if (db_ret == 1403 || db_ret < 0)
	{
		if (db_ret != 1403)
			sql_error(sqlca, oraca);
		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5399;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5422;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}
	
	lvr = (long_varraw *)malloc(sizeof(unsigned int) + bufsize);
	lvr->len = bufsize;
	
	int iNum = 0;
	for(;;)
	{  
		amt = 0; offset=1;bufsize = MAX_MSG_LEN;
		/* EXEC SQL FETCH cur_familysysmsg INTO :sendno,:time,:msgtype,:msglen,:blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5441;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&sendno;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&time;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&msgtype;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&msglen;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&blob;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[5] = (unsigned long )34;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5480;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if ( db_ret < 0 || db_ret == 1403 || iNum >= nRetCount)
		{
			if (db_ret != 1403)
				sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5499;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			free(lvr);
			/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5522;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			if (iNum > 0)
			{
				/* EXEC SQL AT :db_link_name DELETE FROM IM_CLUSTERSYSMSG WHERE  ClusterId = :clusid; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "delete  from IM_CLUSTERSYSMSG  where ClusterId=:b1";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )5541;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&clusid;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[1] = (unsigned long )34;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

//清除验证记录
				/* EXEC SQL AT :db_link_name COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )5564;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[0] = (unsigned long )34;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			}
			nRetCount = iNum;
			return DB_SUCCESS;
		} 
		 
		pFamilyMsg[iNum].recvID		= userID;
		pFamilyMsg[iNum].sendID		= sendno;
		pFamilyMsg[iNum].tm			= (unsigned int)time;
		pFamilyMsg[iNum].msgtype	= (unsigned short)msgtype;  
		pFamilyMsg[iNum].msglen		= msglen + 1;

 
		/* EXEC SQL AT :db_link_name LOB READ :amt FROM :blob AT :offset INTO :lvr WITH LENGTH :msglen; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5583;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&amt;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&blob;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&offset;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)lvr;
  sqlstm.sqhstl[3] = (unsigned long )4;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&msglen;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[5] = (unsigned long )34;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		if (sqlca.sqlcode == 1403 )
		{
			memcpy((char*)pFamilyMsg[iNum].msg,lvr->buf,msglen + 1);
		}
		++iNum;
	}
	
	/* EXEC SQL AT :db_link_name DELETE FROM IM_CLUSTERSYSMSG WHERE  ClusterId = :clusid; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "delete  from IM_CLUSTERSYSMSG  where ClusterId=:b1";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5622;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&clusid;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

//清除验证记录
	

	/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5645;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5668;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	free(lvr);
	return DB_SUCCESS;
}

/* 获取某个用户的离线聊天信息 */
int  CDatabaseMini::Get_User_Offline_Msg(uint32 userID,ST_OFFLINE_MSG *pFamilyMsg, uint8 &nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		long_varraw *lvr;
		unsigned int     recv,origin,time,msglen, int_tmp;
		short   msgtype;
		OCIBlobLocator *blob;
		unsigned int amt,offset,bufsize;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	amt = 0; offset=1;bufsize = MAX_MSG_LEN;

	recv = userID;

	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	if (nRetCount == 0)
	{
		/* EXEC SQL AT :db_link_name SELECT COUNT(Sender) INTO :int_tmp 
			FROM  IM_OFFLINETALKMSG WHERE Receiver= :recv; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select count(Sender) into :b1  from IM_OFFLINETALKMSG wher\
e Receiver=:b2";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5687;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&recv;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 
		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( db_ret < 0 )
		{
			sql_error(sqlca, oraca);
			return DB_ERROR;
		}
		nRetCount = int_tmp;
		return DB_SUCCESS;	
	}

	/* EXEC SQL AT :db_link_name DECLARE cur_getusermsg CURSOR FOR
		SELECT Sender,
			(cast(cast(SendTime as timestamp) at time zone 'gmt' as date)-to_date('19700101 00:00:00','yyyymmdd hh24:mi:ss'))*24*60*
60 as SendTime, MsgType,MsgLen,MSG
		FROM  IM_OFFLINETALKMSG WHERE Receiver= :recv order by SendTime; */ 

	/* EXEC SQL OPEN cur_getusermsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0127;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5714;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&recv;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5737;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 && db_ret != -1405)
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5756;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}
	
	/* EXEC SQL AT :db_link_name ALLOCATE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5775;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5798;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if (db_ret == 1403 || db_ret < 0)
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5817;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5840;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}
 
	lvr = (long_varraw *)malloc(sizeof(unsigned int) + bufsize);
	lvr->len = bufsize;
	
	int iNum = 0;

	for(;;)
	{  
		amt = 0; offset=1;bufsize = MAX_MSG_LEN;
	 
		/* EXEC SQL FETCH cur_getusermsg INTO :origin,:time,:msgtype,:msglen,:blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5859;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&origin;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&time;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&msgtype;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&msglen;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&blob;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[5] = (unsigned long )34;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			/* EXEC SQL CLOSE cur_getusermsg; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5898;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if (( db_ret < 0  && db_ret != -1405)|| db_ret == 1403 || iNum >= nRetCount)
		{
			if (db_ret != 1403)
				sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5917;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL CLOSE cur_getusermsg; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5940;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			free(lvr);
			nRetCount = iNum;
			return DB_SUCCESS;
		} 
		 
		pFamilyMsg[iNum].recvID		= userID;
		pFamilyMsg[iNum].sendID		= origin;
		pFamilyMsg[iNum].tm			= (unsigned int)time;
		pFamilyMsg[iNum].msgtype	= (unsigned short)msgtype;  
		pFamilyMsg[iNum].msglen		= msglen + 1;

 
		/* EXEC SQL AT :db_link_name LOB READ :amt FROM :blob AT :offset INTO :lvr WITH LENGTH :msglen; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5959;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&amt;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&blob;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&offset;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)lvr;
  sqlstm.sqhstl[3] = (unsigned long )4;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&msglen;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[5] = (unsigned long )34;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		if (sqlca.sqlcode == 1403 )
		{
			memcpy((char*)pFamilyMsg[iNum].msg,lvr->buf,msglen + 1);
		}
		++iNum;
		
	}

	/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5998;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL CLOSE cur_getusermsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6021;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	free(lvr);
	return DB_SUCCESS;
}


//   获取某个用户的离线系统信息 
int  CDatabaseMini::Get_User_Offline_SysMsg(uint32 userID,ST_OFFLINE_MSG *pFamilyMsg, uint8 &nRetCount)
{ 
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		long_varraw *lvr;
		unsigned int     recv,origin,time,msglen, int_tmp;
		short   msgtype;
		OCIBlobLocator *blob;
		unsigned int amt,offset,bufsize;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);


	amt = 0; offset=1;bufsize = MAX_MSG_LEN;

	recv = userID;
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	if (nRetCount == 0)
	{
		/* EXEC SQL AT :db_link_name SELECT COUNT(Sender) INTO :int_tmp 
			FROM  IM_OFFLINESYSMSG WHERE Receiver= :recv; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select count(Sender) into :b1  from IM_OFFLINESYSMSG where\
 Receiver=:b2";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6040;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&int_tmp;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&recv;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 
		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if( db_ret < 0 )
		{
			sql_error(sqlca, oraca);
			return DB_ERROR;
		}
		nRetCount = int_tmp;
		return DB_SUCCESS;	
	}
	 
	/* EXEC SQL AT :db_link_name DECLARE cur_getusersysmsg CURSOR FOR
		SELECT Sender,SendTime,MsgType,MsgLen,MSG 
		FROM   IM_OFFLINESYSMSG WHERE Receiver= :recv; */ 

	/* EXEC SQL OPEN cur_getusersysmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0129;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6067;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&recv;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6090;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0  && db_ret != -1405)
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6109;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}
	
	/* EXEC SQL AT :db_link_name ALLOCATE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6128;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6151;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if (db_ret == 1403 || (db_ret < 0  && db_ret != -1405) )
	{
		if (db_ret != 1403)
			sql_error(sqlca, oraca);
		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6170;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL CLOSE cur_familysysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6193;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}
 
	lvr = (long_varraw *)malloc(sizeof(unsigned int) + bufsize);
	lvr->len = bufsize;
	
	int iNum = 0;

	for(;;)
	{  
		amt = 0; offset=1;bufsize = MAX_MSG_LEN;
		/* EXEC SQL FETCH cur_getusersysmsg INTO :origin,:time,:msgtype,:msglen,:blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6212;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&origin;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&time;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&msgtype;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&msglen;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&blob;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[5] = (unsigned long )34;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		
		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			/* EXEC SQL CLOSE cur_getusersysmsg; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )6251;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if ( (db_ret < 0 && db_ret != -1405) || db_ret == 1403 || iNum >= nRetCount)
		{
			if (db_ret != 1403)
				sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )6270;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL CLOSE cur_getusersysmsg; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )6293;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			free(lvr);
			nRetCount = iNum;
			return DB_SUCCESS;
		} 
		 
		pFamilyMsg[iNum].recvID		= userID;
		pFamilyMsg[iNum].sendID		= origin;
		pFamilyMsg[iNum].tm			= (unsigned int)time;
		pFamilyMsg[iNum].msgtype	= (unsigned short)msgtype;  
		pFamilyMsg[iNum].msglen		= msglen + 1;

 
		/* EXEC SQL AT :db_link_name LOB READ :amt FROM :blob AT :offset INTO :lvr WITH LENGTH :msglen; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6312;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&amt;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&blob;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&offset;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)lvr;
  sqlstm.sqhstl[3] = (unsigned long )4;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&msglen;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[5] = (unsigned long )34;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		if (sqlca.sqlcode == 1403 )
		{
			memcpy((char*)pFamilyMsg[iNum].msg,lvr->buf,msglen + 1);
		}
		++iNum;
	}

	/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6351;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL CLOSE cur_getusersysmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6374;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	free(lvr);
	return DB_SUCCESS;
}

//获取客户端系统消息（公告)
int CDatabaseMini::Get_Pub_Msg(uint32 userID,ST_WEB_MSG *pMsg, uint8 &nRetCount)
{ 
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     recv;
		unsigned int msgno, msgrecvid, timenow;
		/* VARCHAR var_msg[1401]; */ 
struct { unsigned short len; unsigned char arr[1401]; } var_msg;

		/* VARCHAR db_link_name[32],timesend[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

struct { unsigned short len; unsigned char arr[32]; } timesend;

	/* EXEC SQL END DECLARE SECTION; */ 

	
	SET_INIT_NULL(var_msg);

	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
		
	var_msg.arr[0]='\0';
	var_msg.len=0;
	timesend.arr[0] = '\0';
	timesend.len = 0;
	msgrecvid = 0;
	msgno = 0;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


	recv = userID;
	int db_ret;
	int iReconnTimes;
	bool bReconn;	
	
 start:
	/* EXEC SQL AT :db_link_name SELECT MsgID INTO :msgno FROM IM_USERSTAT WHERE UserId=:recv; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select MsgID into :b1  from IM_USERSTAT where UserId=:b2";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6393;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&msgno;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&recv;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if (db_ret < 0) 
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
		
	if (db_ret == 1403)//没有该用户
	{
		msgno = 0;
		
		time_t t;
		time(&t);
		timenow = (unsigned int)t;
		/* EXEC SQL AT :db_link_name INSERT INTO  IM_USERSTAT (UserId,MSGID,LstLoginTime,LstLogoutTime)
			values(:recv, :msgno, :timenow,:timenow); */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "insert into IM_USERSTAT (UserId,MSGID,LstLoginTime,LstLogo\
utTime) values (:b1,:b2,:b3,:b3)";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6420;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&recv;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&msgno;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&timenow;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&timenow;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[4] = (unsigned long )34;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (conn_error(db_ret))
		{
			iReconnTimes=0;
			bReconn=ConnectDB();
			while(!bReconn)
			{
				iReconnTimes++;
				sleep(iReconnTimes%10);
				bReconn=ConnectDB();
			}
			goto start;				
		}
		
		if (db_ret < 0 ) //出错
		{
			sql_error(sqlca, oraca);
			return DB_ERROR;
		}
		/* EXEC SQL AT :db_link_name COMMIT; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6455;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	}
 
	/* EXEC SQL AT :db_link_name DECLARE cur_pubmsg CURSOR FOR
		SELECT Msgid, msg,TO_CHAR(PubTime,'YYYY/MM/DD HH24:MI:SS')  
		FROM IM_SYSTEMMSG WHERE Msgid > :msgno AND ExpireTime>= sysdate ORDER BY MSGID; */ 

	/* EXEC SQL OPEN cur_pubmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0133;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6474;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&msgno;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	int iNum = 0;
	for(;;)
	{  
		/* EXEC SQL FETCH cur_pubmsg INTO :msgrecvid, :var_msg,:timesend; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6497;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&msgrecvid;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&var_msg;
  sqlstm.sqhstl[1] = (unsigned long )1403;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&timesend;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[3] = (unsigned long )34;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		db_ret = sqlca.sqlcode;
		if (db_ret == 1403 )
		{
			/* EXEC SQL CLOSE cur_pubmsg; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )6528;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 
			if (iNum != 0)
			{
				/* EXEC SQL AT :db_link_name UPDATE IM_USERSTAT SET MSGID = :msgno	WHERE UserId=:recv; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "update IM_USERSTAT  set MSGID=:b1 where UserId=:b2";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )6547;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&msgno;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&recv;
    sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[2] = (unsigned long )34;
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         short *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned long )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
				/* EXEC SQL AT :db_link_name COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )6574;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[0] = (unsigned long )34;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			}
			nRetCount = iNum;
			return DB_SUCCESS;
		}
		if( db_ret < 0 && db_ret != -1405 )
		{
			sql_error(sqlca, oraca);
			/* EXEC SQL CLOSE cur_pubmsg; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )6593;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			nRetCount = iNum;
			return DB_SUCCESS;
		}
		msgno = msgrecvid;
		
		var_msg.arr[var_msg.len]='\0';
		timesend.arr[timesend.len] = '\0';
		
		pMsg[iNum].recID			= 0;
		pMsg[iNum].recvID		= recv;
		pMsg[iNum].sendID		= 0;
		pMsg[iNum].msgtype		= 0;  
		//pMsg[iNum].msglen		= var_msg.len + 1;
		memcpy((char*)pMsg[iNum].msg,var_msg.arr,var_msg.len + 1);
		strncpy(pMsg[iNum].sendTime, (char*)timesend.arr,31);
		
		iNum++;
		memset(var_msg.arr, 0,var_msg.len);
		memset(timesend.arr, 0, timesend.len);
	}

	/* EXEC SQL CLOSE cur_pubmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6612;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	return DB_SUCCESS;
}

int CDatabaseMini::Save_OnlineUser_Num(uint32 onlinenum)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     ionlinenum;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	

	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
		
	ionlinenum = onlinenum;	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.SaveOnlineNum(:ionlinenum,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . SaveOnlineNum ( :ionlinenum , :retco\
de ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6631;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&ionlinenum;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if (db_ret < 0) 
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
 
	return DB_SUCCESS;
}

int CDatabaseMini::TestContext(void)
{
//	DEF_ORASET;

// 	EXEC SQL BEGIN DECLARE SECTION;
// 		sql_context testctx[ 1 ];
// 	EXEC SQL END DECLARE SECTION;
// 
// 	for (int i = 0; i < 1; ++i)
// 	{
// 		EXEC SQL CONTEXT ALLOCATE :testctx[i];
// 	}
// 	for (int i = 0; i < 1; ++i)
// 	{
// 		EXEC SQL CONTEXT USE :testctx[i];
// 		EXEC SQL CONTEXT FREE :testctx[i];
// 	}	
	return 0;
}

//test blob procedure
int CDatabaseMini::Save_Family_RequestMsg2(const uint32 c_iFamilyID, uint32 iSenderID,uint32 iSendTime,const char *strMsg, int iMsgLen)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		long_varraw * lvr;
		unsigned int     nFamilyID, nSenderID, nSendTime, nMsgSeq;
		int nMsgLen, nMsgType;
		char*   pmsg;
		OCIBlobLocator *blob;
		unsigned int offset = 1; /* attention: typedef ub4 unsigned int */
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);

	
	nFamilyID = c_iFamilyID;
	nSenderID = iSenderID;
	nSendTime = iSendTime;
	nMsgLen = iMsgLen;
	nMsgType = CMD_CLUSTER_AUTH_REQ;
	pmsg = (char*)strMsg;
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;	

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:   	
	/* EXEC SQL AT :db_link_name ALLOCATE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6658;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	
  
	lvr = (long_varraw *)malloc(sizeof(ub4) + nMsgLen);
	lvr->len = nMsgLen;
	memcpy(lvr->buf,pmsg,nMsgLen);
	/* EXEC SQL AT :db_link_name LOB WRITE ONE :nMsgLen 
			FROM :lvr WITH LENGTH :nMsgLen INTO :blob AT :offset; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6681;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nMsgLen;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)lvr;
 sqlstm.sqhstl[1] = (unsigned long )4;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nMsgLen;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&blob;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&offset;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[5] = (unsigned long )34;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 || db_ret == 1403 )
	{
		sql_error(sqlca, oraca);	
		/* EXEC SQL AT :db_link_name ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6720;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6739;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		free(lvr);
		return DB_ERROR;
	} 
	free(lvr);

	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.SaveFamilyRequestMsg(:nMsgSeq, :nFamilyID,:nSenderID,:nSendTime,:nMsgType,:nMsgLen,:blob,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . SaveFamilyRequestMsg ( :nMsgSeq , :n\
FamilyID , :nSenderID , :nSendTime , :nMsgType , :nMsgLen , :blob , :retcode \
) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6762;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nMsgSeq;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&nFamilyID;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nSenderID;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&nSendTime;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&nMsgType;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&nMsgLen;
 sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (unsigned char  *)&blob;
 sqlstm.sqhstl[6] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         short *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned long )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         short *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned long )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[8] = (unsigned long )34;
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         short *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned long )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	

	/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6813;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL AT :db_link_name COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6836;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	
	return DB_SUCCESS;
}

int CDatabaseMini::SetWokuMsgRecvID(uint32 userID, uint8 Type, uint32 recID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     recid, userid;
		int				type;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	recid = recID;
	userid = userID;
	type = Type;
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

start: 		

	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.SetWokuMsgRecvID(:userid,:type,:recid,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . SetWokuMsgRecvID ( :userid , :type ,\
 :recid , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6855;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&userid;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&type;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&recid;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[4] = (unsigned long )34;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if (db_ret < 0 || db_ret == 1403)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;	
	}	
	return DB_SUCCESS;
}

int CDatabaseMini::GetWokuMsgID(uint32 userID,uint32 &nOperationID,uint32 &nUserMsgID,uint32 &nSysMsgID,uint32 &nBroadcastID,uint32 &nUsercommentID,uint32 &nLogcommentID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     userid,OperationID,UserMsgID,SysMsgID,BroadcastID,UsercommentID,LogcommentID;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	userid = userID;
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

start: 		
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Local_Pkg.GetWokuMsgID(:userid,:OperationID,:UserMsgID,:SysMsgID,:BroadcastID,:UsercommentID,:LogcommentID,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Local_Pkg . GetWokuMsgID ( :userid , :OperationI\
D , :UserMsgID , :SysMsgID , :BroadcastID , :UsercommentID , :LogcommentID , \
:retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6890;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&userid;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&OperationID;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&UserMsgID;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&SysMsgID;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&BroadcastID;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&UsercommentID;
 sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (unsigned char  *)&LogcommentID;
 sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         short *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned long )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         short *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned long )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[8] = (unsigned long )34;
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         short *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned long )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if ((db_ret < 0 && db_ret != -1405)|| db_ret == 1403)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;	
	}	
	if(retcode != 1403)
	{
		nOperationID = OperationID;
		nUserMsgID = UserMsgID;
		nSysMsgID = SysMsgID;
		nBroadcastID = BroadcastID;
		nUsercommentID = UsercommentID;
		nLogcommentID = LogcommentID;
	}
	return DB_SUCCESS;
}

/* 获取WEBIM互通消息(及离线消息) */
int  CDatabaseMini::Get_WebIM_Msg(int ServerNo,ST_WEBIM_MSG *pWebIMMsg, uint16 &nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		int v_serverno;
		unsigned long v_msgid;
		unsigned int v_receiver, v_sender, v_sendtime, v_msglen, v_reqcount;
		unsigned int amt,offset, bufsize = MAX_MSG_LEN;
		short  v_msgtype;
		long_varraw *lvr;
		OCIBlobLocator *blob;
		/* varchar v_sendername[41], v_text[1401]; */ 
struct { unsigned short len; unsigned char arr[41]; } v_sendername;

struct { unsigned short len; unsigned char arr[1401]; } v_text;

		/* varchar db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 

	
	SET_INIT_NULL(db_link_name);

	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	v_serverno = ServerNo;
	v_reqcount = nRetCount;

	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	/* EXEC SQL AT :db_link_name ALLOCATE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6941;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



start:

	/* EXEC SQL AT :db_link_name DECLARE cur_webimmsg CURSOR FOR
		select t1.MsgId,t1.Receiver,t1.Sender,
		(cast(cast(t1.SendTime as timestamp) at time zone 'gmt' as date)-to_date('19700101 00:00:00','yyyymmdd hh24:mi:ss'))*24*60*60 as SendTime,
		t1.MsgType,t1.MsgLen,t1.Msg,t1.SenderName,t1.Text
		from IM_OFFLINETALKMSG t1, IM_USERONLINE t2
		where t2.ServerNo=:v_serverno and t2.status=1 and t1.Receiver=t2.UserId
		and rownum <= :v_reqcount
		order by t1.Receiver,t1.MsgId; */ 

	/* EXEC SQL OPEN cur_webimmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0142;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )6964;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&v_serverno;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&v_reqcount;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[2] = (unsigned long )34;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		/* EXEC SQL CLOSE cur_webimmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )6991;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if( db_ret < 0 && db_ret != -1405)
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL CLOSE cur_webimmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )7010;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}
	
	db_ret = sqlca.sqlcode;
	if (conn_error(db_ret))
	{
		/* EXEC SQL CLOSE cur_webimmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )7029;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		iReconnTimes=0;
		bReconn=ConnectDB();
		while(!bReconn)
		{
			iReconnTimes++;
			sleep(iReconnTimes%10);
			bReconn=ConnectDB();
		}
		goto start;				
	}
	
	if (db_ret < 0)
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )7048;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&blob;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		/* EXEC SQL CLOSE cur_webimmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )7071;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[0] = (unsigned long )34;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		return DB_ERROR;
	}
 
	lvr = (long_varraw *)malloc(sizeof(unsigned int) + bufsize);
	lvr->len = bufsize;
	
	int iNum = 0;

	for(;;)
	{  
		amt = 0; offset = 1; bufsize = MAX_MSG_LEN;
	 
		SET_INIT_NULL(v_sendername);
		SET_INIT_NULL(v_text);
		
		/* EXEC SQL FETCH cur_webimmsg INTO 
			:v_msgid, :v_receiver, :v_sender, :v_sendtime,:v_msgtype,:v_msglen,:blob, :v_sendername, :v_text; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )7090;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&v_msgid;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(long);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&v_receiver;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&v_sender;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&v_sendtime;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&v_msgtype;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&v_msglen;
  sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&blob;
  sqlstm.sqhstl[6] = (unsigned long )sizeof(void *);
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         short *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned long )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (unsigned char  *)&v_sendername;
  sqlstm.sqhstl[7] = (unsigned long )43;
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         short *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned long )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (unsigned char  *)&v_text;
  sqlstm.sqhstl[8] = (unsigned long )1403;
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         short *)0;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned long )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqhstv[9] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[9] = (unsigned long )34;
  sqlstm.sqhsts[9] = (         int  )0;
  sqlstm.sqindv[9] = (         short *)0;
  sqlstm.sqinds[9] = (         int  )0;
  sqlstm.sqharm[9] = (unsigned long )0;
  sqlstm.sqadto[9] = (unsigned short )0;
  sqlstm.sqtdso[9] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}



		db_ret = sqlca.sqlcode;
		if (( db_ret < 0  && db_ret != -1405)|| db_ret == 1403 || iNum >= nRetCount)
		{
			if (db_ret != 1403)
				sql_error(sqlca, oraca);
			/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )7145;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&blob;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			/* EXEC SQL CLOSE cur_webimmsg; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )7168;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			free(lvr);
			/* EXEC SQL AT :db_link_name COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )7187;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[0] = (unsigned long )34;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			nRetCount = iNum;
			return DB_SUCCESS;
		} 
		 
		pWebIMMsg[iNum].msgid	= v_msgid;
		pWebIMMsg[iNum].recvID	= v_receiver;
		pWebIMMsg[iNum].sendID	= v_sender;
		pWebIMMsg[iNum].tm		= (unsigned int)v_sendtime;
		pWebIMMsg[iNum].msgtype	= (unsigned short)v_msgtype;  
		pWebIMMsg[iNum].msglen	= v_msglen;
		strncpy(pWebIMMsg[iNum].from_nickname, (char*)v_sendername.arr, MAX_NICKNAME_LEN+1);
		strncpy(pWebIMMsg[iNum].msgtext, (char*)v_text.arr, MAX_MSG_LEN);
 
		if(v_msglen > 0)
		{
			/* EXEC SQL AT :db_link_name LOB READ :amt FROM :blob AT :offset INTO :lvr WITH LENGTH :v_msglen; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )7206;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&amt;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&blob;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(void *);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&offset;
   sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)lvr;
   sqlstm.sqhstl[3] = (unsigned long )4;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)&v_msglen;
   sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[5] = (unsigned long )34;
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         short *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned long )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			if (sqlca.sqlcode == 1403 )
			{
				memcpy((char*)pWebIMMsg[iNum].msg,lvr->buf,v_msglen);
			}
		}
		++iNum;

		/* EXEC SQL AT :db_link_name insert into 
			IM_OFFLINETALKMSG_ARC (
						msgid,receiver,sender,srcip,srcport,sendtime,msgtype,msglen,msg,
						receivername,sendername,readtime,status,text) 
					select
						msgid,receiver,sender,srcip,srcport,sendtime,msgtype,msglen,msg,
						receivername,sendername,sysdate,1,text
					from IM_OFFLINETALKMSG where msgid=:v_msgid; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "insert into IM_OFFLINETALKMSG_ARC (msgid,receiver,sender,s\
rcip,srcport,sendtime,msgtype,msglen,msg,receivername,sendername,readtime,sta\
tus,text)select msgid ,receiver ,sender ,srcip ,srcport ,sendtime ,msgtype ,m\
sglen ,msg ,receivername ,sendername ,sysdate ,1 ,text  from IM_OFFLINETALKMS\
G where msgid=:b1";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )7245;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&v_msgid;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(long);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

 
		/* EXEC SQL AT	:db_link_name delete from IM_OFFLINETALKMSG where msgid=:v_msgid; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "delete  from IM_OFFLINETALKMSG  where msgid=:b1";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )7268;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&v_msgid;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(long);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		LPONLINE_USER_INFO lpUser = CAppManager::GetInstance().GetAppNetWork().FindOnlineUser(v_receiver);
		if(lpUser == NULL)
		{
			/* EXEC SQL AT :db_link_name UPDATE IM_USERONLINE SET STATUS=0 WHERE USERId= :v_receiver; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "update IM_USERONLINE  set STATUS=0 where USERId=:b1";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )7291;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&v_receiver;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
   sqlstm.sqhstl[1] = (unsigned long )34;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


			db_ret = sqlca.sqlcode;
			if (db_ret < 0 ) 
			{
				/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )7314;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&blob;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[1] = (unsigned long )34;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				/* EXEC SQL CLOSE cur_webimmsg; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )7337;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
    sqlstm.sqhstl[0] = (unsigned long )34;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


				free(lvr);
				return DB_ERROR;
			}
		}
	}
	/* EXEC SQL AT :db_link_name FREE :blob; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )7356;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&blob;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[1] = (unsigned long )34;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL CLOSE cur_webimmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )7379;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	free(lvr);
	/* EXEC SQL AT :db_link_name COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )7398;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[0] = (unsigned long )34;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


	return DB_SUCCESS;
}
void ForDisableBuildWarningMini()
{
	unsigned int nTest =  sqlstm.sqlvsn;
	nTest++;
}
