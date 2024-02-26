# Многопоточное клиент-сервреное приложение на С++

Инструкция по запуску:
1. Если СMake не установлен, то для установки выполните в консоли следующие команды </br>
<code>sudo apt-get update</code></br>
<code>sudo apt-get install cmake</code>
2. В окне терминала Linux выполните команду</br>
<code>git clone <адрес_репозитория></code>
5. Для установки и запуска сервера выполните команду</br>
<code>cd tcp-cpp && cd serverPath && mkdir build && cd build && cmake .. && make run_server</code>
7. Для установки и запуска клиента в новом окне терминала выполните команду</br>
<code>cd tcp-cpp && cd clientPath && mkdir build && cd build && cmake .. && make run_client</code>

*путь тестового файла для отправки на сервер: <code>../test.txt</code>


