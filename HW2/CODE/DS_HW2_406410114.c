

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//node_struct
typedef struct node{
	int point;
	char type[10] ;
	int white_index; // sorting
	int heap;
	struct node *original_path;
	struct node *next_path;
	struct node *solution_path;
} node;

//update_buffer
typedef struct update_infor{
	int update_point ;
	int distance_max ;
	int white_index;
	int heap_same;
} update_infor;


int number = 0,turn = 1;
void Debug_Print_Node_Point( node node_point[],int number );
node * ReadInput( node node_point[], char *filename );
void * PointRedConnectNextpath( node node_point[]);
void Print_Node_Point( node node_point[],int number );
void * AddPointType( node node_point[]);
//void * Odd_Shortcut_Phase( node node_point[],int up_info[]);
void * Odd_Shortcut_Phase_Mnay( node node_point[],update_infor up_info[]);
//void * Even_Prune_Phase( node node_point[],int update_point,int distance_max);
void * Even_Prune_Phase_Many( node node_point[],update_infor up_info[]);
int * IsFinishOddEven( node node_point[]);
void * PointBlackDisconnectNextpath( node node_point[]);
int * IsFinishRed( node node_point[]);
int * IsFinishBlack( node node_point[]);
update_infor up_info_many[1000] = {0,0};


int main()
{
//*****************************************
//1.ReadInput
//2.add type
//3.PointRedConnectNextpath
//4.odd and even
//5.PointBlackDisconnectNextpath
//6.print_result
//******************************************

    node node_point[1000];
    int round = 0,i = 0,isfinoddeven = 0,j = 0,isfinred = 0,isfinblack=0;
    int debug_i =0;
    int result[1000][100];

    //1.ReadInput
    ReadInput( node_point, "input.txt" );

    //2.add type

    //if no point return 0
    if ( number == 0 )
        return 0;
    AddPointType( node_point);
    //Debug_Print_Node_Point( node_point,number );
    //store result
    for ( i = 0 ; i < number ; i++ ){
        if ( node_point[i].original_path == NULL )
            result[round][i] = -1;
        else
            result[round][i] = node_point[i].original_path->point ;
    }//for
    round++;
    //Print_Node_Point( node_point,number );
    //printf( "ori:\n" );
    //Debug_Print_Node_Point( node_point,number );

    isfinred = IsFinishRed( node_point);
    //3.PointRedConnectNextpath

    for ( j =0 ; j < number ; j++ ){
        if ( isfinred == 0 )
            break;
        if((strcmp(node_point[j].type,"red") == 0)){
            PointRedConnectNextpath( node_point);
            //Print_Node_Point( node_point,number );
            //store result
            for ( i = 0 ; i < number ; i++ ){
                if ( node_point[i].original_path == NULL )
                    result[round][i] = -1;
                else
                    result[round][i] = node_point[i].original_path->point ;
            }//for
            round++;
            break;
        }//if
    }//for

    //printf( "red:\n" );
    //Debug_Print_Node_Point( node_point,number );
    //determine IsFinishOddEven
    isfinoddeven = IsFinishOddEven( node_point);

    //4.odd and even
    for ( round ; isfinoddeven == 0  ;debug_i++ ){
        // reset buffer
        for ( j = 0 ; j <1000;j++ ){
            up_info_many[j].update_point = 0;
            up_info_many[j].distance_max = 0;
            up_info_many[j].white_index = 0;
        }//for


        //odd
        Odd_Shortcut_Phase_Mnay( node_point,up_info_many);
        //printf( "odd_round%d:\n",round );
        //Debug_Print_Node_Point( node_point,number );
        //store result
        for ( i = 0 ; i < number ; i++ ){
            if ( node_point[i].original_path == NULL )
                result[round][i] = -1;
            else
                result[round][i] = node_point[i].original_path->point ;
        }//for
        round++;


        //check is_finish
        if ( IsFinishOddEven( node_point)==true )
            break;

        //even
        Even_Prune_Phase_Many( node_point,up_info_many);
        //printf( "even_round%d:\n",round );
        //Debug_Print_Node_Point( node_point,number );

        //store result
        for ( i = 0 ; i < number ; i++ ){
            if ( node_point[i].original_path == NULL )
                result[round][i] = -1;
            else
                result[round][i] = node_point[i].original_path->point ;
        }//for
        round++;
        //Debug_Print_Node_Point( node_point,number );
        //Print_Node_Point( node_point,number );
        isfinoddeven = IsFinishOddEven( node_point);

    }//for

    //5.PointBlackDisconnectNextpath

    isfinblack =IsFinishBlack( node_point);
    for ( j =0 ; j < number && isfinblack!=0 ; j++ ){
        if((strcmp(node_point[j].type,"black") == 0)){
            PointBlackDisconnectNextpath( node_point);
            //Print_Node_Point( node_point,number );
            //store result
            for ( i = 0 ; i < number ; i++ ){
                if ( node_point[i].original_path == NULL )
                    result[round][i] = -1;
                else
                    result[round][i] = node_point[i].original_path->point ;
            }//for
            round++;
            break;
        }//if
    }//for

    //6.print_result
    printf( "%d\n",round );
    for ( i = 0 ; i < round ; i++ ){
        for ( j = 0; j <number ; j++ ){
            printf( "%d",result[i][j] );
            if ( j < number-1 )
                printf( " " );
        }//for
        printf( "\n" );
    }//for
    //Print_Node_Point( node_point,number );
    //Debug_Print_Node_Point( node_point,number );


    return 0 ;
}//main

