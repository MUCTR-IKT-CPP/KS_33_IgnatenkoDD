#include <iostream>
#include <cmath>

const double PI = 3.141592653589793;
const int MAX_SHAPES = 10;

/*
 * Интерфейс масштабирования геометрических фигур.
 */
class IScalable{
public:

    /*
     * Масштабирование геометрической фигуры.
     *
     * @param factor коэффициент масштабирования.
     */
    virtual void scale(double const factor) = 0;

    /*
     * Виртуальный деструктор интерфейса.
     */
    virtual ~IScalable() = default;
};

/*
 * Абстрактный базовый класс геометрической фигуры.
 */
class Shape{
public:

    /*
     * Вычисление площади геометрической фигуры.
     *
     * @return возвращает площадь фигуры.
     */
    virtual double area() const = 0;

    /*
     * Вычисление периметра геометрической фигуры.
     *
     * @return возвращает периметр фигуры.
     */
    virtual double perimeter() const = 0;

    /*
     * Вывод информации о геометрической фигуре.
     */
    virtual void printInfo() const = 0;

    /*
     * Виртуальный деструктор базового класса.
     */
    virtual ~Shape() = default;
};

/*
 * Класс окружности.
 */
class Circle : public Shape, public IScalable{
private:
    double _radius;

public:

    /*
     * Создание окружности с заданным радиусом.
     *
     * @param radius радиус окружности.
     */
    Circle(double const radius) : _radius(radius){}

    /*
     * Вычисление площади окружности.
     *
     * @return возвращает площадь окружности.
     */
    double area() const override{
        return PI * _radius * _radius;
    }

    /*
     * Вычисление длины окружности.
     *
     * @return возвращает длину окружности.
     */
    double perimeter() const override{
        return 2.0 * PI * _radius;
    }

    /*
     * Вывод информации об окружности.
     */
    void printInfo() const override{
        std::cout << "Окружность" << std::endl;
        std::cout << "Радиус: " << _radius << std::endl;
        std::cout << "Площадь: " << area() << std::endl;
        std::cout << "Периметр: " << perimeter() << std::endl;
    }

    /*
     * Масштабирование окружности.
     *
     * @param factor коэффициент масштабирования.
     */
    void scale(double const factor) override{
        if(factor > 0.0){
            _radius *= factor;
        }
    }
};

/*
 * Класс прямоугольника.
 */
class Rectangle : public Shape, public IScalable{
private:
    double _width;
    double _height;

public:

    /*
     * Создание прямоугольника с заданными сторонами.
     *
     * @param width ширина прямоугольника.
     * @param height высота прямоугольника.
     */
    Rectangle(double const width, double const height)
        : _width(width), _height(height){}

    /*
     * Вычисление площади прямоугольника.
     *
     * @return возвращает площадь прямоугольника.
     */
    double area() const override{
        return _width * _height;
    }

    /*
     * Вычисление периметра прямоугольника.
     *
     * @return возвращает периметр прямоугольника.
     */
    double perimeter() const override{
        return 2.0 * (_width + _height);
    }

    /*
     * Вывод информации о прямоугольнике.
     */
    void printInfo() const override{
        std::cout << "Прямоугольник" << std::endl;
        std::cout << "Ширина: " << _width << std::endl;
        std::cout << "Высота: " << _height << std::endl;
        std::cout << "Площадь: " << area() << std::endl;
        std::cout << "Периметр: " << perimeter() << std::endl;
    }

    /*
     * Масштабирование прямоугольника.
     *
     * @param factor коэффициент масштабирования.
     */
    void scale(double const factor) override{
        if(factor > 0.0){
            _width *= factor;
            _height *= factor;
        }
    }
};

/*
 * Класс треугольника.
 */
class Triangle : public Shape{
private:
    double _side_a;
    double _side_b;
    double _side_c;

public:

    /*
     * Создание треугольника с заданными сторонами.
     *
     * @param side_a первая сторона треугольника.
     * @param side_b вторая сторона треугольника.
     * @param side_c третья сторона треугольника.
     */
    Triangle(
        double const side_a,
        double const side_b,
        double const side_c
    ) : _side_a(side_a), _side_b(side_b), _side_c(side_c){}

