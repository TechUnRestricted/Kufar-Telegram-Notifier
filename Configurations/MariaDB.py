import mariadb


class MariaDB:
    # Подключение к платформе MariaDB
    # Необходимо для логирования объявлений и предотвращения дублирования результатов при перезапуске скрипта
    try:
        conn = mariadb.connect(
            user="johnappleseed",
            password="admin",
            host="127.0.0.1",
            port=3306,
            database="kufar"
        )
        conn.autocommit = True
    except mariadb.Error as e:
        print(f"Error connecting to MariaDB Platform: {e}")
        exit(1)
