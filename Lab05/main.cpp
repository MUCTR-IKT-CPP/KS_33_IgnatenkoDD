#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <iomanip>

using namespace std;

const double MIN_RANDOM_COEFFICIENT = 0.5;
const double MAX_RANDOM_COEFFICIENT = 2.0;
const double REPAIR_PERCENT = 0.2;
const double REPLACEMENT_WEAR = 70.0;
const double BREAKDOWN_WEAR = 80.0;
const double SUDDEN_BREAKDOWN_CHANCE = 0.01;

random_device random_device_generator;
mt19937 random_generator(random_device_generator());


enum class EquipmentType {
    Ball,
    JumpRope,
    Dumbbells
};


/*
 * Генерация случайного целого числа.
 *
 * @param min_value минимальное значение.
 * @param max_value максимальное значение.
 * @return возвращает случайное целое число.
 */
int getRandomInt(int const min_value, int const max_value) {
    uniform_int_distribution<int> distribution(min_value, max_value);
    return distribution(random_generator);
}


/*
 * Генерация случайного вещественного числа.
 *
 * @param min_value минимальное значение.
 * @param max_value максимальное значение.
 * @return возвращает случайное вещественное число.
 */
double getRandomDouble(double const min_value, double const max_value) {
    uniform_real_distribution<double> distribution(min_value, max_value);
    return distribution(random_generator);
}


/*
 * Базовый класс спортивного инвентаря.
 */
class SportEquipment {
private:
    string _type;
    int _wear_resistance;
    double _cost;
    double _wear;
    int _usage_count;

protected:

    /*
     * Добавление износа спортивному инвентарю.
     *
     * @param wear_value величина дополнительного износа.
     * @return ничего не возвращает.
     */
    void addWear(double const wear_value) {
        _wear += wear_value;

        if(_wear > 100.0) {
            _wear = 100.0;
        }
    }

public:

    /*
     * Создание спортивного инвентаря.
     *
     * @param type тип инвентаря.
     * @param wear_resistance износостойкость.
     * @param cost стоимость.
     */
    SportEquipment(
        string const& type,
        int const wear_resistance,
        double const cost
    ) :
        _type(type),
        _wear_resistance(wear_resistance),
        _cost(cost),
        _wear(0.0),
        _usage_count(0) {
    }


    /*
     * Получение типа инвентаря.
     *
     * @return возвращает тип инвентаря.
     */
    string getType() const {
        return _type;
    }


    /*
     * Получение износостойкости.
     *
     * @return возвращает износостойкость.
     */
    int getWearResistance() const {
        return _wear_resistance;
    }


    /*
     * Получение стоимости.
     *
     * @return возвращает стоимость.
     */
    double getCost() const {
        return _cost;
    }


    /*
     * Получение текущего износа.
     *
     * @return возвращает процент износа.
     */
    double getWear() const {
        return _wear;
    }


    /*
     * Получение состояния инвентаря.
     *
     * @return возвращает оставшийся процент состояния.
     */
    double getCondition() const {
        return 100.0 - _wear;
    }


    /*
     * Получение количества использований.
     *
     * @return возвращает количество использований.
     */
    int getUsageCount() const {
        return _usage_count;
    }


    /*
     * Проверка доступности инвентаря.
     *
     * @return возвращает true, если предмет не сломан.
     */
    bool isAvailable() const {
        return _wear < 100.0;
    }


    /*
     * Проверка необходимости замены.
     *
     * @return возвращает true при износе больше 70 процентов.
     */
    bool needsReplacement() const {
        return _wear > REPLACEMENT_WEAR;
    }


    /*
     * Регистрация использования инвентаря.
     *
     * @return ничего не возвращает.
     */
    void registerUsage() {
        _usage_count++;
    }


    /*
     * Поломка инвентаря.
     *
     * @return ничего не возвращает.
     */
    void breakEquipment() {
        _wear = 100.0;
    }


    /*
     * Ремонт спортивного инвентаря.
     *
     * @return возвращает стоимость ремонта.
     */
    double repair() {
        double const old_wear = _wear;
        double const recovery_percent = getRandomDouble(0.5, 0.8);

        _wear -= _wear * recovery_percent;

        if(_wear < 0.0) {
            _wear = 0.0;
        }

        double const damage_coefficient = old_wear / 100.0;

        double const repair_cost =
            _cost * REPAIR_PERCENT * damage_coefficient;

        return repair_cost;
    }
};


/*
 * Класс мяча.
 */
class Ball : public SportEquipment {
private:
    double _diameter;

public:

