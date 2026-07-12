from backend.tables.database import Database

class GamesLibraryTable(Database):
    def __init__(self):
        schema = """
        id INTEGER PRIMARY KEY,
        user_id INTEGER NOT NULL,
        game_id INTEGER NOT NULL,
        console TEXT NOT NULL,
        status TEXT NOT NULL,
        FOREIGN KEY (game_id) REFERENCES games(id)
        """

        super().__init__(table_name="games_library",schema=schema)

    def add_game(self, user_id:int, game_id:int, console:str, status:str):
        query = f"""
        INSERT INTO {self.table_name}(user_id,game_id,console,status)
        VALUES (?,?,?,?)
        """

        self.sql_execute(query,(user_id,game_id,console,status))