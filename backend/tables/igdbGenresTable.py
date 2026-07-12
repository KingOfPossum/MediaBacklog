from charset_normalizer.cli import query_yes_no

from backend.tables.database import Database

class IgdbGenresTable(Database):
    def __init__(self):
        schema = """
        id INTEGER PRIMARY KEY,
        genre_name TEXT NOT NULL
        """

        super().__init__(table_name="igdb_genres",schema=schema)

    def add_genre(self, genre_name: str):
        query = f"""
        INSERT INTO {self.table_name}(genre_name)
        VALUES (?)
        """

        print(genre_name)

        self.sql_execute(query,(genre_name,))

    def check_if_genre_exists(self,genre_name: str):
        query = f"""
        SELECT *
        FROM {self.table_name}
        WHERE genre_name=?
        """

        genres = self.sql_execute_fetchone(query,(genre_name,))

        if genres is None:
            return False
        return True

    def get_genre_id(self, genre_name: str) -> int | None:
        query = f"""
        SELECT id
        FROM {self.table_name}
        WHERE genre_name=?
        """

        id = self.sql_execute_fetchone(query,(genre_name,))
        return id[0] if id else None