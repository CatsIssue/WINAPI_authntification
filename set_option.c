#define UNICODE
#define _UNICODE

#include <windows.h>
#include <winldap.h>
#include <winber.h>
#include <string.h> 
	
#include <stdio.h>
#include <stdlib.h>
#include <windef.h> 

int main (int argc, char **argv)
{
	// INIT LDAP session
	WCHAR p_host_name[] = {'d','c','.','e','t','e','r','s','o','f','t','.','r','u',0};

	LDAP * p_LDAP_connection = NULL;

	p_LDAP_connection = ldap_init(p_host_name, LDAP_PORT);
	
	if (p_LDAP_connection == NULL) 
	{
		printf("ERROR ldap_init: 0x%lu\n", LdapGetLastError());
		ldap_unbind(p_LDAP_connection);
	}
	else
	{
		printf("SUCCESS in ldap_init\n");
	}


	// Set up session vars and options
	ULONG version = LDAP_VERSION3;
	ULONG number_of_returns = 10;
	ULONG ldap_return = 0;

	// set options
	ldap_return = ldap_set_option( 
			p_LDAP_connection,
			LDAP_OPT_PROTOCOL_VERSION,
			(void *)&version);
	
	if (ldap_return == LDAP_SUCCESS) 
	{
		printf("ldap version 3.0.\n");
	}
	else
	{
		printf("Set option ERROR: %0lX\n", ldap_return);
		ldap_unbind(p_LDAP_connection);
	}
	
	ldap_return = ldap_connect(p_LDAP_connection, NULL);

	if (ldap_return == LDAP_SUCCESS) 
	{
		printf("ldap_connect succeded\n");
	}
	else
	{
		printf("failed ldap_connect: 0x%0lu\n", ldap_return); 
	
	}

	// BIND with using some credentials

	const PCHAR pMyDN = "cn=users,dc=etersoft,dc=ru";
	/*
	SEC_WINNT_AUTH_IDENTITY security_identity;
	PCHAR p_user_name = "konstantin";
	PCHAR p_password  = " ";
	

	security_identity.User = (unsigned char*)p_user_name;
	security_identity.UserLength = strlen(p_user_name);
	security_identity.Password = (unsigned char*)p_password;
	security_identity.PasswordLength = strlen(p_password);
	security_identity.Domain = (unsigned char*)p_host_name;
	security_identity.DomainLength = strlen(p_host_name);
	security_identity.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
	*/

	ldap_return = ldap_bind_sA( 
			p_LDAP_connection,
			pMyDN,
			NULL, 			   // credential structure 
			LDAP_AUTH_NEGOTIATE);      // mode of authentification


	if (ldap_return == LDAP_SUCCESS)
	{
		printf("ldap_bind_s success\n");
		
	}
	else
	{
		printf("ERROR ldap_bind_s: 0x%0lu\n", ldap_return);
	}

	



    ULONG errorCode = LDAP_SUCCESS;
    LDAPMessage* pSearchResult;
    PCHAR pMyFilter = "(sAMAccountName=konstantin)";
    PCHAR pMyAttributes[] = { 
	"cn",
	"dn",
	"sAMAccountName",
	"sAMAccountType",
	"userPrincipalName",
	"objectSid",
	"objectGUID",
	NULL
    };

    
    errorCode = ldap_search_sA(
                    p_LDAP_connection,    	// Session handle
                    pMyDN,              	// DN to start search
                    LDAP_SCOPE_SUBTREE, 	// Scope
              	    pMyFilter,          	// Filter
                    pMyAttributes,      	// Retrieve list of attributes
                    0,                  	// Get both attributes and values
                    &pSearchResult);    		// [out] Search results
    
    if (errorCode != LDAP_SUCCESS)
    {
        printf("ERROR ldap_search_s failed with 0x%0lx \n",errorCode);
        ldap_unbind_s(p_LDAP_connection);
        if(pSearchResult != NULL)
            ldap_msgfree(pSearchResult);
    }
    else
        printf("SUCCESS ldap_search\n");
    
    //----------------------------------------------------------
    // Get the number of entries returned.
    //----------------------------------------------------------
    
    ULONG numberOfEntries;
    
    numberOfEntries = ldap_count_entries(
                        p_LDAP_connection,    // Session handle
                        pSearchResult);     // Search result
    
    if(numberOfEntries == 0)
    {
        printf("ldap_count_entries failed with 0x%0lx \n",errorCode);
        ldap_unbind_s(p_LDAP_connection);
        if(pSearchResult != NULL)
            ldap_msgfree(pSearchResult);
    }
    else
        printf("ldap_count_entries succeeded \n");
    
    printf("The number of entries is: %lu \n", numberOfEntries);
    
    
    //----------------------------------------------------------
    // Loop through the search entries, get, and output the
    // requested list of attributes and values.
    //----------------------------------------------------------
    
    LDAPMessage* pEntry = NULL;
    PCHAR pEntryDN = NULL;
    ULONG iCnt;
    char* sMsg;
    BerElement* pBer = NULL;
    PCHAR pAttribute = NULL;
    PCHAR* ppValue = NULL;
    ULONG iValue = 0;
    
    for( iCnt = 0; iCnt < numberOfEntries; ++iCnt )
    {
        // Get the first/next entry.
        if( !iCnt )
            pEntry = ldap_first_entry(p_LDAP_connection, pSearchResult);
        else
            pEntry = ldap_next_entry(p_LDAP_connection, pEntry);
        
        // Output a status message.
        sMsg = (!iCnt ? "ldap_first_entry" : "ldap_next_entry");
        if( pEntry == NULL )
        {
            printf("%s failed with 0x%0lx \n", sMsg, LdapGetLastError());
            ldap_unbind_s(p_LDAP_connection);
            ldap_msgfree(pSearchResult);
        }
        else
            printf("%s SUCCEEDED \n",sMsg);
        
        // Output the entry number.
        printf("ENTRY NUMBER %lu \n", iCnt);
                
        // Get the first attribute name.
        pAttribute = ldap_first_attributeA(
                      p_LDAP_connection,   // Session handle
                      pEntry,            // Current entry
                      &pBer);            // [out] Current BerElement

        
        // Output the attribute names for the current object
        // and output values.
        while(pAttribute != NULL)
        {
            // Output the attribute name.
            printf("     ATTR: %s",pAttribute);
            
            // Get the string values.

            ppValue = ldap_get_valuesA(
                          p_LDAP_connection,  // Session Handle
                          pEntry,           // Current entry
                          pAttribute);      // Current attribute

            // Print status if no values are returned (NULL ptr)
            if(ppValue == NULL)
            {
                printf(": [NO ATTRIBUTE VALUE RETURNED]");
            }
	
            // Output the attribute values
            else
            {
                iValue = ldap_count_valuesA(ppValue);
                if(!iValue)
                {
                    printf(": [BAD VALUE LIST]");
                }
                else
                {

                    // Output the first attribute value
		    if (!strcmp(pAttribute, "objectGUID"))
		    {
			GUID* guid_pointer = (GUID*)ppValue;
			printf(": %0lx-%0x-%0x",  guid_pointer->Data1, guid_pointer->Data2, guid_pointer->Data3 );
			for(int i = 0; i < 8; ++i)
			{
				printf("-%0x", guid_pointer->Data4[i]);
			}

		    }
		    else if (!strcmp(pAttribute, "objectSid")) 
		    {
			SID* sid_pointer = (SID*)ppValue;	
			printf(": %d-%d-%d", (unsigned int)sid_pointer->Revision, (unsigned int)sid_pointer->SubAuthorityCount, (unsigned int)(sid_pointer->IdentifierAuthority).Value);
			for(int i = 0; i < 5; ++i)
                        {
                                printf("-%0x", sid_pointer->SubAuthority[i]);
                        }

		    }
		    else{
		   	 printf(": %s", *ppValue);
		    }
                    // Output more values if available
                   	 ULONG z;
		   
		    

                    for(z=1; z<iValue; z++)
                    {
                        printf(", %s", ppValue[z]);
                    }
                }
            } 

            // Free memory.
            if(ppValue != NULL)  
                ldap_value_freeA(ppValue);
            ppValue = NULL;
            ldap_memfreeA(pAttribute);
            
            // Get next attribute name.
            pAttribute = ldap_next_attributeA(
                            p_LDAP_connection,   // Session Handle
                            pEntry,            // Current entry
                            pBer);             // Current BerElement
            printf("\n");
        }
        
        if( pBer != NULL )
            ber_free(pBer,0);
        pBer = NULL;
    }
    
    //----------------------------------------------------------
    // Normal cleanup and exit.
    //----------------------------------------------------------
    ldap_unbind(p_LDAP_connection);
    ldap_msgfree(pSearchResult);
    ldap_value_freeA(ppValue);
    return 0;
}

