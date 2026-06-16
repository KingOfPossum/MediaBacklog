import {games} from "~/data/gameData.ts"
import {platforms} from "~/data/platforms.ts"

export const useGames = () => {
  const platformFilter = useState('platformFilter',() => platforms.map((platform) => platform.label));
  const statusFilter = useState('statusFilter',() => ['Not Played','Started','Completed']);

  const {searchQuery} = mainLayout();

  const displayedGames = computed(() => {
    const query = searchQuery.value.toLowerCase();

    return games.value.filter((item) => {
      const matchesPlatform = item.platforms.some((platform) => platformFilter.value.includes(platform));
      const matchesStatus = statusFilter.value.includes(item.status);
      const matchesQuery = item.name.toLowerCase().includes(query);

      return matchesPlatform && matchesStatus && matchesQuery;
    })
  });

  return {platformFilter,statusFilter, displayedGames};
}
