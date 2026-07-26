from backend.GameType import GameType
from backend.HowLongToBeatEntry import HowLongToBeatEntry
from howlongtobeatpy import HowLongToBeat, SearchModifiers

class HowLongToBeatSearcher:
    @staticmethod
    def search(game_name: str, game_type: GameType = GameType.GAME) -> None | HowLongToBeatEntry:
        """
        Searches HowLongToBeat for informations about how long it takes to beat a specific game/dlc/mod.
        :param game_name: The name of the game to search for
        :param game_type: The type, game/dlc/mod
        :return: None if nothing was found else a HowLongToBeatEntry object containing the information
        """

        type_filter = SearchModifiers.NONE
        if game_type == GameType.DLC:
            type_filter = SearchModifiers.ISOLATE_DLC
        elif game_type == GameType.MOD:
            type_filter = SearchModifiers.ISOLATE_MODS

        results = HowLongToBeat().search(game_name, type_filter)

        if results is None or len(results) == 0:
            return None

        best_result = max(results, key=lambda element: element.similarity)

        return HowLongToBeatEntry(
            game_name=best_result.game_name,
            main_story=round(best_result.main_story),
            main_extra=round(best_result.main_extra),
            completionist=round(best_result.completionist),
            cover_url="https://howlongtobeat.com/games/" + best_result.json_content['game_image']
        )