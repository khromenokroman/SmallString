#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>

#include <boost/container/small_vector.hpp>

namespace ngfw::sdk::string {

    /**
     * @brief Класс SmallString - альтернатива std::string с поддержкой SSO (small string optimization).
     *
     * В отличие от std::string, строки SmallString не являются нуль-терминированными,
     * поэтому методы c_str() отсутствуют. Метод data() возвращает буфер без завершающего
     * символа `\0`.
     * @tparam N Размер внутреннего буфера для SSO.
     */
    template<std::size_t N>
    class SmallString final {
        using Container = boost::container::small_vector<char, N>;

    public:
        using value_type = char;

        /** @brief Конструктор для создания пустой строки. */
        SmallString() = default;

        /** @brief Копирующий конструктор */
        SmallString(SmallString<N> const &) = default;

        /** @brief Перемещающий конструктор. */
        explicit SmallString(SmallString<N> &&) noexcept = default;

        /**
         * @brief Конструктор создания строки из std::string_view.
         * @param sv Строка, из которой будут скопированы данные.
         */
        explicit SmallString(std::string_view sv);

        /**
         * @brief Оператор присваивания значения строки из std::string_view.
         * @param sv Строка, из которой будут скопированы данные.
         * @return Ссылка на текущий объект.
         */
        SmallString &operator=(std::string_view sv);

        /** @brief Оператор копирующего присваивания. */
        SmallString &operator=(SmallString const &) = default;

        /** @brief Оператор перемещающего присваивания. */
        SmallString &operator=(SmallString &&) noexcept = default;

        /**
         * @brief Оператор добавления содержимого строки std::string_view в текущую строку.
         * @param sv Строка, добавляемая в конец текущей строки.
         * @return Ссылка на текущий объект.
         */
        SmallString &operator+=(std::string_view sv) {
            append(sv);
            return *this;
        }

        /**
         * @brief Оператор добавления одного символа в конец строки.
         * @param c Символ, который добавляется в конец.
         * @return Ссылка на текущий объект.
         */
        SmallString &operator+=(char c) {
            push_back(c);
            return *this;
        }

        /**
         * @brief Оператор преобразования строки в std::string_view.
         * @return Объект std::string_view с содержимым строки.
         */
        operator std::string_view() const;

        /**
         * @brief Оператор индексирования для чтения символа по позиции.
         * @param pos Позиция символа.
         * @return Символ из строки по заданной позиции.
         */
        char const &operator[](std::size_t pos) const;

        /**
         * @brief Оператор индексирования для чтения/изменения символа по позиции.
         * @param pos Позиция символа.
         * @return Ссылка на символ строки по заданной позиции.
         */
        char &operator[](std::size_t pos);

        /**
         * @brief Возвращает символ по индексу с проверкой на выход за границы.
         * @param pos Позиция символа.
         * @return Символ по заданной позиции.
         * @throw std::out_of_range Если индекс выходит за пределы строки.
         */
        char const &at(std::size_t pos) const;

        /**
         * @brief Чтение/изменение символа по индексу с проверкой на выход за границы.
         * @param pos Позиция символа.
         * @return Ссылка на символ строки.
         * @throw std::out_of_range Если индекс выходит за пределы строки.
         */
        char &at(std::size_t pos);

        using iterator = typename Container::iterator; /**< Итератор для перебора символов строки. */
        using const_iterator =
                typename Container::const_iterator; /**< Константный итератор для перебора символов строки. */

        /** @brief Возвращает итератор на начало строки. */
        iterator begin() noexcept;
        const_iterator begin() const noexcept;

        /** @brief Возвращает итератор на конец строки. */
        iterator end() noexcept;
        const_iterator end() const noexcept;

        /** @brief Возвращает текущий размер строки. */
        std::size_t size() const noexcept;

        /**
         * @brief Возвращает указатель на данные строки (только для чтения).
         * @note Буфер не включает завершающий нуль-символ (`\0`).
         * @return Указатель на буфер строки.
         */
        char const *data() const noexcept;

        /**
         * @brief Возвращает указатель на данные строки (для изменения).
         * @note Буфер не включает завершающий нуль-символ (`\0`).
         * @return Указатель на буфер строки.
         */
        char *data() noexcept;

        /**
         * @brief Изменяет размер строки.
         * При увеличении размера новые символы заполняются указанным значением.
         * @param n Новый размер строки.
         * @param c Заполняющий символ.
         */
        void resize(std::size_t n, char c);

        /**
         * @brief Изменяет размер строки.
         * При увеличении размера новые символы заполняются символом `\0`.
         * @param n Новый размер строки.
         */
        void resize(std::size_t n);

        /**
         * @brief Изменяет размер строки с использованием пользовательской операции.
         * @tparam Operation Тип операции.
         * @param size Новый размер строки.
         * @param op Пользовательская операция, которая модифицирует строку.
         */
        template<class Operation>
        void resize_and_overwrite(std::size_t size, Operation op);

        /** @brief Уменьшает емкость строки до текущего размера. */
        void shrink_to_fit();

        /** @brief Возвращает емкость строки (выделенная память). */
        std::size_t capacity() const noexcept;

        /** @brief Резервирует память под строку размером не менее n байт. */
        void reserve(std::size_t n);

        /** @brief Очищает строку. */
        void clear() noexcept;

        /** @brief Проверяет, является ли строка пустой. */
        bool empty() const noexcept;

        /** @brief Возвращает ссылку на первый символ строки. */
        char &front();
        char const &front() const;

        /** @brief Возвращает ссылку на последний символ строки. */
        char &back();
        char const &back() const;

        /** @brief Добавляет символ в конец строки. */
        void push_back(char c);

