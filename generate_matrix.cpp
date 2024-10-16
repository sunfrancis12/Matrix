// 利用亂數生成一個大小為N的矩陣，並將其保存到文件中
// 使用方法: ./generate_matrix <矩陣大小>

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
            fprintf(file, "%d ", rand() % 10);  // 生成0-9之間的隨機整數
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