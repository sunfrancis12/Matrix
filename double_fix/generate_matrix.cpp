#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_and_save_matrix(int size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("無法創建文件 %s\n", filename);
        return;
    }

    fprintf(file, "%d\n", size);  // 先寫入矩陣大小

    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            double random_value = (double)rand() / RAND_MAX * 10.0;  // 生成0-10之間的隨機浮點數
            fprintf(file, "%.6f ", random_value);  // 保留6位小數
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("矩陣已保存到 %s\n", filename);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("使用方法: %s <矩陣大小>\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);
    if (size <= 0) {
        printf("矩陣大小必須為正整數\n");
        return 1;
    }

    char filename[20];
    sprintf(filename, "%d.txt", size);
    
    generate_and_save_matrix(size, filename);

    return 0;
}