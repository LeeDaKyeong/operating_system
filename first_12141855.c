#include <stdio.h>
#include <stdlib.h>
/*pthread 사용*/
#include <pthread.h>

#define LENGTH 9

int input[LENGTH][LENGTH]; // 9by9 input
int sub_input[LENGTH]; //3by3의 sub_input을 1by9로 flatten
int last_array[LENGTH]; //main thread에 반환되는 값을 저장할 array

void *tret = NULL; // exit의 parameter passing 

struct inputStruct 
{ 
	int start, end; 
};

void* findHigh(void *arr);
int findHigh2(int arr[LENGTH]);

int main()
{
	//Input Array를 입력받아 input 에 저장합니다.
	FILE *rfp = fopen("highest input.txt", "r");

	for(int i=0;i<LENGTH;i++)
	{
		for(int j=0;j<LENGTH;j++)
		{
			fscanf(rfp,"%d",&input[i][j]);
		}
	}
	
	int index_ = 0;
	for(int i = 0; i<LENGTH;i=i+3)
	{
		for(int j = 0;j<LENGTH;j=j+3)
		{
			// 3 by 3 grid를 sub_input에 저장합니다.
			sub_input[0] = input[i][j];
			sub_input[1] = input[i][j+1];
			sub_input[2] = input[i][j+2];
			sub_input[3] = input[i+1][j];
			sub_input[4] = input[i+1][j+1];
			sub_input[5] = input[i+1][j+2];
			sub_input[6] = input[i+2][j];
			sub_input[7] = input[i+2][j+1];
			sub_input[8] = input[i+2][j+2];

			// thread를 생성합니다.
			pthread_t thrd;

			// thread에 input parameter로 들어갈 구조체를 만듭니다
    		struct inputStruct splitArray;

    		splitArray.start = 0;
    		splitArray.end = 9;

			//스레드를 활성화시킵니다.
			pthread_create(&thrd, NULL, findHigh, &splitArray); 

			//스레드가 끝나기를 기다리며, return된 exit값을 last_array에 넣어줍니다.
    		pthread_join(thrd, &tret);
    		last_array[index_++] = (int)tret;

		}

	}	

	// 최종 highest값을 추출합니다.
	int high = findHigh2(last_array);
	printf("Highest number : %d\n", high);

}

//입력된 arr에서 high구하기
void* findHigh(void *arr)
{
	struct inputStruct *temp = (struct inputStruct*) arr;

	int start = temp->start,
        end = temp->end;

    int i = start;

	int high = sub_input[i];

	while(i<=end)
	{
		if(high<sub_input[i])
		{
			high = sub_input[i];
		}
		i++;
	}

	//printf("%d\n",min);
	pthread_exit((void *)high);
}

// 최종 main thread에 반환된 array의 highest값 return
int findHigh2(int arr[LENGTH])
{
	int high = arr[0];
	for(int i=1;i<LENGTH;i++)
	{
		if(high < arr[i])
		{
			high = arr[i];
		}
	}

	return high;
}

