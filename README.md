<h1>Kufar Telegram Notifier</h1>
<b>Kufar Telegram Notifier</b> - бот, который раз в заданное время ведет опрос официального Kufar API для поиска новых объявлений по заданным ключевым словам для последующей отправки результатов в Telegram чат.<br>
Использование данного бота предусматривается только для ознакомительных целей.<br><br>
<pre>Если Вы являетесь администрацией Kufar и у Вас имеются претензии к данному проекту - обращайтесь в раздел «<i>Issues</i>» для обратной связи.</pre>
<p align="center">
   <img src="https://user-images.githubusercontent.com/83237609/180989226-ec24b7d5-63ea-4ed5-9830-dd40d27ee30d.png" width="600px"/>
</p>
<h2>Инструкция по сборке</h2>
<details>
   <summary>
      <h3>
         Linux<!-- / macOS-->
      </h3>
   </summary>
   <ol>
      <li>Установите g++: <code>sudo apt-get install g++</code></li>
      <li>
         Установите curl: <code>sudo apt-get install curl libcurl4-gnutls-dev
         </code>
      </li>
      <li>
         <a href="https://github.com/TechUnRestricted/Kufar-Telegram-Notifier/releases">Загрузите</a> исходный код последней версии программы:<br>
      </li>
      <li>
         Распакуйте архив с исходным кодом программы
      </li>
      <li>С помощью <code>cd</code> перейдите в директорию <i>Kufar Telegram Notifier</i>, где находятся .cpp/.hpp файлы
      </li>
      <li>Соберите исполняемый файл с помощью: <code>g++ *.cpp -o executable -std=c++17 -lcurl</code></li>
   </ol>
</details>
<details>
   <summary>
      <h3>macOS</h3>
   </summary>
   <details>
      <summary>Через Xcode (через графический интерфейс)</summary>
      <ol>
         <li>
            <a href="https://github.com/TechUnRestricted/Kufar-Telegram-Notifier/releases">Загрузите</a> исходный код последней версии программы:<br>
         </li>
         <li>
            Распакуйте архив с исходным кодом программы
         </li>
         <li>
            Откройте <code>Kufar Telegram Notifier.xcodeproj</code> в Xcode
         </li>
         <li>
            В верхнем меню-баре выберите "Product" -> "Archive"
         </li>
         <li>
            В новом открывшемся окне нажмите "Distribute Content" -> "Built Products" -> "Next" -> "Export"
         </li>
      </ol>
   </details>
   <details>
      <summary>Через Xcode Command Line Tools (через консольный интерфейс)</summary>
      <ol>
         <li>
            Установите Xcode Command Line Tools: <code>xcode-select —install</code>
         </li>
         <li>
            <a href="https://github.com/TechUnRestricted/Kufar-Telegram-Notifier/releases">Загрузите</a> исходный код последней версии программы:<br>
         </li>
         <li>
            Распакуйте архив с исходным кодом программы
         </li>
         <li>С помощью <code>cd</code> перейдите в директорию <i>Kufar Telegram Notifier</i>, где находятся .cpp/.hpp файлы
         </li>
         <li>Соберите исполняемый файл с помощью: <code>g++ *.cpp -o executable -std=c++17 -lcurl</code></li>
      </ol>
   </details>
</details>
<h2>Конфигурация программы</h2>
<details>
   <summary>
      Настройка kufar-configuration.json
   </summary>
   <details>
      <summary>
         Telegram
      </summary>
      <b>bot-token</b> - токен вашего бота, который будет отправлять сообщения.<br>
      <b>chat-id</b> - идентификатор чата, в который будут отправляться сообщения.
   </details>
   <details>
      <summary>
         Queries
      </summary>
      <b>tag</b> - поисковой запрос.<br>
      <b>only-title-search</b> - осуществление поиска только в заголовках. <sup>(Опционально)</sup>
      <details>
         <summary>
            Price <sup>(Опционально)</sup>
         </summary>
         <b>min</b> - минимальная цена (целочисленное значение в BYN). <sup>(Опционально)</sup><br>
         <b>max</b> - максимальная цена (целочисленное значение в BYN). <sup>(Опционально)</sup>
      </details>
      <b>language</b> - язык. <sup>(Опционально)</sup><br>
      <b>limit</b> - ограничение на количество получаемых объявлений за один запрос. <sup>(Опционально)</sup><br>
      <b>region</b> - <a href="https://github.com/TechUnRestricted/Kufar-Telegram-Notifier/blob/af75848093db0d21959128c56176e3ad2ae9bc29/Kufar%20Telegram%20Notifier/kufar.hpp#L15">номер региона</a> для поиска объявлений. <sup>(Опционально)</sup><br>
      <b>areas</b> - <a href="https://github.com/TechUnRestricted/Kufar-Telegram-Notifier/blob/af75848093db0d21959128c56176e3ad2ae9bc29/Kufar%20Telegram%20Notifier/kufar.hpp#L25">номера областей</a> для поиска объявлений. <sup>(Опционально)</sup>
   </details>
   <details>
      <summary>
         Delays
      </summary>
      <b>query</b> - задержка (в секундах) перед переходом к следующему поисковому запросу.<br>
      <b>loop</b> - задержка (в секундах) перед повторением поиска по очереди с начала.<br>
   </details>
</details>
<h2>Запуск:</h2>
<code>./executable "/путь/к/kufar-configuration.json"</code>
<p align="center">
   <img src="https://user-images.githubusercontent.com/83237609/181288185-7f9c23b0-32bf-4a1a-a3fd-168ed38255e1.png"/>
</p>