        /**
         * @brief Добавляет содержимое строки std::string_view в конец текущей строки.
         * @param str Строка для добавления.
         */
        void append(std::string_view str);

        /**
         * @brief Вставляет элементы из диапазона [begin, end) перед указанной позицией.
         * @tparam InputIt Тип итератора.
         * @param pos Позиция, куда вставляются элементы.
         * @param begin Итератор начала диапазона.
         * @param end Итератор конца диапазона.
         */
        template<class InputIt>
        void insert(const_iterator pos, InputIt begin, InputIt end);

        /** @brief Удаляет последний символ строки. */
        void pop_back();

    private:
        ::boost::container::small_vector<char, N> m_data; /**< Контейнер для хранения данных строки. */
    };


    template<std::size_t N>
    SmallString<N>::SmallString(std::string_view sv) : m_data(sv.begin(), sv.end()) {}

    template<std::size_t N>
    SmallString<N>::operator std::string_view() const {
        return std::string_view{m_data.data(), m_data.size()};
    }

    template<std::size_t N>
    SmallString<N> &SmallString<N>::operator=(std::string_view sv) {
        m_data = {sv.begin(), sv.end()};
        return *this;
    }

    template<std::size_t N>
    bool operator==(SmallString<N> const &str, std::string_view sv) {
        return std::string_view{str} == sv;
    }

    template<std::size_t N>
    bool operator==(std::string_view sv, SmallString<N> const &str) {
        return std::string_view{str} == sv;
    }

    template<std::size_t N>
    bool operator==(SmallString<N> const &str1, SmallString<N> const &str2) {
        return std::string_view{str1} == std::string_view{str2};
    }

    template<std::size_t N>
    bool operator!=(SmallString<N> const &str1, SmallString<N> const &str2) {
        return !(str1 == str2);
    }

    template<std::size_t N>
    char const &SmallString<N>::operator[](std::size_t pos) const {
        return m_data[pos];
    }

    template<std::size_t N>
    char &SmallString<N>::operator[](std::size_t pos) {
        return m_data[pos];
    }

    template<std::size_t N>
    char const &SmallString<N>::at(std::size_t pos) const {
        if (size() <= pos)
            throw std::out_of_range("at");
        return m_data[pos];
    }

    template<std::size_t N>
    char &SmallString<N>::at(std::size_t pos) {
        if (size() <= pos)
            throw std::out_of_range("at");
        return m_data[pos];
    }

    template<std::size_t N>
    typename SmallString<N>::iterator SmallString<N>::begin() noexcept {
        return {m_data.begin()};
    }

    template<std::size_t N>
    typename SmallString<N>::const_iterator SmallString<N>::begin() const noexcept {
        return {m_data.begin()};
    }

    template<std::size_t N>
    typename SmallString<N>::iterator SmallString<N>::end() noexcept {
        return {m_data.end()};
    }

    template<std::size_t N>
    typename SmallString<N>::const_iterator SmallString<N>::end() const noexcept {
        return {m_data.end()};
    }

    template<std::size_t N>
    std::size_t SmallString<N>::size() const noexcept {
        return m_data.size();
    }

    template<std::size_t N>
    char const *SmallString<N>::data() const noexcept {
        return m_data.data();
    }

    template<std::size_t N>
    char *SmallString<N>::data() noexcept {
        return m_data.data();
    }

    template<std::size_t N>
    bool SmallString<N>::empty() const noexcept {
        return m_data.empty();
    }

    template<std::size_t N>
    char &SmallString<N>::front() {
        return m_data.front();
    }

    template<std::size_t N>
    char const &SmallString<N>::front() const {
        return m_data.front();
    }

    template<std::size_t N>
    char &SmallString<N>::back() {
        return m_data.back();
    }

    template<std::size_t N>
    char const &SmallString<N>::back() const {
        return m_data.back();
    }

    template<std::size_t N>
    void SmallString<N>::push_back(char c) {
        m_data.push_back(c);
    }

    template<std::size_t N>
    void SmallString<N>::append(std::string_view str) {
        std::size_t old_size = m_data.size();
        m_data.insert(m_data.begin() + old_size, str.begin(), str.end());
    }

    template<std::size_t N>
    template<class InputIt>
    void SmallString<N>::insert(SmallString::const_iterator pos, InputIt begin, InputIt end) {
        m_data.insert(pos, begin, end);
    }

    template<std::size_t N>
    void SmallString<N>::pop_back() {
        m_data.pop_back();
    }

    template<std::size_t N>
    void SmallString<N>::resize(std::size_t n, char c) {
        m_data.resize(n, c);
    }

    template<std::size_t N>
    void SmallString<N>::resize(std::size_t n) {
        resize(n, '\0');
    }

    template<std::size_t N>
    template<class Operation>
    void SmallString<N>::resize_and_overwrite(std::size_t size, Operation op) {
        m_data.resize(size, boost::container::default_init);
        m_data.resize(std::move(op)(m_data.data(), size), boost::container::default_init);
        assert(m_data.size() <= size);
    }

    template<std::size_t N>
    void SmallString<N>::shrink_to_fit() {
        m_data.shrink_to_fit();
    }

    template<std::size_t N>
    std::size_t SmallString<N>::capacity() const noexcept {
        return m_data.capacity();
    }

    template<std::size_t N>
    void SmallString<N>::reserve(std::size_t n) {
        return m_data.reserve(n);
    }

    template<std::size_t N>
    void SmallString<N>::clear() noexcept {
        m_data.clear();
    }

} // namespace ngfw::sdk::string

template<std::size_t N>
struct std::hash<::ngfw::sdk::string::SmallString<N>> {
    std::size_t operator()(::ngfw::sdk::string::SmallString<N> const &s) const noexcept {
        return std::hash<std::string_view>{}(std::string_view{s});
    }
};
