#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define bool int
#define false 0
#define true 1


main()
{
// rule value
    int rule_number = 0;
    char rule_point1[ 100 ][1000] = {'/0'}; //rule_point1
    char rule_point2[ 100 ][1000] = {'/0'}; //rule_point2
    char rule_port[ 100 ][1000] = {'/0'};   //rule_port

// input value
    int input_number = 0;
    char input_point1[ 100 ][1000] = {'/0'};
    char input_point2[ 100 ][1000] = {'/0'};


//determine rule
    //char star[1] = "*";
    bool isfind_port = true;
    int i=0;
    int j=0;

//file input rule
    FILE *fin;
	fin=fopen("input.txt","r");
    fscanf(fin,"%d\n",&rule_number);
    //printf( "rule_number:%d\n",rule_number );
    for (i=0;i<rule_number;i++)
    {
        fscanf(fin,"%s ",&rule_point1[i]);
        fscanf(fin,"%s ",&rule_point2[i]);
        fscanf(fin,"%s\n",&rule_port[i]);
    }//for

//file input input_point
    fscanf(fin,"%d\n",&input_number);
    for (i=0;i<input_number;i++)
    {
        fscanf(fin,"%s ",&input_point1[i]);
        fscanf(fin,"%s\n",&input_point2[i]);

    }//for
    fclose(fin);

//output file
    FILE *fout;
    fout = fopen( "output.txt","w" );
    fprintf( fout,"%d\n",input_number );
    for (i=0;i<input_number;i++){
        fprintf( fout,"%s ",input_point1[i] );
        fprintf( fout, "%s ",input_point2[i] );

//determine
//****************************************************
//if Fully equal
//else if number1 = * , input_point_number2 is correspond to rule_point_number2
//else if number2 = * , input_point_number1 is correspond to rule_point_number1
//else if number1 = * ,number2 = *
//esle drop
//****************************************************
        for (j=0;j<rule_number;j++){
            //printf( "i:%d\n",i );
            //printf( "j:%d\n",j );
            //printf( "input_point1[i]:%s\n",input_point1[i] );
            //printf( "rule_point1[j]:%s\n",rule_point1[j] );
            //printf( "strcmp(rule_point1[j],star)%d\n",strcmp(rule_point1[j],"*") );
            if ( strcmp(input_point1[i],rule_point1[j])==0 && strcmp(input_point2[i],rule_point2[j] )==0 ) {
                fprintf( fout, "%s",rule_port[j] );
                isfind_port = true;
                //printf( "strcmp(input_point1[i],rule_point1[j]) && strcmp(input_point2[i],rule_point2[j] )\n" );
                //printf( "rule_port[j]:%s\n",rule_port[j] );
                break;
            }//if

            else if( strcmp(rule_point1[j],"*")==0 && strcmp(input_point2[i],rule_point2[j] )==0 ){
                fprintf( fout, "%s",rule_port[j] );
                isfind_port = true;
                //printf( "strcmp(rule_point1[j],star) && strcmp(input_point2[i],rule_point2[j] )\n" );
                //printf( "rule_port[j]:%s\n",rule_port[j] );
                break;
            }//else if

            else if( strcmp(input_point1[i],rule_point1[j] )==0 && strcmp(rule_point2[j],"*")==0 ){
                fprintf( fout, "%s",rule_port[j] );
                isfind_port = true;
                //printf( "strcmp(input_point1[i],rule_point1[j] ) && strcmp(rule_point2[j],star)\n" );
                //printf( "rule_port[j]:%s\n",rule_port[j] );
                break;
            }//else if
            else if( strcmp(rule_point1[j], "*")==0 && strcmp(rule_point2[j],"*")==0 ){
                fprintf( fout, "%s",rule_port[j] );
                isfind_port = true;
                //printf( "strcmp(rule_point1[j], star) && strcmp(rule_point2[j],star)\n" );
                //printf( "rule_port[j]:%s\n",rule_port[j] );
                break;
            }//else if
            else{
                isfind_port = false;
            }//else
        }//for
        if ( isfind_port == false ){
            fprintf( fout,  "drop" );
            isfind_port = true ;
        }//if

        fprintf( fout, "\n" );

    }//for
    fclose(fout);


//debug

    printf("%d\n",rule_number);

    for (i=0;i<rule_number;i++)
    {
        printf("%s ",rule_point1[i]);
        printf("%s ",rule_point2[i]);
        printf("%s\n",rule_port[i]);
    }//for



    printf("%d\n",input_number);
    for (i=0;i<input_number;i++)
    {
        printf("%s ",input_point1[i]);
        printf("%s\n",input_point2[i]);

    }//for

}//main
