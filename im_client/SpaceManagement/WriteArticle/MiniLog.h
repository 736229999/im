//
// sproxy.exe generated file
// do not modify this file
//
// Created: 03/21/2007@10:37:59
//

#pragma once


#ifndef _WIN32_WINDOWS
#pragma message("warning: defining _WIN32_WINDOWS = 0x0410")
#define _WIN32_WINDOWS 0x0410
#endif

#include <atlsoap.h>

namespace MiniLog
{

	template <typename TClient = CSoapSocketClientT<> >
	class CMiniLogT : 
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

		CMiniLogT(CString s,ISAXXMLReader *pReader = NULL)
			:TClient(s)
		{
			SetClient(true);
			SetReader(pReader);
		}

		~CMiniLogT()
		{
			Uninitialize();
		}

		void Uninitialize()
		{
			UninitializeSOAP();
		}	

		HRESULT GetArticleBookList(
			int articleId, 
			BSTR* GetArticleBookListResult
			);

		HRESULT GetLastArticleUpdateTime(
			int userId, 
			int friendId, 
			BSTR* GetLastArticleUpdateTimeResult
			);

		HRESULT GetUserBookList(
			BSTR userId, 
			BSTR* GetUserBookListResult
			);

		HRESULT GetUserAlbumPicList(
			int userId, 
			int bookId, 
			BSTR* GetUserAlbumPicListResult
			);

		HRESULT SaveLastArticleUpdateTime(
			int userId, 
			int friendId, 
			BSTR* SaveLastArticleUpdateTimeResult
			);

		HRESULT GetArticleInfo(
			int articleId, 
			BSTR* GetArticleInfoResult
			);

