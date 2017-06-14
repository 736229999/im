
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
           char  filnam[17];
};
static const struct sqlcxp sqlfpn =
{
    16,
    "./DatabaseWeb.pc"
};


static unsigned int sqlctx = 3409563;


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
   unsigned char  *sqhstv[24];
   unsigned long  sqhstl[24];
            int   sqhsts[24];
            short *sqindv[24];
            int   sqinds[24];
   unsigned long  sqharm[24];
   unsigned long  *sqharc[24];
   unsigned short  sqadto[24];
   unsigned short  sqtdso[24];
} sqlstm = {12,24};

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

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4138,852,0,0,
5,0,0,1,117,0,1542,93,0,0,5,5,0,0,128,3,3,0,0,3,9,0,0,3,9,0,0,3,9,0,0,3,3,0,0,
1,9,0,0,
44,0,0,2,97,0,1542,166,0,0,4,4,0,0,128,3,9,0,0,3,3,0,0,3,9,0,0,3,3,0,0,1,9,0,0,
79,0,0,3,121,0,1542,236,0,0,5,5,0,0,128,3,9,0,0,3,9,0,0,3,3,0,0,3,9,0,0,3,3,0,
0,1,9,0,0,
118,0,0,4,117,0,1542,312,0,0,5,5,0,0,128,3,3,0,0,3,9,0,0,3,9,0,0,3,9,0,0,3,3,0,
0,1,9,0,0,
157,0,0,5,185,0,1542,381,0,0,9,9,0,0,128,3,3,0,0,3,9,0,0,3,9,0,0,3,9,0,0,3,9,0,
0,3,9,0,0,3,9,0,0,3,9,0,0,3,3,0,0,1,9,0,0,
212,0,0,6,136,0,1542,447,0,0,7,7,0,0,128,3,3,0,0,3,3,0,0,3,3,0,0,3,9,0,0,3,9,0,
0,3,9,0,0,3,3,0,0,1,9,0,0,
259,0,0,7,331,0,1542,551,0,0,23,23,0,0,128,3,68,0,0,3,9,0,0,3,9,0,0,3,3,0,0,3,
9,0,0,3,9,0,0,3,9,0,0,3,3,0,0,3,9,0,0,3,9,0,0,3,3,0,0,3,3,0,0,3,3,0,0,3,9,0,0,
3,3,0,0,3,9,0,0,3,3,0,0,3,4,0,0,3,9,0,0,3,3,0,0,3,3,0,0,3,3,0,0,3,3,0,0,1,9,0,
0,
370,0,0,8,87,0,1542,642,0,0,3,3,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,1,9,0,0,
401,0,0,0,0,0,568,705,0,0,1,1,0,1,0,3,102,0,0,
420,0,0,9,121,0,1542,713,0,0,6,6,0,0,128,3,68,0,0,3,3,0,0,3,3,0,0,3,102,0,0,3,
3,0,0,3,68,0,0,1,9,0,0,
463,0,0,0,0,0,527,722,0,0,1,1,0,1,0,1,102,0,0,
482,0,0,0,0,0,527,738,0,0,1,1,0,1,0,1,102,0,0,
501,0,0,0,0,0,590,739,0,0,1,1,0,1,0,3,102,0,0,
520,0,0,0,0,0,525,746,0,0,3,1,0,1,0,1,102,0,0,2,68,0,0,2,3,0,0,
547,0,0,0,0,0,527,768,0,0,1,1,0,1,0,1,102,0,0,
566,0,0,0,0,0,590,769,0,0,1,1,0,1,0,3,102,0,0,
585,0,0,0,0,0,568,789,0,0,1,1,0,1,0,3,102,0,0,
604,0,0,10,83,0,1542,797,0,0,3,3,0,0,128,3,68,0,0,3,102,0,0,3,3,0,0,1,9,0,0,
635,0,0,0,0,0,527,806,0,0,1,1,0,1,0,1,102,0,0,
654,0,0,0,0,0,527,821,0,0,1,1,0,1,0,1,102,0,0,
673,0,0,0,0,0,590,822,0,0,1,1,0,1,0,3,102,0,0,
692,0,0,0,0,0,525,828,0,0,2,1,0,1,0,1,102,0,0,2,68,0,0,
715,0,0,0,0,0,527,844,0,0,1,1,0,1,0,1,102,0,0,
734,0,0,0,0,0,590,845,0,0,1,1,0,1,0,3,102,0,0,
753,0,0,11,70,0,1542,871,0,0,3,3,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,1,9,0,0,
784,0,0,0,0,0,568,945,0,0,1,1,0,1,0,3,102,0,0,
803,0,0,12,95,0,1542,946,0,0,4,4,0,0,128,3,68,0,0,3,9,0,0,3,102,0,0,3,3,0,0,1,
9,0,0,
838,0,0,0,0,0,527,955,0,0,1,1,0,1,0,1,102,0,0,
857,0,0,0,0,0,527,970,0,0,1,1,0,1,0,1,102,0,0,
876,0,0,0,0,0,590,971,0,0,1,1,0,1,0,3,102,0,0,
895,0,0,0,0,0,525,977,0,0,3,1,0,1,0,1,102,0,0,2,68,0,0,2,68,0,0,
922,0,0,0,0,0,527,1001,0,0,1,1,0,1,0,1,102,0,0,
941,0,0,0,0,0,590,1002,0,0,1,1,0,1,0,3,102,0,0,
960,0,0,13,81,0,1542,1027,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,3,0,0,1,
9,0,0,
995,0,0,14,73,0,1542,1080,0,0,3,3,0,0,128,3,68,0,0,3,3,0,0,3,3,0,0,1,9,0,0,
1026,0,0,15,88,0,1542,1141,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,3,3,0,0,1,
9,0,0,
1061,0,0,16,76,0,1542,1195,0,0,3,3,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,1,9,0,0,
1092,0,0,17,114,0,1542,1249,0,0,6,6,0,0,128,3,68,0,0,3,9,0,0,3,9,0,0,3,3,0,0,3,
3,0,0,3,3,0,0,1,9,0,0,
1135,0,0,18,115,0,1542,1318,0,0,6,6,0,0,128,3,9,0,0,3,68,0,0,3,9,0,0,3,3,0,0,3,
3,0,0,3,3,0,0,1,9,0,0,
1178,0,0,0,0,0,568,1386,0,0,1,1,0,1,0,3,102,0,0,
1197,0,0,19,120,0,1542,1391,0,0,5,5,0,0,128,3,9,0,0,3,3,0,0,3,3,0,0,3,102,0,0,
3,3,0,0,1,9,0,0,
1236,0,0,0,0,0,527,1400,0,0,1,1,0,1,0,1,102,0,0,
1255,0,0,0,0,0,527,1415,0,0,1,1,0,1,0,1,102,0,0,
1274,0,0,0,0,0,590,1416,0,0,1,1,0,1,0,3,102,0,0,
1293,0,0,0,0,0,525,1423,0,0,6,1,0,1,0,1,102,0,0,2,68,0,0,2,9,0,0,2,9,0,0,2,3,0,
0,2,3,0,0,
1332,0,0,0,0,0,527,1450,0,0,1,1,0,1,0,1,102,0,0,
1351,0,0,0,0,0,590,1451,0,0,1,1,0,1,0,3,102,0,0,
1370,0,0,0,0,0,568,1488,0,0,1,1,0,1,0,3,102,0,0,
1389,0,0,20,111,0,1542,1491,0,0,5,5,0,0,128,3,9,0,0,3,3,0,0,3,3,0,0,3,102,0,0,
3,3,0,0,1,9,0,0,
1428,0,0,0,0,0,527,1499,0,0,1,1,0,1,0,1,102,0,0,
1447,0,0,0,0,0,527,1514,0,0,1,1,0,1,0,1,102,0,0,
1466,0,0,0,0,0,590,1515,0,0,1,1,0,1,0,3,102,0,0,
1485,0,0,0,0,0,525,1522,0,0,6,1,0,1,0,1,102,0,0,2,68,0,0,2,3,0,0,2,9,0,0,2,9,0,
0,2,3,0,0,
1524,0,0,0,0,0,527,1550,0,0,1,1,0,1,0,1,102,0,0,
1543,0,0,0,0,0,590,1551,0,0,1,1,0,1,0,3,102,0,0,
1562,0,0,21,59,0,1542,1575,0,0,2,2,0,0,128,3,68,0,0,3,3,0,0,1,9,0,0,
1589,0,0,22,268,0,1542,1712,0,0,19,19,0,0,128,3,68,0,0,3,3,0,0,3,9,0,0,3,9,0,0,
3,9,0,0,3,9,0,0,3,9,0,0,3,9,0,0,3,3,0,0,3,9,0,0,3,3,0,0,3,3,0,0,3,3,0,0,3,3,0,
0,3,9,0,0,3,3,0,0,3,3,0,0,3,9,0,0,3,3,0,0,1,9,0,0,
1684,0,0,23,113,0,1542,1772,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,9,0,0,3,3,0,0,
1,9,0,0,
1719,0,0,24,85,0,1542,1836,0,0,3,3,0,0,128,3,68,0,0,3,9,0,0,3,3,0,0,1,9,0,0,
1750,0,0,25,98,0,1542,1887,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,3,0,0,
1,9,0,0,
1785,0,0,0,0,0,568,1948,0,0,1,1,0,1,0,3,102,0,0,
1804,0,0,26,92,0,1542,1951,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,102,0,0,3,3,0,0,
1,9,0,0,
1839,0,0,0,0,0,527,1959,0,0,1,1,0,1,0,1,102,0,0,
1858,0,0,0,0,0,527,1974,0,0,1,1,0,1,0,1,102,0,0,
1877,0,0,0,0,0,590,1975,0,0,1,1,0,1,0,3,102,0,0,
1896,0,0,0,0,0,525,1983,0,0,7,1,0,1,0,1,102,0,0,2,68,0,0,2,68,0,0,2,68,0,0,2,3,
0,0,2,9,0,0,2,9,0,0,
1939,0,0,0,0,0,527,2014,0,0,1,1,0,1,0,1,102,0,0,
1958,0,0,0,0,0,590,2015,0,0,1,1,0,1,0,3,102,0,0,
1977,0,0,0,0,0,568,2046,0,0,1,1,0,1,0,3,102,0,0,
1996,0,0,27,101,0,1542,2049,0,0,4,4,0,0,128,3,68,0,0,3,3,0,0,3,102,0,0,3,3,0,0,
1,9,0,0,
2031,0,0,0,0,0,527,2058,0,0,1,1,0,1,0,1,102,0,0,
2050,0,0,0,0,0,527,2073,0,0,1,1,0,1,0,1,102,0,0,
2069,0,0,0,0,0,590,2074,0,0,1,1,0,1,0,3,102,0,0,
2088,0,0,0,0,0,525,2082,0,0,6,1,0,1,0,1,102,0,0,2,68,0,0,2,68,0,0,2,3,0,0,2,9,
0,0,2,9,0,0,
2127,0,0,0,0,0,527,2111,0,0,1,1,0,1,0,1,102,0,0,
2146,0,0,0,0,0,590,2112,0,0,1,1,0,1,0,3,102,0,0,
2165,0,0,28,70,0,1542,2140,0,0,2,2,0,0,128,3,68,0,0,3,3,0,0,1,9,0,0,
2192,0,0,29,93,0,1542,2193,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,3,3,0,0,1,
9,0,0,
2227,0,0,30,96,0,1542,2256,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,3,3,0,0,1,
9,0,0,
2262,0,0,31,100,0,1542,2309,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,9,0,0,3,3,0,0,
1,9,0,0,
2297,0,0,0,0,0,568,2401,0,0,1,1,0,1,0,3,102,0,0,
2316,0,0,32,93,0,1542,2402,0,0,3,3,0,0,128,3,9,0,0,3,102,0,0,3,3,0,0,1,9,0,0,
2347,0,0,0,0,0,527,2411,0,0,1,1,0,1,0,1,102,0,0,
2366,0,0,0,0,0,527,2426,0,0,1,1,0,1,0,1,102,0,0,
2385,0,0,0,0,0,590,2427,0,0,1,1,0,1,0,3,102,0,0,
2404,0,0,0,0,0,525,2438,0,0,8,1,0,1,0,1,102,0,0,2,68,0,0,2,9,0,0,2,9,0,0,2,3,0,
0,2,9,0,0,2,3,0,0,2,9,0,0,
2451,0,0,0,0,0,527,2470,0,0,1,1,0,1,0,1,102,0,0,
2470,0,0,0,0,0,590,2471,0,0,1,1,0,1,0,3,102,0,0,
2489,0,0,0,0,0,568,2476,0,0,1,1,0,1,0,3,102,0,0,
2508,0,0,33,122,0,1542,2477,0,0,4,4,0,0,128,3,68,0,0,3,9,0,0,3,102,0,0,3,3,0,0,
1,9,0,0,
2543,0,0,0,0,0,527,2486,0,0,1,1,0,1,0,1,102,0,0,
2562,0,0,0,0,0,527,2501,0,0,1,1,0,1,0,1,102,0,0,
2581,0,0,0,0,0,590,2502,0,0,1,1,0,1,0,3,102,0,0,
2600,0,0,0,0,0,525,2509,0,0,11,1,0,1,0,1,102,0,0,2,68,0,0,2,9,0,0,2,9,0,0,2,3,
0,0,2,9,0,0,2,9,0,0,2,9,0,0,2,3,0,0,2,3,0,0,2,9,0,0,
2659,0,0,0,0,0,527,2548,0,0,1,1,0,1,0,1,102,0,0,
2678,0,0,0,0,0,590,2549,0,0,1,1,0,1,0,3,102,0,0,
2697,0,0,0,0,0,568,2600,0,0,1,1,0,1,0,3,102,0,0,
2716,0,0,34,97,0,1542,2607,0,0,3,3,0,0,128,3,9,0,0,3,102,0,0,3,3,0,0,1,9,0,0,
2747,0,0,0,0,0,527,2616,0,0,1,1,0,1,0,1,102,0,0,
2766,0,0,0,0,0,527,2631,0,0,1,1,0,1,0,1,102,0,0,
2785,0,0,0,0,0,590,2632,0,0,1,1,0,1,0,3,102,0,0,
2804,0,0,0,0,0,525,2639,0,0,3,1,0,1,0,1,102,0,0,2,68,0,0,2,3,0,0,
2831,0,0,0,0,0,527,2662,0,0,1,1,0,1,0,1,102,0,0,
2850,0,0,0,0,0,590,2663,0,0,1,1,0,1,0,3,102,0,0,
2869,0,0,0,0,0,568,2709,0,0,1,1,0,1,0,3,102,0,0,
2888,0,0,35,91,0,1542,2712,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,102,0,0,3,3,0,0,
1,9,0,0,
2923,0,0,0,0,0,527,2721,0,0,1,1,0,1,0,1,102,0,0,
2942,0,0,0,0,0,527,2736,0,0,1,1,0,1,0,1,102,0,0,
2961,0,0,0,0,0,590,2737,0,0,1,1,0,1,0,3,102,0,0,
2980,0,0,0,0,0,525,2746,0,0,11,1,0,1,0,1,102,0,0,2,68,0,0,2,68,0,0,2,68,0,0,2,
68,0,0,2,9,0,0,2,9,0,0,2,3,0,0,2,9,0,0,2,9,0,0,2,9,0,0,
3039,0,0,0,0,0,527,2788,0,0,1,1,0,1,0,1,102,0,0,
3058,0,0,0,0,0,590,2789,0,0,1,1,0,1,0,3,102,0,0,
3077,0,0,0,0,0,568,2827,0,0,1,1,0,1,0,3,102,0,0,
3096,0,0,36,88,0,1542,2831,0,0,4,4,0,0,128,3,68,0,0,3,3,0,0,3,102,0,0,3,3,0,0,
1,9,0,0,
3131,0,0,0,0,0,527,2854,0,0,1,1,0,1,0,1,102,0,0,
3150,0,0,0,0,0,590,2855,0,0,1,1,0,1,0,3,102,0,0,
3169,0,0,0,0,0,525,2864,0,0,6,1,0,1,0,1,102,0,0,2,68,0,0,2,9,0,0,2,9,0,0,2,9,0,
0,2,9,0,0,
3208,0,0,0,0,0,527,2903,0,0,1,1,0,1,0,1,102,0,0,
3227,0,0,0,0,0,590,2904,0,0,1,1,0,1,0,3,102,0,0,
3246,0,0,0,0,0,568,2936,0,0,1,1,0,1,0,3,102,0,0,
3265,0,0,37,101,0,1542,2940,0,0,5,5,0,0,128,3,68,0,0,3,9,0,0,3,3,0,0,3,102,0,0,
3,3,0,0,1,9,0,0,
3304,0,0,0,0,0,527,2963,0,0,1,1,0,1,0,1,102,0,0,
3323,0,0,0,0,0,590,2964,0,0,1,1,0,1,0,3,102,0,0,
3342,0,0,0,0,0,525,2972,0,0,5,1,0,1,0,1,102,0,0,2,68,0,0,2,9,0,0,2,9,0,0,2,9,0,
0,
3377,0,0,0,0,0,527,3009,0,0,1,1,0,1,0,1,102,0,0,
3396,0,0,0,0,0,590,3010,0,0,1,1,0,1,0,3,102,0,0,
3415,0,0,38,80,0,1542,3079,0,0,3,3,0,0,128,3,68,0,0,3,9,0,0,3,3,0,0,1,9,0,0,
3446,0,0,39,80,0,1542,3139,0,0,3,3,0,0,128,3,68,0,0,3,9,0,0,3,3,0,0,1,9,0,0,
};


