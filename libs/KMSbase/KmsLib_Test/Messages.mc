
;// Author / Auteur	 :	KMS -	Martin Dubois, ing.
;// Project / Projet :	KmsBase
;// File / Fichier   :	KmsLib_Test/Messages.mc

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
Severity=Informational
Facility=System
SymbolicName=MSG_TEST
Language=English
KmsLib_Test - Test Message!
.
