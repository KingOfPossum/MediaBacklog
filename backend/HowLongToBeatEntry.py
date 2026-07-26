from dataclasses import dataclass

@dataclass
class HowLongToBeatEntry:
    game_name: str
    main_story: int
    main_extra: int
    completionist: int
    cover_url: str

    def __str__(self) -> str:
        return f"""
HowLongToBeat Entry:
    game_name: {self.game_name}
    main_story: {self.main_story} Hours
    main_extra: {self.main_extra} Hours
    completionist: {self.completionist} Hours
    cover_url: {self.cover_url}
    """