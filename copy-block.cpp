// 使用 copy-block 的方式進行矩陣乘法，將矩陣分成區塊進行計算，並將子矩陣複製到臨時區域進行計算。這樣可以提高 CPU 快取的效率，減少 cache miss 的次數，進而提高程式的效能。
// Author: Ruki 白兔
// 使用方法: ./filename matrix_size(假設有個檔案是 100.txt，直接設定 100 就好)
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

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

// 複製子矩陣到臨時區域
void copy_submatrix(const vector<vector<int>>& src, vector<vector<int>>& dest, int start_row, int start_col, int block_size) {
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            if (start_row + i < src.size() && start_col + j < src.size()) {
                dest[i][j] = src[start_row + i][start_col + j];
            }
        }
    }
}

// 執行子矩陣乘法
void multiply_submatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int block_size) {
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            for (int k = 0; k < block_size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// 將結果加回到 C 矩陣
void add_to_submatrix(vector<vector<int>>& C, const vector<vector<int>>& temp_C, int start_row, int start_col, int block_size) {
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            if (start_row + i < C.size() && start_col + j < C.size()) {
                C[start_row + i][start_col + j] += temp_C[i][j];
            }
        }
    }
}

// 矩陣乘法
void matrix_multiply(const vector<vector<int>>& A,
                     const vector<vector<int>>& B,
                     vector<vector<int>>& C, int BLOCK_SIZE) {
    int size = A.size();
    vector<vector<int>> temp_A(BLOCK_SIZE, vector<int>(BLOCK_SIZE));
    vector<vector<int>> temp_B(BLOCK_SIZE, vector<int>(BLOCK_SIZE));
    vector<vector<int>> temp_C(BLOCK_SIZE, vector<int>(BLOCK_SIZE));

    // 將矩陣分成區塊進行計算
    for (int i = 0; i < size; i += BLOCK_SIZE) {
        for (int j = 0; j < size; j += BLOCK_SIZE) {
            for (int k = 0; k < size; k += BLOCK_SIZE) {
                // 複製 A 和 B 的子矩陣到臨時區域
                copy_submatrix(A, temp_A, i, k, BLOCK_SIZE);
                copy_submatrix(B, temp_B, k, j, BLOCK_SIZE);

                // 在臨時區域執行子矩陣乘法
                multiply_submatrices(temp_A, temp_B, temp_C, BLOCK_SIZE);

                // 將結果累加回 C
                add_to_submatrix(C, temp_C, i, j, BLOCK_SIZE);
            }
        }
    }
    cout << "\r counting... " << flush;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            throw runtime_error("Usage: " + string(argv[0]) + " <matrix size>");
        }

        int size = stoi(argv[1]);
        int block_size = 32;
        string filename = to_string(size) + ".txt";

        vector<vector<int>> A, B, C(size, vector<int>(size, 0));
        read_matrix(filename, A);

        cout << "------Start------" << endl;
        clock_t start = clock();
        matrix_multiply(A, B, C, block_size);
        clock_t end = clock();

        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        cout << "\n Total Time: " << cpu_time_used << " seconds" << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
