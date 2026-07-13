from backend.Status import Status
from dataclasses import dataclass

@dataclass
class GameLibraryEntry:
    id: int
    user_id: int
    game_id: int
    console: str
    status: Status

    def __str__(self):
        return f"""
GameLibraryEntry:
    id: {self.id}
    user_id: {self.user_id}
    game_id: {self.game_id}
    console: {self.console}
    status: {self.status}
        """