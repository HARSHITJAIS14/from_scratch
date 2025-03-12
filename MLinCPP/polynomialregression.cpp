// imports
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <chrono>
using namespace __gnu_pbds;
using namespace std;
template<typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
// ordered set has these 2 features 
// order_of_key (k) : Number of items strictly smaller than k .
// find_by_order(k) : K-th element in a set (counting from zero).

// define
#define ll long long
#define reps(i, start, end) for (ll i = start; i < end; i++)
#define repsb(i, end, start) for (ll i = end; i >= start; i--)
#define vi vector<int>
#define vd vector<double>
#define vl vector<ll>
#define vvl vector<vector<ll>>
#define vvd vector<vector<double>>
#define vpl vector<pair<ll, ll>>
#define mnpq priority_queue<ll, vector<ll>, greater<ll>>
#define mxpq priority_queue<ll>
#define all(v) v.begin(), v.end()
#define read(a, n) reps(i, 0, n) cin >> a[i]
#define give(a, n) reps(i, 0, n) cout << a[i] << " "
#define nl cout << endl;

//....................................................

//constants
const ll MOD = 1e9 + 7;
const ll mod=998244353;
vl fact(2e5+1,1);

// functions
void yesno(bool done) {
cout << (done ? "YES" : "NO") << endl;
}

void give2d(vvd &a){
    for(auto it:a){
        for (auto it2: it) cout<<it2<<" ";
        nl;
    }
}

ll powm(ll n,ll p,ll modu){
    n%=modu; ll result=1;
    while(p>0){
        if(p%2==1) result=(result*n)%modu;
        n=(n*n)%modu;
        p/=2;
    }

    return result;
}

ll ncr_(ll n,ll r){
    return ((fact[n]*powm(fact[r],MOD-2,MOD))%MOD)*powm((n-r>=0?fact[n-r]:0),MOD-2,MOD)%MOD;
}
bool fun(pair<char,ll> a,pair<char,ll> b){
    if(a.first!=b.first) return a.first>b.first;
    else return a.second<b.second;
}

// solve
// scratchpad
double mean(vd &a){
    double sum=0;
    for(auto it:a) sum+=it;
    return sum/a.size();
}

vd predict(vvd &polx, vd weights){
    vd preds(polx.size());
    reps(i,0,preds.size()){
        double weightedsum=0;
        reps(j,0,weights.size()){
            weightedsum+=weights[j]*polx[i][j];
        }
        preds[i]=weightedsum;
    }
    return preds;
}

double MSE(vd &y, vd &preds){
    double sum=0;
    reps(i,0,y.size()){
        sum+=pow((y[i]-preds[i]),2);
    }
    return sum/y.size();
}

double RMAE(vd &y, vd &preds){
    double sum=0;
    reps(i,0,y.size()){
        sum+=abs(y[i]-preds[i])/y[i];
    }
    return sum/y.size();
}

vd sub_arr(vd &a, vd &b){
    vd diff(a.size());
    reps(i,0,a.size()){
        diff[i]=a[i]-b[i];
    }
    return diff;
}

void takeinput(vvd &a){
    // taking the dataset as input...
    for(auto &it:a){
        for(auto &it2:it) cin>>it2;
    }
}

void gen_polyterms(ll degree, ll start, ll end, vd &curr, vvd &result){
    /*creating polynomial terms for a given degree for any number of variables
    example x1,x2 and degree 2  --- x1x1, x2x2, x1x2
    */ 
    if(degree==0){
        result.push_back(curr);
        return;
    }
    for(int i=start;i<=end;i++){
        curr.push_back(i);
        gen_polyterms(degree-1,i,end,curr,result);
        curr.pop_back();
    }
}

pair<vvd, vd> creatematrix(vvd &x, vd &y, ll degree){
    vvd polterms;
    
    for(int i=1;i<=degree;i++){
        vd curr;
        gen_polyterms(i,0,x[0].size()-1,curr,polterms);
    }
    // give2d(polterms);
    ll n=x.size();
    ll m=polterms.size()+1;
    vvd matrix(n, vd(m,1));
    vd weights(m);
    reps(i,0,n){
        reps(j,1,m){
            for(auto it:polterms[j-1]){
                matrix[i][j]*=x[i][it];
            }
        }
    }
    return {matrix,weights};
}


void solve(){
    ll n,m;
    cin>>n>>m;
    vvd a(n, vd (m));
    vvd x(n, vd(m-1)); vd y;
    takeinput(a);
    // dividing the data into x and y;
    reps(i,0,n){
        reps(j,0,m){
            if(j==m-1){
                y.push_back(a[i][j]);
            }
            else x[i][j]=a[i][j];
        }
    }
    
    // creating the matrix of of polynomial terms and their weights ;
    pair<vvd, vd> result = creatematrix(x, y, 4);
    vvd polx = result.first;
    vd weights = result.second;
    // give2d(polx); nl;
    // training
    ll num_epochs=51;
    double learningrate=0.00002;
    for(int epoch=0;epoch<num_epochs;epoch++){
        vd preds=predict(polx,weights);
        if(epoch%10==0){
            // give(preds,preds.size()); nl;
            double mse=MSE(y,preds);
            cout<<epoch<<"  "<<mse<<endl;
        }
        vd diffs=sub_arr(preds,y);

        reps(j,0,weights.size()){
            double derivative=0;
            reps(k,0,preds.size()){
                derivative+=diffs[k]*polx[k][j];
            }
            derivative*=2;
            weights[j]=weights[j]-derivative*learningrate;
            // preds=predict(polx,weights);
            
        }
    
    }
    // prediction
    vd preds=predict(polx,weights);
    double mse=MSE(y,preds);
    double rmae=RMAE(y,preds);
    cout<<"final error: "<<mse<<endl;
    cout<<"final relative error: "<<rmae<<endl;

}




int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll t=1;
    // cin >> t;
    reps(i,1,2e5+1){
        fact[i]=(fact[i-1]*i)%MOD;
    }

    auto start = chrono::high_resolution_clock::now();
    
    while (t--) {
        solve();

    }

    #ifndef ONLINE_JUDGE
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        cout << "Time taken: " << duration.count() << " seconds" << endl;
    #endif

    return 0;   
}

// ****first make logic*****
// v = {1, 2, 4, 4, 5, 6};
// lower bound first position jha usse bda ya barabar number aaye;lb(4)=2;
// upper bound is first position jha usse bda number aa jaye;ub(4)=4;
// c makes the difference ....
// avoid using math function hack ho jata h
