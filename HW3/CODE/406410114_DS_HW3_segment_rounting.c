#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define V 100
typedef struct link_input{
	int linkID;
	int firstNodeID;
	int secondNodeID; // sorting
	double linkCapacity;
} link_input;

typedef struct flow_input{
	int flowID;
	int sourceID;
	int destinationID; // sorting
	double flowSize;
} flow_input;

typedef struct node{
	int point;
	double weight;
	double load; // sorting
	double linkCapacity;
	struct node *next;
} node;






void dijkstra(double graph[V][V], int src);
void ReadInput();
void Debug_node_printf();
void Debug_flow_printf();
void Debug_node_printf();
void create_wei_graph( node node_info[] );
int minDistance(int dist[], bool sptSet[]);
int printSolution(int dist[]);
bool capacity_determine( node node_info[], double capacity );
void update_weight( node node_info[],double graph[V][V] );
void seg_select_path( int src,int des );
void segment_rounting();

int node_num = 0,link_num =0,flow_num =0,n,sou_des_path[V][V],sdp_i=0;
int seg_tb[100][100][100];
double wei_graph[100][100];
link_input link_info[10000];
flow_input flow_path[10000];
node node_info[1000] = { 0,0,0,0,NULL };


//===========================================方法如下=====================================//
//1.Read input後產生node_info
//2.利用 create_wei_graph  建立出graph
//3.呼叫 dijkstra 計算shorest path
//4.dijkstra呼叫segment_rounting  建立出  segment_table[source][destination][path]
//5.輸入flow 運用seg_select_path  選擇最佳路徑
//6.capacity_determine  判斷是否最佳路徑上還有capacity  回傳true or false
//6.1若為false 則continue
//6.2若為true 則新增path  計算flow_num  throughput  並且更新權重
//7.不斷重複 4~6  直到輸入flow結束


int main() {
    int i,j,k;
    int throughput=0;
    int accept_flow=0;
    bool is_cap;
    ReadInput();
    for ( i = 0;i<node_num;i++ ){
        for ( j = 0;j<node_num;j++ ){
            sou_des_path[i][j] = -1;
            for ( k =0 ; k <node_num;k++ ){
                seg_tb[i][j][k] =-1 ;
            }//for
        }//for
    }//for

    //generate graph
    create_wei_graph( node_info );
    //create segment table
    for (i =0 ; i < node_num ; i++){
        dijkstra(wei_graph, i);
    }//for

    /*
    for ( i = 0;i<node_num;i++ ){
        for ( j = 0;j<node_num;j++ ){
            for ( k =0 ; k <node_num;k++ ){
                if ( seg_tb[i][j][k] !=-1 )
                    printf( "seg_tb[%d][%d][%d]:%d\n",i,j,k,seg_tb[i][j][k] );
            }//for
        }//for
    }//for
    */

     for (i =0 ; i < flow_num ; i++){
        create_wei_graph( node_info );
        if ( flow_path[i].sourceID < node_num &&flow_path[i].destinationID  < node_num ){
            seg_select_path(flow_path[i].sourceID,flow_path[i].destinationID );
            is_cap = capacity_determine( node_info, flow_path[i].flowSize );
            if (is_cap == true){
                accept_flow++;
                throughput = throughput + flow_path[i].flowSize;
            }//if
        }//if


     }//for


    printf( "%d %d\n",accept_flow,throughput );
    for ( i = 0; i<sdp_i;i++ ){
        printf( "%d ",i );
        for( j= 0 ;j<node_num ;j++){
            if ( sou_des_path[i][j] == -1 )
                continue;
            else{
                printf( "%d ",sou_des_path[i][j] );
            }//else
        }//for
        printf( "\n" );
    }//for





    //Debug_link_printf();
    //Debug_flow_printf();
    //Debug_node_printf();

    return 0;
}//main



