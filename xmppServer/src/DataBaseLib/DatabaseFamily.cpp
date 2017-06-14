
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
           char  filnam[20];
};
static const struct sqlcxp sqlfpn =
{
    19,
    "./DatabaseFamily.pc"
};


static unsigned int sqlctx = 27273731;


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

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4138,852,0,0,
5,0,0,0,0,0,568,90,0,0,1,1,0,1,0,3,102,0,0,
24,0,0,1,88,0,1542,95,0,0,3,3,0,0,128,3,68,0,0,3,102,0,0,3,3,0,0,1,9,0,0,
55,0,0,0,0,0,527,104,0,0,1,1,0,1,0,1,102,0,0,
74,0,0,0,0,0,527,119,0,0,1,1,0,1,0,1,102,0,0,
93,0,0,0,0,0,590,121,0,0,1,1,0,1,0,3,102,0,0,
112,0,0,0,0,0,525,132,0,0,2,1,0,1,0,1,102,0,0,2,68,0,0,
135,0,0,0,0,0,527,137,0,0,1,1,0,1,0,1,102,0,0,
154,0,0,0,0,0,527,163,0,0,1,1,0,1,0,1,102,0,0,
173,0,0,0,0,0,590,164,0,0,1,1,0,1,0,3,102,0,0,
192,0,0,2,273,0,1542,218,0,0,14,14,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,3,3,0,0,3,
3,0,0,3,3,0,0,3,9,0,0,3,9,0,0,3,9,0,0,3,9,0,0,3,3,0,0,3,9,0,0,3,9,0,0,3,3,0,0,
1,9,0,0,
267,0,0,3,174,0,1542,362,0,0,8,8,0,0,128,3,68,0,0,3,3,0,0,3,3,0,0,3,9,0,0,3,9,
0,0,3,9,0,0,3,9,0,0,3,3,0,0,1,9,0,0,
318,0,0,0,0,0,568,432,0,0,1,1,0,1,0,3,102,0,0,
337,0,0,4,90,0,1542,436,0,0,3,3,0,0,128,3,68,0,0,3,102,0,0,3,3,0,0,1,9,0,0,
368,0,0,0,0,0,527,445,0,0,1,1,0,1,0,1,102,0,0,
387,0,0,0,0,0,527,460,0,0,1,1,0,1,0,1,102,0,0,
406,0,0,0,0,0,590,462,0,0,1,1,0,1,0,3,102,0,0,
425,0,0,0,0,0,525,472,0,0,3,1,0,1,0,1,102,0,0,2,68,0,0,2,68,0,0,
452,0,0,0,0,0,527,481,0,0,1,1,0,1,0,1,102,0,0,
471,0,0,0,0,0,590,482,0,0,1,1,0,1,0,3,102,0,0,
490,0,0,0,0,0,527,494,0,0,1,1,0,1,0,1,102,0,0,
509,0,0,0,0,0,590,495,0,0,1,1,0,1,0,3,102,0,0,
528,0,0,5,182,0,1542,591,0,0,9,9,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,3,3,0,0,3,3,
0,0,3,9,0,0,3,9,0,0,3,9,0,0,3,3,0,0,1,9,0,0,
583,0,0,6,99,0,1542,678,0,0,4,4,0,0,128,3,68,0,0,3,9,0,0,3,3,0,0,3,3,0,0,1,9,0,
0,
618,0,0,7,84,0,1542,740,0,0,3,3,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,1,9,0,0,
649,0,0,8,97,0,1542,803,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,68,0,0,3,3,0,0,1,9,
0,0,
684,0,0,9,167,0,1542,881,0,0,8,8,0,0,128,3,68,0,0,3,9,0,0,3,9,0,0,3,3,0,0,3,3,
0,0,3,68,0,0,3,68,0,0,3,3,0,0,1,9,0,0,
735,0,0,0,0,0,568,928,0,0,1,1,0,1,0,3,102,0,0,
754,0,0,10,123,0,1542,930,0,0,5,5,0,0,128,3,3,0,0,3,3,0,0,3,3,0,0,3,102,0,0,3,
3,0,0,1,9,0,0,
793,0,0,0,0,0,527,939,0,0,1,1,0,1,0,1,102,0,0,
812,0,0,0,0,0,527,954,0,0,1,1,0,1,0,1,102,0,0,
831,0,0,0,0,0,590,955,0,0,1,1,0,1,0,3,102,0,0,
850,0,0,0,0,0,525,963,0,0,7,1,0,1,0,1,102,0,0,2,68,0,0,2,9,0,0,2,9,0,0,2,3,0,0,
2,3,0,0,2,68,0,0,
893,0,0,0,0,0,527,992,0,0,1,1,0,1,0,1,102,0,0,
912,0,0,0,0,0,590,993,0,0,1,1,0,1,0,3,102,0,0,
931,0,0,0,0,0,568,999,0,0,1,1,0,1,0,3,102,0,0,
950,0,0,11,124,0,1542,1001,0,0,5,5,0,0,128,3,9,0,0,3,3,0,0,3,3,0,0,3,102,0,0,3,
3,0,0,1,9,0,0,
989,0,0,0,0,0,527,1010,0,0,1,1,0,1,0,1,102,0,0,
1008,0,0,0,0,0,527,1025,0,0,1,1,0,1,0,1,102,0,0,
1027,0,0,0,0,0,590,1026,0,0,1,1,0,1,0,3,102,0,0,
1046,0,0,0,0,0,525,1034,0,0,8,1,0,1,0,1,102,0,0,2,68,0,0,2,9,0,0,2,9,0,0,2,3,0,
0,2,3,0,0,2,68,0,0,2,68,0,0,
1093,0,0,0,0,0,527,1065,0,0,1,1,0,1,0,1,102,0,0,
1112,0,0,0,0,0,590,1066,0,0,1,1,0,1,0,3,102,0,0,
1131,0,0,12,70,0,1542,1107,0,0,2,2,0,0,128,3,68,0,0,3,3,0,0,1,9,0,0,
1158,0,0,13,88,0,1542,1160,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,3,3,0,0,1,
9,0,0,
1193,0,0,14,81,0,1542,1215,0,0,3,3,0,0,128,3,68,0,0,3,68,0,0,3,3,0,0,1,9,0,0,
1224,0,0,0,0,0,568,1266,0,0,1,1,0,1,0,3,102,0,0,
1243,0,0,15,86,0,1542,1269,0,0,3,3,0,0,128,3,68,0,0,3,102,0,0,3,3,0,0,1,9,0,0,
1274,0,0,0,0,0,527,1278,0,0,1,1,0,1,0,1,102,0,0,
1293,0,0,0,0,0,527,1293,0,0,1,1,0,1,0,1,102,0,0,
1312,0,0,0,0,0,590,1294,0,0,1,1,0,1,0,3,102,0,0,
1331,0,0,0,0,0,525,1301,0,0,3,1,0,1,0,1,102,0,0,2,68,0,0,2,9,0,0,
1358,0,0,0,0,0,527,1319,0,0,1,1,0,1,0,1,102,0,0,
1377,0,0,0,0,0,590,1320,0,0,1,1,0,1,0,3,102,0,0,
1396,0,0,16,91,0,1542,1349,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,9,0,0,3,3,0,0,1,
9,0,0,
1431,0,0,17,91,0,1542,1406,0,0,4,4,0,0,128,3,68,0,0,3,68,0,0,3,9,0,0,3,3,0,0,1,
9,0,0,
};


