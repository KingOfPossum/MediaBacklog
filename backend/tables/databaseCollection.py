from Game import Game
from backend.GameAPIEntry import GameAPIEntry
from backend.tables.gamesBacklogTable import GamesBacklogTable
from backend.tables.gamesLibraryConsoleTable import GamesLibraryConsoleTable
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
        self.gamesLibraryConsole = GamesLibraryConsoleTable()
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

        library_id = self.gamesLibrary.get_library_id(user_id,game_id)
        if not library_id:
            library_id = self.gamesLibrary.add_game(user_id,game_id,console,status)

        if not self.gamesLibraryConsole.check_if_entry_already_exists(library_id,console):
            self.gamesLibraryConsole.add_entry(library_id, console)

    def get_all_games(self) -> list[GameAPIEntry]:
        library_games = self.gamesLibrary.get_all_games()

        games = list()

        for game in library_games:
            game_entry = self.games.get_game(game.game_id)
            igdb_entry = self.igdbGames.get_entry(game_entry.igdb_id)

            platforms = [self.igdbPlatforms.get_platform_name(platform) for platform in self.igdbGamesPlatforms.get_platforms_for_game(game_entry.igdb_id)]
            genres = [self.igdbGenres.get_genre_name(genre) for genre in self.igdbGamesGenres.get_genres_for_game(game_entry.igdb_id)]

            consoles = [x[0] for x in self.gamesLibraryConsole.get_all_consoles_for_game(game.id)]

            entry = GameAPIEntry(
                name=game_entry.name,
                library_id=game.id,
                user_id=game.user_id,
                consoles=consoles,
                cover_url=igdb_entry.cover_url,
                genres=genres,
                platforms=platforms,
                game_length=game_entry.game_length,
                min_price=game_entry.min_price,
                avg_price=game_entry.avg_price,
                max_price=game_entry.max_price,
                status=game.status
            )

            games.append(entry)

        return games