#include "stdafx.h"

#define SQLCA_NONE 
#define ORACA_NONE
#include <sqlcpr.h>
#include <string>

#include "DatabaseWeb.h"
#include <sqlda.h>


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
// SQLDA * select_des;
// SQLDA * bind_des;

extern void sqlgls(char*,size_t*,size_t *);

//线程运行上下文
/* EXEC SQL BEGIN DECLARE SECTION; */ 

	extern sql_context m_ctx[ 200 ];
/* EXEC SQL END DECLARE SECTION; */ 

//连接池编译选项需增加：
//CPOOL=YES CMIN=5 CMAX=20 	
//代码检验
// sqlcheck=FULL USERID=blog01/blog01@test139

CDatabaseWeb::CDatabaseWeb(void)
{

}

CDatabaseWeb::~CDatabaseWeb(void)
{

}

void CDatabaseWeb::SetConnectDB(char* uid, char* pwd, char* sid)
{
	strncpy(webuid, uid,32);
	strncpy(webpwd, pwd,32);
	strncpy(websid, sid,32);
}

//连接WEB数据库  (初始化连接名，上下文)
bool CDatabaseWeb::ConnectDB()
{
	return ConnectWeb();
}

/*================================================================ 
*
* 函 数 名：Get_UserID_PWD 
** 根据用户名获取ID与密码
* 参 数： 
* char *c_pcUserName [IN] : 用户名 
* uint32 &iUserID [[IN,OUT]] : 用户ID
* char *pcPassWord [IN,OUT]: 用户密码
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 用户不存在: DB_NOUSER;
* 
================================================================*/ 
int CDatabaseWeb::Get_PWD_ByID(uint32 id,char* cPass,char* username,char* mobileno)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR var_username[40],var_password[60],var_mobileno[51]; */ 
struct { unsigned short len; unsigned char arr[40]; } var_username;

struct { unsigned short len; unsigned char arr[60]; } var_password;

struct { unsigned short len; unsigned char arr[51]; } var_mobileno;

		int i_userid;   
		int	retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	i_userid = id;
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);

	SET_INIT_NULL(var_password);
	SET_INIT_NULL(var_username);
	SET_INIT_NULL(var_mobileno);
		
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


	int db_ret;
	int iReconnTimes;
	bool bReconn;

start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetUserPwdByID(:i_userid,:var_password,:var_username,:var_mobileno,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 6;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetUserPwdByID ( :i_userid , :var_pas\
sword , :var_username , :var_mobileno , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&i_userid;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&var_password;
 sqlstm.sqhstl[1] = (unsigned long )62;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_username;
 sqlstm.sqhstl[2] = (unsigned long )42;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&var_mobileno;
 sqlstm.sqhstl[3] = (unsigned long )53;
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

	if (retcode == 1403 || db_ret == 1403 || db_ret == -1405)
	{
		return DB_NOUSER;
	}
	
	if (db_ret >= 0)
	{
		SET_END_NULL(var_password);
		SET_END_NULL(var_username);
		SET_END_NULL(var_mobileno);
		strncpy(cPass,(char*)var_password.arr,MAX_PASSWORD_LEN);
		strncpy(username,(char*)var_username.arr,MAX_USERNAME_LEN);
		strncpy(mobileno,(char*)var_mobileno.arr,MAX_PHONE_LEN);
		if (retcode == DB_NOACTIVE)
			return DB_NOACTIVE;
		else
			return DB_SUCCESS;
	}

	char szError[255];
	sprintf(szError, "Get_PWD_ByID() ID:%d,ErrorCode:%d\n",id, db_ret);
	sql_error(sqlca, oraca);
	return CError::DB_Error(DB_NOUSER, szError);
}

int CDatabaseWeb::Get_UserID_PWD(const char *c_pcUserName,uint32 &iUserID,char *pcPassWord)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR var_username[40],var_password[60]; */ 
struct { unsigned short len; unsigned char arr[40]; } var_username;

struct { unsigned short len; unsigned char arr[60]; } var_password;

		int i_userid;   
		int retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);

	i_userid = 0;
	SET_INIT_NULL(var_password);
	SET_INIT_NULL(var_username);
		
	if (strlen(c_pcUserName) > 38)
		return CError::DB_Error(DB_NOUSER, (char*)c_pcUserName);
	strncpy((char*)var_username.arr,c_pcUserName,MAX_USERNAME_LEN);
	var_username.len = (short)strlen((char *)var_username.arr);
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;
		
start:
		
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetUserPwd(:var_username,:i_userid,:var_password,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 6;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetUserPwd ( :var_username , :i_useri\
d , :var_password , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )44;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&var_username;
 sqlstm.sqhstl[0] = (unsigned long )42;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&i_userid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_password;
 sqlstm.sqhstl[2] = (unsigned long )62;
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

	if (retcode == 1403 || db_ret == 1403 || db_ret == -1405)
	{
		return DB_NOUSER;
	}		
	
	if (db_ret >= 0)
	{
		iUserID = i_userid;
		SET_END_NULL(var_password);
		strncpy(pcPassWord,(char*)var_password.arr,MAX_PASSWORD_LEN);
		if (retcode == DB_NOACTIVE)
			return DB_NOACTIVE;
		return DB_SUCCESS;
	}

	char szError[255];
	sprintf(szError, "Get_UserID_PWD() UserName:%s,ErrorCode:%d\n",c_pcUserName, db_ret);
	sql_error(sqlca, oraca);
	return CError::DB_Error(DB_NOUSER, szError);
}

int CDatabaseWeb::Get_UserID_PWD_ByMobileNo(const char *c_pcmobileno,uint32 &iUserID,char *pcPassWord,char *username)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR var_username[40],var_password[60],var_mobileno[51]; */ 
struct { unsigned short len; unsigned char arr[40]; } var_username;

struct { unsigned short len; unsigned char arr[60]; } var_password;

struct { unsigned short len; unsigned char arr[51]; } var_mobileno;

		int i_userid;   
		int	retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);

	i_userid = 0;
	SET_INIT_NULL(var_password);
	SET_INIT_NULL(var_username);
	SET_INIT_NULL(var_mobileno);
		
	if (strlen(c_pcmobileno) > 50)
		return CError::DB_Error(DB_NOUSER, (char*)c_pcmobileno);
	strncpy((char*)var_mobileno.arr,c_pcmobileno,MAX_PHONE_LEN);
	var_mobileno.len = (short)strlen((char *)var_mobileno.arr);
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;
		
start:
		
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetUserPwdByMobile(:var_mobileno,:var_username,:i_userid,:var_password,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 6;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetUserPwdByMobile ( :var_mobileno , \
:var_username , :i_userid , :var_password , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )79;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&var_mobileno;
 sqlstm.sqhstl[0] = (unsigned long )53;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&var_username;
 sqlstm.sqhstl[1] = (unsigned long )42;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&i_userid;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&var_password;
 sqlstm.sqhstl[3] = (unsigned long )62;
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

	if (retcode == 1403 || db_ret == 1403 || db_ret == -1405)
	{
		return DB_NOUSER;
	}
	
	if (db_ret >= 0)
	{
		iUserID = i_userid;
		SET_END_NULL(var_password);
		SET_END_NULL(var_username);
		strncpy(pcPassWord,(char*)var_password.arr,MAX_PASSWORD_LEN);
		strncpy(username,(char*)var_username.arr,MAX_USERNAME_LEN);
		if (retcode == DB_NOACTIVE)
			return DB_NOACTIVE;
		return DB_SUCCESS;
	}

	char szError[255];
	sprintf(szError, "Get_UserID_PWD_ByMobileNo() MobileNO:%s,ErrorCode:%d\n",c_pcmobileno, db_ret);
	sql_error(sqlca, oraca);
	return CError::DB_Error(DB_NOUSER, szError);
}

int CDatabaseWeb::SaveAccount(char * account,char* pwd,uint32 id,char* truename)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

	int nId,retcode;
	/* VARCHAR var_account[40],var_password[60],db_link_name[32],var_truename[100]; */ 
struct { unsigned short len; unsigned char arr[40]; } var_account;

struct { unsigned short len; unsigned char arr[60]; } var_password;

struct { unsigned short len; unsigned char arr[32]; } db_link_name;

struct { unsigned short len; unsigned char arr[100]; } var_truename;

	/* EXEC SQL END DECLARE SECTION; */ 


	nId=id;

	SET_INIT_NULL(var_account);
	SET_INIT_NULL(var_password);
	SET_INIT_NULL(var_truename);

	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	strncpy((char*)var_account.arr,account,MAX_USERNAME_LEN);
	var_account.len = (short)strlen((char *)var_account.arr);

	strncpy((char*)var_password.arr,pwd,MAX_PASSWORD_LEN);
	var_password.len = (short)strlen((char *)var_password.arr);

	strncpy((char*)var_truename.arr,truename,100);
	var_truename.len = (short)strlen((char *)var_truename.arr);


	int db_ret;
	int iReconnTimes;
	bool bReconn;

start:

	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.AccountSave_20150916(:nId,:var_account,:var_password,:var_truename,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 6;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . AccountSave_20150916 ( :nId , :var_ac\
count , :var_password , :var_truename , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )118;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nId;
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
 sqlstm.sqhstv[2] = (unsigned char  *)&var_password;
 sqlstm.sqhstl[2] = (unsigned long )62;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&var_truename;
 sqlstm.sqhstl[3] = (unsigned long )102;
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
	CLog::Log("SaveAccount", CLog::TYPE_DEBUG, "user:%s, db_ret:%d, retcode:%d ", account, db_ret, retcode);
		
	if (retcode == 1)
	{
		return DB_ERROR;
	}
	
	return DB_SUCCESS;
}

int CDatabaseWeb::SaveAccountBuyer(char * account,char* pwd,char* guid,uint32& id,char* mobileno,char*emailaddress,char*nickname,char*headImg)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

	int nId,retcode;
	/* VARCHAR var_account[41],var_password[60],db_link_name[32],var_guid[40],var_moblieno[22],var_emailaddress[42],var_nickname[41],var_headImg[101]; */ 
struct { unsigned short len; unsigned char arr[41]; } var_account;

struct { unsigned short len; unsigned char arr[60]; } var_password;

struct { unsigned short len; unsigned char arr[32]; } db_link_name;

struct { unsigned short len; unsigned char arr[40]; } var_guid;

struct { unsigned short len; unsigned char arr[22]; } var_moblieno;

struct { unsigned short len; unsigned char arr[42]; } var_emailaddress;

struct { unsigned short len; unsigned char arr[41]; } var_nickname;

struct { unsigned short len; unsigned char arr[101]; } var_headImg;

	/* EXEC SQL END DECLARE SECTION; */ 


	SET_INIT_NULL(var_account);
	SET_INIT_NULL(var_password);

	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	strncpy((char*)var_account.arr,account,MAX_USERNAME_LEN);
	var_account.len = (short)strlen((char *)var_account.arr);

	strncpy((char*)var_password.arr,pwd,MAX_PASSWORD_LEN);
	var_password.len = (short)strlen((char *)var_password.arr);

	strncpy((char*)var_guid.arr,guid,40);
	var_guid.len = (short)strlen((char *)var_guid.arr);
        strncpy((char*)var_moblieno.arr,mobileno,22);
	var_moblieno.len = (short)strlen((char *)var_moblieno.arr);

	strncpy((char*)var_emailaddress.arr,emailaddress,42);
	var_emailaddress.len = (short)strlen((char *)var_emailaddress.arr);
	
	strncpy((char*)var_nickname.arr,nickname,MAX_NICKNAME_LEN+1);
	var_nickname.len = (short)strlen((char *)var_nickname.arr);

	strncpy((char*)var_headImg.arr,headImg,MAX_HEADURL_LEN+1);
	var_headImg.len = (short)strlen((char *)var_headImg.arr);
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;

start:

	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.AccountSaveBuyer_20150907(:nId,:var_account,:var_password,:var_guid,:var_moblieno,:var_emailaddress,:var_nickname,:var_headImg,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 10;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . AccountSaveBuyer_20150907 ( :nId , :v\
ar_account , :var_password , :var_guid , :var_moblieno , :var_emailaddress , \
:var_nickname , :var_headImg , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )157;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nId;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&var_account;
 sqlstm.sqhstl[1] = (unsigned long )43;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_password;
 sqlstm.sqhstl[2] = (unsigned long )62;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&var_guid;
 sqlstm.sqhstl[3] = (unsigned long )42;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&var_moblieno;
 sqlstm.sqhstl[4] = (unsigned long )24;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&var_emailaddress;
 sqlstm.sqhstl[5] = (unsigned long )44;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (unsigned char  *)&var_nickname;
 sqlstm.sqhstl[6] = (unsigned long )43;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         short *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned long )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (unsigned char  *)&var_headImg;
 sqlstm.sqhstl[7] = (unsigned long )103;
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
	CLog::Log("SaveAccountBuyer", CLog::TYPE_DEBUG, "user:%s, db_ret:%d, retcode:%d ", account, db_ret, retcode);
		
	if (retcode == 1)
	{
		return DB_ERROR;
	}
	
	id = nId;
	
	return DB_SUCCESS;
}

