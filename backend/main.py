import uvicorn

from backend.tables.gamesBacklogTable import GamesBacklogTable
from backend.tables.gamesLibraryTable import GamesLibraryTable
from backend.tables.gamesTable import GamesTable
from backend.tables.igdbGamesGenresTable import IgdbGamesGenresTable
from backend.tables.igdbGamesPlatformsTable import IgdbGamesPlatformsTable
from backend.tables.igdbGamesTable import IgdbGamesTable
from backend.tables.igdbGenresTable import IgdbGenresTable
from backend.tables.igdbPlatformsTable import IgdbPlatformsTable
from backend.tables.usersTable import UsersTable
from fastapi import FastAPI
app = FastAPI()

@app.get("/test")
def test():
    return {"Test":10}

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

    uvicorn.run(app, host="127.0.0.1",port=5049)