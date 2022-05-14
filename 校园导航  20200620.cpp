#include <bits/stdc++.h>
using namespace std;

#define MaxInt 32767                    	//表示极大值，即∞
#define MVNum 100                       	//最大顶点数
#define OK 1	
 						
typedef int VerTexType;              		//点的数据类型为int型 
typedef int ArcType;                  		//边的权值类型为整型 
typedef int OtherInfo;						//和边相关的信息 

//- - - - -图的邻接表存储表示- - - - - 
typedef struct ArcNode{                		//边结点 
    int adjvex;                          	//该边所指向的顶点的位置 
    struct ArcNode *nextarc;          		//指向下一条边的指针 
    OtherInfo info;                      	//和边相关的信息 
}ArcNode; 

typedef struct VNode{ 
    VerTexType data;                    	//顶点信息 
    ArcNode *firstarc;                		//指向第一条依附该顶点的边的指针 
}VNode, AdjList[MVNum];               		//AdjList表示邻接表类型 

//- - - - -图的邻接矩阵存储表示- - - - -
typedef struct{ 
    AdjList vertices;                 		//邻接表  顶点表 
	VerTexType vexs[MVNum];            		//顶点表 
	ArcType arcs[MVNum][MVNum];      		//邻接矩阵 
	int vexnum,arcnum;                		//图的当前点数和边数 
}AMGraph;

bool visited[MVNum];           				//访问标志数组，其初值为"false" 

int LocateVex(AMGraph G , VerTexType v){
	//确定点v在G中的位置
	for(int i = 0; i < G.vexnum; ++i)
		if(G.vexs[i] == v)
			return i;
   return -1;
}//LocateVex

int CreateUDN(AMGraph &G){                                    //主要用邻接矩阵表示法，创建无向网G 

	fstream file;
	file.open("map.txt");
	if (!file) 
	{
		printf("----------------------------------文件不存在！--------------------------------------------\n");
		exit(0);
	}
	
	int i , j , k;
    file >> G.vexnum >> G.arcnum;							//输入总顶点数，总边数

    for(i = 0; i < G.vexnum; ++i){   
		G.vexs[i] = i;                        	     		//点的信息 
		G.vertices[i].data = i;               	//顶点值 
		G.vertices[i].firstarc=NULL;			//初始化表头结点的指针域为NULL 
	}

    for(i = 0; i < G.vexnum; ++i)                			//初始化邻接矩阵，边的权值均置为极大值MaxInt 
		for(j = 0; j < G.vexnum; ++j)   
			G.arcs[i][j] = MaxInt;  

	for(k = 0; k < G.arcnum;++k){							//构造邻接矩阵 
	
		VerTexType v1 , v2;
		ArcType w;

		file >> v1 >> v2 >> w;								//输入一条边依附的顶点及权值
		i = LocateVex(G, v1);  j = LocateVex(G, v2);		//确定v1和v2在G中的位置，即顶点数组的下标 
		                                                    //确定v1和v2在G中位置，即顶点在G.vertices中的序号 
		G.arcs[i][j] = w;									//边<v1, v2>的权值置为w 
		G.arcs[j][i] = G.arcs[i][j];						//置<v1, v2>的对称边<v2, v1>的权值为w 
		
		ArcNode *p1=new ArcNode;               	//生成一个新的边结点*p1 
		p1->adjvex=j;                   		//邻接点序号为j 
		p1->nextarc= G.vertices[i].firstarc;  G.vertices[i].firstarc=p1;  
		//将新结点*p1插入顶点vi的边表头部

		ArcNode *p2=new ArcNode;                //生成另一个对称的新的边结点*p2 
		p2->adjvex=i;                   		//邻接点序号为i 
		p2->nextarc= G.vertices[j].firstarc;  G.vertices[j].firstarc=p2;  
		//将新结点*p2插入顶点vj的边表头部 
	}//for	
	
	file.close();
	return OK; 
}//CreateUDN 