void ReadInput(){
//*****************************************

//******************************************
    //int input_number = 0;
    int input = 0;
    double d_input ;

//determine rule
    int i=0;
//file input rule

    //FILE *fin;
	//fin=fopen(filename,"r");
    scanf("%d",&node_num);
    //printf( "node_num:%d\n",node_num );
    scanf("%d",&link_num);
    //printf( "node_num:%d\n",node_num );

    for (i=0;i<link_num;i++)
    {
        scanf("%d",&input);
        link_info[i].linkID = input ;
        scanf("%d",&input);
        link_info[i].firstNodeID = input ;
        scanf("%d",&input);
        link_info[i].secondNodeID = input ;
        scanf("%lf",&d_input);
        link_info[i].linkCapacity = d_input ;
    }//for

    scanf("%d",&flow_num);
    //printf( "flow_num:%d\n",flow_num );

    for (i=0;i<flow_num;i++)
    {
        scanf("%d",&input);
        flow_path[i].flowID = input ;
        scanf("%d",&input);
        flow_path[i].sourceID = input ;
        scanf("%d",&input);
        flow_path[i].destinationID = input ;
        scanf("%lf",&d_input);
        flow_path[i].flowSize = d_input ;
    }//for


    //int node_index = -1 ;
    //int max_finode_id =  link_info[node_num-1].firstNodeID ;
    node *now_ptr = (node *) malloc(sizeof(node));
    for ( i=0;i<node_num;i++ ){
        now_ptr = &node_info[node_num];
        now_ptr->next = NULL;
    }//for

    //create Adjacency Lists  use pointer array
    for ( i=0;i<link_num;i++ ){
        //printf( "-----------------------round--------------------------\n" );
        //printf( "link_info[i].firstNodeID :%d\n",link_info[i].firstNodeID );
        //printf( "node_index :%d\n ",node_index  );
        //first node
        now_ptr = &node_info[link_info[i].firstNodeID];
        node *new_ptr = (node *) malloc(sizeof(node));
        while ( now_ptr->next != NULL )
            now_ptr = now_ptr->next;

        new_ptr->point = link_info[i].secondNodeID;
        //printf( "new_ptr->point:%d\n",new_ptr->point );
        new_ptr->linkCapacity = link_info[i].linkCapacity;
        new_ptr->load = 0;
        new_ptr->weight = (new_ptr->load)/(new_ptr->linkCapacity - new_ptr->load);
        //printf( "new_ptr->linkCapacity:%d\n",new_ptr->linkCapacity );
        now_ptr->next = new_ptr;
        now_ptr = now_ptr->next;
        now_ptr->next = NULL;

        //second node
        now_ptr = &node_info[link_info[i].secondNodeID];
        node *new_ptr2 = (node *) malloc(sizeof(node));
        while ( now_ptr->next != NULL )
            now_ptr = now_ptr->next;

        new_ptr2->point = link_info[i].firstNodeID;
        //printf( "new_ptr2->point:%d\n",new_ptr2->point );
        new_ptr2->linkCapacity = link_info[i].linkCapacity;
        new_ptr2->load = 0;
        new_ptr2->weight = (new_ptr2->load)/(new_ptr2->linkCapacity - new_ptr2->load);
        //printf( "new_ptr2->linkCapacity:%d\n",new_ptr2->linkCapacity );
        now_ptr->next = new_ptr2;
        now_ptr = now_ptr->next;
        now_ptr->next = NULL;
    }//for

}//ReadInput

void create_wei_graph( node node_info[] ){
    int i,j;

    for ( i=0;i<node_num;i++ ){
        for ( j=0;j<node_num;j++ ){
            //wei_graph[i][j]=  -1.0;
            wei_graph[i][j]=  0;
        }//for
    }//for



    for ( i=0;i<node_num;i++ ){
        for ( j=0;j<node_num;j++ ){
            node *now_ptr = (node *) malloc(sizeof(node));
            now_ptr = &node_info[i];
            while ( now_ptr->next != NULL ){
                //wei_graph[i][now_ptr->next->point] = now_ptr->next->weight;
                wei_graph[i][now_ptr->next->point] = 1 + now_ptr->next->weight;
                now_ptr = now_ptr->next;
            }//wile
        }//for
    }//for

    /*
    printf( "wei_graph\n" );
    for ( i=0;i<node_num;i++ ){
        for ( j=0;j<node_num;j++ ){
            printf( "%lf ",wei_graph[i][j] );
        }//for

        printf( "\n" );
    }//for
    */


}//shorest_path



