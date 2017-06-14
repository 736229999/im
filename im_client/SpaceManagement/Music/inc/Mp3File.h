
#if !defined(AFX_MP3FILE1_H__CCE801D8_6BDE_4114_A455_5EC30BC59B40__INCLUDED_)
#define AFX_MP3FILE1_H__CCE801D8_6BDE_4114_A455_5EC30BC59B40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
#include <assert.h>
using namespace std;

#define ID3V2_VER		3
#define LANGUAGE		"chi"

typedef unsigned char byte;

class CMp3File;

typedef struct tagID3V1
{
	char Header[3];		/*标签头必须是"TAG"否则认为没有标签*/
	char Title[30];		/*标题*/
	char Artist[30];	/*作者*/
	char Album[30];		/*专集*/
	char Year[4];		/*出品年代*/
	char Comment[28];	/*备注*/
	char Reserve;		/*保留*/
	char Track;			/*音轨*/
	char Genre;			/*类型*/
}ID3V1,*PID3V1;
/*
--------------
类型
--------------
0="Blues";
1="ClassicRock";
2="Country";
3="Dance";
4="Disco";
5="Funk";
6="Grunge";
7="Hip-Hop";
8="Jazz";
9="Metal";
10="NewAge";
11="Oldies";
12="Other";
13="Pop";
14="R&B";
15="Rap";
16="Reggae";
17="Rock";
18="Techno";
19="Industrial";
20="Alternative";
21="Ska";
22="DeathMetal";
23="Pranks";
24="Soundtrack";
25="Euro-Techno";
26="Ambient";
27="Trip-Hop";
28="Vocal";
29="Jazz+Funk";
30="Fusion";
31="Trance";
32="Classical";
33="Instrumental";
34="Acid";
35="House";
36="Game";
37="SoundClip";
38="Gospel";
39="Noise";
40="AlternRock";
41="Bass";
42="Soul";
43="Punk";
44="Space";
45="Meditative";
46="InstrumentalPop";
47="InstrumentalRock";
48="Ethnic";
49="Gothic";
50="Darkwave";
51="Techno-Industrial";
52="Electronic";
53="Pop-Folk";
54="Eurodance";
55="Dream";
56="SouthernRock";
57="Comedy";
58="Cult";
59="Gangsta";
60="Top40";
61="ChristianRap";
62="Pop/Funk";
63="Jungle";
64="NativeAmerican";
65="Cabaret";
66="NewWave";
67="Psychadelic";
68="Rave";
69="Showtunes";
70="Trailer";
71="Lo-Fi";
72="Tribal";
73="AcidPunk";
74="AcidJazz";
75="Polka";
76="Retro";
77="Musical";
78="Rock&Roll";
79="HardRock";
-- Extended genres --
80="Folk";
81="Folk-Rock";
82="NationalFolk";
83="Swing";
84="FastFusion";
85="Bebob";
86="Latin";
87="Revival";
88="Celtic";
89="Bluegrass";
90="Avantgarde";
91="GothicRock";
92="ProgessiveRock";
93="PsychedelicRock";
94="SymphonicRock";
95="SlowRock";
96="BigBand";
97="Chorus";
98="EasyListening";
99="Acoustic";
100="Humour";
101="Speech";
102="Chanson";
103="Opera";
104="ChamberMusic";
105="Sonata";
106="Symphony";
107="BootyBass";
108="Primus";
109="PornGroove";
110="Satire";
111="SlowJam";
112="Club";
113="Tango";
114="Samba";
115="Folklore";
116="Ballad";
117="PowerBallad";
118="RhythmicSoul";
119="Freestyle";
120="Duet";
121="PunkRock";
122="DrumSolo";
123="Acapella";
124="Euro-House";
125="DanceHall";
126="Goa";
127="Drum&Bass";
128="Club-House";
129="Hardcore";
130="Terror";
131="Indie";
132="BritPop";
133="Negerpunk";
134="PolskPunk";
135="Beat";
136="ChristianGangstaRap";
137="HeavyMetal";
138="BlackMetal";
139="Crossover";
140="ContemporaryChristian";
141="ChristianRock";
142="Merengue";
143="Salsa";
144="TrashMetal";
145="Anime";
146="JPop";
147="Synthpop";
*/



class CID3V1:public ID3V1
{
public:
	// 构造初始化
	CID3V1();
	// 注意不能为virtual
	~CID3V1();

public:

	void operator = (const ID3V1 & v1);

public:
	// 设置标题
	void			SetTitle(const string& sTitle);
	// 获得标题
	string			GetTitle() const;
	// 设置作者
	void			SetArtist(const string& sArtist);
	// 获得作者
	string			GetArtist() const;
	// 设置专辑
	void			SetAlbum(const string& sAlbum);
	// 获得专辑
	string			GetAlbum() const;
	// 设置出品年代
	void			SetYear(const string& sYear);
	// 获得出品年代
	string			GetYear() const;
	// 设置备注
	void			SetComment(const string& sComment);
	// 获得备注
	string			GetComment() const;
	// 设置音轨
	void			SetTrack(char track);
	// 获得音轨
	char			GetTrack() const;
	// 设置类型
	void			SetGenre(char genre);
	// 获得类型
	char			GetGenre() const;
	// 保存到文件
	void			Save(ofstream& ofs);

};



struct ID3V2_HEADER
{
	char Header[3];		/*必须为"ID3"否则认为标签不存在*/
	char Ver;			/*版本号ID3V2.3 就记录3*/
	char Revision;		/*副版本号此版本记录为0*/
	char Flag;			/*存放标志的字节，这个版本只定义了三位，稍后详细解说*/
	byte Size[4];		/*标签大小，包括标签头的10 个字节和所有的标签帧的大小*/
};


struct ID3V2_FRAME_HEADER
{
	char FrameID[4];	/*用四个字符标识一个帧，说明其内容，稍后有常用的标识对照表*/
	byte Size[4];		/*帧内容的大小，不包括帧头，不得小于1*/
	char Flags[2];		/*存放标志，只定义了6 位，稍后详细解说*/
};


/*
AENC Audio encryption
APIC Attached picture
COMM Comments
COMR Commercial frame
ENCR Encryption method registration
EQUA Equalization
ETCO Event timing codes
GEOB General encapsulated object
GRID Group identification registration
IPLS Involved people list
LINK Linked information
MCDI Music CD identifier
MLLT MPEG location lookup table
OWNE Ownership frame
PRIV Private frame
PCNT Play counter
POPM Popularimeter
POSS Position synchronisation frame
RBUF Recommended buffer size
RVAD Relative volume adjustment
RVRB Reverb
SYLT Synchronized lyric/text
SYTC Synchronized tempo codes
TALB Album/Movie/Show title
TBPM BPM (beats per minute)
TCOM Composer
TCON Content type
TCOP Copyright message
TDAT Date
TDLY Playlist delay
TENC Encoded by
TEXT Lyricist/Text writer
TFLT File type
TIME Time
TIT1 Content group description
TIT2 Title/songname/content description
TIT3 Subtitle/Description refinement
TKEY Initial key
TLAN Language(s)
TLEN Length
TMED Media type
TOAL Original album/movie/show title
TOFN Original filename
TOLY Original lyricist(s)/text writer(s)
TOPE Original artist(s)/performer(s)
TORY Original release year
TOWN File owner/licensee
TPE1 Lead performer(s)/Soloist(s)
TPE2 Band/orchestra/accompaniment
TPE3 Conductor/performer refinement
TPE4 Interpreted, remixed, or otherwise modified by
TPOS Part of a set
TPUB Publisher
TRCK Track number/Position in set
TRDA Recording dates
TRSN Internet radio station name
TRSO Internet radio station owner
TSIZ Size
TSRC ISRC (international standard recording code)
TSSE Software/Hardware and settings used for encoding
TYER Year
TXXX User defined text information frame
UFID Unique file identifier
USER Terms of use
USLT Unsychronized lyric/text transcription
WCOM Commercial information
WCOP Copyright/Legal information
WOAF Official audio file webpage
WOAR Official artist/performer webpage
WOAS Official audio source webpage
WORS Official internet radio station homepage
WPAY Payment
WPUB Publishers official webpage
WXXX User defined URL link frame
*/

class CID3V2FrameItem:public ID3V2_FRAME_HEADER
{
public:

	CID3V2FrameItem();
	
	~CID3V2FrameItem();

	CID3V2FrameItem(const CID3V2FrameItem&other);

	void operator =(const CID3V2FrameItem&o);

public:
	// 不包括头的大小
	int		GetSize() const;
	// 设置大小不包括头的大小
	void	SetSize(int nSize);
	// 返回加载的数据的总长度（包括头），如果加载失败，返回-1
	int		Load(const char*  pFrameItem);

	char*	GetData() { return m_pData; };
	
	void	Save(ofstream &ofs);

	void	Save(char*&pBuf);
	// 
	void	SetData(char*pData,int nLen);

	
protected:

	char*	m_pData;

};



class CID3V2
{
public:
	// 构造初始化
	CID3V2();
	// 注意不能为virtual
	~CID3V2();

	friend class CMp3File;



public:
	// 不包括头的大小
	int					GetSize() const;
	
	// 保存
	void				Save(ofstream&ofs);

	void				SetTitle(const string&sTitle);

protected:

