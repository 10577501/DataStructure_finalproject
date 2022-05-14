#include <bits/stdc++.h>
using namespace std;

#define MaxInt 32767                    	//��ʾ����ֵ������
#define MVNum 100                       	//��󶥵���
#define OK 1	
 						
typedef int VerTexType;              		//�����������Ϊint�� 
typedef int ArcType;                  		//�ߵ�Ȩֵ����Ϊ���� 
typedef int OtherInfo;						//�ͱ���ص���Ϣ 

//- - - - -ͼ���ڽӱ�洢��ʾ- - - - - 
typedef struct ArcNode{                		//�߽�� 
    int adjvex;                          	//�ñ���ָ��Ķ����λ�� 
    struct ArcNode *nextarc;          		//ָ����һ���ߵ�ָ�� 
    OtherInfo info;                      	//�ͱ���ص���Ϣ 
}ArcNode; 

typedef struct VNode{ 
    VerTexType data;                    	//������Ϣ 
    ArcNode *firstarc;                		//ָ���һ�������ö���ıߵ�ָ�� 
}VNode, AdjList[MVNum];               		//AdjList��ʾ�ڽӱ����� 

//- - - - -ͼ���ڽӾ���洢��ʾ- - - - -
typedef struct{ 
    AdjList vertices;                 		//�ڽӱ�  ����� 
	VerTexType vexs[MVNum];            		//����� 
	ArcType arcs[MVNum][MVNum];      		//�ڽӾ��� 
	int vexnum,arcnum;                		//ͼ�ĵ�ǰ�����ͱ��� 
}AMGraph;

bool visited[MVNum];           				//���ʱ�־���飬���ֵΪ"false" 

int LocateVex(AMGraph G , VerTexType v){
	//ȷ����v��G�е�λ��
	for(int i = 0; i < G.vexnum; ++i)
		if(G.vexs[i] == v)
			return i;
   return -1;
}//LocateVex

int CreateUDN(AMGraph &G){                                    //��Ҫ���ڽӾ����ʾ��������������G 

	fstream file;
	file.open("map.txt");
	if (!file) 
	{
		printf("----------------------------------�ļ������ڣ�--------------------------------------------\n");
		exit(0);
	}
	
	int i , j , k;
    file >> G.vexnum >> G.arcnum;							//�����ܶ��������ܱ���

    for(i = 0; i < G.vexnum; ++i){   
		G.vexs[i] = i;                        	     		//�����Ϣ 
		G.vertices[i].data = i;               	//����ֵ 
		G.vertices[i].firstarc=NULL;			//��ʼ����ͷ����ָ����ΪNULL 
	}

    for(i = 0; i < G.vexnum; ++i)                			//��ʼ���ڽӾ��󣬱ߵ�Ȩֵ����Ϊ����ֵMaxInt 
		for(j = 0; j < G.vexnum; ++j)   
			G.arcs[i][j] = MaxInt;  

	for(k = 0; k < G.arcnum;++k){							//�����ڽӾ��� 
	
		VerTexType v1 , v2;
		ArcType w;

		file >> v1 >> v2 >> w;								//����һ���������Ķ��㼰Ȩֵ
		i = LocateVex(G, v1);  j = LocateVex(G, v2);		//ȷ��v1��v2��G�е�λ�ã�������������±� 
		                                                    //ȷ��v1��v2��G��λ�ã���������G.vertices�е���� 
		G.arcs[i][j] = w;									//��<v1, v2>��Ȩֵ��Ϊw 
		G.arcs[j][i] = G.arcs[i][j];						//��<v1, v2>�ĶԳƱ�<v2, v1>��ȨֵΪw 
		
		ArcNode *p1=new ArcNode;               	//����һ���µı߽��*p1 
		p1->adjvex=j;                   		//�ڽӵ����Ϊj 
		p1->nextarc= G.vertices[i].firstarc;  G.vertices[i].firstarc=p1;  
		//���½��*p1���붥��vi�ı߱�ͷ��

		ArcNode *p2=new ArcNode;                //������һ���ԳƵ��µı߽��*p2 
		p2->adjvex=i;                   		//�ڽӵ����Ϊi 
		p2->nextarc= G.vertices[j].firstarc;  G.vertices[j].firstarc=p2;  
		//���½��*p2���붥��vj�ı߱�ͷ�� 
	}//for	
	
	file.close();
	return OK; 
}//CreateUDN 

