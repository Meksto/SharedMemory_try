# SharedMemory_try

### Описание для кода _writer.cpp_

Данный код представляет собой программу на языке программирования C++ для записи данных в разделяемую память, используя семафоры для синхронизации доступа к разделяемой памяти. Он создает разделяемую память, инициализирует ее значениями, ожидает разрешения на доступ с помощью семафора, записывает данные от пользователя в разделяемую память, ожидает пока пользователь введёт `0` и освобождает семафор.

Пример запуска: `./writer`

### Описание для кода _reader.cpp_

Данный код представляет собой программу на языке программирования C++, которая выполняет чтение данных из разделяемой памяти. Она подключается к разделяемому сегменту памяти, созданному `writer.cpp`, и читает данные, записанные в этот сегмент. Для взаимодействия с разделяемой памятью и синхронизации доступа используются семафоры. После чтения данных `reader.cpp` ожидает пока пользователь введёт `0`, освобождает ресурсы, закрывает доступ к разделяемой памяти, семафору и завершает свою работу.

Пример запуска: `./reader`