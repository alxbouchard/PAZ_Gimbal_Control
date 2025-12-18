
// Auteur	:	KMS - Martin Dubois, ing.
// Client	:	TODO
// Project	:	TODO
// Fichier	:	TODO/Dialog.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== Local ==============================================================
#include "Dialog.h"

// Public
/////////////////////////////////////////////////////////////////////////////

// Constructeur par defaut
Dialog::Dialog()
{
	InitializeComponent();
}

// Protected
/////////////////////////////////////////////////////////////////////////////

// Destructeur
Dialog::~Dialog()
{
	if (nullptr != components)
	{
		delete components;
		components = nullptr;
	}
}