//1.ReadInput
node * ReadInput( node node_point[], char *filename ){
//*****************************************
//point: point number
//type : red or black or white
// white_index  :  white_point index
// node *original_path:solid line
// node *next_path ; dotted line
//  node *solution_path : answer
//******************************************
    int input_number = 0;
    int node_number = 0 ;

//determine rule
    int i=0;
    int j=0;
//file input rule

    FILE *fin;
	fin=fopen(filename,"r");
    fscanf(fin,"%d\n",&number);
    //printf( "number:%d\n",number );


    for (i=0;i<number;i++)
    {
        node_point[i].point = i ;
    }//for

    for (i=0;i<number;i++)
    {
        fscanf(fin,"%d",&node_number);
        //printf("node_number%d\n",node_number);
        if (node_number == -1)
            node_point[i].original_path = NULL ;
        else
            node_point[i].original_path = &node_point[node_number] ;

        if ( i < number-1 )
            fscanf(fin," ");
        //printf("node_point[i].original_path:%d\n",node_point[i].original_path);
    }//for
    fscanf(fin,"\n");

    for (i=0;i<number;i++)
    {
        fscanf(fin,"%d ",&node_number);
        if (node_number == -1) {
            node_point[i].next_path = NULL ;
            node_point[i].solution_path = NULL ;
        }//if
        else{
            node_point[i].next_path = &node_point[node_number] ;
            node_point[i].solution_path = &node_point[node_number] ;
        }//else
        if ( i < number-1 )
            fscanf(fin," ");
        node_point[i].heap = 1;
    }//for

    fclose(fin);
    return node_point;
}//ReadInput



void * AddPointType( node node_point[]){
//*****************************************
//red : original_path == NULL
//black : next_path == NULL
//white: d and else
//******************************************
    int i,white_index;
    white_index = 0;
    node *now_point;
    now_point = malloc(sizeof(now_point));
    for ( i = 0 ; i < number ; i++ ){
        if ( i == number-1 ){
            strcpy(node_point[i].type,"white");
            node_point[i].white_index = white_index;
            white_index++;
            //printf( "%d\n",i );
            //printf( "node_point[i].white_index:%d\n",node_point[i].white_index );
        }//if
        else if(node_point[i].original_path == NULL)
            strcpy(node_point[i].type,"red");
        else if ( node_point[i].next_path == NULL )
            strcpy(node_point[i].type,"black");
        else{
            strcpy(node_point[i].type,"white");
            //node_point[i].white_index = white_index;
            //white_index++;
            //printf( "%d\n",i );
            //printf( "node_point[i].white_index:%d\n",node_point[i].white_index );
        }//else
    }//for

    if ( strcmp( node_point[0].type,"white" ) == 0 ){
        node_point[0].white_index = white_index;
        white_index++;
    }//if

    now_point = node_point[0].original_path;
    while ( now_point != NULL ){
        //printf( "now_point->:%d\n",now_point->point );
        if ( strcmp( now_point->type,"white" ) == 0 ){
            now_point->white_index = white_index;
            white_index++;
        }//while

        now_point = now_point->original_path;
    }//while
}//ReadInput