		HRESULT GetUserArticleList(
			BSTR userId, 
			int operUserId, 
			BSTR beginDate, 
			BSTR endDate, 
			BSTR* GetUserArticleListResult
			);
	};

	typedef CMiniLogT<> CMiniLog;

	struct __CMiniLog_GetArticleBookList_struct
	{
		int articleId;
		BSTR GetArticleBookListResult;
	};

	extern __declspec(selectany) const _soapmapentry __CMiniLog_GetArticleBookList_entries[] =
	{

		{
			0x36053951, 
				"articleId", 
				L"articleId", 
				sizeof("articleId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniLog_GetArticleBookList_struct, articleId),
				NULL,
				NULL,
				-1,
		},
		{
			0x6577406A, 
				"GetArticleBookListResult", 
				L"GetArticleBookListResult", 
				sizeof("GetArticleBookListResult")-1, 
				SOAPTYPE_STRING, 
				SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
				offsetof(__CMiniLog_GetArticleBookList_struct, GetArticleBookListResult),
				NULL,
				NULL,
				-1,
		},
			{ 0x00000000 }
	};

	extern __declspec(selectany) const _soapmap __CMiniLog_GetArticleBookList_map =
	{
		0x9FF0233A,
			"GetArticleBookList",
			L"GetArticleBookListResponse",
			sizeof("GetArticleBookList")-1,
			sizeof("GetArticleBookListResponse")-1,
			SOAPMAP_FUNC,
			__CMiniLog_GetArticleBookList_entries,
			sizeof(__CMiniLog_GetArticleBookList_struct),
			1,
			-1,
			SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
			0xC2E575C3,
			"http://tempuri.org/",
			L"http://tempuri.org/",
			sizeof("http://tempuri.org/")-1
	};


	struct __CMiniLog_GetLastArticleUpdateTime_struct
	{
		int userId;
		int friendId;
		BSTR GetLastArticleUpdateTimeResult;
	};

	extern __declspec(selectany) const _soapmapentry __CMiniLog_GetLastArticleUpdateTime_entries[] =
	{

		{
			0x1945E48C, 
				"userId", 
				L"userId", 
				sizeof("userId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniLog_GetLastArticleUpdateTime_struct, userId),
				NULL,
				NULL,
				-1,
		},
		{
			0x65F93965, 
				"friendId", 
				L"friendId", 
				sizeof("friendId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniLog_GetLastArticleUpdateTime_struct, friendId),
				NULL,
				NULL,
				-1,
		},
			{
				0x70E001E9, 
					"GetLastArticleUpdateTimeResult", 
					L"GetLastArticleUpdateTimeResult", 
					sizeof("GetLastArticleUpdateTimeResult")-1, 
					SOAPTYPE_STRING, 
					SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
					offsetof(__CMiniLog_GetLastArticleUpdateTime_struct, GetLastArticleUpdateTimeResult),
					NULL,
					NULL,
					-1,
			},
			{ 0x00000000 }
	};

	extern __declspec(selectany) const _soapmap __CMiniLog_GetLastArticleUpdateTime_map =
	{
		0x288F4079,
			"GetLastArticleUpdateTime",
			L"GetLastArticleUpdateTimeResponse",
			sizeof("GetLastArticleUpdateTime")-1,
			sizeof("GetLastArticleUpdateTimeResponse")-1,
			SOAPMAP_FUNC,
			__CMiniLog_GetLastArticleUpdateTime_entries,
			sizeof(__CMiniLog_GetLastArticleUpdateTime_struct),
			1,
			-1,
			SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
			0xC2E575C3,
			"http://tempuri.org/",
			L"http://tempuri.org/",
			sizeof("http://tempuri.org/")-1
	};


	struct __CMiniLog_GetUserBookList_struct
	{
		BSTR userId;
		BSTR GetUserBookListResult;
	};

	extern __declspec(selectany) const _soapmapentry __CMiniLog_GetUserBookList_entries[] =
	{

		{
			0x1945E48C, 
				"userId", 
				L"userId", 
				sizeof("userId")-1, 
				SOAPTYPE_STRING, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
				offsetof(__CMiniLog_GetUserBookList_struct, userId),
				NULL,
				NULL,
				-1,
		},
		{
			0x20DE5825, 
				"GetUserBookListResult", 
				L"GetUserBookListResult", 
				sizeof("GetUserBookListResult")-1, 
				SOAPTYPE_STRING, 
				SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
				offsetof(__CMiniLog_GetUserBookList_struct, GetUserBookListResult),
				NULL,
				NULL,
				-1,
		},
			{ 0x00000000 }
	};

	extern __declspec(selectany) const _soapmap __CMiniLog_GetUserBookList_map =
	{
		0xD17C15B5,
			"GetUserBookList",
			L"GetUserBookListResponse",
			sizeof("GetUserBookList")-1,
			sizeof("GetUserBookListResponse")-1,
			SOAPMAP_FUNC,
			__CMiniLog_GetUserBookList_entries,
			sizeof(__CMiniLog_GetUserBookList_struct),
			1,
			-1,
			SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
			0xC2E575C3,
			"http://tempuri.org/",
			L"http://tempuri.org/",
			sizeof("http://tempuri.org/")-1
	};


	struct __CMiniLog_GetUserAlbumPicList_struct
	{
		int userId;
		int bookId;
		BSTR GetUserAlbumPicListResult;
	};

	extern __declspec(selectany) const _soapmapentry __CMiniLog_GetUserAlbumPicList_entries[] =
	{

		{
			0x1945E48C, 
				"userId", 
				L"userId", 
				sizeof("userId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniLog_GetUserAlbumPicList_struct, userId),
				NULL,
				NULL,
				-1,
		},
		{
			0xECB0DC98, 
				"bookId", 
				L"bookId", 
				sizeof("bookId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniLog_GetUserAlbumPicList_struct, bookId),
				NULL,
				NULL,
				-1,
		},
			{
				0x03F4E8E7, 
					"GetUserAlbumPicListResult", 
					L"GetUserAlbumPicListResult", 
					sizeof("GetUserAlbumPicListResult")-1, 
					SOAPTYPE_STRING, 
					SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
					offsetof(__CMiniLog_GetUserAlbumPicList_struct, GetUserAlbumPicListResult),
					NULL,
					NULL,
					-1,
			},
			{ 0x00000000 }
	};

	extern __declspec(selectany) const _soapmap __CMiniLog_GetUserAlbumPicList_map =
	{
		0xD479DEF7,
			"GetUserAlbumPicList",
			L"GetUserAlbumPicListResponse",
			sizeof("GetUserAlbumPicList")-1,
			sizeof("GetUserAlbumPicListResponse")-1,
			SOAPMAP_FUNC,
			__CMiniLog_GetUserAlbumPicList_entries,
			sizeof(__CMiniLog_GetUserAlbumPicList_struct),
			1,
			-1,
			SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
			0xC2E575C3,
			"http://tempuri.org/",
			L"http://tempuri.org/",
			sizeof("http://tempuri.org/")-1
	};


	struct __CMiniLog_SaveLastArticleUpdateTime_struct
	{
		int userId;
		int friendId;
		BSTR SaveLastArticleUpdateTimeResult;
	};

	extern __declspec(selectany) const _soapmapentry __CMiniLog_SaveLastArticleUpdateTime_entries[] =
	{

		{
			0x1945E48C, 
				"userId", 
				L"userId", 
				sizeof("userId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniLog_SaveLastArticleUpdateTime_struct, userId),
				NULL,
				NULL,
				-1,
		},
		{
			0x65F93965, 
				"friendId", 
				L"friendId", 
				sizeof("friendId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniLog_SaveLastArticleUpdateTime_struct, friendId),
				NULL,
				NULL,
				-1,
		},
			{
				0xFEF958B8, 
					"SaveLastArticleUpdateTimeResult", 
					L"SaveLastArticleUpdateTimeResult", 
					sizeof("SaveLastArticleUpdateTimeResult")-1, 
					SOAPTYPE_STRING, 
					SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
					offsetof(__CMiniLog_SaveLastArticleUpdateTime_struct, SaveLastArticleUpdateTimeResult),
					NULL,
					NULL,
					-1,
			},
			{ 0x00000000 }
	};

	extern __declspec(selectany) const _soapmap __CMiniLog_SaveLastArticleUpdateTime_map =
	{
		0xA2598708,
			"SaveLastArticleUpdateTime",
			L"SaveLastArticleUpdateTimeResponse",
			sizeof("SaveLastArticleUpdateTime")-1,
			sizeof("SaveLastArticleUpdateTimeResponse")-1,
			SOAPMAP_FUNC,
			__CMiniLog_SaveLastArticleUpdateTime_entries,
			sizeof(__CMiniLog_SaveLastArticleUpdateTime_struct),
			1,
			-1,
			SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
			0xC2E575C3,
			"http://tempuri.org/",
			L"http://tempuri.org/",
			sizeof("http://tempuri.org/")-1
	};


	struct __CMiniLog_GetArticleInfo_struct
	{
		int articleId;
		BSTR GetArticleInfoResult;
	};

	extern __declspec(selectany) const _soapmapentry __CMiniLog_GetArticleInfo_entries[] =
	{

		{
			0x36053951, 
				"articleId", 
				L"articleId", 
				sizeof("articleId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniLog_GetArticleInfo_struct, articleId),
				NULL,
				NULL,
				-1,
		},
		{
			0xBDA4B72F, 
				"GetArticleInfoResult", 
				L"GetArticleInfoResult", 
				sizeof("GetArticleInfoResult")-1, 
				SOAPTYPE_STRING, 
				SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
				offsetof(__CMiniLog_GetArticleInfo_struct, GetArticleInfoResult),
				NULL,
				NULL,
				-1,
		},
			{ 0x00000000 }
	};

	extern __declspec(selectany) const _soapmap __CMiniLog_GetArticleInfo_map =
	{
		0xB9565F3F,
			"GetArticleInfo",
			L"GetArticleInfoResponse",
			sizeof("GetArticleInfo")-1,
			sizeof("GetArticleInfoResponse")-1,
			SOAPMAP_FUNC,
			__CMiniLog_GetArticleInfo_entries,
			sizeof(__CMiniLog_GetArticleInfo_struct),
			1,
			-1,
			SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
			0xC2E575C3,
			"http://tempuri.org/",
			L"http://tempuri.org/",
			sizeof("http://tempuri.org/")-1
	};


	struct __CMiniLog_GetUserArticleList_struct
	{
		BSTR userId;
		int operUserId;
		BSTR beginDate;
		BSTR endDate;
		BSTR GetUserArticleListResult;
	};

	extern __declspec(selectany) const _soapmapentry __CMiniLog_GetUserArticleList_entries[] =
	{

		{
			0x1945E48C, 
				"userId", 
				L"userId", 
				sizeof("userId")-1, 
				SOAPTYPE_STRING, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
				offsetof(__CMiniLog_GetUserArticleList_struct, userId),
				NULL,
				NULL,
				-1,
		},
		{
			0x030F38E2, 
				"operUserId", 
				L"operUserId", 
				sizeof("operUserId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniLog_GetUserArticleList_struct, operUserId),
				NULL,
				NULL,
				-1,
		},
			{
				0xC325D803, 
					"beginDate", 
					L"beginDate", 
					sizeof("beginDate")-1, 
					SOAPTYPE_STRING, 
					SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
					offsetof(__CMiniLog_GetUserArticleList_struct, beginDate),
					NULL,
					NULL,
					-1,
			},
			{
				0x6689F575, 
					"endDate", 
					L"endDate", 
					sizeof("endDate")-1, 
					SOAPTYPE_STRING, 
					SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
					offsetof(__CMiniLog_GetUserArticleList_struct, endDate),
					NULL,
					NULL,
					-1,
			},
				{
					0x7D69817E, 
						"GetUserArticleListResult", 
						L"GetUserArticleListResult", 
						sizeof("GetUserArticleListResult")-1, 
						SOAPTYPE_STRING, 
						SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
						offsetof(__CMiniLog_GetUserArticleList_struct, GetUserArticleListResult),
						NULL,
						NULL,
						-1,
				},
				{ 0x00000000 }
	};

	extern __declspec(selectany) const _soapmap __CMiniLog_GetUserArticleList_map =
	{
		0x7D76F94E,
			"GetUserArticleList",
			L"GetUserArticleListResponse",
			sizeof("GetUserArticleList")-1,
			sizeof("GetUserArticleListResponse")-1,
			SOAPMAP_FUNC,
			__CMiniLog_GetUserArticleList_entries,
			sizeof(__CMiniLog_GetUserArticleList_struct),
			1,
			-1,
			SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
			0xC2E575C3,
			"http://tempuri.org/",
			L"http://tempuri.org/",
			sizeof("http://tempuri.org/")-1
	};

	extern __declspec(selectany) const _soapmap * __CMiniLog_funcs[] =
	{
		&__CMiniLog_GetArticleBookList_map,
			&__CMiniLog_GetLastArticleUpdateTime_map,
			&__CMiniLog_GetUserBookList_map,
			&__CMiniLog_GetUserAlbumPicList_map,
			&__CMiniLog_SaveLastArticleUpdateTime_map,
			&__CMiniLog_GetArticleInfo_map,
			&__CMiniLog_GetUserArticleList_map,
			NULL
	};

	template <typename TClient>
		inline HRESULT CMiniLogT<TClient>::GetArticleBookList(
		int articleId, 
		BSTR* GetArticleBookListResult
		)
	{
		if ( GetArticleBookListResult == NULL )
			return E_POINTER;

		HRESULT __atlsoap_hr = InitializeSOAP(NULL);
		if (FAILED(__atlsoap_hr))
		{
			SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
			return __atlsoap_hr;
		}

		CleanupClient();

		CComPtr<IStream> __atlsoap_spReadStream;
		__CMiniLog_GetArticleBookList_struct __params;
		memset(&__params, 0x00, sizeof(__params));
		__params.articleId = articleId;

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

		__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetArticleBookList\"\r\n"));
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

		*GetArticleBookListResult = __params.GetArticleBookListResult;
		goto __skip_cleanup;

__cleanup:
		Cleanup();
__skip_cleanup:
		ResetClientState(true);
		memset(&__params, 0x00, sizeof(__params));
		return __atlsoap_hr;
	}

	template <typename TClient>
		inline HRESULT CMiniLogT<TClient>::GetLastArticleUpdateTime(
		int userId, 
		int friendId, 
		BSTR* GetLastArticleUpdateTimeResult
		)
	{
		if ( GetLastArticleUpdateTimeResult == NULL )
			return E_POINTER;

		HRESULT __atlsoap_hr = InitializeSOAP(NULL);
		if (FAILED(__atlsoap_hr))
		{
			SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
			return __atlsoap_hr;
		}

		CleanupClient();

		CComPtr<IStream> __atlsoap_spReadStream;
		__CMiniLog_GetLastArticleUpdateTime_struct __params;
		memset(&__params, 0x00, sizeof(__params));
		__params.userId = userId;
		__params.friendId = friendId;

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

		__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetLastArticleUpdateTime\"\r\n"));
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

		*GetLastArticleUpdateTimeResult = __params.GetLastArticleUpdateTimeResult;
		goto __skip_cleanup;

__cleanup:
		Cleanup();
__skip_cleanup:
		ResetClientState(true);
		memset(&__params, 0x00, sizeof(__params));
		return __atlsoap_hr;
	}

	template <typename TClient>
		inline HRESULT CMiniLogT<TClient>::GetUserBookList(
		BSTR userId, 
		BSTR* GetUserBookListResult
		)
	{
		if ( GetUserBookListResult == NULL )
			return E_POINTER;

		HRESULT __atlsoap_hr = InitializeSOAP(NULL);
		if (FAILED(__atlsoap_hr))
		{
			SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
			return __atlsoap_hr;
		}

		CleanupClient();

		CComPtr<IStream> __atlsoap_spReadStream;
		__CMiniLog_GetUserBookList_struct __params;
		memset(&__params, 0x00, sizeof(__params));
		__params.userId = userId;

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

		__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetUserBookList\"\r\n"));
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

		*GetUserBookListResult = __params.GetUserBookListResult;
		goto __skip_cleanup;

__cleanup:
		Cleanup();
__skip_cleanup:
		ResetClientState(true);
		memset(&__params, 0x00, sizeof(__params));
		return __atlsoap_hr;
	}

	template <typename TClient>
		inline HRESULT CMiniLogT<TClient>::GetUserAlbumPicList(
		int userId, 
		int bookId, 
		BSTR* GetUserAlbumPicListResult
		)
	{
		if ( GetUserAlbumPicListResult == NULL )
			return E_POINTER;

		HRESULT __atlsoap_hr = InitializeSOAP(NULL);
		if (FAILED(__atlsoap_hr))
		{
			SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
			return __atlsoap_hr;
		}

		CleanupClient();

		CComPtr<IStream> __atlsoap_spReadStream;
		__CMiniLog_GetUserAlbumPicList_struct __params;
		memset(&__params, 0x00, sizeof(__params));
		__params.userId = userId;
		__params.bookId = bookId;

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

		__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetUserAlbumPicList\"\r\n"));
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

		*GetUserAlbumPicListResult = __params.GetUserAlbumPicListResult;
		goto __skip_cleanup;

__cleanup:
		Cleanup();
__skip_cleanup:
		ResetClientState(true);
		memset(&__params, 0x00, sizeof(__params));
		return __atlsoap_hr;
	}

	template <typename TClient>
		inline HRESULT CMiniLogT<TClient>::SaveLastArticleUpdateTime(
		int userId, 
		int friendId, 
		BSTR* SaveLastArticleUpdateTimeResult
		)
	{
		if ( SaveLastArticleUpdateTimeResult == NULL )
			return E_POINTER;

		HRESULT __atlsoap_hr = InitializeSOAP(NULL);
		if (FAILED(__atlsoap_hr))
		{
			SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
			return __atlsoap_hr;
		}

		CleanupClient();

		CComPtr<IStream> __atlsoap_spReadStream;
		__CMiniLog_SaveLastArticleUpdateTime_struct __params;
		memset(&__params, 0x00, sizeof(__params));
		__params.userId = userId;
		__params.friendId = friendId;

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

		__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/SaveLastArticleUpdateTime\"\r\n"));
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

		*SaveLastArticleUpdateTimeResult = __params.SaveLastArticleUpdateTimeResult;
		goto __skip_cleanup;

