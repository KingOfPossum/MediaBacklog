from database import Database

class GamesBacklogTable(Database):
    def __init__(self):
        schema = """
        id INTEGER PRIMARY KEY,
        user_id INTEGER NOT NULL,
        game_id INTEGER NOT NULL
        """

        super().__init__(table_name="games_backlog",schema=schema)