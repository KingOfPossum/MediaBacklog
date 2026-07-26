from dataclasses import dataclass

@dataclass
class GameEntry:
    id: int
    igdb_id: int
    name: str
    howlongtobeat_cover_url: str
    main_story_length: int
    main_extra_length: int
    completionist_length: int
    min_price: int
    avg_price: int
    max_price: int

    def __str__(self):
        return f"""
GameEntry:
    id: {self.id}
    igdb_id: {self.igdb_id}
    name: {self.name}
    howlongtobeat_cover_url: {self.howlongtobeat_cover_url}
    main_story_length: {self.main_story_length}
    main_extra_length: {self.main_extra_length}
    completionist_length: {self.completionist_length}
    min_price: {self.min_price}
    avg_price: {self.avg_price}
    max_price: {self.max_price}
        """