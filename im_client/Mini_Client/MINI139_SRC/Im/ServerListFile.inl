
//------------------------------------------------------
// ��ȡҵ��������б�
//------------------------------------------------------

inline IMSERVER_LISTC	CServerListFile::GetBusiSrvList() const 
{ return m_arrBusiSrv; };

inline IMSERVER_LIST	CServerListFile::GetBusiSrvList()
{ return m_arrBusiSrv; };

//------------------------------------------------------
// ��ȡת���������б�
//------------------------------------------------------

inline IMSERVER_LISTC	CServerListFile::GetTranSrvList() const
{ return m_arrTranSrv; };

inline IMSERVER_LIST	CServerListFile::GetTranSrvList()
{ return m_arrTranSrv; };

//------------------------------------------------------
// ����ҵ��������б�
//------------------------------------------------------

inline void CServerListFile::SetBusiSrvList(IMSERVER_LISTC & arrBusiSrv)
{ m_arrBusiSrv = arrBusiSrv; };

//------------------------------------------------------
// ����ת���������б�
//------------------------------------------------------

inline void	CServerListFile::SetTranSrvList(IMSERVER_LISTC & arrTranSrv)
{ m_arrTranSrv = arrTranSrv; }
