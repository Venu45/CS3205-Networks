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
   /* for(int i=1;i<argc;i++){
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
    double pr = 1-ps; */
    t = 3000;
    double k_i[2] = {1,4};
    double k_m[2] = {1,1.5};
    double k_n[2] = {0.5,1};
    double k_f[2] = {0.1,0.3};
    double p_s[2] = {0.01,0.0001};
    int per=0;
    for(int a1=0;a1<2;a1++){
        for(int a2=0;a2<2;a2++){
            for(int a3=0;a3<2;a3++){
                for(int a4=0;a4<2;a4++){
                    for(int a5=0;a5<2;a5++){
                        ki = k_i[a1];
                        km = k_m[a2];
                        kn = k_n[a3];
                        kf = k_f[a4];
                        ps = p_s[a5];
                        per++;
                        int counter = 0;
                        int phase = 0; // 0 for exponential growth phase and 1 for linear growth phase
                        int thre=0; // if thre =1 threshold has been set
                        double cw,ct;
                        ct = 100000000;
                        cw = ki; // initial cw
                        fstream fout;
                        string str = "venu_out";
                        str = str + to_string(per) + ".txt";
                        //cout<<str<<endl;
                        fout.open(str, ios::out);
                        fout<<"k_i = "<<ki<<" ,k_m = "<<km<<" ,k_n = "<<kn<<" ,k_f= "<<kf<<" ,p_s =  "<<ps<<" ,t = "<<t<<endl;
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
                                cw = min( (cw + (kn*1.0/cw) ) , 1000 );
                                fout<<cw<<endl;
                                }
                                else{
                                // below threshold
                                    cw = min(cw+km , 1000);
                                    fout<<cw<<endl;
                                }
                            }
                            else{
                                cw = min( (cw + (kn*1.0/cw) ) , 1000 );
                                fout<<cw<<endl;
                            }
                        }
                    }
                }
                fout.close();
                    }
                }
            }
        }
    }
    return 0;
}