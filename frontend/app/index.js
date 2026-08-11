import {makeHTTPRequest} from "~/utils"
import {games} from "~/data/gameData.ts";

export const load_library = async () => {
  const library = await makeHTTPRequest('http://127.0.0.1:5049/games/gamesLibrary')
  console.log(library);

  games.value = []
  library.forEach((game) => {
    games.value.push({id:game['library_id'],name:game['name'],img:game['cover_url'],platforms:game['consoles'],status:game['status'],main_story:game['main_story_length']})
  });
}

