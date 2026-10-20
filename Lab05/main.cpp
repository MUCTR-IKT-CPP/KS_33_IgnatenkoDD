#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <iomanip>
#include <algorithm>

using namespace std;

const double MIN_RANDOM_COEFFICIENT = 0.5;
const double MAX_RANDOM_COEFFICIENT = 2.0;
const double REPAIR_PERCENT = 0.2;
const double REPLACEMENT_WEAR = 70.0;
const double BREAKDOWN_WEAR = 80.0;
const double SUDDEN_BREAKDOWN_CHANCE = 0.01;

random_device random_device_generator;
mt19937 random_generator(random_device_generator());


/*
 * Генерация случайного целого числа в заданном диапазоне.
 *
 * @param min_value минимальное возможное значение.
 * @param max_value максимальное возможное значение.
 * @return возвращает случайное целое число.
 */
int getRandomInt(int const min_value, int const max_value) {
    uniform_int_distribution<int> distribution(min_value, max_value);
    return distribution(random_generator);
}


/*
 * Генерация случайного вещественного числа в заданном диапазоне.
 *
 * @param min_value минимальное возможное значение.
 * @param max_value максимальное возможное значение.
 * @return возвращает случайное вещественное число.
 */
double getRandomDouble(double const min_value, double const max_value) {
    uniform_real_distribution<double> distribution(min_value, max_value);
    return distribution(random_generator);
}


/*
 * Базовый класс спортивного инвентаря.
 *
 * Хранит общую информацию о спортивном инвентаре:
 * тип, износостойкость, стоимость, состояние и статистику использования.
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
     * Увеличение износа спортивного инвентаря.
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
     * @param type тип спортивного инвентаря.
     * @param wear_resistance износостойкость от 1 до 100.
     * @param cost стоимость спортивного инвентаря.
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
     * Уничтожение объекта спортивного инвентаря.
     *
     * @return ничего не возвращает.
     */
    virtual ~SportEquipment() {
    }


    /*
     * Получение типа спортивного инвентаря.
     *
     * @return возвращает тип спортивного инвентаря.
     */
    string getType() const {
        return _type;
    }


    /*
     * Получение износостойкости спортивного инвентаря.
     *
     * @return возвращает износостойкость от 1 до 100.
     */
    int getWearResistance() const {
        return _wear_resistance;
    }


    /*
     * Получение стоимости спортивного инвентаря.
     *
     * @return возвращает исходную стоимость.
     */
    double getCost() const {
        return _cost;
    }


    /*
     * Получение текущего процента износа.
     *
     * @return возвращает процент износа от 0 до 100.
     */
    double getWear() const {
        return _wear;
    }


    /*
     * Получение текущего состояния инвентаря.
     *
     * Состояние рассчитывается как процент оставшегося ресурса.
     *
     * @return возвращает состояние от 0 до 100 процентов.
     */
    double getCondition() const {
        return 100.0 - _wear;
    }


    /*
     * Получение количества использований инвентаря.
     *
     * @return возвращает количество использований.
     */
    int getUsageCount() const {
        return _usage_count;
    }


    /*
     * Проверка возможности использования инвентаря.
     *
     * @return возвращает true, если предмет не сломан.
     */
    bool isAvailable() const {
        return _wear < 100.0;
    }


    /*
     * Проверка необходимости замены инвентаря.
     *
     * @return возвращает true при износе более 70 процентов.
     */
    bool needsReplacement() const {
        return _wear > REPLACEMENT_WEAR;
    }


    /*
     * Регистрация одного использования инвентаря.
     *
     * @return ничего не возвращает.
     */
    void registerUsage() {
        _usage_count++;
    }


    /*
     * Поломка спортивного инвентаря.
     *
     * Устанавливает износ равным 100 процентам.
     *
     * @return ничего не возвращает.
     */
    void breakEquipment() {
        _wear = 100.0;
    }


    /*
     * Выполнение ремонта спортивного инвентаря.
     *
     * Ремонт устраняет случайно от 50 до 80 процентов
     * текущего износа.
     *
     * @return возвращает стоимость выполненного ремонта.
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


    /*
     * Использование спортивного инвентаря.
     *
     * @param intensity интенсивность упражнения от 1 до 5.
     * @param duration длительность упражнения в минутах.
     * @return возвращает величину полученного износа.
     */
    virtual double use(
        int const intensity,
        int const duration
    ) = 0;


    /*
     * Проверка степени износа спортивного инвентаря.
     *
     * @return возвращает текстовое описание степени износа.
     */
    virtual string checkWear() const = 0;


    /*
     * Получение информации о специфическом параметре предмета.
     *
     * @return возвращает строку с дополнительными параметрами.
     */
    virtual string getSpecificInfo() const = 0;
};