#include "stdafx.h"

#define SQLCA_NONE 
#define ORACA_NONE
#include <sqlcpr.h>
#include <string>

#include "DatabaseFamily.h"
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
//CPOOL=YES CMIN = 5 CMAX = 15 	

CDatabaseFamily::CDatabaseFamily(void)
{

}

CDatabaseFamily::~CDatabaseFamily(void)
{

}

void CDatabaseFamily::SetConnectDB(char* uid, char* pwd, char* sid)
{
	strncpy(familyuid, uid,31);
	strncpy(familypwd,pwd,31);
	strncpy(familysid, sid,31);
}

//连接WEB数据库  (初始化连接名，上下文)
bool CDatabaseFamily::ConnectDB()
{
	return ConnectFamily();
}


/*================================================================ 
*
* 函 数 名：Get_Family_List 
** 根据用户ID获取用户的加入的固定群列表
* 参 数： 
* const uint32 c_iUserID [IN] : 用户ID 
* uint32 *FamilyID [[IN,OUT]] : 用户加入的家族数组
* int &FamilyCount	[IN, OUT] : 要获取的家族数目和返回的数目, 如果为0，则输出为用户加入的家族数目
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 用户不存在: DB_NOUSER; 操作失败: DB_ERROR;
* 
================================================================*/

