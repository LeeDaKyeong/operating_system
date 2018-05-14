#include <iostream> 
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


// vector 원소 프린트
void vec_print(vector<int>&vec)
{
  for(int i=0;i<vec.size();i++)
    cout << vec[i] << " ";
  cout << "\n";
  return;
}

void vec_print(vector<float>&vec)
{
  for(int i=0;i<vec.size();i++)
    cout << vec[i] << " ";
  cout << "\n";
  return;
}

// vector에서 남은 수행횟수가 0이되면 삭제
vector<int> vec_remove(vector<int>&vec)
{
  for(int i=2;i<vec.size();i+=3)
  {
    if(vec[i] == 0)
    {
      vec.erase(vec.begin()+i-2);
      vec.erase(vec.begin()+i-2);
      vec.erase(vec.begin()+i-2);
    }
  }

  return vec;
}
// RR scheduling
void RR_print(vector<int>&RR)
{

  int min;

  while(!RR.empty())
  {
    // find min priority(우선순위 높은 process 찾기)
    min = RR[0];
    for(int i=0;i<RR.size()-2;i+=3)
    {
      if(RR[i] < min)
      {
        min = RR[i];
      }
    }

    // 같은 priority 존재 한다면 해당 index 저장
    vector<int>index;
    for(int i=0;i<RR.size()-2;i+=3)
    {
      if(RR[i] == min)
        index.push_back(i);
    }

    // 같은 priority에 해당하는 모든 procese RR -> temp
    vector<int>temp;   
    vector<int>::iterator iter=index.begin(); 
    for(iter = index.begin(); iter != index.end(); ++iter)
    {
      temp.push_back(RR[*iter]);
      temp.push_back(RR[*iter+1]);
      temp.push_back(RR[*iter+2]);
    }

    // RR에서는 삭제
    iter=index.end();
    for(iter = index.end()-1; iter >= index.begin(); --iter)
    {
      RR.erase(RR.begin()+ *iter);
      RR.erase(RR.begin()+ *iter);
      RR.erase(RR.begin()+ *iter);
    }

    // temp 가 빌 때 까지 수행
    while(!temp.empty())
    {
      for(int i=2;i<temp.size();i+=3)
      {
        if(temp[i] >= 2) // 남은 수행이 2 이상이면 두 번 수행
        {
          cout << temp[i-2] << " " << temp[i-1] << " ";
          cout << temp[i-2] << " " << temp[i-1] << " ";
          temp[i] = temp[i] - 2;
          temp = vec_remove(temp);
        }
        else if(temp[i] == 1) // 남은 수행이 1이면 한 번만 수행
        {
          cout << temp[i-2] << " " << temp[i-1] << " ";
          temp[i] = temp[i] - 1;
          temp = vec_remove(temp);
        }
      }
    }
    cout << "\n";
  }
  
  return;
}

// vruntime 계산
float cal_vruntime(int weight, int delta)
{
  return ((1024/(float)weight)*(float)delta);
}

int cal_weight(int priority)
{
  if(priority>= 100 and priority <= 109)
    return 10;
  else if(priority>=110 and priority <= 119)
    return 8;
  else if(priority >= 120 and priority <= 129)
    return 6;
  else if(priority >= 130 and priority <= 139)
    return 4;
  return 0;
}

// 같은 process 한 줄에 print
void print_result(vector<int>&result)
{
  cout << result[0] << " " << result[1] << " ";
  for(int i = 2; i<result.size();i++)
  {
    if(result[i-2] == result[i])
    {
      cout << result[i] << " ";
    }
    else
    {
      cout << endl;
      cout << result[i] << " ";
    }
  }
}

