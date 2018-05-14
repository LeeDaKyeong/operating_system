#include <stdio.h>
#include <stdlib.h>
/*pthread 사용*/
#include <pthread.h>

//배열의 길이 설정. 여기서는 10
#define LENGTH 10

int input[LENGTH];
int last_array[3]; // 세 쓰레드에서 작은 값 저장
void *tret = NULL; // exit의 parameter passing 

void getUnsorted();
void printInputArray(int len);
void *findMin(void *data);

struct inputStruct 
{ 
	int start, end; 
};

int main()
{
	//Input Array를 입력받아 input 에 저장합니다.
	FILE *rfp = fopen("input.txt", "r");
	fscanf(rfp, "%d %d %d %d %d %d %d %d %d %d", &input[0], &input[1], &input[2], &input[3], &input[4], &input[5], &input[6], &input[7], &input[8], &input[9]);

	//세 개의 스레드를 나눌 구간을 설정합니다.
	int start1 = 0,
    	end1 = start1+2,
    	start2 = end1+1,
        end2 = start2+2,
        start3 = end2+1,
        end3 = LENGTH-1;

    //총 3개의 스레드를 생성합니다.
    pthread_t thrd[3];

    //thread에 input parameter로 들어갈 구조체 3개를 만듭니다
    struct inputStruct splitArray[3];

    //첫번째 구조체의 시작점과 끝점을 각각 0,2로 할당
    splitArray[0].start = start1;
    splitArray[0].end = end1;

    //두번째 구조체의 시작점과 끝점을 각각 3,5로 할당
    splitArray[1].start = start2;
    splitArray[1].end = end2;

    //두번째 구조체의 시작점과 끝점을 각각 6,9로 할당
    splitArray[2].start = start3;
    splitArray[2].end = end3;

    //스레드 세개를 활성화시킵니다.
    pthread_create(&thrd[0], NULL, findMin, &splitArray[0]); 
    pthread_create(&thrd[1], NULL, findMin, &splitArray[1]); 
    pthread_create(&thrd[2], NULL, findMin, &splitArray[2]); 

    //세개의 스레드가 끝나기를 기다리며, return된 exit값을 last_array에 넣고, 최종 min을 구합니다.
    pthread_join(thrd[0], &tret);
    last_array[0] = (int)tret;
    pthread_join(thrd[1], &tret);
    last_array[1] = (int)tret;
    pthread_join(thrd[2], &tret);
    last_array[2] = (int)tret;
	
	int min = last_array[0];
	for(int i = 1;i<3;i++)
	{
		if(min>last_array[i])
		{
			min = last_array[i];
		}
	}

	FILE *wfp = fopen("output.txt", "w");
	fprintf(wfp, "%d\n", min);
	printf("Lowest: %d\n", min);

}

//Input 받은 배열을 출력
void printInputArray(int len)
{
    int i;
    printf("___Input Array___: ");
    for (i=0; i<len; i++)
        printf("%d ", input[i]);
    printf("\n");
}

//입력된 arr에서 min구하기
void* findMin(void *arr)
{
	struct inputStruct *temp = (struct inputStruct*) arr;

	int start = temp->start,
        end = temp->end;

    int i = start;

	int min = input[i];

	while(i<=end)
	{
		if(min>input[i])
		{
			min = input[i];
		}
		i++;
	}

	//printf("%d\n",min);
	pthread_exit((void *)min);
}