void PrintUDG(AMGraph &G)                           //��ӡ�ڽӱ� 
{
	int i,j;
	cout << endl; 

	printf("----------------------------------�ڽӱ��ʾ������������ͼ--------------------------------\n");
	
	for(i = 0 ; i < G.vexnum ; ++i){
		VNode temp = G.vertices[i];
		ArcNode *p = temp.firstarc;
		if(p == NULL){
			cout << G.vertices[i].data;
			cout << endl;
		}
		else{
			cout << temp.data;
			while(p){
				cout << "->";
				cout << p->adjvex;
				p = p->nextarc;
			}
		}
		cout << endl;
	}
	cout<<endl;
}

void PrintUDN(AMGraph &G)                                  //��ӡ�ڽӾ��� 
{
	int i,j;
	cout <<endl;
	printf("----------------------------------�ڽӾ����ʾ��������������------------------------------\n");
	
	for(i = 0 ; i < G.vexnum ; ++i){
		for(j = 0; j < G.vexnum; ++j){
			if(j != G.vexnum - 1){
				if(G.arcs[i][j] != MaxInt)
					cout << G.arcs[i][j] << "\t";
				else
					cout << "��" << "\t";
			}
			else{
				if(G.arcs[i][j] != MaxInt)
					cout << G.arcs[i][j] <<endl;
				else
					cout << "��" <<endl;
			}
		}
	}//for
	cout <<endl;
}

void DFS(AMGraph &G, int &v){        		
	//ͼGΪ�ڽӾ������� 
	int w;
	if(visited[v]) return;
	visited[v] = true;  		//���ʵ�v�����㣬���÷��ʱ�־������Ӧ����ֵΪtrue
	 
	for(w = 0; w < G.vexnum; w++)  	//w��1��ʼ						//���μ���ڽӾ���v���ڵ���  
		if((G.arcs[v][w] != 0)&& (!visited[w])) //G.arcs[v][w]!=0��ʾw��v���ڽӵ㣬���wδ���ʣ���ݹ����DFS 
		{
			printf("->%d",w);
			DFS(G, w); 
			printf("->%d",v);//ԭ·���� 
		}
}//DFS

