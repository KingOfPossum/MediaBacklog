from backend.tables.database import Database

class UsersTable(Database):
    def __init__(self):
        schema = """
        id INTEGER PRIMARY KEY,
        user_name TEXT UNIQUE,
        password TEXT,
        profile_picture_url TEXT,
        is_admin INTEGER DEFAULT 0
        """

        super().__init__(table_name="users",schema=schema)