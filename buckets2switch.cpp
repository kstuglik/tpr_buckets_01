#include<iostream>
#include <stdio.h>
#include <omp.h>
#include <random>
#include <ctime>
#include <vector>
#include <algorithm>

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
    
    double timeGenerateDataStart = omp_get_wtime();
    int* elements = generateData(numberOfPoints, rangeOfNumbers);
    double timeGenerateDataStop = stopTimer(timeGenerateDataStart);
    
    vector<int>* buckets[numberOfBuckets];
    for(int i=0; i<numberOfBuckets; i++) {
        buckets[i] = new vector<int>();
    }
    
    int bucketRange = rangeOfNumbers/numberOfBuckets;
    int threadRange = numberOfPoints/numberOfThreads;
        
    double timeExecStart = omp_get_wtime();

    #pragma omp parallel for schedule(static,threadRange)
    for(int i=0;i<numberOfPoints;i++) 
    {   
        if( elements[i] == rangeOfNumbers){
            #pragma omp critical
            buckets[numberOfBuckets-1]->push_back(elements[i]);    
        }
        else{
            const int idOfBucket = (elements[i]/bucketRange);

            switch(idOfBucket){
                case 0:
                    #pragma omp critical
                    buckets[0]->push_back(elements[i]); break;
                case 1:
                    #pragma omp critical
                    buckets[1]->push_back(elements[i]); break;
                case 2:
                    #pragma omp critical
                    buckets[2]->push_back(elements[i]); break;
                case 3:
                    #pragma omp critical
                    buckets[3]->push_back(elements[i]); break;
                case 4:
                    #pragma omp critical
                    buckets[4]->push_back(elements[i]); break;
                case 5:
                    #pragma omp critical
                    buckets[5]->push_back(elements[i]); break;
                case 6:
                    #pragma omp critical
                    buckets[6]->push_back(elements[i]); break;
                case 7:
                    #pragma omp critical
                    buckets[7]->push_back(elements[i]); break;
                case 8:
                    #pragma omp critical
                    buckets[8]->push_back(elements[i]); break;
                case 9:
                    #pragma omp critical
                    buckets[9]->push_back(elements[i]); break;
                case 10:
                    #pragma omp critical
                    buckets[10]->push_back(elements[i]); break;
                case 11:
                    #pragma omp critical
                    buckets[11]->push_back(elements[i]); break;
                case 12:
                    #pragma omp critical
                    buckets[12]->push_back(elements[i]); break;
                case 13:
                    #pragma omp critical
                    buckets[13]->push_back(elements[i]); break;
                case 14:
                    #pragma omp critical
                    buckets[14]->push_back(elements[i]); break;
                case 15:
                    #pragma omp critical
                    buckets[15]->push_back(elements[i]); break;
                case 16:
                    #pragma omp critical
                    buckets[16]->push_back(elements[i]); break;
                case 17:
                    #pragma omp critical
                    buckets[17]->push_back(elements[i]); break;
                case 18:
                    #pragma omp critical
                    buckets[18]->push_back(elements[i]); break;
                case 19:
                    #pragma omp critical
                    buckets[19]->push_back(elements[i]); break;
                case 20:
                    #pragma omp critical
                    buckets[20]->push_back(elements[i]); break;
                case 21:
                    #pragma omp critical
                    buckets[21]->push_back(elements[i]); break;
                case 22:
                    #pragma omp critical
                    buckets[22]->push_back(elements[i]); break;
                case 23:
                    #pragma omp critical
                    buckets[23]->push_back(elements[i]); break;
                case 24:
                    #pragma omp critical
                    buckets[24]->push_back(elements[i]); break;
                case 25:
                    #pragma omp critical
                    buckets[25]->push_back(elements[i]); break;
                case 26:
                    #pragma omp critical
                    buckets[26]->push_back(elements[i]); break;
                case 27:
                    #pragma omp critical
                    buckets[27]->push_back(elements[i]); break;
                case 28:
                    #pragma omp critical
                    buckets[28]->push_back(elements[i]); break;
                case 29:
                    #pragma omp critical
                    buckets[29]->push_back(elements[i]); break;
                case 30:
                    #pragma omp critical
                    buckets[30]->push_back(elements[i]); break;
                case 31:
                    #pragma omp critical
                    buckets[31]->push_back(elements[i]); break;
                case 32:
                    #pragma omp critical
                    buckets[32]->push_back(elements[i]); break;
                case 33:
                    #pragma omp critical
                    buckets[33]->push_back(elements[i]); break;
                case 34:
                    #pragma omp critical
                    buckets[34]->push_back(elements[i]); break;
                case 35:
                    #pragma omp critical
                    buckets[35]->push_back(elements[i]); break;
                case 36:
                    #pragma omp critical
                    buckets[36]->push_back(elements[i]); break;
                case 37:
                    #pragma omp critical
                    buckets[37]->push_back(elements[i]); break;
                case 38:
                    #pragma omp critical
                    buckets[38]->push_back(elements[i]); break;
                case 39:
                    #pragma omp critical
                    buckets[39]->push_back(elements[i]); break;
                default:
                    #pragma omp critical
                    buckets[numberOfBuckets-1]->push_back(elements[i]); break;
            }
        }    
    }
    
    double timeExecStop = stopTimer(timeExecStart);

    double timeSortBucketStart = omp_get_wtime();
    
    #pragma omp parallel
    {
        const int id = omp_get_thread_num();
        sort(buckets[id]->begin(), buckets[id]->end()); 
    }
    
    double timeSortBucketStop = stopTimer(timeSortBucketStart);

    vector<int> sortedData;
    sortedData.reserve(numberOfPoints);
    for(int t=0;t<numberOfBuckets;t++)
        copy(buckets[t]->begin(), buckets[t]->end(),back_inserter(sortedData));

    cout<<numberOfPoints<<"     "<<numberOfBuckets<<"    "<<rangeOfNumbers<<"    "<<numberOfThreads<<"    "<<timeExecStop<<"    "
        <<timeGenerateDataStop<<"    "<<timeSortBucketStop<<endl;
    delete[] elements;

    return 0;
}