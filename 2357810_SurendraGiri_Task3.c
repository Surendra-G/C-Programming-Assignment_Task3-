// Assignment Task 3
// Name : Surendra Giri
// Student Id : 2357810

#include <stdio.h>  //this library is used for the input & output operations 
#include <pthread.h> //this library allows used for the multithreading 
#include <stdlib.h> //this library is used for the dynamic  memory allocation 
#include <math.h>  //this library is used to do different mathmatical operations like pow,sqrt,cbrt etc
#include <unistd.h> //this library is used for the sleep functions
#include <stdbool.h> //this library is used t for boolean

//typedef function is used for creating user-defined datatype
// here "helperVar" is user-defined datatype for the stucture that contains number pointer, start,end and primes
typedef struct {
	int *numbers;
    int start;
    int end;
    int primes;
    int *prime_numbers; // Array to store prime numbers
} helperVar;

//crating a function for checking the Prime number
int check_primeNum(int num) {
    int i;
    if (num <= 1)
        return 0;
    for (i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

//creating a void function for counting the prime numbers present in the files
void *count_primeNum(void *arg){
	helperVar *values = (helperVar *)arg;
	int i=0;
	for(i=values->start;i<= values->end; i++){
		if(check_primeNum(values->numbers[i])) {
			values->primes++;
			//storing the prime numbers into the array
			values->prime_numbers[values->primes - 1] = values->numbers[i];
		}
	}
	pthread_exit(NULL);
}

//creating the Main Function
int main(){
	int total_num = 0;
	int *numbers;
	int total_files = 3;
	FILE *files[total_files];
	//creating the arrayof all three given prime numberts files
	char *filename[3] = {"PrimeData1.txt","PrimeData2.txt","PrimeData3.txt"};
	
	int i=0;
	for (i=0;i<total_files;i++){
		files[i] = fopen(filename[i],"r");
		if(files[i] == NULL){
			printf("File %d : Error in Opening the file.\n", i+1,filename[i]);
			exit(1);
		}else{
			printf("File %d : opened Succesfully.\n",i+1,filename[i]);
		}
		int num;
		//counting the total number present in all three files
		while (fscanf(files[i], "%d", &num) != EOF){
			total_num = total_num + 1;
		}
		rewind(files[i]);
	}
	
	//dynamically allocating memory and reading the numbers from the given files
	numbers = (int *) malloc(total_num*sizeof(int));
	int index = 0;
	
	for (i=0;i<total_files;i++){
		int num;
		//counting the total prime numbers present in all three files
		while (fscanf(files[i], "%d", &num) != EOF){
			numbers[index++] = num;
		}
		fclose(files[i]);
	}
	
	
	//now using the thread concepts, creating the threads and didviding the tasks among the threads
	int number_of_threads;
	
	//taking the user input for the threads 
	printf("Enter the number of threads: ");
	scanf("%d",&number_of_threads);
	
//	helperVar *threadData = (ThreadData *)malloc(numThreads * sizeof(ThreadData));
//    pthread_t *threads = (pthread_t *)malloc(numThreads * sizeof(pthread_t));
//	
	pthread_t threads[number_of_threads];
	helperVar thread_data[number_of_threads];
	
	int total_num_per_threads = total_num/number_of_threads;
	int remainder = total_num%number_of_threads;
	int start = 0;
	for (i=0;i<number_of_threads;i++){
		int end = start + total_num_per_threads - 1;
		if(i<remainder){
			end++;
		}
		thread_data[i].numbers = numbers;
		thread_data[i].start = start;
		thread_data[i].end = end;
		thread_data[i].primes = 0;
		thread_data[i].prime_numbers = (int *)malloc(total_num_per_threads * sizeof(int)); // Allocate memory for prime numbers array
		start = end + 1;
		pthread_create(&threads[i],NULL,count_primeNum, (void *)&thread_data[i]);
	}
	
	//using join method of threads which waits for the completion of the threads
	for (i=0;i<number_of_threads;i++){
		pthread_join(threads[i],NULL);
	}
	
	// now storing the prime numbers of the files into new files
    FILE *newPrimeNumbersFile = fopen("NewPrimeNumbersFile.txt", "w");
    int totalPrimes = 0;
    for (i = 0; i < number_of_threads; i++) {
        totalPrimes += thread_data[i].primes;
    }
    fprintf(newPrimeNumbersFile, "\nTotal prime numbers found: %d\n", totalPrimes);
    printf("Total prime numbers found: %d\n", totalPrimes);
    for (i = 0; i < number_of_threads; i++) {
        fprintf(newPrimeNumbersFile, "\nThread %d found %d prime numbers.\n", i + 1, thread_data[i].primes);
        printf("Thread %d found %d prime numbers.\n", i + 1, thread_data[i].primes);
        int j=0;
        for ( j = 0; j < thread_data[i].primes; j++) {
            fprintf(newPrimeNumbersFile, "%d ", thread_data[i].prime_numbers[j]);
        }
        fprintf(newPrimeNumbersFile, "\n");
    }
    
    printf("\n------------------------------------------------------------------------------------------------\n\n");
    printf("The Prime Numbers are saved into new file names as : NewPrimeNumbersFile.txt \n\n");
	
	
	//closing the files
	fclose(newPrimeNumbersFile);
	
	
	//allocating memory should be free 
	free(numbers);
	free(thread_data);
	free(threads);
	return 0;
}

