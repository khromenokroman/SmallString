### Что делает этот пример:
- На первом этапе демонстрируется основное использование, включая инициализацию, копирование и присваивание.
- Далее показано использование индексов, включая методы `[]` и `at`, для работы с символами.
- Объясняются методы добавления данных (через `+=`, `push_back` и `append`).
- Демонстрируется использование итераторов, в том числе через цикл `range-based for`.
- Показано, как изменять размер строки, резервировать память, а также очищать строку.
- И наконец, приведен пример использования объекта `SmallString` в качестве ключа в контейнере `std::unordered_map`, где используется пользовательский хеш-функционал.

````c++
#include <iostream>
#include <unordered_map>
#include "SmallString.hpp" // Заголовочный файл из вашего проекта

int main() {
    // 1. Создание и базовые операции
    {
        // Создаем пустую строку
        ::string::SmallString<32> smallStr;
        std::cout << "Empty SmallString size: " << smallStr.size() << "\n"; // Output: 0

        // Инициализируем строку при создании
        ::string::SmallString<32> fromStr("Hello, SmallString!");
        std::cout << "Initialized SmallString: " << std::string_view(fromStr) << "\n";

        // Присваивание строки
        smallStr = "New content";
        std::cout << "After assignment: " << std::string_view(smallStr) << "\n";
    }

    // 2. Индексация, чтение и модификация
    {
        // Создаем SmallString с размером внутреннего буфера 16
        ::string::SmallString<16> myString("SmallString");

        // Доступ по индексу
        std::cout << "First character: " << myString[0] << "\n"; // Output: S

        // Изменение символа
        myString[0] = 's';
        std::cout << "Modified string: " << std::string_view(myString) << "\n"; // Output: smallString

        // Проверка at() с проверкой границ
        try {
            std::cout << "Character at 50: " << myString.at(50) << "\n"; // Ошибка: выход за пределы строки
        } catch (const std::out_of_range &e) {
            std::cerr << "Error: " << e.what() << "\n"; // Ожидаем поимку исключения
        }
    }

    // 3. Добавление данных
    {
        ::string::SmallString<32> str("Hello");

        // Добавление строки через оператор +=
        str += ", ";
        str += "World!";
        str.push_back(' '); // Добавление отдельного символа
        std::cout << "Concatenated string: " << std::string_view(str) << "\n"; // Output: Hello, World!

        // Добавление строки через append
        str.append("Extra String");
        std::cout << "After append: " << std::string_view(str) << "\n"; // Output: Hello, World! Extra String
    }

    // 4. Итераторы
    {
        ::string::SmallString<16> str("Iterator Example");

        // Перебор строк с использованием range-based for
        std::cout << "Iterating over characters: ";
        for (const auto &c: str) {
            std::cout << c << ' ';
        }
        std::cout << "\n";

        // Можно использовать и стандартные итераторы
        for (auto it = str.begin(); it != str.end(); ++it) {
            std::cout << *it;
        }
        std::cout << "\n";
    }

    // 5. Резервирование памяти и изменение размера
    {
        ::string::SmallString<16> str("Resize Test");

        // Проверка начального размера и емкости
        std::cout << "Initial size: " << str.size() << "\n"; // Output: 11
        std::cout << "Initial capacity: " << str.capacity() << "\n"; // Зависит от реализации

        // Изменение размера с добавлением символов
        str.resize(20, '.');
        std::cout << "After resize: " << std::string_view(str) << "\n"; // Output: Resize Test........
        std::cout << "New size: " << str.size() << "\n";

        // Резервируем новую емкость
        str.reserve(50);
        std::cout << "Capacity after reserving: " << str.capacity() << "\n"; // Output зависит от реализации
    }

    // 6. Использование с хеш-таблицами
    {
        // Создаем unordered_map с ключами SmallString
        std::unordered_map<string::SmallString<16>, int> myMap;

        // Добавляем элементы
        myMap[string::SmallString<16>("First Key")] = 42;
        myMap[string::SmallString<16>("Second Key")] = 84;

        // Поиск элемента
        ::string::SmallString<16> key("First Key");
        if (myMap.find(key) != myMap.end()) {
            std::cout << "Found key: " << std::string_view(key) << " with value: " << myMap[key] << "\n";
        }

        // Если ключ не найден
        ::string::SmallString<16> missingKey("Missing Key");
        if (myMap.find(missingKey) == myMap.end()) {
            std::cout << "Key not found: " << std::string_view(missingKey) << "\n";
        }
    }

    // 7. Проверка на пустоту и очистка строки
    {
        ::string::SmallString<16> str("Non-empty");

        // Проверяем, пустая ли строка
        std::cout << "Is empty? " << (str.empty() ? "Yes" : "No") << "\n"; // Output: No

        // Очищаем строку
        str.clear();
        std::cout << "Is empty after clear? " << (str.empty() ? "Yes" : "No") << "\n"; // Output: Yes
    }

    return 0;
}
````