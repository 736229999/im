
#ifndef PUBLIC_GIPS_VIDEO_ENGINEINTERFACE_H
#define PUBLIC_GIPS_VIDEO_ENGINEINTERFACE_H
#ifdef WEBRTC_EXPORT
#define VIDEOENGINE_DLLEXPORT _declspec(dllexport)
#else 
#define VIDEOENGINE_DLLEXPORT _declspec(dllimport)
#endif


//#include <xstring>

struct VIDEOENGINE_DLLEXPORT _CaptureCapability {
	unsigned int width;
	unsigned int height;
	unsigned int maxFPS;
	int rawType;
	int codecType;
	unsigned int expectedCaptureDelay;
	bool interlaced;
	_CaptureCapability() {
		width = 0;
		height = 0;
		maxFPS = 0;
		rawType = 0;
		codecType = 4;
		expectedCaptureDelay = 0;
		interlaced = false;
	}
};
#include "voe_base.h"
#include "common_types.h"
#include <string.h>

class VIDEOENGINE_DLLEXPORT GipsVideoEngineInterface
{
public:
	virtual int LastError() =0;
	virtual int InitVideo() = 0;
	virtual int CreateChannal(int &channel) =0;

	virtual int ConnctDevice(int channel,int & capture_id) =0;
	virtual int TestCapDevice(int & capture_id,void *localVideoWindow,int IndexCap) = 0 ;
	virtual int CloseTestCapDevice(int capture_id) = 0;
	virtual int RegisterVideoRenderModule(int channel,int capture_id,void * localVideoWindow, void * remoteVideoWindow)=0;
	virtual int NumberOfCodecs()=0;
	virtual int GetCodec(int index,webrtc::VideoCodec &video_codec)=0;
	virtual int SetSendCodec(int channel ,webrtc::VideoCodec video_codec)=0;
	virtual int GetSendCodec(int channel,webrtc::VideoCodec& video_codec)=0;
	virtual int SetReceiveCodec(int channel ,webrtc::VideoCodec video_codec)=0;
	virtual int GetReceiveCodec(int channel ,webrtc::VideoCodec& video_codec)=0;
	virtual int StartRecv(int channel)=0;
	virtual int StartSend(int channel)=0;
	virtual int StopRecv(int channel)=0;
	virtual int StopSend(int channel)=0;
	virtual int GetVersion(char *version)=0;
	virtual int DeleteChannel(int channel)=0;
	virtual int SetLocalSSRC(int channel ,unsigned int SSRC)=0;
	virtual int SetSendDestination(const int video_channel,
		const char* ip_address,
		const unsigned short rtp_port,
		const unsigned short rtcp_port = 0,
		const unsigned short source_rtp_port = 0,
		const unsigned short source_rtcp_port = 0)=0;
	virtual int SetLocalRecv(const int video_channel,
		const unsigned short rtp_port,
		const unsigned short rtcp_port = 0)=0;
	///获取带宽
	virtual int GetEstimatedBandwidth(const int video_channel,
		unsigned int* estimated_recv_bandwidth,unsigned int* estimated_send_bandwidth)=0;

	virtual int ConnectAudioChannal(int channel,int &voice_channel,webrtc::VoiceEngine * voice_engin)=0;
	virtual int NumberofDevices() = 0;
	virtual int GetCaptureDevice(unsigned int list_number,char* device_nameUTF8,
								const unsigned int device_nameUTF8Length,char* unique_idUTF8,
								const unsigned int unique_idUTF8Length) = 0;
	virtual int SetCaptureDevice(int nIndex = 0)=0;

	// Returns the number of sets of capture capabilities the capture device
	// supports.
	virtual int NumberOfCapabilities(
		const char* unique_id_utf8,
		const unsigned int unique_id_utf8_length) = 0;

	// Gets a set of capture capabilities for a specified capture device.
	virtual int GetCaptureCapability(const char* unique_id_utf8,
		const unsigned int unique_id_utf8_length,
		const unsigned int capability_number,
		_CaptureCapability& capability) = 0;
	virtual int SafeClose(int channel) = 0 ;
	virtual int DisConnectAudio(int channel) = 0 ;
	//////////////////////////////////////////////////////////////////////////
	//图像质量处理
	//取消闪烁
	virtual int EnableDeflickering(const int capture_id, const bool enable) = 0;
	// 减少相机噪声
	virtual int EnableDenoising(const int capture_id, const bool enable) = 0;

	// 提高颜色.
	virtual int EnableColorEnhancement(const int video_channel,const bool enable) = 0;

	//实现点对点通讯
	virtual int SetServerIP(const int channel,const int port,const char ipAddr[64]) = 0;
	//登录中继服务器，获取对方的外网端口、ip
	virtual int Login(int channel,const char *login_msg,int longin_length,unsigned short &rtpport,unsigned short &rtcpport,char *ip)=0;
	//点对点尝试
	virtual int StartP2PTest(int channel) = 0;

	virtual int RegisterExteralMakePacket(int channel ,void *pObj,pMakeIMPacket p) = 0;
	virtual int UnRegisterExteralMakePacket(int channel )=0;
private:
	virtual int RenderInWindow(void* video_render_interface,int frame_provider_id,
		void* os_window,float z_index)=0 ;
	virtual int ConfigureRtpRtcp(void* rtcp_interface,int video_channel)=0;
	virtual int FindCaptureDeviceOnSystem(void* capture,char* device_name,
		const unsigned int kDeviceNameLength,int* device_id,void** device_video,int indexDevice = 0)=0;

public:
	//virtual void SetErrorMsg(std::string &str){strError = str;}
	//virtual std::string GetErrorMsg()const {return strError;}

	virtual ~GipsVideoEngineInterface(){;}
private:
	//std::string strError;
};


VIDEOENGINE_DLLEXPORT GipsVideoEngineInterface &GetGipsVideoEngine();
VIDEOENGINE_DLLEXPORT GipsVideoEngineInterface *GetGipsVideoEngineByNew();
#endif