int CDatabaseWeb::SaveSubAccount(uint32 uid,uint32 fid,uint32 groupid,char* password,char* nickname,char* truename)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

	int n_uid,n_fid,n_groupid,retcode;
	/* VARCHAR var_nickname[41],var_password[60],var_truename[32],db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[41]; } var_nickname;

struct { unsigned short len; unsigned char arr[60]; } var_password;

struct { unsigned short len; unsigned char arr[32]; } var_truename;

struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	n_uid = uid;
	n_fid = fid;
	n_groupid = groupid;

	SET_INIT_NULL(var_nickname);
	SET_INIT_NULL(var_password);
	SET_INIT_NULL(var_truename);
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	strncpy((char*)var_nickname.arr,nickname,MAX_NICKNAME_LEN+1);
	var_nickname.len = (short)strlen((char *)var_nickname.arr);

	strncpy((char*)var_password.arr,password,MAX_PASSWORD_LEN);
	var_password.len = (short)strlen((char *)var_password.arr);

	strncpy((char*)var_truename.arr,truename,MAX_NICKNAME_LEN+1);
	var_truename.len = (short)strlen((char *)var_truename.arr);
	
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;

start:

	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.SubAccountSave(:n_uid,:n_fid,:n_groupid,:var_nickname,:var_password,:var_truename,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 10;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . SubAccountSave ( :n_uid , :n_fid , :n\
_groupid , :var_nickname , :var_password , :var_truename , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )212;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&n_uid;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&n_fid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&n_groupid;
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
 sqlstm.sqhstv[4] = (unsigned char  *)&var_password;
 sqlstm.sqhstl[4] = (unsigned long )62;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&var_truename;
 sqlstm.sqhstl[5] = (unsigned long )34;
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
	CLog::Log("SaveAccountBuyer", CLog::TYPE_DEBUG, "id:%d, db_ret:%d, retcode:%d ", uid, db_ret, retcode);
		
	if (retcode == 1)
	{
		return DB_ERROR;
	}
	
	return DB_SUCCESS;
}

/*================================================================ 
*
* 函 数 名：Get_Basic_UserInfo 
** 根据用户ID获取用户基本信息
* 参 数：
*const uint32 requestID [IN] : 请求者的ID 
* const uint32 c_iUserID [IN] : 要查看用户ID 
* XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo & st_BasicUserInfo [[IN,OUT]] : 用户基本信息结构
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 用户不存在: DB_NOUSER; 操作失败: DB_ERROR;
* 
================================================================*/
int  CDatabaseWeb::Get_Basic_UserInfo(const uint32 requestID, uint32 *iUserID, XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo *pBasicUserInfo, uint8 &nRetCount)
{
	nRetCount=0;
	return DB_SUCCESS;	
}


/*================================================================ 
*
* 函 数 名：Get_User_Info 
** 根据用户ID获取用户详细信息
* 参 数： 
* uint32 iUserID [IN] : 用户ID 
* XT_SERVER_USERINFO_GET_ACK *lpAck [[IN,OUT]] : 用户基本信息结构
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 用户不存在: DB_NOUSER; 操作失败: DB_ERROR;
* 
================================================================*/
int CDatabaseWeb::Get_User_Info(uint32 iUserID, XT_SERVER_USERINFO_GET_ACK &lpAck)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int nUserID;
		double  money;
		int sex,rule,linkprop,familyflag,mobilestatus,integral,career;
		/* VARCHAR  var_email[61],var_username[40]; */ 
struct { unsigned short len; unsigned char arr[61]; } var_email;

struct { unsigned short len; unsigned char arr[40]; } var_username;

		/* VARCHAR  var_addr[81],var_mobile[51],var_desc[501]; */ 
struct { unsigned short len; unsigned char arr[81]; } var_addr;

struct { unsigned short len; unsigned char arr[51]; } var_mobile;

struct { unsigned short len; unsigned char arr[501]; } var_desc;

		/* VARCHAR  var_faceurl[201],var_usersign[101]; */ 
struct { unsigned short len; unsigned char arr[201]; } var_faceurl;

struct { unsigned short len; unsigned char arr[101]; } var_usersign;

		/* VARCHAR  var_birthday[9],var_nickname[41],var_avatoruri[501]; */ 
struct { unsigned short len; unsigned char arr[9]; } var_birthday;

struct { unsigned short len; unsigned char arr[41]; } var_nickname;

struct { unsigned short len; unsigned char arr[501]; } var_avatoruri;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int province,country,city;
		int	retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	SET_INIT_NULL(var_email);
	SET_INIT_NULL(var_username);
	SET_INIT_NULL(var_addr);
	SET_INIT_NULL(var_mobile);
	SET_INIT_NULL(var_desc);
	SET_INIT_NULL(var_faceurl);
	SET_INIT_NULL(var_birthday);
	SET_INIT_NULL(var_nickname);
	SET_INIT_NULL(var_usersign);
	SET_INIT_NULL(var_avatoruri);
	money = 0;
	country = 0;
	province = 0;
	city = 0;
	linkprop = 3;
	familyflag = 0;
	rule = 0;
	mobilestatus = 0;
	integral = 0;
	career = 0;
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nUserID = iUserID;	
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;
	
start:    
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetUserInfo(:nUserID,:var_username,:var_nickname,:sex,:var_usersign,:var_faceurl,:var_email,:rule,:var_mobile,:var_desc,:country,:province,:city,:var_birthday,:familyflag,:var_addr,:linkprop,:money,:var_avatoruri,:mobilestatus,:integral,:career,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetUserInfo ( :nUserID , :var_usernam\
e , :var_nickname , :sex , :var_usersign , :var_faceurl , :var_email , :rule \
, :var_mobile , :var_desc , :country , :province , :city , :var_birthday , :f\
amilyflag , :var_addr , :linkprop , :money , :var_avatoruri , :mobilestatus ,\
 :integral , :career , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )259;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&var_username;
 sqlstm.sqhstl[1] = (unsigned long )42;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_nickname;
 sqlstm.sqhstl[2] = (unsigned long )43;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&sex;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&var_usersign;
 sqlstm.sqhstl[4] = (unsigned long )103;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&var_faceurl;
 sqlstm.sqhstl[5] = (unsigned long )203;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (unsigned char  *)&var_email;
 sqlstm.sqhstl[6] = (unsigned long )63;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         short *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned long )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (unsigned char  *)&rule;
 sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         short *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned long )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (unsigned char  *)&var_mobile;
 sqlstm.sqhstl[8] = (unsigned long )53;
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         short *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned long )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqhstv[9] = (unsigned char  *)&var_desc;
 sqlstm.sqhstl[9] = (unsigned long )503;
 sqlstm.sqhsts[9] = (         int  )0;
 sqlstm.sqindv[9] = (         short *)0;
 sqlstm.sqinds[9] = (         int  )0;
 sqlstm.sqharm[9] = (unsigned long )0;
 sqlstm.sqadto[9] = (unsigned short )0;
 sqlstm.sqtdso[9] = (unsigned short )0;
 sqlstm.sqhstv[10] = (unsigned char  *)&country;
 sqlstm.sqhstl[10] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[10] = (         int  )0;
 sqlstm.sqindv[10] = (         short *)0;
 sqlstm.sqinds[10] = (         int  )0;
 sqlstm.sqharm[10] = (unsigned long )0;
 sqlstm.sqadto[10] = (unsigned short )0;
 sqlstm.sqtdso[10] = (unsigned short )0;
 sqlstm.sqhstv[11] = (unsigned char  *)&province;
 sqlstm.sqhstl[11] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[11] = (         int  )0;
 sqlstm.sqindv[11] = (         short *)0;
 sqlstm.sqinds[11] = (         int  )0;
 sqlstm.sqharm[11] = (unsigned long )0;
 sqlstm.sqadto[11] = (unsigned short )0;
 sqlstm.sqtdso[11] = (unsigned short )0;
 sqlstm.sqhstv[12] = (unsigned char  *)&city;
 sqlstm.sqhstl[12] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[12] = (         int  )0;
 sqlstm.sqindv[12] = (         short *)0;
 sqlstm.sqinds[12] = (         int  )0;
 sqlstm.sqharm[12] = (unsigned long )0;
 sqlstm.sqadto[12] = (unsigned short )0;
 sqlstm.sqtdso[12] = (unsigned short )0;
 sqlstm.sqhstv[13] = (unsigned char  *)&var_birthday;
 sqlstm.sqhstl[13] = (unsigned long )11;
 sqlstm.sqhsts[13] = (         int  )0;
 sqlstm.sqindv[13] = (         short *)0;
 sqlstm.sqinds[13] = (         int  )0;
 sqlstm.sqharm[13] = (unsigned long )0;
 sqlstm.sqadto[13] = (unsigned short )0;
 sqlstm.sqtdso[13] = (unsigned short )0;
 sqlstm.sqhstv[14] = (unsigned char  *)&familyflag;
 sqlstm.sqhstl[14] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[14] = (         int  )0;
 sqlstm.sqindv[14] = (         short *)0;
 sqlstm.sqinds[14] = (         int  )0;
 sqlstm.sqharm[14] = (unsigned long )0;
 sqlstm.sqadto[14] = (unsigned short )0;
 sqlstm.sqtdso[14] = (unsigned short )0;
 sqlstm.sqhstv[15] = (unsigned char  *)&var_addr;
 sqlstm.sqhstl[15] = (unsigned long )83;
 sqlstm.sqhsts[15] = (         int  )0;
 sqlstm.sqindv[15] = (         short *)0;
 sqlstm.sqinds[15] = (         int  )0;
 sqlstm.sqharm[15] = (unsigned long )0;
 sqlstm.sqadto[15] = (unsigned short )0;
 sqlstm.sqtdso[15] = (unsigned short )0;
 sqlstm.sqhstv[16] = (unsigned char  *)&linkprop;
 sqlstm.sqhstl[16] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[16] = (         int  )0;
 sqlstm.sqindv[16] = (         short *)0;
 sqlstm.sqinds[16] = (         int  )0;
 sqlstm.sqharm[16] = (unsigned long )0;
 sqlstm.sqadto[16] = (unsigned short )0;
 sqlstm.sqtdso[16] = (unsigned short )0;
 sqlstm.sqhstv[17] = (unsigned char  *)&money;
 sqlstm.sqhstl[17] = (unsigned long )sizeof(double);
 sqlstm.sqhsts[17] = (         int  )0;
 sqlstm.sqindv[17] = (         short *)0;
 sqlstm.sqinds[17] = (         int  )0;
 sqlstm.sqharm[17] = (unsigned long )0;
 sqlstm.sqadto[17] = (unsigned short )0;
 sqlstm.sqtdso[17] = (unsigned short )0;
 sqlstm.sqhstv[18] = (unsigned char  *)&var_avatoruri;
 sqlstm.sqhstl[18] = (unsigned long )503;
 sqlstm.sqhsts[18] = (         int  )0;
 sqlstm.sqindv[18] = (         short *)0;
 sqlstm.sqinds[18] = (         int  )0;
 sqlstm.sqharm[18] = (unsigned long )0;
 sqlstm.sqadto[18] = (unsigned short )0;
 sqlstm.sqtdso[18] = (unsigned short )0;
 sqlstm.sqhstv[19] = (unsigned char  *)&mobilestatus;
 sqlstm.sqhstl[19] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[19] = (         int  )0;
 sqlstm.sqindv[19] = (         short *)0;
 sqlstm.sqinds[19] = (         int  )0;
 sqlstm.sqharm[19] = (unsigned long )0;
 sqlstm.sqadto[19] = (unsigned short )0;
 sqlstm.sqtdso[19] = (unsigned short )0;
 sqlstm.sqhstv[20] = (unsigned char  *)&integral;
 sqlstm.sqhstl[20] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[20] = (         int  )0;
 sqlstm.sqindv[20] = (         short *)0;
 sqlstm.sqinds[20] = (         int  )0;
 sqlstm.sqharm[20] = (unsigned long )0;
 sqlstm.sqadto[20] = (unsigned short )0;
 sqlstm.sqtdso[20] = (unsigned short )0;
 sqlstm.sqhstv[21] = (unsigned char  *)&career;
 sqlstm.sqhstl[21] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[21] = (         int  )0;
 sqlstm.sqindv[21] = (         short *)0;
 sqlstm.sqinds[21] = (         int  )0;
 sqlstm.sqharm[21] = (unsigned long )0;
 sqlstm.sqadto[21] = (unsigned short )0;
 sqlstm.sqtdso[21] = (unsigned short )0;
 sqlstm.sqhstv[22] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[22] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[22] = (         int  )0;
 sqlstm.sqindv[22] = (         short *)0;
 sqlstm.sqinds[22] = (         int  )0;
 sqlstm.sqharm[22] = (unsigned long )0;
 sqlstm.sqadto[22] = (unsigned short )0;
 sqlstm.sqtdso[22] = (unsigned short )0;
 sqlstm.sqhstv[23] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[23] = (unsigned long )34;
 sqlstm.sqhsts[23] = (         int  )0;
 sqlstm.sqindv[23] = (         short *)0;
 sqlstm.sqinds[23] = (         int  )0;
 sqlstm.sqharm[23] = (unsigned long )0;
 sqlstm.sqadto[23] = (unsigned short )0;
 sqlstm.sqtdso[23] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}

	
		
	SET_END_NULL(var_username);
	SET_END_NULL(var_nickname);
	SET_END_NULL(var_usersign);
	SET_END_NULL(var_faceurl);
	SET_END_NULL(var_email);
	SET_END_NULL(var_desc);
	SET_END_NULL(var_birthday);
	SET_END_NULL(var_addr);
	SET_END_NULL(var_avatoruri);
		
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
	
	CLog::Log("CDatabaseWeb::Get_User_Info", CLog::TYPE_IMPORTANT, "id:%d, db_ret:%d, retcode:%d",nUserID, db_ret, retcode);

	if (db_ret == 1403 || retcode == 1403)
	{	
		
		return DB_NOUSER;
	}

	if( db_ret < 0 && db_ret != -1405) 
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
		
	lpAck.id		= nUserID;	// 0 用户id
	strncpy(lpAck.email, (char*)var_email.arr,MAX_EMAIL_LEN);	// 1 电子邮件
	strncpy(lpAck.username, (char*)var_username.arr,MAX_USERNAME_LEN) ;	// 3 用户名
	strncpy(lpAck.nickname, (char*)var_nickname.arr,MAX_NICKNAME_LEN+1) ;
	strncpy(lpAck.face_url, (char*)var_faceurl.arr,MAX_HEADURL_LEN) ;
	lpAck.sex	= sex          ;
	lpAck.gold_money	= uint32(money * 100);
	lpAck.province = province	 ;//5 省
	lpAck.country = country		 ;//6 国家
	lpAck.city	= city	     ;//7 城市
	lpAck.career	= 0       ;//9 职业
	strncpy(lpAck.birthday, (char*)var_birthday.arr,MAX_BIRTHDAY_LEN) ;
	lpAck.rule	= rule         ;//10 加为好友的认证方式
	lpAck.familyflag = familyflag;
	lpAck.linkpop = linkprop;
	strncpy(lpAck.address, (char*)var_addr.arr,MAX_ADDRESS_LEN) ;//11 联系地址
	strncpy(lpAck.mobile, (char*)var_mobile.arr,MAX_PHONE_LEN)   ;//12 手机
	strncpy(lpAck.description, (char*)var_desc.arr,MAX_DESCRIPTION_LEN); //13 个人说明
	strncpy(lpAck.usersign, (char*)var_usersign.arr,MAX_USERSIGN_LEN);//个人签名
	strncpy(lpAck.avatoruri, (char*)var_avatoruri.arr,MAX_AVATOR_LEN); //闪秀;
	lpAck.mobilestatus = mobilestatus;
	lpAck.integral = (uint32)integral;
	lpAck.career = career;
	return DB_SUCCESS;
}

