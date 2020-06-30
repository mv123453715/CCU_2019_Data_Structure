#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#define SIZE 100
#define V 100
typedef struct link_input{
	int linkID;
	int firstNodeID;
	int secondNodeID; // sorting
	double distance;
} link_input;

typedef struct node{
	int point;
	double weight;
	struct node *next;
} node;

typedef struct flow_input{
	int flowID;
	int sourceID;
	int destinationID; // sorting
} flow_input;


struct DataItem {
   int node1;
   int node2;
   double distance;
   int key;
   struct DataItem *next;
};

typedef struct node_dis{
    char type;// 's':sample  'n' :not sample
    double shortest_dis;
    int shortest_dis_sample;
    int shortest_dis_sample_index;
	double distance[100];
    int sml_than_sht_num;
    int sml_than_sht_node[100];
    double sml_than_sht_dis[100];
} node_dis;



struct DataItem* dummyItem;
struct DataItem* item;
int node_num = 0,link_num =0,flow_num =0,n,hash_prime,sample_node_num,nd_dis_smp_i = 0,hash_pair_num = 0,hash_tb_size;
int sample_node[100];
node_dis node_dis_sample[100];
double wei_graph[100][100];
link_input link_info[10000];
flow_input flow_path[10000];
node node_info[1000] = { 0,0,NULL };



int hashCode(int key );
//struct DataItem* hashArray = malloc(sizeof(struct DataItem) * hash_tb_size);
struct DataItem* hashArray[SIZE];
struct DataItem *search(int key ) ;
void insert(int key,int node1,int node2,double distance);
int compute_key(int node1,int node2 );
void display();
void ReadInput();
void create_wei_graph( node node_info[] );
int minDistance(int dist[], bool sptSet[]);
int printSolution(int dist[]);
void dijkstra(double graph[V][V], int src,char mode[]) ;
int seclect_next_sample();
void Debug_node_dis_sample();
double pair_distance_search( int src,int des );
void store_to_hash_tb();

int main() {
    int i,j,k;
    double pari_distance;
    //=======================================read_input================================================================//
    ReadInput();
    //=======================================compute hash_tb_size and  sample_node_num=================================//
    hash_tb_size = ceil(pow((float)node_num,1.5));
    sample_node_num =ceil(sqrt((float)node_num));

    //=======================================hash_pair_num=============================================================//
    for ( i = node_num- sample_node_num; i< node_num ; i++)
        hash_pair_num+=i;
    //printf( "hash_pair_num:%d\n",hash_pair_num );

    //=======================================generate graph=============================================================//
    create_wei_graph( node_info );

    //=======================================boot =========================================================================//
    for ( i =0 ; i < node_num ; i++ ){
        node_dis_sample[i].type = 'n';
        node_dis_sample[i].sml_than_sht_num =0;
    }//for

    //=======================================first sample_node is 0 ======================================================//
    sample_node[0] = 0;
    node_dis_sample[0].type = 's';

    //printf( "sample_node_num:%d\n",sample_node_num );

    //=======================================select other sample  ======================================================//
    for (i =0 ; i < sample_node_num ; i++){
        dijkstra(wei_graph, sample_node[i],"sample_node");  //sample_node
        sample_node[nd_dis_smp_i] = seclect_next_sample();
    }//for

    //======================================= store smaller_than_shortest_distance =====================================//
    for (i =0 ; i < node_num ; i++){
        dijkstra(wei_graph, i,"smaller_than_shortest_dis");  //smaller_than_shortest_dis
    }//for

    //======================================= store into has table =====================================================//
    store_to_hash_tb();
    printf( "%d\n",hash_pair_num );

    //======================================= pair_distance_search =====================================================//
    for ( i = 0;i < flow_num;i++ ){
        pari_distance = pair_distance_search( flow_path[i].sourceID,flow_path[i].destinationID );
        printf( "%d %d %d %.0lf\n",i,flow_path[i].sourceID,flow_path[i].destinationID,pari_distance );
    }//for


    //Debug_node_dis_sample();

   return 0;
}



