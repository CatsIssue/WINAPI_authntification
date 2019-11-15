#define UNICODE
#define _UNICODE

#include <windows.h>
#include <winldap.h>
#include <winber.h>
	
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

	if (ldap_connect == LDAP_SUCCESS) 
	{
		printf("ldap_connect succeded\n");
	}
	else
	{
		printf("failed ldap_connect: 0x%0lu\n", ldap_return); 
	
	}
	
	// BIND with using some credentials

	PCHAR p_my_DN = " ";


	ldap_return = ldap_bind_s( 
			p_LDAP_connection,
			p_my_DN,
			(PCHAR) security_identification,
			LDAP_AUTH_NEGOTIATE);

	if (ldap_return == LDAP_SUCCESS)
	{
		
	}
	else
	{

	}
}

