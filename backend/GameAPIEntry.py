from dataclasses import dataclass
from backend.Status import Status

@dataclass
class GameAPIEntry:
    name: str
    library_id: int
    user_id: int
    consoles: list[str]
    cover_url: str
    genres: list[str]
    platforms: list[str]
    game_length: int
    min_price: int
    max_price: int
    avg_price: int
    status: Status

    def to_dict(self) -> dict:
        return {
            'name':self.name,
            'library_id':self.library_id,
            'user_id':self.user_id,
            'consoles':self.consoles,
            'cover_url':self.cover_url,
            'genres':self.genres,
            'platforms':self.platforms,
            'game_length':self.game_length,
            'min_price':self.min_price,
            'max_price':self.max_price,
            'avg_price':self.avg_price,
            'status':self.status
        }