const items = ref([
  {
    id: 1,
    name: 'God of War',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3ddc.jpg',
    platform: 'PS2'
  },
  {
    id: 2,
    name: 'Similarity',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co1r7l.jpg',
    platform: 'PC'
  },
  {
    id: 3,
    name: 'Donkey Kong Country',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co70qn.jpg',
    platform: 'SNES'
  },
  {
    id: 4,
    name: 'Metal Gear Solid 4',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co5eju.jpg',
    platform: 'PS3'
  },
  {
    id: 5,
    name: 'Jet Set Radio Future',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co87hw.jpg',
    platform: 'XBOX'
  },
  {
    id: 6,
    name: 'Rayman 3 Hoodlum Havoc',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3jky.jpg',
    platform: 'GameCube'
  },
  {
    id: 7,
    name: 'Rabbids Go Home',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co4a3e.jpg',
    platform: 'Wii'
  },
  {
    id: 8,
    name: 'Ape Escape',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co2gzc.jpg',
    platform: 'PS1'
  },
  {
    id: 9,
    name: 'Killer7',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/cobw10.jpg',
    platform: 'GameCube'
  },
  {
    id: 10,
    name: 'God Hand',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co55n7.jpg',
    platform: 'PS2'
  },
  {
    id: 11,
    name: 'God of War III',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3koh.jpg',
    platform: 'PS3'
  },
  {
    id: 12,
    name: 'Sonic Mania',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/cob1t0.jpg',
    platform: 'PC'
  },
  {
    id: 13,
    name: 'Dark Souls III',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co4990.jpg',
    platform: 'PC'
  },
  {
    id: 14,
    name: 'Metroid Prime 4 Beyond',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/cob9xh.jpg',
    platform: 'Switch 2'
  },
  {
    id: 15,
    name: 'Doom Eternal',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3p5n.jpg',
    platform: 'PC'
  },
  {
    id: 16,
    name: 'Mortal Kombat 11',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co20mh.jpg',
    platform: 'PC'
  },
  {
    id: 17,
    name: 'One Piece Pirate Warriors 4',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co2m9q.jpg',
    platform: 'PC'
  },
  {
    id: 18,
    name: 'Ratchet & Clank Rift Apart',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co2str.jpg',
    platform: 'PS5'
  },
  {
    id: 19,
    name: 'Bomb Rush Cyberfunk',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co6ya8.jpg',
    platform: 'PC'
  },
  {
    id: 20,
    name: 'Lies of P',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co6lxr.jpg',
    platform: 'PC'
  },
  {
    id: 21,
    name: 'Metal Slug 3',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/coakuz.jpg',
    platform: 'PC'
  },
  {
    id: 22,
    name: 'Star Wars Jedi Survivor',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co5uzk.jpg',
    platform: 'PC'
  }
])

export const displayedItems = ref(items.value)

export const filterDisplayedItems = (searchQuery) => {
  const searchQueryL = searchQuery.toLowerCase();
  const newItems = items.value

  newItems = newItems.filter((item) => {return platformFilter.value.includes(item.platform)});
  newItems = newItems.filter((item) => {return item.name.toLowerCase().includes(searchQueryL)})

  displayedItems.value = newItems;
}

export const usePlatformFilter = () => {
  const platformFilter = useState('platformFilter',() => {getPlatforms().map((platform) => platform.label)});
  return {platformFilter};
}
