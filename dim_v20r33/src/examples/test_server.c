#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dis.h>

//variables
//int t_alarm=0;
//double percentage=30.;
//this file contain the running variables
char file[] ="/home/gem3/GasMon/Readout/logs/gemreadout.log";

typedef struct {
	double t_alarm;
	double percentage;
}GM_var;
GM_var gm;

GM_var get_vars(char path[]) {
	GM_var test_gm;

	//test_gm.t_alarm=0;
	//test_gm.percentage=0.;\

	//read the file in path up to the end
	//at the end of cycle the tmp variable will contain teh last row
	char tmp[1024];
	FILE *fp;
	fp = fopen(path, "r");
	while(!feof(fp))
		fgets(tmp, 1024, fp);
	//printf("%s", tmp);

	//strip the last row with delimiter to get every variable
	char *token = strtok(tmp, ",");
	int i=0;
	while( token != NULL ) {
		token = strtok(NULL, ",");
		//printf( "%d,  %s\n", i, token ); //printing each token
		if(i==20){
			//printf( "%d,  %s\n", i, token ); //printing each token
			test_gm.percentage=atof(token);
			//printf( "%d, %f,  %f\n",i, test_gm.t_alarm, test_gm.percentage );
		};
		if(i==23){
			//printf( "%d,  %s\n", i, token ); //printing each token
			test_gm.t_alarm=atof(token);
			//printf( "%d, %f,  %f\n",i, test_gm.t_alarm, test_gm.percentage );
			};
		i++;
	}
	return test_gm; // Return the local structure
} 

void cmnd_rout(int *tag, GM_var *buf, int *size)
{
	if (tag) {}
	printf("#################################\n");
	dim_print_date_time();
	printf("\nCommand received, size = %d, GM_var size = %d:\n", *size, (int)sizeof(GM_var));
	printf("buf->t_alarm = %f, buf->percentage = %f\n",
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
		//gm.t_alarm++;
		//gm.percentage++;
		gm=get_vars(file);

		dis_update_service(serv_alarm);
		printf("t_alarm = %f, percentage = %f\n", gm.t_alarm, gm.percentage);

		
		//printf("t_alarm = %d, percentage = %f\n", gm.t_alarm, gm.percentage);
		sleep(1);
	}
	return 1;

}