void * PointRedConnectNextpath( node node_point[]){
//*****************************************
//if red connect dotted line , disconnect solid line
//******************************************
    int i;
    for ( i = 0 ; i < number ; i++ ){
        if((strcmp(node_point[i].type,"red") == 0)){
            node_point[i].original_path = node_point[i].next_path;
            node_point[i].next_path = NULL;
        }//if
    }//for
}//PointRedConnectNextpath

int * IsFinishOddEven( node node_point[]){
//*****************************************
//determine IsFinishOddEven ,if Finish  return 1 else return 0
//******************************************
    int i;
    for ( i = 0 ; i < number ; i++ ){
        if ( node_point[i].solution_path == NULL )
            continue;
        if(node_point[i].original_path != node_point[i].solution_path)
            return 0;
    }//for

    return 1;//true
}//PointRedConnectNextpath

int * IsFinishRed( node node_point[]){
//*****************************************
//determine IsFinishhRed ,if Finish  return 1 else return 0
//******************************************
    int i;
    for ( i = 0 ; i < number ; i++ ){
        if ( (strcmp( node_point[i].type,"red" ) == 0))
            return 1; //true
    }//for

    return 0; //false
}//PointRedConnectNextpath


int * IsFinishBlack( node node_point[]){
//*****************************************
//determine IsFinishBlack ,if Finish  return 1 else return 0
//******************************************
    int i;
    for ( i = 0 ; i < number ; i++ ){
        if ( (strcmp( node_point[i].type,"black" ) == 0))
            return 1; //true
    }//for

    return 0; //false
}//PointRedConnectNextpath






