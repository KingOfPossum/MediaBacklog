from backend.GameEntry import GameEntry
from backend.HowLongToBeatEntry import HowLongToBeatEntry
from backend.tables.database import Database

class GamesTable(Database):
    def __init__(self):
        schema  = """
        id INTEGER PRIMARY KEY,
        igdb_id INTEGER,
        name TEXT NOT NULL,
        howlongtobeat_cover_url TEXT,
        main_story_length INTEGER,
        main_extra_length INTEGER,
        completionist_length INTEGER,
        min_price INTEGER,
        avg_price INTEGER,
        max_price INTEGER
        """

        super().__init__(table_name="games",schema=schema)

    def add_game(self,game_name: str, igdb_id: int = -1, howlongtobeat_infos: HowLongToBeatEntry = None) -> int:
        query = f"""
        INSERT INTO {self.table_name}(igdb_id, name,howlongtobeat_cover_url,main_story_length,main_extra_length,completionist_length)
        VALUES (?,?,?,?,?,?)
        RETURNING id
        """

        howlongtobeat_cover_url = howlongtobeat_infos.cover_url if howlongtobeat_infos else ""
        main_story = howlongtobeat_infos.main_story if howlongtobeat_infos else -1
        main_extra = howlongtobeat_infos.main_extra if howlongtobeat_infos else -1
        completionist = howlongtobeat_infos.completionist if howlongtobeat_infos else -1

        return self.sql_execute_fetchone(query,(igdb_id,game_name,howlongtobeat_cover_url,main_story,main_extra,completionist))[0]

    def get_game_id(self,game_name:str) -> int | None:
        query = f"""
        SELECT id 
        FROM {self.table_name}
        WHERE name=?
        """

        id = self.sql_execute_fetchone(query,(game_name,))
        return id[0] if id else None

    def get_game(self,game_id: int) -> GameEntry:
        query = f"""
        SELECT *
        FROM {self.table_name}
        WHERE id=?
        """

        entry = self.sql_execute_fetchone(query,(game_id,))
        return GameEntry(*entry) if entry else None