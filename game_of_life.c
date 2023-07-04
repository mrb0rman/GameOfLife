#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80
void init_world(int data[HEIGHT][WIDTH]);  // Инициализация первого поколения
                                           // случайными значениями
int get_live_count(int data[HEIGHT][WIDTH]);  // Подсчет живых клеток
int count_live_neighbors(
    int data[HEIGHT][WIDTH], int x,
    int y);  // Количество живых соседей у клетки с координатами x, y
void read_point_neighbors(int nb[][2], int x,
                          int y);  // Получение координат соседей точки
void next_generation(
    int data[HEIGHT][WIDTH],
    int prev_is_live[HEIGHT]
                    [WIDTH]);  // Сгенерировать следующее поколение игрового мира
void copy_world(int data[HEIGHT][WIDTH],
                int prev_is_live[HEIGHT][WIDTH]);  // Копирование игрового мира
void print_world(int data[HEIGHT][WIDTH]);  // Вывод игрового поля
int cmp_world(int data[HEIGHT][WIDTH],
              int prev_is_live[HEIGHT][WIDTH]);  // Проверка на статичность поля
// Много функций координат клеток
void left_up(int nb[][2], int x, int y);
void up(int nb[][2], int y);
void right_up(int nb[][2], int x, int y);
void right(int nb[][2], int x);
void right_down(int nb[][2], int x, int y);
void down(int nb[][2], int y);
void left_down(int nb[][2], int x, int y);
void left(int nb[][2], int x);
// Доп функции
int speed_select(int *speed);

