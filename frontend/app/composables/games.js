import {games} from "~/data/gameData.ts"
import {platforms} from "~/data/platforms.ts"

export const useGames = () => {
  const platformFilter = useState('platformFilter',() => {return platforms.map((platform) => platform.label)});

  const {searchQuery} = mainLayout();

  const displayedGames = computed(() => {
    const query = searchQuery.value.toLowerCase();

    return games.value.filter((item) => {
      const matchesPlatform = item.platforms.some((platform) => platformFilter.value.includes(platform));
      const matchesQuery = item.name.toLowerCase().includes(query);

      return matchesPlatform && matchesQuery;
    })
  });

  return {platformFilter, displayedGames};
}
