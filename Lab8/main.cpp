
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdint>
#include <utility>

/**
 * Интерфейс тарифного плана.
 */
class IPlan {
public:
    /**
     * Получение идентификатора тарифа.
     *
     * @return идентификатор тарифа.
     */
    virtual int getId() const = 0;

    /**
     * Получение названия тарифа.
     *
     * @return название тарифа.
     */
    virtual std::string getName() const = 0;

    /**
     * Получение месячной стоимости.
     *
     * @return стоимость в копейках.
     */
    virtual int64_t getMonthlyPrice() const = 0;

    /**
     * Виртуальный деструктор.
     */
    virtual ~IPlan() = default;
};

/**
 * Реализация тарифного плана.
 */
class Plan : public IPlan {
private:
    int _id;
    std::string _name;
    int64_t _monthly_price;

public:
    /**
     * Создание тарифного плана.
     *
     * @param id идентификатор тарифа.
     * @param name название тарифа.
     * @param monthly_price стоимость в копейках.
     */
    Plan(int id, const std::string& name, int64_t monthly_price)
        : _id(id),
          _name(name),
          _monthly_price(monthly_price) {}

    /**
     * Получение идентификатора тарифа.
     *
     * @return идентификатор тарифа.
     */
    int getId() const override {
        return _id;
    }

    /**
     * Получение названия тарифа.
     *
     * @return название тарифа.
     */
    std::string getName() const override {
        return _name;
    }

    /**
     * Получение стоимости тарифа.
     *
     * @return стоимость в копейках.
     */
    int64_t getMonthlyPrice() const override {
        return _monthly_price;
    }
};

/**
 * Интерфейс подписки.
 */
class ISubscription {
public:
    /**
     * Получение идентификатора подписки.
     *
     * @return идентификатор подписки.
     */
    virtual int getId() const = 0;

    /**
     * Получение идентификатора пользователя.
     *
     * @return идентификатор пользователя.
     */
    virtual int getUserId() const = 0;

    /**
     * Получение идентификатора тарифа.
     *
     * @return идентификатор тарифа.
     */
    virtual int getPlanId() const = 0;

    /**
     * Проверка активности подписки.
     *
     * @return true, если подписка активна.
     */
    virtual bool isActive() const = 0;

    /**
     * Изменение идентификатора тарифа.
     *
     * @param plan_id новый идентификатор тарифа.
     */
    virtual void setPlanId(int plan_id) = 0;

    /**
     * Отмена подписки.
     */
    virtual void cancel() = 0;

    /**
     * Виртуальный деструктор.
     */
    virtual ~ISubscription() = default;
};

/**
 * Реализация подписки пользователя.
 */
class Subscription : public ISubscription {
private:
    int _id;
    int _user_id;
    int _plan_id;
    bool _is_active;

public:
    /**
     * Создание подписки.
     *
     * @param id идентификатор подписки.
     * @param user_id идентификатор пользователя.
     * @param plan_id идентификатор тарифа.
     */
    Subscription(int id, int user_id, int plan_id)
        : _id(id),
          _user_id(user_id),
          _plan_id(plan_id),
          _is_active(true) {}

    /**
     * Получение идентификатора подписки.
     *
     * @return идентификатор подписки.
     */
    int getId() const override {
        return _id;
    }

    /**
     * Получение идентификатора пользователя.
     *
     * @return идентификатор пользователя.
     */
    int getUserId() const override {
        return _user_id;
    }

    /**
     * Получение идентификатора тарифа.
     *
     * @return идентификатор тарифа.
     */
    int getPlanId() const override {
        return _plan_id;
    }

    /**
     * Проверка активности подписки.
     *
     * @return true, если подписка активна.
     */
    bool isActive() const override {
        return _is_active;
    }

    /**
     * Изменение тарифа подписки.
     *
     * @param plan_id идентификатор нового тарифа.
     */
    void setPlanId(int plan_id) override {
        _plan_id = plan_id;
    }

