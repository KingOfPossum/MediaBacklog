import {games} from "~/data/gameData"
import {makeHTTPRequest} from "~/utils"

export const addGameModal = () => {
  const addGameName = useState('addGameName',() => '');
  const platformSelect = useState('platformSelect',() => '');
  const statusSelect = useState('statusSelect',() => 'Not Played');
  const modalOpen = useState('modalOpen',() => false);
  const loading_request = useState('loading_request',() => false);

  const addGame = async () => {
    if(games.value.some(game => game.name.toLowerCase() === addGameName.value.toLowerCase())){
      games.value.forEach((game) => {
        if(game.name.toLowerCase().replace(' ','') === addGameName.value.toLowerCase().replace(' ','')) {
          if(game.platforms.includes(platformSelect.value) === false){
            game.platforms.push(platformSelect.value);
          }
        }
      })
    }
    else {
      loading_request.value = true;
      const result = await(makeHTTPRequest(`http://127.0.0.1:5049/games/${addGameName.value}_${platformSelect.value}_${statusSelect.value}`))
      games.value.push({
        id:games.value.length+1,
        name: addGameName.value,
        platforms:[platformSelect.value],
        status:statusSelect.value,
        img: result['game']
      })
      loading_request.value = false;
    }

    modalOpen.value = false;
    addGameName.value = '';
    platformSelect.value = '';
    statusSelect.value = 'Not Played';
  }

  return {addGameName,platformSelect,statusSelect,modalOpen,loading_request,addGame};
}
