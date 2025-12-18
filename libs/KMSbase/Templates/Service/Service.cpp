
// Author/Auteur	:	KMS -	Martin Dubois, ing.
// Client			:	TODO
// Project/Projet	:	TODO
// File/Fichier		:	TODO/Service.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>

// ===== Windows ============================================================
#include <Windows.h>

// ===== KmsBase ============================================================
#include <KmsLib/Windows/SystemLog.h>

// ===== Local ==============================================================
#include "Messages.h"

// Static function declaration / Declaration des fonctions statiques
/////////////////////////////////////////////////////////////////////////////

static void ReportError		(unsigned int aLine, const char * aFunction, const char * aMsg, DWORD aCode);
static void ReportInfo		(DWORD aMsgId);
static void ReportStatus	(DWORD aCurrentState);

// ===== Event handler / Gestionnaires d'evennements ========================
static void Shutdown	();
static void Start		();
static void Stop		();

// ===== Entry points / points d'entres =====================================
static DWORD	WINAPI ServiceCtrlHandler	(DWORD aControl, DWORD aEventType, LPVOID aEventData, LPVOID aContext);
static VOID		WINAPI ServiceMain			(DWORD aCount, LPTSTR * aVector);

// Constants / Constantes
/////////////////////////////////////////////////////////////////////////////

static const SERVICE_TABLE_ENTRY	SERVICES[]
{
	// TODO : Change the service name / Changer le nom du service
	{ "TODO", ServiceMain },

	{ NULL, NULL }
};

// Variables
/////////////////////////////////////////////////////////////////////////////

static unsigned int					sCheckPoint		= 1;
static SERVICE_STATUS_HANDLE		sStatusHandle	= 0;
static KmsLib::Windows::SystemLog	sSystemLog("System", SERVICES[0].lpServiceName);

// Entry point / Point d'entree
/////////////////////////////////////////////////////////////////////////////

int __stdcall WinMain(HINSTANCE aInstance, HINSTANCE, LPSTR, int)
{
	assert(NULL != aInstance);

	try
	{
		BOOL lRetB = StartServiceCtrlDispatcher(SERVICES);
		if (!lRetB)
		{
			ReportError(__LINE__, __FUNCTION__, "StartServiceCtrlDispatcher(  ) failed", lRetB);
		}
	}
	catch ( ... )
	{
		ReportError(__LINE__, __FUNCTION__, "Unknown exception", 0);
	}
}

// Static functions / Fonction statiques
/////////////////////////////////////////////////////////////////////////////

// aLine		:
// aFunction	: [in]
// aMsg			: [in]
// aCode		:
void ReportError(unsigned int aLine, const char * aFunction, const char * aMsg, DWORD aCode)
{
	assert(0	<	aLine		);
	assert(NULL !=	aFunction	);
	assert(NULL !=	aMsg		);

	const char * lStrings[2];

	lStrings[0] = aFunction	;
	lStrings[1] = aMsg		;

	unsigned int lData[3];

	lData[0] = aLine;
	lData[1] = aCode;
	lData[2] = GetLastError();

	sSystemLog.LogEvent(EVENTLOG_ERROR_TYPE, 0, MSG_ERROR, 2, sizeof(lData), lStrings, lData);
}

// aMsgId	:	See / Void Messages.h
void ReportInfo(DWORD aMsgId)
{
	sSystemLog.LogEvent(EVENTLOG_INFORMATION_TYPE, 0, aMsgId, 0, 0, NULL, NULL);
}

// aCurrentState	: See / Voir SERVICE_...
void ReportStatus(DWORD aCurrentState)
{
	SERVICE_STATUS lStatus;

	memset(&lStatus, 0, sizeof(lStatus));

	switch (aCurrentState)
	{
	case SERVICE_RUNNING :
	case SERVICE_STOPPED :
		break;

	default :
		lStatus.dwCheckPoint	= sCheckPoint	;
		lStatus.dwWaitHint		= 10			; // ms
		sCheckPoint++;
	}

	lStatus.dwControlsAccepted	= SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
	lStatus.dwCurrentState		= aCurrentState				;
	lStatus.dwServiceType		= SERVICE_WIN32_OWN_PROCESS	;

	BOOL lRetB = SetServiceStatus(sStatusHandle, &lStatus);
	if (!lRetB)
	{
		ReportError(__LINE__, __FUNCTION__, "SetServiceStatus( ,  ) failed", lRetB);
	}
}

// ===== Event handler / Gestionnaire d'evennement ==========================

void Shutdown()
{
	// TODO : Prepare for shutdown
}

void Start()
{
	ReportStatus(SERVICE_START_PENDING);

	// TODO : Start the service / Lancer le service

	ReportInfo(MSG_STARTED);

	ReportStatus(SERVICE_RUNNING);
}

void Stop()
{
	ReportStatus(SERVICE_STOP_PENDING);

	// TODO : Stop the service / Stopper le service

	ReportStatus(SERVICE_STOPPED);
}

// ===== Entry points / Points d'entres =====================================

DWORD WINAPI ServiceCtrlHandler(DWORD aControl, DWORD aEventType, LPVOID aEventData, LPVOID aContext)
{
	assert(NULL == aContext);

	DWORD lResult;

	try
	{
		switch (aControl)
		{
		case SERVICE_CONTROL_SHUTDOWN	: Shutdown	(); lResult = NO_ERROR; break;
		case SERVICE_CONTROL_STOP		: Stop		(); lResult = NO_ERROR; break;

		case SERVICE_INTERROGATE: lResult = NO_ERROR; break;

		default: lResult = ERROR_CALL_NOT_IMPLEMENTED;
		}
	}
	catch (...)
	{
		ReportError(__LINE__, __FUNCTION__, "Unknown exception", 0);
		lResult = ERROR_UNHANDLED_EXCEPTION;
	}

	return lResult;
}

VOID WINAPI ServiceMain(DWORD aCount, LPTSTR * aVector)
{
	assert(0 == sStatusHandle);

	try
	{
		sStatusHandle = RegisterServiceCtrlHandlerEx(SERVICES[0].lpServiceName, ServiceCtrlHandler, NULL);
		if (0 == sStatusHandle)
		{
			ReportError(__LINE__, __FUNCTION__, "RegisterServiceCtrlHandler( ,  ) failed", 0);
			return;
		}

		Start();
	}
	catch (...)
	{
		ReportError(__LINE__, __FUNCTION__, "Unknown exception", 0);
	}
}