    /**
     * Отмена подписки без удаления объекта.
     */
    void cancel() override {
        _is_active = false;
    }
};

/**
 * Интерфейс репозитория подписок.
 */
class ISubscriptionRepo {
public:
    /**
     * Добавление новой подписки.
     *
     * @param p_subscription объект подписки.
     * @return true при успешном добавлении.
     */
    virtual bool add(
        std::unique_ptr<ISubscription> p_subscription
    ) = 0;

    /**
     * Отмена подписки по идентификатору.
     *
     * @param subscription_id идентификатор подписки.
     * @return true при успешной отмене.
     */
    virtual bool cancel(int subscription_id) = 0;

    /**
     * Поиск подписки по идентификатору.
     *
     * @param subscription_id идентификатор подписки.
     * @return указатель на подписку или nullptr.
     */
    virtual ISubscription* findById(int subscription_id) = 0;

    /**
     * Поиск всех подписок пользователя.
     *
     * @param user_id идентификатор пользователя.
     * @return список подписок, включая отменённые.
     */
    virtual std::vector<const ISubscription*> findByUser(
        int user_id
    ) const = 0;

    /**
     * Виртуальный деструктор.
     */
    virtual ~ISubscriptionRepo() = default;
};

/**
 * Репозиторий подписок в оперативной памяти.
 */
class SubscriptionRepoInMemory : public ISubscriptionRepo {
private:
    std::vector<std::unique_ptr<ISubscription>> _subscriptions;

    std::unordered_map<int, std::vector<ISubscription*>>
        _subscriptions_by_user;

    std::unordered_map<int, ISubscription*>
        _subscriptions_by_id;

public:
    /**
     * Добавление подписки в репозиторий.
     *
     * @param p_subscription новая подписка.
     * @return true, если идентификатор свободен.
     */
    bool add(
        std::unique_ptr<ISubscription> p_subscription
    ) override {
        if(p_subscription == nullptr) {
            return false;
        }

        const int ID = p_subscription->getId();
        const int USER_ID = p_subscription->getUserId();

        if(_subscriptions_by_id.count(ID) != 0) {
            return false;
        }

        ISubscription* p_saved = p_subscription.get();

        _subscriptions.push_back(std::move(p_subscription));
        _subscriptions_by_id[ID] = p_saved;
        _subscriptions_by_user[USER_ID].push_back(p_saved);

        return true;
    }

    /**
     * Отмена подписки без физического удаления.
     *
     * @param subscription_id идентификатор подписки.
     * @return true, если подписка найдена.
     */
    bool cancel(int subscription_id) override {
        ISubscription* p_subscription = findById(subscription_id);

        if(p_subscription == nullptr) {
            return false;
        }

        p_subscription->cancel();
        return true;
    }

    /**
     * Поиск подписки по идентификатору.
     *
     * @param subscription_id идентификатор подписки.
     * @return указатель на найденную подписку.
     */
    ISubscription* findById(int subscription_id) override {
        auto it = _subscriptions_by_id.find(subscription_id);

        if(it == _subscriptions_by_id.end()) {
            return nullptr;
        }

        return it->second;
    }

    /**
     * Получение всех подписок пользователя.
     *
     * @param user_id идентификатор пользователя.
     * @return список активных и отменённых подписок.
     */
    std::vector<const ISubscription*> findByUser(
        int user_id
    ) const override {
        std::vector<const ISubscription*> result;

        auto it = _subscriptions_by_user.find(user_id);

        if(it == _subscriptions_by_user.end()) {
            return result;
        }

        for(const ISubscription* p_subscription : it->second) {
            result.push_back(p_subscription);
        }

        return result;
    }
};

/**
 * Интерфейс платёжного шлюза.
 */
class IPaymentGateway {
public:
    /**
     * Списание денежных средств.
     *
     * @param user_id идентификатор пользователя.
     * @param amount сумма списания в копейках.
     * @return true при успешном списании.
     */
    virtual bool charge(int user_id, int64_t amount) = 0;

