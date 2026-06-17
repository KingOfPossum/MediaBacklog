from backend.gamesBacklogTable import GamesBacklogTable
from backend.gamesLibraryTable import GamesLibraryTable
from backend.gamesTable import GamesTable
from backend.igdbGamesGenresTable import IgdbGamesGenresTable
from backend.igdbGamesPlatforms import IgdbGamesPlatformsTable
from backend.igdbGamesTable import IgdbGamesTable
from backend.igdbGenresTable import IgdbGenresTable
from backend.igdbPlatformsTable import IgdbPlatformsTable
from backend.users import UsersTable

if __name__ == '__main__':
    games = GamesTable()
    gamesLibrary = GamesLibraryTable()
    gamesBacklog = GamesBacklogTable()
    users = UsersTable()
    igdbGames = IgdbGamesTable()
    igdbGenres = IgdbGenresTable()
    igdbPlatforms = IgdbPlatformsTable()
    igdbGamesGenres = IgdbGamesGenresTable()
    igdbGamesPlatforms = IgdbGamesPlatformsTable()