int minDistance(int dist[], bool sptSet[])
{
    // Initialize min value
    int min = INT_MAX, min_index;  //

    for (int v = 0; v < node_num; v++)
        if (sptSet[v] == false && dist[v] <= min){

            min = dist[v], min_index = v;  //  距離  並且回傳index
            //printf( " min_index:%d\n", min_index );
        }//if


    return min_index;
}

int printSolution(int dist[])
{
    printf("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < node_num; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

void dijkstra(double graph[V][V], int src) // shortest path
{
    int path[V],path_i=0,sou_des_trace[V][V],sdt_i = 0;
    int dist[V];
    // distance from src to i

    for ( int i = 0 ;i<node_num;i++){
        for(int j= 0 ;j<node_num ;j++)
            sou_des_trace[i][j] = -1;
    }//for


    bool sptSet[V]; // sptSet[i] will be true if vertex i is included in shortest
    // path tree or shortest distance from src to i is finalized

    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < node_num; i++)
        dist[i] = INT_MAX, sptSet[i] = false;   //所有的距離都最大  且還沒選過

    // Distance of source vertex from itself is always 0
    dist[src] = 0;  //source點

    // Find shortest path for all vertices
    for (int count = 0; count < node_num - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in the first iteration.
        int u = minDistance(dist, sptSet);

        // Mark the picked vertex as processed
        sptSet[u] = true;   //改成true
        //path[path_i] = u;  //走過路徑
        //path_i++;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < node_num; v++){
            //sdt_i=0;
            // Update dist[v] only if is not in sptSet, there is an edge from
            // u to v, and total weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v]){  //假設是false 並且graph不是0  dist有值  可以到
                dist[v] = dist[u] + graph[u][v];  //
                //sou_des_trace[v][sdt_i] = u;
                //printf( "sou_des_trace[%d][sdt_i]:%d\n",v,sou_des_trace[v][sdt_i] );
                //sdt_i++;
            }//if
        }//for
    }//for






    // print the constructed distance array
    //printSolution(dist);
    segment_rounting( dist,graph,src );
}//dijkstra

//===================================segment_rounting_table=======================================//
void segment_rounting( int dist[],double graph[V][V],int src ){
    int i,j,k,m,l;
    int now_hop,max_hop=0;
    bool min_idx = true; //找最小index
    for ( i =0 ; i < node_num;i++ ){
        if ( dist[i]>max_hop )
            max_hop = dist[i];
    }//for

    //printf( "max_hop:%d\n",max_hop );
    // hop
    for ( l = 0; l <= max_hop;l++ ){
        //printf( "l:%d\n",l );
        for ( i = 0;i<node_num;i++ ){
            //從最小hop開始做到max_hop
            if ( dist[i] != l  )
                continue;
            //printf( "i:%d\n",i );
            if ( dist[i] == 0 )  // hop = 0
                seg_tb[src][i][0] = src;
            else if ( dist[i] == 1 ){ // hop = 1
                seg_tb[src][i][0] = src;
                seg_tb[src][i][1] = i;
            }//else if


            // hop >2
            else {
                for ( j = 0;j<node_num;j++ ){
                    //printf( "j:%d\n",j );
                    if ( dist[j] == dist[i]-1 ){  //使用hop小的去擴散
                        min_idx = false;
                        for ( m = 0;m<dist[i]-1 ;m++ ){
                            if ( seg_tb[src][i][m] > seg_tb[src][j][m] ) {  //若有更小的min_idx  就去找  否則不做
                                //printf( "seg_tb[src][i][m]:%d\n",seg_tb[src][i][m] );
                                //printf( "seg_tb[src][j][m]:%d\n",seg_tb[src][j][m] );
                                min_idx = true;
                                break;
                            }//if

                            else if ( seg_tb[src][i][m] < seg_tb[src][j][m]  ){
                                //printf( "seg_tb[src][i][m]:%d\n",seg_tb[src][i][m] );
                                //printf( "seg_tb[src][j][m]:%d\n",seg_tb[src][j][m] );
                                min_idx = false;
                                break;
                            }//else if
                            else
                                continue;
                        }//for

                        //printf( "min_idx:%d\n",min_idx );

                        //有更得min_index 或是第一次
                        if ( min_idx == true || seg_tb[src][i][0] == -1 ){
                            int temp_arr[100];
                            for ( k = 0;k<dist[i];k++ ){
                                temp_arr[k] = seg_tb[src][i][k];    //temp_arr暫存  若沒有找到救回填
                                seg_tb[src][i][k] = seg_tb[src][j][k];
                            }//for

                            //開始找  找到NULL代表沒找到  否則就是找到
                            node *now_ptr = (node *) malloc(sizeof(node));
                            now_ptr = &node_info[j];
                            while ( now_ptr->next != NULL && now_ptr->next->point != i )
                                now_ptr = now_ptr->next;


                            //沒找到  還原
                            if ( now_ptr->next == NULL ){
                                for ( k = 0;k<dist[i];k++ )
                                    seg_tb[src][i][k] = temp_arr[k];
                                continue;
                            }//if
                            //找到 複寫
                            else if ( now_ptr->next->point == i ){
                                seg_tb[src][i][l] = i;
                                continue;
                            }//else
                        }//if
                    }//if
                }//for

            }//else




        }//for
    }//for

    /*
    for ( i = 0;i<node_num;i++ ){
        for ( j = 0;j<node_num;j++ ){
            for ( k =0 ; k <node_num;k++ ){
                if ( seg_tb[i][j][k] !=-1 )
                printf( "seg_tb[%d][%d][%d]:%d\n",i,j,k,seg_tb[i][j][k] );
            }//for
        }//for
    }//for
    */

}