    /**
     * Виртуальный деструктор.
     */
    virtual ~IPaymentGateway() = default;
};

/**
 * Тестовая реализация платёжного шлюза.
 */
class PaymentGatewayInMemory : public IPaymentGateway {
private:
    std::unordered_map<int, int64_t> _balances;

public:
    /**
     * Установка баланса пользователя.
     *
     * @param user_id идентификатор пользователя.
     * @param amount баланс в копейках.
     */
    void setBalance(int user_id, int64_t amount) {
        if(amount >= 0) {
            _balances[user_id] = amount;
        }
    }

    /**
     * Получение баланса пользователя.
     *
     * @param user_id идентификатор пользователя.
     * @return доступный баланс в копейках.
     */
    int64_t getBalance(int user_id) const {
        auto it = _balances.find(user_id);

        if(it == _balances.end()) {
            return 0;
        }

        return it->second;
    }

    /**
     * Списание средств с баланса пользователя.
     *
     * @param user_id идентификатор пользователя.
     * @param amount сумма списания в копейках.
     * @return true, если средств достаточно.
     */
    bool charge(int user_id, int64_t amount) override {
        if(amount < 0) {
            return false;
        }

        if(getBalance(user_id) < amount) {
            return false;
        }

        _balances[user_id] -= amount;
        return true;
    }
};

/**
 * Запись о попытке изменения тарифного плана.
 */
class Transaction {
private:
    int _id;
    int _user_id;
    int _subscription_id;
    int64_t _amount;
    bool _is_successful;

public:
    /**
     * Создание записи транзакции.
     *
     * @param id идентификатор транзакции.
     * @param user_id идентификатор пользователя.
     * @param subscription_id идентификатор подписки.
     * @param amount сумма списания в копейках.
     * @param is_successful результат операции.
     */
    Transaction(
        int id,
        int user_id,
        int subscription_id,
        int64_t amount,
        bool is_successful
    ) : _id(id),
        _user_id(user_id),
        _subscription_id(subscription_id),
        _amount(amount),
        _is_successful(is_successful) {}

    /**
     * Получение идентификатора транзакции.
     *
     * @return идентификатор транзакции.
     */
    int getId() const {
        return _id;
    }

    /**
     * Получение идентификатора пользователя.
     *
     * @return идентификатор пользователя.
     */
    int getUserId() const {
        return _user_id;
    }

    /**
     * Получение идентификатора подписки.
     *
     * @return идентификатор подписки.
     */
    int getSubscriptionId() const {
        return _subscription_id;
    }

    /**
     * Получение суммы списания.
     *
     * @return сумма в копейках.
     */
    int64_t getAmount() const {
        return _amount;
    }

    /**
     * Проверка успешности операции.
     *
     * @return true при успешной операции.
     */
    bool isSuccessful() const {
        return _is_successful;
    }
};

/**
 * Хранилище истории транзакций.
 */
class TransactionHistory {
private:
    std::vector<Transaction> _transactions;
    std::unordered_map<int, std::vector<size_t>>
        _transactions_by_user;

public:
    /**
     * Добавление транзакции в историю.
     *
     * @param transaction запись о транзакции.
     */
    void add(const Transaction& transaction) {
        const size_t INDEX = _transactions.size();

        _transactions.push_back(transaction);

        _transactions_by_user[transaction.getUserId()]
            .push_back(INDEX);
    }

    /**
     * Получение истории транзакций пользователя.
     *
     * @param user_id идентификатор пользователя.
     * @return список транзакций пользователя.
     */
    std::vector<Transaction> findByUser(int user_id) const {
        std::vector<Transaction> result;

        auto it = _transactions_by_user.find(user_id);

        if(it == _transactions_by_user.end()) {
            return result;
        }

        for(size_t index : it->second) {
            result.push_back(_transactions[index]);
        }

        return result;
    }
};

/**
 * Сервис управления оплатой и сменой тарифов.
 */
