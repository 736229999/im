//
// sproxy.exe generated file
// do not modify this file
//
// Created: 02/06/2007@11:09:14
//

#pragma once


#ifndef _WIN32_WINDOWS
#pragma message("warning: defining _WIN32_WINDOWS = 0x0410")
#define _WIN32_WINDOWS 0x0410
#endif

#include <atlsoap.h>

namespace MiniAlbum
{

	template <typename TClient = CSoapSocketClientT<> >
	class CMiniAlbumT : 
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

		//CMiniAlbumT(ISAXXMLReader *pReader = NULL)
		//	:TClient(_T("http://wsminiv15.139.com/MiniAlbum.asmx"))
		//{
		//	SetClient(true);
		//	SetReader(pReader);
		//}

		CMiniAlbumT(CString s,ISAXXMLReader *pReader = NULL)
			:TClient(s)
		{
			SetClient(true);
			SetReader(pReader);
		}

		~CMiniAlbumT()
		{
			Uninitialize();
		}

		void Uninitialize()
		{
			UninitializeSOAP();
		}	

		HRESULT SaveLastPictureUpdateTime(
			int userId, 
			int friendId, 
			BSTR* SaveLastPictureUpdateTimeResult
			);

		HRESULT GetUserAlbumPicList(
			int userId, 
			int albumId, 
			BSTR* GetUserAlbumPicListResult
			);

		HRESULT GetUserPictureList(
			BSTR userId, 
			int operUserId, 
			BSTR beginDate, 
			BSTR endDate, 
			BSTR* GetUserPictureListResult
			);

		HRESULT GetUserAlbumList(
			BSTR userId, 
			BSTR* GetUserAlbumListResult
			);