int CDatabaseFamily::Get_Family_List(const uint32 c_iUserID, uint32 *FamilyID, int &FamilyCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		int ret_count;
		unsigned int      i_userid, i_familyid;
		/* VARCHAR	 db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
		SQL_CURSOR cur_familylist;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);
	
	ret_count = 0;
	i_userid = c_iUserID;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
    
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	if (FamilyCount == 0)
		FamilyCount = MAX_PERMFAMILY_NUM;
	/* EXEC SQL ALLOCATE :cur_familylist; */ 

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
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_familylist;
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
			Mini_Family_Pkg.GetFamilyList(:i_userid,:cur_familylist,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . GetFamilyList ( :i_userid , :cur_fa\
milylist , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )24;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&cur_familylist;
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
		/* EXEC SQL CLOSE :cur_familylist; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )55;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_familylist;
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
		/* EXEC SQL CLOSE :cur_familylist; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )74;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_familylist;
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


		FamilyCount = 0;
		/* EXEC SQL FREE  :cur_familylist; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )93;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_familylist;
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
	
	//EXEC SQL WHENEVER NOT FOUND DO BREAK;
	while(1)
	{		
		if( ret_count >= FamilyCount )	//指定获取的数目
		{			
			break;
		}
		/* EXEC SQL FETCH :cur_familylist INTO :i_familyid; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )112;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_familylist;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&i_familyid;
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
		if (conn_error(db_ret))
		{
			ret_count=0;
			/* EXEC SQL CLOSE :cur_familylist; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )135;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_familylist;
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
		
		if( db_ret == 1403 )
		{			
			FamilyCount = ret_count;
			break;
		} 
		if( db_ret < 0 )
		{
			sql_error(sqlca, oraca);
			break;
		}
		FamilyID[ret_count] = i_familyid;	
		++ret_count;
	}

	/* EXEC SQL CLOSE :cur_familylist; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )154;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_familylist;
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


	/* EXEC SQL FREE :cur_familylist; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )173;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_familylist;
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


	FamilyCount = ret_count;
	return  DB_SUCCESS;
}

/*================================================================ 
*
* 函 数 名：Get_Family_Info 
** 根据家族ID获取家族信息
* 参 数： 
* const uint32 c_iFamilyID [IN] : 家族ID 
* LPXT_GET_CINFO_ACK lpAck [[IN,OUT]] : 家族的详细信息
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 传入的参数错误：DB_PARAM；家族不存在: DB_NOFAMILY; 操作失败: DB_ERROR;
* 
================================================================*/

int CDatabaseFamily::Get_Family_Info(const uint32 c_iFamilyID, LPXT_GET_CINFO_ACK lpAck)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int  nFamilyID, nCreatorID;
		int		 nFamilyType, nFamilyClass,  nJoinFlag, nViewFlag, nMemberLimit;
		/* VARCHAR  var_familyname[101],var_familydesc[501], var_familydomain[21], var_familyfaceurl[101], var_familysign[101], var_familynotice[101]; */ 
struct { unsigned short len; unsigned char arr[101]; } var_familyname;

struct { unsigned short len; unsigned char arr[501]; } var_familydesc;

struct { unsigned short len; unsigned char arr[21]; } var_familydomain;

struct { unsigned short len; unsigned char arr[101]; } var_familyfaceurl;

struct { unsigned short len; unsigned char arr[101]; } var_familysign;

struct { unsigned short len; unsigned char arr[101]; } var_familynotice;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int		 retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	if (!lpAck)
		return DB_PARAM;
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen(m_Connect);
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
  
	
	nFamilyID = c_iFamilyID;
	int db_ret;
	nFamilyType				= 0;
	nFamilyClass			= 0;
	nJoinFlag				= 0;
	nViewFlag				= 0;
	nCreatorID				= 0;	

	SET_INIT_NULL(var_familyname);
	SET_INIT_NULL(var_familydesc);
	SET_INIT_NULL(var_familydomain);
	SET_INIT_NULL(var_familyfaceurl);
	SET_INIT_NULL(var_familysign);
	SET_INIT_NULL(var_familynotice);
	
	int iReconnTimes;
	bool bReconn;

start:

	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Family_Pkg.GetFamilyInfo(:nFamilyID,:nCreatorID,:nFamilyType,:nFamilyClass,:nViewFlag,:nJoinFlag,:var_familyname,:var_familydesc,:var_familydomain,:var_familyfaceurl,:nMemberLimit,:var_familysign, :var_familynotice,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . GetFamilyInfo ( :nFamilyID , :nCrea\
torID , :nFamilyType , :nFamilyClass , :nViewFlag , :nJoinFlag , :var_familyn\
ame , :var_familydesc , :var_familydomain , :var_familyfaceurl , :nMemberLimi\
t , :var_familysign , :var_familynotice , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )192;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&nCreatorID;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nFamilyType;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&nFamilyClass;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&nViewFlag;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&nJoinFlag;
 sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (unsigned char  *)&var_familyname;
 sqlstm.sqhstl[6] = (unsigned long )103;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         short *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned long )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (unsigned char  *)&var_familydesc;
 sqlstm.sqhstl[7] = (unsigned long )503;
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         short *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned long )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (unsigned char  *)&var_familydomain;
 sqlstm.sqhstl[8] = (unsigned long )23;
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         short *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned long )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqhstv[9] = (unsigned char  *)&var_familyfaceurl;
 sqlstm.sqhstl[9] = (unsigned long )103;
 sqlstm.sqhsts[9] = (         int  )0;
 sqlstm.sqindv[9] = (         short *)0;
 sqlstm.sqinds[9] = (         int  )0;
 sqlstm.sqharm[9] = (unsigned long )0;
 sqlstm.sqadto[9] = (unsigned short )0;
 sqlstm.sqtdso[9] = (unsigned short )0;
 sqlstm.sqhstv[10] = (unsigned char  *)&nMemberLimit;
 sqlstm.sqhstl[10] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[10] = (         int  )0;
 sqlstm.sqindv[10] = (         short *)0;
 sqlstm.sqinds[10] = (         int  )0;
 sqlstm.sqharm[10] = (unsigned long )0;
 sqlstm.sqadto[10] = (unsigned short )0;
 sqlstm.sqtdso[10] = (unsigned short )0;
 sqlstm.sqhstv[11] = (unsigned char  *)&var_familysign;
 sqlstm.sqhstl[11] = (unsigned long )103;
 sqlstm.sqhsts[11] = (         int  )0;
 sqlstm.sqindv[11] = (         short *)0;
 sqlstm.sqinds[11] = (         int  )0;
 sqlstm.sqharm[11] = (unsigned long )0;
 sqlstm.sqadto[11] = (unsigned short )0;
 sqlstm.sqtdso[11] = (unsigned short )0;
 sqlstm.sqhstv[12] = (unsigned char  *)&var_familynotice;
 sqlstm.sqhstl[12] = (unsigned long )103;
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
	
	if( db_ret < 0 && db_ret != -1405)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}

  
	if (retcode == 1403)
	{
		char szError[255];
		sprintf(szError, "%s, %d\n", "Get_Family_Info()",c_iFamilyID); 
		return CError::DB_Error(DB_NOFAMILY, szError);
	}
	
	SET_END_NULL(var_familyfaceurl);
	SET_END_NULL(var_familyname);
	SET_END_NULL(var_familydesc);
	SET_END_NULL(var_familydomain);
	SET_END_NULL(var_familysign);
	SET_END_NULL(var_familynotice);
	
	lpAck->nCreatorID		= nCreatorID;	
	lpAck->nFamilyType		= nFamilyType;		//家族类型 FAMILY_COMMON 普通家族，FAMILY_VIP VIP家族
	lpAck->nFamilyClass		= nFamilyClass;		//分类
	lpAck->nFamilyFaceID	= 0;				//族徽ID
	strncpy(lpAck->szFamilyFaceUrl, (char*)var_familyfaceurl.arr,MAX_FAMILYFACEURL_LEN);	//族徽链接地址
	lpAck->nJoinFlag		= nJoinFlag;		//加入规则 FAMILY_JOIN_ANY|FAMILY_JOIN_VALI|FAMILY_JOIN_UNALLOW
	lpAck->nViewFlag		= nViewFlag;		//页面浏览权限	FAMILY_VIEW_MEM|FAMILY_VIEW_REG|FAMILY_VIEW_ALL
	strncpy(lpAck->szFamilyName, (char*)var_familyname.arr,MAX_FAMILYNAME_LEN);
	strncpy(lpAck->szFamilyDesc, (char*)var_familydesc.arr,MAX_FAMILYDESC_LEN);
	strncpy(lpAck->szFamilyDomain, (char*)var_familydomain.arr,MAX_FAMILYDOMAIN_LEN);
	lpAck->nMemberLimit = nMemberLimit;
	strncpy(lpAck->szFamilySign, (char*)var_familysign.arr,MAX_FAMILYNAME_LEN);
	strncpy(lpAck->szFamilyNotice, (char*)var_familynotice.arr,MAX_FAMILYNAME_LEN);
		
	return DB_SUCCESS;
}


/*================================================================ 
*
* 函 数 名：Set_Family_Info 
** 设置家族信息
* 参 数： 
* LPXT_SET_CINFO_REQ lpReq [[IN]] : 家族的详细信息
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 传入的参数错误：DB_PARAM；家族不存在: DB_NOFAMILY; 操作失败: DB_ERROR;
* 
================================================================*/
int	CDatabaseFamily::Set_Family_Info(LPXT_SET_CINFO_REQ lpReq)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int      nFamilyID;
		int      nJoinFlag,nFamilyClass;
		/* VARCHAR  var_familyname[101],/ovar_testname[31],o/var_familydesc[501],var_familyfaceurl[101],var_familysign[101],var_familynotice[101]; */ 
struct { unsigned short len; unsigned char arr[101]; } var_familyname;

struct { unsigned short len; unsigned char arr[501]; } var_familydesc;

struct { unsigned short len; unsigned char arr[101]; } var_familyfaceurl;

struct { unsigned short len; unsigned char arr[101]; } var_familysign;

