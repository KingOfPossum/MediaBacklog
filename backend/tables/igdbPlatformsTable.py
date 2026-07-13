from backend.tables.database import Database

class IgdbPlatformsTable(Database):
    def __init__(self):
        schema = """
        id INTEGER PRIMARY KEY,
        platform_name TEXT NOT NULL
        """

        super().__init__(table_name="igdb_platforms",schema=schema)

    def add_platform(self, platform_name: str):
        query = f"""
        INSERT INTO {self.table_name} (platform_name)
        VALUES (?)
        """

        self.sql_execute(query,(platform_name,))

    def check_if_platform_exists(self, platform_name: str) -> bool:
        query = f"""
        SELECT * FROM {self.table_name}
        WHERE platform_name=?
        """

        platform = self.sql_execute_fetchone(query,(platform_name,))

        if platform is None:
            return False
        return True

    def get_platform_id(self, platform_name: str) -> int | None:
        query = f"""
        SELECT id FROM {self.table_name}
        WHERE platform_name=?
        """

        id = self.sql_execute_fetchone(query, (platform_name,))
        return id[0] if id else None

    def get_platform_name(self,platform_id:int) -> str | None:
        query = f"""
        SELECT platform_name
        FROM {self.table_name}
        WHERE id=?
        """

        name = self.sql_execute_fetchone(query,(platform_id,))
        return name[0] if name else None