		HRESULT GetLastPictureUpdateTime(
			int userId, 
			int friendId, 
			BSTR* GetLastPictureUpdateTimeResult
			);
	};

	typedef CMiniAlbumT<> CMiniAlbum;

	struct __CMiniAlbum_SaveLastPictureUpdateTime_struct
	{
		int userId;
		int friendId;
		BSTR SaveLastPictureUpdateTimeResult;
	};

	extern __declspec(selectany) const _soapmapentry __CMiniAlbum_SaveLastPictureUpdateTime_entries[] =
	{

		{
			0x1945E48C, 
				"userId", 
				L"userId", 
				sizeof("userId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniAlbum_SaveLastPictureUpdateTime_struct, userId),
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
				offsetof(__CMiniAlbum_SaveLastPictureUpdateTime_struct, friendId),
				NULL,
				NULL,
				-1,
		},
			{
				0x19C15F50, 
					"SaveLastPictureUpdateTimeResult", 
					L"SaveLastPictureUpdateTimeResult", 
					sizeof("SaveLastPictureUpdateTimeResult")-1, 
					SOAPTYPE_STRING, 
					SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
					offsetof(__CMiniAlbum_SaveLastPictureUpdateTime_struct, SaveLastPictureUpdateTimeResult),
					NULL,
					NULL,
					-1,
			},
			{ 0x00000000 }
	};

	extern __declspec(selectany) const _soapmap __CMiniAlbum_SaveLastPictureUpdateTime_map =
	{
		0x8F3D93A0,
			"SaveLastPictureUpdateTime",
			L"SaveLastPictureUpdateTimeResponse",
			sizeof("SaveLastPictureUpdateTime")-1,
			sizeof("SaveLastPictureUpdateTimeResponse")-1,
			SOAPMAP_FUNC,
			__CMiniAlbum_SaveLastPictureUpdateTime_entries,
			sizeof(__CMiniAlbum_SaveLastPictureUpdateTime_struct),
			1,
			-1,
			SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
			0xC2E575C3,
			"http://tempuri.org/",
			L"http://tempuri.org/",
			sizeof("http://tempuri.org/")-1
	};


	struct __CMiniAlbum_GetUserAlbumPicList_struct
	{
		int userId;
		int albumId;
		BSTR GetUserAlbumPicListResult;
	};

	extern __declspec(selectany) const _soapmapentry __CMiniAlbum_GetUserAlbumPicList_entries[] =
	{

		{
			0x1945E48C, 
				"userId", 
				L"userId", 
				sizeof("userId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniAlbum_GetUserAlbumPicList_struct, userId),
				NULL,
				NULL,
				-1,
		},
		{
			0x2DED8C3E, 
				"albumId", 
				L"albumId", 
				sizeof("albumId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniAlbum_GetUserAlbumPicList_struct, albumId),
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
					offsetof(__CMiniAlbum_GetUserAlbumPicList_struct, GetUserAlbumPicListResult),
					NULL,
					NULL,
					-1,
			},
			{ 0x00000000 }
	};

	extern __declspec(selectany) const _soapmap __CMiniAlbum_GetUserAlbumPicList_map =
	{
		0xD479DEF7,
			"GetUserAlbumPicList",
			L"GetUserAlbumPicListResponse",
			sizeof("GetUserAlbumPicList")-1,
			sizeof("GetUserAlbumPicListResponse")-1,
			SOAPMAP_FUNC,
			__CMiniAlbum_GetUserAlbumPicList_entries,
			sizeof(__CMiniAlbum_GetUserAlbumPicList_struct),
			1,
			-1,
			SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
			0xC2E575C3,
			"http://tempuri.org/",
			L"http://tempuri.org/",
			sizeof("http://tempuri.org/")-1
	};


	struct __CMiniAlbum_GetUserPictureList_struct
	{
		BSTR userId;
		int operUserId;
		BSTR beginDate;
		BSTR endDate;
		BSTR GetUserPictureListResult;
	};

	extern __declspec(selectany) const _soapmapentry __CMiniAlbum_GetUserPictureList_entries[] =
	{

		{
			0x1945E48C, 
				"userId", 
				L"userId", 
				sizeof("userId")-1, 
				SOAPTYPE_STRING, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
				offsetof(__CMiniAlbum_GetUserPictureList_struct, userId),
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
				offsetof(__CMiniAlbum_GetUserPictureList_struct, operUserId),
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
					offsetof(__CMiniAlbum_GetUserPictureList_struct, beginDate),
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
					offsetof(__CMiniAlbum_GetUserPictureList_struct, endDate),
					NULL,
					NULL,
					-1,
			},
				{
					0x18B67616, 
						"GetUserPictureListResult", 
						L"GetUserPictureListResult", 
						sizeof("GetUserPictureListResult")-1, 
						SOAPTYPE_STRING, 
						SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
						offsetof(__CMiniAlbum_GetUserPictureList_struct, GetUserPictureListResult),
						NULL,
						NULL,
						-1,
				},
				{ 0x00000000 }
	};

	extern __declspec(selectany) const _soapmap __CMiniAlbum_GetUserPictureList_map =
	{
		0x1FD373E6,
			"GetUserPictureList",
			L"GetUserPictureListResponse",
			sizeof("GetUserPictureList")-1,
			sizeof("GetUserPictureListResponse")-1,
			SOAPMAP_FUNC,
			__CMiniAlbum_GetUserPictureList_entries,
			sizeof(__CMiniAlbum_GetUserPictureList_struct),
			1,
			-1,
			SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
			0xC2E575C3,
			"http://tempuri.org/",
			L"http://tempuri.org/",
			sizeof("http://tempuri.org/")-1
	};


	struct __CMiniAlbum_GetUserAlbumList_struct
	{
		BSTR userId;
		BSTR GetUserAlbumListResult;
	};

	extern __declspec(selectany) const _soapmapentry __CMiniAlbum_GetUserAlbumList_entries[] =
	{

		{
			0x1945E48C, 
				"userId", 
				L"userId", 
				sizeof("userId")-1, 
				SOAPTYPE_STRING, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
				offsetof(__CMiniAlbum_GetUserAlbumList_struct, userId),
				NULL,
				NULL,
				-1,
		},
		{
			0x17DDDDAB, 
				"GetUserAlbumListResult", 
				L"GetUserAlbumListResult", 
				sizeof("GetUserAlbumListResult")-1, 
				SOAPTYPE_STRING, 
				SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
				offsetof(__CMiniAlbum_GetUserAlbumList_struct, GetUserAlbumListResult),
				NULL,
				NULL,
				-1,
		},
			{ 0x00000000 }
	};

	extern __declspec(selectany) const _soapmap __CMiniAlbum_GetUserAlbumList_map =
	{
		0x867314BB,
			"GetUserAlbumList",
			L"GetUserAlbumListResponse",
			sizeof("GetUserAlbumList")-1,
			sizeof("GetUserAlbumListResponse")-1,
			SOAPMAP_FUNC,
			__CMiniAlbum_GetUserAlbumList_entries,
			sizeof(__CMiniAlbum_GetUserAlbumList_struct),
			1,
			-1,
			SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
			0xC2E575C3,
			"http://tempuri.org/",
			L"http://tempuri.org/",
			sizeof("http://tempuri.org/")-1
	};


	struct __CMiniAlbum_GetLastPictureUpdateTime_struct
	{
		int userId;
		int friendId;
		BSTR GetLastPictureUpdateTimeResult;
	};

	extern __declspec(selectany) const _soapmapentry __CMiniAlbum_GetLastPictureUpdateTime_entries[] =
	{

		{
			0x1945E48C, 
				"userId", 
				L"userId", 
				sizeof("userId")-1, 
				SOAPTYPE_INT, 
				SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				offsetof(__CMiniAlbum_GetLastPictureUpdateTime_struct, userId),
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
				offsetof(__CMiniAlbum_GetLastPictureUpdateTime_struct, friendId),
				NULL,
				NULL,
				-1,
		},
			{
				0x8BA80881, 
					"GetLastPictureUpdateTimeResult", 
					L"GetLastPictureUpdateTimeResult", 
					sizeof("GetLastPictureUpdateTimeResult")-1, 
					SOAPTYPE_STRING, 
					SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
					offsetof(__CMiniAlbum_GetLastPictureUpdateTime_struct, GetLastPictureUpdateTimeResult),
					NULL,
					NULL,
					-1,
			},
			{ 0x00000000 }
	};

	extern __declspec(selectany) const _soapmap __CMiniAlbum_GetLastPictureUpdateTime_map =
	{
		0x15734D11,
			"GetLastPictureUpdateTime",
			L"GetLastPictureUpdateTimeResponse",
			sizeof("GetLastPictureUpdateTime")-1,
			sizeof("GetLastPictureUpdateTimeResponse")-1,
			SOAPMAP_FUNC,
			__CMiniAlbum_GetLastPictureUpdateTime_entries,
			sizeof(__CMiniAlbum_GetLastPictureUpdateTime_struct),
			1,
			-1,
			SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
			0xC2E575C3,
			"http://tempuri.org/",
			L"http://tempuri.org/",
			sizeof("http://tempuri.org/")-1
	};

	extern __declspec(selectany) const _soapmap * __CMiniAlbum_funcs[] =
	{
		&__CMiniAlbum_SaveLastPictureUpdateTime_map,
			&__CMiniAlbum_GetUserAlbumPicList_map,
			&__CMiniAlbum_GetUserPictureList_map,
			&__CMiniAlbum_GetUserAlbumList_map,
			&__CMiniAlbum_GetLastPictureUpdateTime_map,
			NULL
	};

	template <typename TClient>
		inline HRESULT CMiniAlbumT<TClient>::SaveLastPictureUpdateTime(
		int userId, 
		int friendId, 
		BSTR* SaveLastPictureUpdateTimeResult
		)
	{
		if ( SaveLastPictureUpdateTimeResult == NULL )
			return E_POINTER;

		HRESULT __atlsoap_hr = InitializeSOAP(NULL);
		if (FAILED(__atlsoap_hr))
		{
			SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
			return __atlsoap_hr;
		}

		CleanupClient();

		CComPtr<IStream> __atlsoap_spReadStream;
		__CMiniAlbum_SaveLastPictureUpdateTime_struct __params;
		memset(&__params, 0x00, sizeof(__params));
		__params.userId = userId;
		__params.friendId = friendId;

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

		__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/SaveLastPictureUpdateTime\"\r\n"));
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

		*SaveLastPictureUpdateTimeResult = __params.SaveLastPictureUpdateTimeResult;
		goto __skip_cleanup;

__cleanup:
		Cleanup();
__skip_cleanup:
		ResetClientState(true);
		memset(&__params, 0x00, sizeof(__params));
		return __atlsoap_hr;
	}

	template <typename TClient>
		inline HRESULT CMiniAlbumT<TClient>::GetUserAlbumPicList(
		int userId, 
		int albumId, 
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
		__CMiniAlbum_GetUserAlbumPicList_struct __params;
		memset(&__params, 0x00, sizeof(__params));
		__params.userId = userId;
		__params.albumId = albumId;

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
		inline HRESULT CMiniAlbumT<TClient>::GetUserPictureList(
		BSTR userId, 
		int operUserId, 
		BSTR beginDate, 
		BSTR endDate, 
		BSTR* GetUserPictureListResult
		)
	{
		if ( GetUserPictureListResult == NULL )
			return E_POINTER;

		HRESULT __atlsoap_hr = InitializeSOAP(NULL);
		if (FAILED(__atlsoap_hr))
		{
			SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
			return __atlsoap_hr;
		}

		CleanupClient();

		CComPtr<IStream> __atlsoap_spReadStream;
		__CMiniAlbum_GetUserPictureList_struct __params;
		memset(&__params, 0x00, sizeof(__params));
		__params.userId = userId;
		__params.operUserId = operUserId;
		__params.beginDate = beginDate;
		__params.endDate = endDate;

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

		__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetUserPictureList\"\r\n"));
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

		*GetUserPictureListResult = __params.GetUserPictureListResult;
		goto __skip_cleanup;

__cleanup:
		Cleanup();
__skip_cleanup:
		ResetClientState(true);
		memset(&__params, 0x00, sizeof(__params));
		return __atlsoap_hr;
	}

	template <typename TClient>
		inline HRESULT CMiniAlbumT<TClient>::GetUserAlbumList(
		BSTR userId, 
		BSTR* GetUserAlbumListResult
		)
	{
		if ( GetUserAlbumListResult == NULL )
			return E_POINTER;

		HRESULT __atlsoap_hr = InitializeSOAP(NULL);
		if (FAILED(__atlsoap_hr))
		{
			SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
			return __atlsoap_hr;
		}

		CleanupClient();

		CComPtr<IStream> __atlsoap_spReadStream;
		__CMiniAlbum_GetUserAlbumList_struct __params;
		memset(&__params, 0x00, sizeof(__params));
		__params.userId = userId;

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

		__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetUserAlbumList\"\r\n"));
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

		*GetUserAlbumListResult = __params.GetUserAlbumListResult;
		goto __skip_cleanup;

