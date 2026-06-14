import type {ListboxItem} from "@nuxt/ui/components/Listbox.vue.d.ts";

const platforms: ListboxItem[] = ([
  {
    label: 'PC'
  },
  {
    label: 'NES'
  },
  {
    label: 'SNES'
  },
  {
    label: 'N64'
  },
  {
    label: 'GameCube'
  },
  {
    label: 'Wii'
  },
  {
    label: 'WiiU'
  },
  {
    label: 'Switch'
  },
  {
    label: 'Switch 2'
  },
  {
    label: 'GameBoy'
  },
  {
    label: 'GameBoy Advanced'
  },
  {
    label: 'DS'
  },
  {
    label: '3DS'
  },
  {
    label: 'PS1'
  },
  {
    label: 'PS2'
  },
  {
    label: 'PS3'
  },
  {
    label: 'PS4'
  },
  {
    label: 'PS5'
  },
  {
    label: 'PSP'
  },
  {
    label: 'PS Vita'
  },
  {
    label: 'Xbox'
  },
  {
    label: 'Xbox 360'
  },
  {
    label: 'Xbox One'
  },
  {
    label: 'Xbox Series X/S'
  },
]);

export const getPlatforms  = () => {return platforms;};