class BillingService {
private:
    ISubscriptionRepo& _subscription_repo;
    IPaymentGateway& _payment_gateway;
    TransactionHistory& _transaction_history;
    int _next_transaction_id = 1;

public:
    /**
     * Создание сервиса биллинга.
     *
     * @param subscription_repo репозиторий подписок.
     * @param payment_gateway платёжный шлюз.
     * @param transaction_history история транзакций.
     */
    BillingService(
        ISubscriptionRepo& subscription_repo,
        IPaymentGateway& payment_gateway,
        TransactionHistory& transaction_history
    ) : _subscription_repo(subscription_repo),
        _payment_gateway(payment_gateway),
        _transaction_history(transaction_history) {}

    /**
     * Смена тарифного плана с пропорциональной доплатой.
     *
     * @param subscription_id идентификатор подписки.
     * @param old_plan текущий тарифный план.
     * @param new_plan новый тарифный план.
     * @param remaining_days количество оставшихся дней.
     * @param period_days длительность расчётного периода.
     * @return true, если тариф успешно изменён.
     */
    bool changePlan(
        int subscription_id,
        const IPlan& old_plan,
        const IPlan& new_plan,
        int remaining_days,
        int period_days
    ) {
        ISubscription* p_subscription =
            _subscription_repo.findById(subscription_id);

        if(p_subscription == nullptr) {
            return false;
        }

        if(!p_subscription->isActive()) {
            return false;
        }

        if(p_subscription->getPlanId() != old_plan.getId()) {
            return false;
        }

        if(period_days <= 0 ||
           remaining_days < 0 ||
           remaining_days > period_days) {
            return false;
        }

        if(old_plan.getMonthlyPrice() < 0 ||
           new_plan.getMonthlyPrice() < 0) {
            return false;
        }

        if(old_plan.getId() == new_plan.getId()) {
            return true;
        }

        const int USER_ID = p_subscription->getUserId();

        const int64_t PRICE_DIFFERENCE =
            new_plan.getMonthlyPrice() -
            old_plan.getMonthlyPrice();

        int64_t amount = 0;

        if(PRICE_DIFFERENCE > 0) {
            amount = PRICE_DIFFERENCE * remaining_days /
                     period_days;

            if(PRICE_DIFFERENCE * remaining_days %
               period_days != 0) {
                amount++;
            }
        }

        bool is_successful = true;

        if(amount > 0) {
            is_successful = _payment_gateway.charge(
                USER_ID,
                amount
            );
        }

        Transaction transaction(
            _next_transaction_id++,
            USER_ID,
            subscription_id,
            amount,
            is_successful
        );

        _transaction_history.add(transaction);

        if(!is_successful) {
            return false;
        }

        p_subscription->setPlanId(new_plan.getId());

        return true;
    }
};

/**
 * Вывод суммы в рублях и копейках.
 *
 * @param amount сумма в копейках.
 */
void printMoney(int64_t amount){
    std::cout << amount / 100 << ".";

    const int64_t KOPECKS = amount % 100;

    if (KOPECKS < 10) {
        std::cout << "0";
    }

    std::cout << KOPECKS << " руб.";
}

/**
 * Вывод всех подписок пользователя.
 *
 * @param subscription_repo репозиторий подписок.
 * @param user_id идентификатор пользователя.
 */
void printSubscriptions(
    const ISubscriptionRepo& subscription_repo,
    int user_id
){
    const std::vector<const ISubscription*> SUBSCRIPTIONS =
        subscription_repo.findByUser(user_id);

    std::cout << "\nПодписки пользователя " << user_id << ":\n";

    if (SUBSCRIPTIONS.empty()) {
        std::cout << "Подписки не найдены.\n";
        return;
    }

    for (const ISubscription* p_subscription : SUBSCRIPTIONS) {
        std::cout << "Подписка: " << p_subscription->getId()
                  << ", тариф: " << p_subscription->getPlanId()
                  << ", статус: ";

        if (p_subscription->isActive()) {
            std::cout << "Активна\n";
        } else {
            std::cout << "Отменена\n";
        }
    }
}

