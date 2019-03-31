#include <bits/stdc++.h> 
#include <algorithm>
#include <omp.h>
#include <time.h>

using namespace std;
#pragma omp declare reduction(vec_double_plus : std::vector<double> : \
                              std::transform(omp_out.begin(), omp_out.end(), omp_in.begin(), omp_out.begin(), std::plus<double>())) \
                    initializer(omp_priv = omp_orig)
int main()
{
    vector<double> ans(5);
    for(int i =0; i<5; i++){
        ans[i] = 0;
    }

    omp_set_num_threads(4);
    vector<double> myVector = {0, 2, 4, 6, 8}; 
    vector<double> myVector2 = {0, 3, 5, 7, 9}; 
    

    double pstart = omp_get_wtime();
    #pragma omp parallel for reduction(vec_double_plus:ans)
    for(int i =0; i<5; i++){
        ans[i] += myVector[i];
        ans[i] += myVector2[i];
        ans[i] += myVector2[i];
        ans[i] += myVector2[i];


    }
    cout<<"parallel = "<<omp_get_wtime() - pstart;
    
    cout<<"parallel ans : "<<endl;
    for(int i =0; i<5; i++){
        cout<<ans[i]<<" ";
    }
    cout<<endl<<endl;

    for(int i =0; i<5; i++){
        ans[i] = 0;
    }

    double sstart = omp_get_wtime();
    for(int i=0; i<5; i++){
        ans[i] += myVector[i];
        ans[i] += myVector2[i];
        ans[i] += myVector2[i];
        ans[i] += myVector2[i];

    }
    cout<<"serial = "<<omp_get_wtime() - sstart;
    
    cout<<"serial ans : "<<endl;
    for(int i =0; i<5; i++){
        cout<<ans[i]<<" ";
    }
    cout<<endl<<endl;
    return 0;
}