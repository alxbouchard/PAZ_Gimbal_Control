
// Author / Auteur		:	KMS -	Martin Dubois, ing
// Product / Produit	:	KmsBase
// File / Fichier		:	Template/Dialog_Win32/TODO.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <assert.h>

// ===== Windows ============================================================
#include <Windows.h>

// ===== Local ==============================================================
#include "resource.h"

// Static function declaration / Declaration des fonctions statiques
/////////////////////////////////////////////////////////////////////////////

// ===== Entry point / Point d'entree =======================================
static LRESULT CALLBACK DialogProc(HWND aWindow, UINT aMsg, WPARAM aParam1, LPARAM aParam2);

// Entry point / Point d'entree
/////////////////////////////////////////////////////////////////////////////

int APIENTRY WinMain(HINSTANCE aInstance, HINSTANCE aPrevInstance, LPSTR aCmdLine, int aCmdShow)
{
	assert(NULL != aInstance	);
	assert(NULL != aCmdLine		);

	DialogBox(aInstance, reinterpret_cast<LPCTSTR>(IDD_DIALOG1), 0, reinterpret_cast<DLGPROC>(DialogProc));

	return 0;
}

// Static functions / Fonctions statiques
/////////////////////////////////////////////////////////////////////////////

// ===== Entry point / Point d'entree =======================================

LRESULT CALLBACK DialogProc(HWND aWindow, UINT aMsg, WPARAM aParam1, LPARAM aParam2)
{
	assert(NULL != aWindow);

	switch (aMsg)
	{
	case WM_INITDIALOG :
		return TRUE;

	case WM_CLOSE :
		EndDialog(aWindow, 0);
		break;

		/*
		WM_DESTROY
		WM_ACTIVATE
		WM_SETFOCUS
		WM_PAINT
		WM_CLOSE
		WM_ERASEBKGND
		WM_SHOWWINDOW
		WM_ACTIVATEAPP
		WM_SETFONT
		WM_WINDOWPOSCHANGING
		WM_WINDOWPOSCHANGED
		WM_GETICON
		WM_NCDESTROY
		WM_NCCALCSIZE
		WM_NCPAINT
		WM_NCACTIVATE
		WM_INITDIALOG
		WM_CHANGEUISTATE
		WM_CTLCOLORBTN
		WM_CTLCOLORDLG
		WM_IME_SETCONTEXT
		WM_USER
		*/
	}

	return FALSE;
}
