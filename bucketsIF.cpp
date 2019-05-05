#include<iostream>
#include <stdio.h>
#include <omp.h>
#include <random>
#include <ctime>
#include <vector>
#include <algorithm>
#include <ctime>
#include <unistd.h>

using namespace std;

double stopTimer(double timer){    return omp_get_wtime() - timer;}

int* generateData(int numberOfElements,int rangeOfNumbers){
    int* elements = new int[numberOfElements];
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int>  distr(0, rangeOfNumbers);
    #pragma omp parallel for
    for(int i=0;i<numberOfElements;i++){
        elements[i] = distr(generator);
    }
    return elements;
}

void print_vector(std::vector<int> const &input){
    for (auto const& i: input) {
        std::cout << i << " ";
    }
}

int main(int argc, const char * argv[]) {
	omp_lock_t writelock;
    omp_init_lock(&writelock);
    
    if (argc != 5) {
		cout<< "Provided: " << argc << " arguments"<<endl;
        cout<< "Please provide 4 arguments [numberOfPoints numberOfBuckets rangeOfNumbers numberOfThreads]" << endl;
        return -1;
    }
    
    int numberOfPoints = atoi(argv[1]);
    int numberOfBuckets = atoi(argv[2]);
    int rangeOfNumbers = atoi(argv[3]);
    int numberOfThreads = atoi(argv[4]);
    
    omp_set_num_threads(numberOfThreads);
    
    int bucketRange = rangeOfNumbers/numberOfBuckets;
    int threadRange = numberOfPoints/numberOfThreads;

    omp_lock_t writeLock;
    omp_init_lock(&writeLock);
    
    
//    cout<<"|--------------GENERATE DATA---------------|"<<endl;
    
    double timeGenerateDataStart = omp_get_wtime();
    int* elements = generateData(numberOfPoints, rangeOfNumbers);
    double timeGenerateDataStop = stopTimer(timeGenerateDataStart);
    
    vector<int>* buckets[numberOfBuckets];
    for(int i=0; i<numberOfBuckets; i++) {
        buckets[i] = new vector<int>();
    }
    
//    cout<<"|--------DIVIDE DATA INTO BUCKETS----------|"<<endl;

    double timeExecStart = omp_get_wtime();
    
    #pragma omp parallel for schedule(static,threadRange)
    for(int i=0;i<numberOfPoints;i++) 
    {   
        if( elements[i] == rangeOfNumbers){
            omp_set_lock(&writeLock);
            buckets[numberOfBuckets-1]->push_back(elements[i]);  
            omp_unset_lock(&writeLock); 
        }
        else{
            int idOfBucket = (elements[i]/bucketRange);

            if(idOfBucket>numberOfBuckets){
                omp_set_lock(&writeLock);
                buckets[numberOfBuckets-1]->push_back(elements[i]);  
                omp_unset_lock(&writeLock);  
            }
            else{
                omp_set_lock(&writeLock);
                buckets[idOfBucket]->push_back(elements[i]);  
                omp_unset_lock(&writeLock);
            }
        }
    }    
  
    double timeExecStop = stopTimer(timeExecStart);
//    cout<<"|------------SORTED BUCKETS----------------|"<<endl; 

    double timeSortBucketStart = omp_get_wtime();
    
    #pragma omp parallel
    {
        const int id = omp_get_thread_num();
        sort(buckets[id]->begin(), buckets[id]->end()); 
    }
    double timeSortBucketStop = stopTimer(timeSortBucketStart);
//    cout<<"|-------MERGE BUCKETS INTO sortedData------|"<<endl; 
    vector<int> sortedData;
    sortedData.reserve(numberOfPoints);
    for(int t=0;t<numberOfBuckets;t++)
        copy(buckets[t]->begin(), buckets[t]->end(),back_inserter(sortedData));

    cout<<numberOfPoints<<"     "<<numberOfBuckets<<"    "<<rangeOfNumbers<<"    "<<numberOfThreads<<"    "<<timeExecStop<<"    "
        <<timeGenerateDataStop<<"    "<<timeSortBucketStop<<endl;
    delete[] elements;
    omp_destroy_lock(&writeLock);
}