struct { unsigned short len; unsigned char arr[101]; } var_familynotice;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

		
	if( !lpReq )
		return DB_PARAM;
		
	SET_INIT_NULL(var_familyname);
	SET_INIT_NULL(var_familydesc);
	SET_INIT_NULL(var_familyfaceurl);
	SET_INIT_NULL(var_familysign);
	SET_INIT_NULL(var_familynotice);
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nFamilyID		= lpReq->nFamilyID;
	nJoinFlag		= lpReq->nJoinFlag;
	nFamilyClass	= lpReq->nFamilyClass;
	
	strncpy((char*)var_familyname.arr,lpReq->szFamilyName,MAX_FAMILYNAME_LEN);
	var_familyname.len = (uint16)strlen((char*)var_familyname.arr);
	strncpy((char*)var_familydesc.arr,lpReq->szFamilyDesc,MAX_FAMILYDESC_LEN);
	var_familydesc.len = (uint16)strlen((char*)var_familydesc.arr);
	strncpy((char*)var_familyfaceurl.arr,lpReq->szFamilyFaceUrl,MAX_FAMILYFACEURL_LEN);
	var_familyfaceurl.len = (uint16)strlen((char*)var_familyfaceurl.arr);
	strncpy((char*)var_familysign.arr,lpReq->szFamilySign,MAX_FAMILYNAME_LEN);
	var_familysign.len = (uint16)strlen((char*)var_familysign.arr);
	strncpy((char*)var_familynotice.arr,lpReq->szFamilyNotice,MAX_FAMILYNAME_LEN);
	var_familynotice.len = (uint16)strlen((char*)var_familynotice.arr);

	/*
	char nickname[33]={0},repname[33]={0};
	strncpy(nickname, lpReq->szFamilyName,MAX_FAMILYNAME_LEN);	
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
	
	strncpy((char*)var_testname.arr, repname,MAX_FAMILYNAME_LEN);
	var_testname.len = (unsigned short)strlen(repname);	
	*/
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	
	
start:

	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Family_Pkg.SetFamilyInfo(:nFamilyID,:nFamilyClass,:nJoinFlag,:var_familyname,:var_familydesc,:var_familysign,:var_familynotice,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . SetFamilyInfo ( :nFamilyID , :nFami\
lyClass , :nJoinFlag , :var_familyname , :var_familydesc , :var_familysign , \
:var_familynotice , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )267;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&nFamilyClass;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nJoinFlag;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&var_familyname;
 sqlstm.sqhstl[3] = (unsigned long )103;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&var_familydesc;
 sqlstm.sqhstl[4] = (unsigned long )503;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&var_familysign;
 sqlstm.sqhstl[5] = (unsigned long )103;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (unsigned char  *)&var_familynotice;
 sqlstm.sqhstl[6] = (unsigned long )103;
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
	
	if( db_ret < 0 && db_ret != -1405)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
	if (retcode == 1403)
		return CError::DB_Error(DB_NOFAMILY);	
	else if( retcode == DB_FAMILYNAME_EXIST )
		return CError::DB_Error(DB_FAMILYNAME_EXIST,lpReq->szFamilyName);    
	else if (retcode == DB_FAMILYNAME_ERROR)
		return DB_FAMILYNAME_ERROR;
	
	//need added 修改faceid 和faceuri
	
	return DB_SUCCESS;
}

