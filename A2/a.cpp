#include <bits\stdc++.h>
#include <iostream>
#include <string>
#include <random>
#include <fstream>
using namespace std;

double max(double a , double b){
    if(a>=b)return a;
    return b;
}

double min(double a,double b){
    if(a<=b)return a;
    return b;
}


int main(int argc , char * argv[]){
    double ki,km,kn,kf,ps,t;
    for(int i=1;i<argc;i++){
        cout<<argv[i]<<" ";
    }
    cout<<endl;
    ki = stod(argv[1]);
    km = stod(argv[2]);
    kn = stod(argv[3]);
    kf = stod(argv[4]);
    ps = stod(argv[5]);
    t = stod(argv[6]);
    cout<<ki<<" "<<km<<" "<<kn<<" "<<kf<<" "<<ps<<" "<<t<<endl;
    double pr = 1-ps;
    int counter = 0;
    int phase = 0; // 0 for exponential growth phase and 1 for linear growth phase
    int thre=0; // if thre =1 threshold has been set
    double cw,ct;
    ct = 100000000;
    cw = ki; // initial cw
    fstream fout;
    fout.open("outfile.txt", ios::out);
    fout<<cw<<endl;
    counter++;

    //The random number generator
	default_random_engine generator(69420);
	
	//Initialising the bernoulli distribution
	bernoulli_distribution distribution(ps);

    
    int n;
    while(counter < t){
        n = ceil(cw);
        for(int ii=0;ii<n&& counter<t;ii++){
            counter++;
            if (distribution(generator)){
                // timeout has occured
                thre = 1; // saying threashold has been set
                ct = (cw*1.0)/2;
                cw = max(1,kf*cw);
                fout<<cw<<endl; // writing new value to outfile
                phase = 0; // as we go to exponential phase
                break;
            }
            else{
                // no timeout
                if(phase==0){
                    // in exponential growth phase
                    if(thre==1 && cw>=ct){
                        // we reached threshold
                        phase = 1; // change phase to congestion aviodance
                        cw = min( (cw + (kn*1.0/cw) ) , 1000.0 );
                        fout<<cw<<endl;
                    }
                    else{
                        // below threshold
                        cw = min(cw+km , 1000.0);
                        fout<<cw<<endl;
                    }
                }
                else{
                    cw = min( (cw + (kn*1.0/cw) ) , 1000.0 );
                    fout<<cw<<endl;
                }
            }
        }
    }
    fout.close();
    return 0;
}