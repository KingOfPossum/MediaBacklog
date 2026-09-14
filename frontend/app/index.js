import {makeHTTPGetRequest, makeHTTPPostRequest} from "~/utils"
import {games} from "~/data/gameData.ts";

export const load_library = async () => {
  //const library = await makeHTTPGetRequest('http://127.0.0.1:5049/games/gamesLibrary')
  const library = await makeHTTPGetRequest('http://localhost:1234/getLibrary');
  console.log(library);

  games.value = []
  library.forEach((game) => {
    games.value.push({id:game['library_id'],name:game['game_infos']['name'],img:game['igdb_infos']['cover_url'],platforms:game['consoles'],status:game['status'],main_story:game['game_infos']['main_story_length']})
  });

  console.log(games.value);
}

