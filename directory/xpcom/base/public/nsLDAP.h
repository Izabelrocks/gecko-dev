#include "ldap.h"
#include "nsError.h"
#include "nspr.h"

#define NS_ERROR_LDAP_OPERATIONS_ERROR \
  NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_LDAP, LDAP_OPERATIONS_ERROR)

#define NS_ERROR_LDAP_ENCODING_ERROR \
  NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_LDAP, LDAP_ENCODING_ERROR)

#define NS_ERROR_LDAP_SERVER_DOWN \
  NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_LDAP, LDAP_SERVER_DOWN)

#define NS_ERROR_LDAP_NOT_SUPPORTED \
  NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_LDAP, LDAP_NOT_SUPPORTED)

#ifdef DEBUG
extern PRLogModuleInfo *gLDAPLogModule;	   // defn in nsLDAPProtocolModule.cpp
#endif
