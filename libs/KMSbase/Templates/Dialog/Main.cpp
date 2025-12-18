
// Auteur	:	KMS -	Martin Dubois, ing.
// Client	:	TODO
// Projet	:	TODO
// Fichier	:	TODO/Main.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== Windows ============================================================
#include <Windows.h>

// ===== Local ==============================================================
#include "Dialog.h"

// Point d'entree
/////////////////////////////////////////////////////////////////////////////

int APIENTRY WinMain(HINSTANCE aInstance, HINSTANCE aPrevious, LPSTR aCommand, int aCmdShow)
{
	System::Windows::Forms::Application::Run(gcnew Dialog());
}
