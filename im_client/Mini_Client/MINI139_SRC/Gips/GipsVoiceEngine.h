// GipsVoiceEngine.h
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 
//  Created by: tzh
//  Date      : 20121224
//

#ifndef PUBLIC_GIPS_VOICE_ENGINE_H
#define PUBLIC_GIPS_VOICE_ENGINE_H

#include "GIPS_common_types.h"
#include "common_types.h"
#include "voe_base.h"

using namespace webrtc;

#ifdef WEBRTC_EXPORT
#define VOICEENGINE_DLLEXPORT _declspec(dllexport)
#else 
#define VOICEENGINE_DLLEXPORT _declspec(dllimport)
#endif


//////////////////////////////////////////////////////////////////////
// GipsVoiceEngineLib
//
// Public interface to the GIPS Voice Engine for PC platforms
//////////////////////////////////////////////////////////////////////

#ifndef NULL
#define NULL 0L
#endif

class VOICEENGINE_DLLEXPORT GipsVoiceEngineInterface
{
public:
	virtual int Init(int month = 0,int day = 0,int year = 0 ) = 0;
	virtual int SetNetworkStatus(int networktype) = 0;
	virtual int GetNetworkStatus() = 0;
	virtual int CreateChannel() = 0;
	virtual int DeleteChannel(int channel) = 0;
	virtual int GetVoiceEngine(webrtc::VoiceEngine **_voiceEngine) = 0;
	virtual int GetCodec(short listnr, GIPS_CodecInst *codec_inst) = 0;
	virtual int GetNofCodecs() = 0;
	virtual int SetSendCodec(int channel, GIPS_CodecInst *codec_inst) = 0;
	virtual int GetCurrentSendCodec(short channel, GIPS_CodecInst *inst) = 0;
	virtual int GetRecCodec(int channel, GIPS_CodecInst *recCodec) = 0;
	virtual int SetRecPort(int channel, int port,int RTCPport = -1,const char ipAddr[64] = NULL,const char multiCastAddr[64] = NULL) = 0;
	virtual int GetRecPort(int channel) = 0;
	virtual int GetSendPort(int channel) = 0;
	virtual int SetSendIP(int channel, int port,
		const char ipAddr[64],
		int sourcePort = -1,
		int RTCPport = -1) = 0;
	virtual int GetSendIP(int channel, char *ipadr, int bufsize) = 0;
	virtual int StartListen(int channel) = 0;
	virtual int StartPlayout(int channel) = 0;
	virtual int StartSend(int channel) = 0;
	virtual int StopListen(int channel) = 0;
	virtual int StopPlayout(int channel) = 0;
	virtual int StopSend(int channel) = 0;
	virtual int GetLastError() = 0;
	virtual int SetSpeakerVolume(unsigned int level) = 0;
	virtual int GetSpeakerVolume() = 0;
	virtual int SetMicVolume(unsigned int level) = 0;
	virtual int GetMicVolume() = 0;
	//////////////////////////////////////////////////////////////////////////
	//增益
	virtual int SetAGCStatus(int mode) = 0;
	virtual int GetAGCStatus() = 0;
	//////////////////////////////////////////////////////////////////////////
	//设置回声控制(EC)地位和模式。欧共体缓解了回声,用户可以听到他们自己的演讲重复回,因一声耦合扬声器和麦克风在远程端。
	virtual int SetAECStatus(int enlabla,EcModes mode = kEcAec)=0;
	virtual int GetAECStatus(int& enlabla,EcModes &mode)=0;

	//////////////////////////////////////////////////////////////////////////
	//集噪声抑制(NS)地位和模式。NS的减少噪音在麦克风信号
	virtual int SetNSStatus(int enlabla,NsModes mode = kNsModerateSuppression)=0;
	virtual int GetNSStatus(int& enlabla,NsModes &mode)=0;

	virtual int SetRxNsStatus(int channel,
								bool enable,
								NsModes mode = kNsUnchanged) = 0;
	virtual int GetRxNsStatus(int channel,
								bool& enabled,
								NsModes& mode) = 0;


	virtual int SetRxAgcStatus(int channel,
								bool enable,
								AgcModes mode = kAgcUnchanged) = 0;

	// Gets status and mode of the receiving-side AGC.
	virtual int GetRxAgcStatus(int channel,
								bool& enabled,
								AgcModes& mode) = 0;