void DFloyd1(AMGraph &G)                   //���ͼ����������֮�����̾���
{
	//���·������DFloyd[][]
	
	int **DFloyd = (int **)malloc(G.vexnum * sizeof(int *));					//��̬��ά����DFloyd[][]������G.vexnum��
	for (int i = 0; i < G.vexnum; ++i) {										//Ϊÿ�з���G.vexnum��
		DFloyd[i] = (int *)malloc(G.vexnum * sizeof(int));
	}
	for (int v = 0; v < G.vexnum; v++)
		for (int w = 0; w < G.vexnum; w++) {
			if (v == w)															
				G.arcs[v][w] = 0;
			DFloyd[v][w] = G.arcs[v][w];									//�ѳ�ʼֵ����DFloyd[][]
		}
	for (int u = 0; u < G.vexnum; u++)
		for (int v = 0; v < G.vexnum; v++)
			for (int w = 0; w < G.vexnum; w++)
				if (DFloyd[v][u] + DFloyd[u][w] < DFloyd[v][w]) {				//�Ƚ����ַ�����ȡ��С��
					DFloyd[v][w] = DFloyd[v][u] + DFloyd[u][w];
				}
	printf("���  �յ�  ��̾���\n"); 
	for(int i=0;i<G.vexnum;i++)
	{
		for(int j=i+1;j<G.vexnum;j++)
		{
			printf("%d     %d     %d\n",i,j,DFloyd[i][j]);
		}
	}
	free(DFloyd);
	return;
}
void DFloyd2(AMGraph &G,int x)                          //���ָ����������̾��뼰�����ĵص�
{

	for(int i=0;i<G.vexnum;i++)G.vexs[i]=i;

	int **DFloyd = (int **)malloc(G.vexnum * sizeof(int *));					
	for (int i = 0; i < G.vexnum; ++i) {									
		DFloyd[i] = (int *)malloc(G.vexnum * sizeof(int));
	}


	int **Path = (int **)malloc(G.vexnum * sizeof(int *));						
	for (int i = 0; i < G.vexnum; ++i) {										
		Path[i] = (int *)malloc(G.vexnum * sizeof(int));
	}

	for (int v = 0; v < G.vexnum; v++)
		for (int w = 0; w < G.vexnum; w++) {
			if (v == w)														
				G.arcs[v][w] = 0;
			DFloyd[v][w] = G.arcs[v][w];									
			Path[v][w] = G.vexs[w];												
		}
	for (int u = 0; u < G.vexnum; u++)
		for (int v = 0; v < G.vexnum; v++)
			for (int w = 0; w < G.vexnum; w++)
				if (DFloyd[v][u] + DFloyd[u][w] < DFloyd[v][w]) {			
					DFloyd[v][w] = DFloyd[v][u] + DFloyd[u][w];
					Path[v][w] = Path[v][u];									
				}
			
		int temp = 0;
		for (int j = 0; j < G.vexnum; j++) {
			if(DFloyd[x][j] != MaxInt)
				if (j != x)														
					printf("�Ӷ��� %d ---> %d ���·������Ϊ��%d\t���·��Ϊ: %d %d", G.vexs[x], G.vexs[j], DFloyd[x][j], G.vexs[x], Path[x][j]);	
			temp = LocateVex(G, Path[x][j]);
			while (temp != j) {														
				printf(" %d", Path[temp][j]);										
				temp = LocateVex(G, Path[temp][j]);
			}
			if (j != x)															
				printf("\n");														
		}	
		printf("\n");																
	free(DFloyd);																	
	free(Path);																		
	return;
}
void DFloyd3(AMGraph &G,int x,int y)  					      //�����ָ�������������ͼ�����е����̾��뼰�����ĵص� 
{

	for(int i=0;i<G.vexnum;i++)G.vexs[i]=i;
	int **DFloyd = (int **)malloc(G.vexnum * sizeof(int *));				
	for (int i = 0; i < G.vexnum; ++i) {										
		DFloyd[i] = (int *)malloc(G.vexnum * sizeof(int));
	}


	int **Path = (int **)malloc(G.vexnum * sizeof(int *));						
	for (int i = 0; i < G.vexnum; ++i) {										
		Path[i] = (int *)malloc(G.vexnum * sizeof(int));
	}

	for (int v = 0; v < G.vexnum; v++)
		for (int w = 0; w < G.vexnum; w++) {
			if (v == w)															
				G.arcs[v][w] = 0;
			DFloyd[v][w] = G.arcs[v][w];									
			Path[v][w] = G.vexs[w];												
		}
	for (int u = 0; u < G.vexnum; u++)
		for (int v = 0; v < G.vexnum; v++)
			for (int w = 0; w < G.vexnum; w++)
				if (DFloyd[v][u] + DFloyd[u][w] < DFloyd[v][w]) {				
					DFloyd[v][w] = DFloyd[v][u] + DFloyd[u][w];
					Path[v][w] = Path[v][u];								
				}
				
		int temp = 0;
			if(DFloyd[x][y] != MaxInt)
				if (y != x)															
					printf("�Ӷ��� %d ---> %d ���·������Ϊ��%d\t���·��Ϊ: %d %d", G.vexs[x], G.vexs[y], DFloyd[x][y], G.vexs[x], Path[x][y]);

			temp = LocateVex(G, Path[x][y]);
			while (temp != y) {													
				printf(" %d", Path[temp][y]);										
				temp = LocateVex(G, Path[temp][y]);
			}
			if (y != x)																
				printf("\n");															
		printf("\n");																
	free(DFloyd);																
	free(Path);																	
	return;
}


