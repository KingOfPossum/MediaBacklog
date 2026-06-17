from database import Database

class IgdbPlatformsTable(Database):
    def __init__(self):
        schema = """
        id INTEGER PRIMARY KEY,
        platform_name TEXT NOT NULL
        """

        super().__init__(table_name="igdb_platforms",schema=schema)
