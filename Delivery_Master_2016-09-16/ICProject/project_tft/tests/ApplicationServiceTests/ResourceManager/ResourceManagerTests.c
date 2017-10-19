#include "check.h"
#include <stdio.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include "ResourceManager.h"

void CHKTestGranted(void);
void CHKTestDenied(void);

void MMTestGranted(void);
void MMTestDenied(void);

void DMTestGranted(void);
void DMTestDenied(void);

void resetResources(void);

static int iCHKGranted = 0;
static int iMMGranted = 0;
static int iDMGranted = 0;

START_TEST(test_RegisterClients)
{
	//Arrange
	resetResources();
	ResourceManager_Init();

	//Act
	RM_RegisterResourceClient(RM_CLIENT_MENUCONTROLLER, MMTestGranted,MMTestDenied);
    RM_RegisterResourceClient(RM_CLIENT_CHKMSGHANDLER, CHKTestGranted,CHKTestDenied);
    RM_RegisterResourceClient(RM_CLIENT_CONTENTSMANAGER, DMTestGranted,DMTestDenied);

	//Assert
	ck_assert(iCHKGranted == 0);
	ck_assert(iMMGranted == 0);
    ck_assert(iDMGranted == 0);
    

}
END_TEST

START_TEST(test_MM_RequestResource)
{
	//Arrange
	resetResources();
	ResourceManager_Init();

	//Act
	RM_RegisterResourceClient(RM_CLIENT_MENUCONTROLLER, MMTestGranted,MMTestDenied);
	RM_RequestResource(RM_CLIENT_MENUCONTROLLER,RM_PRIO_MENUCONTROLLER);

	//Assert
	ck_assert(iMMGranted == 1);
    
 

}
END_TEST

START_TEST(test_CHKMSG_RequestResource)
{
    
	//Arrange
	resetResources();
	ResourceManager_Init();

	//Act
	RM_RegisterResourceClient(RM_CLIENT_CHKMSGHANDLER, CHKTestGranted,CHKTestDenied);
	RM_RequestResource(RM_CLIENT_CHKMSGHANDLER,RM_PRIO_CHECKMESSAGEHANDLER);

	//Assert
	ck_assert(iCHKGranted == 1);
    
}
END_TEST

/* START_TEST(test_DM_RequestResource)
{
	//Arrange
	resetResources();
	ResourceManager_Init();

	//Act
	RM_RegisterResourceClient(RM_CLIENT_CONTENTSMANAGER, DMTestGranted,DMTestDenied);
	RM_RequestResource(RM_CLIENT_CONTENTSMANAGER, RM_PRIO_STARTUP);

	//Assert
	ck_assert(iCHKGranted == 1);
}
END_TEST
 */
START_TEST(test_CHKMSG_RequestResourceWhileOwningIt)
{
    
	//Arrange
    resetResources();
	ResourceManager_Init();
	RM_RegisterResourceClient(RM_CLIENT_CHKMSGHANDLER, CHKTestGranted,CHKTestDenied);
	RM_RequestResource(RM_CLIENT_CHKMSGHANDLER,RM_RESOURCE_DISPLAY);
    int iCHKGrantedArrange = iCHKGranted;
    
    

	//Act
    RM_RequestResource(RM_CLIENT_CHKMSGHANDLER,RM_PRIO_CHECKMESSAGEHANDLER);

	//Assert
	ck_assert(iCHKGrantedArrange == 1);
	ck_assert(iCHKGranted == 1);
    
}
END_TEST

START_TEST(test_RequestResourceInUseByOthers)
{
    
	//Arrange
	resetResources();
	ResourceManager_Init();
	RM_RegisterResourceClient(RM_CLIENT_CHKMSGHANDLER, CHKTestGranted,CHKTestDenied);
	RM_RegisterResourceClient(RM_CLIENT_MENUCONTROLLER, MMTestGranted,MMTestDenied);
    RM_RequestResource(RM_CLIENT_MENUCONTROLLER,RM_PRIO_MENUCONTROLLER);
    
	//Act
    RM_RequestResource(RM_CLIENT_CHKMSGHANDLER,RM_PRIO_CHECKMESSAGEHANDLER);
    
    //Assert
	ck_assert(iMMGranted == 0);
	ck_assert(iCHKGranted == 1);
    
}
END_TEST

