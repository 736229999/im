#ifndef __NETTRANS_CONFIG__
#define __NETTRANS_CONFIG__
#pragma once

#ifdef MININETTRANS_EXPORTS
#define MININETTRANS_API __declspec(dllexport)
#else
#define MININETTRANS_API __declspec(dllimport)
#endif


typedef  int  NETSOCKET;    //�߼�Socket���



#endif/*__NETTRANS_CONFIG__*/