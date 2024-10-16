// 最基本的 IJK 矩陣乘法
// Author: Ruki 白兔
// 使用方法: ./filename matrix_size(假設有個檔案是 100.txt，直接設定 100 就好)
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

// 讀檔用的函數
void read_matrix(const string& filename, vector<vector<int>>& matrix) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Unable to open file " + filename);
    }

    int size;
    if (!(file >> size)) {
        throw runtime_error("Unable to read matrix size");
    }
    cout << "Reading matrix of size: " << size << "x" << size << endl;
    matrix.resize(size, vector<int>(size));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (!(file >> matrix[i][j])) {
                throw runtime_error("Error reading matrix element");
            }
        }
    }
}

void matrix_multiply(const vector<vector<int>>& A,
                     const vector<vector<int>>& B,
                     vector<vector<int>>& C) {
    int size = A.size();
    int total = size * size;
    int count = 0;
    // 核心算法
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
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

        vector<vector<int>> A, B, C;
        read_matrix(filename, A);
        read_matrix(filename, B);

        C.resize(size, vector<int>(size));

        cout << "------Start------" << endl;
        clock_t start = clock();
        matrix_multiply(A, B, C);
        clock_t end = clock();

        double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "\n Total Time: " << cpu_time_used << " seconds" << endl;

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}