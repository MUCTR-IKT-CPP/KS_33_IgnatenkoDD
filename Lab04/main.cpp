#include <iostream>
#include <string>
#include <utility>

struct Texture{
    std::string name;
    int width;
    int height;
};

struct Mesh{
    std::string name;
    int vertex_count;
};

struct Sound{
    std::string name;
    float duration;
};

enum class ResourceType{
    TEXTURE,
    MESH,
    SOUND
};

class Resource{
private:
    void* _p_data;
    ResourceType _type;
    bool _is_loaded;

    /*
     * Копирование данных ресурса
     *
     * @param other ресурс, данные которого необходимо скопировать.
     */
    void copyData(const Resource& other){
        if(!other._is_loaded || other._p_data == nullptr){
            _p_data = nullptr;
            _is_loaded = false;
            return;
        }

        switch(other._type){
            case ResourceType::TEXTURE:
                _p_data = new Texture(
                    *static_cast<Texture*>(other._p_data)
                );
                break;

            case ResourceType::MESH:
                _p_data = new Mesh(
                    *static_cast<Mesh*>(other._p_data)
                );
                break;

            case ResourceType::SOUND:
                _p_data = new Sound(
                    *static_cast<Sound*>(other._p_data)
                );
                break;
        }

        _is_loaded = true;
    }

public:
    /*
     * Конструктор ресурса по умолчанию
     *
     * Создает пустой незагруженный ресурс.
     */
    Resource(){
        _p_data = nullptr;
        _type = ResourceType::TEXTURE;
        _is_loaded = false;
    }

    /*
     * Конструктор ресурса с параметрами
     *
     * @param p_data указатель на данные ресурса.
     * @param type тип создаваемого ресурса.
     */
    Resource(void* p_data, ResourceType type){
        _p_data = p_data;
        _type = type;
        _is_loaded = p_data != nullptr;
    }

    /*
     * Конструктор копирования ресурса
     *
     * Создает глубокую копию переданного ресурса.
     *
     * @param other ресурс, который необходимо скопировать.
     */
    Resource(const Resource& other){
        _p_data = nullptr;
        _type = other._type;
        _is_loaded = false;

        copyData(other);
    }

    /*
     * Оператор копирующего присваивания
     *
     * Освобождает текущие данные и создает глубокую копию
     * переданного ресурса.
     *
     * @param other ресурс, который необходимо скопировать.
     * @return ссылка на текущий объект.
     */
    Resource& operator=(const Resource& other){
        if(this != &other){
            unload();

            _type = other._type;
            copyData(other);
        }

        return *this;
    }

    /*
     * Конструктор перемещения ресурса
     *
     * Передает владение памятью от другого ресурса текущему объекту.
     *
     * @param other ресурс, данные которого необходимо переместить.
     */
    Resource(Resource&& other) noexcept{
        _p_data = other._p_data;
        _type = other._type;
        _is_loaded = other._is_loaded;

        other._p_data = nullptr;
        other._is_loaded = false;
    }

    /*
     * Оператор перемещающего присваивания
     *
     * Освобождает текущие данные и принимает владение
     * ресурсом другого объекта.
     *
     * @param other ресурс, данные которого необходимо переместить.
     * @return ссылка на текущий объект.
     */
    Resource& operator=(Resource&& other) noexcept{
        if(this != &other){
            unload();

            _p_data = other._p_data;
            _type = other._type;
            _is_loaded = other._is_loaded;

            other._p_data = nullptr;
            other._is_loaded = false;
        }

        return *this;
    }

    /*
     * Деструктор ресурса
     *
     * Освобождает динамически выделенную память.
     */
    ~Resource(){
        unload();
    }

    /*
     * Выгрузка ресурса
     *
     * Освобождает память, занятую данными ресурса,
     * в соответствии с его типом.
     */
    void unload(){
        if(!_is_loaded || _p_data == nullptr){
            return;
        }

        switch(_type){
            case ResourceType::TEXTURE:
                delete static_cast<Texture*>(_p_data);
                break;

            case ResourceType::MESH:
                delete static_cast<Mesh*>(_p_data);
                break;

            case ResourceType::SOUND:
                delete static_cast<Sound*>(_p_data);
                break;
        }

        _p_data = nullptr;
        _is_loaded = false;
    }

    /*
     * Получение типа ресурса
     *
     * @return тип текущего ресурса.
     */
    ResourceType getType() const{
        return _type;
    }

    /*
     * Проверка состояния ресурса
     *
     * @return true, если ресурс загружен, иначе false.
     */
    bool isLoaded() const{
        return _is_loaded;
    }