    /*
     * Вычисление площади треугольника по формуле Герона.
     *
     * @return возвращает площадь треугольника.
     */
    double area() const override{
        double const half_perimeter = perimeter() / 2.0;

        return std::sqrt(
            half_perimeter *
            (half_perimeter - _side_a) *
            (half_perimeter - _side_b) *
            (half_perimeter - _side_c)
        );
    }

    /*
     * Вычисление периметра треугольника.
     *
     * @return возвращает периметр треугольника.
     */
    double perimeter() const override{
        return _side_a + _side_b + _side_c;
    }

    /*
     * Вывод информации о треугольнике.
     */
    void printInfo() const override{
        std::cout << "Треугольник" << std::endl;
        std::cout << "Сторона A: " << _side_a << std::endl;
        std::cout << "Сторона B: " << _side_b << std::endl;
        std::cout << "Сторона C: " << _side_c << std::endl;
        std::cout << "Площадь: " << area() << std::endl;
        std::cout << "Периметр: " << perimeter() << std::endl;
    }
};

/*
 * Класс группы геометрических фигур.
 */
class ShapeGroup{
private:
    Shape* _shapes[MAX_SHAPES];
    int _shape_count;

public:

    /*
     * Создание пустой группы геометрических фигур.
     */
    ShapeGroup() : _shapes{}, _shape_count(0){}

    /*
     * Добавление геометрической фигуры в группу.
     *
     * @param p_shape указатель на добавляемую фигуру.
     */
    void addShape(Shape* p_shape){
        if(p_shape != nullptr && _shape_count < MAX_SHAPES){
            _shapes[_shape_count] = p_shape;
            _shape_count++;
        }
    }

    /*
     * Вычисление общей площади всех фигур группы.
     *
     * @return возвращает общую площадь фигур.
     */
    double totalArea() const{
        double total_area = 0.0;

        for(int i = 0; i < _shape_count; i++){
            total_area += _shapes[i]->area();
        }

        return total_area;
    }

    /*
     * Вычисление общего периметра всех фигур группы.
     *
     * @return возвращает сумму периметров фигур.
     */
    double totalPerimeter() const{
        double total_perimeter = 0.0;

        for(int i = 0; i < _shape_count; i++){
            total_perimeter += _shapes[i]->perimeter();
        }

        return total_perimeter;
    }

    /*
     * Вывод информации обо всех фигурах группы.
     */
    void printInfo() const{
        for(int i = 0; i < _shape_count; i++){
            _shapes[i]->printInfo();
            std::cout << std::endl;
        }
    }
};

/*
 * Главная функция программы.
 *
 * @return возвращает код завершения программы.
 */
int main(){
    Circle circle(5.0);
    Rectangle rectangle(4.0, 6.0);
    Triangle triangle(3.0, 4.0, 5.0);

    Shape* p_shapes[] = {
        &circle,
        &rectangle,
        &triangle
    };

    const int SHAPE_COUNT = 3;

    std::cout << "Исходные параметры фигур:" << std::endl;
    std::cout << "--------------------------" << std::endl;

    for(int i = 0; i < SHAPE_COUNT; i++){
        p_shapes[i]->printInfo();
        std::cout << std::endl;
    }

    ShapeGroup group;

    group.addShape(&circle);
    group.addShape(&rectangle);
    group.addShape(&triangle);

    std::cout << "Общая площадь группы: "
              << group.totalArea() << std::endl;

    std::cout << "Общий периметр группы: "
              << group.totalPerimeter() << std::endl;

    std::cout << std::endl;
    std::cout << "Масштабирование фигур:" << std::endl;

    circle.scale(2.0);
    rectangle.scale(1.5);

    std::cout << "После масштабирования:" << std::endl;
    std::cout << std::endl;

    group.printInfo();

    std::cout << "Новая общая площадь группы: "
              << group.totalArea() << std::endl;

    std::cout << "Новый общий периметр группы: "
              << group.totalPerimeter() << std::endl;

    return 0;
}