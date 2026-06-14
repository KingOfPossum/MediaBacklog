<script setup lang="ts">
definePageMeta({
  layout:'main-layout'
})

const {searchQuery} = mainLayoutSearchbar();

watch(searchQuery, () => {
  const items = getFilteredItems();
  const searchQueryLower = searchQuery.value.toLowerCase();

  const filteredItems = items.filter((item) => {return item.name.toLowerCase().includes(searchQueryLower)});
  setDisplayedItems(filteredItems);
})
</script>

<template>
  <GameView />
</template>