void dijkstra(double graph[V][V], int src,char mode[]) // shortest path
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
    //printf( "%s\n",mode );
    // append node to sample node distance
    if ( strcmp(mode,"sample_node")==0 ){
        //printf( "%c\n",mode );
        for ( int i = 0; i<node_num;i++ )
            node_dis_sample[i].distance[nd_dis_smp_i] =dist[i];
    nd_dis_smp_i++;
    }//if

    bool record = false;
    bool sample = false;

    //smaller_than_shortest_distance
    if ( strcmp(mode,"smaller_than_shortest_dis")==0 ){
        for ( int i=0;i<node_num;i++ ){
            record = false;
            sample = false;
            if ((dist[i] <= node_dis_sample[src].shortest_dis) && ( dist[i]!=0 )){
                for ( int j = 0 ; j<sample_node_num;j++){
                    if ( sample_node[j] == i )
                        sample =true;
                }//for
                for ( int j = 0;j<node_dis_sample[i].sml_than_sht_num;j++ ){
                    //printf( "node_dis_sample[%d].sml_than_sht_node[%d]:%d\n",i,j,node_dis_sample[i].sml_than_sht_node[j] );
                    //printf( "src:%d\n",src );
                    if ( node_dis_sample[i].sml_than_sht_node[j] == src )
                        record = true;
                }//for

                //it is not sample node  and not record
                if ( record == false && sample ==false && node_dis_sample[src].type == 'n' ){
                    node_dis_sample[src].sml_than_sht_node[node_dis_sample[src].sml_than_sht_num] = i;
                    node_dis_sample[src].sml_than_sht_dis[node_dis_sample[src].sml_than_sht_num] = dist[i];
                    node_dis_sample[src].sml_than_sht_num++;
                    hash_pair_num++;
                }//false
            }//if
        }//for
    }//if



    printSolution(dist);
}//dijkstra


int seclect_next_sample(){
    int i,j;
    int max_dis_node,sht_sample_node = 0;
    double shortest_dis,longest_dis ;

    //every node update shortest_distance
    for ( i = 0 ;i<node_num;i++ ){
        //printf( "i:%d\n",i );
        for ( j = 0 ; j< nd_dis_smp_i;j++ ){
            if ( j == 0 ){
                shortest_dis =node_dis_sample[i].distance[j];
                sht_sample_node = j;
                //printf( "shortest_dis:%lf\n",shortest_dis );
                //printf( "sample_node[sht_sample_node]:%d\n",sample_node[sht_sample_node] );
            }//if
            else {
                if ( node_dis_sample[i].distance[j] < shortest_dis ){
                    shortest_dis =node_dis_sample[i].distance[j];
                    sht_sample_node = j;
                    //printf( "shortest_dis:%lf\n",shortest_dis );
                    //printf( "sample_node[sht_sample_node]:%d\n",sample_node[sht_sample_node] );
                }//if
            }//else
        }//for
        node_dis_sample[i].shortest_dis = shortest_dis;
        node_dis_sample[i].shortest_dis_sample = sample_node[sht_sample_node];
        node_dis_sample[i].shortest_dis_sample_index = sht_sample_node;
    }//for

    //select the distance which is to nearest sample node  longest node
    for ( i = 0 ;i<node_num;i++ ){
        if ( node_dis_sample[i].shortest_dis > longest_dis ){
            longest_dis = node_dis_sample[i].shortest_dis;
            max_dis_node = i;
        }//if
    }//for

    //return
    if ( nd_dis_smp_i < sample_node_num  )
        node_dis_sample[max_dis_node].type = 's';
    return max_dis_node;
}//seclect_next_sample


void store_to_hash_tb(){
    int i,j,k,key;

    //sample node
    for ( i=0;i<node_num;i++ ){
        for ( j = 0;j<sample_node_num;j++ ){
            if ( i != sample_node[j]  ){ // 自己不用存

                //compute key
                key = compute_key(i,sample_node[j] );
                insert( key,i,sample_node[j],node_dis_sample[i].distance[j] );
                //printf( "i:%d\n",i );
                //printf( "key:%d\n",key );
                //printf( "sample_node[%d]:%d\n",j,sample_node[j] );
                //printf( "node_dis_sample[%d].distance[%d]:%lf\n",i,j,node_dis_sample[i].distance[j] );
            }//if
        }//for
    }//for

    //smaller_than_shortest_distance
    for (  i=0;i<node_num;i++  ){
        for ( j = 0 ; j<node_dis_sample[i].sml_than_sht_num;j++ ){
            key = compute_key(i,node_dis_sample[i].sml_than_sht_node[j] );
            insert( key,i,node_dis_sample[i].sml_than_sht_node[j],node_dis_sample[i].sml_than_sht_dis[j] );
        }//for
    }//for


    //display();




}//store_to_hash_tb()


int compute_key(int node1,int node2 ) {
   return ((node1+1)*(node2+1))%hash_prime%hash_tb_size;
}

int hashCode(int key) {
   return key % SIZE;
}

