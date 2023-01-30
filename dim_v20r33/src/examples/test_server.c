#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dis.h>

//variables
//int t_alarm=0;
//double percentage=30.;

typedef struct {
	int t_alarm;
	double percentage;
}GM_var;

GM_var gm;


void cmnd_rout(int *tag, GM_var *buf, int *size)
{
	if (tag) {}
	printf("#################################\n");
	dim_print_date_time();
	printf("\nCommand received, size = %d, GM_var size = %d:\n", *size, (int)sizeof(GM_var));
	printf("buf->t_alarm = %d, buf->percentage = %f\n",
			buf->t_alarm,buf->percentage);
}

void client_exited(int *tag)
{
	char name[84];

	if(dis_get_client(name))
		printf("Client %s (%d) exited\n", name, *tag);
	else
		printf("Client %d exited\n", *tag);
}

void exit_cmnd(int *code)
{
	printf("Exit_cmnd %d\n", *code);
	exit(*code);
}

int main(int argc, char **argv)
{
	if(argc){}//for now just to hinibit the warning on unused variable
	int serv_alarm;
	char aux[100];//storage for the strings to use
	char name[84];//client name
	char srvname[64];//server name

	//setting server name
	strcpy(srvname, argv[1]);

	//print date and starting up
	dim_print_date_time();
	printf("Dim Server Starting up...\n");
	fflush(stdout);

	//handeler for exit comand and exited status
	dis_add_exit_handler(exit_cmnd);
	dis_add_client_exit_handler(client_exited);

	//ADD SERVICE METHOD
	//dis_add_service( char *name, char *type, void *address, int size, 
	//					 void (*user_routine)(), dim_long tag)
	//creating the service
	
	snprintf(aux, sizeof(aux),"%s/GASMON",srvname);
	serv_alarm=dis_add_service( aux, "d:1;d:1", &gm, sizeof(gm), 
		(void *)0, 0 );

	gm.t_alarm = 0;
	gm.percentage = 30.;

	//ADD COMMAND
	//unsigned dis_add_cmnd( char *name, char *type, void (*user_routine)(), dim_long tag ) 
	//creating the command
	snprintf(aux, sizeof(aux), "%s/GASMON_CMD", srvname);
	dis_add_cmnd(aux, "X", cmnd_rout, 0);
	
	//sart serving
	dis_start_serving( argv[1] );

	//getting client name
	if(dis_get_client(name))
	{
		printf("client %s\n",name);
	}
	
	while(1)
	{
		gm.t_alarm++;
		gm.percentage++;
		dis_update_service(serv_alarm);
		//printf("t_alarm = %d, percentage = %f\n", gm.t_alarm, gm.percentage);
		sleep(5);
	}
	return 1;

}






/*
char str[10][80];

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


int big_buff[1024];

int testDimInt = 0;
int testDimLong = 0;
longlong testDimXLong = 0;


void cmnd_rout(int *tag, TT *buf, int *size)
{
	if(tag){}
	dim_print_date_time();
	printf("Command received, size = %d, TT size = %d:\n", *size,
	       (int)sizeof(TT));
	printf("buf->i = %d, buf->d = %2.2f, buf->s = %d, buf->c = %c, buf->f = %2.2f, buf->str = %s\n",
			buf->i,buf->d,buf->s,buf->c,buf->f,buf->str);
}

void cmnd_rout_long(int *tag, longlong *buf, int *size)
{

	if (tag) {}
	dim_print_date_time();
	printf("Command received, size = %d\n", *size);
	printf("contents = %lld, %llx\n", *buf, *buf);
}

void client_exited(int *tag)
{
	char name[84];

	if(dis_get_client(name))
		printf("Client %s (%d) exited\n", name, *tag);
	else
		printf("Client %d exited\n", *tag);
}

void exit_cmnd(int *code)
{
	printf("Exit_cmnd %d\n", *code);
	exit(*code);
}

int NewData;
int NewIds[11];

int more_ids[1024];
int curr_more_index = 0;
char more_str[1024][80];

int main(int argc, char **argv)
{
	int i, id, xlong_id, int_id;
	char aux[80];
	char name[84];
	char srvname[64];

	dim_long dnsid = 0;
	char extra_dns[128];
	int new_dns = 0;
	int index = 0;

	//rpint data and starting up
	dim_print_date_time();
	printf("Dim Server Starting up...\n");
	fflush(stdout);
	//this ad an extra DNS if needed (proably not needed)
	if(argc){}
	new_dns = dim_get_env_var("EXTRA_DNS_NODE", extra_dns, sizeof(extra_dns));
	if(new_dns)
		dnsid = dis_add_dns(extra_dns,0);
	if(dnsid){}

	//handeler for exit comand and exited status
	dis_add_exit_handler(exit_cmnd);
	dis_add_client_exit_handler(client_exited);

	//setting server name
	strcpy(srvname, argv[1]);

	//ADD SERVICE METHOD
	//dis_add_service( char *name, char *type, void *address, int size, 
	//					 void (*user_routine)(), dim_long tag)

	for(i = 0; i< 10; i++)
	{
		snprintf(str[i],sizeof(str[i]),"%s/Service_%03d",srvname,i);
		dis_add_service( str[i], "C", str[i], (int)strlen(str[i])+1, 
		(void *)0, 0 );
	}
	t.i = 123;
	t.j = 456;
	t.k = 789;
	t.d = 56.78;
	t.s = 12;
	t.t = 12;
	t.c = 'a';
	t.f = (float)4.56;
	strcpy(t.str,"hello world");

	snprintf(aux, sizeof(aux),"%s/TEST_SWAP",srvname);
	id = dis_add_service( aux, "l:3;d:1;s:1;c:1;s:1;f:1;c:20", &t, sizeof(t), 
		(void *)0, 0 );

	snprintf(aux, sizeof(aux),"%s/TEST_SWAP1",srvname);
	id = dis_add_service( aux, "l:3;d:1;s:1;c:1;s:1;f:1;c:20", &t, sizeof(t), 
		(void *)0, 0 );

	//ADD COMMAND
	//unsigned dis_add_cmnd( char *name, char *type, void (*user_routine)(), dim_long tag ) 
	//the user_routine here is basically for prining out

	if(id){}
	snprintf(aux, sizeof(aux),"%s/TEST_CMD",srvname);
	dis_add_cmnd(aux,"l:3;d:1;s:1;c:1;s:1;f:1;c:20",cmnd_rout, 0);

	snprintf(aux, sizeof(aux), "%s/Service_DimInt", srvname);
	int_id = dis_add_service(aux, "I", &testDimInt, sizeof(testDimInt),
		(void *)0, 0);
	snprintf(aux, sizeof(aux), "%s/Service_DimLong", srvname);
	dis_add_service(aux, "L", &testDimLong, sizeof(testDimLong),
		(void *)0, 0);
	snprintf(aux, sizeof(aux), "%s/Service_DimXLong", srvname);
	xlong_id = dis_add_service(aux, "X", &testDimXLong, sizeof(testDimXLong),
		(void *)0, 0);

	snprintf(aux, sizeof(aux), "%s/TEST_XCMD", srvname);
	dis_add_cmnd(aux, "X", cmnd_rout_long, 0);
	
	dis_start_serving( argv[1] );

	if(dis_get_client(name))
	{
		printf("client %s\n",name);
	}

	while(1)
	{
		index++;
		testDimXLong++;
		dis_update_service(xlong_id);
		testDimInt = -index;
		dis_update_service(int_id);

		sleep(1);
	}
	return 1;
}
*/
