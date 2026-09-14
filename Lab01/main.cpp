#include <iostream>

int isprime(int number){
    if (number < 2){
        return 0;
    }
    for (int div = 2; div <= number / div; div++){
        if (number % div == 0){
            return 0;
        }
    }
    return 1;
}

int main(){
    int N;
    std::cout << "Ввод: ";
    std::cin >> N;
    if (N < 2){
        std::cout << "N не меньше 2." << std::endl;
    }
    int size = N - 1;
    int* numbers = new int[size];
    for (int i = 2; i < size; i++){
        numbers[i] = i;
    }
    int count = 0;
    std::cout <<"Вывод: ";
    for (int i = 0; i < size; i++){
        if (isprime(numbers[i])){
            std::cout << numbers[i] << " ";
            count++;
        }
    }  
    std::cout << std::endl;
    std::cout << "Количество простых чисел: " << count << std::endl;
    delete[] numbers;
    return 0;
     
}