/*================================================================ 
*
* 函 数 名：Get_Family_Mem 
** 根据用户ID获取家族成员列表
* 参 数： 
* const uint32 c_iFamilyID [IN] : 家族ID 
* LPST_FAMILY_MEMBER pFamilyMem [[IN,OUT]] : 成员结构。ID和属性
* int &nMemNum	[IN, OUT] : 要获取的成员数目和返回的数目, 如果为0，则输出家族成员数
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 家族不存在: DB_NOFAMILY; 操作失败: DB_ERROR;
* 
================================================================*/
int CDatabaseFamily::Get_Family_Mem(const uint32 c_iFamilyID, LPST_FAMILY_MEMBER pFamilyMem, int &nMemNum)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		unsigned int	nFamilyID,nMemID,nProp;
		int				ret_count;
		int		 retcode;
		SQL_CURSOR cur_familymem;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nFamilyID = c_iFamilyID;
	ret_count = 0;
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	
	if (nMemNum == 0)
		nMemNum = MAX_VIPFAMILYMEM_NUM;
	/* EXEC SQL ALLOCATE :cur_familymem; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )318;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_familymem;
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
			Mini_Family_Pkg.GetFamilyMember(:nFamilyID,:cur_familymem,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . GetFamilyMember ( :nFamilyID , :cur\
_familymem , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )337;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&cur_familymem;
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
		/* EXEC SQL CLOSE :cur_familymem; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )368;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_familymem;
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
		/* EXEC SQL CLOSE :cur_familymem; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )387;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_familymem;
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


		nMemNum = 0;
		/* EXEC SQL FREE  :cur_familymem; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )406;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_familymem;
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
		if( ret_count >= nMemNum )	//指定获取的个数
		{			
			break;
		}
		/* EXEC SQL FETCH :cur_familymem INTO :nMemID,:nProp; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )425;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_familymem;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&nMemID;
  sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&nProp;
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
		if( db_ret == 1403)
		{
			nMemNum = ret_count;
			break;
		}
		if( db_ret < 0 && db_ret != -1405)
		{
			/* EXEC SQL CLOSE :cur_familymem; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )452;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_familymem;
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


			/* EXEC SQL FREE  :cur_familymem; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )471;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_familymem;
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


			char szError[255];
			sprintf(szError, "获取家族成员出错.家族ID：%d", nFamilyID);
			return CError::DB_Error(DB_ERROR, szError);
		}
		if (nMemID != 0)
		{
			pFamilyMem[ret_count].nUserID = nMemID;
			pFamilyMem[ret_count].nProp = nProp;
		}
		++ret_count;
	}
	/* EXEC SQL CLOSE :cur_familymem; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )490;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_familymem;
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


	/* EXEC SQL FREE  :cur_familymem; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )509;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_familymem;
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
* 函 数 名：Create_Family 
** 设置家族信息
* 参 数：
* const uint32 iCreaterID [IN] : 创建者ID
* LPXT_CREATE_CLUSTER_REQ lpReq [IN] : 家族的详细信息
* uint32 &iFamilyID	[IN,OUT] : 输出创建的家族ID
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 传入的参数错误：DB_PARAM；操作失败: DB_ERROR;
*					家族名称存在 DB_FAMILYNAME_EXIST; 域名存在 DB_FAMILYDOMAIN_EXIST;
*					创建数达到限制 DB_CREATE_FAMILY_FULL;
* 
================================================================*/
int CDatabaseFamily::Create_Family(const uint32 iCreaterID, LPXT_CREATE_CLUSTER_REQ lpReq, uint32 &iFamilyID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		int				nJoinFlag,nViewFlag,nFamilyClass;
		unsigned int	nCreaterID;
		unsigned int   nFamilyID;
		/* VARCHAR  db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		/* VARCHAR  var_familydomain[21], var_familyname[101],var_testname[31],var_familydesc[501], var_familyfaceurl[101]; */ 
struct { unsigned short len; unsigned char arr[21]; } var_familydomain;

struct { unsigned short len; unsigned char arr[101]; } var_familyname;

struct { unsigned short len; unsigned char arr[31]; } var_testname;

struct { unsigned short len; unsigned char arr[501]; } var_familydesc;

struct { unsigned short len; unsigned char arr[101]; } var_familyfaceurl;

//		VARCHAR  var_pic[61], var_audio[61], var_video[61], var_share[61];
		int				retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	if( !lpReq )
	{
		return DB_PARAM;
	}
	
	SET_INIT_NULL(var_familydomain);
	SET_INIT_NULL(var_familyname);
	SET_INIT_NULL(var_familydesc);
	SET_INIT_NULL(var_familyfaceurl);
	SET_INIT_NULL(var_testname);
	nCreaterID = iCreaterID;
	   
	nJoinFlag			 = lpReq->nJoinFlag;
	nViewFlag			 = lpReq->nViewFlag;
	nFamilyClass		 = lpReq->nFamilyClass;

	strncpy((char*)var_familyname.arr,lpReq->szFamilyName,MAX_FAMILYNAME_LEN);
	var_familyname.len = (uint16)strlen((char*)var_familyname.arr);
	strncpy((char*)var_familydesc.arr,lpReq->szFamilyDesc,MAX_FAMILYDESC_LEN);
	var_familydesc.len = (uint16)strlen((char*)var_familydesc.arr);
	strncpy((char*)var_familyfaceurl.arr,lpReq->szFamilyFaceUrl,MAX_FAMILYFACEURL_LEN);
	var_familyfaceurl.len = (uint16)strlen((char*)var_familyfaceurl.arr);			

	strncpy((char*)var_familydomain.arr,lpReq->szFamilyDomain,MAX_FAMILYDOMAIN_LEN);
	var_familydomain.len = (uint16)strlen((char*)var_familydomain.arr);	
			
// 	char nickname[33]={0},repname[33]={0};
// 	strncpy(nickname, lpReq->szFamilyName,MAX_FAMILYNAME_LEN);	
// 	char *p = nickname;
// 	char *pd = repname;
// 	while(*p)
// 	{		
// 		if( (*p >= 'a' && *p <= 'z') ||
// 			(*p >= 'A' && *p <= 'Z') || 
// 			(*p >= '0' && *p <= '9'))
// 		{	
// 			*pd++ = *p++;
// 		}
// 		else if ((BYTE)*p > 127)
// 		{
// 			if( (BYTE)*p == 0xa1 && (BYTE)*(p+1) == 0xa1)
// 			{
// 				p += 2;
// 			}
// 			else
// 				*pd++ = *p++;	
// 		}
// 		else
// 			p++;
// 	}
// 	*pd = 0;
// 	
// 	strncpy((char*)var_testname.arr, repname,MAX_FAMILYNAME_LEN);
// 	var_testname.len = (unsigned short)strlen(repname);	
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

		
start:

	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Family_Pkg.CreateFamily(:nFamilyID,:nCreaterID,:nFamilyClass,:nViewFlag,:nJoinFlag,:var_familyname,:var_familydesc,:var_familydomain,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . CreateFamily ( :nFamilyID , :nCreat\
erID , :nFamilyClass , :nViewFlag , :nJoinFlag , :var_familyname , :var_famil\
ydesc , :var_familydomain , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )528;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&nCreaterID;
 sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&nFamilyClass;
 sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&nViewFlag;
 sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)&nJoinFlag;
 sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)&var_familyname;
 sqlstm.sqhstl[5] = (unsigned long )103;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (unsigned char  *)&var_familydesc;
 sqlstm.sqhstl[6] = (unsigned long )503;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         short *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned long )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (unsigned char  *)&var_familydomain;
 sqlstm.sqhstl[7] = (unsigned long )23;
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
	
	if( db_ret < 0 && db_ret != -1405)
	{
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}

	if (retcode == DB_CREATE_FAMILY_FULL)
	{
		char szError[MAX_ERROR_BUF];
		sprintf(szError, "用户ID:%d", iCreaterID);
		return CError::DB_Error(DB_CREATE_FAMILY_FULL, szError);
	}
	else if (retcode == DB_FAMILYNAME_EXIST)
		return DB_FAMILYNAME_EXIST;
	else if (retcode == DB_FAMILYDOMAIN_EXIST)
		return DB_FAMILYDOMAIN_EXIST;
	
	//need added 添加faceid 和faceuri
	
	iFamilyID = nFamilyID;
	return DB_SUCCESS;
}

