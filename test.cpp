/*
g++ test.cpp -o test -fopenmp -Wall
./test 100 4 100 4 a
*/
#include<iostream>
#include <stdio.h>
#include <omp.h>
//////////////////
#include <random> // for std::mt19937
#include <ctime> // for std::time
 ////////////////////////////
#include <vector>
/////////////////////////////
#include <algorithm>
/////////////////////////////


using namespace std;

double stopTimer(double timer){    return omp_get_wtime() - timer;}

int* generateData(int numberOfElements,int rangeOfNumbers){
    int* elements = new int[numberOfElements];
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int>  distr(0, rangeOfNumbers);
    #pragma omp parallel for 
    for(int i=0;i<numberOfElements;i++){
            
        #pragma omp critical
        {
            elements[i] = distr(generator);
        
            cout << "TID: "<<omp_get_thread_num()<<", i: " << i <<", generated: "<<elements[i]<<endl;  
        }
            
    }
    return elements;
}

void print_vector(std::vector<int> const &input)
{
    for (auto const& i: input) {
        std::cout << i << " ";
    }
}

int main(int argc, const char * argv[]) {
	
    if (argc != 6) {
		cout<< "Provided: " << argc << " arguments"<<endl;
        cout<< "Please provide 5 arguments [numberOfPoints numberOfBuckets rangeOfNumbers numberOfThreads isScalable]" << endl;
        return -1;
    }
    
    int numberOfPoints = atoi(argv[1]);
    int numberOfBuckets = atoi(argv[2]);
    int rangeOfNumbers = atoi(argv[3]);
    int numberOfThreads = atoi(argv[4]);
    
    omp_set_num_threads(numberOfThreads);
    
    cout<<"|----=---------GENERATE DATA---------------|"<<endl;
    
    double timeGenerateDataStart = omp_get_wtime();
    int* elements = generateData(numberOfPoints, rangeOfNumbers);
    double timeGenerateDataStop = stopTimer(timeGenerateDataStart);
    
    vector<int>* buckets[numberOfBuckets];
    for(int i=0; i<numberOfBuckets; i++) {
        buckets[i] = new vector<int>();
    }
    
    cout<<"|--------DIVIDE DATA INTO BUCKETS----------|"<<endl;

    
    double timeExecStart = omp_get_wtime();
    
    #pragma omp parallel shared(buckets)
    {    
        const int id = omp_get_thread_num();
        
        #pragma omp parallel for schedule(static)
        for(int i=id*(numberOfPoints/4);i<((id+1)*(numberOfPoints/4));i++) 
        {     
            if(elements[i]>(((rangeOfNumbers/4))*3)){//>75
                
                    #pragma omp critical
                    {
                        cout << "TID: "<<id<<", buckets[3]: "<<elements[i]<<endl;
                        buckets[3]->push_back(elements[i]); 
                    }
                }
            else if(elements[i]>(((rangeOfNumbers/4))*2)){//50
                
                #pragma omp critical
                {
                    cout << "TID: "<<id<<", buckets[2]: "<<elements[i]<<endl;
                    buckets[2]->push_back(elements[i]);
                }
            }
            else if(elements[i]>(((rangeOfNumbers/4))*1)){//25
                
                #pragma omp critical
                {
                    cout << "TID: "<<id<<", buckets[1]: "<<elements[i]<<endl;
                    buckets[1]->push_back(elements[i]);
                }
            }
            else{
                                    
                #pragma omp critical
                {
                    cout << "TID: "<<id<<", buckets[0]: "<<elements[i]<<endl;
                    buckets[0]->push_back(elements[i]);
                } 
            }
        }    
    }
        
    double timeExecStop = stopTimer(timeExecStart);
    cout<<"|------------SORTED BUCKETS----------------|"<<endl; 
    double timeSortBucketStart = omp_get_wtime();
    #pragma omp parallel
    {
        const int id = omp_get_thread_num();
        sort(buckets[id]->begin(), buckets[id]->end()); 
    }
    double timeSortBucketStop = stopTimer(timeSortBucketStart);
    
    for(int p=0;p<4;p++){
        cout<<"bucket["<<p<<"]:"<<endl;
        print_vector(*buckets[p]);
        cout<<endl;
    }
    cout<<"|-------MERGE BUCKETS INTO sortedData------|"<<endl; 
    
    vector<int> sortedData;
    sortedData.reserve(numberOfPoints);
    for(int t=0;t<numberOfThreads;t++)
        copy(buckets[t]->begin(), buckets[t]->end(),back_inserter(sortedData));
    print_vector(sortedData);
    cout<<endl;
    cout<<"|-------------STATS------------------------|"<<endl; 
    cout<<"buckets[0]->size():  "<<buckets[0]->size()<<endl;
    cout<<"buckets[1]->size():  "<<buckets[1]->size()<<endl;
    cout<<"buckets[2]->size():  "<<buckets[2]->size()<<endl;
    cout<<"buckets[3]->size():  "<<buckets[3]->size()<<endl;
    cout<<"|------------------------------------------|"<<endl;
    cout<<"sortedData.size():  "<<sortedData.size()<<endl;;
    cout<<"|------------------------------------------|"<<endl;
    cout<<"generateTime: "<<timeGenerateDataStop<<endl;
    cout<<"execTime: "<<timeExecStop<<endl;
    cout<<"sortTime: "<<timeSortBucketStop<<endl;
	cout<<"numberOfPoints: "<<numberOfPoints<<endl;
	cout<<"numberOfBuckets: "<<numberOfBuckets<<endl;
	cout<<"rangeOfNumbers: 0-"<<rangeOfNumbers<<endl;
	cout<<"numberOfThreads: "<<numberOfThreads<<endl;
	cout<<"other option: "<<argv[5]<<endl;
    cout<<"|------------------------------------------|"<<endl;
    delete[] elements;

    return 0;
}