void PrintUDG(AMGraph &G)                           //打印邻接表 
{
	int i,j;
	cout << endl; 

	printf("----------------------------------邻接表表示法创建的无向图--------------------------------\n");
	
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

void PrintUDN(AMGraph &G)                                  //打印邻接矩阵 
{
	int i,j;
	cout <<endl;
	printf("----------------------------------邻接矩阵表示法创建的无向网------------------------------\n");
	
	for(i = 0 ; i < G.vexnum ; ++i){
		for(j = 0; j < G.vexnum; ++j){
			if(j != G.vexnum - 1){
				if(G.arcs[i][j] != MaxInt)
					cout << G.arcs[i][j] << "\t";
				else
					cout << "∞" << "\t";
			}
			else{
				if(G.arcs[i][j] != MaxInt)
					cout << G.arcs[i][j] <<endl;
				else
					cout << "∞" <<endl;
			}
		}
	}//for
	cout <<endl;
}

void DFS(AMGraph &G, int &v){        		
	//图G为邻接矩阵类型 
	int w;
	if(visited[v]) return;
	visited[v] = true;  		//访问第v个顶点，并置访问标志数组相应分量值为true
	 
	for(w = 0; w < G.vexnum; w++)  	//w从1开始						//依次检查邻接矩阵v所在的行  
		if((G.arcs[v][w] != 0)&& (!visited[w])) //G.arcs[v][w]!=0表示w是v的邻接点，如果w未访问，则递归调用DFS 
		{
			printf("->%d",w);
			DFS(G, w); 
			printf("->%d",v);//原路返回 
		}
}//DFS

void DFloyd1(AMGraph &G)                   //输出图中所有两点之间的最短距离
{
	//最短路径长度DFloyd[][]
	
	int **DFloyd = (int **)malloc(G.vexnum * sizeof(int *));					//动态二维数组DFloyd[][]，分配G.vexnum行
	for (int i = 0; i < G.vexnum; ++i) {										//为每行分配G.vexnum列
		DFloyd[i] = (int *)malloc(G.vexnum * sizeof(int));
	}
	for (int v = 0; v < G.vexnum; v++)
		for (int w = 0; w < G.vexnum; w++) {
			if (v == w)															
				G.arcs[v][w] = 0;
			DFloyd[v][w] = G.arcs[v][w];									//把初始值存入DFloyd[][]
		}
	for (int u = 0; u < G.vexnum; u++)
		for (int v = 0; v < G.vexnum; v++)
			for (int w = 0; w < G.vexnum; w++)
				if (DFloyd[v][u] + DFloyd[u][w] < DFloyd[v][w]) {				//比较两种方案，取更小的
					DFloyd[v][w] = DFloyd[v][u] + DFloyd[u][w];
				}
	printf("起点  终点  最短距离\n"); 
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
void DFloyd2(AMGraph &G,int x)                          //输出指定两点间的最短距离及经过的地点
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
					printf("从顶点 %d ---> %d 最短路径长度为：%d\t最短路径为: %d %d", G.vexs[x], G.vexs[j], DFloyd[x][j], G.vexs[x], Path[x][j]);	
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
void DFloyd3(AMGraph &G,int x,int y)  					      //输出从指定点出发，到达图中所有点的最短距离及经过的地点 
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
					printf("从顶点 %d ---> %d 最短路径长度为：%d\t最短路径为: %d %d", G.vexs[x], G.vexs[y], DFloyd[x][y], G.vexs[x], Path[x][y]);

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


void analyse()                                                               //邻接矩阵 
{
	AMGraph G; 
	int judge = -1;
	    
	while(judge!=8)
	{
			cin>>judge;   
			switch(judge)
	        {
	        	case 1:                                                    //构建地图
                CreateUDN(G);
                cout<<endl;
	    		printf("----------------------------------地图构建完毕--------------------------------------------\n");
	    		cout<<endl;
	    		break;
			
    		case 2:                                                    //查看地图的邻接矩阵
    			PrintUDN(G);
    			break;			
			
    		case 3:                                                    //查看地图的邻接表
    			PrintUDG(G);
	    		break;
			
	    	case 4:                                                    //输出从某地点出发，打卡所有点之后回到起点的经过地点序列
			    VerTexType s; 
			    cout<<endl;
			    printf("----------------------------------请输入起始地点：----------------------------------------\n");
			    cout<<endl;
			    cin>>s;                                                //探索起始节点 
			    int i;//标记s的位置 
			    
            	for(i = 0 ; i < G.vexnum ; ++i)
				{
            		if(s == G.vexs[i])
            			break;
            	}
            	
            	while(i >= G.vexnum){
            		cout<<endl;
					printf("----------------------------------该点不存在，请重新输入起始地点:-------------------------\n");
					cout<<endl;
					
					cin >> s;
					for(i = 0 ; i < G.vexnum ; ++i){
						if(s == G.vexs[i])
							break;
					}
				}
				cout<<endl;
	            printf("%d",i);//先输出起始点 
	            memset(visited,0,sizeof(visited));
                DFS(G , i);  
		    	break;
			
    		case 5:                                                    //输出从指定点出发，到达图中所有点的最短距离及经过的地点
    			
    			printf("----------------------------------请输入起始地点：----------------------------------------\n");
    			int tmp1,tmp2;
    			cin>>tmp1;
    			printf("----------------------------------请输入终止地点：----------------------------------------\n");
    			cin>>tmp2;
	     		DFloyd3(G,tmp1,tmp2);
    			break;			
			
	    	case 6:                                                    //输出指定两点间的最短距离及经过的地点
	    		
	    		printf("----------------------------------请输入起始地点：----------------------------------------\n");
	    		int tmp3;
	    		cin>>tmp3;
	    		DFloyd2(G,tmp3);
	    		break;	
					
	    	case 7:                                                    //输出图中所有两点之间的最短距离
	  			DFloyd1(G);
	    		break;			
	 		
		case 8:                                                        //退出
            cout<<endl;
		    printf("----------------------------------感谢您的使用！------------------------------------------\n");
			return;
			break;		
				
		default:
			cout<<endl;
			printf("----------------------------------请重新输入选择项目序号(1-8):----------------------------\n");
			analyse();
			break;		 
	}
	}    
    return ;
}

int main()
{
    system("color 07");
       
    printf("—————————————————欢迎来到校园导航———————————————————--         \n");
    printf("                                                                                                   \n");
    printf("—————————————————菜单项——————————————————————--——         \n");
    printf("                                                                                                   \n");
    printf("—————————————————1.构建地图——————————————————---------- 0.体育场\n");
    printf("                                                                                           1.大会堂\n");
    printf("—————————————————2.查看地图的邻接矩阵—————————————————— 2.图书馆\n");
    printf("                                                                                           3.学术报告厅\n");
    printf("—————————————————3.查看地图的邻接表——————————————————— 4.门诊楼\n");
    printf("                                                                                           5.第一饭堂\n");
    printf("———————————————----4.输出从某地点出发，打卡所有点之后回到起点的经过地点序列 6.教学楼\n");
    printf("                                                                                           7.实验楼\n");
    printf("———————————————----5.输出从指定点出发，到达图中所有点的最短距离及经过的地点 8.GOGO新天地\n");
    printf("                                                                                           9.又康超市\n");
    printf("———————————————----6.输出指定两点间的最短距离及经过的地点————————— 10.隧道口\n");
    printf("                                                                                           11.第二饭堂\n");
    printf("———————————————----7.输出图中所有两点之间的最短距离———————————— 12.八角楼\n");
    printf("                                                                                           13.办公楼\n");
    printf("———————————————----8.退出——————————————---------------------- 14.宿舍\n");
    printf("                                                                                                   \n");
    printf("----------------------------------输入选择项目序号(1-8):----------------------------------         \n");
    cout<<endl;
    analyse();
    
    return 0;
}