/*
 * Класс спортивного мяча.
 *
 * Наследуется от SportEquipment и дополнительно хранит диаметр мяча.
 */
class Ball : public SportEquipment {
private:
    double _diameter;

public:

    /*
     * Создание спортивного мяча.
     *
     * @param diameter диаметр мяча в сантиметрах.
     * @param wear_resistance износостойкость мяча.
     * @param cost стоимость мяча.
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
     * Использование мяча во время упражнения.
     *
     * @param intensity интенсивность упражнения от 1 до 5.
     * @param duration длительность упражнения в минутах.
     * @return возвращает величину полученного износа.
     */
    double use(
        int const intensity,
        int const duration
    ) override {
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
     * Проверка износа мяча.
     *
     * @return возвращает текстовое описание состояния мяча.
     */
    string checkWear() const override {
        if(getWear() >= 100.0) {
            return "Мяч сломан";
        }

        if(getWear() > REPLACEMENT_WEAR) {
            return "Мяч рекомендуется заменить";
        }

        return "Мяч пригоден к использованию";
    }


    /*
     * Получение информации о диаметре мяча.
     *
     * @return возвращает строку с диаметром мяча.
     */
    string getSpecificInfo() const override {
        return "Диаметр: " + to_string(_diameter) + " см";
    }
};


/*
 * Класс спортивной скакалки.
 *
 * Наследуется от SportEquipment и дополнительно хранит длину скакалки.
 */
class JumpRope : public SportEquipment {
private:
    double _length;

public:

    /*
     * Создание спортивной скакалки.
     *
     * @param length длина скакалки в метрах.
     * @param wear_resistance износостойкость скакалки.
     * @param cost стоимость скакалки.
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
     * Использование скакалки во время упражнения.
     *
     * @param intensity интенсивность упражнения от 1 до 5.
     * @param duration длительность упражнения в минутах.
     * @return возвращает величину полученного износа.
     */
    double use(
        int const intensity,
        int const duration
    ) override {
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
     * Проверка износа скакалки.
     *
     * @return возвращает текстовое описание состояния скакалки.
     */
    string checkWear() const override {
        if(getWear() >= 100.0) {
            return "Скакалка сломана";
        }

        if(getWear() > REPLACEMENT_WEAR) {
            return "Скакалку рекомендуется заменить";
        }

        return "Скакалка пригодна к использованию";
    }


    /*
     * Получение информации о длине скакалки.
     *
     * @return возвращает строку с длиной скакалки.
     */
    string getSpecificInfo() const override {
        return "Длина: " + to_string(_length) + " м";
    }
};


/*
 * Класс спортивных гантелей.
 *
 * Наследуется от SportEquipment и дополнительно хранит вес гантелей.
 */
class Dumbbells : public SportEquipment {
private:
    double _weight;

public:

