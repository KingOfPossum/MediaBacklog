import uvicorn

from Game import Game
from backend.tables.databaseCollection import DatabaseCollection
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

import sys
from pathlib import Path
sys.path.append(str(Path(__file__).parent.parent / "IGDB-PythonWrapper"))

from wrapper import IGDBWrapper
app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

wrapper = IGDBWrapper('m4nkh7koxu6lq6ndaj4bzs3n1148l5','3ksot8eb6syir0p3cz7i812xnwm0va')
database = DatabaseCollection()
@app.get("/games/gamesLibrary")
def get_library_games():
    return [game.to_dict() for game in database.get_all_games()]

@app.get("/games/{game_name}_{platform}_{status}")
def get_game(game_name: str, platform: str, status:str):
    game = Game.from_igdb(wrapper,game_name,platform)

    database.add_game_to_library(game_name,-1,platform,status,game)

    return {"game":game.cover}

if __name__ == '__main__':
    database.get_all_games()

    uvicorn.run(app, host="127.0.0.1",port=5049)