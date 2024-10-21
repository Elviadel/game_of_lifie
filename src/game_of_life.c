#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 25
#define WIDTH 80

void ChangeSpeed(int *delay, char direction);
int Input(int **field);
void InitializeFieldFromFile(int ***field, const char *filename);
void PrintField(int **field, int gen);
void UpdateField(int ***field);
int CountNeighbors(int **field, int x, int y);

int main() {
    int gen = 0;
    int input_gen = 0;
    int **field;
    int delay = 500000000;
    while (1) {
        printf(
            "Введите число от 1 до 5 для инициализации фигур или число 6 для ручного ввода, знак + или - для "
            "изменения скорости, q чтобы выйти: ");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'q' || choice == 'Q') {
            return 0;
        } else if (choice == '+' || choice == '-') {
            ChangeSpeed(&delay, choice);
        } else {
            int fileNumber = choice - '0';
            if (fileNumber >= 1 && fileNumber <= 6) {
                char filename[20];
                snprintf(filename, sizeof(filename), "input%d.txt", fileNumber);
                InitializeFieldFromFile(&field, filename);
                if (fileNumber == 6) {
                    Input(field);
                }
                printf("Введите количество поколений: ");
                scanf("%d", &input_gen);
                while (gen != input_gen) {
                    gen++;
                    PrintField(field, gen);
                    UpdateField(&field);
                    for (int i = 0; i < delay; i++) {
                    }
                }
                break;
            } else {
                printf("Введено неверное число, повторите ввод: \n");
            }
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        free(field[i]);
    }
    free(field);
    return 0;
}

int Input(int **field) {
    int x, y;
    for (int i = 0; i < 2000; i++) {
        if (scanf("%d %d", &x, &y) == 2 && x >= -1 && y >= -1 && y <= 79 && x <= 24) {
            if (x == -1 || y == -1) {
                break;
            }
            field[x][y] = 1;
        } else {
            printf("n/a");
            exit(1);
        }
    }
    return 0;
}

int **AllocateFieldMemory() {
    int **field = (int **)malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++) {
        field[i] = (int *)malloc(WIDTH * sizeof(int));
    }
    return field;
}

void InitializeFieldFromFile(int ***field, const char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Ошибка открытия файла: %s\n", filename);
        exit(1);
    }

    *field = AllocateFieldMemory();

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char ch;
            if (fscanf(file, " %c", &ch) != 1) {
                printf("Ошибка чтения файла: %s\n", filename);
                fclose(file);
                exit(1);
            }
            (*field)[i][j] = (ch == '1') ? 1 : 0;
        }
    }
    fclose(file);
}

void PrintField(int **field, int gen) {
    system("clear");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 1) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("Generations: %d\n", gen);
}

int CountNeighbors(int **field, int x, int y) {
    int count = 0;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            int cell_x = i;
            int cell_y = j;
            if (cell_x < 0) {
                cell_x = HEIGHT - 1;
            } else if (cell_x >= HEIGHT) {
                cell_x = 0;
            }
            if (cell_y < 0) {
                cell_y = WIDTH - 1;
            } else if (cell_y >= WIDTH) {
                cell_y = 0;
            }
            count += field[cell_x][cell_y];
        }
    }
    count -= field[x][y];
    return count;
}

void UpdateField(int ***field) {
    int **new_field = AllocateFieldMemory();

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int neighbors = CountNeighbors(*field, i, j);
            if ((*field)[i][j] == 1) {
                if (neighbors < 2 || neighbors > 3) {
                    new_field[i][j] = 0;
                } else {
                    new_field[i][j] = 1;
                }
            } else {
                if (neighbors == 3) {
                    new_field[i][j] = 1;
                } else {
                    new_field[i][j] = 0;
                }
            }
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        free((*field)[i]);
    }
    free(*field);
    *field = new_field;
}

void ChangeSpeed(int *delay, char direction) {
    if (direction == '+' && *delay > 0) {
        *delay -= 20000000;
        if (*delay == 0) {
            printf("Установлена максимальная скорость!");
        }
    }
    if (direction == '-' && *delay < 2000000000) {
        *delay += 20000000;
        if (*delay == 2000000000) {
            printf("Установлена минимальная скорость!");
        }
    }
    if (*delay < 0) {
        *delay = 0;
    }
    printf("Скорость итерации: %d \n", *delay);
}