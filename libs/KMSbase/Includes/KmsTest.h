
// Product / Produit  KmsBase

/// \author  KMS - Martin Dubois, ing.
/// \file    Includes/KmsTest.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== C++ ================================================================
#include <exception>

// ===== Windows ============================================================
#ifdef _WIN32
    #include "Windows.h"
#endif // _WIN32

// Constants
/////////////////////////////////////////////////////////////////////////////

/// \cond en
/// \brief  This flag indicates the test require user interaction.
/// \endcond
/// \cond fr
/// \brief	Ce drapeau indique que le test necessite une interaction avec
///			l'utilisateur.
/// \endcond
#define KMS_TEST_FLAG_INTERACTION_NEEDED	(0x00000001)

// Type de donnees
/////////////////////////////////////////////////////////////////////////////

/// \cond en
/// \brief  Definition of the type of a test function
/// \endcond
/// \cond fr
/// \brief	Definition du type d'une fonction de test
/// \endcond
typedef int(*KmsTestFunction)(void);

/// \cond

typedef struct
{
	unsigned int	mFlags		;
	KmsTestFunction	mFunction	;
	unsigned int	mGroup		;
	const char	  * mName		;
}
KmsTestDescription;

/// \endcond

// Macro
/////////////////////////////////////////////////////////////////////////////

#define  BLUE    (0x09)
#define  GREEN   (0x0a)
#define  RED     (0x0c)
#define  WHITE   (0x07)

#ifdef _WIN32
    #define  COLOR(A)  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (A))
#else
    #define  COLOR(A)
#endif // _WIN32

// ===== Test ===============================================================

/// \cond	en
/// \brief  This macro display the error and increment the error counter.
/// \endcond
/// \cond	fr
/// \brief	Cette macro affiche l'erreur et incremente le compteur d'erreur.
/// \endcond
#define KMS_TEST_ERROR()                                                                     \
    {                                                                                        \
        COLOR( RED );                                                                        \
        printf( "Test " __FUNCTION__ " failed at line %u of file " __FILE__ "\n", __LINE__ ); \
        COLOR( WHITE );                                                                      \
        lResult++;                                                                           \
    }

/// \cond	en
/// \brief  This macro display the error and increment the error counter.
/// \endcond
/// \cond	fr
/// \brief	Cette macro affiche l'erreur et incremente le compteur d'erreur.
/// \endcond
#define KMS_TEST_ERROR_1(S)                                                                         \
    {                                                                                               \
        COLOR( RED );                                                                               \
        printf( "Test " __FUNCTION__ " failed at line %u of file " __FILE__ " (" S ")\n", __LINE__ ); \
        COLOR( WHITE );                                                                             \
        lResult++;                                                                                  \
    }

/// \cond	en
/// \brief  This macro display the error and increment the error counter.
/// \endcond
/// \cond	fr
/// \brief	Cette macro affiche l'erreur et incremente le compteur d'erreur.
/// \endcond
#define KMS_TEST_ERROR_2(E,G)                                                                                               \
    {                                                                                                                       \
        COLOR( RED );                                                                                                       \
        printf( "Test " __FUNCTION__ " failed at line %u of file " __FILE__ " (Expected = %d, Get = %d)\n", __LINE__, E, G ); \
        COLOR( WHITE );                                                                                                     \
        lResult++;                                                                                                          \
    }