    /*
     * Вывод информации о ресурсе
     *
     * Выводит данные ресурса в консоль
     * в соответствии с его типом.
     */
    void print() const{
        if(!_is_loaded || _p_data == nullptr){
            std::cout << "Ресурс не загружен\n";
            return;
        }

        switch(_type){
            case ResourceType::TEXTURE:{
                const Texture* p_texture =
                    static_cast<const Texture*>(_p_data);

                std::cout
                    << "Текстура: "
                    << p_texture->name
                    << ", размер: "
                    << p_texture->width
                    << "x"
                    << p_texture->height
                    << '\n';

                break;
            }

            case ResourceType::MESH:{
                const Mesh* p_mesh =
                    static_cast<const Mesh*>(_p_data);

                std::cout
                    << "Модель: "
                    << p_mesh->name
                    << ", количество вершин: "
                    << p_mesh->vertex_count
                    << '\n';

                break;
            }

            case ResourceType::SOUND:{
                const Sound* p_sound =
                    static_cast<const Sound*>(_p_data);

                std::cout
                    << "Звук: "
                    << p_sound->name
                    << ", длительность: "
                    << p_sound->duration
                    << " сек.\n";

                break;
            }
        }
    }
};

class ResourceManager{
private:
    Resource* _p_resources;
    int _resource_count;
    int _capacity;

    /*
     * Расширение массива ресурсов
     *
     * Создает новый массив большей вместимости,
     * перемещает в него существующие ресурсы
     * и освобождает старый массив.
     */
    void expand(){
        const int NEW_CAPACITY =
            _capacity == 0 ? 2 : _capacity * 2;

        Resource* p_new_resources =
            new Resource[NEW_CAPACITY];

        for(int i = 0; i < _resource_count; i++){
            p_new_resources[i] = std::move(_p_resources[i]);
        }

        delete[] _p_resources;

        _p_resources = p_new_resources;
        _capacity = NEW_CAPACITY;
    }

public:
    /*
     * Конструктор менеджера ресурсов по умолчанию
     *
     * Создает пустой менеджер без выделенной памяти.
     */
    ResourceManager(){
        _p_resources = nullptr;
        _resource_count = 0;
        _capacity = 0;
    }

    /*
     * Конструктор менеджера ресурсов с параметрами
     *
     * @param capacity начальная вместимость массива ресурсов.
     */
    ResourceManager(int capacity){
        _resource_count = 0;
        _capacity = capacity > 0 ? capacity : 0;

        if(_capacity > 0){
            _p_resources = new Resource[_capacity];
        }else{
            _p_resources = nullptr;
        }
    }

    /*
     * Конструктор копирования менеджера ресурсов
     *
     * Создает глубокую копию массива ресурсов.
     *
     * @param other менеджер ресурсов, который необходимо скопировать.
     */
    ResourceManager(const ResourceManager& other){
        _resource_count = other._resource_count;
        _capacity = other._capacity;

        if(_capacity > 0){
            _p_resources = new Resource[_capacity];

            for(int i = 0; i < _resource_count; i++){
                _p_resources[i] = other._p_resources[i];
            }
        }else{
            _p_resources = nullptr;
        }
    }

    /*
     * Оператор копирующего присваивания менеджера ресурсов
     *
     * Создает глубокую копию переданного менеджера.
     *
     * @param other менеджер ресурсов, который необходимо скопировать.
     * @return ссылка на текущий объект.
     */
    ResourceManager& operator=(const ResourceManager& other){
        if(this != &other){
            Resource* p_new_resources = nullptr;

            if(other._capacity > 0){
                p_new_resources =
                    new Resource[other._capacity];

                for(int i = 0; i < other._resource_count; i++){
                    p_new_resources[i] =
                        other._p_resources[i];
                }
            }

            delete[] _p_resources;

            _p_resources = p_new_resources;
            _resource_count = other._resource_count;
            _capacity = other._capacity;
        }

        return *this;
    }

    /*
     * Конструктор перемещения менеджера ресурсов
     *
     * Передает владение динамическим массивом
     * от другого менеджера текущему объекту.
     *
     * @param other менеджер, данные которого необходимо переместить.
     */
    ResourceManager(ResourceManager&& other) noexcept{
        _p_resources = other._p_resources;
        _resource_count = other._resource_count;
        _capacity = other._capacity;

        other._p_resources = nullptr;
        other._resource_count = 0;
        other._capacity = 0;
    }

    /*
     * Оператор перемещающего присваивания менеджера ресурсов
     *
     * Освобождает текущий массив и принимает владение
     * массивом другого менеджера.
     *
     * @param other менеджер, данные которого необходимо переместить.
     * @return ссылка на текущий объект.
     */
    ResourceManager& operator=(ResourceManager&& other) noexcept{
        if(this != &other){
            delete[] _p_resources;

            _p_resources = other._p_resources;
            _resource_count = other._resource_count;
            _capacity = other._capacity;

            other._p_resources = nullptr;
            other._resource_count = 0;
            other._capacity = 0;
        }

        return *this;
    }

    /*
     * Деструктор менеджера ресурсов
     *
     * Освобождает динамический массив ресурсов.
     */
    ~ResourceManager(){
        delete[] _p_resources;
    }

