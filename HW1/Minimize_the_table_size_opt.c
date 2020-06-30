#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE 1024
#define bool int
#define false 0
#define true 1


main()
{
// rule value
    int rule_number = 0;
    char rule_point1[ 1024 ]; //rule_point1
    char rule_point2[ 1024 ]; //rule_point2
    char rule_port[ 1024 ];   //rule_port

// input value
    int input_number = 0;
    char input_point1[ 1024 ];
    char input_point2[ 1024 ];


//determine rule
	char star[1] = "*";
    bool isfind_port = true;

//file input rule
    FILE *fin;
	fin=fopen("input.txt","r");
    fscanf(fin,"%d\n",&rule_number);

    for (int i=0;i<rule_number;i++)
    {
        fscanf(fin,"%c ",&rule_point1[i]);
        fscanf(fin,"%c ",&rule_point2[i]);
        fscanf(fin,"%c\n",&rule_port[i]);
    }//for

//file input input_point
    fscanf(fin,"%d\n",&input_number);
    for (int i=0;i<input_number;i++)
    {
        fscanf(fin,"%c ",&input_point1[i]);
        fscanf(fin,"%c\n",&input_point2[i]);

    }//for
    fclose(fin);

//output file
    FILE *fout;
    fout = fopen( "output.txt","w" );
    fprintf( fout,"%d\n",rule_number);
    for (int i=0;i<input_number;i++){
        fprintf( fout,"%c ",input_point1[i] );
        fprintf( fout, "%c ",input_point2[i] );

//determine
//****************************************************
//if Fully equal
//else if number1 = * , input_point_number2 is correspond to rule_point_number2
//else if number2 = * , input_point_number1 is correspond to rule_point_number1
//else if number1 = * ,number2 = *
//esle drop
//****************************************************
        for (int j=0;j<rule_number;j++){
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
            else if( (rule_point1[j] == star[0]) && (rule_point2[j] == star[0]) ){
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


//debug
    /*
    printf("%d\n",rule_number);

    for (int i=0;i<rule_number;i++)
    {
        printf("%c ",rule_point1[i]);
        printf("%c ",rule_point2[i]);
        printf("%c\n",rule_port[i]);
    }//for



    printf("%d\n",input_number);
    for (int i=0;i<input_number;i++)
    {
        printf("%c ",input_point1[i]);
        printf("%c\n",input_point2[i]);

    }//for
    */
}//main