/*================================================================ 
*
* 函 数 名：Family_Add_Member 
** 添加家族普通成员
* 参 数：
* const uint32 iFamilyID [IN]	家族ID
* uint32 *iUserID	[IN]		要添加的成员ID列表
* int	&nRetCount	[IN]		要添加的个数,返回成功的个数
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 操作失败: DB_ERROR;
* 
================================================================*/
int	CDatabaseFamily::Family_Add_Member(const uint32 iFamilyID, uint32 *iUserID, int &nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int      nFamilyID;
		/* VARCHAR db_link_name[32],var_userid[4000]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

struct { unsigned short len; unsigned char arr[4000]; } var_userid;

		int retcode,retcount;
	/* EXEC SQL END DECLARE SECTION; */ 


	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nFamilyID	= iFamilyID;
	retcount = 0;
	
	int db_ret;

	SET_INIT_NULL(var_userid);
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	
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

start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Family_Pkg.AddFamilyMember(:nFamilyID,:var_userid,:retcount,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . AddFamilyMember ( :nFamilyID , :var\
_userid , :retcount , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )583;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&var_userid;
 sqlstm.sqhstl[1] = (unsigned long )4002;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&retcount;
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
	
	if( (db_ret < 0 && db_ret != -1405) || retcode == 1)
	{ 
		sql_error(sqlca, oraca);
		nRetCount = retcount;
		return DB_ERROR;

	}
	nRetCount = retcount;
	return DB_SUCCESS; 
}

/*================================================================ 
*
* 函 数 名：Family_Delete_Member 
** 删除家族成员
* 参 数：
* const uint32 iFamilyID [IN]	家族ID
* uint32 iUserID	[IN]		要删除的成员ID
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 操作失败: DB_ERROR;
* 
================================================================*/
int CDatabaseFamily::Family_Delete_Member(const uint32 iFamilyID, uint32 iUserID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int      nFamilyID, nUserID;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nFamilyID	= iFamilyID;
	nUserID		= iUserID;
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	
start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Family_Pkg.DelFamilyMember(:nFamilyID,:nUserID,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . DelFamilyMember ( :nFamilyID , :nUs\
erID , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )618;
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
	
	if( (db_ret < 0 && db_ret != -1405) || retcode == 1)
	{ 
		sql_error(sqlca, oraca);
		return DB_ERROR;
	} 

	return DB_SUCCESS;  
}

/*================================================================ 
*
* 函 数 名：Family_SetMem_Prop 
** 设置家族成员属性
* 参 数：
* const uint32 iFamilyID [IN]	家族ID
* uint32 iUserID	[IN]		要修改的成员ID
* uint8	iMemProp	[IN]		成员的属性	FAMILY_CREATER|FAMILY_MANAGER|FAMILY_NORMAL
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 操作失败: DB_ERROR;
* 
================================================================*/
int CDatabaseFamily::Family_SetMem_Prop(const uint32 iFamilyID, uint32 iUserID, uint8 iMemProp)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     nFamilyID, nUserID, nMemProp;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nFamilyID	= iFamilyID;
	nUserID		= iUserID;
	nMemProp	= iMemProp;
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	

start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Family_Pkg.FamilySetMemProp(:nFamilyID,:nUserID,:nMemProp,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . FamilySetMemProp ( :nFamilyID , :nU\
serID , :nMemProp , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )649;
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
 sqlstm.sqhstv[2] = (unsigned char  *)&nMemProp;
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
	
	if( (db_ret < 0 && db_ret != -1405) || retcode == 1)
	{ 
		sql_error(sqlca, oraca);
		return DB_ERROR;
	} 
	
	return DB_SUCCESS;
}

/*================================================================ 
*
* 函 数 名：Family_Find 
** 查找家族
* 参 数：
* XT_CLUSTER_SEARCH_REQ Req			[IN]			查找家族请求
* CLUSTER_SEARCH_RESULT *FamilyData	[IN/OUT]		查找结果
* int &nRetCount					[IN/OUT]		输入请求个数，返回结果个数
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 操作失败: DB_ERROR;
* 
================================================================*/
int CDatabaseFamily::Family_Find(XT_CLUSTER_SEARCH_REQ Req,XT_CLUSTER_SEARCH_ACK::tagFamilyFindRet *FamilyData, int &nRetCount)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int nFamilyID, nCreaterID,nMemberCnt;
		int			nJoinFlag, nFamilyClass, nStartPos, nReqCount;
		/* VARCHAR var_familyname[101], var_familydomain[21],var_familyfindname[101]; */ 
struct { unsigned short len; unsigned char arr[101]; } var_familyname;

struct { unsigned short len; unsigned char arr[21]; } var_familydomain;

struct { unsigned short len; unsigned char arr[101]; } var_familyfindname;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int retcode;
		SQL_CURSOR cur_familyclass;
	/* EXEC SQL END DECLARE SECTION; */ 


	SET_INIT_NULL(var_familyname);
	SET_INIT_NULL(var_familydomain);
	SET_INIT_NULL(var_familyfindname);
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	if (nRetCount == 0)
		return DB_PARAM;
	
	nStartPos		= Req.start_pos;
	nReqCount		=  nRetCount;
	nFamilyID		= Req.type_data.nFamilyID;
	nFamilyClass	= Req.type_data.nFamilyClass;
	strncpy((char*)var_familyname.arr, Req.type_data.szFamilyName,MAX_FAMILYNAME_LEN);
	var_familyname.len	= (short)strlen((char*)var_familyname.arr);
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

start:
	switch(Req.search_type)
	{
	case CLUSTER_SEARCH_ID:
		{	
			/* EXEC SQL AT :db_link_name EXECUTE
				begin
					Mini_Family_Pkg.SearchFamilyID(:nFamilyID,:var_familyname,:var_familydomain,:nJoinFlag,:nFamilyClass,:nCreaterID,:nMemberCnt,:retcode);
				end;
			END-EXEC; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "begin Mini_Family_Pkg . SearchFamilyID ( :nFamilyID , :va\
r_familyname , :var_familydomain , :nJoinFlag , :nFamilyClass , :nCreaterID ,\
 :nMemberCnt , :retcode ) ; end ;";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )684;
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
   sqlstm.sqhstv[1] = (unsigned char  *)&var_familyname;
   sqlstm.sqhstl[1] = (unsigned long )103;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&var_familydomain;
   sqlstm.sqhstl[2] = (unsigned long )23;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&nJoinFlag;
   sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)&nFamilyClass;
   sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (unsigned char  *)&nCreaterID;
   sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         short *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned long )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqhstv[6] = (unsigned char  *)&nMemberCnt;
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
			
			if( (db_ret < 0 && db_ret != -1405) || retcode == 1)
			{ 
				sql_error(sqlca, oraca);
				return DB_ERROR;
			} 
						
			if (retcode == 1403)
			{
				nRetCount = 0;
				return DB_SUCCESS;
			}
			
			SET_END_NULL(var_familyname);
			SET_END_NULL(var_familydomain);
			
			FamilyData->nFamilyID		= nFamilyID;
			FamilyData->nCreaterID		= nCreaterID;
			FamilyData->nJoinFlag		= nJoinFlag;
			FamilyData->nFamilyClass	= nFamilyClass;
			FamilyData->nMemCount		= nMemberCnt;
			strncpy(FamilyData->szFamilyName, (char*)var_familyname.arr,MAX_FAMILYNAME_LEN);
			strncpy(FamilyData->szFamilyDomain, (char*)var_familydomain.arr,MAX_FAMILYDOMAIN_LEN);
		}
		break;
	case CLUSTER_SEARCH_CLASS:
		{
		
			/* EXEC SQL ALLOCATE :cur_familyclass; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )735;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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
					Mini_Family_Pkg.SearchFamilyClass(:nFamilyClass,:nStartPos,:nReqCount,:cur_familyclass,:retcode);
				end;
			END-EXEC; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "begin Mini_Family_Pkg . SearchFamilyClass ( :nFamilyClass\
 , :nStartPos , :nReqCount , :cur_familyclass , :retcode ) ; end ;";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )754;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&nFamilyClass;
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
   sqlstm.sqhstv[3] = (unsigned char  *)&cur_familyclass;
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
				/* EXEC SQL CLOSE :cur_familyclass; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )793;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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
				/* EXEC SQL CLOSE :cur_familyclass; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )812;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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


				/* EXEC SQL FREE :cur_familyclass; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )831;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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


				nRetCount = 0;
				return DB_ERROR;
			}  						

			int iNum = 0;
			for(;;)
			{  
				/* EXEC SQL FETCH :cur_familyclass INTO :nFamilyID, :var_familyname, :var_familydomain, :nJoinFlag, :nFamilyClass, :nCreaterID; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )850;
    sqlstm.selerr = (unsigned short)1;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqfoff = (         int )0;
    sqlstm.sqfmod = (unsigned int )2;
    sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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
    sqlstm.sqhstv[2] = (unsigned char  *)&var_familyname;
    sqlstm.sqhstl[2] = (unsigned long )103;
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         short *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned long )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (unsigned char  *)&var_familydomain;
    sqlstm.sqhstl[3] = (unsigned long )23;
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         short *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned long )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (unsigned char  *)&nJoinFlag;
    sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         short *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned long )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqhstv[5] = (unsigned char  *)&nFamilyClass;
    sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[5] = (         int  )0;
    sqlstm.sqindv[5] = (         short *)0;
    sqlstm.sqinds[5] = (         int  )0;
    sqlstm.sqharm[5] = (unsigned long )0;
    sqlstm.sqadto[5] = (unsigned short )0;
    sqlstm.sqtdso[5] = (unsigned short )0;
    sqlstm.sqhstv[6] = (unsigned char  *)&nCreaterID;
    sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
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
				if (db_ret == 1403 )
				{
					nRetCount = iNum;
					break;					
				}
				if (iNum > nRetCount)
				{
					break;
				}
				if( db_ret < 0 && db_ret !=-1405)
				{
					sql_error(sqlca, oraca);					
					nRetCount = iNum;
					break;
				}
				
				SET_END_NULL(var_familyname);
				SET_END_NULL(var_familydomain);
				
				FamilyData[iNum].nFamilyID		= nFamilyID;
				FamilyData[iNum].nCreaterID		= nCreaterID;
				FamilyData[iNum].nJoinFlag		= nJoinFlag;
				FamilyData[iNum].nFamilyClass	= nFamilyClass;
				strncpy(FamilyData[iNum].szFamilyName, (char*)var_familyname.arr,MAX_FAMILYNAME_LEN);
				strncpy(FamilyData[iNum].szFamilyDomain, (char*)var_familydomain.arr,MAX_FAMILYDOMAIN_LEN);
				++iNum;
			}
			/* EXEC SQL CLOSE :cur_familyclass; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )893;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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


			/* EXEC SQL FREE :cur_familyclass; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )912;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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
		break;
	case CLUSTER_SEARCH_NAME:
		{
			/* EXEC SQL ALLOCATE :cur_familyclass; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )931;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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
					Mini_Family_Pkg.SearchFamilyName(:var_familyname,:nStartPos,:nReqCount,:cur_familyclass,:retcode);
				end;
			END-EXEC; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "begin Mini_Family_Pkg . SearchFamilyName ( :var_familynam\
e , :nStartPos , :nReqCount , :cur_familyclass , :retcode ) ; end ;";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )950;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&var_familyname;
   sqlstm.sqhstl[0] = (unsigned long )103;
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
   sqlstm.sqhstv[3] = (unsigned char  *)&cur_familyclass;
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
				/* EXEC SQL CLOSE :cur_familyclass; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )989;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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
				/* EXEC SQL CLOSE :cur_familyclass; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )1008;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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


				/* EXEC SQL FREE :cur_familyclass; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )1027;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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


				nRetCount = 0;
				return DB_ERROR;
			}  						

			int iNum = 0;
			for(;;)
			{  
				/* EXEC SQL FETCH :cur_familyclass INTO :nFamilyID, :var_familyname, :var_familydomain, :nJoinFlag, :nFamilyClass, :nCreaterID,:nMemberCnt; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 15;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )1046;
    sqlstm.selerr = (unsigned short)1;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqfoff = (         int )0;
    sqlstm.sqfmod = (unsigned int )2;
    sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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
    sqlstm.sqhstv[2] = (unsigned char  *)&var_familyname;
    sqlstm.sqhstl[2] = (unsigned long )103;
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         short *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned long )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (unsigned char  *)&var_familydomain;
    sqlstm.sqhstl[3] = (unsigned long )23;
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         short *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned long )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (unsigned char  *)&nJoinFlag;
    sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         short *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned long )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqhstv[5] = (unsigned char  *)&nFamilyClass;
    sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[5] = (         int  )0;
    sqlstm.sqindv[5] = (         short *)0;
    sqlstm.sqinds[5] = (         int  )0;
    sqlstm.sqharm[5] = (unsigned long )0;
    sqlstm.sqadto[5] = (unsigned short )0;
    sqlstm.sqtdso[5] = (unsigned short )0;
    sqlstm.sqhstv[6] = (unsigned char  *)&nCreaterID;
    sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[6] = (         int  )0;
    sqlstm.sqindv[6] = (         short *)0;
    sqlstm.sqinds[6] = (         int  )0;
    sqlstm.sqharm[6] = (unsigned long )0;
    sqlstm.sqadto[6] = (unsigned short )0;
    sqlstm.sqtdso[6] = (unsigned short )0;
    sqlstm.sqhstv[7] = (unsigned char  *)&nMemberCnt;
    sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
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
				if (iNum > nRetCount)
				{
					break;
				}
				if( db_ret < 0 && db_ret !=-1405)
				{
					sql_error(sqlca, oraca);					
					nRetCount = iNum;
					break;
				}
				
				SET_END_NULL(var_familyname);
				SET_END_NULL(var_familydomain);
				
				FamilyData[iNum].nFamilyID		= nFamilyID;
				FamilyData[iNum].nCreaterID		= nCreaterID;
				FamilyData[iNum].nJoinFlag		= nJoinFlag;
				FamilyData[iNum].nFamilyClass	= nFamilyClass;
				strncpy(FamilyData[iNum].szFamilyName, (char*)var_familyname.arr,MAX_FAMILYNAME_LEN);
				strncpy(FamilyData[iNum].szFamilyDomain, (char*)var_familydomain.arr,MAX_FAMILYDOMAIN_LEN);
				FamilyData[iNum].nMemCount	= nMemberCnt;
				++iNum;
			}
			/* EXEC SQL CLOSE :cur_familyclass; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1093;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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


			/* EXEC SQL FREE :cur_familyclass; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1112;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_familyclass;
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
		break;
	default:
		break;
	}
	return DB_SUCCESS;
}

/*================================================================ 
*
* 函 数 名：Delete_Family 
** 删除家族
* 参 数：
const uint32 iFamilyID	[IN] : 删除家族
* 返 回 值：int -	SUCCESS: DB_SUCCESS; 操作失败: DB_ERROR;
* 
================================================================*/
int CDatabaseFamily::Delete_Family(const uint32 iFamilyID)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int      nFamilyID;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nFamilyID		= iFamilyID;
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;	

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


start:	
	CLog::Log("Delete_Family","解散家族%d", iFamilyID);
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Family_Pkg.DeleteFamily(:nFamilyID,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . DeleteFamily ( :nFamilyID , :retcod\
e ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1131;
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
	
	if( (db_ret < 0 && db_ret != -1405) || retcode == 1)
	{ 
		sql_error(sqlca, oraca);
		return DB_ERROR;
	} 

	return DB_SUCCESS;
}


int	CDatabaseFamily::Get_FamilyMem_Porp(uint32 familyID, uint32 userID, uint8 &userProp)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int      nFamilyID, nUserID;
		int role;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 


	nFamilyID = familyID;
	nUserID=userID;
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	

start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Family_Pkg.GetMemLevel(:nFamilyID,:nUserID,:role,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . GetMemLevel ( :nFamilyID , :nUserID\
 , :role , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1158;
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
 sqlstm.sqhstv[2] = (unsigned char  *)&role;
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
	
	if( (db_ret < 0 && db_ret != -1405) || retcode == 1)
	{ 
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}

	if (retcode == 1403)
		return DB_NOUSER;

	userProp = role;
	return DB_SUCCESS;
}


int	CDatabaseFamily::Get_Family_BlackList(uint32 familyID, uint32 userID, int &retCode)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int      nFamilyID, nUserID;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 


	nFamilyID = familyID;
	nUserID=userID;
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	
start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Family_Pkg.GetBlackList(:nFamilyID,:nUserID,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . GetBlackList ( :nFamilyID , :nUserI\
D , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1193;
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
	
	if( (db_ret < 0 && db_ret != -1405) || retcode == 1)
	{ 
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}
	if(db_ret == 1403)
		retcode = 0; 

	retCode = retcode;
	return DB_SUCCESS;
}


int CDatabaseFamily::Get_All_Profile(uint32 iFamilyID, CMEM_INFO* profileName, uint32& nRetCnt)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int nFamilyID,nID;
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		/* VARCHAR sProfile[11]; */ 
struct { unsigned short len; unsigned char arr[11]; } sProfile;

		SQL_CURSOR cur_profile;
		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	SET_INIT_NULL(sProfile);
	nFamilyID = iFamilyID;
	int db_ret;
	int iReconnTimes;
	bool bReconn;
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
  
	/* EXEC SQL ALLOCATE :cur_profile; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1224;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&cur_profile;
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
			Mini_Family_Pkg.GetAllProfile(:nFamilyID,:cur_profile,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . GetAllProfile ( :nFamilyID , :cur_p\
rofile , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1243;
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
 sqlstm.sqhstv[1] = (unsigned char  *)&cur_profile;
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
			/* EXEC SQL CLOSE :cur_profile; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1274;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_profile;
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
			/* EXEC SQL CLOSE :cur_profile; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1293;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_profile;
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


			/* EXEC SQL FREE  :cur_profile; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1312;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_profile;
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
		for(int i=0; i<30; i++)
		{  
			/* EXEC SQL FETCH :cur_profile INTO :nID,:sProfile; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 15;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1331;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqfoff = (         int )0;
   sqlstm.sqfmod = (unsigned int )2;
   sqlstm.sqhstv[0] = (unsigned char  *)&cur_profile;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&nID;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&sProfile;
   sqlstm.sqhstl[2] = (unsigned long )13;
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
			profileName[iNum].nMemID = nID;
			SET_END_NULL(sProfile);
			strncpy(profileName[iNum].sProfileName,(char*)sProfile.arr,sProfile.len);										
			++iNum;
		}
		nRetCnt = iNum;
		/* EXEC SQL CLOSE :cur_profile; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1358;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_profile;
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


		/* EXEC SQL FREE  :cur_profile; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 15;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1377;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&cur_profile;
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

int	CDatabaseFamily::Get_Mem_Profile(uint32 familyID, uint32 userID, char* profileName)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int      nFamilyID, nUserID;
		/* VARCHAR pName[10]; */ 
struct { unsigned short len; unsigned char arr[10]; } pName;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 


	nFamilyID 		= familyID;
	nUserID			= userID;
	pName.arr[0] 	= 0;
	pName.len		= 0;
	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);

	int db_ret;
	int iReconnTimes;
	bool bReconn;
	
	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	

start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Family_Pkg.GetMemProfile(:nFamilyID,:nUserID,:pName,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . GetMemProfile ( :nFamilyID , :nUser\
ID , :pName , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1396;
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
 sqlstm.sqhstv[2] = (unsigned char  *)&pName;
 sqlstm.sqhstl[2] = (unsigned long )12;
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
	
	if( (db_ret < 0 && db_ret != -1405) || retcode == 1)
	{ 
		sql_error(sqlca, oraca);
		return DB_ERROR;
	}

	if (retcode == 1403)
		return DB_NOUSER;

	strncpy(profileName, (char*)pName.arr, pName.len);
	return DB_SUCCESS;
}

int CDatabaseFamily::Set_Mem_Profile(uint32 iFamilyID, uint32 iUserID, const char* profileName)
{
	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		unsigned int     nFamilyID, nUserID;
		/* VARCHAR pName[20]; */ 
struct { unsigned short len; unsigned char arr[20]; } pName;

		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;

		int retcode;
	/* EXEC SQL END DECLARE SECTION; */ 

	
	strncpy((char*)db_link_name.arr, m_Connect,32);
	db_link_name.len = (short)strlen((char*)db_link_name.arr);
	
	nFamilyID	= iFamilyID;
	nUserID		= iUserID;
	pName.len	= strlen(profileName);
	strncpy((char*)pName.arr, profileName, pName.len);
	
	int db_ret;
	int iReconnTimes;
	bool bReconn;

	/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 
	
start:
	/* EXEC SQL AT :db_link_name EXECUTE
		begin
			Mini_Family_Pkg.SetMemProfile(:nFamilyID,:nUserID,:pName,:retcode);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 15;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin Mini_Family_Pkg . SetMemProfile ( :nFamilyID , :nUser\
ID , :pName , :retcode ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1431;
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
 sqlstm.sqhstv[2] = (unsigned char  *)&pName;
 sqlstm.sqhstl[2] = (unsigned long )22;
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
	
	if( (db_ret < 0 && db_ret != -1405) || retcode == 1)
	{ 
		sql_error(sqlca, oraca);
		return DB_ERROR;
	} 
	
	return DB_SUCCESS;
}

void ForDisableBuildWarningFamily()
{
	unsigned int nTest =  sqlstm.sqlvsn;
	nTest++;
}
