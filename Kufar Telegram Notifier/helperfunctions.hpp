//
//  helperfunctions.hpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 04.06.2022.
//

#ifndef helperfunctions_hpp
#define helperfunctions_hpp

/**
Определение состояния вывода отладочных
сообщений на экран в зависимости от типа
сборки программы
 */

#ifdef DEBUG
    #define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
    #define DEBUG_MSG(str) do { } while ( false )
#endif

#include <optional>
#include "json.hpp"

static const std::string PROPERTY_UNDEFINED = "[UNDEFINED]";

/**
 Получение значения из JSON
 для переменных опционального типа.
 */

template<typename T>
std::optional<T> getOptionalValue(const nlohmann::json &obj, const std::string &key) try {
    return obj.at(key).get<T>();
} catch (...) {
    return std::nullopt;
}

/**
 Вывод переменных опционального типа в удобном формате.
 Значение есть: *unwrapped optional*
 Значения нет: [UNDEFINED]
 */

template<typename T>
std::ostream &operator << (std::ostream &os, std::optional<T> const &opt) {
    if (opt.has_value()) {
        if constexpr (std::is_same_v<T, bool>) {
            return (os << (opt.value() == true ? "Да" : "Нет"));
        }
        return (os << opt.value());
    }
    return (os << PROPERTY_UNDEFINED);
}


/**
  Проверка вектора на наличие заданного значения
 */
bool vectorContains(const std::vector<int> &, const int &);

/**
  Проверка на существование файла
 */

bool fileExists(const std::string &);

/**
  Возврат размера файла
 */
uint64_t getFileSize(const std::string &);

/**
 Чтение файла и возврат
 содержимого в виде string.
 */

std::string getTextFromFile(const std::string &);

/**
 Конвертирование времени Zulu формата
 в unix timestamp
 */

time_t zuluToTimestamp(const std::string &);

/**
 Соединение значений вектора
 с использованием заданного разделителя
 */

std::string joinIntVector(const std::vector<int> &, const std::string &);

/**
 Возвращает дату в формате unix timestamp,
 со здвигом на указанное количество часов
 */

time_t timestampShift(const time_t &, int);

#endif /* helperfunctions_hpp */
