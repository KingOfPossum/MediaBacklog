export const gridDisplayTypeButton = () => {
  const showGrid = useState('showGrid', () => true);

  const changeDisplayType = () => {
    showGrid.value = !showGrid.value;
  }

  return {showGrid, changeDisplayType};
}