    /*
     * Создание гантелей.
     *
     * @param weight вес гантелей в килограммах.
     * @param wear_resistance износостойкость гантелей.
     * @param cost стоимость гантелей.
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
     * Использование гантелей во время упражнения.
     *
     * @param intensity интенсивность упражнения от 1 до 5.
     * @param duration длительность упражнения в минутах.
     * @return возвращает величину полученного износа.
     */
    double use(
        int const intensity,
        int const duration
    ) override {
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
     * Проверка износа гантелей.
     *
     * @return возвращает текстовое описание состояния гантелей.
     */
    string checkWear() const override {
        if(getWear() >= 100.0) {
            return "Гантели сломаны";
        }

        if(getWear() > REPLACEMENT_WEAR) {
            return "Гантели рекомендуется заменить";
        }

        return "Гантели пригодны к использованию";
    }


    /*
     * Получение информации о весе гантелей.
     *
     * @return возвращает строку с весом гантелей.
     */
    string getSpecificInfo() const override {
        return "Вес: " + to_string(_weight) + " кг";
    }
};


/*
 * Класс спортивного зала.
 *
 * Содержит спортивный инвентарь, историю его использования
 * и методы проведения тренировок и ремонта.
 */
class Gym {
private:
    vector<unique_ptr<SportEquipment>> _equipment;
    vector<string> _usage_history;
    double _total_repair_cost;

    /*
     * Получение случайного доступного спортивного инвентаря.
     *
     * @return возвращает указатель на доступный предмет
     * или nullptr, если доступного инвентаря нет.
     */
    SportEquipment* getRandomAvailableEquipment() {
        vector<SportEquipment*> available_equipment;

        for(auto& p_item : _equipment) {
            if(p_item->isAvailable()) {
                available_equipment.push_back(p_item.get());
            }
        }

        if(available_equipment.empty()) {
            return nullptr;
        }

        int const index = getRandomInt(
            0,
            static_cast<int>(available_equipment.size()) - 1
        );

        return available_equipment[index];
    }


    /*
     * Проверка возможности внезапной поломки инвентаря.
     *
     * При износе более 80 процентов вероятность внезапной
     * поломки составляет 1 процент.
     *
     * @param p_item указатель на проверяемый инвентарь.
     * @return возвращает true, если произошла внезапная поломка.
     */
    bool checkSuddenBreakdown(SportEquipment* p_item) {
        if(p_item == nullptr) {
            return false;
        }

        if(
            p_item->getWear() > BREAKDOWN_WEAR &&
            getRandomDouble(0.0, 1.0) < SUDDEN_BREAKDOWN_CHANCE
        ) {
            p_item->breakEquipment();
            return true;
        }

        return false;
    }

public:

    /*
     * Создание спортивного зала.
     *
     * Инициализирует пустой список инвентаря,
     * пустую историю и нулевые расходы на ремонт.
     */
    Gym() :
        _total_repair_cost(0.0) {
    }


    /*
     * Добавление спортивного инвентаря в спортзал.
     *
     * @param p_item умный указатель на добавляемый предмет.
     * @return ничего не возвращает.
     */
    void addEquipment(unique_ptr<SportEquipment> p_item) {
        _equipment.push_back(move(p_item));
    }


