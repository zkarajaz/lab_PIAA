#include <iostream>
#include <vector>
#include <string>

#define DEBUG 1

using namespace std;

vector<int> getPref(const string& pattern) {
    int n = pattern.size();
    vector<int> prefs(n, 0);
    int len = 0;
    int i = 1;
    
    while (i < n) {
        if (DEBUG) {
            cout << "i=" << i << " len=" << len << " | Comparing pattern[" << i 
                 << "]=" << pattern[i] << " with pattern[" << len << "]=" << pattern[len] << endl;
        }
        
        if (pattern[i] == pattern[len]) {
            len++;
            prefs[i] = len;
            i++;
            if (DEBUG) {
                cout << "Match! prefs[" << i-1 << "]=" << len << endl;
            }
        } else {
            if (len != 0) {
                if (DEBUG) {
                    cout << "Mismatch! Updating len from " << len << " to prefs[" 
                         << len-1 << "]=" << prefs[len-1] << endl;
                }
                len = prefs[len - 1];
            } else {
                prefs[i] = 0;
                i++;
                if (DEBUG) {
                    cout << "Mismatch! prefs[" << i-1 << "]=0" << endl;
                }
            }
        }
    }
    
    if (DEBUG) {
        cout << "Final prefix array: [";
        for (int val : prefs) cout << val << " ";
        cout << "]" << endl << endl;
    }
    
    return prefs;
}

int kmpSearch(const string& pattern, const string& text) {    
    if (DEBUG) {
        cout << "Starting KMP search for pattern: " << pattern << " in text: " << text << endl;
    }
    
    vector<int> prefs = getPref(pattern);
    int i = 0;
    int j = 0;
    int n = text.size();
    int m = pattern.size();
    
    while (i < n) {
        if (DEBUG) {
            cout << "i=" << i << " j=" << j << " | Comparing text[" << i << "]=" << text[i] 
                 << " with pattern[" << j << "]=" << pattern[j] << endl;
        }
        
        if (pattern[j] == text[i]) {
            i++;
            j++;
            if (DEBUG) {
                cout << "Match! i=" << i << " j=" << j << endl;
            }
            
            if (j == m) {
                if (DEBUG) {
                    cout << "FULL MATCH FOUND at position " << i-j << endl << endl;
                }
                return i - j;
            }
        } else {
            if (j != 0) {
                if (DEBUG) {
                    cout << "Mismatch! Updating j from " << j << " to prefs[" << j-1 << "]=" << prefs[j-1] << endl;
                }
                j = prefs[j - 1];
            } else {
                if (DEBUG) {
                    cout << "Mismatch! Incrementing i from " << i << " to " << i+1 << endl;
                }
                i++;
            }
        }
    }
    
    if (DEBUG) {
        cout << "No match found" << endl << endl;
    }
    
    return -1;
}

int main() {
    string A, B;
    getline(cin, A);
    getline(cin, B);
    
    if (DEBUG) {
        cout << "Input strings:" << endl;
        cout << "A: " << A << endl;
        cout << "B: " << B << endl << endl;
    }
    
    if (A.size() != B.size()) {
        if (DEBUG) {
            cout << "Length mismatch: " << A.size() << " != " << B.size() << endl;
        }
        cout << -1;
        return 0;
    }
    
    string doubledA = A + A;
    if (DEBUG) {
        cout << "Doubled A: " << doubledA << endl;
    }
    
    int result = kmpSearch(B, doubledA);
    
    cout << result;
    
    return 0;
}