/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

#ifndef __nsRespository_h
#define __nsRespository_h

#include "prtypes.h"
#include "prlog.h"
#include "prmon.h"
#include "nsCom.h"
#include "nsID.h"
#include "nsError.h"
#include "nsISupports.h"
#include "nsIFactory.h"
#include "nsHashtable.h"

/*
 * Prototypes for dynamic library export functions
 */

class nsIServiceManager;


//***********************************************************
//
// NSGetFactory should take nsISupports instead of nsIServiceManager
// as the second param. This is done on purpose!! Please don't change
// it. 
//       sudu / stanley
//
//***********************************************************
extern "C" NS_EXPORT nsresult NSGetFactory(const nsCID &aClass,
                                           nsISupports* serviceMgr,
                                           nsIFactory **aFactory);
extern "C" NS_EXPORT PRBool   NSCanUnload();
extern "C" NS_EXPORT nsresult NSRegisterSelf(const char *fullpath);
extern "C" NS_EXPORT nsresult NSUnregisterSelf(const char *fullpath);

/* Quick Registration
 *
 * For quick registration, dlls can define
 *		NSQuickRegisterClassData g_NSQuickRegisterData[];
 * and export the symbol "g_NSQuickRegisterData"
 *
 * Quick registration is tried only if the symbol "NSRegisterSelf"
 * is not found. If it is found but fails registration, quick registration
 * will not kick in.
 *
 * The array is terminated by having a NULL last element. Specifically, the
 * array will be assumed to end when
 *		(g_NSQuickRegisterData[i].classIdStr == NULL)
 *
 */
#define NS_QUICKREGISTER_DATA_SYMBOL "g_NSQuickRegisterData"

typedef struct NSQuickRegisterClassData {
	const char *CIDString;	// {98765-8776-8958758759-958785}
	const char *className;	// "Layout Engine"
	const char *progID;		// "Gecko.LayoutEngine.1"
} NSQuickRegisterClassData;

typedef NSQuickRegisterClassData* NSQuickRegisterData;

/* Autoregistration will try only files with these extensions.
  * All extensions are case insensitive.
 	".dll",	// Windows
	".dso",	// Unix
	".so",	// Unix
	".sl",	// Unix: HP
	"_dll",	// Mac
	".dlm",	// new for all platforms
*/

/*
 * Dynamic library export function types
 */

typedef nsresult (*nsFactoryProc)(const nsCID &aCLass,
                                  nsISupports* serviceMgr,
                                  nsIFactory **aFactory);
typedef PRBool (*nsCanUnloadProc)(void);
typedef nsresult (*nsRegisterProc)(const char *path);
typedef nsresult (*nsUnregisterProc)(const char *path);

/*
 * Support types
 */

class FactoryEntry;
class nsDllStore;
class nsDll;

enum NSRegistrationInstant
{
	NS_Startup = 0,
	NS_Script = 1,
	NS_Timer = 2
};

/* Error codes generated by the repository methods */

#define NS_XPCOM_ERRORCODE_IS_DIR	1

/* The separator for each path part in the pathlist for autoregistration */

#define NS_PATH_SEPARATOR	';'


/*
 * nsRepository class
 */

class NS_COM nsRepository {
private:

#define NS_MAX_FILENAME_LEN	1024

  static nsHashtable *factories;
  static PRMonitor *monitor;

  static nsresult checkInitialized(void);
  static nsresult loadFactory(FactoryEntry *aEntry, nsIFactory **aFactory);
  static nsresult SelfRegisterDll(nsDll *dll);
  static nsresult SelfUnregisterDll(nsDll *dll);

public:
  static nsDllStore *dllStore;

  static nsresult Initialize(void);
  // Finds a factory for a specific class ID
  static nsresult FindFactory(const nsCID &aClass,
                              nsIFactory **aFactory);

  // Finds a class ID for a specific Program ID
  static nsresult ProgIDToCLSID(const char *aProgID,
                              nsCID *aClass);

  // Creates a class instance for a specific class ID
  static nsresult CreateInstance(const nsCID &aClass, 
                                 nsISupports *aDelegate,
                                 const nsIID &aIID,
                                 void **aResult);

  // Creates a class instance for a specific class ID
  /*
  static nsresult CreateInstance2(const nsCID &aClass, 
                                  nsISupports *aDelegate,
                                  const nsIID &aIID,
                                  void *aSignature,
                                  void **aResult);
  */

  // Manually registry a factory for a class
  static nsresult RegisterFactory(const nsCID &aClass,
                                  nsIFactory *aFactory,
                                  PRBool aReplace);

  // Manually registry a dynamically loaded factory for a class
  static nsresult RegisterFactory(const nsCID &aClass,
                                  const char *aLibrary,
                                  PRBool aReplace,
                                  PRBool aPersist);

  // Manually register a dynamically loaded component.
  static nsresult RegisterComponent(const nsCID &aClass,
                                  const char *aClassName,
                                  const char *aProgID,
                                  const char *aLibrary,
                                  PRBool aReplace,
                                  PRBool aPersist);

  // Manually unregister a factory for a class
  static nsresult UnregisterFactory(const nsCID &aClass,
                                    nsIFactory *aFactory);

  // Manually unregister a dynamically loaded factory for a class
  static nsresult UnregisterFactory(const nsCID &aClass,
                                    const char *aLibrary);

  // Manually unregister a dynamically loaded component
  static nsresult UnregisterComponent(const nsCID &aClass,
                                    const char *aLibrary);

  // Unload dynamically loaded factories that are not in use
  static nsresult FreeLibraries(void);

  // DLL registration support
  static nsresult AutoRegister(NSRegistrationInstant when,
									const char* pathlist);
  // Pathlist is a semicolon separated list of pathnames
  static nsresult AddToDefaultPathList(const char *pathlist);
  static nsresult SyncComponentsInPathList(const char *pathlist);
  static nsresult SyncComponentsInDir(const char *path);
  static nsresult SyncComponentsInFile(const char *fullname);

};

#endif