void analyse()                                                               //�ڽӾ��� 
{
	AMGraph G; 
	int judge = -1;
	    
	while(judge!=8)
	{
			cin>>judge;   
			switch(judge)
	        {
	        	case 1:                                                    //������ͼ
                CreateUDN(G);
                cout<<endl;
	    		printf("----------------------------------��ͼ�������--------------------------------------------\n");
	    		cout<<endl;
	    		break;
			
    		case 2:                                                    //�鿴��ͼ���ڽӾ���
    			PrintUDN(G);
    			break;			
			
    		case 3:                                                    //�鿴��ͼ���ڽӱ�
    			PrintUDG(G);
	    		break;
			
	    	case 4:                                                    //�����ĳ�ص�����������е�֮��ص����ľ����ص�����
			    VerTexType s; 
			    cout<<endl;
			    printf("----------------------------------��������ʼ�ص㣺----------------------------------------\n");
			    cout<<endl;
			    cin>>s;                                                //̽����ʼ�ڵ� 
			    int i;//���s��λ�� 
			    
            	for(i = 0 ; i < G.vexnum ; ++i)
				{
            		if(s == G.vexs[i])
            			break;
            	}
            	
            	while(i >= G.vexnum){
            		cout<<endl;
					printf("----------------------------------�õ㲻���ڣ�������������ʼ�ص�:-------------------------\n");
					cout<<endl;
					
					cin >> s;
					for(i = 0 ; i < G.vexnum ; ++i){
						if(s == G.vexs[i])
							break;
					}
				}
				cout<<endl;
	            printf("%d",i);//�������ʼ�� 
	            memset(visited,0,sizeof(visited));
                DFS(G , i);  
		    	break;
			
    		case 5:                                                    //�����ָ�������������ͼ�����е����̾��뼰�����ĵص�
    			
    			printf("----------------------------------��������ʼ�ص㣺----------------------------------------\n");
    			int tmp1,tmp2;
    			cin>>tmp1;
    			printf("----------------------------------��������ֹ�ص㣺----------------------------------------\n");
    			cin>>tmp2;
	     		DFloyd3(G,tmp1,tmp2);
    			break;			
			
	    	case 6:                                                    //���ָ����������̾��뼰�����ĵص�
	    		
	    		printf("----------------------------------��������ʼ�ص㣺----------------------------------------\n");
	    		int tmp3;
	    		cin>>tmp3;
	    		DFloyd2(G,tmp3);
	    		break;	
					
	    	case 7:                                                    //���ͼ����������֮�����̾���
	  			DFloyd1(G);
	    		break;			
	 		
		case 8:                                                        //�˳�
            cout<<endl;
		    printf("----------------------------------��л����ʹ�ã�------------------------------------------\n");
			return;
			break;		
				
		default:
			cout<<endl;
			printf("----------------------------------����������ѡ����Ŀ���(1-8):----------------------------\n");
			analyse();
			break;		 
	}
	}    
    return ;
}

int main()
{
    system("color 07");
       
    printf("������������������������������������ӭ����У԰������������������������������������������--         \n");
    printf("                                                                                                   \n");
    printf("�����������������������������������˵��������������������������������������������--����         \n");
    printf("                                                                                                   \n");
    printf("����������������������������������1.������ͼ������������������������������������---------- 0.������\n");
    printf("                                                                                           1.�����\n");
    printf("����������������������������������2.�鿴��ͼ���ڽӾ��󡪡��������������������������������� 2.ͼ���\n");
    printf("                                                                                           3.ѧ��������\n");
    printf("����������������������������������3.�鿴��ͼ���ڽӱ������������������������������������� 4.����¥\n");
    printf("                                                                                           5.��һ����\n");
    printf("������������������������������----4.�����ĳ�ص�����������е�֮��ص����ľ����ص����� 6.��ѧ¥\n");
    printf("                                                                                           7.ʵ��¥\n");
    printf("������������������������������----5.�����ָ�������������ͼ�����е����̾��뼰�����ĵص� 8.GOGO�����\n");
    printf("                                                                                           9.�ֿ�����\n");
    printf("������������������������������----6.���ָ����������̾��뼰�����ĵص㡪���������������� 10.�����\n");
    printf("                                                                                           11.�ڶ�����\n");
    printf("������������������������������----7.���ͼ����������֮�����̾��롪���������������������� 12.�˽�¥\n");
    printf("                                                                                           13.�칫¥\n");
    printf("������������������������������----8.�˳�����������������������������---------------------- 14.����\n");
    printf("                                                                                                   \n");
    printf("----------------------------------����ѡ����Ŀ���(1-8):----------------------------------         \n");
    cout<<endl;
    analyse();
    
    return 0;
}

