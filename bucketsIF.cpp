//g++ -fopenmp -Wall bucketsIF.cpp -o bif
// ./bif numberOfPoints numberOfBuckets rangeOfNumbers numberOfThreads textScalable chunkSize

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
    
    if (argc != 7) {
		cout<< "Provided: " << argc << " arguments"<<endl;
        cout<< "Please provide 7 arguments [numberOfPoints numberOfBuckets rangeOfNumbers numberOfThreads textScalable chunkSize]" << endl;
        return -1;
    }
    
    int numberOfPoints = atoi(argv[1]);
    int numberOfBuckets = atoi(argv[2]);
    int rangeOfNumbers = atoi(argv[3]);
    int numberOfThreads = atoi(argv[4]);
    int points = numberOfPoints;
    
    if(string(argv[5])=="scalable") {
        points = numberOfPoints * numberOfThreads;
    }
    
    int* elements = generateData(points, rangeOfNumbers);
    
    vector<int>* buckets[numberOfBuckets];
    for(int i=0; i<numberOfBuckets; i++) {
        buckets[i] = new vector<int>();
    }


    omp_lock_t writeLock[numberOfBuckets];
    for (int i = 0; i < numberOfBuckets; i++)
        omp_init_lock(&writeLock[i]);

    omp_set_num_threads(numberOfThreads);
    
    int bucketRange = rangeOfNumbers/numberOfBuckets;
    
    int chunkSize;
    
    if(string(argv[6])=="no")
        chunkSize = numberOfPoints/numberOfThreads;
    else{
        chunkSize = atoi(argv[6]);
    }

    int step = numberOfBuckets/numberOfThreads;

    double timeExecStart = omp_get_wtime();

    #pragma omp parallel shared(writeLock) 
    #pragma omp for schedule(static,chunkSize)
    for(int i=0;i<points;i++) 
    {   
        int idOfBucket;

        if( elements[i] == rangeOfNumbers || numberOfBuckets==1){
            idOfBucket = numberOfBuckets-1;
        }
        else{
            idOfBucket = (elements[i]/bucketRange);
            if(idOfBucket>=numberOfBuckets) idOfBucket = numberOfBuckets-1;  
        }
//        cout<<"TID: "<<omp_get_thread_num()<<" rezerwuje sobie dostep do kubelka: "<<idOfBucket<<" dla i = !"<<i<< endl;
//        omp_set_lock(&writeLock[idOfBucket]);
        buckets[idOfBucket]->push_back(elements[i]);
//        omp_unset_lock(&writeLock[idOfBucket]); 
    }
    
    
    #pragma omp parallel for schedule(static,step)
    for(int i=0;i<numberOfBuckets;i++)
        sort(buckets[i]->begin(), buckets[i]->end()); 
    
    double timeExecStop = stopTimer(timeExecStart);
    
    vector<int> sortedData;
    sortedData.reserve(numberOfPoints);
    
    for(int t=0;t<numberOfBuckets;t++)
        copy(buckets[t]->begin(), buckets[t]->end(),back_inserter(sortedData));

    cout<<numberOfPoints<<","<<numberOfBuckets<<","<<rangeOfNumbers<<","<<numberOfThreads<<","<<timeExecStop<<endl;
    
    delete[] elements;
    
    for (int i = 0; i < numberOfBuckets; i++)
        omp_destroy_lock(&writeLock[i]);
}