	void				LoadFrameData(const char*pTagFrame,int nSize);

protected:
	// 不包括头的大小
	void				SetSize(int nSize);
	
protected:
	
	int					FindFrame(char ID[4])
	{
		int iS = (int)m_items.size();
		for(int i=0;i<iS;i++)
		{
			if(strncmp(m_items[i].FrameID,ID,4)==0)
				return i;
		}
		return -1;
	};

	char*				MakeItem(LPCTSTR sz1,int nL1,LPCTSTR sz2,int nL2);


public:
	ID3V2_HEADER			m_header; /*帧头*/
protected:

	vector<CID3V2FrameItem>	m_items;
	int						m_headerFrameSize;
//	CID3V2FrameItem		m_TIT2;	/*标题 表示内容为这首歌的标题，下同*/
//	CID3V2FrameItem		m_TPE1;	/*作者*/
//	CID3V2FrameItem		m_TALB;	/*专集*/
//	CID3V2FrameItem		m_TYER;	/*出品年代 是用ASCII 码表示的数字*/
//	CID3V2FrameItem		m_TRCK;	/*音轨 格式：N/M 其中N 为专集中的第N 首，M 为专集中共M 首，N 和M 为ASCII 码表示的数字*/
//	CID3V2FrameItem		m_TCON;	/*类型*/
//	CID3V2FrameItem		m_COMM;	/*备注 格式："eng\0 备注内容"，其中eng 表示备注所使用的自然语言*/

};


/*
AAAAAAAA AAABBCCD EEEEFFGH IIJJKLMM
*/

enum MP3_VERSION{
	MP3Version2_5=0,
	MP3VersionReserved,
	MP3Version2,
	MP3Version1
};

enum MP3_LAYER{
	MP3L_Reserved=0,
	MP3L_LayerIII,
	MP3L_LayerII,
	MP3L_LayerI
};

enum MP3_PROTECTION_BIT
{
	MP3_Protected=0,		// Protected by CRC (16bit crc follows header)
	MP3_NotProtected
};


// 比特率表

static const int BITRATE_FREE=0;
static const int BITRATE_BAD=-1;
// [索引值][版本][层]
static const int BITRATE_TABLE[16][2][3]={
	{{BITRATE_FREE,BITRATE_FREE,BITRATE_FREE},{BITRATE_FREE,BITRATE_FREE,BITRATE_FREE}},
	{{32000,32000,32000},{32000,32000,8000}},
	{{64000,48000,40000},{64000,48000,16000}},
	{{96000,56000,48000},{96000,56000,24000}},
	{{128000,64000,56000},{128000,64000,32000}},
	{{160000,80000,64000},{160000,80000,64000}},
	{{192000,96000,80000},{192000,96000,80000}},
	{{224000,112000,96000},{224000,112000,56000}},
	{{256000,128000,112000},{256000,128000,64000}},
	{{288000,160000,128000},{288000,160000,128000}},
	{{320000,192000,160000},{320000,192000,160000}},
	{{352000,224000,192000},{352000,224000,112000}},
	{{384000,256000,224000},{384000,256000,128000}},
	{{416000,320000,256000},{416000,320000,256000}},
	{{448000,384000,320000},{448000,384000,320000}},
	{{BITRATE_BAD ,BITRATE_BAD ,BITRATE_BAD },{BITRATE_BAD ,BITRATE_BAD ,BITRATE_BAD}},
};

// 采样频率表
// [索引值][版本]
static const int SAMPLING_RATE_FREQUENCY_TABLE[4][3]={
	{44100 ,22050 ,11025},
	{48000 ,24000 ,12000},
	{32000 ,16000 ,8000},
	{0,0,0}	// 保留
};

enum MP3_CHANNEL_MODE{
	MP3_Stereo,
	MP3_Joint_Stereo,
	MP3_Dual_Channel,
	MP3_Single_Channel
};

/*
Mode extension (Only if Joint stereo)
value Intensity stereo MS stereo
00 off off
01 on off
10 off on
11 on on
K 1 (3) Copyright
0 - Audio is not copyrighted
1 - Audio is copyrighted
L 1 (2) Original
0 - Copy of original media
1 - Original media
M 2 (1,0) Emphasis
00 - none
01 - 50/15 ms
10 - reserved
11 - CCIT J.17
*/

#pragma pack(4)

