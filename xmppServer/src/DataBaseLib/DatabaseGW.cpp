
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
           char  filnam[16];
};
static const struct sqlcxp sqlfpn =
{
    15,
    "./DatabaseGW.pc"
};


static unsigned int sqlctx = 1704611;


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
   unsigned char  *sqhstv[5];
   unsigned long  sqhstl[5];
            int   sqhsts[5];
            short *sqindv[5];
            int   sqinds[5];
   unsigned long  sqharm[5];
   unsigned long  *sqharc[5];
   unsigned short  sqadto[5];
   unsigned short  sqtdso[5];
} sqlstm = {12,5};

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
5,0,0,0,0,0,572,87,0,0,0,0,0,1,0,
20,0,0,0,0,0,570,90,0,0,1,1,0,1,0,3,109,0,0,
39,0,0,0,0,0,1563,130,0,0,4,4,0,0,128,1,9,0,0,1,9,0,0,1,9,0,0,1,10,0,0,1,9,0,0,
74,0,0,2,0,0,1566,197,0,0,0,0,0,0,128,1,9,0,0,
93,0,0,0,0,0,571,199,0,0,1,1,0,1,0,3,109,0,0,
};



#include "stdafx.h"
#include <sqlca.h>
#include <oraca.h>
#include <sqlcpr.h>
#include <oci.h>


#define SQLCA_NONE 
#define ORACA_NONE
#include <sqlcpr.h>



#include "StdPublic.h"
#include "Log.h"
#include "DatabaseGW.h"
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

//线程运行上下文
/* EXEC SQL BEGIN DECLARE SECTION; */ 

	sql_context m_ctx[ 200 ];
/* EXEC SQL END DECLARE SECTION; */ 


	
	
int  CDatabaseGW::m_nConnectCount = 0;
int	 CDatabaseGW::m_nContextid = 0;	
char CDatabaseGW::miniuid[32] = "";
char CDatabaseGW::minipwd[32] = "";
char CDatabaseGW::minisid[32] = "";
char CDatabaseGW::webuid[32] = "";
char CDatabaseGW::webpwd[32] = "";
char CDatabaseGW::websid[32] = "";
char CDatabaseGW::familyuid[32] = "";
char CDatabaseGW::familypwd[32] = "";
char CDatabaseGW::familysid[32] = "";

CSyncCritical	CDatabaseGW::m_CriticalLock;

CDatabaseGW::CDatabaseGW(void)
{
	m_Connect[0] = '\0';
	m_nMyContextid = -1;
}

CDatabaseGW::~CDatabaseGW(void)
{
	DisConnect();
}
  