/// \cond	en
/// \brief  If the condition \a Co is false, this macro count the error.
/// \endcond
/// \cond	fr
/// \brief	Si la condition \a Co est fausse, cette macro compte l'erreur.
/// \endcond
#define KMS_TEST_ASSERT(Co)     \
    if (!(Co))                  \
    {                           \
        KMS_TEST_ERROR_1( #Co ) \
    }

/// \cond	en
/// \brief  If the condition \a Co is false, this macro count the error and
///         execute the break instruction.
/// \endcond
/// \cond	fr
/// \brief	Si la condition \a Co est fausse, cette macro compte l'erreur et
///         execute l'instruction break.
/// \endcond
#define KMS_TEST_ASSERT_BREAK(Co) \
    if (!(Co))                    \
    {                             \
        KMS_TEST_ERROR_1( #Co );  \
        break;                    \
    }

/// \cond	en
/// \brief	If the condition \a Co is false, this macro fail the test and
///			continue the execution at \a Cl label for cleanup.
/// \endcond
/// \cond	fr
/// \brief	Si la condition \a Co est fausse, cette macro echoue le test et
///			continue l'execution a l'ettiquette \a Cl pour le nettoyage.
/// \endcond
#define KMS_TEST_ASSERT_CLEANUP(Co,Cl)														\
	if (!(Co))																				\
	{																						\
        KMS_TEST_ERROR_1( #Co );       \
		goto Cl;																			\
	}

/// \cond	en
/// \brief  If the condition \a Co is false, this macro count the error and
///         execute the continue instruction.
/// \endcond
/// \cond	fr
/// \brief	Si la condition \a Co est fausse, cette macro compte l'erreur et
///         execute l'instruction continue.
/// \endcond
#define KMS_TEST_ASSERT_CONTINUE(Co) \
    if (!(Co))                       \
    {                                \
        KMS_TEST_ERROR_1( #Co );     \
        continue;                    \
    }

/// \cond	en
/// \brief  If the condition \a Co is false, this macro count the error and
///         execute the goto instruction.
/// \endcond
/// \cond	fr
/// \brief	Si la condition \a Co est fausse, cette macro compte l'erreur et
///         execute l'instruction goto.
/// \endcond
#define KMS_TEST_ASSERT_GOTO(Co,To) \
    if (!(Co))                      \
    {                               \
        KMS_TEST_ERROR_1( #Co );    \
        goto To;                    \
    }

/// \cond	en
/// \brief  If the condition \a Co is false, this macro count the error and
///         execute the return instruction.
/// \endcond
/// \cond	fr
/// \brief	Si la condition \a Co est fausse, cette macro compte l'erreur et
///         execute l'instruction return.
/// \endcond
#define KMS_TEST_ASSERT_RETURN(Co) \
    if (!(Co))                     \
    {                              \
        KMS_TEST_ERROR_1( #Co );   \
        return lResult;            \
    }

/// \cond	en
/// \brief	This macro indicates the begining of the test named \a Na.
/// \endcond
/// \cond	fr
/// \brief	Cette macro indique le debut du test nomme \a Na.
/// \endcond
#define KMS_TEST_BEGIN(Na)			\
	int Na()						\
	{								\
		unsigned int lResult = 0;	\
        int          lRet;          \
        (void)(lRet);               \
		try							\
		{

/// \cond	en
/// \brief  If the expected value \a E is not equal to the get value \a G,
///         this macro count the error.
/// \endcond
/// \cond	fr
/// \brief	Si la valeur expecte \a E n'est pas egal a la valeur obtenue \a
///         G, cette macro compte l'erreur.
/// \endcond
#define KMS_TEST_COMPARE(E,G)          \
    lRet = (G);                        \
    if ( (E) != lRet )                 \
    {                                  \
        KMS_TEST_ERROR_2( (E), lRet ); \
    }

/// \cond	en
/// \brief  If the expected value \a E is not equal to the get value \a G,
///         this macro count the error and execute the break instruction.
/// \endcond
/// \cond	fr
/// \brief	Si la valeur expecte \a E n'est pas egal a la valeur obtenue \a
///         G, cette macro compte l'erreur et execute l'instruction break.
/// \endcond
#define KMS_TEST_COMPARE_BREAK(E,G)    \
    lRet = (G);                        \
    if ( (E) != lRet )                 \
    {                                  \
        KMS_TEST_ERROR_2( (E), lRet ); \
        break;                         \
    }

/// \cond	en
/// \brief  If the expected value \a E is not equal to the get value \a G,
///         this macro count the error and execute the continue instruction.
/// \endcond
/// \cond	fr
/// \brief	Si la valeur expecte \a E n'est pas egal a la valeur obtenue \a
///         G, cette macro compte l'erreur et execute l'instruction continue.
/// \endcond
#define KMS_TEST_COMPARE_CONTINUE(E,G) \
    lRet = (G);                        \
    if ( (E) != lRet )                 \
    {                                  \
        KMS_TEST_ERROR_2( (E), lRet ); \
        continue;                      \
    }

/// \cond	en
/// \brief  If the expected value \a E is not equal to the get value \a G,
///         this macro count the error and execute the goto instruction.
/// \endcond
/// \cond	fr
/// \brief	Si la valeur expecte \a E n'est pas egal a la valeur obtenue \a
///         G, cette macro compte l'erreur et execute l'instruction goto.
/// \endcond
#define KMS_TEST_COMPARE_GOTO(E,G,To)  \
    lRet = (G);                        \
    if ( (E) != lRet )                 \
    {                                  \
        KMS_TEST_ERROR_2( (E), lRet ); \
        goto To;                       \
    }

/// \cond	en
/// \brief  If the expected value \a E is not equal to the get value \a G,
///         this macro count the error and execute the return instruction.
/// \endcond
/// \cond	fr
/// \brief	Si la valeur expecte \a E n'est pas egal a la valeur obtenue \a
///         G, cette macro compte l'erreur et execute l'instruction return.
/// \endcond
#define KMS_TEST_COMPARE_RETURN(E,G)   \
    lRet = (G);                        \
    if ( (E) != lRet )                 \
    {                                  \
        KMS_TEST_ERROR_2( (E), lRet ); \
        return lResult;                \
    }

/// \cond	en
/// \brief	This macro indicate the end of the test.
/// \endcond
/// \cond	fr
/// \brief	Cette macro indique la fin du test.
/// \endcond
#define KMS_TEST_END				\
		}							\
		catch (std::exception * eE)	\
		{							\
            KMS_TEST_ERROR_1( "std::exception" ); \
            printf( "%s\n", eE->what() );         \
            lResult ++;                           \
		}							\
		catch ( ... )				\
		{							\
            KMS_TEST_ERROR_1( "Unknown exception" ); \
            lResult ++;                              \
		}							\
		return lResult;				\
	}

/// \cond	en
/// \brief	As KMS_TEST_END but display KmsLib::Exception.
/// \note	If a test program use this macro, it must link with KmsLib.lib.
/// \endcond
/// \cond	fr
/// \brief	Comme KMS_TEST_END, mais affiche les KmsLib::Exception.
/// \note	Si un programme de test utilise cette macro, KmsLib.lib doit etre
///			inclue lors de l'edition des liens.
/// \endcond
#define KMS_TEST_END_2					\
		}								\
		catch (KmsLib::Exception * eE)	\
		{								\
            KMS_TEST_ERROR_1( "KmsLib::Exception" ); \
			eE->Write(stdout);			\
            lResult ++;                              \
		}								\
		catch (std::exception * eE)		\
		{								\
            KMS_TEST_ERROR_1( "std::exception" ); \
            printf( "%s\n", eE->what() );         \
            lResult ++;                           \
		}								\
		catch (...)						\
		{								\
            KMS_TEST_ERROR_1( "Unknown exception" ); \
            lResult ++;                              \
		}								\
	return lResult;						\
	}

/// \cond	en
/// \brief	This macro display a message indicating the following error
///			message is caused by a test of error handling code.
/// \endcond
/// \cond	fr
/// \brief	Cette macro affiche un message qui indique que le message
//			d'erreur qui suit est du a un test du code de gestion d'erreur.
/// \endcond
#define KMS_TEST_ERROR_INFO	 \
    COLOR(BLUE);             \
    printf("    ===== Information about a succesfully tested error condition =====\n"); \
    COLOR(WHITE)

// ===== Test group list ====================================================

/// \cond	en
/// \brief	This macro indicate the begining of the test group list.
/// \endcond
/// \cond	fr
/// \brief	Cette macro indique le debut de la liste de groupes de tests.
/// \endcond
#define KMS_TEST_GROUP_LIST_BEGIN					\
	static const char * KMS_TEST_GROUP_NAMES[] =	\
{

/// \cond	en
/// \brief	This macro indicate the end of the test group list.
/// \endcond
/// \cond	fr
/// \brief	Cette macro indique la fin de la liste de groupes de tests.
/// \endcond
#define KMS_TEST_GROUP_LIST_END	\
};

/// \cond	en
/// \brief	This macro add a test group to the test group list.
/// \endcond
/// \cond	fr
/// \brief	Cette macro ajoute un groupe a la liste des groupes de tests.
/// \endcond
#define KMS_TEST_GROUP_LIST_ENTRY(Na)	\
	Na,

// ===== Test list ==========================================================

/// \cond	en
/// \brief	This macro indicate the begining of the test list.
/// \endcond
/// \cond	fr
/// \brief	Cette macro indique le debut de la liste de tests.
/// \endcond
#define	KMS_TEST_LIST_BEGIN					\
	static KmsTestDescription KMS_TESTS[] = \
{

/// \cond	en
/// \brief	This macro indicate the end of the test list.
/// \endcond
/// \cond	fr
/// \brief	Cette macro indique la fin de la liste de tests.
/// \endcond
#define KMS_TEST_LIST_END	\
};

/// \cond	en
/// \brief	This macro add a test to the test list.
/// \endcond
/// \cond	fr
/// \brief	Cette macro ajoute un test a la liste de tests.
/// \endcond
#define KMS_TEST_LIST_ENTRY(Fu,Na,Gr,Fl)	\
{ Fl, Fu, Gr, Na },

// ===== Main ===============================================================

/// \cond	en
/// \brief	This macro define the test executable entry point.
/// \endcond
/// \cond	fr
/// \brief  Cet macro definit le point d'entre d'un executable de test.
/// \endcond
#define KMS_TEST_MAIN														\
	static void	DisplayHelp(const char * aExe);								\
	static void	DisplayTests();												\
	static void	DisplayResults(int aResult);								\
	static int	RunAll();													\
	static int	RunAuto();													\
	static int	RunGroup(const char * aGroup);								\
	static int	RunTest(unsigned int aTest);								\
	static int	RunTests(const char ** aTests, int aCount);					\
																			\
	int main(int aCount, const char ** aVector)								\
	{																		\
        COLOR(BLUE);                                                        \
		printf("Test program compiler at " __TIME__ " on " __DATE__ "\n\n");	\
        COLOR(WHITE);                                                       \
																			\
		if ((1 >= aCount) || (0 == _stricmp("auto", aVector[1])))			\
		{																	\
			if (2 < aCount) { DisplayHelp(aVector[0]); return 1000; }		\
																			\
			return RunAuto();												\
		}																	\
																			\
		if (0 == _stricmp("all", aVector[1]))								\
		{																	\
			if (2 != aCount) { DisplayHelp(aVector[0]); return 1001; }		\
																			\
			return RunAll();												\
		}																	\
																			\
		if (0 == _stricmp("group", aVector[1]))								\
		{																	\
			if (3 != aCount) { DisplayHelp(aVector[0]);	return 1002; }		\
																			\
			return RunGroup(aVector[2]);									\
		}																	\
																			\
		if (0 == _stricmp("help", aVector[1]))								\
		{																	\
			DisplayHelp(aVector[0]);										\
			return ((2 == aCount) ? 0 : 1003);								\
		}																	\
																			\
		if (0 == _stricmp("list", aVector[1]))								\
		{																	\
			if (2 != aCount) { DisplayHelp(aVector[0]); return 1003; }		\
																			\
			DisplayTests();													\
			return 0;														\
		}																	\
																			\
		return RunTests( aVector + 1, aCount - 1);							\
	}																		\
																			\
	void DisplayHelp(const char * aExe)										\
	{																		\
        COLOR(BLUE);                                                        \
		printf(																\
			"Usage:\n"														\
			"    %s\n"														\
			"	 %s all\n"													\
			"    %s auto\n"													\
			"    %s group {GroupNumber}\n"									\
			"    %s help\n"													\
			"    %s list\n"													\
			"    %s {TestNumber} [TestNumber] ...\n"						\
			"\n",															\
			aExe, aExe, aExe, aExe, aExe, aExe, aExe );						\
        COLOR(WHITE);                                                       \
	}																		\
																			\
	void DisplayResults(int aResult)										\
	{																		\
		switch (aResult)													\
		{																	\
		case 0:                                                             \
            COLOR(GREEN);                                                   \
            printf("\nAll tests PASSED\n\n"			);                      \
            break;                                                   		\
		case 1:                                                             \
            COLOR(RED);                                                     \
            printf("\n1 test FAILED\n\n"				);                  \
            break;                                                          \
		default	:                                                           \
            COLOR(RED);                                                     \
            printf("\n%u tests FAILED\n\n", aResult	);                      \
            break;                                                          \
		}																	\
        COLOR(WHITE);                                                       \
	}																		\
																			\
	void DisplayTests()														\
	{																		\
        COLOR(BLUE);                                                        \
		unsigned int i;														\
		printf("Groups:\n");												\
		printf("    #      Name\n");										\
		for (i = 0; i < (sizeof(KMS_TEST_GROUP_NAMES) / sizeof(KMS_TEST_GROUP_NAMES[0])); i++)	\
		{																	\
			printf("    %2u    %s\n", i, KMS_TEST_GROUP_NAMES[i]);			\
		}																	\
		printf( "\n" );														\
		printf("Tests\n");													\
		printf("    #     Group		Name\n");								\
		for (i = 0; i < (sizeof(KMS_TESTS) / sizeof(KMS_TESTS[0])); i++)	\
		{																	\
			printf("    %2u    %2u        %s", i, KMS_TESTS[i].mGroup, KMS_TESTS[i].mName);	\
			if (0 != (KMS_TEST_FLAG_INTERACTION_NEEDED & KMS_TESTS[i].mFlags))	\
			{																\
				printf("\t- Interaction needed");							\
			}																\
			printf("\n");													\
		}																	\
		printf("\n");														\
        COLOR(WHITE);                                                       \
	}																		\
																			\
	int RunAll()															\
	{																		\
        COLOR(BLUE);                                                        \
		printf("Executing all tests...\n\n");								\
        COLOR(WHITE);                                                       \
		int lResult = 0;													\
																			\
		for (unsigned int i = 0; i < (sizeof(KMS_TESTS) / sizeof(KMS_TESTS[0])); i++)	\
		{																	\
			lResult += RunTest(i);											\
		}																	\
																			\
		DisplayResults(lResult);											\
		return lResult;														\
	}																		\
																			\
	int RunAuto()															\
	{																		\
        COLOR(BLUE);                                                        \
		printf("Executing all tests that doesn't require interaction...\n\n");	\
        COLOR(WHITE);                                                       \
		int lResult = 0;													\
																			\
		for (unsigned int i = 0; i < (sizeof(KMS_TESTS) / sizeof(KMS_TESTS[0])); i++)	\
		{																	\
			if (0 == (KMS_TESTS[i].mFlags & KMS_TEST_FLAG_INTERACTION_NEEDED))	\
			{																\
				lResult += RunTest(i);										\
			}																\
		}																	\
																			\
		DisplayResults(lResult);											\
		return lResult;														\
	}																		\
																			\
	int RunGroup(const char * aGroupNumber)									\
	{																		\
		unsigned int lGroup	= static_cast< unsigned int >( strtoul(aGroupNumber, NULL, 10) );	\
																			\
        COLOR(BLUE);                                                        \
		printf("Executing test group %u ...\n\n", lGroup);					\
        COLOR(WHITE);                                                       \
		int	lResult = 0;													\
																			\
		for (unsigned int i = 0; i < (sizeof(KMS_TESTS) / sizeof(KMS_TESTS[0])); i++)	\
		{																	\
			if (lGroup == KMS_TESTS[i].mGroup)								\
			{																\
				lResult += RunTest(i);										\
			}																\
		}																	\
																			\
		DisplayResults(lResult);											\
		return lResult;														\
	}																		\
																			\
	int RunTest(unsigned int aTest)											\
	{																		\
	    if ((sizeof(KMS_TESTS) / sizeof(KMS_TESTS[0])) <= aTest)            \
		{                                                                   \
			COLOR(RED);                                                     \
			printf("%u is not a valid test number\n", aTest);               \
			COLOR(WHITE);                                                   \
			return 1;                                                       \
		}                                                                   \
        COLOR(BLUE);                                                        \
		printf("Executing test %u : %s\n", aTest, KMS_TESTS[aTest].mName);	\
        COLOR(WHITE);                                                       \
		int lResult = KMS_TESTS[aTest].mFunction();							\
        COLOR(0 >= lResult ? GREEN : RED);                                  \
        printf("Test %u : %s\n", aTest, 0 >= lResult ? "PASSED" : "FAILED"); \
        COLOR(WHITE);                                                       \
		return (0 >= lResult) ? 0 : 1;        								\
	}																		\
																			\
	int RunTests(const char ** aTests, int aCount)							\
	{																		\
        COLOR(BLUE);                                                        \
		printf("Executing indicated tests...\n\n");							\
        COLOR(WHITE);                                                       \
		unsigned int lResult = 0;											\
																			\
		while (0 < aCount)													\
		{																	\
			lResult += RunTest( static_cast< unsigned int >( strtoul(*aTests, NULL, 10) ) );	\
																			\
			aCount--;														\
			aTests++;														\
		}																	\
																			\
		DisplayResults(lResult);											\
		return lResult;														\
	}