//===================================seg_select_path=======================================//
void seg_select_path( int src,int des ){
    //computing weight
    int i,j,k,m,l;
    int hop_num =-1,mid = -1,hop_temp = 0;  //mid為選中中間的的點
    double wei_num = -1,wei_temp = 0;



    for ( i = 0; i < node_num ; i++ ){
        wei_temp = 0;
        hop_temp = 0;
        //printf( "i:%d\n",i );

        //計算到source-mid
        for ( j = 0; j<node_num;j++ ){
            //printf( "j:%d\n",j );
            //找到最後一個
            if ( seg_tb[src][i][j+1] == -1 ){
                hop_temp++;
                break;
            }//if


            //source ->i
            wei_temp = wei_temp + wei_graph[seg_tb[src][i][j]][seg_tb[src][i][j+1]]-1;
            hop_temp++;
        }//for

        //計算到mid-destination
        for ( j = 0; j<node_num;j++ ){
            if ( seg_tb[i][des][j+1] == -1 ){
                hop_temp++;
                break;
            }//if

            //i->des
            wei_temp = wei_temp + wei_graph[ seg_tb[i][des][j]][ seg_tb[i][des][j+1]]-1;
            hop_temp++;
        }//for

        //printf( "wei_temp:%d\n",wei_temp );
        //printf( "hop_temp:%d\n",hop_temp );
        //printf( "mid:%d\n",mid );
        //第一次直接複寫
        if ( wei_num == -1 ){
            wei_num = wei_temp;
            hop_num = hop_temp;
            mid = i;
            //printf( "wei_num:%d\n",wei_num );
            //printf( "hop_num:%d\n",hop_num );
            //printf( "mid:%d\n",mid );
        }//if

        // 取wei小
        else if ( wei_num > wei_temp ){
            wei_num = wei_temp;
            hop_num = hop_temp;
            mid = i;
            //printf( "wei_num:%d\n",wei_num );
            //printf( "hop_num:%d\n",hop_num );
            //printf( "mid:%d\n",mid );
        }//else if

        // 相等  取hop小 因為從小開始找故一定min_index
        else if ( wei_num == wei_temp ){
            if ( hop_num > hop_temp ){
                hop_num = hop_temp;
                mid = i;
                //printf( "wei_num:%d\n",wei_num );
                //printf( "hop_num:%d\n",hop_num );
                //printf( "mid:%d\n",mid );
            }//if
        }//else if
    }//for

    //新增路徑
    int mid_index = 0;
    for ( i = 0; i < node_num ; i++ ){
        if ( seg_tb[src][mid][i] != -1 ){
            sou_des_path[sdp_i][mid_index++] = seg_tb[src][mid][i];
        }//if
    }//for
    mid_index--; //扣掉中間重複的
    for ( i = 0; i < node_num ; i++ ){
        if ( seg_tb[mid][des][i] != -1 ){
            sou_des_path[sdp_i][mid_index++] = seg_tb[mid][des][i];

        }//if
    }//for

    sdp_i++;
    /*
    printf( "sou_des_path:\n" );
    for ( i = 0; i < node_num ; i++ ){
        if ( sou_des_path[sdp_i-1][i] != -1 ){
            printf( "%d ",sou_des_path[sdp_i-1][i] );
        }//if
    }//for
    printf( "\n" );
    */

}//seg_select_path



