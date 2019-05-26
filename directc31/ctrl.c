#include "stdio.h"
#include "stdlib.h"
#include <sys/shm.h>

int	User_No;

struct shared_use_st
{
  int op_keyin;         // ��Ϊһ����־�����ӽ��̾��ɶ�д
  char action_state[8]; // �����̶����ӽ���д
};

int main(int argc,char *argv[])
{

	void *shm = NULL;
	struct shared_use_st *shared = NULL;
	int i,shmid;
	//���������ڴ�
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);
	if( shmid == -1 )
	{
		printf("shmget failed\n");
		return -1;
	}
	//�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ�
	shm = shmat(shmid, (void*)0, 0);
	if( shm == (void*)-1 )
	{
		printf("shmat failed\n");
		return -1;
	}
	//���ù����ڴ�
	shared = (struct shared_use_st*)shm;

	User_No = atoi(argv[1]);

	shared->op_keyin = 1<<User_No;
	if( User_No == 0xff ) shared->op_keyin = 0xffff;
	//shared->action_state[User_No] = 0;
	while(1)
	{
		printf("%d==>", shared->op_keyin);
		for(i=0;i<6;i++) printf("%3d ",shared->action_state[i]);
		printf("\r");
	}
	return 0;
}
