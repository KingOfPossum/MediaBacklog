from dataclasses import dataclass

@dataclass
class GameEntry:
    id: int
    igdb_id: int
    name: str
    game_length: int
    min_price: int
    avg_price: int
    max_price: int

    def __str__(self):
        return f"""
GameEntry:
    id: {self.id}
    igdb_id: {self.igdb_id}
    name: {self.name}
    game_length: {self.game_length}
    min_price: {self.min_price}
    avg_price: {self.avg_price}
    max_price: {self.max_price}
        """