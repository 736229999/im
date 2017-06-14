//
// sproxy.exe generated file
// do not modify this file
//
// Created: 03/15/2007@18:49:4
//

#pragma once


#ifndef _WIN32_WINDOWS
#pragma message("warning: defining _WIN32_WINDOWS = 0x0410")
#define _WIN32_WINDOWS 0x0410
#endif

#include <atlsoap.h>

namespace MiniPublic
{

template <typename TClient = CSoapSocketClientT<> >
class CMiniPublicT : 
	public TClient, 
	public CSoapRootHandler
{
protected:

	const _soapmap ** GetFunctionMap();
	const _soapmap ** GetHeaderMap();
	void * GetHeaderValue();
	const wchar_t * GetNamespaceUri();
	const char * GetServiceName();
	const char * GetNamespaceUriA();
	HRESULT CallFunction(
		void *pvParam, 
		const wchar_t *wszLocalName, int cchLocalName,
		size_t nItem);
	HRESULT GetClientReader(ISAXXMLReader **ppReader);

public:

	HRESULT __stdcall QueryInterface(REFIID riid, void **ppv)
	{
		if (ppv == NULL)
		{
			return E_POINTER;
		}

		*ppv = NULL;

		if (InlineIsEqualGUID(riid, IID_IUnknown) ||
			InlineIsEqualGUID(riid, IID_ISAXContentHandler))
		{
			*ppv = static_cast<ISAXContentHandler *>(this);
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	ULONG __stdcall AddRef()
	{
		return 1;
	}

	ULONG __stdcall Release()
	{
		return 1;
	}

	CMiniPublicT(CString s,ISAXXMLReader *pReader = NULL)
		:TClient(s)
	{
		SetClient(true);
		SetReader(pReader);
	}
	
	~CMiniPublicT()
	{
		Uninitialize();
	}
	
	void Uninitialize()
	{
		UninitializeSOAP();
	}	

	HRESULT SetUserMiniStatus(
		int userId, 
		int status, 
		BSTR* SetUserMiniStatusResult
	);

	HRESULT GetVouchFriend(
		int count, 
		BSTR* GetVouchFriendResult
	);

	HRESULT GetInterFaceUrl(
		BSTR key, 
		BSTR* GetInterFaceUrlResult
	);

	HRESULT GetUserDefaultFace(
		BSTR* GetUserDefaultFaceResult
	);

	HRESULT GetUserPhotoTag(
		int userId, 
		int tagCount, 
		BSTR* GetUserPhotoTagResult
	);

	HRESULT GetUserSpaceTotal(
		int userId, 
		int* GetUserSpaceTotalResult
	);

	HRESULT GetCommentList(
		int userId, 
		BSTR username, 
		BSTR* GetCommentListResult
	);

	HRESULT GetUserArticleTag(
		int userId, 
		int tagCount, 
		BSTR* GetUserArticleTagResult
	);

	HRESULT GetMessageList(
		int userId, 
		BSTR* GetMessageListResult
	);

	HRESULT GetTitleList(
		BSTR logId, 
		int logType, 
		BSTR* GetTitleListResult
	);
};

typedef CMiniPublicT<> CMiniPublic;

struct __CMiniPublic_SetUserMiniStatus_struct
{
	int userId;
	int status;
	BSTR SetUserMiniStatusResult;
};

extern __declspec(selectany) const _soapmapentry __CMiniPublic_SetUserMiniStatus_entries[] =
{

	{
		0x1945E48C, 
		"userId", 
		L"userId", 
		sizeof("userId")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CMiniPublic_SetUserMiniStatus_struct, userId),
		NULL,
		NULL,
		-1,
	},
	{
		0x14AB8884, 
		"status", 
		L"status", 
		sizeof("status")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CMiniPublic_SetUserMiniStatus_struct, status),
		NULL,
		NULL,
		-1,
	},
	{
		0x0A72DCDB, 
		"SetUserMiniStatusResult", 
		L"SetUserMiniStatusResult", 
		sizeof("SetUserMiniStatusResult")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CMiniPublic_SetUserMiniStatus_struct, SetUserMiniStatusResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMiniPublic_SetUserMiniStatus_map =
{
	0x72449FEB,
	"SetUserMiniStatus",
	L"SetUserMiniStatusResponse",
	sizeof("SetUserMiniStatus")-1,
	sizeof("SetUserMiniStatusResponse")-1,
	SOAPMAP_FUNC,
	__CMiniPublic_SetUserMiniStatus_entries,
	sizeof(__CMiniPublic_SetUserMiniStatus_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};


struct __CMiniPublic_GetVouchFriend_struct
{
	int count;
	BSTR GetVouchFriendResult;
};

extern __declspec(selectany) const _soapmapentry __CMiniPublic_GetVouchFriend_entries[] =
{

	{
		0x073E5849, 
		"count", 
		L"count", 
		sizeof("count")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CMiniPublic_GetVouchFriend_struct, count),
		NULL,
		NULL,
		-1,
	},
	{
		0x6F5BA0DC, 
		"GetVouchFriendResult", 
		L"GetVouchFriendResult", 
		sizeof("GetVouchFriendResult")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CMiniPublic_GetVouchFriend_struct, GetVouchFriendResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMiniPublic_GetVouchFriend_map =
{
	0xB46E682C,
	"GetVouchFriend",
	L"GetVouchFriendResponse",
	sizeof("GetVouchFriend")-1,
	sizeof("GetVouchFriendResponse")-1,
	SOAPMAP_FUNC,
	__CMiniPublic_GetVouchFriend_entries,
	sizeof(__CMiniPublic_GetVouchFriend_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};


struct __CMiniPublic_GetInterFaceUrl_struct
{
	BSTR key;
	BSTR GetInterFaceUrlResult;
};

extern __declspec(selectany) const _soapmapentry __CMiniPublic_GetInterFaceUrl_entries[] =
{

	{
		0x0001D4A9, 
		"key", 
		L"key", 
		sizeof("key")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CMiniPublic_GetInterFaceUrl_struct, key),
		NULL,
		NULL,
		-1,
	},
	{
		0x0AE91A63, 
		"GetInterFaceUrlResult", 
		L"GetInterFaceUrlResult", 
		sizeof("GetInterFaceUrlResult")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CMiniPublic_GetInterFaceUrl_struct, GetInterFaceUrlResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMiniPublic_GetInterFaceUrl_map =
{
	0x69405F73,
	"GetInterFaceUrl",
	L"GetInterFaceUrlResponse",
	sizeof("GetInterFaceUrl")-1,
	sizeof("GetInterFaceUrlResponse")-1,
	SOAPMAP_FUNC,
	__CMiniPublic_GetInterFaceUrl_entries,
	sizeof(__CMiniPublic_GetInterFaceUrl_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};


struct __CMiniPublic_GetUserDefaultFace_struct
{
	BSTR GetUserDefaultFaceResult;
};

extern __declspec(selectany) const _soapmapentry __CMiniPublic_GetUserDefaultFace_entries[] =
{

	{
		0x318DEB32, 
		"GetUserDefaultFaceResult", 
		L"GetUserDefaultFaceResult", 
		sizeof("GetUserDefaultFaceResult")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CMiniPublic_GetUserDefaultFace_struct, GetUserDefaultFaceResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMiniPublic_GetUserDefaultFace_map =
{
	0xCC5CA002,
	"GetUserDefaultFace",
	L"GetUserDefaultFaceResponse",
	sizeof("GetUserDefaultFace")-1,
	sizeof("GetUserDefaultFaceResponse")-1,
	SOAPMAP_FUNC,
	__CMiniPublic_GetUserDefaultFace_entries,
	sizeof(__CMiniPublic_GetUserDefaultFace_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};


struct __CMiniPublic_GetUserPhotoTag_struct
{
	int userId;
	int tagCount;
	BSTR GetUserPhotoTagResult;
};

extern __declspec(selectany) const _soapmapentry __CMiniPublic_GetUserPhotoTag_entries[] =
{

	{
		0x1945E48C, 
		"userId", 
		L"userId", 
		sizeof("userId")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CMiniPublic_GetUserPhotoTag_struct, userId),
		NULL,
		NULL,
		-1,
	},
	{
		0x2DE41C05, 
		"tagCount", 
		L"tagCount", 
		sizeof("tagCount")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CMiniPublic_GetUserPhotoTag_struct, tagCount),
		NULL,
		NULL,
		-1,
	},
	{
		0x5CF12084, 
		"GetUserPhotoTagResult", 
		L"GetUserPhotoTagResult", 
		sizeof("GetUserPhotoTagResult")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CMiniPublic_GetUserPhotoTag_struct, GetUserPhotoTagResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMiniPublic_GetUserPhotoTag_map =
{
	0x5D6271D4,
	"GetUserPhotoTag",
	L"GetUserPhotoTagResponse",
	sizeof("GetUserPhotoTag")-1,
	sizeof("GetUserPhotoTagResponse")-1,
	SOAPMAP_FUNC,
	__CMiniPublic_GetUserPhotoTag_entries,
	sizeof(__CMiniPublic_GetUserPhotoTag_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};


struct __CMiniPublic_GetUserSpaceTotal_struct
{
	int userId;
	int GetUserSpaceTotalResult;
};

extern __declspec(selectany) const _soapmapentry __CMiniPublic_GetUserSpaceTotal_entries[] =
{

	{
		0x1945E48C, 
		"userId", 
		L"userId", 
		sizeof("userId")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CMiniPublic_GetUserSpaceTotal_struct, userId),
		NULL,
		NULL,
		-1,
	},
	{
		0xF7C2B6CE, 
		"GetUserSpaceTotalResult", 
		L"GetUserSpaceTotalResult", 
		sizeof("GetUserSpaceTotalResult")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CMiniPublic_GetUserSpaceTotal_struct, GetUserSpaceTotalResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMiniPublic_GetUserSpaceTotal_map =
{
	0xF2F2C29E,
	"GetUserSpaceTotal",
	L"GetUserSpaceTotalResponse",
	sizeof("GetUserSpaceTotal")-1,
	sizeof("GetUserSpaceTotalResponse")-1,
	SOAPMAP_FUNC,
	__CMiniPublic_GetUserSpaceTotal_entries,
	sizeof(__CMiniPublic_GetUserSpaceTotal_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};


struct __CMiniPublic_GetCommentList_struct
{
	int userId;
	BSTR username;
	BSTR GetCommentListResult;
};

extern __declspec(selectany) const _soapmapentry __CMiniPublic_GetCommentList_entries[] =
{

	{
		0x1945E48C, 
		"userId", 
		L"userId", 
		sizeof("userId")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CMiniPublic_GetCommentList_struct, userId),
		NULL,
		NULL,
		-1,
	},
	{
		0x82658340, 
		"username", 
		L"username", 
		sizeof("username")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CMiniPublic_GetCommentList_struct, username),
		NULL,
		NULL,
		-1,
	},
	{
		0x4BC5D9EE, 
		"GetCommentListResult", 
		L"GetCommentListResult", 
		sizeof("GetCommentListResult")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CMiniPublic_GetCommentList_struct, GetCommentListResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMiniPublic_GetCommentList_map =
{
	0x544B2DBE,
	"GetCommentList",
	L"GetCommentListResponse",
	sizeof("GetCommentList")-1,
	sizeof("GetCommentListResponse")-1,
	SOAPMAP_FUNC,
	__CMiniPublic_GetCommentList_entries,
	sizeof(__CMiniPublic_GetCommentList_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};


struct __CMiniPublic_GetUserArticleTag_struct
{
	int userId;
	int tagCount;
	BSTR GetUserArticleTagResult;
};

extern __declspec(selectany) const _soapmapentry __CMiniPublic_GetUserArticleTag_entries[] =
{

	{
		0x1945E48C, 
		"userId", 
		L"userId", 
		sizeof("userId")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CMiniPublic_GetUserArticleTag_struct, userId),
		NULL,
		NULL,
		-1,
	},
	{
		0x2DE41C05, 
		"tagCount", 
		L"tagCount", 
		sizeof("tagCount")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CMiniPublic_GetUserArticleTag_struct, tagCount),
		NULL,
		NULL,
		-1,
	},
	{
		0x8A82B09E, 
		"GetUserArticleTagResult", 
		L"GetUserArticleTagResult", 
		sizeof("GetUserArticleTagResult")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CMiniPublic_GetUserArticleTag_struct, GetUserArticleTagResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMiniPublic_GetUserArticleTag_map =
{
	0x3598706E,
	"GetUserArticleTag",
	L"GetUserArticleTagResponse",
	sizeof("GetUserArticleTag")-1,
	sizeof("GetUserArticleTagResponse")-1,
	SOAPMAP_FUNC,
	__CMiniPublic_GetUserArticleTag_entries,
	sizeof(__CMiniPublic_GetUserArticleTag_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};


struct __CMiniPublic_GetMessageList_struct
{
	int userId;
	BSTR GetMessageListResult;
};

extern __declspec(selectany) const _soapmapentry __CMiniPublic_GetMessageList_entries[] =
{

	{
		0x1945E48C, 
		"userId", 
		L"userId", 
		sizeof("userId")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CMiniPublic_GetMessageList_struct, userId),
		NULL,
		NULL,
		-1,
	},
	{
		0x60D0CD00, 
		"GetMessageListResult", 
		L"GetMessageListResult", 
		sizeof("GetMessageListResult")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CMiniPublic_GetMessageList_struct, GetMessageListResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMiniPublic_GetMessageList_map =
{
	0xD7DF2D50,
	"GetMessageList",
	L"GetMessageListResponse",
	sizeof("GetMessageList")-1,
	sizeof("GetMessageListResponse")-1,
	SOAPMAP_FUNC,
	__CMiniPublic_GetMessageList_entries,
	sizeof(__CMiniPublic_GetMessageList_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};


struct __CMiniPublic_GetTitleList_struct
{
	BSTR logId;
	int logType;
	BSTR GetTitleListResult;
};

extern __declspec(selectany) const _soapmapentry __CMiniPublic_GetTitleList_entries[] =
{

	{
		0x07E0F46F, 
		"logId", 
		L"logId", 
		sizeof("logId")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CMiniPublic_GetTitleList_struct, logId),
		NULL,
		NULL,
		-1,
	},
	{
		0x83F63C84, 
		"logType", 
		L"logType", 
		sizeof("logType")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CMiniPublic_GetTitleList_struct, logType),
		NULL,
		NULL,
		-1,
	},
	{
		0x6014FDBD, 
		"GetTitleListResult", 
		L"GetTitleListResult", 
		sizeof("GetTitleListResult")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CMiniPublic_GetTitleList_struct, GetTitleListResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMiniPublic_GetTitleList_map =
{
	0xB8F2814D,
	"GetTitleList",
	L"GetTitleListResponse",
	sizeof("GetTitleList")-1,
	sizeof("GetTitleListResponse")-1,
	SOAPMAP_FUNC,
	__CMiniPublic_GetTitleList_entries,
	sizeof(__CMiniPublic_GetTitleList_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xC2E575C3,
	"http://tempuri.org/",
	L"http://tempuri.org/",
	sizeof("http://tempuri.org/")-1
};

extern __declspec(selectany) const _soapmap * __CMiniPublic_funcs[] =
{
	&__CMiniPublic_SetUserMiniStatus_map,
	&__CMiniPublic_GetVouchFriend_map,
	&__CMiniPublic_GetInterFaceUrl_map,
	&__CMiniPublic_GetUserDefaultFace_map,
	&__CMiniPublic_GetUserPhotoTag_map,
	&__CMiniPublic_GetUserSpaceTotal_map,
	&__CMiniPublic_GetCommentList_map,
	&__CMiniPublic_GetUserArticleTag_map,
	&__CMiniPublic_GetMessageList_map,
	&__CMiniPublic_GetTitleList_map,
	NULL
};

template <typename TClient>
inline HRESULT CMiniPublicT<TClient>::SetUserMiniStatus(
		int userId, 
		int status, 
		BSTR* SetUserMiniStatusResult
	)
{
    if ( SetUserMiniStatusResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMiniPublic_SetUserMiniStatus_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.userId = userId;
	__params.status = status;

	__atlsoap_hr = SetClientStruct(&__params, 0);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/SetUserMiniStatus\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*SetUserMiniStatusResult = __params.SetUserMiniStatusResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMiniPublicT<TClient>::GetVouchFriend(
		int count, 
		BSTR* GetVouchFriendResult
	)
{
    if ( GetVouchFriendResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMiniPublic_GetVouchFriend_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.count = count;

	__atlsoap_hr = SetClientStruct(&__params, 1);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetVouchFriend\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetVouchFriendResult = __params.GetVouchFriendResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMiniPublicT<TClient>::GetInterFaceUrl(
		BSTR key, 
		BSTR* GetInterFaceUrlResult
	)
{
    if ( GetInterFaceUrlResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMiniPublic_GetInterFaceUrl_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.key = key;

	__atlsoap_hr = SetClientStruct(&__params, 2);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetInterFaceUrl\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetInterFaceUrlResult = __params.GetInterFaceUrlResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMiniPublicT<TClient>::GetUserDefaultFace(
		BSTR* GetUserDefaultFaceResult
	)
{
    if ( GetUserDefaultFaceResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMiniPublic_GetUserDefaultFace_struct __params;
	memset(&__params, 0x00, sizeof(__params));

	__atlsoap_hr = SetClientStruct(&__params, 3);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetUserDefaultFace\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetUserDefaultFaceResult = __params.GetUserDefaultFaceResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMiniPublicT<TClient>::GetUserPhotoTag(
		int userId, 
		int tagCount, 
		BSTR* GetUserPhotoTagResult
	)
{
    if ( GetUserPhotoTagResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMiniPublic_GetUserPhotoTag_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.userId = userId;
	__params.tagCount = tagCount;

	__atlsoap_hr = SetClientStruct(&__params, 4);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetUserPhotoTag\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetUserPhotoTagResult = __params.GetUserPhotoTagResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMiniPublicT<TClient>::GetUserSpaceTotal(
		int userId, 
		int* GetUserSpaceTotalResult
	)
{
    if ( GetUserSpaceTotalResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMiniPublic_GetUserSpaceTotal_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.userId = userId;

	__atlsoap_hr = SetClientStruct(&__params, 5);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetUserSpaceTotal\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetUserSpaceTotalResult = __params.GetUserSpaceTotalResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMiniPublicT<TClient>::GetCommentList(
		int userId, 
		BSTR username, 
		BSTR* GetCommentListResult
	)
{
    if ( GetCommentListResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMiniPublic_GetCommentList_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.userId = userId;
	__params.username = username;

	__atlsoap_hr = SetClientStruct(&__params, 6);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetCommentList\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetCommentListResult = __params.GetCommentListResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMiniPublicT<TClient>::GetUserArticleTag(
		int userId, 
		int tagCount, 
		BSTR* GetUserArticleTagResult
	)
{
    if ( GetUserArticleTagResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMiniPublic_GetUserArticleTag_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.userId = userId;
	__params.tagCount = tagCount;

	__atlsoap_hr = SetClientStruct(&__params, 7);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetUserArticleTag\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetUserArticleTagResult = __params.GetUserArticleTagResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMiniPublicT<TClient>::GetMessageList(
		int userId, 
		BSTR* GetMessageListResult
	)
{
    if ( GetMessageListResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMiniPublic_GetMessageList_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.userId = userId;

	__atlsoap_hr = SetClientStruct(&__params, 8);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetMessageList\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetMessageListResult = __params.GetMessageListResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMiniPublicT<TClient>::GetTitleList(
		BSTR logId, 
		int logType, 
		BSTR* GetTitleListResult
	)
{
    if ( GetTitleListResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMiniPublic_GetTitleList_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.logId = logId;
	__params.logType = logType;

	__atlsoap_hr = SetClientStruct(&__params, 9);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetTitleList\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetTitleListResult = __params.GetTitleListResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CMiniPublicT<TClient>::GetFunctionMap()
{
	return __CMiniPublic_funcs;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CMiniPublicT<TClient>::GetHeaderMap()
{
	static const _soapmapentry __CMiniPublic_SetUserMiniStatus_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMiniPublic_SetUserMiniStatus_atlsoapheader_map = 
	{
		0x72449FEB,
		"SetUserMiniStatus",
		L"SetUserMiniStatusResponse",
		sizeof("SetUserMiniStatus")-1,
		sizeof("SetUserMiniStatusResponse")-1,
		SOAPMAP_HEADER,
		__CMiniPublic_SetUserMiniStatus_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};

	static const _soapmapentry __CMiniPublic_GetVouchFriend_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMiniPublic_GetVouchFriend_atlsoapheader_map = 
	{
		0xB46E682C,
		"GetVouchFriend",
		L"GetVouchFriendResponse",
		sizeof("GetVouchFriend")-1,
		sizeof("GetVouchFriendResponse")-1,
		SOAPMAP_HEADER,
		__CMiniPublic_GetVouchFriend_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};

	static const _soapmapentry __CMiniPublic_GetInterFaceUrl_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMiniPublic_GetInterFaceUrl_atlsoapheader_map = 
	{
		0x69405F73,
		"GetInterFaceUrl",
		L"GetInterFaceUrlResponse",
		sizeof("GetInterFaceUrl")-1,
		sizeof("GetInterFaceUrlResponse")-1,
		SOAPMAP_HEADER,
		__CMiniPublic_GetInterFaceUrl_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};

	static const _soapmapentry __CMiniPublic_GetUserDefaultFace_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMiniPublic_GetUserDefaultFace_atlsoapheader_map = 
	{
		0xCC5CA002,
		"GetUserDefaultFace",
		L"GetUserDefaultFaceResponse",
		sizeof("GetUserDefaultFace")-1,
		sizeof("GetUserDefaultFaceResponse")-1,
		SOAPMAP_HEADER,
		__CMiniPublic_GetUserDefaultFace_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};

	static const _soapmapentry __CMiniPublic_GetUserPhotoTag_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMiniPublic_GetUserPhotoTag_atlsoapheader_map = 
	{
		0x5D6271D4,
		"GetUserPhotoTag",
		L"GetUserPhotoTagResponse",
		sizeof("GetUserPhotoTag")-1,
		sizeof("GetUserPhotoTagResponse")-1,
		SOAPMAP_HEADER,
		__CMiniPublic_GetUserPhotoTag_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};

	static const _soapmapentry __CMiniPublic_GetUserSpaceTotal_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMiniPublic_GetUserSpaceTotal_atlsoapheader_map = 
	{
		0xF2F2C29E,
		"GetUserSpaceTotal",
		L"GetUserSpaceTotalResponse",
		sizeof("GetUserSpaceTotal")-1,
		sizeof("GetUserSpaceTotalResponse")-1,
		SOAPMAP_HEADER,
		__CMiniPublic_GetUserSpaceTotal_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};

	static const _soapmapentry __CMiniPublic_GetCommentList_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMiniPublic_GetCommentList_atlsoapheader_map = 
	{
		0x544B2DBE,
		"GetCommentList",
		L"GetCommentListResponse",
		sizeof("GetCommentList")-1,
		sizeof("GetCommentListResponse")-1,
		SOAPMAP_HEADER,
		__CMiniPublic_GetCommentList_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};

	static const _soapmapentry __CMiniPublic_GetUserArticleTag_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMiniPublic_GetUserArticleTag_atlsoapheader_map = 
	{
		0x3598706E,
		"GetUserArticleTag",
		L"GetUserArticleTagResponse",
		sizeof("GetUserArticleTag")-1,
		sizeof("GetUserArticleTagResponse")-1,
		SOAPMAP_HEADER,
		__CMiniPublic_GetUserArticleTag_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};

	static const _soapmapentry __CMiniPublic_GetMessageList_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMiniPublic_GetMessageList_atlsoapheader_map = 
	{
		0xD7DF2D50,
		"GetMessageList",
		L"GetMessageListResponse",
		sizeof("GetMessageList")-1,
		sizeof("GetMessageListResponse")-1,
		SOAPMAP_HEADER,
		__CMiniPublic_GetMessageList_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};

	static const _soapmapentry __CMiniPublic_GetTitleList_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMiniPublic_GetTitleList_atlsoapheader_map = 
	{
		0xB8F2814D,
		"GetTitleList",
		L"GetTitleListResponse",
		sizeof("GetTitleList")-1,
		sizeof("GetTitleListResponse")-1,
		SOAPMAP_HEADER,
		__CMiniPublic_GetTitleList_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xC2E575C3,
		"http://tempuri.org/",
		L"http://tempuri.org/",
		sizeof("http://tempuri.org/")-1
	};


	static const _soapmap * __CMiniPublic_headers[] =
	{
		&__CMiniPublic_SetUserMiniStatus_atlsoapheader_map,
		&__CMiniPublic_GetVouchFriend_atlsoapheader_map,
		&__CMiniPublic_GetInterFaceUrl_atlsoapheader_map,
		&__CMiniPublic_GetUserDefaultFace_atlsoapheader_map,
		&__CMiniPublic_GetUserPhotoTag_atlsoapheader_map,
		&__CMiniPublic_GetUserSpaceTotal_atlsoapheader_map,
		&__CMiniPublic_GetCommentList_atlsoapheader_map,
		&__CMiniPublic_GetUserArticleTag_atlsoapheader_map,
		&__CMiniPublic_GetMessageList_atlsoapheader_map,
		&__CMiniPublic_GetTitleList_atlsoapheader_map,
		NULL
	};
	
	return __CMiniPublic_headers;
}

template <typename TClient>
ATL_NOINLINE inline void * CMiniPublicT<TClient>::GetHeaderValue()
{
	return this;
}

template <typename TClient>
ATL_NOINLINE inline const wchar_t * CMiniPublicT<TClient>::GetNamespaceUri()
{
	return L"http://tempuri.org/";
}

template <typename TClient>
ATL_NOINLINE inline const char * CMiniPublicT<TClient>::GetServiceName()
{
	return NULL;
}

template <typename TClient>
ATL_NOINLINE inline const char * CMiniPublicT<TClient>::GetNamespaceUriA()
{
	return "http://tempuri.org/";
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CMiniPublicT<TClient>::CallFunction(
	void *, 
	const wchar_t *, int,
	size_t)
{
	return E_NOTIMPL;
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CMiniPublicT<TClient>::GetClientReader(ISAXXMLReader **ppReader)
{
	if (ppReader == NULL)
	{
		return E_INVALIDARG;
	}
	
	CComPtr<ISAXXMLReader> spReader = GetReader();
	if (spReader.p != NULL)
	{
		*ppReader = spReader.Detach();
		return S_OK;
	}
	return TClient::GetClientReader(ppReader);
}

} // namespace MiniPublic