	// Enables a high pass filter on the capture signal. This removes DC bias
	// and low-frequency noise. Recommended to be enabled.
	virtual int EnableHighPassFilter(bool enable) = 0;
	virtual bool IsHighPassFilterEnabled() = 0;

	//only using EcMode==kEcAec
	virtual int EnableDriftCompensation(bool enable) = 0;
	virtual bool DriftCompensationEnabled() = 0;

	virtual int GetVersion(char *version, int buflen) = 0;
	virtual int Terminate() = 0;
	virtual int SetDTMFPayloadType(int channel, int payloadType) = 0;
	virtual int SendDTMF(int channel, int eventnr, int inBand) = 0;
	virtual int PlayDTMFTone(int eventnr) = 0;	
	virtual unsigned short GetFromPort(int channel)=0;
	virtual int SetFilterPort(int channel,unsigned short filter) = 0;
	virtual int SetFilterIP(int channel,int rtpPort, int rtcpPort = 0, const char ipAddr[64] = 0) = 0;
	virtual unsigned short GetFilterPort(int channel) = 0;
	virtual int SetRecCodes(short channel, GIPS_CodecInst *codec_inst)=0;
	virtual int RTCPStat(int channel, unsigned short *fraction_lost, unsigned long *cum_lost, unsigned long *ext_max, unsigned long *jitter, int *RTT)=0;
	virtual int SetSoundDevices(unsigned int WaveInDevice, unsigned int WaveOutDevice, bool disableMicBoost = false)= 0;
	virtual int SetDTMFFeedbackStatus(int mode) = 0;
	virtual int GetDTMFFeedbackStatus() = 0;

	virtual int GetNoOfChannels() = 0;
	virtual int GetInputLevel() = 0;
	virtual int GetOutputLevel(int channel = -1) = 0;
	virtual int MuteMic(int channel,int Mute) = 0;
	virtual int PutOnHold(int channel,bool enable) = 0;

	virtual int CheckIfAudioIsAvailable(int &checkPlay, int &checkRec) = 0;

	// RTCP calls
	virtual int EnableRTCP(int channel, int enable) = 0;
	virtual int SetRTCPCNAME(int channel, char * str) = 0;
	virtual int getRemoteRTCPCNAME(int channel, char * str) = 0;
	virtual int SetLocalSSRC(int channel ,unsigned int SSRC)=0;

	virtual int SetPacketTimeout(int channel, bool enable, int time_sec) = 0;
	virtual int GetWaveInDevNum()= 0;
	virtual int GetWaveInDev(int nIndex, char * devname) = 0;
	virtual int GetWaveOutDevNum()= 0;
	virtual int GetWaveOutDev(int nIndex, char * devname) = 0;


	// Send extra packet over RTP / RTCP channel (no RTP headers added)
	virtual int sendExtraPacket_RTP(int channel, unsigned char* data, int nbytes) = 0;
	virtual int sendExtraPacket_RTCP(int channel, unsigned char* data, int nbytes) = 0;

	// Voice Activity
	virtual int GetVoiceActivityIndicator(int channel) = 0;

	// Use these function calls ONLY when a customer specific transport protocol is going to be used
	virtual int SetSendTransport(int channel, webrtc::Transport &transport) = 0;
	virtual int ReceivedRTPPacket(int channel, const void *data, int len) = 0;
	virtual int ReceivedRTCPPacket(int channel, const void *data, int len) = 0;


	//实现点对点通讯
	virtual int SetServerIP(const int channel,const int port,const char ipAddr[64]) = 0;
	//登录中继服务器，获取对方的外网端口、ip
	virtual int Login(int channel,const char *login_msg,int longin_length,unsigned short &rtpport,unsigned short &rtcpport,char *ip)=0;
	//点对点尝试
	virtual int StartP2PTest(int channel) = 0;

	virtual int RegisterExteralMakePacket(int channel ,void *pObj,pMakeIMPacket p) = 0;
	virtual int UnRegisterExteralMakePacket(int channel )=0;
	virtual ~GipsVoiceEngineInterface();


};

//////////////////////////////////////////////////////////////////////
// Factory method
//////////////////////////////////////////////////////////////////////

VOICEENGINE_DLLEXPORT GipsVoiceEngineInterface &GetGipsVoiceEngine();
VOICEENGINE_DLLEXPORT GipsVoiceEngineInterface *GetGipsVoiceEngineByNew();



#endif // PUBLIC_GIPS_VOICE_ENGINE_LIB_H