void insert(int key,int node1,int node2,double distance) {

    struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
    item->node1 = node1;
    item->node2 = node2;
    item->distance = distance;
    item->key = key;
    item->next = NULL;
    //get the hash
    int hashIndex = hashCode(key);
    struct DataItem *now_ptr = (struct DataItem*) malloc(sizeof(struct DataItem));
    now_ptr = hashArray[hashIndex];
    //printf( " hashArray[hashIndex]:%d", hashArray[hashIndex] );
    //printf( " now_ptr:%d\n", now_ptr );
    if ( now_ptr == NULL ){
        //printf( "hashIndex:%d is null.\n",hashIndex );
        hashArray[hashIndex] = item;
    }//if
    else {

        while ( now_ptr->next != NULL ){
            //printf(" (%d,%d,%d,%lf)\n",now_ptr->key,now_ptr->node1,now_ptr->node2,now_ptr->distance);
            now_ptr = now_ptr->next;
        }//while

        //printf( "hashIndex:%d is not null.\n",hashIndex );
        now_ptr->next = item;
        //printf(" (%d,%d,%d,%lf)\n",now_ptr->key,now_ptr->node1,now_ptr->node2,now_ptr->distance);
    }//else
}



struct DataItem *search(int key) {
   //get the hash
   int hashIndex = hashCode(key);

   //move in array until an empty
   while(hashArray[hashIndex] != NULL) {

      if(hashArray[hashIndex]->key == key)
         return hashArray[hashIndex];

      //go to next cell
      ++hashIndex;

      //wrap around the table
      hashIndex %= SIZE;
   }

   return NULL;
}



double pair_distance_search( int src,int des ){
    //printf( "src:%d des:%d\n",src,des );
    int i,j,k;
    int index;
    double distance;
    double src_to_des_dis ,des_to_src_dis;


    //case 1: src = des ,return 0
    //case 2: src is sample and des is sample ,directly search des to src
    //case 3: src is sample and des is not sample ,directly search des to src
    //case 4: src is sample and des is not sample ,directly search  src to des
    //case 5:  src is not sample and des is not sample ,compare  src_to_des and des_to_src
    if ( src == des )
        return 0.0;
    else if ( node_dis_sample[src].type == 's' && node_dis_sample[des].type == 's' ){
        for ( i = 0 ;i<sample_node_num;i++ ){
            if ( sample_node[i] == des )
                index = i;
        }//for
        distance = node_dis_sample[src].distance[index] ;
        return distance;
    }//if
    else if (  node_dis_sample[src].type == 's' && node_dis_sample[des].type == 'n'  ){
        //printf( "s,n\n" );
        for ( i = 0 ;i<sample_node_num;i++ ){
            if ( sample_node[i] == src )
                index = i;
        }//for

        //printf( "index;%d\n",index );
        distance = node_dis_sample[des].distance[index] ;
        return distance;
    }//else if
    else if (  node_dis_sample[src].type == 'n' && node_dis_sample[des].type == 's'  ){
        for ( i = 0 ;i<sample_node_num;i++ ){
            if ( sample_node[i] == des )
                index = i;
        }//for
        distance = node_dis_sample[src].distance[index] ;
        return distance;
    }//else if

    else if (   node_dis_sample[src].type == 'n' && node_dis_sample[des].type == 'n'  ){

        //compute source to destination
        //printf( "src:%d des:%d\n",src,des );
        //printf( "=========================compute source to destination=====================\n" );
        bool is_smaller_than_shortest_dis = false;
        for ( i = 0 ;i<node_dis_sample[src].sml_than_sht_num;i++ ){
            if ( des == node_dis_sample[src].sml_than_sht_node[i]  ){
                //printf( "is_smaller_than_shortest_dis is true\n" );
                is_smaller_than_shortest_dis = true;
                src_to_des_dis = node_dis_sample[src].sml_than_sht_dis[i];
            }//if
        }//for

        if ( is_smaller_than_shortest_dis == false ){
            //printf( "is_smaller_than_shortest_dis is false\n" );
            src_to_des_dis = node_dis_sample[src].shortest_dis
                            + node_dis_sample[des].distance[node_dis_sample[src].shortest_dis_sample_index];
            //printf( "node_dis_sample[src].shortest_dis:%lf\n",node_dis_sample[src].shortest_dis );
            //printf( "node_dis_sample[des].distance[node_dis_sample[src].shortest_dis_sample_index]:%lf\n",node_dis_sample[des].distance[node_dis_sample[src].shortest_dis_sample_index] );
            //printf ( "src_to_des_dis:%lf\n",src_to_des_dis );
        }//if
        //compute destination to source

        //printf( "=========================compute destination to source=====================\n" );
        is_smaller_than_shortest_dis = false;
        for ( i = 0 ;i<node_dis_sample[des].sml_than_sht_num;i++ ){
            if ( src == node_dis_sample[des].sml_than_sht_node[i]  ){
                 //printf( "is_smaller_than_shortest_dis is true\n" );
                is_smaller_than_shortest_dis = true;
                //printf( "node_dis_sample[des].sml_than_sht_dis[%d]:%lf\n",i,node_dis_sample[des].sml_than_sht_dis[i] );
                //printf( "src:%d,i:%d\n",src,i );
                des_to_src_dis = node_dis_sample[des].sml_than_sht_dis[i];
                //printf ( "des_to_src_dis:%lf\n",des_to_src_dis );
            }//if
        }//for

        if ( is_smaller_than_shortest_dis == false ){
            //printf( "is_smaller_than_shortest_dis is false\n" );
            des_to_src_dis = node_dis_sample[des].shortest_dis
                            + node_dis_sample[src].distance[node_dis_sample[des].shortest_dis_sample_index];
            //printf( "node_dis_sample[des].shortest_dis:%lf\n",node_dis_sample[des].shortest_dis );
            //printf( "node_dis_sample[src].distance[node_dis_sample[des].shortest_dis_sample_index]:%lf\n",node_dis_sample[src].distance[node_dis_sample[des].shortest_dis_sample_index] );
            //printf ( "des_to_src_dis:%lf\n",des_to_src_dis );
        }//if


        //take smaller distance

        //printf ( "des_to_src_dis:%lf\n",des_to_src_dis );

        if ( src_to_des_dis<des_to_src_dis )
            distance = src_to_des_dis;
        else
            distance = des_to_src_dis;

        return distance;
    }//else if

}//pair_distance_search


