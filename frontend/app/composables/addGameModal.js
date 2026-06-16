import {games} from "~/data/gameData"

export const addGameModal = () => {
  const addGameName = useState('addGameName',() => '');
  const platformSelect = useState('platformSelect',() => '');
  const statusSelect = useState('statusSelect',() => 'Not Played');
  const modalOpen = useState('modalOpen',() => false);

  const addGame = () => {
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
      games.value.push({
        id:games.value.length+1,
        name: addGameName.value,
        platforms:[platformSelect.value],
        status:statusSelect.value,
        img:''
      })
      console.log("ASDHkaj");
    }


    modalOpen.value = false;
    addGameName.value = '';
    platformSelect.value = '';
    statusSelect.value = 'Not Played';
  }

  return {addGameName,platformSelect,statusSelect,modalOpen,addGame};
}