void * Odd_Shortcut_Phase_Mnay( node node_point[],update_infor up_info[]){
//*****************************************
//1.find white and record the point number and  distance
//2.check update_point
//    (1)select farthest distance white_point  , if there are more than one , the smallest_point_number priority.
//    (2)check no_conflict_white_point update together
//    (3)return the update numbers
//******************************************

    //single update
    node *now_point;
    now_point = malloc(sizeof(now_point));
    int i,j,k;
    int array_i = 0,distance_max = 0,update_point = 0,proposol_point_white_index_index = 0;
    int up_i = 0 ;
    int distance[1000] = {0};
    int start_point[1000] = {0};
    int ori_pheap[1000] = {0};
    int next_pheap[1000] = {0};
    int white_in[1000] = {0};
    int proposol_point_white_index[1000] = {0};

//1.find white and record the point number and  distance
    for ( i = 0 ; i < number ; i++ ){
        if ( strcmp( node_point[i].type,"white" ) == 0 ){
            //printf( "i:%d\n",i );
            //printf("node_point[i].point: %d\n ",node_point[i].point);
            //next_path pass
            if ( i == number-1 )
                continue;

            //printf( "it is not final point.\n" );
            if ( node_point[i].original_path->point ==  node_point[i].solution_path->point )
                continue;
            now_point = node_point[i].next_path;
            if ( now_point == NULL )
                continue;

            //printf( "%s",now_point -> type );
            for ( j = 0 ; j < number ; j++ ){
                //printf("now_point->point: %d\n",now_point->point);
                if ( strcmp(now_point->type,"white" ) == 0  ){
                    //printf("strcmp(now_point->type,white ) == 0 point: %d\n",now_point->next_path->point);
                    start_point[array_i] = i;
                    distance[array_i] = now_point->white_index - node_point[i].white_index;
                    white_in[array_i] = node_point[i].white_index;
                    ori_pheap[array_i] = node_point[i].heap;
                    next_pheap[array_i] = now_point->heap;
/*
                    printf(  "start_point[array_i]:%d\n",start_point[array_i]);
                    printf(  "distance[array_i]:%d\n",distance[array_i]);
                    printf( "white_in[array_i]:%d\n",white_in[array_i]);
                    printf( "ori_pheap[array_i]:%d\n",ori_pheap[array_i]);
                    printf( "next_pheap[array_i]:%d\n",next_pheap[array_i] );
                    proposol_point_white_index[array_i] = now_point->white_index;
*/
                    array_i++;
                    //printf( "array_i:%d\n",array_i );
                    break;
                }//if
                else{
                    now_point = now_point -> original_path;
                }//else
            }//for
        }//if
    }//for

    //printf( "array_i:%d\n",array_i );
    //find distance_max
    //printf( "distance[array_i]:");

//2.check update_point
//    (1)select farthest distance white_point  , if there are more than one , the smallest_point_number priority.

    //printf( "distance[0]:%d\n",distance[0] );

    //printf( "array_i:%d\n",array_i );
    for ( i = 0 ; i < array_i ; i++ ){
        //printf( "%d ",distance[i] );
        if ( i == 0 )
            distance_max = distance[0];
        if ( distance[i] > distance_max )
            distance_max = distance[i] ;
    }//for



//2.check update_point
//    (2)check no_conflict_white_point update together

    for ( i = 0 ; i < array_i ; i++ ){
        //printf( "start_point[i]:%d\n",start_point[i] );
        //printf( "distance[i]:%d\n",distance[i] );
        if ( distance[i] == distance_max ){
            update_point = start_point[i];
            proposol_point_white_index_index = i;
            up_info[up_i].update_point = update_point; // insert first update point
            up_info[up_i].distance_max = distance_max;
            up_info[up_i].white_index= node_point[update_point].white_index;
            //if heap is same = 1 , else = 0
            if ( ori_pheap[up_i] == next_pheap[up_i]  )
                up_info[up_i].heap_same = 1;
            else
                up_info[up_i].heap_same = 0;
            //printf( "distance_max:%d\n",distance_max );
            up_i++;
            break;
        }//if
    }//for

    //printf( "distance[0]:%d\n",distance[0] );
    bool confi = false ;

//2.check update_point
//    (3)return the update numbers

//isconflict
//another point >= update point + distance && another point + distance > update point + distance
//another point < update point && another point + distance <= update point

// if equal first pass
    //printf( "array_i:%d\n",array_i );
    for ( k = distance_max; k > 0 ; k--  ){ //距離下去找
        //printf( "k:%d\n",k );
        for ( i = 0 ; i < array_i ; i++ ){ //找每個點
            //printf( "i:%d\n",i );
            if ( distance[i] != k )
                continue;
            for ( j = 0;j < up_i  ;j++ ){ //找衝突
                if ( start_point[i] == up_info[j].update_point ){ //等於就跳過
                    confi = true;
                    break ;
                }//if

                if ( ori_pheap[i] != next_pheap[i] )
                    confi = false;
                else if ( (distance[i] < 0) //距離為負
                    || (((node_point[start_point[i]].white_index < node_point[up_info[j].update_point].white_index))
                        &&(node_point[start_point[i]].white_index  + distance[i]) > node_point[up_info[j].update_point].white_index)  // 左邊進到中間
                    || ((node_point[start_point[i]].white_index >= node_point[up_info[j].update_point].white_index) // 中間不含最右的點
                        && (node_point[start_point[i]].white_index < node_point[up_info[j].update_point].white_index +up_info[j].distance_max ))){
                    confi = true;
                    //printf( "start_point[i]:%d is confi\n",start_point[i] );
                    //printf( "node_point[start_point[i]].white_index  + distance[i]  is confi:%d\n",node_point[start_point[i]].white_index  + distance[i] );
                }//if
                //printf( "start_point[i]:%d is not confi\n",start_point[i] );
                //printf( "node_point[start_point[i]].white_index  + distance[i]  is not confi:%d\n",node_point[start_point[i]].white_index  + distance[i] );
            }//for

            if (confi == true){
                confi = false;
                continue ;
            }//if
            else{//confi == false
                /*
                printf( "i:%d\n",i );
                printf( "start_point[i]:%d\n",start_point[i] );
                printf( "distance[i]:%d\n",distance[i]);
                printf( "white_in[i]:%d\n",white_in[i] );
                printf( "up_info[i].distance_max:%d\n",up_info[i].distance_max );
                printf( "white_index:%d\n",node_point[up_info[j].update_point].white_index );
                printf( "distance_max:%d\n",up_info[j].distance_max );
                */

                up_info[up_i].update_point = start_point[i];
                up_info[up_i].distance_max = distance[i];
                up_info[up_i].white_index = white_in[i];
                if ( ori_pheap[up_i] == next_pheap[up_i]  ){
                    up_info[up_i].heap_same = 1;
                    //printf("up_info[up_i].heap_same:%d\n",up_info[up_i].heap_same);
                }//if
                else
                    up_info[up_i].heap_same = 0;
            //printf( "distance_max:%d\n",distance_max );
                up_i++;
               // printf("up_i:%d\n",up_i);
                confi = true;
            }//else
        }//for
    }//for

    /*
    for ( i = 0 ; i < up_i ; i++ ){
        printf( "i:%d\n",i );
        printf( "up_info[i].update_point:%d\n",up_info[i].update_point );
        printf( "up_info[i].distance_max:%d\n",up_info[i].distance_max );
    }//for
    */


    //printf( "up_i:%d\n",up_i );
    //update
    //1. point merge
    //2.original_path = next_path
    //3.next_path = NULL
    //printf( "up_i:%d\n",up_i );

    int max_heap = 0;
    for ( i = 0 ; i < up_i ; i++ ){
        //printf( "update point :%d\n",node_point[up_info[i].update_point].point );
        //printf( "after update:%d\n",node_point[up_info[i].update_point].solution_path->point);
        now_point = node_point[up_info[i].update_point].original_path;
        //printf( "i:%d\n",i );
        if ( (up_info[i].heap_same == 1) &&(up_info[i].distance_max > 1)   ) {
            for (  ;  now_point->white_index > 0 && now_point->white_index <= node_point[up_info[i].update_point].white_index + up_info[i].distance_max  ; ){
                //printf( "test\n" );
                if ( now_point->white_index < node_point[up_info[i].update_point].white_index + up_info[i].distance_max ){
                    now_point -> heap = 2;
                    now_point = now_point -> original_path;
                }//if
                else{
                    //printf( "now_point ->white_index:%d\n",now_point ->white_index );
                    max_heap = now_point -> heap;
                    break;
                    //now_point = now_point -> original_path;
                }//else
            }//for
        }//if
        node_point[up_info[i].update_point].heap = max_heap;
        node_point[up_info[i].update_point].white_index = node_point[up_info[i].update_point].white_index + up_info[i].distance_max;
        node_point[up_info[i].update_point].original_path = node_point[up_info[i].update_point].solution_path;
        node_point[up_info[i].update_point].next_path = NULL;
    }//for

}//Odd_Shortcut_phase