//AAAAAAAA AAABBCCD EEEEFFGH IIJJKLMM
typedef struct tagMP3FrameHeader 
{
	union{
		struct{		
			unsigned int emphasis: 2;			// 强调模式		
			unsigned int original: 1;			// 原版标志	
			unsigned int copyright: 1;			// 版权	
			unsigned int mode_extension: 2;		// 扩充模式	
			unsigned int mode: 2;				// 声道模式	
			unsigned int resv: 1;				// 保留字
			unsigned int padding: 1;			// 帧长调节
			unsigned int sampling_frequency: 2;	// 采样频率
			unsigned int bitrate_index: 4;		// 位率
			unsigned int crc: 1;				// CRC校验
			unsigned int layer: 2;				// 层
			unsigned int version: 2;			// 版本
			unsigned int sync: 11;				// 同步信息	
		};
		UINT data;
		struct{
			unsigned char b1;
			unsigned char b2;
			unsigned char b3;
			unsigned char b4;
		};
	};
}MP3FREAMHEADER, *LPMP3FREAMHEADER;
#pragma pack()

class CMp3FrameItem
{
public:

	CMp3FrameItem();
	
	~CMp3FrameItem();

	CMp3FrameItem(const CMp3FrameItem&other);

	void operator =(const CMp3FrameItem&o);


public:
	// 失败返回-1,返回加载数据的长度
	int				Load(const char* pFrame);
	//
	void			Save(char*& pPool);
	//
	void			Save(ofstream& ofs);


	
	int				GetSize() const
	{
		int nSR=GetSamplingRate();
		if(nSR==0)
			return 0;
		MP3_LAYER l=GetLayer();
/*
. mpeg1.0       layer1 :	帧长= (48000*bitrate)/sampling_freq + padding
				layer2&3:	帧长= (144000*bitrate)/sampling_freq + padding
. mpeg2.0       layer1 :	帧长= (24000*bitrate)/sampling_freq + padding
				layer2&3 :	帧长= (72000*bitrate)/sampling_freq + padding
*/		
		if(GetVersion()==MP3Version1)
		{
			if(l==MP3L_LayerI)
				return 48*GetBitrate()/nSR+m_header.padding;
			else
				return 144*GetBitrate()/nSR+m_header.padding;
		}
		else
		{
			if(l==MP3L_LayerI)
				return 24*GetBitrate()/nSR+m_header.padding;
			else
				return 72*GetBitrate()/nSR+m_header.padding;
		}
		//return (((m_header.version == MP3Version1 ? 144 : 72) * GetBitrate()) / nSR) + m_header.padding;
	};

	int				GetBitrate() const
	{	
		int l=m_header.layer;
		int v=m_header.version;
		if(v==MP3Version2_5||v==MP3Version2) 
			v=1;
		else if(v==MP3Version1)
			v=0;
		else
		{
			v=1;
			assert(0);
		}
		if( l==	MP3L_LayerIII)
			l=2;
		else if(l==MP3L_LayerII)
			l=1;
		else if(l==MP3L_LayerI)
			l=0;
		else
		{
			l=0;
			assert(0);
		}
		return BITRATE_TABLE[m_header.bitrate_index][v][l];
	};


	int				GetSamplingRate() const
	{
		int v=m_header.version;
		if(v==MP3Version2_5) 
			v=2;
		else if(v==MP3Version2)
			v=1;
		else if(v==MP3Version1)
			v=0;
		else
		{
			v=1;
			assert(0);
		}
		return SAMPLING_RATE_FREQUENCY_TABLE[m_header.sampling_frequency][v];
	};

	BOOL			IsCopyright() const
	{
		return m_header.copyright;
	};

	MP3_LAYER		GetLayer() const
	{ return (MP3_LAYER) m_header.layer; };

	MP3_VERSION		GetVersion() const
	{ return (MP3_VERSION) m_header.version; };

	BYTE *				GetData()
	{
		return (BYTE*)m_pData;
	}

public:
	MP3FREAMHEADER		m_header;
protected:

	
	WORD				m_crc;
	char*				m_pData;	// 包括头
	int					m_nSize;	// 包括头
};

class CMp3File  
{
public:

	CMp3File();

	virtual ~CMp3File();

public:
	
	static int		GetFileSize(LPCTSTR szFile);

	const string&	GetLastErrMsg() const
	{ return m_sErrMsg; };

public:

	int				GetFrameCount() const
	{ return (int)m_frames.size(); };

	BYTE *			GetFrame(int n);

	int				GetFrameSize(int n);
	
	INT				LoadFile(LPCTSTR szFile);

	BOOL			SaveFile(LPCTSTR szFile,int nStart = 0);

	BOOL			HasID3V1() const
	{ return m_bID3V1; };

	BOOL			HasID3V2() const
	{ return m_bID3V2; };

protected:

	ifstream		m_ifs;
	ofstream		m_ofs;
	CID3V2			m_id3v2;
	CID3V1			m_id3v1;
	vector<CMp3FrameItem>
					m_frames;
	BOOL			m_bID3V1;
	BOOL			m_bID3V2;

	string			m_sErrMsg;

	
};

#endif 
