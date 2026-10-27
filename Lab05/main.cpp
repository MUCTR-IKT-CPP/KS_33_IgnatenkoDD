#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <iomanip>

using namespace std;

const double MIN_RANDOM_COEFFICIENT = 0.5;
const double MAX_RANDOM_COEFFICIENT = 2.0;
const double REPLACEMENT_WEAR = 70.0;
const double BREAKDOWN_WEAR = 80.0;
const double SUDDEN_BREAKDOWN_CHANCE = 0.01;
const double REPAIR_PERCENT = 0.2;


/*
 * Генерация случайного целого числа.
 *
 * @param min_value минимальное значение.
 * @param max_value максимальное значение.
 * @return возвращает случайное целое число.
 */
int getRandomInt(int const min_value, int const max_value) {
    static random_device random_device_generator;
    static mt19937 random_generator(random_device_generator());

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
double getRandomDouble(
    double const min_value,
    double const max_value
) {
    static random_device random_device_generator;
    static mt19937 random_generator(random_device_generator());

    uniform_real_distribution<double> distribution(
        min_value,
        max_value
    );

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
     * Увеличение износа инвентаря.
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
     * @param wear_resistance износостойкость от 1 до 100.
     * @param cost стоимость инвентаря.
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
     * @return возвращает стоимость инвентаря.
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
     * Увеличение количества использований.
     *
     * @return ничего не возвращает.
     */
    void registerUsage() {
        _usage_count++;
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
     * @return возвращает true при износе более 70 процентов.
     */
    bool needsReplacement() const {
        return _wear > REPLACEMENT_WEAR;
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

        double const recovery_percent =
            getRandomDouble(0.5, 0.8);

        _wear -= _wear * recovery_percent;

        if(_wear < 0.0) {
            _wear = 0.0;
        }

        double const damage_coefficient =
            old_wear / 100.0;

        double const repair_cost =
            _cost *
            REPAIR_PERCENT *
            damage_coefficient;

        return repair_cost;
    }
};


/*
 * Класс спортивного мяча.
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
     * Использование мяча.
     *
     * @param intensity интенсивность от 1 до 5.
     * @param duration длительность в минутах.
     * @return возвращает полученный износ.
     */
    double use(
        int const intensity,
        int const duration
    ) {
        double const base_wear_rate = 0.06;

        double const random_coefficient =
            getRandomDouble(
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
     * Проверка состояния мяча.
     *
     * @return возвращает описание состояния мяча.
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


    /*
     * Получение диаметра мяча.
     *
     * @return возвращает диаметр мяча.
     */
    double getDiameter() const {
        return _diameter;
    }
};


/*
 * Класс спортивной скакалки.
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
     * @param cost стоимость скакалки.
     */
    JumpRope(
        double const length,
        int const wear_resistance,
        double const cost
    ) :
        SportEquipment(
            "Скакалка",
            wear_resistance,
            cost
        ),
        _length(length) {
    }


    /*
     * Использование скакалки.
     *
     * @param intensity интенсивность от 1 до 5.
     * @param duration длительность в минутах.
     * @return возвращает полученный износ.
     */
    double use(
        int const intensity,
        int const duration
    ) {
        double const base_wear_rate = 0.09;

        double const random_coefficient =
            getRandomDouble(
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
     * Проверка состояния скакалки.
     *
     * @return возвращает описание состояния скакалки.
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


    /*
     * Получение длины скакалки.
     *
     * @return возвращает длину скакалки.
     */
    double getLength() const {
        return _length;
    }
};


/*
 * Класс спортивных гантелей.
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
     * @param cost стоимость гантелей.
     */
    Dumbbells(
        double const weight,
        int const wear_resistance,
        double const cost
    ) :
        SportEquipment(
            "Гантели",
            wear_resistance,
            cost
        ),
        _weight(weight) {
    }


    /*
     * Использование гантелей.
     *
     * @param intensity интенсивность от 1 до 5.
     * @param duration длительность в минутах.
     * @return возвращает полученный износ.
     */
    double use(
        int const intensity,
        int const duration
    ) {
        double const base_wear_rate = 0.04;

        double const random_coefficient =
            getRandomDouble(
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
     * Проверка состояния гантелей.
     *
     * @return возвращает описание состояния гантелей.
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


    /*
     * Получение веса гантелей.
     *
     * @return возвращает вес гантелей.
     */
    double getWeight() const {
        return _weight;
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
     * Проверка внезапной поломки мяча.
     *
     * @param ball проверяемый мяч.
     * @return возвращает true при поломке.
     */
    bool checkSuddenBreakdown(Ball& ball) {
        if(
            ball.getWear() > BREAKDOWN_WEAR &&
            getRandomDouble(0.0, 1.0) <
            SUDDEN_BREAKDOWN_CHANCE
        ) {
            ball.breakEquipment();

            return true;
        }

        return false;
    }


    /*
     * Проверка внезапной поломки скакалки.
     *
     * @param jump_rope проверяемая скакалка.
     * @return возвращает true при поломке.
     */
    bool checkSuddenBreakdown(JumpRope& jump_rope) {
        if(
            jump_rope.getWear() > BREAKDOWN_WEAR &&
            getRandomDouble(0.0, 1.0) <
            SUDDEN_BREAKDOWN_CHANCE
        ) {
            jump_rope.breakEquipment();

            return true;
        }

        return false;
    }


    /*
     * Проверка внезапной поломки гантелей.
     *
     * @param dumbbells проверяемые гантели.
     * @return возвращает true при поломке.
     */
    bool checkSuddenBreakdown(Dumbbells& dumbbells) {
        if(
            dumbbells.getWear() > BREAKDOWN_WEAR &&
            getRandomDouble(0.0, 1.0) <
            SUDDEN_BREAKDOWN_CHANCE
        ) {
            dumbbells.breakEquipment();

            return true;
        }

        return false;
    }


    /*
     * Проверка наличия доступного инвентаря.
     *
     * @return возвращает true, если есть исправный предмет.
     */
    bool hasAvailableEquipment() const {
        for(Ball const& ball : _balls) {
            if(ball.isAvailable()) {
                return true;
            }
        }

        for(JumpRope const& jump_rope : _jump_ropes) {
            if(jump_rope.isAvailable()) {
                return true;
            }
        }

        for(Dumbbells const& dumbbells : _dumbbells) {
            if(dumbbells.isAvailable()) {
                return true;
            }
        }

        return false;
    }


    /*
     * Проведение упражнения с мячом.
     *
     * @param intensity интенсивность упражнения.
     * @param duration длительность упражнения.
     * @param show_process необходимость вывода информации.
     * @return возвращает true, если упражнение выполнено.
     */
    bool useRandomBall(
        int const intensity,
        int const duration,
        bool const show_process
    ) {
        vector<int> available_indexes;

        for(size_t i = 0; i < _balls.size(); i++) {
            if(_balls[i].isAvailable()) {
                available_indexes.push_back(
                    static_cast<int>(i)
                );
            }
        }

        if(available_indexes.empty()) {
            return false;
        }

        int const random_index = getRandomInt(
            0,
            static_cast<int>(
                available_indexes.size()
            ) - 1
        );

        int const ball_index =
            available_indexes[random_index];

        Ball& ball = _balls[ball_index];

        double const received_wear =
            ball.use(intensity, duration);

        string const history_record =
            "Мяч, интенсивность: " +
            to_string(intensity) +
            ", длительность: " +
            to_string(duration) +
            " мин.";

        _usage_history.push_back(history_record);

        if(show_process) {
            cout << "Инвентарь: Мяч\n";
            cout << "Интенсивность: "
                 << intensity << "\n";
            cout << "Длительность: "
                 << duration << " мин.\n";
            cout << "Полученный износ: "
                 << received_wear << "%\n";
            cout << "Общий износ: "
                 << ball.getWear() << "%\n";
        }

        if(ball.getWear() < 100.0) {
            if(checkSuddenBreakdown(ball)) {
                if(show_process) {
                    cout << "Внезапная поломка!\n";
                }
            }
        }

        return true;
    }


    /*
     * Проведение упражнения со скакалкой.
     *
     * @param intensity интенсивность упражнения.
     * @param duration длительность упражнения.
     * @param show_process необходимость вывода информации.
     * @return возвращает true, если упражнение выполнено.
     */
    bool useRandomJumpRope(
        int const intensity,
        int const duration,
        bool const show_process
    ) {
        vector<int> available_indexes;

        for(size_t i = 0; i < _jump_ropes.size(); i++) {
            if(_jump_ropes[i].isAvailable()) {
                available_indexes.push_back(
                    static_cast<int>(i)
                );
            }
        }

        if(available_indexes.empty()) {
            return false;
        }

        int const random_index = getRandomInt(
            0,
            static_cast<int>(
                available_indexes.size()
            ) - 1
        );

        int const jump_rope_index =
            available_indexes[random_index];

        JumpRope& jump_rope =
            _jump_ropes[jump_rope_index];

        double const received_wear =
            jump_rope.use(intensity, duration);

        string const history_record =
            "Скакалка, интенсивность: " +
            to_string(intensity) +
            ", длительность: " +
            to_string(duration) +
            " мин.";

        _usage_history.push_back(history_record);

        if(show_process) {
            cout << "Инвентарь: Скакалка\n";
            cout << "Интенсивность: "
                 << intensity << "\n";
            cout << "Длительность: "
                 << duration << " мин.\n";
            cout << "Полученный износ: "
                 << received_wear << "%\n";
            cout << "Общий износ: "
                 << jump_rope.getWear() << "%\n";
        }

        if(jump_rope.getWear() < 100.0) {
            if(checkSuddenBreakdown(jump_rope)) {
                if(show_process) {
                    cout << "Внезапная поломка!\n";
                }
            }
        }

        return true;
    }


    /*
     * Проведение упражнения с гантелями.
     *
     * @param intensity интенсивность упражнения.
     * @param duration длительность упражнения.
     * @param show_process необходимость вывода информации.
     * @return возвращает true, если упражнение выполнено.
     */
    bool useRandomDumbbells(
        int const intensity,
        int const duration,
        bool const show_process
    ) {
        vector<int> available_indexes;

        for(size_t i = 0; i < _dumbbells.size(); i++) {
            if(_dumbbells[i].isAvailable()) {
                available_indexes.push_back(
                    static_cast<int>(i)
                );
            }
        }

        if(available_indexes.empty()) {
            return false;
        }

        int const random_index = getRandomInt(
            0,
            static_cast<int>(
                available_indexes.size()
            ) - 1
        );

        int const dumbbells_index =
            available_indexes[random_index];

        Dumbbells& dumbbells =
            _dumbbells[dumbbells_index];

        double const received_wear =
            dumbbells.use(intensity, duration);

        string const history_record =
            "Гантели, интенсивность: " +
            to_string(intensity) +
            ", длительность: " +
            to_string(duration) +
            " мин.";

        _usage_history.push_back(history_record);

        if(show_process) {
            cout << "Инвентарь: Гантели\n";
            cout << "Интенсивность: "
                 << intensity << "\n";
            cout << "Длительность: "
                 << duration << " мин.\n";
            cout << "Полученный износ: "
                 << received_wear << "%\n";
            cout << "Общий износ: "
                 << dumbbells.getWear() << "%\n";
        }

        if(dumbbells.getWear() < 100.0) {
            if(checkSuddenBreakdown(dumbbells)) {
                if(show_process) {
                    cout << "Внезапная поломка!\n";
                }
            }
        }

        return true;
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
     * @param show_process необходимость вывода процесса.
     * @return ничего не возвращает.
     */
    void conductWorkout(bool const show_process = true) {
        int const exercise_count =
            getRandomInt(5, 15);

        if(show_process) {
            cout << "\nТРЕНИРОВКА\n";

            cout << "Количество упражнений: "
                 << exercise_count << "\n\n";
        }

        for(int i = 0; i < exercise_count; i++) {
            if(!hasAvailableEquipment()) {
                if(show_process) {
                    cout << "Доступного инвентаря нет.\n";
                }

                break;
            }

            int const intensity =
                getRandomInt(1, 5);

            int const duration =
                getRandomInt(1, 10);

            bool exercise_completed = false;

            while(!exercise_completed) {
                int const equipment_type =
                    getRandomInt(1, 3);

                if(show_process) {
                    cout << "Упражнение "
                         << i + 1 << ":\n";
                }

                if(equipment_type == 1) {
                    exercise_completed =
                        useRandomBall(
                            intensity,
                            duration,
                            show_process
                        );
                }
                else if(equipment_type == 2) {
                    exercise_completed =
                        useRandomJumpRope(
                            intensity,
                            duration,
                            show_process
                        );
                }
                else {
                    exercise_completed =
                        useRandomDumbbells(
                            intensity,
                            duration,
                            show_process
                        );
                }
            }

            if(show_process) {
                cout << "\n";
            }
        }
    }


    /*
     * Вывод состояния всего инвентаря.
     *
     * @return ничего не возвращает.
     */
    void printReport() const {
        cout << "\nОТЧЕТ\n";

        cout << "\nМячи:\n";

        for(size_t i = 0; i < _balls.size(); i++) {
            cout << i + 1
                 << ". Диаметр: "
                 << _balls[i].getDiameter()
                 << " см | Износ: "
                 << fixed << setprecision(2)
                 << _balls[i].getWear()
                 << "% | "
                 << _balls[i].checkWear()
                 << "\n";
        }

        cout << "\nСкакалки:\n";

        for(size_t i = 0; i < _jump_ropes.size(); i++) {
            cout << i + 1
                 << ". Длина: "
                 << _jump_ropes[i].getLength()
                 << " м | Износ: "
                 << _jump_ropes[i].getWear()
                 << "% | "
                 << _jump_ropes[i].checkWear()
                 << "\n";
        }

        cout << "\nГантели:\n";

        for(size_t i = 0; i < _dumbbells.size(); i++) {
            cout << i + 1
                 << ". Вес: "
                 << _dumbbells[i].getWeight()
                 << " кг | Износ: "
                 << _dumbbells[i].getWear()
                 << "% | "
                 << _dumbbells[i].checkWear()
                 << "\n";
        }

        double repair_cost = 0.0;

        for(Ball const& ball : _balls) {
            if(!ball.isAvailable()) {
                repair_cost +=
                    ball.getCost() *
                    REPAIR_PERCENT;
            }
        }

        for(JumpRope const& jump_rope : _jump_ropes) {
            if(!jump_rope.isAvailable()) {
                repair_cost +=
                    jump_rope.getCost() *
                    REPAIR_PERCENT;
            }
        }

        for(Dumbbells const& dumbbells : _dumbbells) {
            if(!dumbbells.isAvailable()) {
                repair_cost +=
                    dumbbells.getCost() *
                    REPAIR_PERCENT;
            }
        }

        cout << "\nСтоимость ремонта сломанных предметов: "
             << repair_cost << " руб.\n";

        cout << "\nРекомендации по замене:\n";

        for(Ball const& ball : _balls) {
            if(ball.needsReplacement()) {
                cout << "- Мяч с износом "
                     << ball.getWear()
                     << "%\n";
            }
        }

        for(JumpRope const& jump_rope : _jump_ropes) {
            if(jump_rope.needsReplacement()) {
                cout << "- Скакалка с износом "
                     << jump_rope.getWear()
                     << "%\n";
            }
        }

        for(Dumbbells const& dumbbells : _dumbbells) {
            if(dumbbells.needsReplacement()) {
                cout << "- Гантели с износом "
                     << dumbbells.getWear()
                     << "%\n";
            }
        }
    }


    /*
     * Ремонт сильно изношенного инвентаря.
     *
     * @return ничего не возвращает.
     */
    void repairEquipment() {
        for(Ball& ball : _balls) {
            if(ball.getWear() > REPLACEMENT_WEAR) {
                _total_repair_cost +=
                    ball.repair();
            }
        }

        for(JumpRope& jump_rope : _jump_ropes) {
            if(
                jump_rope.getWear() >
                REPLACEMENT_WEAR
            ) {
                _total_repair_cost +=
                    jump_rope.repair();
            }
        }

        for(Dumbbells& dumbbells : _dumbbells) {
            if(
                dumbbells.getWear() >
                REPLACEMENT_WEAR
            ) {
                _total_repair_cost +=
                    dumbbells.repair();
            }
        }
    }


    /*
     * Вывод истории использования.
     *
     * @return ничего не возвращает.
     */
    void printUsageHistory() const {
        cout << "\nИСТОРИЯ\n";

        for(
            size_t i = 0;
            i < _usage_history.size();
            i++
        ) {
            cout << i + 1
                 << ". "
                 << _usage_history[i]
                 << "\n";
        }
    }


    /*
     * Симуляция ста тренировок.
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
     * Вывод статистики использования.
     *
     * @return ничего не возвращает.
     */
    void printUsageStatistics() const {
        cout << "\nСТАТИСТИКА\n";

        int max_usage_count = -1;
        string most_used_item = "Нет";

        for(Ball const& ball : _balls) {
            cout << "Мяч: "
                 << ball.getUsageCount()
                 << " использований\n";

            if(ball.getUsageCount() > max_usage_count) {
                max_usage_count =
                    ball.getUsageCount();

                most_used_item = "Мяч";
            }
        }

        for(JumpRope const& jump_rope : _jump_ropes) {
            cout << "Скакалка: "
                 << jump_rope.getUsageCount()
                 << " использований\n";

            if(
                jump_rope.getUsageCount() >
                max_usage_count
            ) {
                max_usage_count =
                    jump_rope.getUsageCount();

                most_used_item = "Скакалка";
            }
        }

        for(Dumbbells const& dumbbells : _dumbbells) {
            cout << "Гантели: "
                 << dumbbells.getUsageCount()
                 << " использований\n";

            if(
                dumbbells.getUsageCount() >
                max_usage_count
            ) {
                max_usage_count =
                    dumbbells.getUsageCount();

                most_used_item = "Гантели";
            }
        }

        cout << "\nСамый используемый предмет: "
             << most_used_item << "\n";

        cout << "Количество использований: "
             << max_usage_count << "\n";

        cout << "Общие расходы на ремонт: "
             << fixed << setprecision(2)
             << _total_repair_cost
             << " руб.\n";
    }


    /*
     * Вывод плана замены инвентаря.
     *
     * @return ничего не возвращает.
     */
    void printReplacementPlan() const {
        cout << "\nПЛАН ЗАМЕНЫ\n";

        bool replacement_needed = false;

        for(Ball const& ball : _balls) {
            if(ball.getWear() > REPLACEMENT_WEAR) {
                replacement_needed = true;

                cout << "Мяч: рекомендуется замена, износ "
                     << ball.getWear()
                     << "%\n";
            }
        }

        for(JumpRope const& jump_rope : _jump_ropes) {
            if(
                jump_rope.getWear() >
                REPLACEMENT_WEAR
            ) {
                replacement_needed = true;

                cout << "Скакалка: рекомендуется замена, износ "
                     << jump_rope.getWear()
                     << "%\n";
            }
        }

        for(Dumbbells const& dumbbells : _dumbbells) {
            if(
                dumbbells.getWear() >
                REPLACEMENT_WEAR
            ) {
                replacement_needed = true;

                cout << "Гантели: рекомендуется замена, износ "
                     << dumbbells.getWear()
                     << "%\n";
            }
        }

        if(!replacement_needed) {
            cout << "Замена инвентаря пока не требуется.\n";
        }
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

    gym.printUsageStatistics();

    gym.printReplacementPlan();

    cout << "\nНажмите Enter для завершения программы...";
    cin.get();

    return 0;
}
