import {games} from "~/data/gameData"

export const addGameModal = () => {
  const addGameName = useState('addGameName',() => '');
  const platformSelect = useState('platformSelect',() => '');
  const statusSelect = useState('statusSelect',() => 'Not Played');
  const modalOpen = useState('modalOpen',() => false);

  const addGame = () => {
    console.log(modalOpen.value);

    games.value.push({
      id:games.value.length+1,
      name: addGameName.value,
      platforms:[platformSelect.value],
      img:''
    })

    modalOpen.value = false;
    addGameName.value = '';
    platformSelect.value = '';
    statusSelect.value = 'Not Played';

    console.log(modalOpen.value);
  }

  return {addGameName,platformSelect,statusSelect,modalOpen,addGame};
}
