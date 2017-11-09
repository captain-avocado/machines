#include <ncurses.h>

#define TICKRATE 150

#define WORLD_WIDTH 50
#define WORLD_HEIGHT 20

enum direction { UP, DOWN, RIGHT, LEFT };

typedef struct symb {
    int x;
    int y;
} symb;

int move_symb(WINDOW *win, int direction,
                symb *s);

int main(int argc, char *argv[]) {
    
    //выделение памяти под stdscr(стандартный экран терминала)
    initscr();
    
    //введенные с клавиатуры символы не печаются на экране
    noecho();
    
    //отмена строчной буферизации (без этой функции символ не будет напечатан, пока не будет достигнут конец строки)
    cbreak();
    
    //устанавливает задержку ввода равную TICKRATE (если пользователь ничего не ввел в течение указанного периода времени, возвращает ERR)
    timeout(TICKRATE);
    
    //включает специальные клавиши (F-, Arrow-keys) для ввода
    keypad(stdscr, TRUE);
    
    //форматированные вывод на главный экран (stdscr)
    printw("Press (q) to quit...");
    
    //обновляет содержимое экрана
    refresh();
    
    //объявление окна, в котором будет перемещаться символ
    WINDOW *area_for_moving;
    
    //объявление символа ввода в клавиатуры и начальных позиций символа
    int offsetx, offsety, ch;
    
    //инициализация параметров окна
    offsetx = (COLS - WORLD_WIDTH) / 2;
    offsety = (LINES - WORLD_HEIGHT) / 2;
    area_for_moving = newwin(WORLD_HEIGHT,
                           WORLD_WIDTH,
                           offsety,
                           offsetx);
    
    //отображение границы окна
    box(area_for_moving, 0 , 0);
    
    //объявление и инициализация полей символа
    symb s;
    s.x = (WORLD_WIDTH - 1) / 2;
    s.y = (WORLD_HEIGHT - 1) / 2;
    
    //печать символа в окне
    mvwaddch(area_for_moving, s.y, s.x, '#');
    wrefresh(area_for_moving);

    int cur_dir = RIGHT;
    while ((ch = getch()) != 'q') {
        move_symb(area_for_moving, cur_dir, &s);
        if (ch != ERR) {
            switch (ch) {
                case KEY_UP:
                    cur_dir = UP;
                    break;
                case KEY_DOWN:
                    cur_dir = DOWN;
                    break;
                case KEY_RIGHT:
                    cur_dir = RIGHT;
                    break;
                case KEY_LEFT:
                    cur_dir = LEFT;
                    break;
                default:
                    break;
            }
        }
    }
    
    //освобождение памяти, выделенной для окна и экрана
    delwin(area_for_moving);
    endwin();
    return 0;
}

int move_symb(WINDOW *win, int direction, symb *s) {
    //в функцию передается указатель на структуру, чтобы "запомнить" ее координаты, которые будут изменены в функции
    
    wclear(win);
    switch (direction) {
        case UP:
            s->y - 1 == 0 ? s->y = WORLD_HEIGHT - 2 : s->y--;
            break;
        case DOWN:
            s->y + 1 == WORLD_HEIGHT - 1 ? s->y = 1 : s->y++;
            break;
        case RIGHT:
            s->x + 1 == WORLD_WIDTH - 1 ? s->x = 1 : s->x++;
            break;
        case LEFT:
            s->x - 1 == 0 ? s->x = WORLD_WIDTH - 2 : s->x--;
            break;
        default:
            break;
    }
    
    //печать символа
    mvwaddch(win, s->y, s->x, '#');
    
    //прорисовка границы окна
    box(win, 0 , 0);
    
    //обновление окна
    wrefresh(win);
    return 0;
}
