# Многопоточное клиент-сервреное приложение на С++

Инструкция по запуску:
1. Если СMake не установлен, то для установки выполните в консоли следующие команды </br>
<code>sudo apt-get update</code></br>
<code>sudo apt-get install cmake</code>
2. В окне терминала Linux выполните команду</br>
<code>git clone <адрес_репозитория></code>
5. Для установки и запуска сервера выполните команду</br>
<code>cd tcp-cpp && cd serverPath && mkdir build && cd build && cmake .. && make</code>
6. Для запуска сервера выполните команду</br>
<code>./server</code>
7. Для установки клиента в новом окне терминала выполните команду</br>
<code>cd tcp-cpp && cd clientPath && mkdir build && cd build && cmake .. && make</code>
8. Для запуска клиента выполните команду</br>
<code>./client -s 127.0.0.1 -p 12345</code>

*путь тестового файла для отправки на сервер: <code>../test.txt</code>


