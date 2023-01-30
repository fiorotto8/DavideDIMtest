#include <dic.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

//int t_alarm;
typedef struct {
	int t_alarm;
	double percentage;
}GM_var;

GM_var gm;

char client_str[80];
int no_link = -1;

void rout( tag, buf, size )
char *buf;
int *tag, *size;
{
	if(tag){}
	char node[128], str[256];
	int secs, millis;
	time_t tsecs;

	dic_get_dns_node(node);
	printf("#################################\n");
	printf("DNS node = %s\n",node);
	printf("size = %d\n",*size);

	memcpy(&gm, buf, (size_t)*size);
	printf("Alarm : %d, Percentage: %f\n", gm.t_alarm, gm.percentage);
	
	dic_get_timestamp(0, &secs, &millis);
	tsecs = secs;
	my_ctime(&tsecs, str, 128);
	str[(int)strlen(str)-1] = '\0';
	printf("timestamp = %s.%d\n",str,millis);
}

int main(int argc, char **argv)
{
	char aux[100];//storage for the strings to use
	char str[100];//storage for the strings to use
	int id=0;//to test, lets see what it does

	if(argc){}
	//int dic_cmnd_service( char *serv_name, void *serv_address, int serv_size )
	//non ho capito cosa faccia
	sprintf(str,"%s/SET_EXIT_HANDLER",argv[2]);
	dic_cmnd_service(str, &id, 4);
	
	//int dic_get_id(char *name)
	dic_get_id(aux);
	printf("ID: %s\n",aux);
	//give name to client
	strcpy(client_str,argv[1]);

	//not clear what it does
	sprintf(aux,"%s/GASMON",argv[2]);
	dic_info_service_stamped( aux, TIMED, 5, 0, 0, rout, 1200,
			  &no_link, 4 );

	//creating the command for the server
	sprintf(aux,"%s/GASMON_CMD",argv[2]);
	while(1)
	{
		sleep(1);
		//printf("Sending Command, alarm = %d\n", t_alarm);
		dic_cmnd_service(aux,&gm,(int)sizeof(gm));
	}
	return 1;
}

/*
char str[80];
char str_res[10][80];
char client_str[80];
int no_link = -1;
float no_link_float = -1.0;
char buff[80];

typedef struct {
	int i;
	int j;
	int k;
	double d;
	short s;
    char c;
	short t;
	float f;
	char str[20];
}TT;

TT t;

void big_rout( int *tag, int *buf, int *size )
{

	if(size){}
	printf("Received %d for TestMem_%d\n", *buf, *tag);
}


void got_servers( int *tag, char *list, int *size)
{
	if(tag){}
	if(size){}
	printf("%s",list);
}

void got_services( int *tag, char *list, int *size)
{
	if(tag){}
	if(size){}
	printf("%s",list);
}

void version_rout( int *tag, int *buf, int *size)
{
	if(tag){}
	printf("Received VERSION %lx, %d\n", (dim_long)buf, *size);
}

void rout( tag, buf, size )
char *buf;
int *tag, *size;
{
	if(*tag == 1100)
	{
		printf("Received ONCE_ONLY : %s\n",buff);
		return;
	}
	if(*tag == 1200)
	{
		char node[128], str[256];
		int secs, millis;
		time_t tsecs;

		dic_get_dns_node(node);
		printf("DNS node = %s\n",node);
		printf("size = %d\n",*size);
		memcpy(&t, buf, (size_t)*size);
		printf("t.i = %d, t.d = %2.2f, t.s = %d, t.c = %c, t.f = %2.2f, t.str = %s\n",
			t.i,t.d,t.s,t.c,t.f,t.str);
		dic_get_timestamp(0, &secs, &millis);
		tsecs = secs;
		my_ctime(&tsecs, str, 128);
		str[(int)strlen(str)-1] = '\0';
		printf("timestamp = %s.%d\n",str,millis);

		return;
	}
	else
		printf("%s Received %s for Service%03d\n",client_str,buf,*tag);

}

int main(int argc, char **argv)
{
	int i;
	char aux[80];
	int id = 123;

	if(argc){}
	//int dic_cmnd_service( char *serv_name, void *serv_address, int serv_size )
	//non ho capito cosa faccia
	sprintf(str,"%s/SET_EXIT_HANDLER",argv[2]);
	dic_cmnd_service(str, &id, 4);
	
	//int dic_get_id(char *name)
	dic_get_id(aux);
	printf("ID: %s\n",aux);
	//give name to client
	strcpy(client_str,argv[1]);

	for(i = 0; i< 10; i++)
	{
		sprintf(str,"%s/Service_%03d",argv[2],i);
		dic_info_service( str, TIMED, 10, 0, 0, rout, i,
			  "No Link", 8 );
	}
	
	sprintf(aux,"%s/TEST_SWAP",argv[2]);
	dic_info_service_stamped( aux, TIMED, 5, 0, 0, rout, 1200,
			  &no_link, 4 );

	sprintf(str,"%s/VERSION_NUMBER",argv[2]);
	dic_info_service( str, MONITORED, 0, 0, 0, version_rout, 0,
			  NULL, 0 );

	sprintf(aux,"%s/TEST_CMD",argv[2]);

	dim_long dnsid;
	dnsid = dic_add_dns("lxplus613.cern.ch", 0);
	for (i = 0; i< 10; i++)
	{
		sprintf(str, "%s1/Service_%03d", argv[2], i);
		dic_info_service_dns(dnsid, str, TIMED, 10, 0, 0, rout, i,
			"No Link", 8);
	}
	while(1)
	{
		sleep(1);
		printf("Sending Command, size = %d, i = %d\n",(int)sizeof(t), t.i);
		dic_cmnd_service(aux,&t,(int)sizeof(t));
	}
	return 1;
}
*/