bool capacity_determine( node node_info[],double capacity ){
    bool rule = true;
    int i,j;
    /*
    for(i= 0 ;i < node_num ;i++){
        if ( sou_des_path[sdp_i-1][i] == -1 )
            continue;
        else
            printf( "sou_des_path[sdp_i][%d]:%d\n",i,sou_des_path[sdp_i-1][i] );
    }//for
    */

    //printf( "sou_des_path[sdp_i-1][0]:%d\n",sou_des_path[sdp_i-1][0] );
    if ( sou_des_path[sdp_i-1][1] == -1 ){
        rule = false;

        for(int i= 0 ;i<node_num ;i++)
            sou_des_path[sdp_i-1][i] = -1;
        sdp_i--;
        return rule;
    }//IF


    for(i= 0 ;i < node_num ;i++){
        if ( sou_des_path[sdp_i-1][i] == -1 )
            continue;
        else{
            node *now_ptr = (node *) malloc(sizeof(node));
            now_ptr = &node_info[sou_des_path[sdp_i-1][i]];
            //找到要確認的cap

            while ( now_ptr->next != NULL && now_ptr->next->point != sou_des_path[sdp_i-1][i+1] )
                now_ptr= now_ptr->next;

            //若爆掉則reject  刪除此路徑

            if ( now_ptr->next != NULL && capacity > now_ptr->next->linkCapacity ){
                //printf( "capacity:%lf\n",capacity );
                //printf( "now_ptr->next->linkCapacity:%lf\n", now_ptr->next->linkCapacity );
                //printf( "now_ptr->point:%lf\n",now_ptr->point );
                //printf( "now_ptr->next->point:%lf\n",now_ptr->next->point );
                rule = false;
                for(int i= 0 ;i<node_num ;i++)
                    sou_des_path[sdp_i-1][i] = -1;
                sdp_i--;
                return rule;
            }//if
        }//else
    }//for


    //printf( "sdp_i:%d\n",sdp_i );



    int update_link[100],upli_i=0;


    if ( rule == true ){
        //printf( "rule is true.\n" );
        for(i= 0 ;i < node_num ;i++){
            if ( sou_des_path[sdp_i-1][i] == -1 || sou_des_path[sdp_i-1][i+1] == -1 ){
                //printf( "sou_des_path[sdp_i-1][%d]:%d\n",i,sou_des_path[sdp_i-1][i] );
                //printf( "sou_des_path[sdp_i-1][%d]:%d\n",i+1,sou_des_path[sdp_i-1][i+1] );
                continue;
            }//if

            for( j = 0 ; j< link_num ;j++ ){

                if ( (link_info[j].firstNodeID == sou_des_path[sdp_i-1][i+1] && link_info[j].secondNodeID == sou_des_path[sdp_i-1][i])
                    ||(link_info[j].secondNodeID == sou_des_path[sdp_i-1][i+1] && link_info[j].firstNodeID == sou_des_path[sdp_i-1][i]) ){
                    //printf( "link_info[j].firstNodeID:%d\n",link_info[j].firstNodeID );
                    //printf( "sou_des_path[sdp_i][i+1]:%d\n",sou_des_path[sdp_i-1][i+1] );
                    //printf( "link_info[j].secondNodeID:%d\n",link_info[j].secondNodeID );
                    //printf( "sou_des_path[sdp_i][i]:%d\n",sou_des_path[sdp_i-1][i] );
                    if ( link_info[j].linkCapacity - capacity >= 0.0 ){
                        link_info[j].linkCapacity = link_info[j].linkCapacity - capacity;
                        update_link[upli_i] = j;
                        upli_i++;
                    }//if
                    else{
                        rule = false;
                        for(int i= 0 ;i<node_num ;i++)
                            sou_des_path[sdp_i-1][i] = -1;
                        sdp_i--;
                        return rule;
                    }//else
                }//if
            }//for
        }//for

        //printf(  "upli_i:%d\n",upli_i );

        //printf( "test1\n" );
        for ( i = 0;i <upli_i;i++ ){
            node *now_ptr = (node *) malloc(sizeof(node));
            now_ptr = &node_info[link_info[update_link[i]].firstNodeID];
            //printf( "link_info[update_link[i]].firstNodeID:%d\n",link_info[update_link[i]].firstNodeID );
            now_ptr = now_ptr->next;
            while (now_ptr->point != link_info[update_link[i]].secondNodeID )
                now_ptr = now_ptr->next;
            //printf( "now_ptr->point:%d\n",now_ptr->point );

            now_ptr->load = now_ptr->load + capacity;
            if ( (now_ptr->linkCapacity - now_ptr->load) ==0 )
                now_ptr->weight = INT_MAX;
            else
                now_ptr->weight = (now_ptr->load)/(now_ptr->linkCapacity - now_ptr->load);

            now_ptr = &node_info[link_info[update_link[i]].secondNodeID];
            //printf( "link_info[update_link[i]].secondNodeID:%d\n",link_info[update_link[i]].secondNodeID );
            now_ptr = now_ptr->next;
            while (now_ptr->point != link_info[update_link[i]].firstNodeID ){
                now_ptr = now_ptr->next;
            }//while
            //printf( "now_ptr->point:%d\n",now_ptr->point );
            //printf( "capacity:%lf\n",capacity );
            //printf( "now_ptr->load:%lf\n",now_ptr->load );
            //printf( "now_ptr->load + capacity:%lf\n",now_ptr->load + capacity );
            now_ptr->load = now_ptr->load + capacity;
            if ( (now_ptr->linkCapacity - now_ptr->load) ==0 )
                now_ptr->weight = INT_MAX;
            else
                now_ptr->weight = (now_ptr->load)/(now_ptr->linkCapacity - now_ptr->load);
        }//for

    }//if



    return rule;


}//capacity_determine