bool CDatabaseGW::connect(char* pUserName,char* pPassword,char* pServerName)
{
	DEF_ORASET;
	assert(pUserName);
	assert(pPassword);
	assert(pServerName);
	
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		
		/* VARCHAR username[41] ; */ 
struct { unsigned short len; unsigned char arr[41]; } username;

		/* VARCHAR password[32] ; */ 
struct { unsigned short len; unsigned char arr[32]; } password;

		/* VARCHAR servername[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } servername;

		/* VARCHAR db_link_name[32] ; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;


	/* EXEC SQL END DECLARE SECTION; */ 

	

	if (m_Connect[0] == '\0' && m_nMyContextid == -1) //如果超时断开重连继续使用原连接名和上下文
	{
		//自动分配一个连接标识符，不同的类对象有不同的连接标识符，以便支持多线程
		m_CriticalLock.Lock();
		
		++m_nConnectCount;
		sprintf(m_Connect,"connect%d",m_nConnectCount);
		
		//为多线程连接做准备运行上下文
		/* EXEC SQL ENABLE THREADS; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 0;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		m_nMyContextid = m_nContextid++;
		
		/* EXEC SQL CONTEXT ALLOCATE :m_ctx[m_nMyContextid]; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 1;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )20;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&m_ctx[m_nMyContextid];
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
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
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

				
		m_CriticalLock.Unlock(); 
		
		strncpy((char*)db_link_name.arr , m_Connect,32);
		db_link_name.len = (short)strlen(m_Connect);
	}
// 	else
// 	{
// 		try
// 		{
// 			strncpy((char*)db_link_name.arr , m_Connect,32);
// 			db_link_name.len = (short)strlen(m_Connect);
// 			EXEC SQL WHENEVER SQLERROR CONTINUE;
// 			EXEC SQL AT :db_link_name COMMIT RELEASE;
// 		}
// 		catch(...)
// 		{
// 			CLog::Log("CDatabaseGW::connect",CLog::TYPE_IMPORTANT,"断开%s连接后出现异常!",m_Connect);
// 		}
// 	}
	
	
// 	try
// 	{	
	
		//初始化数据
		strncpy((char*)db_link_name.arr , m_Connect,32);
		db_link_name.len = (short)strlen(m_Connect);

		strncpy((char*)username.arr , pUserName,MAX_USERNAME_LEN+1);
		username.len = (short)strlen(pUserName);
		strncpy((char*)password.arr , pPassword,32);
		password.len = (short)strlen(pPassword) ;
		strncpy((char*)servername.arr , pServerName,32);
		servername.len = (short)strlen(pServerName);


		//执行连接
		/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 


		/* EXEC SQL CONNECT	:username 
		IDENTIFIED BY		:password
		AT					:db_link_name
		USING				:servername; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )10;
  sqlstm.offset = (unsigned int  )39;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&username;
  sqlstm.sqhstl[0] = (unsigned long )43;
  sqlstm.sqhsts[0] = (         int  )43;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&password;
  sqlstm.sqhstl[1] = (unsigned long )34;
  sqlstm.sqhsts[1] = (         int  )34;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&servername;
  sqlstm.sqhstl[2] = (unsigned long )34;
  sqlstm.sqhsts[2] = (         int  )34;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)&db_link_name;
  sqlstm.sqhstl[4] = (unsigned long )34;
  sqlstm.sqhsts[4] = (         int  )34;
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
  sqlstm.sqlcmax = (unsigned int )100;
  sqlstm.sqlcmin = (unsigned int )2;
  sqlstm.sqlcincr = (unsigned int )1;
  sqlstm.sqlctimeout = (unsigned int )0;
  sqlstm.sqlcnowait = (unsigned int )0;
  sqlcxt(&m_ctx[m_nMyContextid], &sqlctx, &sqlstm, &sqlfpn);
}


		
		int dbret = sqlca.sqlcode;
		if (dbret != 0)
		{
			sql_error(sqlca, oraca);
			return false;
		}
// 	}
// 	catch(...)
// 	{
// 		CLog::Log("CDatabaseGW::connect",CLog::TYPE_IMPORTANT,"断开%s连接后重新连接出现异常!",m_Connect);
// 	}

	CLog::Log("CDatabaseGW::connect",CLog::TYPE_IMPORTANT,"database %s connect OK",pServerName);
	return true;
	
}

bool CDatabaseGW::ConnectMini()
{
	oraca.oradbgf = 1; 
	oraca.oracchf = 1; 
	oraca.orastxtf = 3; 
	return connect(miniuid,minipwd,minisid);
} 


bool CDatabaseGW::ConnectWeb()
{
	oraca.oradbgf = 1; 
	oraca.oracchf = 1; 
	oraca.orastxtf = 3; 
	return connect(webuid,webpwd,websid);
}

bool CDatabaseGW::ConnectFamily()
{
	oraca.oradbgf = 1; 
	oraca.oracchf = 1; 
	oraca.orastxtf = 3; 
	return connect(familyuid,familypwd,familysid);
}

void CDatabaseGW::DisConnect()
{

	DEF_ORASET;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

	
		/* VARCHAR db_link_name[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } db_link_name;


	/* EXEC SQL END DECLARE SECTION; */ 

	
	
	if( m_Connect[0] != '\0' )
	{
		/* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

		
		strncpy((char*)db_link_name.arr , m_Connect,32);
		db_link_name.len = (short)strlen(m_Connect);
		
		/* EXEC SQL CONTEXT USE :m_ctx[m_nMyContextid]; */ 

			
		/* EXEC SQL AT :db_link_name COMMIT RELEASE; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&m_ctx[m_nMyContextid], &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )74;
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


		
		/* EXEC SQL CONTEXT FREE :m_ctx[m_nMyContextid]; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )93;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&m_ctx[m_nMyContextid];
  sqlstm.sqhstl[0] = (unsigned long )sizeof(void *);
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
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		m_nMyContextid = -1;
	}
}

//SQL错误处理
void CDatabaseGW::sql_error(struct sqlca sqlca,struct oraca oraca)
{
	char stm[250];
	size_t sqlfc,stmlen=250;
	 
	//取得错误SQL语句
	sqlgls(stm,&stmlen,&sqlfc);

	CLog::Log("CDatabaseGW",CLog::TYPE_ERROR,
			"\n允许打开的最大游标个数: %d \t打开的最大游标个数: %d \t当前打开游标数: %d \t重新分配的游标高速缓存数: %d \n解析SQL语句的条数: %d \t执行的SQL语句数: %d \n最近执行的SQL语句: %.*s\n文件:%.*s\t行号:%d\t错误码:%d\t原因:%.*s\n",
			oraca.orahoc, oraca.oramoc,oraca.oracoc,oraca.oranor,oraca.oranpr,oraca.oranex,
			oraca.orastxt.orastxtl,oraca.orastxt.orastxtc,
			oraca.orasfnm.orasfnml,oraca.orasfnm.orasfnmc,
			oraca.oraslnr,sqlca.sqlcode, sqlca.sqlerrm.sqlerrml,sqlca.sqlerrm.sqlerrmc);	

			
}

bool CDatabaseGW::conn_error(int db_ret)
{
	if( -1012 == db_ret ||	//"not logged on"
			-3114 == db_ret ||	//"not connected to ORACLE"
			-3113 == db_ret ||	//"end-of-file on communication channel"
			-2396 == db_ret || 	//"exceeded maximum idle time, please connect again"
			-12547 == db_ret || //"TNS:lost contact"
			-12152 == db_ret || //"TNS:unable to send break message"
			-1092 == db_ret ||	//"ORACLE instance terminated. Disconnection forced"
			-12170 == db_ret ||	//"TNS:Connect timeout occurred"
			-24324 == db_ret ||	//"service handle not initialized"
			-24338 == db_ret ||	//"statement handle not executed"
			-28 == db_ret				//"your session has been killed"
	)
		return true;
	else
		return false;
}

void ForDisableBuildWarningGW()
{
	unsigned int nTest =  sqlstm.sqlvsn;
	nTest++;
}