    /*
     * Создание мяча.
     *
     * @param diameter диаметр мяча.
     * @param wear_resistance износостойкость.
     * @param cost стоимость.
     */
    Ball(
        double const diameter,
        int const wear_resistance,
        double const cost
    ) :
        SportEquipment("Мяч", wear_resistance, cost),
        _diameter(diameter) {
    }


    /*
     * Использование мяча.
     *
     * @param intensity интенсивность упражнения.
     * @param duration длительность упражнения.
     * @return возвращает полученный износ.
     */
    double use(
        int const intensity,
        int const duration
    ) {
        double const base_wear_rate = 0.06;

        double const random_coefficient = getRandomDouble(
            MIN_RANDOM_COEFFICIENT,
            MAX_RANDOM_COEFFICIENT
        );

        double const resistance_coefficient =
            (101.0 - getWearResistance()) / 100.0;

        double const wear_value =
            base_wear_rate *
            intensity *
            duration *
            random_coefficient *
            resistance_coefficient;

        addWear(wear_value);
        registerUsage();

        return wear_value;
    }


    /*
     * Получение диаметра мяча.
     *
     * @return возвращает диаметр мяча.
     */
    double getDiameter() const {
        return _diameter;
    }


    /*
     * Проверка состояния мяча.
     *
     * @return возвращает описание состояния.
     */
    string checkWear() const {
        if(getWear() >= 100.0) {
            return "Мяч сломан";
        }

        if(getWear() > REPLACEMENT_WEAR) {
            return "Мяч рекомендуется заменить";
        }

        return "Мяч пригоден к использованию";
    }
};


/*
 * Класс скакалки.
 */
class JumpRope : public SportEquipment {
private:
    double _length;

public:

    /*
     * Создание скакалки.
     *
     * @param length длина скакалки.
     * @param wear_resistance износостойкость.
     * @param cost стоимость.
     */
    JumpRope(
        double const length,
        int const wear_resistance,
        double const cost
    ) :
        SportEquipment("Скакалка", wear_resistance, cost),
        _length(length) {
    }


    /*
     * Использование скакалки.
     *
     * @param intensity интенсивность упражнения.
     * @param duration длительность упражнения.
     * @return возвращает полученный износ.
     */
    double use(
        int const intensity,
        int const duration
    ) {
        double const base_wear_rate = 0.09;

        double const random_coefficient = getRandomDouble(
            MIN_RANDOM_COEFFICIENT,
            MAX_RANDOM_COEFFICIENT
        );

        double const resistance_coefficient =
            (101.0 - getWearResistance()) / 100.0;

        double const wear_value =
            base_wear_rate *
            intensity *
            duration *
            random_coefficient *
            resistance_coefficient;

        addWear(wear_value);
        registerUsage();

        return wear_value;
    }


    /*
     * Получение длины скакалки.
     *
     * @return возвращает длину скакалки.
     */
    double getLength() const {
        return _length;
    }


    /*
     * Проверка состояния скакалки.
     *
     * @return возвращает описание состояния.
     */
    string checkWear() const {
        if(getWear() >= 100.0) {
            return "Скакалка сломана";
        }

        if(getWear() > REPLACEMENT_WEAR) {
            return "Скакалку рекомендуется заменить";
        }

        return "Скакалка пригодна к использованию";
    }
};


/*
 * Класс гантелей.
 */
class Dumbbells : public SportEquipment {
private:
    double _weight;

public:

    /*
     * Создание гантелей.
     *
     * @param weight вес гантелей.
     * @param wear_resistance износостойкость.
     * @param cost стоимость.
     */
    Dumbbells(
        double const weight,
        int const wear_resistance,
        double const cost
    ) :
        SportEquipment("Гантели", wear_resistance, cost),
        _weight(weight) {
    }


    /*
     * Использование гантелей.
     *
     * @param intensity интенсивность упражнения.
     * @param duration длительность упражнения.
     * @return возвращает полученный износ.
     */
    double use(
        int const intensity,
        int const duration
    ) {
        double const base_wear_rate = 0.04;

        double const random_coefficient = getRandomDouble(
            MIN_RANDOM_COEFFICIENT,
            MAX_RANDOM_COEFFICIENT
        );

        double const resistance_coefficient =
            (101.0 - getWearResistance()) / 100.0;

        double const wear_value =
            base_wear_rate *
            intensity *
            duration *
            random_coefficient *
            resistance_coefficient;

        addWear(wear_value);
        registerUsage();

        return wear_value;
    }


    /*
     * Получение веса гантелей.
     *
     * @return возвращает вес гантелей.
     */
    double getWeight() const {
        return _weight;
    }


    /*
     * Проверка состояния гантелей.
     *
     * @return возвращает описание состояния.
     */
    string checkWear() const {
        if(getWear() >= 100.0) {
            return "Гантели сломаны";
        }

        if(getWear() > REPLACEMENT_WEAR) {
            return "Гантели рекомендуется заменить";
        }

        return "Гантели пригодны к использованию";
    }
};


