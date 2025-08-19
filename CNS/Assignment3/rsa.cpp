#include <bits/stdc++.h>
using namespace std;


int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


int modInverse(int e, int phi) {
    int t1 = 0, t2 = 1, r1 = phi, r2 = e;
    while (r2 != 0) {
        int q = r1 / r2;
        int r = r1 - q * r2; r1 = r2; r2 = r;
        int t = t1 - q * t2; t1 = t2; t2 = t;
    }
    if (r1 > 1) return -1; 
    if (t1 < 0) t1 += phi;
    return t1;
}


vector<int> sieve(int n) {
    vector<bool> prime(n+1, true);
    vector<int> primes;
    prime[0] = prime[1] = false;
    for (int i=2; i*i<=n; i++) {
        if (prime[i]) {
            for (int j=i*i; j<=n; j+=i)
                prime[j] = false;
        }
    }
    for (int i=2; i<=n; i++)
        if (prime[i]) primes.push_back(i);
    return primes;
}

int main() {
    vector<int> primes = sieve(100);

   
    ofstream file("rsa_table.csv");
    if (!file.is_open()) {
        cerr << "Error opening file!\n";
        return 1;
    }

    
    file << "p,q,distinct_primes,n,phi_n,e,d\n";

    for (int i = 0; i < (int)primes.size(); i++) {
        for (int j = i; j < (int)primes.size(); j++) { 
            int p = primes[i], q = primes[j];
            long long n = 1LL * p * q;

            
            long long phi;
            if (p == q) phi = 1LL * p * (p - 1);
            else phi = 1LL * (p - 1) * (q - 1);

            for (int e = 1; e <= 50; e++) {
                if (gcd(e, phi) != 1) continue;
                int d = modInverse(e, phi);
                if (d == -1) continue;
                if (d >= 1 && d <= 50) {
                    file << p << "," << q << "," 
                         << (p != q ? "true" : "false") << ","
                         << n << "," << phi << ","
                         << e << "," << d << "\n";
                }
            }
        }
    }

    file.close();
    cout << "CSV file 'rsa_table.csv' generated successfully!\n";
    return 0;
}
