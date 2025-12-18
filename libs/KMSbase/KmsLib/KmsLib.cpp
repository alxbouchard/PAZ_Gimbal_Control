
// Auteur	:	KMS -	Martin Dubois, ing.
// Projet	:	KmsBase
// Fichier	:	KmsLib/KmsLib.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== Interface ==========================================================
#include <KmsLib/KmsLib.h>

// ===== Common =============================================================
#include "../Common/Version.h"

namespace KmsLib
{

	// Constantes
	/////////////////////////////////////////////////////////////////////////

	const char * VERSION = VERSION_STR " - Compiled at " __TIME__ " on " __DATE__

#ifdef _DEBUG
		"(Debug)"
#endif // _DEBUG

		;

}