int CDatabaseWeb::Get_Agent_Merchant(uint32 iUserID, uint32 &MerchantId)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int nUserID;
		unsigned int nMerchantId;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int	retcode;		
		
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	nUserID = iUserID;	
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;
	
start:	  
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.Get_Agent_Merchant(:nUserID,:nMerchantId,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . Get_Agent_Merchant ( :nUserID , :nMer\
chantId , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )370;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&nMerchantId;
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
	
	CLog::Log("CDatabaseWeb::Get_Agent_Merchant", CLog::TYPE_IMPORTANT, "id:%d, db_ret:%d, retcode:%d",nUserID, db_ret, retcode);

	if (db_ret == 1403 || retcode == 1403)
	{	
		
		return DB_NOUSER;
	}

	if( db_ret < 0 && db_ret != -1405) 
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}

	MerchantId = nMerchantId;	
	return DB_SUCCESS;
}

int CDatabaseWeb::Get_User_FriendList(uint32 iUserID, uint16 startPos, XT_FRIENDLIST_ACK::tagFriendInfo * friendInfo, int &nRetCount, uint8 flag)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int nUserID, friendID, nFlag;
		int gid, nStartPos, nReqCount;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		SQL_CURSOR cur_friendlist;
		int	retcode;
	/* EXEC SQL END DECLARE SECTION; */ 


	if (nRetCount <= 0)
		return DB_PARAM;
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	nStartPos		= startPos;
	nReqCount		=  nRetCount;
	
	nUserID = iUserID;
	nFlag = flag;
	
	friendID = 0;
	gid = 0;
	int db_ret;
	/* EXEC SQL ALLOCATE :cur_friendlist; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )401;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendlist;
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



	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;
	
start:

	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetFriendsList(:nUserID,:nStartPos,:nReqCount,:cur_friendlist,:retcode,:nFlag);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetFriendsList ( :nUserID , :nStartPo\
s , :nReqCount , :cur_friendlist , :retcode , :nFlag ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )420;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&nStartPos;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nReqCount;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&cur_friendlist;
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
 sqlstm.sqhstv[5] = (unsigned char  *)&nFlag;
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
		/* EXEC SQL CLOSE :cur_friendlist; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )463;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendlist;
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
	CLog::Log("Get_User_FriendList",CLog::TYPE_IMPORTANT,"db_ret:%d, retcode:%d", db_ret, retcode);

	if(db_ret < 0 && db_ret != -1405)
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL CLOSE :cur_friendlist; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )482;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendlist;
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


		/* EXEC SQL FREE :cur_friendlist; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )501;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendlist;
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

	int iNum = 0;
	for(;;)
	{  
		/* EXEC SQL FETCH :cur_friendlist INTO :friendID,:gid; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )520;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendlist;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&friendID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&gid;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
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
		
		friendInfo[iNum].friendId	= friendID;
		friendInfo[iNum].groupId	= gid;
		++iNum;
	}
	/* EXEC SQL CLOSE :cur_friendlist; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )547;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendlist;
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


	/* EXEC SQL FREE  :cur_friendlist; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )566;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendlist;
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

int CDatabaseWeb::GetSubAccount(uint32 groupid, std::vector<uint32_t>& ids)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int nGroupID,SubID;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		SQL_CURSOR cur_sublist;
		int	retcode;
	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nGroupID = groupid;
	
	int db_ret;
	/* EXEC SQL ALLOCATE :cur_sublist; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )585;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_sublist;
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



	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;
	
start:

	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetSubAccount(:nGroupID,:cur_sublist,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetSubAccount ( :nGroupID , :cur_subl\
ist , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )604;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&nGroupID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&cur_sublist;
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
		/* EXEC SQL CLOSE :cur_sublist; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )635;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_sublist;
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
		/* EXEC SQL CLOSE :cur_sublist; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )654;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_sublist;
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


		/* EXEC SQL FREE :cur_sublist; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )673;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_sublist;
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
		/* EXEC SQL FETCH :cur_sublist INTO :SubID; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )692;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_sublist;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&SubID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
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
		if (db_ret == 1403 )
		{
			break;
		}

		if( db_ret < 0 && db_ret != -1405 )
		{
			sql_error(sqlca, oraca);
			break;
		}		
		
		ids.push_back(SubID);
	}
	
	/* EXEC SQL CLOSE :cur_sublist; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )715;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_sublist;
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


	/* EXEC SQL FREE  :cur_sublist; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )734;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_sublist;
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

int CDatabaseWeb::GetFlowMode(uint32 id, int& mode)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int nID,nMode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int	retcode;
	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nID = id;
	
	int db_ret;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;
	
start:

	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetFlowMode(:nID,:nMode,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetFlowMode ( :nID , :nMode , :retcod\
e ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )753;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&nMode;
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

	if(db_ret < 0 && db_ret != -1405)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}

	if(retcode == 1403)
		return DB_ERROR;
		
	mode = nMode;
	
	return DB_SUCCESS;
}

int CDatabaseWeb::SearchConnAccount(uint32 sid, std::vector<uint32_t>& ids, uint32& did)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int n_sid,n_did,n_time,num;
		/* VARCHAR db_link_name[32],var_id[4000]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

struct { unsigned short len; unsigned char arr[4000]; } var_id;

		int	retcode;
		SQL_CURSOR cur_accounts;
	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	n_sid = sid;
	num = ids.size();
	unsigned int iTime = 0;
	SET_INIT_NULL(var_id);

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int db_ret;
	int iReconnTimes;
	bool bReconn;

	if(num < 100)
	{
		for(unsigned int i = 0;i < num;i++)
		{
			char szUserID[20] = {0};
			itoa(ids[i], szUserID, 20);
			
			strcat((char*)var_id.arr,szUserID);
			
			if (i != num - 1)
				strcat((char*)var_id.arr,",");
		}
		var_id.len = (uint16)strlen((char*)var_id.arr);
	}
	CLog::Log("SearchConnAccount",CLog::TYPE_DEBUG,"var_id.arr is %s\n",(char*)var_id.arr);

	start:
		/* EXEC SQL ALLOCATE :cur_accounts; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )784;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_accounts;
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
				Mini_User_Pkg.SearchConnAccount(:n_sid,:var_id,:cur_accounts,:retcode);
			end;
		END-EXEC; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "begin Mini_User_Pkg . SearchConnAccount ( :n_sid , :var_id\
 , :cur_accounts , :retcode ) ; end ;";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )803;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&n_sid;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&var_id;
  sqlstm.sqhstl[1] = (unsigned long )4002;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&cur_accounts;
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
			/* EXEC SQL CLOSE :cur_accounts; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 24;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )838;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_accounts;
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
			/* EXEC SQL CLOSE :cur_accounts; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 24;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )857;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_accounts;
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


			/* EXEC SQL FREE  :cur_accounts; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 24;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )876;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_accounts;
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
			/* EXEC SQL FETCH :cur_accounts INTO :n_did,n_time; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 24;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )895;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqfoff = (         int )0;
   sqlstm.sqfmod = (unsigned int )2;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_accounts;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&n_did;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&n_time;
   sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
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
			if (db_ret == 1403 )
			{
				break;
			}
			if( db_ret < 0 && db_ret != -1405 )
			{
				sql_error(sqlca, oraca);
				break;
			}	

			CLog::Log("SearchConnAccount",CLog::TYPE_DEBUG,"n_did is %d,n_time is %d\n",n_did,n_time);
			
			if(n_time > iTime)
			{
				iTime = n_time;
				did = n_did;
			}	
		}

		CLog::Log("SearchConnAccount",CLog::TYPE_DEBUG,"the best id is %d\n",did);
		
		/* EXEC SQL CLOSE :cur_accounts; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )922;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_accounts;
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


		/* EXEC SQL FREE  :cur_accounts; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )941;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_accounts;
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

int CDatabaseWeb::GetNoReplyNum(uint32 id,uint32 time,uint32& NoReplyNum)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int n_id,n_time,num;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int	retcode;
	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	n_id = id;
	n_time = time;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int db_ret;
	int iReconnTimes;
	bool bReconn;

	start:
		/* EXEC SQL AT :db_link_name EXECUTE
			begin
				Mini_User_Pkg.GetNoReplyNum(:n_id,:n_time,:num,:retcode);
			end;
		END-EXEC; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "begin Mini_User_Pkg . GetNoReplyNum ( :n_id , :n_time , :n\
um , :retcode ) ; end ;";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )960;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&n_id;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&n_time;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&num;
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
		
		if(db_ret < 0 && db_ret != -1405) 
		{
			sql_error(sqlca, oraca);		
			return DB_ERROR;
		}

		NoReplyNum = num;
		return DB_SUCCESS;	
}

int CDatabaseWeb::Get_User_JoinFlag(uint32 iUserID, uint8 &joinFlag)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int nUserID;
		int rule;
		int	retcode;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	nUserID = iUserID;

	int db_ret;
	rule = 0;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;
	
start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetJoinFlag(:nUserID,:rule,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetJoinFlag ( :nUserID , :rule , :ret\
code ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )995;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&rule;
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

	if( db_ret < 0 && db_ret != -1405) 
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
		 
	if (db_ret == 1403 || retcode == 1403)
		return DB_NOUSER;

	joinFlag = rule;
	return DB_SUCCESS;
}
 
int CDatabaseWeb::Add_Friend(uint32 iUserID, uint32 friendID, uint8 groupID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int  nUserID;
		unsigned int  nFriendID;
		int  nGroupID;
		/* VARCHAR  var_nickname[41], var_username[40]; */ 
struct { unsigned short len; unsigned char arr[41]; } var_nickname;

struct { unsigned short len; unsigned char arr[40]; } var_username;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int	retcode;
	/* EXEC SQL END DECLARE SECTION; */ 
 
	
	SET_INIT_NULL(var_nickname);
	SET_INIT_NULL(var_username);
	nUserID = iUserID;
	nFriendID = friendID;
	nGroupID = groupID;
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	int db_ret;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;
			 