/*
 * Класс спортивного зала.
 */
class Gym {
private:
    vector<Ball> _balls;
    vector<JumpRope> _jump_ropes;
    vector<Dumbbells> _dumbbells;

    vector<string> _usage_history;

    double _total_repair_cost;

    /*
     * Проверка внезапной поломки предмета.
     *
     * @param item ссылка на предмет.
     * @return возвращает true, если предмет сломался.
     */
    bool checkSuddenBreakdown(SportEquipment& item) {
        if(
            item.getWear() > BREAKDOWN_WEAR &&
            getRandomDouble(0.0, 1.0) < SUDDEN_BREAKDOWN_CHANCE
        ) {
            item.breakEquipment();
            return true;
        }

        return false;
    }

public:

    /*
     * Создание спортивного зала.
     */
    Gym() :
        _total_repair_cost(0.0) {
    }


    /*
     * Добавление мяча.
     *
     * @param ball добавляемый мяч.
     * @return ничего не возвращает.
     */
    void addBall(Ball const& ball) {
        _balls.push_back(ball);
    }


    /*
     * Добавление скакалки.
     *
     * @param jump_rope добавляемая скакалка.
     * @return ничего не возвращает.
     */
    void addJumpRope(JumpRope const& jump_rope) {
        _jump_ropes.push_back(jump_rope);
    }


    /*
     * Добавление гантелей.
     *
     * @param dumbbells добавляемые гантели.
     * @return ничего не возвращает.
     */
    void addDumbbells(Dumbbells const& dumbbells) {
        _dumbbells.push_back(dumbbells);
    }


    /*
     * Проведение одной тренировки.
     *
     * @param show_process определяет необходимость вывода процесса.
     * @return ничего не возвращает.
     */
    void conductWorkout(bool const show_process = true) {
        int const exercise_count = getRandomInt(5, 15);

        if(show_process) {
            cout << "\nТРЕНИРОВКА\n";
            cout << "Количество упражнений: "
                 << exercise_count << "\n\n";
        }

        for(int i = 0; i < exercise_count; i++) {
            int const intensity = getRandomInt(1, 5);
            int const duration = getRandomInt(1, 10);

            EquipmentType const type =
                static_cast<EquipmentType>(getRandomInt(0, 2));

            if(type == EquipmentType::Ball && !_balls.empty()) {
                int const index = getRandomInt(
                    0,
                    static_cast<int>(_balls.size()) - 1
                );

                Ball& ball = _balls[index];

                if(!ball.isAvailable()) {
                    continue;
                }

                double const received_wear =
                    ball.use(intensity, duration);

                _usage_history.push_back(
                    "Мяч, интенсивность: " +
                    to_string(intensity) +
                    ", длительность: " +
                    to_string(duration)
                );

                if(show_process) {
                    cout << "Упражнение " << i + 1 << ":\n";
                    cout << "Инвентарь: Мяч\n";
                    cout << "Интенсивность: " << intensity << "\n";
                    cout << "Длительность: " << duration << " мин.\n";
                    cout << "Полученный износ: "
                         << fixed << setprecision(2)
                         << received_wear << "%\n";
                    cout << "Общий износ: "
                         << ball.getWear() << "%\n\n";
                }

                checkSuddenBreakdown(ball);
            }
            else if(
                type == EquipmentType::JumpRope &&
                !_jump_ropes.empty()
            ) {
                int const index = getRandomInt(
                    0,
                    static_cast<int>(_jump_ropes.size()) - 1
                );

                JumpRope& jump_rope = _jump_ropes[index];

                if(!jump_rope.isAvailable()) {
                    continue;
                }

                double const received_wear =
                    jump_rope.use(intensity, duration);

                _usage_history.push_back(
                    "Скакалка, интенсивность: " +
                    to_string(intensity) +
                    ", длительность: " +
                    to_string(duration)
                );

                if(show_process) {
                    cout << "Упражнение " << i + 1 << ":\n";
                    cout << "Инвентарь: Скакалка\n";
                    cout << "Интенсивность: " << intensity << "\n";
                    cout << "Длительность: " << duration << " мин.\n";
                    cout << "Полученный износ: "
                         << fixed << setprecision(2)
                         << received_wear << "%\n";
                    cout << "Общий износ: "
                         << jump_rope.getWear() << "%\n\n";
                }

                checkSuddenBreakdown(jump_rope);
            }
            else if(
                type == EquipmentType::Dumbbells &&
                !_dumbbells.empty()
            ) {
                int const index = getRandomInt(
                    0,
                    static_cast<int>(_dumbbells.size()) - 1
                );

                Dumbbells& dumbbells = _dumbbells[index];

                if(!dumbbells.isAvailable()) {
                    continue;
                }

                double const received_wear =
                    dumbbells.use(intensity, duration);

                _usage_history.push_back(
                    "Гантели, интенсивность: " +
                    to_string(intensity) +
                    ", длительность: " +
                    to_string(duration)
                );

                if(show_process) {
                    cout << "Упражнение " << i + 1 << ":\n";
                    cout << "Инвентарь: Гантели\n";
                    cout << "Интенсивность: " << intensity << "\n";
                    cout << "Длительность: " << duration << " мин.\n";
                    cout << "Полученный износ: "
                         << fixed << setprecision(2)
                         << received_wear << "%\n";
                    cout << "Общий износ: "
                         << dumbbells.getWear() << "%\n\n";
                }

                checkSuddenBreakdown(dumbbells);
            }
            else {
                i--;
            }
        }
    }


