const items = ref([
  {
    id: 1,
    name: 'God of War',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3ddc.jpg'
  },
  {
    id: 2,
    name: 'Similarity',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co1r7l.jpg'
  },
  {
    id: 3,
    name: 'Donkey Kong Country',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co70qn.jpg'
  },
  {
    id: 4,
    name: 'Metal Gear Solid 4',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co5eju.jpg'
  },
  {
    id: 5,
    name: 'Jet Set Radio Future',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co87hw.jpg'
  },
  {
    id: 6,
    name: 'Rayman 3 Hoodlum Havoc',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3jky.jpg'
  },
  {
    id: 7,
    name: 'Rabbids Go Home',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co4a3e.jpg'
  },
  {
    id: 8,
    name: 'Ape Escape',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co2gzc.jpg'
  },
  {
    id: 9,
    name: 'Killer7',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/cobw10.jpg'
  },
  {
    id: 10,
    name: 'God Hand',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co55n7.jpg'
  },
  {
    id: 11,
    name: 'God of War III',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3koh.jpg'
  },
  {
    id: 12,
    name: 'Sonic Mania',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/cob1t0.jpg'
  },
  {
    id: 13,
    name: 'Dark Souls III',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co4990.jpg'
  },
  {
    id: 14,
    name: 'Metroid Prime 4 Beyond',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/cob9xh.jpg'
  },
  {
    id: 15,
    name: 'Doom Eternal',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3p5n.jpg'
  },
  {
    id: 16,
    name: 'Mortal Kombat 11',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co20mh.jpg'
  },
  {
    id: 17,
    name: 'One Piece Pirate Warriors 4',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co2m9q.jpg'
  },
  {
    id: 18,
    name: 'Ratchet & Clank Rift Apart',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co2str.jpg'
  },
  {
    id: 19,
    name: 'Bomb Rush Cyberfunk',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co6ya8.jpg'
  },
  {
    id: 20,
    name: 'Lies of P',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co6lxr.jpg'
  },
  {
    id: 21,
    name: 'Metal Slug 3',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/coakuz.jpg'
  },
  {
    id: 22,
    name: 'Star Wars Jedi Survivor',
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co5uzk.jpg'
  }
])

export var displayedItems = ref(items.value)

export const getItems = () => {return items.value;};
export const setDisplayedItems = (newItems) => {displayedItems.value = newItems;};