void Debug_link_printf(){
    printf( "node_num:%d\n",node_num );
    printf( "link_num:%d\n",link_num );
    printf( "Debug_node_printf:\n" );
    printf( "linkID firstNodeID secondNodeID linkCapacity\n" );
    for (int i=0;i<link_num;i++)
    {
        printf("%d ",link_info[i].linkID);
        printf("%d ",link_info[i].firstNodeID);
        printf("%d ",link_info[i].secondNodeID);
        printf("%lf\n",link_info[i].linkCapacity);
    }//for
}//Debug_node_printf


void Debug_flow_printf(){
    printf( "flow_num:%d\n",flow_num );
    printf( "Debug_flow_printf:\n" );
    printf( "flowID sourceID destinationID flowSize\n" );
    for (int i=0;i<flow_num;i++)
    {
        printf("%d ",flow_path[i].flowID);
        printf("%d ",flow_path[i].sourceID);
        printf("%d ",flow_path[i].destinationID);
        printf("%lf\n",flow_path[i].flowSize);
    }//for
}//Debug_node_printf




void Debug_node_printf(){
    printf( "node_num:%d\n",node_num );
    printf( "Debug_node_printf:\n" );

    node *now_ptr = (node *) malloc(sizeof(node));
    int max_finode_id =  link_info[node_num-1].firstNodeID ;
    for (int i=0;i< node_num;i++){
        now_ptr = &node_info[i];
        printf( "it is node [%d]\n",i);
        while  ( now_ptr->next != NULL ){
            now_ptr = now_ptr->next;
            printf("point:%d ",now_ptr->point);
            printf("linkCapacity:%lf ",now_ptr->linkCapacity);
            printf("load:%lf ",now_ptr->load);
            printf("weight:%lf \n",now_ptr->weight);
        }//if
    }//for
}//Debug_node_printf

