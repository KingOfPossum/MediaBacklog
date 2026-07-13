from dataclasses import dataclass

@dataclass
class IGDBEntry:
    id: int
    game_name: str
    cover_url: str
    description: str