void display() {

    int i = 0,num = 0;
    printf( "key , node1, node2,distance\n" );
    for(i = 0; i<SIZE; i++) {
        if(hashArray[i] != NULL){
            struct DataItem *now_ptr = (struct DataItem*) malloc(sizeof(struct DataItem));
            now_ptr = hashArray[i];
            while ( now_ptr !=NULL  ){
                printf(" (%d,%d,%d,%lf)\n",now_ptr->key,now_ptr->node1,now_ptr->node2,now_ptr->distance);
                now_ptr = now_ptr->next;
                num++;
            }//while
        }//if
    }

    printf("num:%d\n",num);


    /*
    int i = 0;
    printf( "key , node1, node2,distance\n" );
    for(i = 0; i<SIZE; i++) {

        if(hashArray[i] != NULL)
            printf(" (%d,%d,%d,%lf)\n",hashArray[i]->key,hashArray[i]->node1,hashArray[i]->node2,hashArray[i]->distance);
        else
            printf(" ~~ ");
    }

    printf("\n");
    */
}



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
    scanf("%d",&hash_prime);
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
        link_info[i].distance = d_input ;
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
        new_ptr->weight = link_info[i].distance;
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
        new_ptr2->weight = link_info[i].distance;
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
                wei_graph[i][now_ptr->next->point] = now_ptr->next->weight;
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






void Debug_node_dis_sample(){
    int i,j,k;

    printf( "================sample node====================\n" );
    for( i=0;i<nd_dis_smp_i ;i++ ){
        printf( "sample_node[%d]:%d\n",i,sample_node[i] );
    }//for

    printf( "================Debug_node_dis_sample====================\n" );
    for ( i = 0 ; i<node_num;i++ ){
        printf("node_dis_sample[%d].type:%c\n",i,node_dis_sample[i].type);
        printf("node_dis_sample[%d].shortest_dis:%lf\n",i,node_dis_sample[i].shortest_dis);
        printf("node_dis_sample[%d].sml_than_sht_num:%d\n",i,node_dis_sample[i].sml_than_sht_num);
        printf("node_dis_sample[%d].shortest_dis_sample:%d\n",i,node_dis_sample[i].shortest_dis_sample);
        printf("node_dis_sample[%d].shortest_dis_sample_index:%d\n",i,node_dis_sample[i].shortest_dis_sample_index);
        for( j=0;j<nd_dis_smp_i ;j++ ){
            printf("node_dis_sample[%d].distance[%d]:%lf\n",i,j,node_dis_sample[i].distance[j]);
        }//for
        for ( j = 0;j<node_dis_sample[i].sml_than_sht_num;j++ ){
            printf( "node_dis_sample[%d].sml_than_sht_node[%d]:%d\n",i,j,node_dis_sample[i].sml_than_sht_node[j] );
            printf( "node_dis_sample[%d].sml_than_sht_dis[%d]:%lf\n",i,j,node_dis_sample[i].sml_than_sht_dis[j] );
        }//for
    }//for
}//Debug_node_dis_sample