start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.AddFriend(:nUserID,:nFriendID,:nGroupID,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . AddFriend ( :nUserID , :nFriendID , :\
nGroupID , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1026;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&nFriendID;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nGroupID;
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

	if(db_ret < 0 )
	{
		sql_error(sqlca, oraca);		
		return DB_ERROR;
	}    

	if ( retcode == DB_FRIENDS_FULL )
		return DB_FRIENDS_FULL;

	return DB_SUCCESS;
}
	 
int CDatabaseWeb::Delete_Friend(uint32 iUserID, uint32 friendID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int  nUserID,nFriendID;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int	retcode;
	/* EXEC SQL END DECLARE SECTION; */ 
 
	
	nUserID = iUserID;
	nFriendID = friendID;
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
		
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	
	int iReconnTimes;
	bool bReconn;
			
start:  
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.DelFriend(:nUserID,:nFriendID,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . DelFriend ( :nUserID , :nFriendID , :\
retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1061;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&nFriendID;
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
	
	if (db_ret < 0 )
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
	
	return DB_SUCCESS;
}

int CDatabaseWeb::Search_User_WithUserID(uint32 userid, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo)
{
	DEF_ORASET;
		/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR var_username[40],var_nickname[41]; */ 
struct { unsigned short len; unsigned char arr[40]; } var_username;

struct { unsigned short len; unsigned char arr[41]; } var_nickname;

		unsigned int nUserID;
		int     sex, rule;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	SET_INIT_NULL(var_username);
	SET_INIT_NULL(var_nickname);
	
	nUserID = userid;
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
		
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	
	int iReconnTimes;
	bool bReconn;
			
start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.SerarchUserID(:nUserID,:var_nickname,:var_username,:sex,:rule,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . SerarchUserID ( :nUserID , :var_nickn\
ame , :var_username , :sex , :rule , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1092;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&var_nickname;
 sqlstm.sqhstl[1] = (unsigned long )43;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_username;
 sqlstm.sqhstl[2] = (unsigned long )42;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&sex;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&rule;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&retcode;
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
		
	if( (db_ret < 0 && db_ret != -1405))
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
		
	if (db_ret == 1403 || retcode == 1403)
	{
		return DB_NOUSER;
	}
	
	SET_END_NULL(var_username);
	SET_END_NULL(var_nickname);
	friendInfo->id = userid;
	friendInfo->sexy = sex;
	friendInfo->rule = rule;
	strncpy(friendInfo->UserName, (char*)var_username.arr,MAX_USERNAME_LEN+1);
	strncpy(friendInfo->NickName, (char*)var_nickname.arr,MAX_NICKNAME_LEN+1);
	
	return DB_SUCCESS;		
}

int CDatabaseWeb::Search_User_WithUserName(char* UserName, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo)
{
	DEF_ORASET;
		/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR var_username[40],var_nickname[41]; */ 
struct { unsigned short len; unsigned char arr[40]; } var_username;

struct { unsigned short len; unsigned char arr[41]; } var_nickname;

		unsigned int userID;
		int     sex, rule;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	SET_INIT_NULL(var_username);
	SET_INIT_NULL(var_nickname);
	strncpy((char*)var_username.arr, UserName,MAX_USERNAME_LEN);
	var_username.len = (unsigned short)strlen(UserName);
	var_username.arr[var_username.len] = '\0';
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
		
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	
	int iReconnTimes;
	bool bReconn;
	
start:	

	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.SerarchUserName(:var_username,:userID,:var_nickname,:sex,:rule,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . SerarchUserName ( :var_username , :us\
erID , :var_nickname , :sex , :rule , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1135;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&var_username;
 sqlstm.sqhstl[0] = (unsigned long )42;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&userID;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_nickname;
 sqlstm.sqhstl[2] = (unsigned long )43;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&sex;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&rule;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&retcode;
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
		
	if( (db_ret < 0 && db_ret != -1405))
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
		
	if (db_ret == 1403 || retcode == 1403 )
	{
		return DB_NOUSER;
	}

	SET_END_NULL(var_nickname);
	friendInfo->id = userID;
	friendInfo->sexy = sex;
	friendInfo->rule = rule;
	strncpy(friendInfo->UserName, (char*)var_username.arr,MAX_USERNAME_LEN);
	strncpy(friendInfo->NickName, (char*)var_nickname.arr,MAX_NICKNAME_LEN+1);
	
	return DB_SUCCESS;		
}
		
int CDatabaseWeb::Search_User_WithNickName(char* NickName, uint16 startPos, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo, uint8 &nRetCount)
 {
	DEF_ORASET;
		/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR var_username[40], var_nickname[41], var_findnick[40]; */ 
struct { unsigned short len; unsigned char arr[40]; } var_username;

struct { unsigned short len; unsigned char arr[41]; } var_nickname;

struct { unsigned short len; unsigned char arr[40]; } var_findnick;

		unsigned int userID;
		int     sex, rule, nStartPos, nReqPos;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
		SQL_CURSOR cur_searchnickname;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	SET_INIT_NULL(var_username);
	SET_INIT_NULL(var_nickname);
	SET_INIT_NULL(var_findnick);
	strncpy((char*)var_nickname.arr, NickName,MAX_NICKNAME_LEN+1);
	var_nickname.len = (unsigned short)strlen(NickName);
	var_nickname.arr[var_nickname.len] = '\0';
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nStartPos = startPos;
	nReqPos   = nRetCount;
	
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	/* EXEC SQL ALLOCATE :cur_searchnickname; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1178;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchnickname;
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


	int iReconnTimes;
	bool bReconn;
			
start:	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.SerarchNickName(:var_nickname,:nStartPos,:nReqPos,:cur_searchnickname,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . SerarchNickName ( :var_nickname , :nS\
tartPos , :nReqPos , :cur_searchnickname , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1197;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&var_nickname;
 sqlstm.sqhstl[0] = (unsigned long )43;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&nStartPos;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nReqPos;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&cur_searchnickname;
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
		/* EXEC SQL CLOSE :cur_searchnickname; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1236;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchnickname;
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
	
	if( db_ret < 0 && db_ret != -1405)
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL CLOSE :cur_searchnickname; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1255;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchnickname;
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


		/* EXEC SQL FREE  :cur_searchnickname; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1274;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchnickname;
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
	
	int iNum = 0;
	for(;;)
	{  
		/* EXEC SQL FETCH :cur_searchnickname INTO :userID,:var_username,:var_findnick,:sex,:rule; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1293;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchnickname;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&userID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&var_username;
  sqlstm.sqhstl[2] = (unsigned long )42;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&var_findnick;
  sqlstm.sqhstl[3] = (unsigned long )42;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&sex;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&rule;
  sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
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
		if (db_ret == 1403 || ( db_ret < 0 && db_ret != -1405))
		{
			nRetCount = iNum;
			break;
		}
		if (iNum > nRetCount)
		{
			break;
		}	
		
		SET_END_NULL(var_username);
		SET_END_NULL(var_findnick);

		friendInfo[iNum].id = userID;
		friendInfo[iNum].sexy = sex;
		friendInfo[iNum].rule = rule;
		strncpy(friendInfo[iNum].UserName, (char*)var_username.arr,MAX_USERNAME_LEN);
		strncpy(friendInfo[iNum].NickName, (char*)var_findnick.arr,MAX_NICKNAME_LEN+1);
		
		memset(var_username.arr, 0, sizeof(var_username.arr));
		memset(var_findnick.arr, 0, sizeof(var_findnick.arr));
		var_username.len = 0;
		++iNum;
	}

	/* EXEC SQL CLOSE :cur_searchnickname; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1332;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchnickname;
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

	
	/* EXEC SQL FREE  :cur_searchnickname; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1351;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchnickname;
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
	
int CDatabaseWeb::Search_User_WithEmail(char* Email,uint16 startPos,XT_XTREAM_SEARCH_ACK::tagFriendInfo * friendInfo, uint8 &nRetCount)
 {
	DEF_ORASET;
		/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR var_username[40], var_nickname[41], var_email[61]; */ 
struct { unsigned short len; unsigned char arr[40]; } var_username;

struct { unsigned short len; unsigned char arr[41]; } var_nickname;

struct { unsigned short len; unsigned char arr[61]; } var_email;

		unsigned int userID;
		int     sex, rule, nStartPos, nReqPos;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
		SQL_CURSOR cur_searchemail;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	SET_INIT_NULL(var_username);
	SET_INIT_NULL(var_nickname);
	SET_INIT_NULL(var_email);
	strncpy((char*)var_email.arr, Email,60);
	var_email.len = (unsigned short)strlen(Email);
	var_email.arr[var_email.len] = '\0';
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	sex = 2;
	rule = 0;
	
	nStartPos = startPos;
	nReqPos   = nRetCount;
	
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL ALLOCATE :cur_searchemail; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1370;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchemail;
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
			Mini_User_Pkg.SerarchEmail(:var_email,:nStartPos,:nReqPos,:cur_searchemail,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . SerarchEmail ( :var_email , :nStartPo\
s , :nReqPos , :cur_searchemail , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1389;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&var_email;
 sqlstm.sqhstl[0] = (unsigned long )63;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&nStartPos;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nReqPos;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&cur_searchemail;
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
		/* EXEC SQL CLOSE :cur_searchemail; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1428;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchemail;
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

	if( db_ret < 0 && db_ret != -1405)
	{
		sql_error(sqlca, oraca);
		/* EXEC SQL CLOSE :cur_searchemail; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1447;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchemail;
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


		/* EXEC SQL FREE  :cur_searchemail; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1466;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchemail;
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
	
	int iNum = 0;
	for(;;)
	{  
		/* EXEC SQL FETCH :cur_searchemail INTO :userID,:rule,:var_username,:var_nickname,:sex; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1485;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchemail;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&userID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&rule;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&var_username;
  sqlstm.sqhstl[3] = (unsigned long )42;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&var_nickname;
  sqlstm.sqhstl[4] = (unsigned long )43;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&sex;
  sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
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
		if (db_ret == 1403 || (db_ret < 0 && db_ret != -1405))
		{
			nRetCount = iNum;
			break;
		}
		if (iNum > nRetCount)
		{
			break;
		}

		SET_END_NULL(var_username);
		SET_END_NULL(var_nickname);

		friendInfo[iNum].id = userID;
		friendInfo[iNum].sexy = sex;
		friendInfo[iNum].rule = rule;
		strncpy(friendInfo[iNum].UserName, (char*)var_username.arr,MAX_USERNAME_LEN);
		strncpy(friendInfo[iNum].NickName, (char*)var_nickname.arr,MAX_NICKNAME_LEN+1);
		
		memset(var_username.arr, 0, sizeof(var_username.arr));
		var_username.len = 0;
		memset(var_nickname.arr, 0, sizeof(var_nickname.arr));
		var_nickname.len = 0;
		++iNum;
	}

	/* EXEC SQL CLOSE :cur_searchemail; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1524;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchemail;
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


	/* EXEC SQL FREE  :cur_searchemail; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1543;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_searchemail;
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

int CDatabaseWeb::Change_User_PWD(uint32 userid)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 
       
		unsigned int      cno;		
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
		
	cno = userid;
	int db_ret;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;

start:	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.ChangePwd(:cno,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . ChangePwd ( :cno , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1562;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cno;
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

	if( (db_ret < 0 && db_ret != -1405) || db_ret == 1403)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}

	return DB_SUCCESS;	
}

int CDatabaseWeb::Modify_User_Info(XT_USERINFO_MOD_REQ *pUserInfo)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 
       
		unsigned int      cno;
		int      rule,sex,linkprop,familyflag;
		int      province,country,city,career;
		/* VARCHAR  var_email[61],var_pass[90]; */ 
struct { unsigned short len; unsigned char arr[61]; } var_email;

struct { unsigned short len; unsigned char arr[90]; } var_pass;

		/* VARCHAR  var_birthday[9],var_nickname[41],var_testnick[41]; */ 
struct { unsigned short len; unsigned char arr[9]; } var_birthday;

struct { unsigned short len; unsigned char arr[41]; } var_nickname;

struct { unsigned short len; unsigned char arr[41]; } var_testnick;

		/* VARCHAR  var_addr[81],var_mobile[21],var_desc[501],var_url[101]; */ 
struct { unsigned short len; unsigned char arr[81]; } var_addr;

struct { unsigned short len; unsigned char arr[21]; } var_mobile;

struct { unsigned short len; unsigned char arr[501]; } var_desc;

struct { unsigned short len; unsigned char arr[101]; } var_url;
	
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 


	SET_INIT_NULL(var_email);
	SET_INIT_NULL(var_pass);
	SET_INIT_NULL(var_birthday);
	SET_INIT_NULL(var_nickname);
	SET_INIT_NULL(var_testnick);

	SET_INIT_NULL(var_addr);
	SET_INIT_NULL(var_mobile);
	SET_INIT_NULL(var_desc);
	SET_INIT_NULL(var_url);
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	if( !pUserInfo )
		return DB_PARAM;
	cno = pUserInfo->id;
	rule=pUserInfo->rule;
	familyflag = pUserInfo->familyflag; 
	sex=pUserInfo->sex;
	linkprop = pUserInfo->linkpop;

	if(strlen(pUserInfo->email)>=61||
		strlen(pUserInfo->nickname)>=41 ||
		strlen(pUserInfo->mobile)>=21||
		strlen(pUserInfo->description)>=501)
	return DB_PARAM;  

	province=pUserInfo->province;
	country=pUserInfo->country;
	city=pUserInfo->city;
	career=pUserInfo->career;
	strncpy((char*)var_email.arr,pUserInfo->email,MAX_EMAIL_LEN);
	var_email.len = (short)strlen((char *)var_email.arr);

	strncpy((char*)var_pass.arr,pUserInfo->password,MAX_PASSWORD_LEN);
	var_pass.len = (short)strlen((char *)var_pass.arr);  
	strncpy((char*)var_birthday.arr,pUserInfo->birthday,MAX_BIRTHDAY_LEN);/*4*/
	var_birthday.len = (short)strlen((char *)var_birthday.arr);

	strncpy((char*)var_nickname.arr,pUserInfo->nickname,MAX_NICKNAME_LEN+1);/*4*/
	var_nickname.len = (short)strlen((char *)var_nickname.arr);

	strncpy((char*)var_addr.arr,pUserInfo->address,MAX_ADDRESS_LEN);
	var_addr.len = (short)strlen((char *)var_addr.arr);
	strncpy((char*)var_mobile.arr,pUserInfo->mobile,MAX_PHONE_LEN);
	var_mobile.len = (short)strlen((char *)var_mobile.arr);
	strncpy((char*)var_desc.arr,pUserInfo->description,MAX_DESCRIPTION_LEN);
	var_desc.len = (short)strlen((char *)var_desc.arr);
	strncpy((char*)var_url.arr,pUserInfo->face_url,MAX_HEADURL_LEN);
	var_url.len = (short)strlen((char *)var_url.arr);
	
	if (strcmp(pUserInfo->nickname,"") == 0)
	{
	
	}
	else
	{	
		char nickname[41]={0},repname[41]={0};
		strncpy(nickname, pUserInfo->nickname,MAX_NICKNAME_LEN+1);	
		char *p = nickname;
		char *pd = repname;
		while(*p)
		{		
			if( (*p >= 'a' && *p <= 'z') ||
				(*p >= 'A' && *p <= 'Z') || 
				(*p >= '0' && *p <= '9'))
			{	
				*pd++ = *p++;
			}
			else if ((BYTE)*p > 127)
			{
				if( (BYTE)*p == 0xa1 && (BYTE)*(p+1) == 0xa1)
				{
					p += 2;
				}
				else
					*pd++ = *p++;	
			}
			else
				p++;
		}
		*pd = 0;
		
		strncpy((char*)var_testnick.arr, repname,MAX_NICKNAME_LEN+1);
		var_testnick.len = (unsigned short)strlen(repname);	
	}
			
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;

start:
	//昵称不可以重复。		
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.ModifyUserInfo(:cno,:rule,:var_nickname,:var_testnick,:var_pass,:var_desc,:var_email,:var_mobile,:familyflag,:var_birthday,:country,:province,:city,:sex,:var_addr,:linkprop,:career,:var_url,:retcode);
			end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . ModifyUserInfo ( :cno , :rule , :var_\
nickname , :var_testnick , :var_pass , :var_desc , :var_email , :var_mobile ,\
 :familyflag , :var_birthday , :country , :province , :city , :sex , :var_add\
r , :linkprop , :career , :var_url , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1589;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cno;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&rule;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_nickname;
 sqlstm.sqhstl[2] = (unsigned long )43;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&var_testnick;
 sqlstm.sqhstl[3] = (unsigned long )43;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&var_pass;
 sqlstm.sqhstl[4] = (unsigned long )92;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&var_desc;
 sqlstm.sqhstl[5] = (unsigned long )503;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (unsigned char  *)&var_email;
 sqlstm.sqhstl[6] = (unsigned long )63;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         short *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned long )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (unsigned char  *)&var_mobile;
 sqlstm.sqhstl[7] = (unsigned long )23;
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         short *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned long )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (unsigned char  *)&familyflag;
 sqlstm.sqhstl[8] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         short *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned long )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqhstv[9] = (unsigned char  *)&var_birthday;
 sqlstm.sqhstl[9] = (unsigned long )11;
 sqlstm.sqhsts[9] = (         int  )0;
 sqlstm.sqindv[9] = (         short *)0;
 sqlstm.sqinds[9] = (         int  )0;
 sqlstm.sqharm[9] = (unsigned long )0;
 sqlstm.sqadto[9] = (unsigned short )0;
 sqlstm.sqtdso[9] = (unsigned short )0;
 sqlstm.sqhstv[10] = (unsigned char  *)&country;
 sqlstm.sqhstl[10] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[10] = (         int  )0;
 sqlstm.sqindv[10] = (         short *)0;
 sqlstm.sqinds[10] = (         int  )0;
 sqlstm.sqharm[10] = (unsigned long )0;
 sqlstm.sqadto[10] = (unsigned short )0;
 sqlstm.sqtdso[10] = (unsigned short )0;
 sqlstm.sqhstv[11] = (unsigned char  *)&province;
 sqlstm.sqhstl[11] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[11] = (         int  )0;
 sqlstm.sqindv[11] = (         short *)0;
 sqlstm.sqinds[11] = (         int  )0;
 sqlstm.sqharm[11] = (unsigned long )0;
 sqlstm.sqadto[11] = (unsigned short )0;
 sqlstm.sqtdso[11] = (unsigned short )0;
 sqlstm.sqhstv[12] = (unsigned char  *)&city;
 sqlstm.sqhstl[12] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[12] = (         int  )0;
 sqlstm.sqindv[12] = (         short *)0;
 sqlstm.sqinds[12] = (         int  )0;
 sqlstm.sqharm[12] = (unsigned long )0;
 sqlstm.sqadto[12] = (unsigned short )0;
 sqlstm.sqtdso[12] = (unsigned short )0;
 sqlstm.sqhstv[13] = (unsigned char  *)&sex;
 sqlstm.sqhstl[13] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[13] = (         int  )0;
 sqlstm.sqindv[13] = (         short *)0;
 sqlstm.sqinds[13] = (         int  )0;
 sqlstm.sqharm[13] = (unsigned long )0;
 sqlstm.sqadto[13] = (unsigned short )0;
 sqlstm.sqtdso[13] = (unsigned short )0;
 sqlstm.sqhstv[14] = (unsigned char  *)&var_addr;
 sqlstm.sqhstl[14] = (unsigned long )83;
 sqlstm.sqhsts[14] = (         int  )0;
 sqlstm.sqindv[14] = (         short *)0;
 sqlstm.sqinds[14] = (         int  )0;
 sqlstm.sqharm[14] = (unsigned long )0;
 sqlstm.sqadto[14] = (unsigned short )0;
 sqlstm.sqtdso[14] = (unsigned short )0;
 sqlstm.sqhstv[15] = (unsigned char  *)&linkprop;
 sqlstm.sqhstl[15] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[15] = (         int  )0;
 sqlstm.sqindv[15] = (         short *)0;
 sqlstm.sqinds[15] = (         int  )0;
 sqlstm.sqharm[15] = (unsigned long )0;
 sqlstm.sqadto[15] = (unsigned short )0;
 sqlstm.sqtdso[15] = (unsigned short )0;
 sqlstm.sqhstv[16] = (unsigned char  *)&career;
 sqlstm.sqhstl[16] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[16] = (         int  )0;
 sqlstm.sqindv[16] = (         short *)0;
 sqlstm.sqinds[16] = (         int  )0;
 sqlstm.sqharm[16] = (unsigned long )0;
 sqlstm.sqadto[16] = (unsigned short )0;
 sqlstm.sqtdso[16] = (unsigned short )0;
 sqlstm.sqhstv[17] = (unsigned char  *)&var_url;
 sqlstm.sqhstl[17] = (unsigned long )103;
 sqlstm.sqhsts[17] = (         int  )0;
 sqlstm.sqindv[17] = (         short *)0;
 sqlstm.sqinds[17] = (         int  )0;
 sqlstm.sqharm[17] = (unsigned long )0;
 sqlstm.sqadto[17] = (unsigned short )0;
 sqlstm.sqtdso[17] = (unsigned short )0;
 sqlstm.sqhstv[18] = (unsigned char  *)&retcode;
 sqlstm.sqhstl[18] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[18] = (         int  )0;
 sqlstm.sqindv[18] = (         short *)0;
 sqlstm.sqinds[18] = (         int  )0;
 sqlstm.sqharm[18] = (unsigned long )0;
 sqlstm.sqadto[18] = (unsigned short )0;
 sqlstm.sqtdso[18] = (unsigned short )0;
 sqlstm.sqhstv[19] = (unsigned char  *)&db_link_name;
 sqlstm.sqhstl[19] = (unsigned long )34;
 sqlstm.sqhsts[19] = (         int  )0;
 sqlstm.sqindv[19] = (         short *)0;
 sqlstm.sqinds[19] = (         int  )0;
 sqlstm.sqharm[19] = (unsigned long )0;
 sqlstm.sqadto[19] = (unsigned short )0;
 sqlstm.sqtdso[19] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
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
		
	if (retcode  == DB_NICKNAME_EXIST)
	{
		return DB_NICKNAME_EXIST;
	}
	if (retcode  == DB_NICKNAME_ERROR)
	{
		return DB_NICKNAME_ERROR;
	}
	
	return DB_SUCCESS;
}
 
int CDatabaseWeb::Modify_Friend_Comment(uint32 UserID, uint32 friendno, char * comment)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int clientno,friendid;
		/* VARCHAR friendcomment[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } friendcomment;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	SET_INIT_NULL(friendcomment);
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	clientno = UserID;
	friendid = friendno;
	
	strncpy((char*)friendcomment.arr, comment,MAX_COMMENTNAME_LEN);
	friendcomment.len = (short)strlen((char*)friendcomment.arr);
	if (friendcomment.len > 17)
		return DB_PARAM;
	
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;

start: 	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.ModifyFriendAliasname20150824(:clientno,:friendid,:friendcomment,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . ModifyFriendAliasname20150824 ( :clie\
ntno , :friendid , :friendcomment , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1684;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&friendid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&friendcomment;
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

	if( db_ret < 0 )
	{		
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}

	if (db_ret == 1403)
	{
		return DB_NOUSER;
	}
	
	return DB_SUCCESS;	
}
		
int	CDatabaseWeb::Update_User_Money(uint32 clientno, int sendmoney)
{
	return DB_SUCCESS;	
}
 
int CDatabaseWeb::Modify_User_UnderWrite(uint32 UserID, char* userUnderWrite)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int		userid;
		/* VARCHAR				var_underwrite[101]; */ 
struct { unsigned short len; unsigned char arr[101]; } var_underwrite;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	SET_INIT_NULL(var_underwrite);
	userid = UserID;
	
	strncpy((char*)var_underwrite.arr, userUnderWrite,MAX_USERSIGN_LEN);
	var_underwrite.len = (short)strlen((char*)var_underwrite.arr);
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;

start:
		
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.ModifyUserSign(:userid,:var_underwrite,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . ModifyUserSign ( :userid , :var_under\
write , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1719;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&var_underwrite;
 sqlstm.sqhstl[1] = (unsigned long )103;
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

int CDatabaseWeb::Modify_Friend_Flag(uint32 UserID, uint32 friendno, uint8 flag)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int		userid, friendid, iloveflag;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	userid = UserID;
	friendid = friendno;
	iloveflag = flag;
	
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;

start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.ModifyFriendLoveFlag(:userid,:friendid,:iloveflag,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . ModifyFriendLoveFlag ( :userid , :fri\
endid , :iloveflag , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1750;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&friendid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&iloveflag;
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
		sql_error(sqlca, oraca);;
		return DB_ERROR;
	}
	if (db_ret == 1403)
	{
		return DB_NOFRIEND;
	}
	
	return DB_SUCCESS;
}


//nRetCount  请求条数，返回条数
int CDatabaseWeb::Get_Web_Msg(uint32 &recid, ST_WEB_MSG *pWebMsg, uint16 &nRetCount)//每隔一定时间查一次表
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     recvid,sendid,rectID, nReqCount;
		short   imsgtype;
		/* VARCHAR var_time[20], var_msg[1401]; */ 
struct { unsigned short len; unsigned char arr[20]; } var_time;

struct { unsigned short len; unsigned char arr[1401]; } var_msg;
//MAX_MSG_LEN
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
		SQL_CURSOR cur_webmsg;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	rectID = recid;
	nReqCount = nRetCount;
	
	SET_INIT_NULL(var_msg);
	SET_INIT_NULL(var_time);
	
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL ALLOCATE :cur_webmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1785;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_webmsg;
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
			Mini_User_Pkg.GetClientMsg(:rectID,:nReqCount,:cur_webmsg,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetClientMsg ( :rectID , :nReqCount ,\
 :cur_webmsg , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1804;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&rectID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&nReqCount;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&cur_webmsg;
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
		/* EXEC SQL CLOSE :cur_webmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1839;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_webmsg;
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
		/* EXEC SQL CLOSE :cur_webmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1858;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_webmsg;
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


		/* EXEC SQL FREE  :cur_webmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1877;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_webmsg;
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
	
	int iNum = 0;
	
	for(;;)
	{  
		/* EXEC SQL FETCH :cur_webmsg INTO :rectID,:recvid, :sendid,:imsgtype,:var_msg,:var_time; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1896;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_webmsg;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&rectID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&recvid;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&sendid;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&imsgtype;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&var_msg;
  sqlstm.sqhstl[5] = (unsigned long )1403;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&var_time;
  sqlstm.sqhstl[6] = (unsigned long )22;
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
		if (db_ret == 1403 || iNum >= nRetCount)
		{
			recid = rectID;
			nRetCount = iNum;
			break;
		}
		if( db_ret < 0 && db_ret != -1405 )
		{					
			recid = rectID;
			nRetCount = iNum;
			break;
		}
		SET_END_NULL(var_msg);
		SET_END_NULL(var_time);
		
		pWebMsg[iNum].recID			= rectID;
		pWebMsg[iNum].recvID		= recvid;
		pWebMsg[iNum].sendID		= sendid;
		pWebMsg[iNum].msgtype	= (unsigned short)imsgtype;  
		memcpy((char*)pWebMsg[iNum].msg, var_msg.arr,var_msg.len + 1);
		memcpy(pWebMsg[iNum].sendTime, (char*)var_time.arr, var_time.len + 1);

		SET_INIT_NULL(var_msg);
		SET_INIT_NULL(var_time);

		++iNum;		
	}

	/* EXEC SQL CLOSE :cur_webmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1939;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_webmsg;
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


	/* EXEC SQL FREE  :cur_webmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1958;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_webmsg;
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

int CDatabaseWeb::Get_Offline_WebMsg(uint32 userid, ST_WEB_MSG *pWebMsg, uint16 &nRetCount)//用户登录时获取网站消息
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int rectID,sendID,recvID;
		int			 reqCount;
		short   imsgtype;
		/* VARCHAR var_time[20], var_msg[1401]; */ 
struct { unsigned short len; unsigned char arr[20]; } var_time;

struct { unsigned short len; unsigned char arr[1401]; } var_msg;
//MAX_MSG_LEN
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
		SQL_CURSOR cur_offwebmsg;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	recvID = userid;
	reqCount = nRetCount;
	
	SET_INIT_NULL(var_msg);
	SET_INIT_NULL(var_time);
	
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;	
	
	/* EXEC SQL ALLOCATE :cur_offwebmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1977;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_offwebmsg;
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



start:	//bloggerid 发送方博客Id ;  userid 接收方用户ID
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetOfflineClientMsg(:recvID,:reqCount,:cur_offwebmsg,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetOfflineClientMsg ( :recvID , :reqC\
ount , :cur_offwebmsg , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1996;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&recvID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&reqCount;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&cur_offwebmsg;
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
		/* EXEC SQL CLOSE :cur_offwebmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2031;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_offwebmsg;
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
		/* EXEC SQL CLOSE :cur_offwebmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2050;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_offwebmsg;
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


		/* EXEC SQL FREE  :cur_offwebmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2069;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_offwebmsg;
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
	
	int iNum = 0;
	
	for(;;)
	{  
		/* EXEC SQL FETCH :cur_offwebmsg INTO :rectID, :sendID,:imsgtype,:var_msg,:var_time; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2088;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_offwebmsg;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&rectID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&sendID;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&imsgtype;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(short);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&var_msg;
  sqlstm.sqhstl[4] = (unsigned long )1403;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&var_time;
  sqlstm.sqhstl[5] = (unsigned long )22;
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
		if (db_ret == 1403 || iNum >= nRetCount)
		{			
			nRetCount = iNum;			
			break;
		}
		if( db_ret < 0 && db_ret != -1405 )
		{
			sql_error(sqlca, oraca);					
			nRetCount = iNum;
			break;
		}

		SET_END_NULL(var_msg);
		SET_END_NULL(var_time);
		pWebMsg[iNum].recID			= rectID;
		pWebMsg[iNum].recvID		= userid;
		pWebMsg[iNum].sendID		= sendID;
		pWebMsg[iNum].msgtype	= (unsigned short)imsgtype;  
		memcpy((char*)pWebMsg[iNum].msg, (char*)var_msg.arr, var_msg.len + 1);
		memcpy(pWebMsg[iNum].sendTime, (char*)var_time.arr, var_time.len + 1);	 	
		SET_INIT_NULL(var_msg);
		SET_INIT_NULL(var_time);

		++iNum;	
	}

	/* EXEC SQL CLOSE :cur_offwebmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2127;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_offwebmsg;
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


	/* EXEC SQL FREE  :cur_offwebmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2146;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_offwebmsg;
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
int CDatabaseWeb::SendFriendInvite(uint32 userID, char *emailAddress, char *emailTitle, char *emailContent)
{
	return DB_SUCCESS;

}

int CDatabaseWeb::Set_WebMsg_Status(uint32 recID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     recid;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	recid = recID;
	int db_ret;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;

start: 		
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.SetClientMsgStatus(:recid,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . SetClientMsgStatus ( :recid , :retcod\
e ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2165;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&recid;
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
	
	if (db_ret < 0 || db_ret == 1403)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;	
	}

	return DB_SUCCESS;
}
 
int CDatabaseWeb::Get_User_FriendGroup(uint32 UserID, uint32 friendno, uint8 &groupID)     
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int  nUserID;
		unsigned int  nFriendID;
		int  nGroupID;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 
 
	
	nUserID = UserID;
	nFriendID = friendno;

	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	int db_ret;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;
		 
start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetFriendGroup(:nUserID,:nFriendID,:nGroupID,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetFriendGroup ( :nUserID , :nFriendI\
D , :nGroupID , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2192;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&nFriendID;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nGroupID;
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
	
	if (db_ret < 0 )
	{
		sql_error(sqlca, oraca);
		groupID = GROUPID_BLACKLIST;		
		return DB_ERROR;
	}

	if (db_ret == 1403)
	{
		groupID = GROUPID_BLACKLIST;		
		return DB_SUCCESS;
	}
	groupID = nGroupID;		
		
	return DB_SUCCESS;
}

int CDatabaseWeb::Modify_Friend_Group(uint32 userID, uint32 friendID, uint8 groupID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int  nUserID;
		unsigned int  nFriendID;
		int  nGroupID;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 
 
	
	nUserID = userID;
	nFriendID = friendID;
	nGroupID = groupID;

	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	int db_ret;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	int iReconnTimes;
	bool bReconn;
		 
start:
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.ModifyFriendGroup(:nUserID,:nFriendID,:nGroupID,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . ModifyFriendGroup ( :nUserID , :nFrie\
ndID , :nGroupID , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2227;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&nFriendID;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nGroupID;
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

	if (db_ret < 0 )
	{
		sql_error(sqlca, oraca);		
		return DB_ERROR;
	}
		
	return DB_SUCCESS;
}

int CDatabaseWeb::Get_UserFriend_Comment(uint32 UserID, uint32 friendID, char* comment)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     userid, friendid;
		/* VARCHAR  var_aliasname[40]; */ 
struct { unsigned short len; unsigned char arr[40]; } var_aliasname;

		/* VARCHAR	 db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);
	
		SET_INIT_NULL(var_aliasname);
		userid			= UserID;
		friendid		= friendID;
		memset(comment, 0, MAX_COMMENTNAME_LEN);

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
    
	int db_ret;	  
	int iReconnTimes;
	bool bReconn;
	
start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetFriendAliasName(:userid,:friendid,:var_aliasname,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetFriendAliasName ( :userid , :frien\
did , :var_aliasname , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2262;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&friendid;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&var_aliasname;
 sqlstm.sqhstl[2] = (unsigned long )42;
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

	if ( db_ret == 1403 || retcode == 1403)
		return DB_NOUSER;

	if( db_ret < 0 && db_ret != -1405)	//1405 允许部分字段为空
	{
		char szError[255];
		memset(szError, 0, sizeof(szError));
		sprintf(szError, "Get_UserFriend_Comment() ID:%d, ErrorCode:%d",UserID, db_ret); 
		sql_error(sqlca, oraca);
		return CError::DB_Error(DB_ERROR, szError);
	}

	if (db_ret == -1405)
		comment[0] = '\0';
	else
	{
		SET_END_NULL(var_aliasname);
		strncpy(comment, (char*)var_aliasname.arr,MAX_COMMENTNAME_LEN);
	}

	return DB_SUCCESS;
}

int  CDatabaseWeb::Get_Basic_UserInfo2(const uint32 requestID, uint32 *iUserID, XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo *pBasicUserInfo, uint16 &nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR	 db_link_name[32],var_userid[4000]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

struct { unsigned short len; unsigned char arr[4000]; } var_userid;

		unsigned int nUserid,nReqUserID;
		/* VARCHAR var_username[121],var_nickname[121],var_faceurl[201],var_usersign[101],var_commentname[32],var_guid[40]; */ 
struct { unsigned short len; unsigned char arr[121]; } var_username;

struct { unsigned short len; unsigned char arr[121]; } var_nickname;

struct { unsigned short len; unsigned char arr[201]; } var_faceurl;

struct { unsigned short len; unsigned char arr[101]; } var_usersign;

struct { unsigned short len; unsigned char arr[32]; } var_commentname;

struct { unsigned short len; unsigned char arr[40]; } var_guid;

		int sex,nLoveFlag,mobilestatus;
		int		 retcode;
		SQL_CURSOR cur_userbasicinfo;
		SQL_CURSOR cur_friendsbasicinfo;
	/* EXEC SQL END DECLARE SECTION; */ 


	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);
	
	nReqUserID = requestID;

	int iNum = 0;
	mobilestatus = 0;
	SET_INIT_NULL(var_username);
	SET_INIT_NULL(var_nickname);
	SET_INIT_NULL(var_faceurl);
	SET_INIT_NULL(var_usersign);
	SET_INIT_NULL(var_commentname);
	SET_INIT_NULL(var_userid);
	SET_INIT_NULL(var_guid);


	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
    
	int db_ret;	
	int iReconnTimes;
	bool bReconn;	
	
	for(int i = 0; i < nRetCount; ++i)
	{	
		char szUserID[10] = {0};
		itoa(iUserID[i], szUserID, 10);
		strcat((char*)var_userid.arr,szUserID);
		if (i != nRetCount -1)
		strcat((char*)var_userid.arr,",");
	}
	var_userid.len = (uint16)strlen((char*)var_userid.arr);
	CLog::Log("Get_Basic_UserInfo2",CLog::TYPE_DEBUG,"var_userid.arr:%s",(char*)var_userid.arr);

start:  
	if (requestID == 0)
	{
		/* EXEC SQL ALLOCATE :cur_userbasicinfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2297;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_userbasicinfo;
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
				Mini_User_Pkg.GetUserBaseInfo(:var_userid,:cur_userbasicinfo,:retcode);
			end;
		END-EXEC; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "begin Mini_User_Pkg . GetUserBaseInfo ( :var_userid , :cur\
_userbasicinfo , :retcode ) ; end ;";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2316;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&var_userid;
  sqlstm.sqhstl[0] = (unsigned long )4002;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&cur_userbasicinfo;
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
			/* EXEC SQL CLOSE :cur_userbasicinfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 24;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2347;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_userbasicinfo;
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
			/* EXEC SQL CLOSE :cur_userbasicinfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 24;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2366;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_userbasicinfo;
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


			/* EXEC SQL FREE  :cur_userbasicinfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 24;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2385;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_userbasicinfo;
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

		iNum = 0;
		for(;;) //USERID,USERNAME, NICKNAME,USERSEX,USERPICURI
		{  
			pBasicUserInfo[iNum].LoveFlag = 0;
			memset(pBasicUserInfo[iNum].szUserSign, 0, sizeof(pBasicUserInfo[iNum].szUserSign));
			memset(pBasicUserInfo[iNum].szCommentName, 0, sizeof(pBasicUserInfo[iNum].szCommentName));
			memset(pBasicUserInfo[iNum].szGuid, 0, sizeof(pBasicUserInfo[iNum].szGuid));
			/* EXEC SQL FETCH :cur_userbasicinfo INTO :nUserid,:var_username,:var_nickname,:sex,:var_faceurl,:mobilestatus,var_guid; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 24;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2404;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqfoff = (         int )0;
   sqlstm.sqfmod = (unsigned int )2;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_userbasicinfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&nUserid;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&var_username;
   sqlstm.sqhstl[2] = (unsigned long )123;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&var_nickname;
   sqlstm.sqhstl[3] = (unsigned long )123;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)&sex;
   sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (unsigned char  *)&var_faceurl;
   sqlstm.sqhstl[5] = (unsigned long )203;
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         short *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned long )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqhstv[6] = (unsigned char  *)&mobilestatus;
   sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[6] = (         int  )0;
   sqlstm.sqindv[6] = (         short *)0;
   sqlstm.sqinds[6] = (         int  )0;
   sqlstm.sqharm[6] = (unsigned long )0;
   sqlstm.sqadto[6] = (unsigned short )0;
   sqlstm.sqtdso[6] = (unsigned short )0;
   sqlstm.sqhstv[7] = (unsigned char  *)&var_guid;
   sqlstm.sqhstl[7] = (unsigned long )42;
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
			SET_END_NULL(var_username);
			SET_END_NULL(var_nickname);
			SET_END_NULL(var_faceurl);
			SET_END_NULL(var_guid);
			
			pBasicUserInfo[iNum].nID = nUserid;
			strncpy(pBasicUserInfo[iNum].szUserName,(char*)var_username.arr,MAX_USERNAME_LEN);					
			strncpy(pBasicUserInfo[iNum].szNickName,(char*)var_nickname.arr,MAX_NICKNAME_LEN+1);				
			pBasicUserInfo[iNum].nSexy = sex;			
			strncpy(pBasicUserInfo[iNum].szHeadUrl,(char*)var_faceurl.arr,MAX_HEADURL_LEN);	
			pBasicUserInfo[iNum].mobileStatus = mobilestatus;				
			strncpy(pBasicUserInfo[iNum].szGuid,(char*)var_guid.arr,MAX_GUID_LEN);
			CLog::Log("Get_Basic_UserInfo2",CLog::TYPE_IMPORTANT,"1Userid:%d,var_username:%s",nUserid,(char*)var_username.arr);
			++iNum;
		}
		/* EXEC SQL CLOSE :cur_userbasicinfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2451;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_userbasicinfo;
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


		/* EXEC SQL FREE  :cur_userbasicinfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2470;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_userbasicinfo;
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
	else
	{  
		/* EXEC SQL ALLOCATE :cur_friendsbasicinfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2489;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendsbasicinfo;
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
				Mini_User_Pkg.GetFriendsBaseInfo_20150729(:nReqUserID,:var_userid,:cur_friendsbasicinfo,:retcode);
			end;
		END-EXEC; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "begin Mini_User_Pkg . GetFriendsBaseInfo_20150729 ( :nReqU\
serID , :var_userid , :cur_friendsbasicinfo , :retcode ) ; end ;";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2508;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&nReqUserID;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&var_userid;
  sqlstm.sqhstl[1] = (unsigned long )4002;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&cur_friendsbasicinfo;
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
			/* EXEC SQL CLOSE :cur_friendsbasicinfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 24;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2543;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendsbasicinfo;
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
			/* EXEC SQL CLOSE :cur_friendsbasicinfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 24;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2562;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendsbasicinfo;
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


			/* EXEC SQL FREE  :cur_friendsbasicinfo; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 24;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2581;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendsbasicinfo;
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

		iNum = 0;
		for(;;) //USERID,USERNAME, NICKNAME,USERSEX,USERPICURI
		{  
			/* EXEC SQL FETCH :cur_friendsbasicinfo INTO :nUserid,:var_username,:var_nickname,:sex,:var_faceurl,:var_usersign,:var_commentname,:nLoveFlag,:mobilestatus,:var_guid; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 24;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2600;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqfoff = (         int )0;
   sqlstm.sqfmod = (unsigned int )2;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendsbasicinfo;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&nUserid;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&var_username;
   sqlstm.sqhstl[2] = (unsigned long )123;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&var_nickname;
   sqlstm.sqhstl[3] = (unsigned long )123;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)&sex;
   sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (unsigned char  *)&var_faceurl;
   sqlstm.sqhstl[5] = (unsigned long )203;
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         short *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned long )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqhstv[6] = (unsigned char  *)&var_usersign;
   sqlstm.sqhstl[6] = (unsigned long )103;
   sqlstm.sqhsts[6] = (         int  )0;
   sqlstm.sqindv[6] = (         short *)0;
   sqlstm.sqinds[6] = (         int  )0;
   sqlstm.sqharm[6] = (unsigned long )0;
   sqlstm.sqadto[6] = (unsigned short )0;
   sqlstm.sqtdso[6] = (unsigned short )0;
   sqlstm.sqhstv[7] = (unsigned char  *)&var_commentname;
   sqlstm.sqhstl[7] = (unsigned long )34;
   sqlstm.sqhsts[7] = (         int  )0;
   sqlstm.sqindv[7] = (         short *)0;
   sqlstm.sqinds[7] = (         int  )0;
   sqlstm.sqharm[7] = (unsigned long )0;
   sqlstm.sqadto[7] = (unsigned short )0;
   sqlstm.sqtdso[7] = (unsigned short )0;
   sqlstm.sqhstv[8] = (unsigned char  *)&nLoveFlag;
   sqlstm.sqhstl[8] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[8] = (         int  )0;
   sqlstm.sqindv[8] = (         short *)0;
   sqlstm.sqinds[8] = (         int  )0;
   sqlstm.sqharm[8] = (unsigned long )0;
   sqlstm.sqadto[8] = (unsigned short )0;
   sqlstm.sqtdso[8] = (unsigned short )0;
   sqlstm.sqhstv[9] = (unsigned char  *)&mobilestatus;
   sqlstm.sqhstl[9] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[9] = (         int  )0;
   sqlstm.sqindv[9] = (         short *)0;
   sqlstm.sqinds[9] = (         int  )0;
   sqlstm.sqharm[9] = (unsigned long )0;
   sqlstm.sqadto[9] = (unsigned short )0;
   sqlstm.sqtdso[9] = (unsigned short )0;
   sqlstm.sqhstv[10] = (unsigned char  *)&var_guid;
   sqlstm.sqhstl[10] = (unsigned long )42;
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
			SET_END_NULL(var_username);
			SET_END_NULL(var_nickname);
			SET_END_NULL(var_faceurl);	
			SET_END_NULL(var_usersign);
			SET_END_NULL(var_commentname);
			SET_END_NULL(var_guid);
	
			pBasicUserInfo[iNum].nID = nUserid;
			strncpy(pBasicUserInfo[iNum].szUserName,(char*)var_username.arr,MAX_USERNAME_LEN);					
			strncpy(pBasicUserInfo[iNum].szNickName,(char*)var_nickname.arr,MAX_NICKNAME_LEN+1);				
			pBasicUserInfo[iNum].nSexy = sex;			
			strncpy(pBasicUserInfo[iNum].szHeadUrl,(char*)var_faceurl.arr,MAX_HEADURL_LEN);						
			strncpy(pBasicUserInfo[iNum].szUserSign,(char*)var_usersign.arr,MAX_USERSIGN_LEN);					
			strncpy(pBasicUserInfo[iNum].szCommentName,(char*)var_commentname.arr,MAX_COMMENTNAME_LEN);					
			pBasicUserInfo[iNum].LoveFlag = nLoveFlag;	
			pBasicUserInfo[iNum].mobileStatus = mobilestatus;
			strncpy(pBasicUserInfo[iNum].szGuid,(char*)var_guid.arr,MAX_GUID_LEN);		

			CLog::Log("Get_Basic_UserInfo2",CLog::TYPE_IMPORTANT,"2Userid:%d,var_username:%s",
				nUserid,(char*)var_username.arr);
			++iNum;
		}
		/* EXEC SQL CLOSE :cur_friendsbasicinfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2659;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendsbasicinfo;
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


		/* EXEC SQL FREE  :cur_friendsbasicinfo; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2678;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_friendsbasicinfo;
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
	return DB_SUCCESS;	
}

int CDatabaseWeb::Add_Register_Info(LPXT_REGIST_REQ pRegInfo, uint32 &RetUserID, uint8 &ReturnCode)
{
	RetUserID = 0;
	ReturnCode=0;
	return DB_SUCCESS;
}


int  CDatabaseWeb::Get_User_FamilyFlag(uint32 *iUserID, XT_USERFAMILYFLAG_GET_ACK::tagUserFamilyFlag *pUserFamilyFlag, uint8 &nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR	 db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		/* VARCHAR var_userid[4000]; */ 
struct { unsigned short len; unsigned char arr[4000]; } var_userid;

		int		 retcode, nLoveFlag;
		unsigned int nUserID;
		SQL_CURSOR cur_userfamilyflag;
	/* EXEC SQL END DECLARE SECTION; */ 


	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);
	
/*
	if (nRetCount > 500)
	{
		nRetCount = 0;
		return DB_SUCCESS;
	}
*/

	SET_INIT_NULL(var_userid);
	
	for(int i = 0; i < nRetCount; ++i)
	{	
		char szUserID[10] = {0};
		itoa(iUserID[i], szUserID, 10);
		strcat((char*)var_userid.arr,szUserID);
		if (i != nRetCount -1)
			strcat((char*)var_userid.arr,",");
	}
	var_userid.len = (uint16)strlen((char*)var_userid.arr);
	

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
    
	/* EXEC SQL ALLOCATE :cur_userfamilyflag; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2697;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_userfamilyflag;
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


	int db_ret;	
	int iReconnTimes;
	bool bReconn;

start:  
	
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetUserJoin2Family(:var_userid,:cur_userfamilyflag,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetUserJoin2Family ( :var_userid , :c\
ur_userfamilyflag , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2716;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&var_userid;
 sqlstm.sqhstl[0] = (unsigned long )4002;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&cur_userfamilyflag;
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
		/* EXEC SQL CLOSE :cur_userfamilyflag; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2747;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_userfamilyflag;
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
		/* EXEC SQL CLOSE :cur_userfamilyflag; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2766;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_userfamilyflag;
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


		/* EXEC SQL FREE  :cur_userfamilyflag; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2785;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_userfamilyflag;
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

	int iNum = 0;
	for(;;) //USERID,USERNAME, NICKNAME,USERSEX,USERPICURI
	{  
		/* EXEC SQL FETCH :cur_userfamilyflag INTO :nUserID,:nLoveFlag; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2804;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_userfamilyflag;
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
  sqlstm.sqhstv[2] = (unsigned char  *)&nLoveFlag;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
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

		pUserFamilyFlag[iNum].uID = nUserID;
		pUserFamilyFlag[iNum].flag = nLoveFlag;	

		++iNum;
	}
	/* EXEC SQL CLOSE :cur_userfamilyflag; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2831;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_userfamilyflag;
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


	/* EXEC SQL FREE  :cur_userfamilyflag; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2850;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_userfamilyflag;
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

//nRetCount  请求条数，返回条数
int CDatabaseWeb::Get_Operation_Msg(uint32 &recid, ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount)//每隔一定时间查一次表
{
	nRetCount=0;
	return DB_SUCCESS;
}
int CDatabaseWeb::Get_OneOperation_Msg(uint32 &recid, char* strFriend,ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount)
{
	nRetCount=0;
	return DB_SUCCESS;
}

//nRetCount  请求条数，返回条数
int CDatabaseWeb::Get_User_Msg(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount)//收件箱
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     recvID,sendID,rectID,msgID,nReqCount;
		/* VARCHAR var_time[32], var_title[201],var_content[901]; */ 
struct { unsigned short len; unsigned char arr[32]; } var_time;

struct { unsigned short len; unsigned char arr[201]; } var_title;

struct { unsigned short len; unsigned char arr[901]; } var_content;
//MAX_MSG_LEN
		/* VARCHAR db_link_name[32], var_nickname[41], var_username[40]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

struct { unsigned short len; unsigned char arr[41]; } var_nickname;

struct { unsigned short len; unsigned char arr[40]; } var_username;

		int		 retcode,msgtype;
		SQL_CURSOR cur_usermsg;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	rectID = recid;
	nReqCount = nRetCount;
	
	SET_INIT_NULL(var_time);
	SET_INIT_NULL(var_title);
	SET_INIT_NULL(var_content);
	SET_INIT_NULL(var_nickname);
	SET_INIT_NULL(var_username);

	
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

	/* EXEC SQL ALLOCATE :cur_usermsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2869;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_usermsg;
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
			Mini_User_Pkg.GetUserMsg(:rectID,:nReqCount,:cur_usermsg,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetUserMsg ( :rectID , :nReqCount , :\
cur_usermsg , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )2888;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&rectID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&nReqCount;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&cur_usermsg;
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
		/* EXEC SQL CLOSE :cur_usermsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2923;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_usermsg;
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
		/* EXEC SQL CLOSE :cur_usermsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2942;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_usermsg;
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


		/* EXEC SQL FREE  :cur_usermsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2961;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_usermsg;
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
	
	int iNum = 0;
	
	//a.RECID,a.USERID,a.MSGID,b.userid,b.title,b.content,b.msgtype,TO_CHAR(b.createDate,'YYYY/MM/DD HH24:MI:SS'),c.nickname
	for(;;)
	{  
		/* EXEC SQL FETCH :cur_usermsg INTO :rectID,:recvID,:msgID,:sendID,:var_title,:var_content,:msgtype,:var_time,:var_nickname,:var_username; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2980;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_usermsg;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&rectID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&recvID;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&msgID;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&sendID;
  sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&var_title;
  sqlstm.sqhstl[5] = (unsigned long )203;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&var_content;
  sqlstm.sqhstl[6] = (unsigned long )903;
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         short *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned long )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (unsigned char  *)&msgtype;
  sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         short *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned long )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (unsigned char  *)&var_time;
  sqlstm.sqhstl[8] = (unsigned long )34;
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         short *)0;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned long )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqhstv[9] = (unsigned char  *)&var_nickname;
  sqlstm.sqhstl[9] = (unsigned long )43;
  sqlstm.sqhsts[9] = (         int  )0;
  sqlstm.sqindv[9] = (         short *)0;
  sqlstm.sqinds[9] = (         int  )0;
  sqlstm.sqharm[9] = (unsigned long )0;
  sqlstm.sqadto[9] = (unsigned short )0;
  sqlstm.sqtdso[9] = (unsigned short )0;
  sqlstm.sqhstv[10] = (unsigned char  *)&var_username;
  sqlstm.sqhstl[10] = (unsigned long )42;
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
		if (db_ret == 1403 || iNum >= nRetCount)
		{
			recid = rectID;
			nRetCount = iNum;
			break;
		}
		if( db_ret < 0 && db_ret != -1405 )
		{	
			sql_error(sqlca, oraca);					
			recid = rectID;
			nRetCount = iNum;
			break;
		}
		SET_END_NULL(var_title);
		SET_END_NULL(var_content);
		SET_END_NULL(var_time);
		SET_END_NULL(var_nickname);
		SET_END_NULL(var_username);
		pUserMsg[iNum].type		= ST_WOKU_MSG::USERRECVMSG;
		pUserMsg[iNum].recID		= rectID;
		pUserMsg[iNum].recvID		= recvID;
		pUserMsg[iNum].sendID		= sendID;
		memcpy((char*)pUserMsg[iNum].nickname, (char*)var_nickname.arr, var_nickname.len + 1);
		memcpy((char*)pUserMsg[iNum].username, (char*)var_username.arr, var_username.len + 1);
		pUserMsg[iNum].classID		= msgtype;
		pUserMsg[iNum].logID		= 0;
		memset(pUserMsg[iNum].srcuri, 0, sizeof(pUserMsg[iNum].srcuri));
		memcpy((char*)pUserMsg[iNum].title, var_title.arr,var_title.len + 1);
		memcpy((char*)pUserMsg[iNum].content, var_content.arr,var_content.len + 1);
		memcpy((char*)pUserMsg[iNum].sendTime, (char*)var_time.arr, var_time.len + 1);
		SET_INIT_NULL(var_time);
		SET_INIT_NULL(var_title);
		SET_INIT_NULL(var_content);
		SET_INIT_NULL(var_nickname);
		SET_INIT_NULL(var_username);

		++iNum;		
	}

	/* EXEC SQL CLOSE :cur_usermsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3039;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_usermsg;
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


	/* EXEC SQL FREE  :cur_usermsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3058;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_usermsg;
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

int CDatabaseWeb::Get_OneUser_Msg(uint32 &recid, uint32 userID,ST_WOKU_MSG *pUserMsg, uint16 &nRetCount)//收件箱
{
	nRetCount=0;
	return DB_SUCCESS;
}
	
	 
int CDatabaseWeb::Get_Sys_Msg(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount)//系统广播
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     msgID;
		int nReqCount;
		/* VARCHAR var_time[32], var_title[64],var_content[512],var_toid[512]; */ 
struct { unsigned short len; unsigned char arr[32]; } var_time;

struct { unsigned short len; unsigned char arr[64]; } var_title;

struct { unsigned short len; unsigned char arr[512]; } var_content;

struct { unsigned short len; unsigned char arr[512]; } var_toid;
//MAX_MSG_LEN
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
		SQL_CURSOR cur_sysmsg;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	msgID = recid;
	nReqCount = nRetCount;
	
	SET_INIT_NULL(var_time);
	SET_INIT_NULL(var_title);
	SET_INIT_NULL(var_content);
	SET_INIT_NULL(var_toid);
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL ALLOCATE :cur_sysmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3077;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_sysmsg;
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


	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


 start:    
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetSysMsg(:msgID,:nReqCount,:cur_sysmsg,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetSysMsg ( :msgID , :nReqCount , :cu\
r_sysmsg , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3096;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&msgID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&nReqCount;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&cur_sysmsg;
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
		/* EXEC SQL CLOSE :cur_sysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3131;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_sysmsg;
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


		/* EXEC SQL FREE  :cur_sysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3150;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_sysmsg;
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
	
	int iNum = 0;
	
	//MSGID,title,content,TO_CHAR(createDate,'YYYY/MM/DD HH24:MI:SS')
	for(;;)
	{  
		/* EXEC SQL FETCH :cur_sysmsg INTO :msgID,:var_title,:var_content,:var_time,:var_toid; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3169;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_sysmsg;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&msgID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&var_title;
  sqlstm.sqhstl[2] = (unsigned long )66;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&var_content;
  sqlstm.sqhstl[3] = (unsigned long )514;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&var_time;
  sqlstm.sqhstl[4] = (unsigned long )34;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&var_toid;
  sqlstm.sqhstl[5] = (unsigned long )514;
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
		if (db_ret == 1403 || iNum >= nRetCount)
		{
			nRetCount = iNum;
			break;
		}
		if( db_ret < 0 && db_ret != -1405 )
		{					
			sql_error(sqlca, oraca);	
			nRetCount = iNum;
			break;
		}
		
		SET_END_NULL(var_title);
		SET_END_NULL(var_content);
		SET_END_NULL(var_time);
		SET_END_NULL(var_toid);
		
		pUserMsg[iNum].type		= ST_WOKU_MSG::SYSTEMPUB;
		pUserMsg[iNum].recID		= msgID;
		pUserMsg[iNum].recvID		= 0;
		pUserMsg[iNum].sendID		= 0;
		memset(pUserMsg[iNum].nickname, 0,sizeof(pUserMsg[iNum].nickname));
		pUserMsg[iNum].classID		= 0;
		pUserMsg[iNum].logID		= 0;
		memset(pUserMsg[iNum].srcuri, 0, sizeof(pUserMsg[iNum].srcuri));
		memcpy((char*)pUserMsg[iNum].title, var_title.arr,var_title.len + 1);
		memcpy((char*)pUserMsg[iNum].content, var_content.arr,var_content.len + 1);
		memcpy((char*)pUserMsg[iNum].sendTime, (char*)var_time.arr, var_time.len + 1);
		memcpy((char*)pUserMsg[iNum].toids, (char*)var_toid.arr, var_toid.len + 1);

		if(msgID > recid)
			recid = msgID;
		
		++iNum;		
	}

	/* EXEC SQL CLOSE :cur_sysmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3208;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_sysmsg;
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


	/* EXEC SQL FREE  :cur_sysmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3227;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_sysmsg;
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

int CDatabaseWeb::Get_SysOff_Msg(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount, char* uid)//离线系统消息
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int msgID;
		int nReqCount;
		/* VARCHAR var_time[32], var_title[64],var_content[512],var_id[64]; */ 
struct { unsigned short len; unsigned char arr[32]; } var_time;

struct { unsigned short len; unsigned char arr[64]; } var_title;

struct { unsigned short len; unsigned char arr[512]; } var_content;

struct { unsigned short len; unsigned char arr[64]; } var_id;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int retcode;
		SQL_CURSOR cur_sysmsg;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	strncpy((char*)var_id.arr, uid, 64);
	var_id.len = (unsigned short)strlen((char*)var_id.arr);

	msgID = recid;
	nReqCount = nRetCount;
	
	SET_INIT_NULL(var_time);
	SET_INIT_NULL(var_title);
	SET_INIT_NULL(var_content);
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL ALLOCATE :cur_sysmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3246;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_sysmsg;
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


	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


 start:    
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetSysOffMsg(:msgID,:var_id,:nReqCount,:cur_sysmsg,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetSysOffMsg ( :msgID , :var_id , :nR\
eqCount , :cur_sysmsg , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3265;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&msgID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&var_id;
 sqlstm.sqhstl[1] = (unsigned long )66;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nReqCount;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&cur_sysmsg;
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
		/* EXEC SQL CLOSE :cur_sysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3304;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_sysmsg;
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


		/* EXEC SQL FREE  :cur_sysmsg; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3323;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_sysmsg;
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
	
	int iNum = 0;
	
	for(;;)
	{  
		/* EXEC SQL FETCH :cur_sysmsg INTO :msgID,:var_title,:var_content,:var_time; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 24;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3342;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_sysmsg;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&msgID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&var_title;
  sqlstm.sqhstl[2] = (unsigned long )66;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&var_content;
  sqlstm.sqhstl[3] = (unsigned long )514;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&var_time;
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
		if (db_ret == 1403 || iNum >= nRetCount)
		{
			nRetCount = iNum;
			break;
		}
		if( db_ret < 0 && db_ret != -1405 )
		{					
			sql_error(sqlca, oraca);	
			nRetCount = iNum;
			break;
		}
		
		SET_END_NULL(var_title);
		SET_END_NULL(var_content);
		SET_END_NULL(var_time);
		
		pUserMsg[iNum].type		= ST_WOKU_MSG::SYSTEMPUB;
		pUserMsg[iNum].recID		= msgID;
		pUserMsg[iNum].recvID		= 0;
		pUserMsg[iNum].sendID		= 0;
		memset(pUserMsg[iNum].nickname, 0,sizeof(pUserMsg[iNum].nickname));
		pUserMsg[iNum].classID		= 0;
		pUserMsg[iNum].logID		= 0;
		memset(pUserMsg[iNum].srcuri, 0, sizeof(pUserMsg[iNum].srcuri));
		memcpy((char*)pUserMsg[iNum].title, var_title.arr,var_title.len + 1);
		memcpy((char*)pUserMsg[iNum].content, var_content.arr,var_content.len + 1);
		memcpy((char*)pUserMsg[iNum].sendTime, (char*)var_time.arr, var_time.len + 1);

		if(msgID > recid)
			recid = msgID;
		
		++iNum;		
	}

	/* EXEC SQL CLOSE :cur_sysmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3377;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_sysmsg;
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


	/* EXEC SQL FREE  :cur_sysmsg; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3396;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_sysmsg;
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
 
int CDatabaseWeb::Get_Broadcast_Msg(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount)//用户广播
{
	nRetCount=0;
	return DB_SUCCESS;
}
 
int CDatabaseWeb::Get_UserComment(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount)//个人主页评论
{
	nRetCount=0;
	return DB_SUCCESS;
}

 
int CDatabaseWeb::Get_OneUserComment(uint32 &recid, uint32 userid,ST_WOKU_MSG *pUserMsg, uint16 &nRetCount)//个人主页评论
{
	nRetCount=0;
	return DB_SUCCESS;
}

int CDatabaseWeb::Get_LogComment(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount)//博客内容评论
{
	nRetCount=0;
	return DB_SUCCESS;
}

int CDatabaseWeb::Get_OneLogComment(uint32 &recid, uint32 userid,ST_WOKU_MSG *pUserMsg, uint16 &nRetCount)//博客内容评论
{
	nRetCount=0;
	return DB_SUCCESS;
}

int CDatabaseWeb::SetUserMessageStatus(uint32 recID)
{
	return DB_SUCCESS;
}

int CDatabaseWeb::Get_System_Log(uint32 &recid, ST_SYSTEM_LOG *pSystemLog, uint16 &nRetCount)
{
	nRetCount=0;
	return DB_SUCCESS;
}

int CDatabaseWeb::GetNickName(uint32 userid,char *nickname)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     userID;
		/* VARCHAR var_nickname[41]; */ 
struct { unsigned short len; unsigned char arr[41]; } var_nickname;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	userID = userid;
	
	SET_INIT_NULL(var_nickname);
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetNickName(:userID,:var_nickname,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetNickName ( :userID , :var_nickname\
 , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3415;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&userID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&var_nickname;
 sqlstm.sqhstl[1] = (unsigned long )43;
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

	if (retcode != 1403 && db_ret != 1403 && db_ret >= 0)
	{
		SET_END_NULL(var_nickname);
		strncpy(nickname,(char*)var_nickname.arr,MAX_NICKNAME_LEN+1);
		return DB_SUCCESS;
	}
		
	if (db_ret<0)
	{
		sql_error(sqlca,oraca);
		return DB_ERROR;
	}

	return DB_SUCCESS;
}

int CDatabaseWeb::GetUserPicUri(uint32 userid,char *picuri)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     userID;
		/* VARCHAR var_picuri[33]; */ 
struct { unsigned short len; unsigned char arr[33]; } var_picuri;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	userID = userid;
	
	SET_INIT_NULL(var_picuri);
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_User_Pkg.GetUserPicUri(:userID,:var_picuri,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 24;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_User_Pkg . GetUserPicUri ( :userID , :var_picuri\
 , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )3446;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&userID;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)&var_picuri;
 sqlstm.sqhstl[1] = (unsigned long )35;
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

	if (retcode != 1403 && db_ret != 1403 && db_ret >= 0)
	{
		SET_END_NULL(var_picuri);
		strncpy(picuri,(char*)var_picuri.arr,MAX_HEADURL_LEN);
		return DB_SUCCESS;
	}
		
	if (db_ret < 0)
	{
		sql_error(sqlca,oraca);
		return DB_ERROR;
	}

	return DB_SUCCESS;
}
int CDatabaseWeb::Write_System_Log(uint32 projid,char* szUserid,char* szLogid,uint32 flatid,uint32 operid)
{
	return DB_SUCCESS;
}

int CDatabaseWeb::PayMoney(uint32 userid,uint32 consumetype, uint32 logid,float money)
{
	return DB_SUCCESS;
}


int CDatabaseWeb::Get_Mobile_Msg(uint32 &recid, ST_MOBILE_MSG *pUserMsg, uint16 &nRetCount)//手机短信
{
	nRetCount=0;
	return DB_SUCCESS;
}



int CDatabaseWeb::Get_OneMobile_Msg(uint32 &recid, uint32 UserID,ST_MOBILE_MSG *pUserMsg, uint16 &nRetCount)//手机短信
{
	nRetCount=0;
	return DB_SUCCESS;
}

int CDatabaseWeb::Send_Mobile_Msg(uint32 UserID,char *szNickName, ST_MOBILE_SEND *pMsg)
{
	return DB_SUCCESS;
}

void ForDisableBuildWarningWeb()
{
	unsigned int nTest =  sqlstm.sqlvsn;
	nTest++;
}