    /*
     * Вывод состояния всего спортивного инвентаря.
     *
     * @return ничего не возвращает.
     */
    void printReport() const {
        cout << "\nОТЧЕТ\n";

        cout << "\nМячи:\n";

        for(size_t i = 0; i < _balls.size(); i++) {
            cout << i + 1
                 << ". Износ: "
                 << fixed << setprecision(2)
                 << _balls[i].getWear()
                 << "%, состояние: "
                 << _balls[i].getCondition()
                 << "%\n";
        }

        cout << "\nСкакалки:\n";

        for(size_t i = 0; i < _jump_ropes.size(); i++) {
            cout << i + 1
                 << ". Износ: "
                 << _jump_ropes[i].getWear()
                 << "%, состояние: "
                 << _jump_ropes[i].getCondition()
                 << "%\n";
        }

        cout << "\nГантели:\n";

        for(size_t i = 0; i < _dumbbells.size(); i++) {
            cout << i + 1
                 << ". Износ: "
                 << _dumbbells[i].getWear()
                 << "%, состояние: "
                 << _dumbbells[i].getCondition()
                 << "%\n";
        }
    }


    /*
     * Выполнение ремонта всего сильно изношенного инвентаря.
     *
     * @return ничего не возвращает.
     */
    void repairEquipment() {
        for(Ball& ball : _balls) {
            if(ball.getWear() > REPLACEMENT_WEAR) {
                _total_repair_cost += ball.repair();
            }
        }

        for(JumpRope& jump_rope : _jump_ropes) {
            if(jump_rope.getWear() > REPLACEMENT_WEAR) {
                _total_repair_cost += jump_rope.repair();
            }
        }

        for(Dumbbells& dumbbells : _dumbbells) {
            if(dumbbells.getWear() > REPLACEMENT_WEAR) {
                _total_repair_cost += dumbbells.repair();
            }
        }
    }


    /*
     * Проведение ста тренировок.
     *
     * @return ничего не возвращает.
     */
    void simulateHundredWorkouts() {
        const int WORKOUT_COUNT = 100;

        for(int i = 0; i < WORKOUT_COUNT; i++) {
            conductWorkout(false);
            repairEquipment();
        }
    }


    /*
     * Вывод истории использования.
     *
     * @return ничего не возвращает.
     */
    void printUsageHistory() const {
        cout << "\nИСТОРИЯ\n";

        for(size_t i = 0; i < _usage_history.size(); i++) {
            cout << i + 1
                 << ". "
                 << _usage_history[i]
                 << "\n";
        }
    }


    /*
     * Вывод общей стоимости ремонта.
     *
     * @return ничего не возвращает.
     */
    void printRepairCost() const {
        cout << "\nОбщая стоимость ремонта: "
             << fixed << setprecision(2)
             << _total_repair_cost
             << " руб.\n";
    }
};


/*
 * Главная функция программы.
 *
 * @return возвращает код завершения программы.
 */
int main() {
    setlocale(LC_ALL, "Russian");

    Gym gym;

    gym.addBall(
        Ball(22.0, 75, 2500.0)
    );

    gym.addBall(
        Ball(24.0, 80, 3200.0)
    );

    gym.addJumpRope(
        JumpRope(2.8, 60, 1200.0)
    );

    gym.addJumpRope(
        JumpRope(3.0, 70, 1500.0)
    );

    gym.addDumbbells(
        Dumbbells(5.0, 90, 5000.0)
    );

    gym.addDumbbells(
        Dumbbells(10.0, 95, 8000.0)
    );

    gym.conductWorkout();
    gym.printReport();
    gym.printUsageHistory();
    gym.repairEquipment();
    gym.simulateHundredWorkouts();
    gym.printReport();
    gym.printRepairCost();

    return 0;
}
