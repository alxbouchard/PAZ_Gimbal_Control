
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KmsBase

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2021 KMS
/// \file      Includes/KmsLib/ThreadBase.h

#pragma once

namespace KmsLib
{

    // Class
    /////////////////////////////////////////////////////////////////////////

    /// \cond	en
    /// \brief	Manage a thread
    /// \endcond
    /// \cond	fr
    /// \brief	Gere un thread
    /// \endcond
    class ThreadBase
    {

    public:

        typedef enum
        {
            PRIORITY_UNKNOWN,

            PRIORITY_IDLE    ,
            PRIORITY_LOW     ,
            PRIORITY_NORMAL  ,
            PRIORITY_HIGH    ,
            PRIORITY_CRITICAL,

            PRIORITY_QTY
        }
        Priority;

        // --> INIT <--+<------------------------------------------------------+
        //      |      |                                                       |
        //      |     STOPPING <--+<------+<--------+<--------------+          |
        //      |                 |       |         |               |          |
        //      |                 |       |    +--> RUNNING --+     |          |
        //      |                 |       |    |              |     |          |
        //      +--> START_REQUESTED --> STARTING ----------->+--> STOP_REQUESTED
        typedef enum
        {
            STATE_INIT,
            STATE_RUNNING,
            STATE_START_REQUESTED,
            STATE_STARTING,
            STATE_STOP_REQUESTED,
            STATE_STOPPING,

            STATE_QTY
        }
        State;

        /// \cond	en
        ///	\brief	Sleep
        /// \param  aDelay_ms
        /// \endcond
        /// \cond	fr
        /// \brief	Attendre
        /// \param  aDelay_ms
        /// \endcond
        static void Sleep_ms(unsigned int aDelay_ms);

        /// \cond	en
        ///	\brief	Sleep
        /// \param  aDelay_s
        /// \endcond
        /// \cond	fr
        /// \brief	Attendre
        /// \param  aDelay_s
        /// \endcond
        static void Sleep_s(unsigned int aDelay_s);

        /// \cond	en
        ///	\brief	Sleep
        /// \param  aDelay_us
        /// \endcond
        /// \cond	fr
        /// \brief	Attendre
        /// \param  aDelay_us
        /// \endcond
        static void Sleep_us(unsigned int aDelay_us);

        /// \cond	en
        ///	\brief	Destructor
        /// \endcond
        /// \cond	fr
        /// \brief	Destructeur
        /// \endcond
        virtual ~ThreadBase();

        /// \cond	en
        ///	\brief	Get the current priority
        /// \return See PRIORITY_...
        /// \endcond
        /// \cond	fr
        /// \brief	Optenir la priorite actuelle
        /// \return Voir PRIORITY_...
        /// \endcond
        /// \throw  Exception  CODE_THREAD_ERROR
        Priority GetCurrentPriority() const;

        /// \cond	en
        ///	\brief	Get the configure priority
        /// \return See PRIORITY_...
        /// \endcond
        /// \cond	fr
        /// \brief	Optenir la priorit configure
        /// \return Voir PRIORITY_...
        /// \endcond
        Priority GetPriority() const;

        /// \cond	en
        ///	\brief	Get the state
        /// \return See STATE_...
        /// \endcond
        /// \cond	fr
        /// \brief	Optenir l'etat
        /// \return Voir STATE_...
        /// \endcond
        State GetState() const;

        /// \cond	en
        ///	\brief	Is the thrad running?
        /// \retval false  No
        /// \retval true   Yes
        /// \endcond
        /// \cond	fr
        /// \brief	Est-ce que le thread est en cours d'execution?
        /// \retval false  Non
        /// \retval true   Oui
        /// \endcond
        bool IsRunning() const;

        /// \cond	en
        ///	\brief	Is the thrad stopped?
        /// \retval false  No
        /// \retval true   Yes
        /// \endcond
        /// \cond	fr
        /// \brief	Est-ce que le thread est arrete?
        /// \retval false  Non
        /// \retval true   Oui
        /// \endcond
        bool IsStopped() const;

