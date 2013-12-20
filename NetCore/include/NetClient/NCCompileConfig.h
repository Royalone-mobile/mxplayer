////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par ģ����
/// ���������ļ�
/// @par ����ļ�
/// NetClient.h <����ļ���ص������ļ�>
/// @par ������ϸ����
/// <�����Ǳ����ܵ���ϸ���������ݳ���120������ַ���Ҫ�ӷ�б��N���н���һ������>
/// @par ���̰߳�ȫ��
/// <��/��>[��˵��]
/// @par �쳣ʱ��ȫ��
/// <��/��>[��˵��]
/// @note         - 
/// 
/// @file         NCCompileConfig.h
/// @brief        <ģ�鹦�ܼ���>
/// @author       Li.xl
/// @version      1.0
/// @date         2011/06/09
/// @todo         <����Ҫ��������>
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __LEE_NETCLIENT_COMPILE_CONFIG_H__INCLUDE__
#define __LEE_NETCLIENT_COMPILE_CONFIG_H__INCLUDE__

/// GUIScript SDK Version
#define LOTTERYNETCLIENT_VERSION_MAJOR 1
#define LOTTERYNETCLIENT_VERSION_MINOR 0
#define LOTTERYNETCLIENT_VERSION_REVISION 1

/// ����SVN�������ϵİ汾��Ϣ
#define LOTTERYNETCLIENT_VERSION_SVN  10011
/// ����SDK�汾��
#define LOTTERYNETCLIENT_SDK_VERSION "1.0.1"


/// WIN32 for Windowsint32
/// WIN64 for Windowsint64
/// ����WINDOWSƽ̨
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#define _NC_WINDOWS_PLATFORM_
#define _NC_WINDOWS_API_
#endif

/// WINCE is a very restricted environment for mobile devices
/// ����WINDOWS CEƽ̨
#if defined(_WIN32_WCE)
#define _NC_WINDOWS_PLATFORM_
#define _NC_WINDOWS_API_
#define _NC_WINDOWS_CE_PLATFORM_
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1300)
#  error "Only Microsoft Visual Studio 7.0 and later are supported."
#endif

/// ����LINUXϵͳƽ̨
#if !defined(_NC_WINDOWS_PLATFORM_)
#define _NC_LINUX_PLATFORM_
#define _NC_POSIX_API_
#endif

/// WINDOWSƽ̨�£���غ궨��
#ifdef _NC_WINDOWS_API_
/// ���嶯̬������
#ifndef _NC_STATIC_LIB_
#ifdef	LOTTERYNETCLIENT_EXPORTS
#define LOTTERYNETCLIENT_API __declspec(dllexport)
#else
#define LOTTERYNETCLIENT_API __declspec(dllimport)
#endif /// LOTTERYNETCLIENT_API_EXPORTS
#else/// ��̬�����궨��
#define LOTTERYNETCLIENT_API
#endif /// _NC_STATIC_LIB_

/// Declare the calling convention.
#if defined(_STDCALL_SUPPORTED)
#define NCCALLCONV __stdcall
#else
#define NCCALLCONV __cdecl
#endif /// STDCALL_SUPPORTED

/// ����WIN32_LEAN_AND_MEAN�꣬���ε�MFC��winsock.h�ȣ�
/// �ӿ���룬�Ż�Ӧ�ó���
#define WIN32_LEAN_AND_MEAN

#else /// _NC_WINDOWS_API_
/// ��WINDOWSƽ̨�µĶ�̬�⡢��̬������
/// Force symbol export in shared libraries built with gcc.
#if (__GNUC__ >= 4) && !defined(_NC_STATIC_LIB_) \
	&& defined(LOTTERYNETCLIENT_EXPORTS)
#define LOTTERYNETCLIENT_API __attribute__ ((visibility("default")))
#else
#define LOTTERYNETCLIENT_API
#endif

#define NCCALLCONV

#endif /// _NC_WINDOWS_API_

#if defined(__sparc__) || defined(__sun__)
#define __BIG_ENDIAN__
#endif

#endif /// __LEE_NETCLIENT_COMPILE_CONFIG_H__INCLUDE__