void * Even_Prune_Phase_Many( node node_point[],update_infor up_info[]){
//*****************************************
//1.read update_point and distance
//2.update all nodes in the middle
//******************************************
    int i,j,k;
    int array_i = 0;
    int num_of_up = 0 ;
    int ori_white_index = 0 ;
    node *now_point;
    now_point = malloc(sizeof(now_point));
    /*
    for ( i = 0 ; i < 2 ; i++ ){
        printf( "i:%d\n",i );
        printf( "up_info[i].update_point:%d\n",up_info[i].update_point );
        printf( "up_info[i].distance_max:%d\n",up_info[i].distance_max );
    }//for
    */

//1.read update_point and distance
//2.update all nodes in the middle
    for ( array_i = 0 ;array_i <number ;array_i++  ){
        //num_of_up = 0 ;
        //printf( "array_i:%d\n",array_i );
        //printf( "up_info[array_i].white_index:%d\n",up_info[array_i].white_index );
        //printf( "up_info[array_i].distance_max:%d\n",up_info[array_i].distance_max );
        //printf( "up_info[array_i].update_point:%d\n",up_info[array_i].update_point );
        if (  up_info[array_i].distance_max == 0  )
            break;
        for ( i = 0 ; i < number-1 ;i++ ){
            if ( node_point[i].original_path == node_point[i].solution_path )
                continue;
            //找下一個white  若遇到red繼續找
            if ( (strcmp( node_point[i].type,"white" ) != 0) )
                continue;

            now_point = node_point[i].next_path;
            while ( (strcmp( now_point -> type,"white" ) != 0 )){
                //printf( "now_point:%d\n",now_point->point );
                //printf( "now_point->original_path->type:%s\n",now_point->original_path->type );
                now_point = now_point->original_path;
            }//while
                //printf( "now_point:%d\n",now_point->point );
                //printf( "now_point->original_path->type:%s\n",now_point->original_path->type );
            if ( (strcmp( node_point[i].type,"white" ) == 0)
                &&( node_point[i].white_index > up_info[array_i].white_index)
                && ( node_point[i].white_index < up_info[array_i].white_index + up_info[array_i].distance_max)){
                //printf( "node_point[i].point:%d\n",node_point[i].point );

                //要更新的點比下一個大
                if ( node_point[i].white_index > now_point->white_index ){
                    now_point->white_index = node_point[i].white_index ;
                    now_point->heap = node_point[i].heap;
                }//if
                else{
                    ori_white_index = node_point[i].white_index;
                    node_point[i].white_index = now_point->white_index ;
                    node_point[i].heap = now_point->heap;
                    for ( k = 0 ; k < number-1 ;k++  ) {
                        if ( node_point[j].white_index == ori_white_index )
                            node_point[j].white_index = node_point[i].white_index;

                    }//for
                }//else

                node_point[i].original_path = node_point[i].next_path;
                node_point[i].next_path = NULL;
            }//if
        }//for
    }//for

            //if ( i == up_info[array_i].update_point ) {
        /*
        for ( j = up_info[array_i].update_point+1  ; j < number-1 ; j++ ){
            //update update_point < white point < white_index + distance_max

            if ( num_of_up+1 >= up_info[array_i].distance_max)
                break;
           // printf( "node_point[j].point is proposal.:%d\n",node_point[j].point );
            if ( (strcmp( node_point[j].type,"white" ) == 0)) {
                if ( node_point[j].original_path->point ==  node_point[j].solution_path->point )
                    continue;
                else{
                  //  printf( "node_point[j].point is updated.:%d\n",node_point[j].point);

                    num_of_up++;
                }//esle
            }//if
        }//for
        */
            //}//if

}//Even_Prune_Phase

