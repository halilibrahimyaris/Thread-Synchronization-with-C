#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
//define thread mutex values
pthread_mutex_t lockSum;
pthread_mutex_t lockCreateMatrix;
//define a value for matrix sum
int total;
int count;
//define a global matrix varaible 
int arr[20][20] = {'\0'};

void ctrl_c_handler(int dummy)
{
    // this function handles CRTL C signal 
    signal(SIGINT, ctrl_c_handler);
    //define a signal for CRTL C input
    printf("\nCRTL_C girdisi tespit edildi\n");

    pthread_mutex_destroy(&lockSum);
    pthread_mutex_destroy(&lockCreateMatrix);
    //Destroy threads
    printf("\nToplam threadi sonlandırıldı\n");
    printf("\nMatrix olusturma threadi sonlandırıldı\n");
    //end finish the program
    exit(0);
}
void ctrl_z_handler(int dummy)
{

    // this function handles CRTL Z signal 
    signal(SIGTSTP, ctrl_z_handler);
    //define a signal for CRTL C input
    printf("\nCRTL_Z girdisi tespit edildi\n");
    pthread_mutex_destroy(&lockSum);
    pthread_mutex_destroy(&lockCreateMatrix);
    //Destroy threads
    printf("\nToplam threadi sonlandırıldı\n");
    printf("\nMatrix olusturma threadi sonlandırıldı\n");
    exit(0);
    //end finish the program
}
void *createMatrix()
{
    int i = 0;
    int j = 0;
    int c = 0;
    //define some integer values for using in loops
    for (c = 1; c <= count; c++)
    {
        //firsr for return among user input
        pthread_mutex_lock(&lockCreateMatrix);
        //and ı lock createMatrix thread 
        for (i = 0; i < 20; i++)
        {
            for (j = 0; j < 20; j++)
            {
                //these two foor loop for create 20x20 matrix
                int random = 1 + rand() % 99;
                //create random numbers
                arr[i][j] = random; //send these values to arr array
            }
        }
        printMatrix(arr);
        //print that array
        pthread_mutex_unlock(&lockSum);
        //and unlok sum thread
    }
}
void *sumMatrix()
{
    //
    int i = 0;
    int j = 0;
    int c = 0;
    //define some integer values for using in loops
    for (c = 1; c <= count; c++)
    {
        //firsr for return among user input
        total = 0;//there is a sum value
        pthread_mutex_lock(&lockSum);
        //lock sum thread because I want create matrix thread works after the  this thread 
        for (i = 0; i < 20; i++)
        {
            for (j = 0; j < 20; j++)
            {
                //return all values of matrix and sum these numbers
                total += arr[i][j];
            }
        }
        printf("toplam = %d", total);//print sum
        printf("\n-----------------------------------------\n");
        //and unlock createMatrix
        pthread_mutex_unlock(&lockCreateMatrix);
    }
}
void printMatrix(int arr[20][20])
{
    //this function wrote for printing our matrix
    printf("\n");
    int i = 0;
    int j = 0;
    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 20; j++)
        {
            printf("%d \t", arr[i][j]);
        }
        printf("\n");
    }
}
int isNumber(char a[])
{
    int i;
    int length = 0;
    length = strlen(a);
    for (i = 0; i < length; i++)
    {
        //Girilen parametrenin her bir karakteri için isdigit() kontrol edilir.
        if (!isdigit(a[i]))
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

int main()
{   
    //there are init functions for our threads
    pthread_mutex_init(&lockSum, NULL);
    pthread_mutex_init(&lockCreateMatrix, NULL);
    //and lock sum thread
    pthread_mutex_lock(&lockSum);


    pthread_t sumThread;
    pthread_t createThread;


    while (1)
    {   //There are my signal handler functions
        signal(SIGINT, ctrl_c_handler);
        signal(SIGTSTP, ctrl_z_handler);
        //ask user matrix count
        printf("\nLütfen kaç adet matrix oluşturmak istediginizi giriniz: ");
        //read that value
        scanf("%d", &count);

        if (count <= 0)
        {   //there are control for zero or negative values
            printf("\nNegatif ya da 0 degeri tespit edildi\n");
            //destroy these two thread
            pthread_mutex_destroy(&lockSum);
            printf("\nToplam threadi sonlandırıldı\n");
            pthread_mutex_destroy(&lockCreateMatrix);
            printf("\nMatrix olusturma threadi sonlandırıldı\n");
            //finish the program
            exit(0);
        }
        else
        {   //else create our threads
            pthread_create(&sumThread, NULL, sumMatrix, NULL);
            pthread_create(&createThread, NULL, createMatrix, NULL);
            //join threads
            pthread_join(sumThread, NULL);
            pthread_join(createThread, NULL);
        }
    }
    return 0;
}