    /*
     * Загрузка текстуры
     *
     * Создает новую текстуру и добавляет ее
     * в массив ресурсов менеджера.
     *
     * @param name название текстуры.
     * @param width ширина текстуры.
     * @param height высота текстуры.
     */
    void loadTexture(
        const std::string& name,
        int width,
        int height
    ){
        if(_resource_count >= _capacity){
            expand();
        }

        Texture* p_texture = new Texture{
            name,
            width,
            height
        };

        _p_resources[_resource_count] =
            Resource(p_texture, ResourceType::TEXTURE);

        _resource_count++;
    }

    /*
     * Загрузка трехмерной модели
     *
     * Создает новую модель и добавляет ее
     * в массив ресурсов менеджера.
     *
     * @param name название модели.
     * @param vertex_count количество вершин модели.
     */
    void loadMesh(
        const std::string& name,
        int vertex_count
    ){
        if(_resource_count >= _capacity){
            expand();
        }

        Mesh* p_mesh = new Mesh{
            name,
            vertex_count
        };

        _p_resources[_resource_count] =
            Resource(p_mesh, ResourceType::MESH);

        _resource_count++;
    }

    /*
     * Загрузка звукового ресурса
     *
     * Создает новый звуковой ресурс и добавляет его
     * в массив ресурсов менеджера.
     *
     * @param name название звукового ресурса.
     * @param duration продолжительность звука в секундах.
     */
    void loadSound(
        const std::string& name,
        float duration
    ){
        if(_resource_count >= _capacity){
            expand();
        }

        Sound* p_sound = new Sound{
            name,
            duration
        };

        _p_resources[_resource_count] =
            Resource(p_sound, ResourceType::SOUND);

        _resource_count++;
    }

    /*
     * Выгрузка ресурса по индексу
     *
     * Освобождает память выбранного ресурса.
     *
     * @param index индекс ресурса в массиве.
     */
    void unloadResource(int index){
        if(index < 0 || index >= _resource_count){
            std::cout << "Некорректный индекс ресурса.\n";
            return;
        }

        _p_resources[index].unload();
    }

    /*
     * Поиск ресурсов по типу
     *
     * Выводит все загруженные ресурсы выбранного типа.
     *
     * @param type тип ресурсов для поиска.
     */
    void findByType(ResourceType type) const{
        std::cout << "\nРесурсы выбранного типа:\n";

        bool is_found = false;

        for(int i = 0; i < _resource_count; i++){
            if(
                _p_resources[i].isLoaded() &&
                _p_resources[i].getType() == type
            ){
                std::cout << "[" << i << "] ";
                _p_resources[i].print();
                is_found = true;
            }
        }

        if(!is_found){
            std::cout << "Ресурсы не найдены.\n";
        }
    }

    /*
     * Подсчет ресурсов выбранного типа
     *
     * @param type тип ресурсов для подсчета.
     * @return количество загруженных ресурсов выбранного типа.
     */
    int countByType(ResourceType type) const{
        int count = 0;

        for(int i = 0; i < _resource_count; i++){
            if(
                _p_resources[i].isLoaded() &&
                _p_resources[i].getType() == type
            ){
                count++;
            }
        }

        return count;
    }

    /*
     * Вывод отчета о ресурсах
     *
     * Выводит список всех ресурсов,
     * их состояние и количество ресурсов каждого типа.
     */
    void printReport() const{
        std::cout << "\nОТЧЕТ О РЕСУРСАХ\n";

        for(int i = 0; i < _resource_count; i++){
            std::cout << "[" << i << "] ";

            if(_p_resources[i].isLoaded()){
                _p_resources[i].print();
            }else{
                std::cout << "Ресурс выгружен\n";
            }
        }

        std::cout << "\nКоличество ресурсов:\n";

        std::cout
            << "Текстуры: "
            << countByType(ResourceType::TEXTURE)
            << '\n';

        std::cout
            << "Модели: "
            << countByType(ResourceType::MESH)
            << '\n';

        std::cout
            << "Звуки: "
            << countByType(ResourceType::SOUND)
            << '\n';
    }
};

/*
 * Главная функция программы
 *
 * Создает менеджер ресурсов, добавляет фиктивные ресурсы,
 * выполняет поиск, выгрузку, вывод отчета
 * и демонстрирует работу конструктора копирования.
 *
 * @return код завершения программы.
 */
int main(){
    ResourceManager manager(3);

    manager.loadTexture(
        "Текстура стены",
        1920,
        1080
    );

    manager.loadTexture(
        "Текстура пола",
        1024,
        1024
    );

    manager.loadMesh(
        "Модель игрока",
        2500
    );

    manager.loadMesh(
        "Модель автомобиля",
        5000
    );

    manager.loadSound(
        "Взрыв",
        2.5f
    );

    manager.loadSound(
        "Фоновая музыка",
        120.0f
    );

    manager.printReport();

    std::cout << "\nПОИСК ТЕКСТУР\n";
    manager.findByType(ResourceType::TEXTURE);

    std::cout << "\nВЫГРУЗКА РЕСУРСА 1\n";
    manager.unloadResource(1);

    manager.printReport();

    std::cout << "\nКОПИРОВАНИЕ МЕНЕДЖЕРА\n";

    ResourceManager manager_copy = manager;
    manager_copy.printReport();

    return 0;
}