__cleanup:
		Cleanup();
__skip_cleanup:
		ResetClientState(true);
		memset(&__params, 0x00, sizeof(__params));
		return __atlsoap_hr;
	}

	template <typename TClient>
		inline HRESULT CMiniLogT<TClient>::GetArticleInfo(
		int articleId, 
		BSTR* GetArticleInfoResult
		)
	{
		if ( GetArticleInfoResult == NULL )
			return E_POINTER;

		HRESULT __atlsoap_hr = InitializeSOAP(NULL);
		if (FAILED(__atlsoap_hr))
		{
			SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
			return __atlsoap_hr;
		}

		CleanupClient();

		CComPtr<IStream> __atlsoap_spReadStream;
		__CMiniLog_GetArticleInfo_struct __params;
		memset(&__params, 0x00, sizeof(__params));
		__params.articleId = articleId;

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

		__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetArticleInfo\"\r\n"));
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

		*GetArticleInfoResult = __params.GetArticleInfoResult;
		goto __skip_cleanup;

__cleanup:
		Cleanup();
__skip_cleanup:
		ResetClientState(true);
		memset(&__params, 0x00, sizeof(__params));
		return __atlsoap_hr;
	}

	template <typename TClient>
		inline HRESULT CMiniLogT<TClient>::GetUserArticleList(
		BSTR userId, 
		int operUserId, 
		BSTR beginDate, 
		BSTR endDate, 
		BSTR* GetUserArticleListResult
		)
	{
		if ( GetUserArticleListResult == NULL )
			return E_POINTER;

		HRESULT __atlsoap_hr = InitializeSOAP(NULL);
		if (FAILED(__atlsoap_hr))
		{
			SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
			return __atlsoap_hr;
		}

		CleanupClient();

		CComPtr<IStream> __atlsoap_spReadStream;
		__CMiniLog_GetUserArticleList_struct __params;
		memset(&__params, 0x00, sizeof(__params));
		__params.userId = userId;
		__params.operUserId = operUserId;
		__params.beginDate = beginDate;
		__params.endDate = endDate;

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

		__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetUserArticleList\"\r\n"));
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

		*GetUserArticleListResult = __params.GetUserArticleListResult;
		goto __skip_cleanup;

