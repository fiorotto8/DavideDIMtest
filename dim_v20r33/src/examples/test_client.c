#include <dic.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

//int t_alarm;
typedef struct {
	double t_alarm;
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
	printf("Alarm : %f, Percentage: %f\n", gm.t_alarm, gm.percentage);
	
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