START_TEST(test_CHKMSG_ReleaseResource)
{
    
	//Arrange
	resetResources();
	ResourceManager_Init();
	RM_RegisterResourceClient(RM_CLIENT_CHKMSGHANDLER, CHKTestGranted,CHKTestDenied);
	RM_RegisterResourceClient(RM_CLIENT_MENUCONTROLLER, MMTestGranted,MMTestDenied); 
	RM_RequestResource(RM_CLIENT_CHKMSGHANDLER,RM_PRIO_CHECKMESSAGEHANDLER);
    RM_RequestResource(RM_CLIENT_MENUCONTROLLER,RM_PRIO_MENUCONTROLLER);
    
    ck_assert(iCHKGranted == 1);

	//Act
	RM_ReleaseResource(RM_CLIENT_CHKMSGHANDLER);

	//Assert
	ck_assert(iCHKGranted == 0);
	ck_assert(iMMGranted == 1);
    
    

}
END_TEST

START_TEST(test_CHKMSG_take_resource)
{
    
	//Arrange
	resetResources();
	ResourceManager_Init();
	RM_RegisterResourceClient(RM_CLIENT_CHKMSGHANDLER, CHKTestGranted,CHKTestDenied);
	RM_RegisterResourceClient(RM_CLIENT_MENUCONTROLLER, MMTestGranted,MMTestDenied); 
    RM_RequestResource(RM_CLIENT_MENUCONTROLLER,RM_PRIO_MENUCONTROLLER);
    
    ck_assert(iMMGranted == 1);

	//Act
    RM_RequestResource(RM_CLIENT_CHKMSGHANDLER,RM_PRIO_CHECKMESSAGEHANDLER);
	

	//Assert
	ck_assert(iCHKGranted == 1);
	ck_assert(iMMGranted == 0);
    
    

}
END_TEST

START_TEST(test_DMHIGH_PRIO_Startup)
{
    
	//Arrange
	resetResources();
	ResourceManager_Init();
	RM_RegisterResourceClient(RM_CLIENT_CHKMSGHANDLER, CHKTestGranted,CHKTestDenied);
	RM_RegisterResourceClient(RM_CLIENT_MENUCONTROLLER, MMTestGranted,MMTestDenied); 
    RM_RegisterResourceClient(RM_CLIENT_CONTENTSMANAGER, DMTestGranted, DMTestDenied); 
    
    RM_RequestResource(RM_CLIENT_MENUCONTROLLER,RM_PRIO_MENUCONTROLLER);
    ck_assert(iMMGranted == 1);
    RM_RequestResource(RM_CLIENT_CHKMSGHANDLER,RM_PRIO_CHECKMESSAGEHANDLER);
    
    ck_assert(iMMGranted == 0);
    ck_assert(iCHKGranted == 1);

	//Act
    RM_RequestResource(RM_CLIENT_CONTENTSMANAGER,RM_PRIO_STARTUP);
	

	//Assert
    ck_assert(iCHKGranted == 0);
	ck_assert(iMMGranted == 0);
    ck_assert(iDMGranted == 1);
    
    

}
END_TEST

START_TEST(test_Startup_to_normal_to_no_request)
{
    
	//Arrange
	resetResources();
	ResourceManager_Init();
	RM_RegisterResourceClient(RM_CLIENT_CHKMSGHANDLER, CHKTestGranted,CHKTestDenied);
	RM_RegisterResourceClient(RM_CLIENT_MENUCONTROLLER, MMTestGranted,MMTestDenied); 
    RM_RegisterResourceClient(RM_CLIENT_CONTENTSMANAGER, DMTestGranted, DMTestDenied); 
    
    RM_RequestResource(RM_CLIENT_MENUCONTROLLER,RM_PRIO_MENUCONTROLLER);
    ck_assert(iMMGranted == 1);
    RM_RequestResource(RM_CLIENT_CHKMSGHANDLER,RM_PRIO_CHECKMESSAGEHANDLER);
    
    ck_assert(iMMGranted == 0);
    ck_assert(iCHKGranted == 1);

	//Act
    RM_RequestResource(RM_CLIENT_CONTENTSMANAGER,RM_PRIO_STARTUP);
    ck_assert(iCHKGranted == 0);
	ck_assert(iMMGranted == 0);
    ck_assert(iDMGranted == 1);
    
    RM_ReleaseResource(RM_CLIENT_CONTENTSMANAGER);
    
    ck_assert(iCHKGranted == 1);
	ck_assert(iMMGranted == 0);
    ck_assert(iDMGranted == 0);
    
    RM_ReleaseResource(RM_CLIENT_CHKMSGHANDLER);
    
    ck_assert(iCHKGranted == 0);
	ck_assert(iMMGranted == 1);
    ck_assert(iDMGranted == 0);
	
    
    RM_ReleaseResource(RM_CLIENT_MENUCONTROLLER);
   
	//Assert
    ck_assert(iCHKGranted == 0);
	ck_assert(iMMGranted == 0);
    ck_assert(iDMGranted == 0);
}
END_TEST