__cleanup:
		Cleanup();
__skip_cleanup:
		ResetClientState(true);
		memset(&__params, 0x00, sizeof(__params));
		return __atlsoap_hr;
	}

	template <typename TClient>
		ATL_NOINLINE inline const _soapmap ** CMiniLogT<TClient>::GetFunctionMap()
	{
		return __CMiniLog_funcs;
	}

	template <typename TClient>
		ATL_NOINLINE inline const _soapmap ** CMiniLogT<TClient>::GetHeaderMap()
	{
		static const _soapmapentry __CMiniLog_GetArticleBookList_atlsoapheader_entries[] =
		{
			{ 0x00000000 }
		};

		static const _soapmap __CMiniLog_GetArticleBookList_atlsoapheader_map = 
		{
			0x9FF0233A,
				"GetArticleBookList",
				L"GetArticleBookListResponse",
				sizeof("GetArticleBookList")-1,
				sizeof("GetArticleBookListResponse")-1,
				SOAPMAP_HEADER,
				__CMiniLog_GetArticleBookList_atlsoapheader_entries,
				0,
				0,
				-1,
				SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				0xC2E575C3,
				"http://tempuri.org/",
				L"http://tempuri.org/",
				sizeof("http://tempuri.org/")-1
		};

		static const _soapmapentry __CMiniLog_GetLastArticleUpdateTime_atlsoapheader_entries[] =
		{
			{ 0x00000000 }
		};

		static const _soapmap __CMiniLog_GetLastArticleUpdateTime_atlsoapheader_map = 
		{
			0x288F4079,
				"GetLastArticleUpdateTime",
				L"GetLastArticleUpdateTimeResponse",
				sizeof("GetLastArticleUpdateTime")-1,
				sizeof("GetLastArticleUpdateTimeResponse")-1,
				SOAPMAP_HEADER,
				__CMiniLog_GetLastArticleUpdateTime_atlsoapheader_entries,
				0,
				0,
				-1,
				SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				0xC2E575C3,
				"http://tempuri.org/",
				L"http://tempuri.org/",
				sizeof("http://tempuri.org/")-1
		};

		static const _soapmapentry __CMiniLog_GetUserBookList_atlsoapheader_entries[] =
		{
			{ 0x00000000 }
		};

		static const _soapmap __CMiniLog_GetUserBookList_atlsoapheader_map = 
		{
			0xD17C15B5,
				"GetUserBookList",
				L"GetUserBookListResponse",
				sizeof("GetUserBookList")-1,
				sizeof("GetUserBookListResponse")-1,
				SOAPMAP_HEADER,
				__CMiniLog_GetUserBookList_atlsoapheader_entries,
				0,
				0,
				-1,
				SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				0xC2E575C3,
				"http://tempuri.org/",
				L"http://tempuri.org/",
				sizeof("http://tempuri.org/")-1
		};

		static const _soapmapentry __CMiniLog_GetUserAlbumPicList_atlsoapheader_entries[] =
		{
			{ 0x00000000 }
		};

		static const _soapmap __CMiniLog_GetUserAlbumPicList_atlsoapheader_map = 
		{
			0xD479DEF7,
				"GetUserAlbumPicList",
				L"GetUserAlbumPicListResponse",
				sizeof("GetUserAlbumPicList")-1,
				sizeof("GetUserAlbumPicListResponse")-1,
				SOAPMAP_HEADER,
				__CMiniLog_GetUserAlbumPicList_atlsoapheader_entries,
				0,
				0,
				-1,
				SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				0xC2E575C3,
				"http://tempuri.org/",
				L"http://tempuri.org/",
				sizeof("http://tempuri.org/")-1
		};

		static const _soapmapentry __CMiniLog_SaveLastArticleUpdateTime_atlsoapheader_entries[] =
		{
			{ 0x00000000 }
		};

		static const _soapmap __CMiniLog_SaveLastArticleUpdateTime_atlsoapheader_map = 
		{
			0xA2598708,
				"SaveLastArticleUpdateTime",
				L"SaveLastArticleUpdateTimeResponse",
				sizeof("SaveLastArticleUpdateTime")-1,
				sizeof("SaveLastArticleUpdateTimeResponse")-1,
				SOAPMAP_HEADER,
				__CMiniLog_SaveLastArticleUpdateTime_atlsoapheader_entries,
				0,
				0,
				-1,
				SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				0xC2E575C3,
				"http://tempuri.org/",
				L"http://tempuri.org/",
				sizeof("http://tempuri.org/")-1
		};

		static const _soapmapentry __CMiniLog_GetArticleInfo_atlsoapheader_entries[] =
		{
			{ 0x00000000 }
		};

		static const _soapmap __CMiniLog_GetArticleInfo_atlsoapheader_map = 
		{
			0xB9565F3F,
				"GetArticleInfo",
				L"GetArticleInfoResponse",
				sizeof("GetArticleInfo")-1,
				sizeof("GetArticleInfoResponse")-1,
				SOAPMAP_HEADER,
				__CMiniLog_GetArticleInfo_atlsoapheader_entries,
				0,
				0,
				-1,
				SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				0xC2E575C3,
				"http://tempuri.org/",
				L"http://tempuri.org/",
				sizeof("http://tempuri.org/")-1
		};

		static const _soapmapentry __CMiniLog_GetUserArticleList_atlsoapheader_entries[] =
		{
			{ 0x00000000 }
		};

		static const _soapmap __CMiniLog_GetUserArticleList_atlsoapheader_map = 
		{
			0x7D76F94E,
				"GetUserArticleList",
				L"GetUserArticleListResponse",
				sizeof("GetUserArticleList")-1,
				sizeof("GetUserArticleListResponse")-1,
				SOAPMAP_HEADER,
				__CMiniLog_GetUserArticleList_atlsoapheader_entries,
				0,
				0,
				-1,
				SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				0xC2E575C3,
				"http://tempuri.org/",
				L"http://tempuri.org/",
				sizeof("http://tempuri.org/")-1
		};


		static const _soapmap * __CMiniLog_headers[] =
		{
			&__CMiniLog_GetArticleBookList_atlsoapheader_map,
				&__CMiniLog_GetLastArticleUpdateTime_atlsoapheader_map,
				&__CMiniLog_GetUserBookList_atlsoapheader_map,
				&__CMiniLog_GetUserAlbumPicList_atlsoapheader_map,
				&__CMiniLog_SaveLastArticleUpdateTime_atlsoapheader_map,
				&__CMiniLog_GetArticleInfo_atlsoapheader_map,
				&__CMiniLog_GetUserArticleList_atlsoapheader_map,
				NULL
		};

		return __CMiniLog_headers;
	}

	template <typename TClient>
		ATL_NOINLINE inline void * CMiniLogT<TClient>::GetHeaderValue()
	{
		return this;
	}

	template <typename TClient>
		ATL_NOINLINE inline const wchar_t * CMiniLogT<TClient>::GetNamespaceUri()
	{
		return L"http://tempuri.org/";
	}

	template <typename TClient>
		ATL_NOINLINE inline const char * CMiniLogT<TClient>::GetServiceName()
	{
		return NULL;
	}

	template <typename TClient>
		ATL_NOINLINE inline const char * CMiniLogT<TClient>::GetNamespaceUriA()
	{
		return "http://tempuri.org/";
	}

	template <typename TClient>
		ATL_NOINLINE inline HRESULT CMiniLogT<TClient>::CallFunction(
		void *, 
		const wchar_t *, int,
		size_t)
	{
		return E_NOTIMPL;
	}

	template <typename TClient>
		ATL_NOINLINE inline HRESULT CMiniLogT<TClient>::GetClientReader(ISAXXMLReader **ppReader)
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

} // namespace MiniLog
