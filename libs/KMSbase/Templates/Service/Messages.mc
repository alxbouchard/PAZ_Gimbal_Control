
;// Author / Auteur	 :	KMS -	Martin Dubois, ing.
;// Client           :	TODO
;// Project / Projet :	TODO
;// File / Fichier   :	KmsLib_Test/Windows/SystemLog.mc

MessageIdTypedef=DWORD

SeverityNames=(Success=0x0:STATUS_SEVERITY_SUCCESS
Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
Warning=0x2:STATUS_SEVERITY_WARNING
Error=0x3:STATUS_SEVERITY_ERROR
)

FacilityNames=(System=0x0:FACILITY_SYSTEM
Runtime=0x2:FACILITY_RUNTIME
Stubs=0x3:FACILITY_STUBS
Io=0x4:FACILITY_IO_ERROR_CODE
)

LanguageNames=(English=0x409:MSG00409)

MessageId=0x1
Severity=Error
Facility=System
SymbolicName=MSG_ERROR
Language=English
Error in function %1 : %2
.

MessageId=0x2
Severity=Informational
Facility=System
SymbolicName=MSG_STARTED
Language=English
The service TODO is now running
.