void * PointBlackDisconnectNextpath( node node_point[]){
//*****************************************
//final round : if black disconnect solid line
//******************************************
    int i;
    for ( i = 0 ; i < number ; i++ ){
        if(strcmp(node_point[i].type,"black") == 0){
            node_point[i].original_path = NULL;
        }//if
    }//for
}//PointBlackDisconnectNextpath


//*****************************************
//below is Debug Code
//******************************************

void Print_Node_Point( node node_point[],int number ){
    int i =0;
    //printf( "number:%d\n",number );
    for (i=0;i<number;i++)
    {
        if ( node_point[i].original_path == NULL )
            printf( "-1" );
        else
            printf( "%d",node_point[i].original_path->point );

        if ( i<number-1 )
            printf(" ");
        else
            printf( "\n" );
    }//for
}//Print_Node_Point

void Debug_Print_Node_Point( node node_point[],int number ){
    int i =0;
    //printf( "number:%d\n",number );
    for (i=0;i<number;i++)
    {
        printf( "point:%d ",node_point[i].point );
        printf( "type:%s ",node_point[i].type );
        printf( "white_index:%d ",node_point[i].white_index );
        printf( "heap:%d ",node_point[i].heap );
        if ( node_point[i].original_path == NULL )
            printf( "original_path:-1 " );
        else
            printf( "original_path:%d ",node_point[i].original_path->point );

        if ( node_point[i].solution_path == NULL  )
            printf( "solution_path:-1 " );
        else
            printf( "solution_path:%d ",node_point[i].solution_path->point );

        if ( node_point[i].next_path == NULL  )
            printf( "next_path:-1\n" );
        else
            printf( "next_path:%d\n",node_point[i].next_path->point );
    }//for
}//Debug_Print_Node_Point()


