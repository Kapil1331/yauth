#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 - read matrix from file --- function
 - malloc result matrix
 - pass on origninal rows to threads to multiple and store result
 - print matrix --- function

 */

#define FILE1 "matrix1.txt"
#define FILE2 "matrix2.txt"

struct matrix_data {
    int rows;
    int cols;
    int **matrix;
};

struct Thread_data {
    struct matrix_data A;
    struct matrix_data B;
    int **C;
    int rowC;
};

void free_matrix(struct matrix_data M){
    for(int i = 0; i < M.rows; i++){
	free(M.matrix[i]);
    }

    free(M.matrix);

    return;
}

int **read_matrix(const char *filename, int *rows, int *cols){
    FILE *file = fopen(filename, "r");

    if(!file){
	perror("Couln;t open file.");
	exit(EXIT_FAILURE);
    }

    fscanf(file, "%d %d", rows, cols);

    int ** matrix = (int **)malloc(*rows * sizeof(int *));

    for(int i = 0; i < *rows; i++){
	matrix[i] = (int *)malloc(*cols * sizeof(int));

	for(int j = 0; j < *cols; j++){
	    fscanf(file, "%d", &matrix[i][j]);
	}
    }

    fclose(file);
    return matrix;
}


void print_matrix(struct matrix_data M){

    printf("--- Matrix ---\n");
    for(int i = 0; i < M.rows; i++){
	for(int j = 0; j < M.cols; j++){
	    printf(" %d\t", M.matrix[i][j]);
	}
	printf("\n");
    }

    return;
}

void *multiply_row(void *arg){

    struct Thread_data* data = (struct Thread_data *)arg;
    for(int i = 0; i < data->B.cols; i++){
	data->C[data->rowC][i] = 0;
	for(int j = 0; j < data->A.cols; j++){
	    data->C[data->rowC][i] += data->A.matrix[data->rowC][j] * data->B.matrix[j][i];
	}
	
    }

    //checking parallelization
    printf("Fin Pid[getpid()] : %d | Tid[gettiid()] : %d | PosixTid[pthread_self] : %lu | row : %d\n", getpid(), gettid(), pthread_self(), data->rowC);

    return NULL;
}

int **matrix_multiply(struct matrix_data A, struct matrix_data B){

    if(A.cols != B.rows){
	perror("Matrix dimensions not fit for dot product");
	exit(EXIT_FAILURE);
    }

    // initializing Product
    int **C = (int **)malloc(A.rows * sizeof(int *));

    // N threads for N rows (of resultant matrix)
    pthread_t *threads = (pthread_t *)malloc(A.rows * sizeof(pthread_t));
    struct Thread_data *thread_data = (struct Thread_data *)malloc(A.rows * sizeof(struct Thread_data));
    

    //looping
    for(int i = 0; i < A.rows; i++){

	C[i] = (int *)malloc(B.cols * sizeof(int));
	
        thread_data[i].A = A;
        thread_data[i].B = B;
        thread_data[i].C = C;
        thread_data[i].rowC = i;
        pthread_create(&threads[i], NULL, multiply_row, &thread_data[i]);

	
    }
    

    for(int i = 0; i < A.rows; i++){
	pthread_join(threads[i], NULL);
    }

    free(threads);
    free(thread_data);
    



    return C;

    
}




int main(){

    int rowsA, rowsB, colsA, colsB;

    int **A = read_matrix(FILE1, &rowsA, &colsA);
    int **B = read_matrix(FILE2, &rowsB, &colsB);
	printf("%d, %d\n",getpid(), gettid());
    struct matrix_data A1;
    A1.matrix = A;
    A1.rows = rowsA;
    A1.cols = colsA;

    struct matrix_data B1;
    B1.matrix = B;
    B1.rows = rowsB;
    B1.cols = colsB;

    print_matrix(A1);
    print_matrix(B1);

    
    int **C = matrix_multiply(A1, B1);
    
    
    struct matrix_data C1;
    C1.matrix = C;
    C1.rows = A1.rows;
    C1.cols = B1.cols;
    
    print_matrix(C1);
    
    free_matrix(A1);
    free_matrix(B1);
    free_matrix(C1);

    
    return 0;
}