/**
 * Вывод истории транзакций пользователя.
 *
 * @param history хранилище транзакций.
 * @param user_id идентификатор пользователя.
 */
void printTransactions(
    const TransactionHistory& history,
    int user_id
){
    const std::vector<Transaction> TRANSACTIONS =
        history.findByUser(user_id);

    std::cout << "\nИстория транзакций:\n";

    for (const Transaction& transaction : TRANSACTIONS) {
        std::cout << "Транзакция: " << transaction.getId()
                  << ", подписка: "
                  << transaction.getSubscriptionId()
                  << ", сумма: ";

        printMoney(transaction.getAmount());

        std::cout << ", результат: ";

        if (transaction.isSuccessful()) {
            std::cout << "Успешно\n";
        } else {
            std::cout << "Отказ в оплате\n";
        }
    }
}

/**
 * Демонстрация работы системы подписок.
 *
 * @return код завершения программы.
 */
int main(){
    setlocale(LC_ALL, "");

    const Plan BASIC_PLAN(1, "Базовый", 100000);
    const Plan PREMIUM_PLAN(2, "Премиум", 200000);
    const Plan PRO_PLAN(3, "Профессиональный", 400000);

    SubscriptionRepoInMemory subscription_repo;
    PaymentGatewayInMemory payment_gateway;
    TransactionHistory transaction_history;

    BillingService billing(
        subscription_repo,
        payment_gateway,
        transaction_history
    );

    const int USER_ID = 101;
    const int SUBSCRIPTION_ID = 1;
    const int CANCELLED_SUBSCRIPTION_ID = 2;
    const int PERIOD_DAYS = 30;
    const int REMAINING_DAYS = 15;

    payment_gateway.setBalance(USER_ID, 200000);

    subscription_repo.add(
        std::make_unique<Subscription>(
            SUBSCRIPTION_ID,
            USER_ID,
            BASIC_PLAN.getId()
        )
    );

    subscription_repo.add(
        std::make_unique<Subscription>(
            CANCELLED_SUBSCRIPTION_ID,
            USER_ID,
            BASIC_PLAN.getId()
        )
    );

    std::cout << " БАЗОВЫЙ ЭТАП\n";

    printSubscriptions(subscription_repo, USER_ID);

    subscription_repo.cancel(CANCELLED_SUBSCRIPTION_ID);

    std::cout << "\nПосле отмены подписки:\n";

    printSubscriptions(subscription_repo, USER_ID);

    std::cout << "\n УСЛОЖНЁННЫЙ ЭТАП \n";

    std::cout << "\nНачальный баланс: ";
    printMoney(payment_gateway.getBalance(USER_ID));
    std::cout << "\n";

    std::cout << "\nСмена тарифа Базовый -> Премиум:\n";

    bool result = billing.changePlan(
        SUBSCRIPTION_ID,
        BASIC_PLAN,
        PREMIUM_PLAN,
        REMAINING_DAYS,
        PERIOD_DAYS
    );

    std::cout << (result
        ? "Тариф успешно изменён\n"
        : "Ошибка: не удалось изменить тариф\n");

    std::cout << "\nСмена тарифа Премиум -> Профессиональный:\n";

    result = billing.changePlan(
        SUBSCRIPTION_ID,
        PREMIUM_PLAN,
        PRO_PLAN,
        REMAINING_DAYS,
        PERIOD_DAYS
    );

    std::cout << (result
        ? "Тариф успешно изменён\n"
        : "Ошибка: не удалось изменить тариф\n");

    std::cout << "\nИтоговый баланс: ";
    printMoney(payment_gateway.getBalance(USER_ID));
    std::cout << "\n";

    printSubscriptions(subscription_repo, USER_ID);
    printTransactions(transaction_history, USER_ID);

    return 0;
}