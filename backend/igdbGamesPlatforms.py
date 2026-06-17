from database import Database

class IgdbGamesPlatformsTable(Database):
    def __init__(self):
        schema = """
        game_id INTEGER NOT NULL,
        platform_id INTEGER NOT NULL,
        FOREIGN KEY (game_id) REFERENCES igdb_games(game_id),
        FOREIGN KEY (platform_id) REFERENCES igdb_platforms(platform_id)
        """