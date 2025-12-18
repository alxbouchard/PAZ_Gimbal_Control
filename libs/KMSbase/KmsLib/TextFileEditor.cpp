
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib/TextFileEditor.cpp

// Last test coverage update / Derniere mise a jour de la couverture de test
// 2017-11-11

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes/KmsLib ====================================================
#include <KmsLib/Exception.h>
#include <KmsLib/File.h>

#include <KmsLib/TextFileEditor.h>

namespace KmsLib
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    TextFileEditor::TextFileEditor()
    {
    }

    TextFileEditor::~TextFileEditor()
    {
    }
    
    unsigned int TextFileEditor::GetLineNumber_Read() const
    {
        return mRead.GetLineNumber();
    }

    unsigned int TextFileEditor::GetLineNumber_Write() const
    {
        return mWrite.GetLineNumber();
    }

    bool TextFileEditor::IsOpened() const
    {
        return (mRead.IsOpened() && mWrite.IsOpened());
    }

    void TextFileEditor::Close()
    {
        if (IsOpened())
        {
            mRead .Close();
            mWrite.Close();
        }
    }

    void TextFileEditor::Open(const char * aFileName, unsigned int aFlags)
    {
        assert(NULL != aFileName);

        if (IsOpened())
        {
            Close();
        }

        char lBackup[2048];

        sprintf_s(lBackup, "%s.bak", aFileName);

        File::Move(aFileName, lBackup);

        mRead .Open  (lBackup   );
        mWrite.Create(aFileName );
    }

    bool TextFileEditor::ReadLine(char * aOut, unsigned int aOutSize_byte)
    {
        assert(NULL != aOut         );
        assert(   0 <  aOutSize_byte);

        return mRead.ReadLine(aOut, aOutSize_byte);
    }

    void TextFileEditor::WriteLine(const char * aIn)
    {
        assert(NULL != aIn);

        mWrite.WriteLine(aIn);
    }

}