Suite * RM_suite(void)
{
    Suite* s;
    TCase* tc_Register;
	TCase* tc_MMRequest;
	TCase* tc_CHKRequest;
	TCase* tc_RequestResourceInUseByOthers;
	TCase* tc_CHKWhileOwning;
	TCase* tc_CHKRelease;
    TCase* tc_CHKTakeResource;
    TCase* tc_DM_Take_Resource;
    TCase* tc_Startup_to_none;

    s = suite_create("RM_TestSuite");

    /* Core test case */
    tc_Register = tcase_create("RegisterMMAndCHK");
	tc_MMRequest = tcase_create("MenuManagerRequestResource");
	tc_CHKRequest = tcase_create("CHKMsgHRequestResource");
	tc_RequestResourceInUseByOthers = tcase_create("RequestResourceInUseByOthers");
	tc_CHKWhileOwning = tcase_create("CHKInvalidRequest_ReRequest");
	tc_CHKRelease = tcase_create("CHKMsgReleaseResource");
    tc_CHKTakeResource = tcase_create("test_CHKMSG_take_resource");
    tc_DM_Take_Resource = tcase_create("test_DMHIGH_PRIO_Startup");
    tc_Startup_to_none = tcase_create("test_Startup_to_normal_to_no_request");

    tcase_add_test(tc_Register, test_RegisterClients);
	tcase_add_test(tc_MMRequest, test_MM_RequestResource);
	tcase_add_test(tc_CHKRequest, test_CHKMSG_RequestResource);
	tcase_add_test(tc_RequestResourceInUseByOthers, test_RequestResourceInUseByOthers);
	tcase_add_test(tc_CHKWhileOwning, test_CHKMSG_RequestResourceWhileOwningIt);
	tcase_add_test(tc_CHKRelease, test_CHKMSG_ReleaseResource);
    tcase_add_test(tc_CHKTakeResource, test_CHKMSG_take_resource);
    tcase_add_test(tc_DM_Take_Resource, test_DMHIGH_PRIO_Startup);
    tcase_add_test(tc_Startup_to_none,test_Startup_to_normal_to_no_request);

    suite_add_tcase(s, tc_Register);
	suite_add_tcase(s, tc_MMRequest);
	suite_add_tcase(s, tc_CHKRequest);
	suite_add_tcase(s, tc_RequestResourceInUseByOthers);
	suite_add_tcase(s, tc_CHKWhileOwning);
	suite_add_tcase(s, tc_CHKRelease);
    suite_add_tcase(s, tc_CHKTakeResource);
    suite_add_tcase(s, tc_DM_Take_Resource);
    suite_add_tcase(s, tc_Startup_to_none);

    return s;
}

int main(void)
{
	int number_failed;
    Suite *s;
    SRunner *sr;

    s = RM_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

}


void CHKTestGranted(void)
{
	iCHKGranted = 1;
  
}


void CHKTestDenied(void)
{
 	iCHKGranted = 0;
  
}

void MMTestGranted(void)
{
 	iMMGranted = 1;
}


void MMTestDenied(void)
{
 	iMMGranted = 0;
}

void resetResources()
{
	 MMTestDenied();
	 CHKTestDenied();
     DMTestDenied();
}

void DMTestGranted(void)
{
    iDMGranted = 1;
}
void DMTestDenied(void)
{
    iDMGranted = 0;
}