    /*
     * Проведение одной тренировки.
     *
     * Тренировка содержит случайное количество упражнений
     * от 5 до 15. Для каждого упражнения случайно выбираются
     * инвентарь, интенсивность и длительность.
     *
     * @param show_process определяет необходимость вывода процесса тренировки.
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
            SportEquipment* p_item = getRandomAvailableEquipment();

            if(p_item == nullptr) {
                if(show_process) {
                    cout << "Нет доступного инвентаря.\n";
                }

                break;
            }

            int const intensity = getRandomInt(1, 5);
            int const duration = getRandomInt(1, 10);

            double const old_wear = p_item->getWear();
            double const received_wear =
                p_item->use(intensity, duration);

            string history_record =
                "Инвентарь: " + p_item->getType() +
                ", интенсивность: " + to_string(intensity) +
                ", длительность: " + to_string(duration) +
                " мин., износ: " + to_string(received_wear) + "%";

            _usage_history.push_back(history_record);

            if(show_process) {
                cout << "Упражнение " << i + 1 << ":\n";
                cout << "  Инвентарь: "
                     << p_item->getType() << "\n";
                cout << "  Интенсивность: "
                     << intensity << "\n";
                cout << "  Длительность: "
                     << duration << " мин.\n";
                cout << "  Износ до упражнения: "
                     << fixed << setprecision(2)
                     << old_wear << "%\n";
                cout << "  Полученный износ: "
                     << received_wear << "%\n";
                cout << "  Общий износ: "
                     << p_item->getWear() << "%\n";
            }

            if(p_item->getWear() >= 100.0) {
                if(show_process) {
                    cout << "  Предмет сломался из-за износа!\n";
                }
            }
            else if(checkSuddenBreakdown(p_item)) {
                if(show_process) {
                    cout << "  Произошла внезапная поломка!\n";
                }
            }

            if(show_process) {
                cout << "\n";
            }
        }
    }


    /*
     * Вывод отчета о состоянии спортивного инвентаря.
     *
     * Отображает процент износа, сломанные предметы,
     * стоимость ремонта и рекомендации по замене.
     *
     * @return ничего не возвращает.
     */
    void printReport() const {
        cout << "\nОТЧЕТ\n\n";

        cout << "Состояние инвентаря:\n";

        for(size_t i = 0; i < _equipment.size(); i++) {
            SportEquipment const* p_item = _equipment[i].get();

            cout << i + 1 << ". "
                 << p_item->getType()
                 << " | Износ: "
                 << fixed << setprecision(2)
                 << p_item->getWear()
                 << "% | Состояние: "
                 << p_item->getCondition()
                 << "% | "
                 << p_item->getSpecificInfo()
                 << "\n";
        }

        cout << "\nСломанный инвентарь:\n";

        bool has_broken = false;
        double repair_cost = 0.0;

        for(auto const& p_item : _equipment) {
            if(!p_item->isAvailable()) {
                has_broken = true;
                repair_cost += p_item->getCost() * REPAIR_PERCENT;

                cout << "- " << p_item->getType()
                     << " | Стоимость ремонта: "
                     << p_item->getCost() * REPAIR_PERCENT
                     << " руб.\n";
            }
        }

        if(!has_broken) {
            cout << "Сломанного инвентаря нет.\n";
        }

        cout << "\nПредварительная стоимость ремонта: "
             << repair_cost << " руб.\n";

        cout << "\nРекомендации по замене:\n";

        bool has_recommendations = false;

        for(auto const& p_item : _equipment) {
            if(p_item->needsReplacement()) {
                has_recommendations = true;

                cout << "- " << p_item->getType()
                     << ": износ "
                     << p_item->getWear()
                     << "%.\n";
            }
        }

        if(!has_recommendations) {
            cout << "Замена инвентаря пока не требуется.\n";
        }
    }


    /*
     * Выполнение ремонта поврежденного инвентаря.
     *
     * Ремонтируются предметы с износом более 70 процентов.
     * Ремонт восстанавливает от 50 до 80 процентов износа.
     *
     * @return ничего не возвращает.
     */
    void repairEquipment() {
        cout << "\nРЕМОНТ\n";

        bool has_repairs = false;

        for(auto& p_item : _equipment) {
            if(p_item->getWear() > REPLACEMENT_WEAR) {
                has_repairs = true;

                double const old_wear = p_item->getWear();
                double const repair_cost = p_item->repair();

                _total_repair_cost += repair_cost;

                cout << p_item->getType()
                     << ": износ до ремонта "
                     << fixed << setprecision(2)
                     << old_wear
                     << "%, после ремонта "
                     << p_item->getWear()
                     << "%, стоимость ремонта "
                     << repair_cost
                     << " руб.\n";
            }
        }

        if(!has_repairs) {
            cout << "Инвентарь не требует ремонта.\n";
        }
    }


    /*
     * Вывод истории использования спортивного инвентаря.
     *
     * @return ничего не возвращает.
     */
    void printUsageHistory() const {
        cout << "\nИСТОРИЯ ИСПОЛЬЗОВАНИЯ\n";

        if(_usage_history.empty()) {
            cout << "История использования пуста.\n";
            return;
        }

        for(size_t i = 0; i < _usage_history.size(); i++) {
            cout << i + 1 << ". "
                 << _usage_history[i] << "\n";
        }
    }