// Conventional scheduling
void C_print(vector<int>&C)
{
  // scheduling 결과를 저장할 vector
  vector<int>result; 

  // C를 priority가 높은 순서대로 정렬
  vector<int>pri_temp; 

  while(!C.empty())
  {
    int min = C[0];
    int index = 0;
    for(int i=0;i<C.size();i+=3)
    {
      if(C[i] < min)
      {
        min = C[i];
        index = i;
      }
    }
    pri_temp.push_back(C[index]);
    pri_temp.push_back(C[index+1]);
    pri_temp.push_back(C[index+2]);

    C.erase(C.begin()+index);
    C.erase(C.begin()+index);
    C.erase(C.begin()+index);
  }

  // 수행시간 delta 계산하기 위해 미리 저장
  vector<int>num;
  for(int i=2;i<pri_temp.size();i+=3)
  {
    num.push_back(pri_temp[i]);
  }


  while(1)
  {
    int sum = 0;
    for(int i=2;i<pri_temp.size();i+=3)
    {
      sum = sum + pri_temp[i];
    }
    if(sum == 0) //pri_temp의 모든 process 수행 완료
      break;

    vector<float>vruntime(num.size(),0);

    for(int i=0;i<num.size();i++)
    {
      int weight = cal_weight(pri_temp[i*3]);
      int delta = num[i]-pri_temp[(i*3)+2];

      if(delta == num[i]) // 해당 process 수행 완료
        vruntime[i] = 10000;
      else if(delta < num[i])
        vruntime[i] = cal_vruntime(weight,delta);
    }

    // vruntime이 min인 process find
    float min = vruntime[0];
    vector<int>index;

    for(int i=0;i<vruntime.size();i++)
    {
      if(vruntime[i] < min)
        min = vruntime[i];
    }
    for(int i=0;i<vruntime.size();i++)
    {
      if(vruntime[i] == min)
        index.push_back(i);
    } 


    // 수행되지 않은 프로세스가 하나만 있을 때, 그 process만 쭉 실행
    if(index.size() == 1)
    {
      result.push_back(pri_temp[index[0]*3]);
      result.push_back(pri_temp[index[0]*3 + 1]);
      pri_temp[index[0]*3 + 2] = pri_temp[index[0]*3 + 2] -1;
    }
    else
    {
      // 같은 vruntime일 때는 이전에 수행하지 않았거나, 우선순위가 높은것이 먼저 수행
      // 처음 수행
      if(result.size() == 0)
      {
        result.push_back(pri_temp[0]);
        result.push_back(pri_temp[1]);
        pri_temp[2] = pri_temp[2] -1;
        
      }
      //우선순위 가장 높은 게 마지막으로 수행한게 아니라면 
      else if(result.size() != 0 and result[result.size()-2] != pri_temp[index[0]*3]) 
      {
        result.push_back(pri_temp[index[0]*3]);
        result.push_back(pri_temp[index[0]*3 + 1]);
        pri_temp[index[0]*3 + 2] = pri_temp[index[0]*3 + 2] -1;
      }
      // 우선순위 가장 높은 것이 마지막으로 수행했던 거라면
      else if(result.size() != 0 and result[result.size()-2] == pri_temp[index[0]*3])  
      {
        result.push_back(pri_temp[index[1]*3]);
        result.push_back(pri_temp[index[1]*3 + 1]);
        pri_temp[index[1]*3 + 2] = pri_temp[index[1]*3 + 2] -1;
      }
    }
  }

  //결과 print
  print_result(result);

  return;
}

int main()
{
	// inpu.txt에 데이터 불러서 v에 저장
	freopen("output.txt","w",stdout);

	vector<int> v; // input 데이터 저장

	vector<int> RR; // RR 
	vector<int> C; // Convectional

	string line;

  	ifstream myfile ("input.txt");
  	if (myfile.is_open())
  	{
    	while ( getline (myfile,line) )
    	{
    	   string token; 
			   stringstream stream(line); 
         while (stream >> token)
        		v.push_back(stoi(token));
    	}
    	myfile.close();
  	}


    // priority 0~99 : RR
    // priority 100 ~ : Conventional
  	for(int i=0;i<v.size()-2;i+=3)
    {
      if(v[i] < 100)
      {
        RR.push_back(v[i]);
        RR.push_back(v[i+1]);
        RR.push_back(v[i+2]);
      }
      else
      {
        C.push_back(v[i]);
        C.push_back(v[i+1]);
        C.push_back(v[i+2]);
      }
    }

    // scheduling
    RR_print(RR);
    C_print(C);

  		
}
