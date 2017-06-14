#include "XSound.h"


//�������򴰿ڷ���֪ͨ 
//wParam : ֪ͨ����
//lParam : ����ֵ

#define WM_SOUND_NOTIFY  WM_USER+0x2E01

//WM_SOUND_NOTIFY wParam���Ͷ���

//��������ֹͣ
#define SN_STOP  0


class AUDIOPLAYERDLL_API ISoundPlayer
{
public:
	
	ISoundPlayer(){};

	virtual ~ISoundPlayer(){};

	enum RESULT{
		OK,
		NEED_MORE,
		FORMAT_NOT_SUPPORT,
		LOAD_FAILED,
		PLAY_FAILED,
	};

public:

	static ISoundPlayer *		CreatePlayer();
	
	virtual bool				Init(HWND hWnd) = 0;

	virtual RESULT				Load(XSound * pSound) = 0;

	virtual RESULT				Play() = 0;

	virtual RESULT				Seek(int nTime) = 0;

	virtual void				Pause() = 0;

	virtual void				Stop() = 0;

	virtual int					GetCurTime() = 0;

	virtual bool				IsPlaying() = 0;

	virtual void				SetVolume(long nVolume) = 0;

};