    /*
     * Проведение симуляции ста тренировок.
     *
     * После каждой тренировки выполняется ремонт сильно
     * изношенного инвентаря без промежуточного вывода.
     *
     * @return ничего не возвращает.
     */
    void simulateHundredWorkouts() {
        const int WORKOUT_COUNT = 100;

        cout << "\nСИМУЛЯЦИЯ 100 ТРЕНИРОВОК\n";

        for(int i = 0; i < WORKOUT_COUNT; i++) {
            conductWorkout(false);

            for(auto& p_item : _equipment) {
                if(p_item->getWear() > REPLACEMENT_WEAR) {
                    double const repair_cost = p_item->repair();
                    _total_repair_cost += repair_cost;
                }
            }
        }

        cout << "Симуляция завершена.\n";
    }


    /*
     * Вывод статистики использования спортивного инвентаря.
     *
     * Определяет наиболее часто используемый предмет
     * и выводит количество использований каждого предмета.
     *
     * @return ничего не возвращает.
     */
    void printUsageStatistics() const {
        cout << "\nСТАТИСТИКА\n";

        if(_equipment.empty()) {
            cout << "В спортзале нет инвентаря.\n";
            return;
        }

        SportEquipment const* p_most_used = _equipment[0].get();

        for(auto const& p_item : _equipment) {
            cout << p_item->getType()
                 << ": использований — "
                 << p_item->getUsageCount()
                 << ", текущий износ — "
                 << fixed << setprecision(2)
                 << p_item->getWear()
                 << "%\n";

            if(
                p_item->getUsageCount() >
                p_most_used->getUsageCount()
            ) {
                p_most_used = p_item.get();
            }
        }

        cout << "\nСамый используемый предмет: "
             << p_most_used->getType()
             << " ("
             << p_most_used->getUsageCount()
             << " использований).\n";

        cout << "Общие расходы на ремонт: "
             << _total_repair_cost
             << " руб.\n";
    }


    /*
     * Вывод плана замены спортивного инвентаря.
     *
     * Предметы распределяются по степени необходимости замены.
     *
     * @return ничего не возвращает.
     */
    void printReplacementPlan() const {
        cout << "\nПЛАН ЗАМЕНЫ\n";

        for(auto const& p_item : _equipment) {
            cout << p_item->getType()
                 << " | Износ: "
                 << fixed << setprecision(2)
                 << p_item->getWear()
                 << "% | ";

            if(p_item->getWear() >= 100.0) {
                cout << "требуется немедленная замена";
            }
            else if(p_item->getWear() > 80.0) {
                cout << "высокий приоритет замены";
            }
            else if(p_item->getWear() > 70.0) {
                cout << "рекомендуется запланировать замену";
            }
            else if(p_item->getWear() > 50.0) {
                cout << "необходимо наблюдение за состоянием";
            }
            else {
                cout << "замена пока не требуется";
            }

            cout << "\n";
        }
    }
};


/*
 * Главная функция программы.
 *
 * Создает спортзал, добавляет спортивный инвентарь,
 * проводит тренировку, ремонт и симуляцию 100 тренировок.
 *
 * @return возвращает код завершения программы.
 */
int main() {
    setlocale(LC_ALL, "Russian");

    Gym gym;

    gym.addEquipment(
        make_unique<Ball>(22.0, 75, 2500.0)
    );

    gym.addEquipment(
        make_unique<Ball>(24.0, 80, 3200.0)
    );

    gym.addEquipment(
        make_unique<JumpRope>(2.8, 60, 1200.0)
    );

    gym.addEquipment(
        make_unique<JumpRope>(3.0, 70, 1500.0)
    );

    gym.addEquipment(
        make_unique<Dumbbells>(5.0, 90, 5000.0)
    );

    gym.addEquipment(
        make_unique<Dumbbells>(10.0, 95, 8000.0)
    );

    cout << "СПОРТИВНЫЙ ЗАЛ\n";

    gym.conductWorkout();
    gym.printReport();
    gym.printUsageHistory();
    gym.repairEquipment();
    gym.simulateHundredWorkouts();
    gym.printReport();
    gym.printUsageStatistics();
    gym.printReplacementPlan();

    return 0;
}