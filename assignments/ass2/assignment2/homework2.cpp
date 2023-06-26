#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <cstdlib>
#include <cmath>
#include <chrono>
using namespace std;
using namespace std::chrono;


int partition(float* A,int l, int r);
int randomized_partition(float* A,int l, int r);
float randomized_select(float* A,int l, int r, int i);
float get_median(float* A,int len);
float calculate(float* A,int len,float total, string f);

int main(int argc, char **argv){
    srand(time(NULL));
    if(argc>1){
    cout << argv[1] << endl << endl << endl;
    //My data structure is an array.
    float A[1000000];
    int len=0;
    float total=0;
    //I'm watching the total from here to calculate mean easily.
    fstream fin,fout;
    string line;
    fin.open(argv[1],ios::in);
    fout.open("output.txt", ios::out);

    getline(fin,line);
    int func_len=stoi(line); //how many fucntion we gonna need?
    string *func_arr= new string[func_len];
    //function array

    for(int i=0;i<func_len;i++){
        getline(fin,line);
        func_arr[i]=line;
    }
    getline(fin,line);  //Gettin the empty line.
    getline(fin,line); //Now we get how many rows and the data type we gonna process.
    string word;
    stringstream stream(line);
    getline(stream,word,',');
    int row_num=stoi(word); // Number of row we gonna process.
    getline(stream,word,',');
    string data_type=word; // Now we got the data type we gonna process.
    int data_index; //Place of data type we need in data_arr 
    if(data_type=="gap")
        data_index=2;
    else if(data_type=="grp")
        data_index=3;
    else if(data_type=="v")
        data_index=4;
        else if(data_type=="gi")
        data_index=5;
    else
        cout << "ERROR COULDN'T GET THE DATA TYPE!!!" << endl;

    string data_arr[6];
    string starting[2];
    string received[2];


    cout << "data type: "<<data_type << endl;
    cout << "how man rows: "<<row_num << endl;
    cout << "how many functions: "<<func_len << endl;
    cout << "function names:";
    for(int i=0;i<func_len;i++){
        cout <<  " " << func_arr[i];
    }  
    cout << endl;
    
    
    auto start = high_resolution_clock::now();
    // I'm processing the remaining rows.
    for(int i=0;i<row_num;){
        getline(fin,line);
        i++;
        //Getting the data line and updating,adding the necessary things
        //to my array.
        if(line=="add"){
            getline(fin,line);
            i++;
            string word;
            stringstream stream(line);
            
            for(int j=0;j<6;j++){
                getline(stream,word,',');
                data_arr[j]=word;
            }
            float data=stof(data_arr[data_index]);
            A[len]=data;
            total+=data;
            len++;
            
            //For starting and received dates.
            if(len==1){
                starting[0]=data_arr[0];
                starting[1]=data_arr[1];
            }
            received[0]=data_arr[0];
            received[1]=data_arr[1];
        }
        else if(line=="print"){
            fout << starting[0] << "," << starting[1]
            << "," << received[0] << "," << received[1];
            for(int k=0;k<func_len;k++){
                //Temporary array to print. I need this because select 
                //algorithm changes the order of array randomly.
                float *B= new float[len+1];
                for(int t=0;t<len;t++){
                    B[t]=A[t];
                }
                fout << "," << calculate(B,len,total,func_arr[k]);
                delete[] B;
            }
            fout << endl;
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time that is spend to build output.txt: " << duration.count()/1000000.0 
    << " seconds" << endl;

    delete[] func_arr;
    }
    
    return 0;
}




















//This functions takes the function name as an argument and 
//calculate accordingly.
float calculate(float* A, int len, float total, string f){
    
    if(f=="mean"){
        return total/len;
    }
    
    else if(f=="std"){
        float tot=0;
        float mean=total/len;
        for(int i=0;i<len;i++){
            tot+=(A[i]-mean)*(A[i]-mean);
        }
        return sqrt(tot/(len-1));
    }
    
    else if(f=="min"){
        return randomized_select(A,0,len-1,1);
    }



    else if(f=="firstq"){


        float yer=(len-1.0)/4.0;
        int x=static_cast<int>(yer);
        int down=floor(yer);
        int up=ceil(yer);
        float firstq=randomized_select(A,0,len-1,down+1)
        + (yer-x)*(randomized_select(A,0,len-1,up+1) - randomized_select(A,0,len-1,down+1));
        return firstq;

    }




    else if(f=="median"){
        return get_median(A,len);
    }

    
    else if(f=="thirdq"){
        
        
        
        float yer=3*(len-1.0)/4.0;
        int x=static_cast<int>(yer);
        int down=floor(yer);
        int up=ceil(yer);
        float thirdq=randomized_select(A,0,len-1,down+1)
        + (yer-x)*(randomized_select(A,0,len-1,up+1) - randomized_select(A,0,len-1,down+1));
        return thirdq;

    }


    
    else if(f=="max"){
        return randomized_select(A,0,len-1,len);
    }
    
    
    else{
        cout << f << " is not a valid function." << endl;
        return -1;
    }
}






float get_median(float* A,int len){
    if(len%2==0){
        return ( randomized_select(A,0,len-1,len/2)
        + randomized_select(A,0,len-1,len/2 + 1) )/2;
    }
    else{
        return randomized_select(A,0,len-1,(len+1)/2);
    }
}





//I get these algorithms from our textbook.

int partition(float* A,int l, int r){


    float x=A[r];
    int i=l-1;
    int j;

    for(j=l;j<=r-1;j++){
        if(A[j]<=x){
            i++;

            float temp=A[i];
            A[i]=A[j];
            A[j]=temp;
        }
    }
    float temp=A[i+1];
    A[i+1]=A[r];
    A[r]=temp;
    return i+1;
}

int randomized_partition(float* A,int l, int r){
    int i=( rand()%(r-l+1) ) + l;
    float temp=A[i];
    A[i]=A[r];
    A[r]=temp;
    return partition(A,l,r);
}

float randomized_select(float* A,int l, int r, int i){
    if(l==r){
        return A[l];
    }
    int q=randomized_partition(A,l,r);
    int k=q-l+1;

    if(i==k){
        return A[q];
    }
    else if(i<k){
        return randomized_select(A,l,q-1,i);
    }
    else{
        return randomized_select(A,q+1,r,i-k);
    }
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