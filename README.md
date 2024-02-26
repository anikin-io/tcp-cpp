# Многопоточное клиент-серевреное приложение на С++

Инструкция по запуску:
1. Если СMake не установлен на вашем компьютере, то для установки выполните в консоли следующие команды
sudo apt-get update
sudo apt-get install cmake
2. В окне терминала Linux выполнить команду git clone <адрес_репозитория>
3. Для установки и запуска сервера выполнить команду cd tcp-cpp && cd serverPath && mkdir build && cd build && cmake .. && make run_server
4. Для установки и запуска клиента в новом окне терминала выполнить команду cd tcp-cpp && cd clientPath && mkdir build && cd build && cmake .. && make run_client

*путь тестового файла для отправки на сервер: ../test.txt