        /// \cond	en
        ///	\brief	Is the thrad stopping?
        /// \retval false  No
        /// \retval true   Yes
        /// \endcond
        /// \cond	fr
        /// \brief	Est-ce que le thread est en cour d'arret?
        /// \retval false  Non
        /// \retval true   Oui
        /// \endcond
        bool IsStopping() const;

        /// \cond	en
        ///	\brief	Set the priority
        /// \param  aPriority  See PRIORITY_...
        /// \endcond
        /// \cond	fr
        /// \brief	Changer la priority
        /// \return Voir PRIORITY_...
        /// \endcond
        /// \throw  Exception  CODE_THREAD_ERROR
        void SetPriority(Priority aPriority);

        /// \cond	en
        ///	\brief	Start the thread
        /// \endcond
        /// \cond	fr
        /// \brief	Lancer le thread
        /// \endcond
        /// \throw  Exception  CODE_STATE_ERROR, CODE_THREAD_ERROR
        void Start();

        /// \cond	en
        ///	\brief	Stopper the thread
        /// \endcond
        /// \cond	fr
        /// \brief	Arreter le thread
        /// \endcond
        /// \throw  Exception  CODE_STATE_ERROR
        void Stop();

        /// \cond	en
        ///	\brief	Stop the thread and wait for it to stop
        /// \param  aForce       Force the thread end
        /// \param  aTimeout_ms  The timeout
        /// \retval false  The thread did not stop normaly
        /// \endcond
        /// \cond	fr
        /// \brief	Arreter le thread attendre qu'il soit arreter
        /// \param  aForce       Force la fin d<execution du thread
        /// \param  aTimeout_ms  Le temps maximum d'attente
        /// \retval false  Le thread ne c'est pas arreter normalement
        /// \endcond
        /// \retval true  OK
        /// \throw  Exception  CODE_STATE_ERROR
        bool StopAndWait(bool aForce = false, unsigned int aTimeout_ms = 0xffffffff);

        /// \cond	en
        ///	\brief	Wait for the thread to stop.
        /// \param  aForce       Force the thread end
        /// \param  aTimeout_ms  The timeout
        /// \retval false  The thread did not stop normaly
        /// \endcond
        /// \cond	fr
        /// \brief	Attendre que le thread soit arreter
        /// \param  aForce       Force la fin d<execution du thread
        /// \param  aTimeout_ms  Le temps maximum d'attente
        /// \retval false  Le thread ne c'est pas arreter normalement
        /// \endcond
        /// \retval true  OK
        /// \throw  Exception  CODE_THREAD_ERROR
        bool Wait(bool aForce = false, unsigned int aTimeout_ms = 0xffffffff);

    // internal:

        unsigned int Run_Internal();

    protected:

        /// \cond	en
        ///	\brief	Constructor
        /// \endcond
        /// \cond	fr
        /// \brief	Constructeur
        /// \endcond
        ThreadBase();

        /// \cond	en
        ///	\brief	The thrad entry point 
        /// \endcond
        /// \cond	fr
        /// \brief	Le point d'entree du thread
        /// \endcond
        virtual unsigned int Run() = 0;

    private:

        void ApplyPriority();
        void CloseThread  ();
        void Terminate    ();

        unsigned int Wait_Internal(unsigned int aTimeout_ms);

        Priority mPriority;
        State    mState   ;

        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
            pthread_t mThread;
        #endif

        #ifdef _KMS_WINDOWS_
            HANDLE mThread  ;
            DWORD  mThreadId;
        #endif

    };

    // Public
    /////////////////////////////////////////////////////////////////////////

    inline ThreadBase::State ThreadBase::GetState() const
    {
        return mState;
    }

    inline bool ThreadBase::IsRunning() const
    {
        return (STATE_RUNNING == mState);
    }

    inline bool ThreadBase::IsStopped() const
    {
        return (STATE_INIT == mState);
    }

}
