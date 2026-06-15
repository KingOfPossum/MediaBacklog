export const showGrid = ref(true);

export const changeDisplayType = () => {
  showGrid.value = !showGrid.value;
}

export const useSearchQuery = () => {
  const searchQuery = useState('searchQuery','');
  return {searchQuery};
}
