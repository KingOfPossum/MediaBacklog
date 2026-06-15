import {games} from "~/data/gameData.ts"
import {platforms} from "~/data/platforms.ts"
import {mainLayout} from "~/composables/mainLayout.js";

export const useGames = () => {
  const platformFilter = useState('platformFilter',() => {return platforms.map((platform) => platform.label)});

  const {searchQuery} = mainLayout();

  const displayedGames = computed(() => {
    const query = searchQuery.value.toLowerCase();

    return games.value.filter((item) => {
      const matchesPlatform = platformFilter.value.includes(item.platform);
      const matchesQuery = item.name.toLowerCase().includes(query);

      return matchesPlatform && matchesQuery;
    })
  });

  return {platformFilter, displayedGames};
}
