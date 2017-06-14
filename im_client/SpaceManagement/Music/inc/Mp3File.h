
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
	char Header[3];		/*��ǩͷ������"TAG"������Ϊû�б�ǩ*/
	char Title[30];		/*����*/
	char Artist[30];	/*����*/
	char Album[30];		/*ר��*/
	char Year[4];		/*��Ʒ���*/
	char Comment[28];	/*��ע*/
	char Reserve;		/*����*/
	char Track;			/*����*/
	char Genre;			/*����*/
}ID3V1,*PID3V1;
/*
--------------
����
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
	// �����ʼ��
	CID3V1();
	// ע�ⲻ��Ϊvirtual
	~CID3V1();

public:

	void operator = (const ID3V1 & v1);

public:
	// ���ñ���
	void			SetTitle(const string& sTitle);
	// ��ñ���
	string			GetTitle() const;
	// ��������
	void			SetArtist(const string& sArtist);
	// �������
	string			GetArtist() const;
	// ����ר��
	void			SetAlbum(const string& sAlbum);
	// ���ר��
	string			GetAlbum() const;
	// ���ó�Ʒ���
	void			SetYear(const string& sYear);
	// ��ó�Ʒ���
	string			GetYear() const;
	// ���ñ�ע
	void			SetComment(const string& sComment);
	// ��ñ�ע
	string			GetComment() const;
	// ��������
	void			SetTrack(char track);
	// �������
	char			GetTrack() const;
	// ��������
	void			SetGenre(char genre);
	// �������
	char			GetGenre() const;
	// ���浽�ļ�
	void			Save(ofstream& ofs);

};



struct ID3V2_HEADER
{
	char Header[3];		/*����Ϊ"ID3"������Ϊ��ǩ������*/
	char Ver;			/*�汾��ID3V2.3 �ͼ�¼3*/
	char Revision;		/*���汾�Ŵ˰汾��¼Ϊ0*/
	char Flag;			/*��ű�־���ֽڣ�����汾ֻ��������λ���Ժ���ϸ��˵*/
	byte Size[4];		/*��ǩ��С��������ǩͷ��10 ���ֽں����еı�ǩ֡�Ĵ�С*/
};


struct ID3V2_FRAME_HEADER
{
	char FrameID[4];	/*���ĸ��ַ���ʶһ��֡��˵�������ݣ��Ժ��г��õı�ʶ���ձ�*/
	byte Size[4];		/*֡���ݵĴ�С��������֡ͷ������С��1*/
	char Flags[2];		/*��ű�־��ֻ������6 λ���Ժ���ϸ��˵*/
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
	// ������ͷ�Ĵ�С
	int		GetSize() const;
	// ���ô�С������ͷ�Ĵ�С
	void	SetSize(int nSize);
	// ���ؼ��ص����ݵ��ܳ��ȣ�����ͷ�����������ʧ�ܣ�����-1
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
	// �����ʼ��
	CID3V2();
	// ע�ⲻ��Ϊvirtual
	~CID3V2();

	friend class CMp3File;



public:
	// ������ͷ�Ĵ�С
	int					GetSize() const;
	
	// ����
	void				Save(ofstream&ofs);

	void				SetTitle(const string&sTitle);

protected:

	void				LoadFrameData(const char*pTagFrame,int nSize);

protected:
	// ������ͷ�Ĵ�С
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
	ID3V2_HEADER			m_header; /*֡ͷ*/
protected:

	vector<CID3V2FrameItem>	m_items;
	int						m_headerFrameSize;
//	CID3V2FrameItem		m_TIT2;	/*���� ��ʾ����Ϊ���׸�ı��⣬��ͬ*/
//	CID3V2FrameItem		m_TPE1;	/*����*/
//	CID3V2FrameItem		m_TALB;	/*ר��*/
//	CID3V2FrameItem		m_TYER;	/*��Ʒ��� ����ASCII ���ʾ������*/
//	CID3V2FrameItem		m_TRCK;	/*���� ��ʽ��N/M ����N Ϊר���еĵ�N �ף�M Ϊר���й�M �ף�N ��M ΪASCII ���ʾ������*/
//	CID3V2FrameItem		m_TCON;	/*����*/
//	CID3V2FrameItem		m_COMM;	/*��ע ��ʽ��"eng\0 ��ע����"������eng ��ʾ��ע��ʹ�õ���Ȼ����*/

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


// �����ʱ�

static const int BITRATE_FREE=0;
static const int BITRATE_BAD=-1;
// [����ֵ][�汾][��]
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

// ����Ƶ�ʱ�
// [����ֵ][�汾]
static const int SAMPLING_RATE_FREQUENCY_TABLE[4][3]={
	{44100 ,22050 ,11025},
	{48000 ,24000 ,12000},
	{32000 ,16000 ,8000},
	{0,0,0}	// ����
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
			unsigned int emphasis: 2;			// ǿ��ģʽ		
			unsigned int original: 1;			// ԭ���־	
			unsigned int copyright: 1;			// ��Ȩ	
			unsigned int mode_extension: 2;		// ����ģʽ	
			unsigned int mode: 2;				// ����ģʽ	
			unsigned int resv: 1;				// ������
			unsigned int padding: 1;			// ֡������
			unsigned int sampling_frequency: 2;	// ����Ƶ��
			unsigned int bitrate_index: 4;		// λ��
			unsigned int crc: 1;				// CRCУ��
			unsigned int layer: 2;				// ��
			unsigned int version: 2;			// �汾
			unsigned int sync: 11;				// ͬ����Ϣ	
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
	// ʧ�ܷ���-1,���ؼ������ݵĳ���
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
. mpeg1.0       layer1 :	֡��= (48000*bitrate)/sampling_freq + padding
				layer2&3:	֡��= (144000*bitrate)/sampling_freq + padding
. mpeg2.0       layer1 :	֡��= (24000*bitrate)/sampling_freq + padding
				layer2&3 :	֡��= (72000*bitrate)/sampling_freq + padding
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
	char*				m_pData;	// ����ͷ
	int					m_nSize;	// ����ͷ
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
