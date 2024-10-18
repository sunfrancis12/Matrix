#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;

// 讀檔用的函數
void read_matrix(const string& filename, vector<vector<double> >& matrix) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Unable to open file " + filename);
    }

    int size;
    if (!(file >> size)) {
        throw runtime_error("Unable to read matrix size");
    }
    cout << "Reading matrix of size: " << size << "x" << size << endl;
    matrix.resize(size, vector<double>(size));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (!(file >> matrix[i][j])) {
                throw runtime_error("Error reading matrix element");
            }
        }
    }
}

void matrix_multiply(const vector<vector<double> >& A,
                     const vector<vector<double> >& B,
                     vector<vector<double> >& C) {
    int size = A.size();
    int total = size * size;
    int count = 0;
    // 核心算法
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            double temp = A[i][k];
            for (int j = 0; j < size; j++) {
                C[i][j] += temp * B[k][j];
            }
            count++;
            cout << "\rMultiplication progress: " << count * 100 / total << "%" << flush;
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            throw runtime_error("Usage: " + string(argv[0]) + " <matrix size>");
        }

        int size = stoi(argv[1]);
        string filename = to_string(size) + ".txt";

        vector<vector<double> > A, B, C;
        read_matrix(filename, A);
        read_matrix(filename, B);

        C.resize(size, vector<double>(size, 0.0));  // 初始化 C 矩陣為 0

        cout << "------Start------" << endl;
        clock_t start = clock();
        matrix_multiply(A, B, C);
        clock_t end = clock();

        double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "\n Total Time: " << cpu_time_used << " seconds" << endl;

        // 輸出結果矩陣的一小部分來驗證計算
        cout << "\nResult matrix (top-left 5x5 corner):" << endl;
        for (int i = 0; i < min(5, size); ++i) {
            for (int j = 0; j < min(5, size); ++j) {
                cout << setw(12) << fixed << setprecision(6) << C[i][j] << " ";
            }
            cout << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}