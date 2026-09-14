import {games} from "~/data/gameData"
import {makeHTTPGetRequest, makeHTTPPostRequest} from "~/utils"

export const addGameModal = () => {
  const addGameName = useState('addGameName',() => '');
  const platformSelect = useState('platformSelect',() => '');
  const statusSelect = useState('statusSelect',() => 'Not Played');
  const modalOpen = useState('modalOpen',() => false);
  const loading_request = useState('loading_request',() => false);

  const addGame = async () => {
    const data = {'game':addGameName.value, 'platform':platformSelect.value, 'status':statusSelect.value};
    const game_data = await makeHTTPPostRequest('http://localhost:1234/game',data);

    if(game_data === undefined || game_data['error']) {
      return;
    }

    const existing_game = games.value.find(game => game.name === game_data['game_infos']['name']);

    if(existing_game) {
      existing_game['platforms'] = game_data['consoles'];
    }
    else {
      games.value.push({
        id: game_data['library_id'],
        name: game_data['game_infos']['name'],
        img: game_data['igdb_infos']['cover_url'],
        platforms: game_data['consoles'],
        status: game_data['status'],
        main_story: game_data['game_infos']['main_story_length']
      });
    }

    modalOpen.value = false;
    addGameName.value = '';
    platformSelect.value = '';
    statusSelect.value = 'Not Played';
  }

  return {addGameName,platformSelect,statusSelect,modalOpen,loading_request,addGame};
}
