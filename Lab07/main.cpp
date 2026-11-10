
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <utility>

/**
 * Интерфейс метрики.
 */
class IMetric{
public:
    /**
     * Получение имени метрики.
     *
     * @return имя метрики.
     */
    virtual std::string name() const = 0;

    /**
     * Получение числового значения метрики.
     *
     * @return значение метрики.
     */
    virtual double value() const = 0;

    /**
     * Виртуальный деструктор интерфейса.
     */
    virtual ~IMetric() = default;
};

/**
 * Единица измерения в секундах.
 */
class Seconds{
public:
    /**
     * Получение названия единицы измерения.
     *
     * @return обозначение секунд.
     */
    static std::string symbol(){
        return "s";
    }
};

/**
 * Единица измерения в миллисекундах.
 */
class Milliseconds{
public:
    /**
     * Получение названия единицы измерения.
     *
     * @return обозначение миллисекунд.
     */
    static std::string symbol(){
        return "ms";
    }
};

/**
 * Шаблонный класс метрики.
 *
 * @tparam T тип числового значения.
 * @tparam Unit тип единицы измерения.
 */
template<typename T, typename Unit>
class Metric : public IMetric{
private:
    T _val;
    std::string _metric_name;

public:
    /**
     * Конструктор метрики.
     *
     * @param val числовое значение.
     * @param metric_name имя метрики.
     */
    Metric(T const val, std::string const& metric_name)
        : _val(val), _metric_name(metric_name){}

    /**
     * Получение имени метрики с единицей измерения.
     *
     * @return имя метрики.
     */
    std::string name() const override{
        return _metric_name + "_" + Unit::symbol();
    }

    /**
     * Получение значения метрики.
     *
     * @return числовое значение.
     */
    double value() const override{
        return static_cast<double>(_val);
    }
};

/**
 * Класс для хранения и формирования отчета метрик.
 */
class Telemetry{
private:
    std::vector<std::unique_ptr<IMetric>> _metrics;

public:
    /**
     * Добавление метрики в коллекцию.
     *
     * @param p_metric указатель на метрику.
     */
    void add(std::unique_ptr<IMetric> p_metric){
        if(p_metric){
            _metrics.push_back(std::move(p_metric));
        }
    }

    /**
     * Формирование общего отчета метрик.
     *
     * @return строка с именами и значениями метрик.
     */
    std::string report() const{
        std::ostringstream result;

        for(const auto& p_metric : _metrics){
            result << p_metric->name()
                   << "="
                   << p_metric->value()
                   << "\n";
        }

        return result.str();
    }
};

/**
 * Политика отправки отчета без сжатия.
 */
class NoCompression{
public:
    /**
     * Вывод полного отчета.
     *
     * @param report строка отчета.
     */
    static void send(std::string const& report){
        std::cout << "Report without compression:\n";
        std::cout << report;
    }
};

/**
 * Политика отправки отчета с простым сжатием.
 */
class SimpleCompression{
public:
    /**
     * Вывод расчетной длины сжатого отчета.
     *
     * @param report строка отчета.
     */
    static void send(std::string const& report){
        const std::size_t COMPRESSED_SIZE = report.size() / 2;

        std::cout << "Compressed report size: "
                  << COMPRESSED_SIZE
                  << "\n";
    }
};

/**
 * Шаблонный класс отправки телеметрии.
 *
 * @tparam Policy политика отправки отчета.
 */
template<typename Policy>
class TelemetrySender{
public:
    /**
     * Отправка отчета выбранной политикой.
     *
     * @param telemetry объект с метриками.
     */
    void send(Telemetry const& telemetry) const{
        Policy::send(telemetry.report());
    }
};

/**
 * Главная функция программы.
 *
 * @return код завершения программы.
 */
int main(){
    Telemetry telemetry;

    telemetry.add(
        std::make_unique<Metric<double, Seconds>>(
            2.5, "response_time"
        )
    );

    telemetry.add(
        std::make_unique<Metric<int, Milliseconds>>(
            150, "request_time"
        )
    );

    telemetry.add(
        std::make_unique<Metric<double, Seconds>>(
            10.75, "execution_time"
        )
    );

    TelemetrySender<NoCompression> normal_sender;
    TelemetrySender<SimpleCompression> compressed_sender;

    normal_sender.send(telemetry);

    std::cout << "\n";

    compressed_sender.send(telemetry);

    return 0;
}
