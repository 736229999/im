
//------------------------------------------------------
// 获取业务服务器列表
//------------------------------------------------------

inline IMSERVER_LISTC	CServerListFile::GetBusiSrvList() const 
{ return m_arrBusiSrv; };

inline IMSERVER_LIST	CServerListFile::GetBusiSrvList()
{ return m_arrBusiSrv; };

//------------------------------------------------------
// 获取转发服务器列表
//------------------------------------------------------

inline IMSERVER_LISTC	CServerListFile::GetTranSrvList() const
{ return m_arrTranSrv; };

inline IMSERVER_LIST	CServerListFile::GetTranSrvList()
{ return m_arrTranSrv; };

//------------------------------------------------------
// 设置业务服务器列表
//------------------------------------------------------

inline void CServerListFile::SetBusiSrvList(IMSERVER_LISTC & arrBusiSrv)
{ m_arrBusiSrv = arrBusiSrv; };

//------------------------------------------------------
// 设置转发服务器列表
//------------------------------------------------------

inline void	CServerListFile::SetTranSrvList(IMSERVER_LISTC & arrTranSrv)
{ m_arrTranSrv = arrTranSrv; }
