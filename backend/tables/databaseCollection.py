from Game import Game
from backend.tables.gamesBacklogTable import GamesBacklogTable
from backend.tables.gamesLibraryTable import GamesLibraryTable
from backend.tables.gamesTable import GamesTable
from backend.tables.igdbGamesGenresTable import IgdbGamesGenresTable
from backend.tables.igdbGamesPlatformsTable import IgdbGamesPlatformsTable
from backend.tables.igdbGamesTable import IgdbGamesTable
from backend.tables.igdbGenresTable import IgdbGenresTable
from backend.tables.igdbPlatformsTable import IgdbPlatformsTable
from backend.tables.usersTable import UsersTable

class DatabaseCollection:
    def __init__(self):
        self.games = GamesTable()
        self.gamesLibrary = GamesLibraryTable()
        self.gamesBacklog = GamesBacklogTable()
        self.users = UsersTable()
        self.igdbGames = IgdbGamesTable()
        self.igdbGenres = IgdbGenresTable()
        self.igdbPlatforms = IgdbPlatformsTable()
        self.igdbGamesGenres = IgdbGamesGenresTable()
        self.igdbGamesPlatforms = IgdbGamesPlatformsTable()

    def add_game(self,game_name: str,igdb_game: Game) -> int:
        game_id = self.games.add_game(game_name,igdb_game.id)

        if igdb_game is None:
            return game_id

        if self.igdbGames.check_if_game_exists(igdb_game.id):
            return game_id

        self.igdbGames.add_game(igdb_game)

        genre_ids = list()
        platform_ids = list()

        for genre in igdb_game.genres[0]:
            if not self.igdbGenres.check_if_genre_exists(genre):
                self.igdbGenres.add_genre(genre)
                genre_ids.append(self.igdbGenres.get_genre_id(genre))

        for platform in igdb_game.platforms:
            if not self.igdbPlatforms.check_if_platform_exists(platform):
                self.igdbPlatforms.add_platform(platform)
                platform_ids.append(self.igdbPlatforms.get_platform_id(platform))

        for genre_id in genre_ids:
            self.igdbGamesGenres.add_entry(igdb_game.id,genre_id)

        for platform_id in platform_ids:
            self.igdbGamesPlatforms.add_entry(igdb_game.id,platform_id)

        return game_id

    def add_game_to_library(self, game_name:str, user_id:int, console:str, status:str, igdb_game:Game = None):
        game_id = self.games.get_game_id(game_name)

        if not game_id:
            game_id = self.add_game(game_name,igdb_game)

        self.gamesLibrary.add_game(user_id,game_id,console,status)