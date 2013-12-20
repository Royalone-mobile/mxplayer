////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// DEBUG��Ϣ���ģ��
/// @par ����ļ�
/// Debug.cpp, �������ñ�ͷ�ļ����ļ�
/// @par ������ϸ����
/// ʵ�����������DEBUG��Ϣ������������ʼ���������ش�����ʱ����Ϣ��
/// @par ���̰߳�ȫ��
/// ��
/// @par �쳣ʱ��ȫ��
/// ��
/// @par ��ע
/// Include��ͷ�ļ�����
/// 
/// @file Debug.h
/// @brief ������Ϣ���
/// @author Lee@Home
/// @version 1.0
/// @date 2010.12.20
/// @todo 
/// 
////////////////////////////////////////////////////////////////////////////////

#ifndef __LIGHT_DEBUG_INFORMATION__
#define __LIGHT_DEBUG_INFORMATION__

// ������ʼ�궨��
#define _DEBUG_START		0
// ���������궨��
#define _DEBUG_END			1
// һ�������Ϣ�궨��
#define LD_INFO				2
// ������Ϣ�궨��
#define LD_WARN				3
// ������Ϣ�궨��
#define LD_ERR				4

#define LD_LINE				__LINE__
#define LD_FILE				__FILE__
#define LD_FUNC				(char*)__FUNCTION__

#ifdef WIN32
	#define DLL_EXPORT_API __declspec(dllexport)
#else
	#define DLL_EXPORT_API 
#endif


#ifdef _LDEBUG
		// ������ʼ��DEBUG��Ϣ������кš��ļ�����������
		#define START_DEBUG_INFO		P_PrintLog(_DEBUG_START, LD_LINE, \
											LD_FUNC, LD_FILE, "%s\n", "������ʼ");
		
		// ����������DEBUG��Ϣ������кš��ļ�����������
		#define  END_DEBUG_INFO			P_PrintLog(_DEBUG_END, LD_LINE, \
											LD_FUNC, LD_FILE, "%s\n", "��������");
		
		/// һ������Ϣ���
		#define D_INFO(fmt,...)		P_PrintLog(LD_INFO, LD_LINE, LD_FUNC, LD_FILE, \
											fmt, __VA_ARGS__);
		
		/// ������Ϣ���
		#define D_WARN(fmt,...)		P_PrintLog(LD_WARN, LD_LINE, LD_FUNC, LD_FILE, \
											fmt, __VA_ARGS__);
		/// ������Ϣ���
		#define D_ERROR(fmt,...)	P_PrintLog(LD_ERR, LD_LINE, LD_FUNC, LD_FILE, \
											fmt, __VA_ARGS__);
#endif

#ifdef _LDEBUG_W
		#define START_DEBUG_INFO		
		#define END_DEBUG_INFO			
		#define D_INFO(fmt,...)			
		/// ������Ϣ���
		#define D_WARN(fmt,...)		P_PrintLog(LD_WARN, LD_LINE, LD_FUNC, LD_FILE, \
											fmt, __VA_ARGS__);
		/// ������Ϣ���
		#define D_ERROR(fmt,...)	P_PrintLog(LD_ERR, LD_LINE, LD_FUNC, LD_FILE, \
											fmt, __VA_ARGS__);
#endif

#ifdef _LDEBUG_E
		#define START_DEBUG_INFO		
		#define   END_DEBUG_INFO		
		#define D_INFO(fmt,...)		
		#define D_WARN(fmt,...)		
		/// ������Ϣ���
		#define D_ERROR(fmt,...)	P_PrintLog(LD_ERR, LD_LINE, LD_FUNC, LD_FILE, \
											fmt, __VA_ARGS__);
#endif


/// Windowsƽ̨
#ifdef WIN32
#if !defined(_LDEBUG) && !defined(_LDEBUG_W) && !defined(_LDEBUG_E)
		#define START_DEBUG_INFO		
		#define   END_DEBUG_INFO		
		#define D_INFO(fmt,...)		
		/// δ����_LDEBUG_E��_LDEBUG_Wʱ�����������Ϣ
		#define D_WARN(fmt,...)		
		/// δ����_LDEBUG_Eʱ�����������Ϣ
		#define D_ERROR(fmt,...)	
#endif

/// Linuxƽ̨
#else
#ifndef _LDEBUG && _LDEBUG_W && _LDEBUG_E
		#define START_DEBUG_INFO		
		#define   END_DEBUG_INFO		
		#define D_INFO(fmt,...)		
		/// δ����_LDEBUG_E��_LDEBUG_Wʱ�����������Ϣ
		#define D_WARN(fmt,...)		
		/// δ����_LDEBUG_Eʱ�����������Ϣ
		#define D_ERROR(fmt,...)	
#endif
#endif// end of WIN32


/// ����ƽ̨�²�ͬAPI�����Ķ���
#if defined WIN32
	#if defined(_MSC_VER) && _MSC_VER > 1310 && !defined (_WIN32_WCE)
		#define swprintf        swprintf_s
		#define snprintf        sprintf_s
		#define strncasecmp     _memicmp
	#else
		#define swprintf        _snwprintf
		#define snprintf        _snprintf
	#endif
#endif

/// 
/// @par ����
/// �����Ϣ��ʵ�庯��
/// @param[in]  nFlag          Flag��־(0:��ʼ��1:������2:�ش����)
/// @param[in]  nLineNo        �к�
/// @param[in]  szFunctionName ������
/// @param[in]  szFileName     �ļ���
/// @param[in]  szFormat       �����Ϣ�ĸ�ʽ
/// @param[in]  ...            �ɱ����
/// @return ��
/// @note -
/// @par ʾ����
/// @code
/// @endcode
/// @see -
/// @deprecated -
/// 
DLL_EXPORT_API 
void P_PrintLog(int nFlag, int nLineNo, char* szFunctionName, 
				char* szFileName, char *szFormat, ...);

/// 
/// @par ����
/// ���ļ�
/// @param[in]  szFileName     �ļ���
/// @return �򿪵��ļ�ָ��
/// @note -
/// @par ʾ����
/// @code
/// @endcode
/// @see -
/// @deprecated -
/// 
FILE* OpenFile(char* szFileName);


#endif// end of __LIGHT_DEBUG_INFORMATION__