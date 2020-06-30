#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE 1024
#define bool int
#define false 0
#define true 1


main()
{
    int rule_number = 0;
    int point_number = 0;
    char rule_point1[ 1024 ];
    char rule_point2[ 1024 ];
    char rule_port[ 1024 ];
    char input_point1[ 1024 ];
    char input_point2[ 1024 ];
    char star[1] = "*";
    bool isfind_port = true;

    FILE *fpr;
    fpr=fopen("input.txt","r");
    fscanf(fpr,"%d\n",&rule_number);
    for (int i=0;i<rule_number;i++)
    {

        fscanf(fpr,"%c ",&rule_point1[i]);
        fscanf(fpr,"%c ",&rule_point2[i]);
        fscanf(fpr,"%c\n",&rule_port[i]);

    }//for




    fscanf(fpr,"%d\n",&point_number);
    for (int i=0;i<point_number;i++)
    {

        fscanf(fpr,"%c ",&input_point1[i]);
        fscanf(fpr,"%c\n",&input_point2[i]);

    }//for

    fclose(fpr);

//output file

    FILE *fout;
    fout = fopen( "output.txt","w" );
    for (int i=0;i<point_number;i++){
        fprintf( fout,"%c ",input_point1[i] );
        fprintf( fout, "%c ",input_point2[i] );
        for (int j=0;j<rule_number;j++){
            //printf( "rule_point1[j]:%c",rule_point1[j] );
            //printf( "rule_point2[j]:%c",rule_point2[j] );
            if ( (input_point1[i] == rule_point1[j]) && (input_point2[i] == rule_point2[j] ) ) {
                fprintf( fout, "%c",rule_port[j] );
                isfind_port = true;
                break;
            }//if
            else if( (rule_point1[j] == star[0]) &&(input_point2[i] == rule_point2[j] ) ){
                fprintf( fout, "%c",rule_port[j] );
                isfind_port = true;
                break;
            }//else if
            else if( (input_point1[i] == rule_point1[j] ) && (rule_point2[j] == star[0]) ){
                fprintf( fout, "%c",rule_port[j] );
                isfind_port = true;
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

    /*
    printf("%d\n",rule_number);

    for (int i=0;i<rule_number;i++)
    {
        printf("%c ",rule_point1[i]);
        printf("%c ",rule_point2[i]);
        printf("%c\n",rule_port[i]);
    }//for



    printf("%d\n",point_number);
    for (int i=0;i<point_number;i++)
    {
        printf("%c ",input_point1[i]);
        printf("%c\n",input_point2[i]);

    }//for
    */




}
