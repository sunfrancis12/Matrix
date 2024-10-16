// 不利用複製的方式，將矩陣分成區塊進行計算
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
                     vector<vector<int>>& C, int BLOCK_SIZE) {
    int size = A.size();
    int total = size * size;
    int count = 0;

    // 將矩陣分成區塊進行計算
    for (int i = 0; i < size; i += BLOCK_SIZE) {
        for (int j = 0; j < size; j += BLOCK_SIZE) {
            for (int k = 0; k < size; k += BLOCK_SIZE) {

                // 對每個區塊進行計算
                for (int i_inner = i; i_inner < min(i + BLOCK_SIZE, size); i_inner++) {
                    for (int j_inner = j; j_inner < min(j + BLOCK_SIZE, size); j_inner++) {
                        // 初始化 C[i_inner][j_inner]
                        if (k == 0) {
                            C[i_inner][j_inner] = 0;
                        }

                        // A 的子矩陣與 B 的子矩陣相乘，結果累加到 C
                        for (int k_inner = k; k_inner < min(k + BLOCK_SIZE, size); k_inner++) {
                            C[i_inner][j_inner] += A[i_inner][k_inner] * B[k_inner][j_inner];
                        }
                    }
                }

                // 更新計算進度
                cout << "\r counting... " << flush;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            throw runtime_error("Usage: " + string(argv[0]) + " <matrix size>");
        }

        int size = stoi(argv[1]);
        int block_size = 32;
        string filename = to_string(size) + ".txt";

        vector<vector<int>> A, B, C;
        read_matrix(filename, A);
        read_matrix(filename, B);

        C.resize(size, vector<int>(size));

        cout << "------Start------" << endl;
        clock_t start = clock();
        matrix_multiply(A, B, C, block_size);
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