#pragma once
#include "DatabaseGW.h"

class CDatabaseFamily :
	public CDatabaseGW
{
public:
	CDatabaseFamily(void);
	~CDatabaseFamily(void);
	void SetConnectDB(char* uid, char* pwd, char* sid);
	bool ConnectDB();	//连接Family数据库

	
	//家族业务
	int Get_Family_List(const uint32 c_iUserID, uint32 *FamilyID, int &FamilyCount);
	int Get_Family_Info(const uint32 c_iFamilyID, LPXT_GET_CINFO_ACK lpAck);
	int Get_Family_Mem(const uint32 c_iFamilyID, LPST_FAMILY_MEMBER pFamilyMem, int &nMemNum);
	int	Set_Family_Info(LPXT_SET_CINFO_REQ lpReq);
	int Create_Family(const uint32 iCreaterID, LPXT_CREATE_CLUSTER_REQ lpReq, uint32 &iFamilyID);
	int Delete_Family(const uint32 iFamilyID);
	int	Family_Add_Member(const uint32 iFamilyID,  uint32 *iUserID, int &nRetCount);
	int Family_Delete_Member(const uint32 iFamilyID, uint32 iUserID);
	int Family_SetMem_Prop(const uint32 iFamilyID, uint32 iUserID, uint8 iMemProp);
	int Family_Find(XT_CLUSTER_SEARCH_REQ Req,XT_CLUSTER_SEARCH_ACK::tagFamilyFindRet *FamilyData, int &nRetCount);
	int	Get_FamilyMem_Porp(uint32 familyID, uint32 userID,  uint8 &userProp);
	int	Get_Family_BlackList(uint32 familyID, uint32 userID, int &retCode);
	int GetFamilyPicUri(uint32 familyid,char *picuri);
	int	Get_Mem_Profile(uint32 familyID, uint32 userID, char* profileName);
	int Set_Mem_Profile(uint32 iFamilyID, uint32 iUserID, const char* profileName);
	int Get_All_Profile(uint32 iFamilyID, CMEM_INFO* profileName,uint32& nRetCnt);
};

