#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
 * Вывод карты бухты на экран.
 *
 * @param p_map двумерный массив с картой.
 * @param n размер карты.
 */
void printMap(int** p_map, int n)
{
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << p_map[i][j] << " ";
        }

        cout << endl;
    }
}

/*
 * Определение размера одного корабля.
 *
 * Функция рекурсивно проверяет соседние клетки
 * по вертикали и горизонтали.
 *
 * @param p_map двумерный массив с картой.
 * @param p_used массив посещенных клеток.
 * @param n размер карты.
 * @param row номер строки.
 * @param column номер столбца.
 * @return возвращает размер корабля.
 */
int shipSize(
    int** p_map,
    bool** p_used,
    int n,
    int row,
    int column
)
{
    if(
        row < 0 ||
        column < 0 ||
        row >= n ||
        column >= n
    ){
        return 0;
    }

    if(p_map[row][column] == 0 || p_used[row][column]){
        return 0;
    }

    p_used[row][column] = true;

    int size = 1;

    size += shipSize(p_map, p_used, n, row + 1, column);
    size += shipSize(p_map, p_used, n, row - 1, column);
    size += shipSize(p_map, p_used, n, row, column + 1);
    size += shipSize(p_map, p_used, n, row, column - 1);

    return size;
}

/*
 * Подсчет количества кораблей и определение их размеров.
 *
 * @param p_map двумерный массив с картой.
 * @param n размер карты.
 */
void countShips(int** p_map, int n)
{
    bool** p_used = new bool*[n];

    for(int i = 0; i < n; i++){
        p_used[i] = new bool[n]{};
    }

    int ship_count = 0;

    cout << "Размеры кораблей: ";

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(p_map[i][j] == 1 && !p_used[i][j]){
                int ship_size = shipSize(p_map, p_used, n, i, j);

                cout << ship_size << " ";
                ship_count++;
            }
        }
    }

    cout << "\nКоличество кораблей: " << ship_count << endl;

    for(int i = 0; i < n; i++){
        delete[] p_used[i];
    }

    delete[] p_used;
}

/*
 * Зеркальное отражение карты бухты.
 *
 * Функция меняет местами элементы
 * каждой строки относительно вертикальной оси.
 *
 * @param p_map двумерный массив с картой.
 * @param n размер карты.
 */
void mirrorMap(int** p_map, int n)
{
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n / 2; j++){
            int temp = p_map[i][j];

            p_map[i][j] = p_map[i][n - 1 - j];
            p_map[i][n - 1 - j] = temp;
        }
    }
}

/*
 * Удаление корабля по указанным координатам.
 *
 * Функция использует параметры-указатели.
 * Значение выбранной клетки заменяется на 0.
 *
 * @param p_map двумерный массив с картой.
 * @param p_row указатель на номер строки.
 * @param p_column указатель на номер столбца.
 */
void deleteShip(int** p_map, int* p_row, int* p_column)
{
    p_map[*p_row][*p_column] = 0;
}

/*
 * Подсчет количества клеток, в которых находятся корабли.
 *
 * Массив передается в функцию через параметр типа void*.
 *
 * @param p_data указатель типа void* на двумерный массив.
 * @param n размер карты.
 */
void countShipsVoid(void* p_data, int n)
{
    int** p_map = (int**)p_data;

    int ship_cells_count = 0;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(p_map[i][j] == 1){
                ship_cells_count++;
            }
        }
    }

    cout << "Количество клеток с кораблями: "
         << ship_cells_count << endl;
}

/*
 * Главная функция программы.
 *
 * Создает динамический двумерный массив,
 * заполняет его случайными значениями 0 и 1,
 * выводит меню и выполняет выбранную пользователем операцию.
 *
 * @return возвращает 0 при успешном завершении программы.
 */
int main()
{
    srand(time(0));

    int n = 0;

    cout << "Введите N: ";
    cin >> n;

    int** p_map = new int*[n];

    for(int i = 0; i < n; i++){
        p_map[i] = new int[n];
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            p_map[i][j] = rand() % 2;
        }
    }

    cout << "\nКарта:\n";
    printMap(p_map, n);

    int choice = 0;

    cout << "\n1 - Подсчитать корабли\n";
    cout << "2 - Отразить карту\n";
    cout << "3 - Удалить корабль по координатам\n";
    cout << "4 - Подсчитать через void*\n";
    cout << "Выберите действие: ";

    cin >> choice;

    if(choice == 1){
        countShips(p_map, n);
    }
    else if(choice == 2){
        mirrorMap(p_map, n);
        printMap(p_map, n);
    }
    else if(choice == 3){
        int row = 0;
        int column = 0;

        cout << "Введите строку и столбец: ";
        cin >> row >> column;

        deleteShip(p_map, &row, &column);

        printMap(p_map, n);
    }
    else if(choice == 4){
        countShipsVoid((void*)p_map, n);
    }

    for(int i = 0; i < n; i++){
        delete[] p_map[i];
    }

    delete[] p_map;

    return 0;
}