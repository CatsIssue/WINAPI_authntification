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

	if (ldap_return == LDAP_SUCCESS) 
	{
		printf("ldap_connect succeded\n");
	}
	else
	{
		printf("failed ldap_connect: 0x%0lu\n", ldap_return); 
	
	}
	
	// BIND with using some credentials
	/*
	PCHAR p_my_DN = "cn=users,dc=etersoft,dc=ru";
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

	ldap_return = ldap_bind_s( 
			p_LDAP_connection,
			NULL,
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

}

