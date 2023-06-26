#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <time.h>
#include "redblack.h"
using namespace std;
using namespace std::chrono;



int main(int argc, char **argv){   
    RBT rbt;
    fstream fin,fout;
    string line;
    fin.open(argv[1],ios::in);
    fout.open("output.txt", ios::out);

    if(fin){
        getline(fin,line);
        
        string word;
        stringstream stre(line);
        getline(stre,word,' ');
        int process_num=stoi(word); //number of processes
        getline(stre,word,' ');
        int total_time=stoi(word); // total time which our system will be open

        // From input file I'm storing processes and their attributes in
        // an array so that I can handle them easier in the loop.
        Node* process_array = new Node[process_num];
        Node* finished_processes=new Node[process_num];
        int finished_num=0; // number of finished processes
        int minvruntime=0;

        for(int i=0;i<process_num;i++){
            getline(fin,line);
            
            string word;
            stringstream stre(line);
            getline(stre,word,' ');
            process_array[i].name=word;
            getline(stre,word,' ');
            process_array[i].arrival_time=stoi(word);
            getline(stre,word,' ');
            process_array[i].burst_time=stoi(word);
            process_array[i].key=0;
            process_array[i].status="Incomplete";
        }
        //i=0 onemsiz oldugu icin onun ciktisini direkt olarak loop disinda 
        // buraya yaziyorum.
        fout << "0,-,-,-,-,-,-" << endl;

        auto start = high_resolution_clock::now();
        //Her loop basinda array'a bakiyorum eklenme zamani gelen
        //process var mi diye.
        for(int i=1;i<total_time && finished_num!=process_num;i++){
            fout << i << ",";
            for(int j=0;j<process_num;j++){
                //Zamani gelen process'leri rbt'ye insert eyle.
                if(process_array[j].arrival_time==i){
                    rbt.insert(process_array+j);
                }
            }
            Node* pro=rbt.minimum(rbt.root);
            minvruntime=pro->key;
            fout << pro->name << "," << pro->key << "," << minvruntime << ",";
            rbt.inorder_tree_walk(fout,rbt.root);

                

            pro->key+=1;
            // Elimizdeki processor hala minvruntime'a sahip mi diye
            //successor ile karsilastiriyoruz ki o da parent'i oluyor.
            //Successor ile karsilastiriyoruz cunku elimizdeki processor'dan
            //sonra en az vruntime'a sahip o.
            //Eger successor'ın vruntime'ı pro'nun vruntime'dan daha az
            //ise pro'nun successor'unun vruntime'ı minvruntime'a esittir.

            if(pro->key <= pro->p->key)
                minvruntime=pro->key;
            else
                minvruntime= pro->p->key;
            
            
            if(pro->key==pro->burst_time){
                pro->status="Complete";
                finished_processes[finished_num]=*pro;
                finished_num++;
                rbt.delete_(pro);
            }
            
            fout << pro->status << endl;

            if(pro->key > minvruntime && pro->status!="Complete"){
                rbt.delete_(pro);
                rbt.insert(pro);
            }
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

        fout << endl << "Scheduling finished in " << duration.count()/1000.0 << " ms." << endl 
        << finished_num << " of " << process_num << " processes are completed." << endl 
        << "The order of completion of the tasks: ";
        for(int i=0;i<finished_num;i++){
            fout << finished_processes[i].name << "-";
        }
    
    }

    else
        cout << "File could not be opened." << endl;

    return 0;
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