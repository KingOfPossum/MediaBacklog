export const mainLayout = () => {
  const searchQuery = useState('searchQuery',() => {return ''});
  const showGrid = useState('showGrid',() => {return true});

  const changeDisplayType = () => {
    showGrid.value = !showGrid.value;
  };

  return {searchQuery, showGrid, changeDisplayType};
}
