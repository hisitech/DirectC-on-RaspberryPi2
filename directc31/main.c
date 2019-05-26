#include "stdio.h"
#include "stdlib.h"
#include <sys/shm.h>
#include "Release/dpuser.h"
#include "Release/dpalg.h"
#include <wiringPi.h>

#define Max_User_No	4	// Can be 6

// ============= ProgramSM
#define ProgramSM_None    0x0000
#define ProgramSM_Working 0x0001
#define ProgramSM_OK      0x0002
#define ProgramSM_Err     0x0003

int User_No,ProgramSM;
int jtck_gpio,jtdi_gpio,jtms_gpio,jtdo_gpio;

struct shared_use_st
{
  int op_keyin;         // 作为一个标志，父子进程均可读写
  char action_state[8]; // 父进程读，子进程写
};

unsigned char *data_buffer = NULL;

int main(int argc,char *argv[])
{
	FILE	*fp = NULL;
	int	file_size,n;
	DPINT	iExecResult = DPE_SUCCESS;
	void *shm = NULL;
	struct shared_use_st *shared = NULL;

	int shmid;
	//创建共享内存
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);
	if( shmid == -1 )
	{
		printf("shmget failed\n");
		return -1;
	}
	//将共享内存连接到当前进程的地址空间
	shm = shmat(shmid, (void*)0, 0);
	if( shm == (void*)-1 )
	{
		printf("shmat failed\n");
		return -1;
	}
	//设置共享内存
	shared = (struct shared_use_st*)shm;

	if( argc != 4 )
	{
		printf("Useage: $ %s <User_No> <Action_code> <File_name>\n",argv[0]);
		return -1;
	}

	User_No = atoi( argv[1] );
	if( User_No < 1 || User_No > Max_User_No )
	{
		printf("User_No out of range!(1-%d)\n",Max_User_No);
		return -1;
	}
	switch( User_No )
	{
		case 1:
			jtck_gpio = 22;
			jtdi_gpio = 23;
			jtms_gpio = 24;
			jtdo_gpio = 25;
			break;
		case 2:
			jtck_gpio = 26;
			jtdi_gpio = 27;
			jtms_gpio = 28;
			jtdo_gpio = 29;
			break;
		case 3:
			jtck_gpio = 6;
			jtdi_gpio = 10;
			jtms_gpio = 11;
			jtdo_gpio = 31;
			break;
		case 4:
			jtck_gpio = 0;
			jtdi_gpio = 2;
			jtms_gpio = 3;
			jtdo_gpio = 21;
			break;
		case 5:
			jtck_gpio = 7;
			jtdi_gpio = 1;
			jtms_gpio = 4;
			jtdo_gpio = 5;
			break;
		case 6:
			jtck_gpio = 8;
			jtdi_gpio = 12;
			jtms_gpio = 13;
			jtdo_gpio = 14;
			break;
		default:
			jtck_gpio = 22;
			jtdi_gpio = 23;
			jtms_gpio = 24;
			jtdo_gpio = 25;
			break;
	}

	fp = fopen(argv[3],"rb");
	if( fp == NULL )
	{
		printf("File %s Open Error!",argv[3]);
		fclose(fp);
		return -1;
	}
	fseek(fp,0,SEEK_END);
	file_size = ftell(fp);
	fseek(fp,0,SEEK_SET);
	printf("File size = %d\n",file_size);
	data_buffer = malloc(file_size);
	if( data_buffer == NULL )
	{
		printf("Memory malloc() Error!");
		free(data_buffer);
		fclose(fp);
		return -1;
	}
	n = fread(data_buffer,1,file_size,fp);
	fclose(fp);
	if( n != file_size )
	{
		printf("File read error!");
		free(data_buffer);
		return -1;
	}

	shared->op_keyin = 0;
	shared->action_state[User_No] = 0;
	ProgramSM = ProgramSM_None;

	Action_code = atoi( argv[2] );
	printf("Action_code = %d ---->",Action_code);

	wiringPiSetup();
	pinMode( jtck_gpio, OUTPUT );//TCK
	pinMode( jtdi_gpio, OUTPUT );//TDI
	pinMode( jtms_gpio, OUTPUT );//TMS
	pinMode( jtdo_gpio, INPUT );// TDO

	image_buffer = data_buffer;
	//Action_code = DP_DEVICE_INFO_ACTION_CODE;
	while(1)
	{
		while( ( shared->op_keyin && ( 1<<User_No ) ) == 0 );
		if( shared->op_keyin == 0xffff )
		{
			shared->action_state[User_No] = 0xff;
			break;
		}
		ProgramSM = ProgramSM_Working;
		shared->op_keyin &= ~( 1<<User_No );
		iExecResult = dp_top();
		if( iExecResult == 0 ) ProgramSM = ProgramSM_OK;
			else ProgramSM = ProgramSM_Err;
		shared->action_state[User_No] = ProgramSM;
		shared->op_keyin &= ~( 1<<( User_No ) );
		printf("\nResult = %d\n",iExecResult);
	}

	free(data_buffer);
	return (iExecResult);
}
