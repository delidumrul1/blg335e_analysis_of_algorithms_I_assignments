#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
using namespace std;
using namespace std::chrono;



int num_par=0;
int num_swap=0;
void quicksort(string* A, int l, int r);
int partition(string* A, int l, int r);
float getrating(string row);

void quicksort(string* A, int l, int r){
    if(l<r){
        int q=partition(A,l,r);
        quicksort(A, l, q-1);
        quicksort(A, q+1, r);
    }
}

int partition(string* A, int l, int r){
    num_par++;
    //Getting ratings from rows to compare them.
    float x = getrating(A[r]);
    int i=l-1;
    int j;

    for(j=l;j<=r-1;j++){
        if(getrating(A[j])<=x){
            i++;
            num_swap++;
            string temp=A[i];
            A[i]=A[j];
            A[j]=temp;
        }
    }
    num_swap++;
    string temp=A[i+1];
    A[i+1]=A[r];
    A[r]=temp;
    return i+1;
} 


float getrating(string row){
    string _;
    float rating;
    stringstream s(row);
    getline(s,_,',');getline(s,_,',');getline(s,_,',');
    getline(s,_,',');
    rating=stof(_);
    return rating;
} 

int main(void){
    fstream fin,fout;
    string line,titles;
    fin.open("books.csv",ios::in);
    fout.open("sorted_books.csv", ios::out);
    getline(fin,line);
    titles=line;
    int n=0;
    
    while(!fin.eof()){
        getline(fin,line);
        n++;
    }
    n--;
    //n is the size of the array which contains every row as an element.
    string rows[n];
    fin.clear();
    fin.seekg (0, ios::beg);
    
    //Array rows is being filled with data. 
    getline(fin,line);
    for(int i=0;i<n;i++){
        getline(fin,line);
        rows[i]=line;
    }

    auto start = high_resolution_clock::now();
    quicksort(rows,0,n-1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);


    //sorted array rows is printed.
    fout << titles << "\n";
    for(int i=0;i<n;i++){
        fout << rows[i] << "\n";
    }
    
    cout << "Number of swaps: " << num_swap << endl;
    cout << "Number of partitions:" << num_par << endl;
    cout << "Time elapsed for quicksort algorithm: " << duration.count() << " microseconds " << endl;
    cout << "Done!   " << endl;
}












/*
.: ⡴⠑⡄⠀⠀⠀⠀⠀⠀⠀⣀⣀⣤⣤⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠸⡇⠀⠿⡀⠀⠀⠀⣀⡴⢿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠑⢄⣠⠾⠁⣀⣄⡈⠙⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⢀⡀⠁⠀⠀⠈⠙⠛⠂⠈⣿⣿⣿⣿⣿⠿⡿⢿⣆⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⢀⡾⣁⣀⠀⠴⠂⠙⣗⡀⠀⢻⣿⣿⠭⢤⣴⣦⣤⣹⠀⠀⠀⢀⢴⣶⣆ 
⠀⠀⢀⣾⣿⣿⣿⣷⣮⣽⣾⣿⣥⣴⣿⣿⡿⢂⠔⢚⡿⢿⣿⣦⣴⣾⠁⠸⣼⡿ 
⠀⢀⡞⠁⠙⠻⠿⠟⠉⠀⠛⢹⣿⣿⣿⣿⣿⣌⢤⣼⣿⣾⣿⡟⠉⠀⠀⠀⠀⠀ 
⠀⣾⣷⣶⠇⠀⠀⣤⣄⣀⡀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠉⠈⠉⠀⠀⢦⡈⢻⣿⣿⣿⣶⣶⣶⣶⣤⣽⡹⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠉⠲⣽⡻⢿⣿⣿⣿⣿⣿⣿⣷⣜⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣷⣶⣮⣭⣽⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⣀⣀⣈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠿⠿⠿⠿⠛⠉
*/


