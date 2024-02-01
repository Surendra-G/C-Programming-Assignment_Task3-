// Assignment Task 3
// Name : Surendra Giri
// Student Id : 2357810

#include <stdio.h>  //this library is used for the input & output operations 
#include <pthread.h> //this library allows used for the multithreading 
#include <stdlib.h> //this library is used for the dynamic  memory allocation 
#include <math.h>  //this library is used to do different mathmatical operations like pow,sqrt,cbrt etc
#include <unistd.h> //this library is used for the sleep functions

//typedef function is used for creating user-defined datatype
// here "helperVar" is user-defined datatype for the stucture that contains number pointer, start,end and primes
typedef struct {
	int *number;
    int start;
    int end;
    int primes;
} helperVar;

//crating a function for checking the Prime number
int check_primeNum(int num) {
    int i;
    if (num <= 1) {
        return 0;
    } else {
        for (i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                return 0;
            }
        }
        return 1;
    }
}



//creating a void function for counting the prime numbers present in the files
void *count_primeNum(void *arg){
	helperVar *values = (helperVar *)arg;
	int i=0;
	for(i=values->start;i<= values->end; i++){
		if(check_primeNum(values->number[i])) {
			values->primes++;
		}
	}
	pthread_exit(NULL);
}

//creating a function which helps us to save the prime numbers in to new files 
void savePrimeNumbersIntoNewFile(char*filename, int *primes, int primeNumInEachThread){
	FILE *file = fopen(filename,"w");
	if(file == NULL){
		printf("Error in Opening the file.\n",filename);
		return;
	}else{
		printf("File has opened Succesfully.\n",filename);
	}	
	
	int i=0;
	
	for(i=0;i<primeNumInEachThread;i++){
		fprintf(file,"%d\n",primes[i]);
	}
	fclose(file);
}

//creating the Main Function
int main(){
	int total_num = 0;
	int *numbers;
	int total_files = 3;
	//creating the arrayt of all three given prime numberts files
	char *filename[] = {"PrimeData1","PrimeData2","PrimeData3"};

	
	//According to the give files, i only contain only 1 number in each line
	int first_file_num = 0;
	int second_file_num = 0;
	int third_file_num = 0;
	
	int i=0;
	for (i=0;i<total_files;i++){
		FILE *file = fopen(filename[i],"r");
		if(file == NULL){
			printf("Error in Opening the file.\n",filename[i]);
			exit(1);
		}else{
			printf("File has opened Succesfully.\n",filename[i]);
		}
		int num;
		//counting the total number present in all three files
		while (fscanf(file, "%d", &num) != EOF){
			total_num = total_num + 1;
		}
		fclose(file);
	}
	
	//dynamically allocating memory
	numbers = (int *) malloc(total_num*sizeof(int));
	if (numbers == NULL){
		printf("Failed In Allocating the MEmory");
		exit(1);
	}
	
	//now counting the prime numbers from each Files
	int index = 0;
	
	for (i=0;i<total_files;i++){
		FILE *file = fopen(filename[i],"r");
		if(file == NULL){
			printf("Error in Opening the file.\n",filename[i]);
			exit(1);
		}else{
			printf("File has opened Succesfully.\n",filename[i]);
		}
		int num;
		//counting the total prime numbers present in all three files
		while (fscanf(file, "%d", &num) != EOF){
			numbers[index++] = num;
		}
		fclose(file);
	}
	
	//now using the thread concepts, didviding the tasks among the threads
	int number_of_threads;
	
	//taking the user input for the threads 
	printf("Enter the number of threads: ");
	scanf("%d",&number_of_threads);
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
		thread_data[i].number = numbers;
		thread_data[i].start = start;
		thread_data[i].end = end;
		thread_data[i].primes = 0;
		start = end + 1;
	}
	
	//creating a threads
	for (i=0;i<number_of_threads;i++){
		
		pthread_create(&threads[i],NULL,count_primeNum, (void *)&thread_data[i]);
	}
	
	//using join method of threads
	for (i=0;i<number_of_threads;i++){
		pthread_join(threads[i],NULL);
	}
	
	//displaying the prime numbers that are found in loading each threads
	printf("The Prime Numbers found in each threads are as follows: \n");
	for(i=0;i<number_of_threads;i++){
		printf("Thread Number %d has %d Prime NUmbers.\n",i+1,thread_data[i].primes);
	}
	
	//saving the prime numbers into the new file
	for (i=0;i<number_of_threads;i++){
		char newFileOfPrimeNum[10];
		sprintf(newFileOfPrimeNum,"ActualPrimeNumbers.txt\n",i);
		savePrimeNumbersIntoNewFile(newFileOfPrimeNum,numbers+thread_data[i].start,thread_data[i].primes);  //calling the function to save the prime numbers in the new file
	}
	
	//allocating memory should be free 
	free(numbers);
	return 0;
}