int main() {
  int is_live[HEIGHT][WIDTH], prev_is_live[HEIGHT][WIDTH], speed = 1;
  init_world(is_live);
  initscr();
  int live_points = -1;
  int is_optimal = 0;
  do {
    clear();
    print_world(is_live);
    copy_world(is_live, prev_is_live);
    next_generation(is_live, prev_is_live);
    is_optimal = cmp_world(is_live, prev_is_live);
    live_points = get_live_count(is_live);
      if (freopen("/dev/tty", "r", stdin) == NULL) {
      } else {
          speed_select(&speed);
          usleep(100000 / speed);
      }
  } while (live_points != 0 && is_optimal == 0);
  endwin();
  if (is_optimal) {
    printf("Optimal configuration detected");
  }
  if (live_points == 0) {
    printf("All points died");
  }
  return 0;
}
void init_world(int data[HEIGHT][WIDTH]) {
  char c;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      // считывание из файла и запись в массивы
      scanf("%c", &c);
      if (c == '0') {
        data[i][j] = 0;
      } else if (c == '1') {
        data[i][j] = 1;
      }
    }
  }
}
int speed_select(int *speed) {
  halfdelay(1);
  char sp = getch();
  if (sp == '1') {
    *speed = 1;
  } else if (sp == '2') {
    *speed = 2;
  } else if (sp == '3') {
    *speed = 3;
  } else if (sp == '4') {
    *speed = 4;
  } else if (sp == '5') {
    *speed = 5;
  } else if (sp == '6') {
    *speed = 6;
  } else if (sp == '7') {
    *speed = 7;
  } else if (sp == '8') {
    *speed = 8;
  } else if (sp == '9') {
    *speed = 9;
  }
  if (*speed <= 0)
    *speed = 1;
  return *speed;
}
int get_live_count(int data[HEIGHT][WIDTH]) {
  int count = 0;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (data[i][j] == 1)
        count++;
    }
  }
  return count;
}
int count_live_neighbors(int data[HEIGHT][WIDTH], int x, int y) {
  int count = 0;
  int i;
  int nb[8][2];
  read_point_neighbors(nb, x, y);
  for (i = 0; i < 8; i++) {
      if (data[nb[i][0]][nb[i][1]] == 1) count++;
  }
  return count;
}
void read_point_neighbors(int nb[][2], int x, int y) {
  int i, j;
  int k = 0;
  for (i = x - 1; i <= x + 1; i++) {
    for (j = y - 1; j <= y + 1; j++) {
      if (i == x && j == y) {
      } else {
        nb[k][0] = i;
        nb[k][1] = j;
        k++;
      }
    }
  }
  if (x == 0 && y == 0) {  // Левый вверх
    left_up(nb, x, y);
  } else if (x == 0 && y == WIDTH - 1) {  // Правый вверх
    right_up(nb, x, y);
  } else if (x == HEIGHT - 1 && y == WIDTH - 1) {  // Правый низ
    right_down(nb, x, y);
  } else if (x == HEIGHT - 1 && y == 0) {  // Левый низ
    left_down(nb, x, y);
  } else if (x == 0 && y != 0 && y != WIDTH - 1) {  // Вверхнии
    up(nb, y);
  } else if (x != 0 && x != HEIGHT - 1 && y == WIDTH - 1) {  // Правые
    right(nb, x);
  } else if (x == HEIGHT - 1 && y != 0 && y != WIDTH - 1) {  // Нижние
    down(nb, y);
  } else if (x != 0 && x != HEIGHT - 1 && y == 0) {  // Левые
    left(nb, x);
  }
}
void left_up(int nb[][2], int x, int y) {
  nb[0][0] = HEIGHT - 1;
  nb[0][1] = WIDTH - 1;
  nb[1][0] = HEIGHT - 1;
  nb[1][1] = y;
  nb[2][0] = HEIGHT - 1;
  nb[2][1] = y + 1;
  nb[3][0] = x;
  nb[3][1] = WIDTH - 1;
  nb[5][0] = x + 1;
  nb[5][1] = WIDTH - 1;
}
void up(int nb[][2], int y) {
  nb[0][0] = HEIGHT - 1;
  nb[0][1] = y - 1;
  nb[1][0] = HEIGHT - 1;
  nb[1][1] = y;
  nb[2][0] = HEIGHT - 1;
  nb[2][1] = y + 1;
}
void right_up(int nb[][2], int x, int y) {
  nb[0][0] = HEIGHT - 1;
  nb[0][1] = y - 1;
  nb[1][0] = HEIGHT - 1;
  nb[1][1] = y;
  nb[2][0] = HEIGHT - 1;
  nb[2][1] = 0;
  nb[4][0] = x;
  nb[4][1] = 0;
  nb[7][0] = x + 1;
  nb[7][1] = 0;
}
void right(int nb[][2], int x) {
  nb[2][0] = x - 1;
  nb[2][1] = 0;
  nb[4][0] = x;
  nb[4][1] = 0;
  nb[7][0] = x + 1;
  nb[7][1] = 0;
}
void right_down(int nb[][2], int x, int y) {
  nb[2][0] = x - 1;
  nb[2][1] = 0;
  nb[4][0] = x;
  nb[4][1] = 0;
  nb[5][0] = 0;
  nb[5][1] = y - 1;
  nb[6][0] = 0;
  nb[6][1] = y;
  nb[7][0] = 0;
  nb[7][1] = 0;
}
void down(int nb[][2], int y) {
  nb[5][0] = 0;
  nb[5][1] = y - 1;
  nb[6][0] = 0;
  nb[6][1] = y;
  nb[7][0] = 0;
  nb[7][1] = y + 1;
}
void left_down(int nb[][2], int x, int y) {
  nb[0][0] = x - 1;
  nb[0][1] = WIDTH - 1;
  nb[3][0] = x;
  nb[3][1] = WIDTH - 1;
  nb[5][0] = 0;
  nb[5][1] = WIDTH - 1;
  nb[6][0] = 0;
  nb[6][1] = y;
  nb[7][0] = 0;
  nb[7][1] = y + 1;
}
void left(int nb[][2], int x) {
  nb[0][0] = x - 1;
  nb[0][1] = WIDTH - 1;
  nb[3][0] = x;
  nb[3][1] = WIDTH - 1;
  nb[5][0] = x + 1;
  nb[5][1] = WIDTH - 1;
}

void next_generation(int data[HEIGHT][WIDTH], int prev_is_live[HEIGHT][WIDTH]) {
  int i, j;
  int live_nb;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      live_nb = count_live_neighbors(prev_is_live, i, j);
      if (prev_is_live[i][j] == 0) {
        if (live_nb == 3) {
          data[i][j] = 1;
        }
      } else {
        if (live_nb < 2 || live_nb > 3) {
          data[i][j] = 0;
        }
      }
    }
  }
}
void copy_world(int data[HEIGHT][WIDTH], int prev_is_live[HEIGHT][WIDTH]) {
  int i, j;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      prev_is_live[i][j] = data[i][j];
    }
  }
}
void print_world(int data[HEIGHT][WIDTH]) {
  int i, j;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      if (data[i][j] == 1) {
        printw("*");
      } else {
        printw(" ");
      }
    }
    printw("\n");
  }
  refresh();
}
int cmp_world(int data[HEIGHT][WIDTH], int prev_is_live[HEIGHT][WIDTH]) {
  int i, j;
  int cout = 1;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      if (data[i][j] != prev_is_live[i][j]) {
        cout = 0;
      }
    }
  }
  return cout;
}
