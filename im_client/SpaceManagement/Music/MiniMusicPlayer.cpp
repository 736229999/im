// MiniMusicPlayer.cpp : CMiniMusicPlayer ��ʵ��

#include "stdafx.h"
#include "MiniMusicPlayer.h"
#include ".\minimusicplayer.h"
#include "minimusicplayerdlg.h"
#include "WebInterfaceWrapper.h"
// CMiniMusicPlayer

//
//STDMETHODIMP CMiniMusicPlayer::Show(void)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	// TODO: �ڴ����ʵ�ִ���
//	CMiniMusicPlayerDlg dlg;
//	dlg.DoModal();
//
//
//	return S_OK;
//}


//STDMETHODIMP CMiniMusicPlayer::AddFriend(long nUserId, BSTR strUserName, BSTR strNickName, long nSex)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	// TODO: �ڴ����ʵ�ִ���
//	tagFriendInfo f;
//	f.userId = nUserId;
//	f.userName = strUserName;
//	f.userNick = strNickName;
//	f.sex = nSex;
//
//	g_FriendList.push_back(f);
//	
//	
//	return S_OK;
//}

STDMETHODIMP CMiniMusicPlayer::Show(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
		
	CMiniMusicPlayerDlg dlg;
	dlg.DoModal();
	

	return S_OK;
}

STDMETHODIMP CMiniMusicPlayer::AddFriend(LONG nUserId, BSTR strUserName, BSTR strNickName, LONG nSex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ����ʵ�ִ���

	return S_OK;
}
