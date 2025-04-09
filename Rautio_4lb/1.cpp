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
            cout << "\nCompute prefs: i=" << i << " len=" << len;
            cout << "\nCompare pattern[" << i << "]='" << pattern[i] 
                 << "' with pattern[" << len << "]='" << pattern[len] << "'";
        }
        
        if (pattern[i] == pattern[len]) {
            len++;
            prefs[i] = len;
            i++;
            if (DEBUG) cout << "\nMatch! Set prefs[" << i-1 << "]=" << len;
        } else {
            if (len != 0) {
                if (DEBUG) cout << "\nMismatch! Update len from " << len 
                               << " to prefs[" << len-1 << "]=" << prefs[len-1];
                len = prefs[len - 1];
            } else {
                prefs[i] = 0;
                i++;
                if (DEBUG) cout << "\nMismatch! Set prefs[" << i-1 << "]=0";
            }
        }
    }
    
    if (DEBUG) {
        cout << "\nFinal prefs array: [";
        for (int val : prefs) cout << val << " ";
        cout << "]\n";
    }
    
    return prefs;
}

vector<int> kmpSearch(const string& pattern, const string& text) {
    vector<int> occurrences;
    
    if (DEBUG) {
        cout << "\n\nStarting KMP search";
        cout << "\nPattern: " << pattern;
        cout << "\nText: " << text << "\n";
    }
    
    vector<int> prefs = getPref(pattern);
    int i = 0;
    int j = 0;
    int n = text.size();
    int m = pattern.size();
    
    while (i < n) {
        if (DEBUG) {
            cout << "\n\nSearch step: i=" << i << " j=" << j;
            cout << "\nCompare text[" << i << "]='" << text[i] 
                 << "' with pattern[" << j << "]='" << pattern[j] << "'";
        }
        
        if (pattern[j] == text[i]) {
            i++;
            j++;
            if (DEBUG) cout << "\nMatch! Increment i to " << i << " and j to " << j;
            
            if (j == m) {
                int pos = i - j;
                occurrences.push_back(pos);
                if (DEBUG) cout << "\nFULL MATCH FOUND at position " << pos;
                j = prefs[j - 1];
                if (DEBUG) cout << "\nUpdate j to prefs[" << j << "]=" << j;
            }
        } else {
            if (j != 0) {
                if (DEBUG) cout << "\nMismatch! Update j from " << j 
                               << " to prefs[" << j-1 << "]=" << prefs[j-1];
                j = prefs[j - 1];
            } else {
                if (DEBUG) cout << "\nMismatch! Increment i from " << i << " to " << i+1;
                i++;
            }
        }
    }
    
    if (DEBUG) {
        cout << "\n\nSearch complete. Found " << occurrences.size() << " matches: [";
        for (int pos : occurrences) cout << pos << " ";
        cout << "]\n";
    }
    
    return occurrences;
}

int main() {
    string P, T;
    cin >> P >> T;
    
    if (DEBUG) {
        cout << "\nInput:\n";
        cout << "Pattern: " << P << "\n";
        cout << "Text: " << T << "\n";
    }
    
    vector<int> result = kmpSearch(P, T);
    
    if (result.empty()) {
        cout << -1;
    } else {
        for (int i = 0; i < result.size(); ++i) {
            if (i != 0) cout << ",";
            cout << result[i];
        }
    }
    
    return 0;
}