__cleanup:
		Cleanup();
__skip_cleanup:
		ResetClientState(true);
		memset(&__params, 0x00, sizeof(__params));
		return __atlsoap_hr;
	}

	template <typename TClient>
		inline HRESULT CMiniAlbumT<TClient>::GetLastPictureUpdateTime(
		int userId, 
		int friendId, 
		BSTR* GetLastPictureUpdateTimeResult
		)
	{
		if ( GetLastPictureUpdateTimeResult == NULL )
			return E_POINTER;

		HRESULT __atlsoap_hr = InitializeSOAP(NULL);
		if (FAILED(__atlsoap_hr))
		{
			SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
			return __atlsoap_hr;
		}

		CleanupClient();

		CComPtr<IStream> __atlsoap_spReadStream;
		__CMiniAlbum_GetLastPictureUpdateTime_struct __params;
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

		__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://tempuri.org/GetLastPictureUpdateTime\"\r\n"));
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

		*GetLastPictureUpdateTimeResult = __params.GetLastPictureUpdateTimeResult;
		goto __skip_cleanup;

__cleanup:
		Cleanup();
__skip_cleanup:
		ResetClientState(true);
		memset(&__params, 0x00, sizeof(__params));
		return __atlsoap_hr;
	}

	template <typename TClient>
		ATL_NOINLINE inline const _soapmap ** CMiniAlbumT<TClient>::GetFunctionMap()
	{
		return __CMiniAlbum_funcs;
	}

	template <typename TClient>
		ATL_NOINLINE inline const _soapmap ** CMiniAlbumT<TClient>::GetHeaderMap()
	{
		static const _soapmapentry __CMiniAlbum_SaveLastPictureUpdateTime_atlsoapheader_entries[] =
		{
			{ 0x00000000 }
		};

		static const _soapmap __CMiniAlbum_SaveLastPictureUpdateTime_atlsoapheader_map = 
		{
			0x8F3D93A0,
				"SaveLastPictureUpdateTime",
				L"SaveLastPictureUpdateTimeResponse",
				sizeof("SaveLastPictureUpdateTime")-1,
				sizeof("SaveLastPictureUpdateTimeResponse")-1,
				SOAPMAP_HEADER,
				__CMiniAlbum_SaveLastPictureUpdateTime_atlsoapheader_entries,
				0,
				0,
				-1,
				SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				0xC2E575C3,
				"http://tempuri.org/",
				L"http://tempuri.org/",
				sizeof("http://tempuri.org/")-1
		};

		static const _soapmapentry __CMiniAlbum_GetUserAlbumPicList_atlsoapheader_entries[] =
		{
			{ 0x00000000 }
		};

		static const _soapmap __CMiniAlbum_GetUserAlbumPicList_atlsoapheader_map = 
		{
			0xD479DEF7,
				"GetUserAlbumPicList",
				L"GetUserAlbumPicListResponse",
				sizeof("GetUserAlbumPicList")-1,
				sizeof("GetUserAlbumPicListResponse")-1,
				SOAPMAP_HEADER,
				__CMiniAlbum_GetUserAlbumPicList_atlsoapheader_entries,
				0,
				0,
				-1,
				SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				0xC2E575C3,
				"http://tempuri.org/",
				L"http://tempuri.org/",
				sizeof("http://tempuri.org/")-1
		};

		static const _soapmapentry __CMiniAlbum_GetUserPictureList_atlsoapheader_entries[] =
		{
			{ 0x00000000 }
		};

		static const _soapmap __CMiniAlbum_GetUserPictureList_atlsoapheader_map = 
		{
			0x1FD373E6,
				"GetUserPictureList",
				L"GetUserPictureListResponse",
				sizeof("GetUserPictureList")-1,
				sizeof("GetUserPictureListResponse")-1,
				SOAPMAP_HEADER,
				__CMiniAlbum_GetUserPictureList_atlsoapheader_entries,
				0,
				0,
				-1,
				SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				0xC2E575C3,
				"http://tempuri.org/",
				L"http://tempuri.org/",
				sizeof("http://tempuri.org/")-1
		};

		static const _soapmapentry __CMiniAlbum_GetUserAlbumList_atlsoapheader_entries[] =
		{
			{ 0x00000000 }
		};

		static const _soapmap __CMiniAlbum_GetUserAlbumList_atlsoapheader_map = 
		{
			0x867314BB,
				"GetUserAlbumList",
				L"GetUserAlbumListResponse",
				sizeof("GetUserAlbumList")-1,
				sizeof("GetUserAlbumListResponse")-1,
				SOAPMAP_HEADER,
				__CMiniAlbum_GetUserAlbumList_atlsoapheader_entries,
				0,
				0,
				-1,
				SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				0xC2E575C3,
				"http://tempuri.org/",
				L"http://tempuri.org/",
				sizeof("http://tempuri.org/")-1
		};

		static const _soapmapentry __CMiniAlbum_GetLastPictureUpdateTime_atlsoapheader_entries[] =
		{
			{ 0x00000000 }
		};

		static const _soapmap __CMiniAlbum_GetLastPictureUpdateTime_atlsoapheader_map = 
		{
			0x15734D11,
				"GetLastPictureUpdateTime",
				L"GetLastPictureUpdateTimeResponse",
				sizeof("GetLastPictureUpdateTime")-1,
				sizeof("GetLastPictureUpdateTimeResponse")-1,
				SOAPMAP_HEADER,
				__CMiniAlbum_GetLastPictureUpdateTime_atlsoapheader_entries,
				0,
				0,
				-1,
				SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
				0xC2E575C3,
				"http://tempuri.org/",
				L"http://tempuri.org/",
				sizeof("http://tempuri.org/")-1
		};


		static const _soapmap * __CMiniAlbum_headers[] =
		{
			&__CMiniAlbum_SaveLastPictureUpdateTime_atlsoapheader_map,
				&__CMiniAlbum_GetUserAlbumPicList_atlsoapheader_map,
				&__CMiniAlbum_GetUserPictureList_atlsoapheader_map,
				&__CMiniAlbum_GetUserAlbumList_atlsoapheader_map,
				&__CMiniAlbum_GetLastPictureUpdateTime_atlsoapheader_map,
				NULL
		};

		return __CMiniAlbum_headers;
	}

	template <typename TClient>
		ATL_NOINLINE inline void * CMiniAlbumT<TClient>::GetHeaderValue()
	{
		return this;
	}

	template <typename TClient>
		ATL_NOINLINE inline const wchar_t * CMiniAlbumT<TClient>::GetNamespaceUri()
	{
		return L"http://tempuri.org/";
	}

	template <typename TClient>
		ATL_NOINLINE inline const char * CMiniAlbumT<TClient>::GetServiceName()
	{
		return NULL;
	}

	template <typename TClient>
		ATL_NOINLINE inline const char * CMiniAlbumT<TClient>::GetNamespaceUriA()
	{
		return "http://tempuri.org/";
	}

	template <typename TClient>
		ATL_NOINLINE inline HRESULT CMiniAlbumT<TClient>::CallFunction(
		void *, 
		const wchar_t *, int,
		size_t)
	{
		return E_NOTIMPL;
	}

	template <typename TClient>
		ATL_NOINLINE inline HRESULT CMiniAlbumT<TClient>::GetClientReader(ISAXXMLReader **ppReader)
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

} // namespace MiniAlbum
