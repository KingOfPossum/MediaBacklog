<script setup lang="ts">
import {usePlatformFilter} from "~/composables/games";

definePageMeta({
  layout:'main-layout'
})

const {searchQuery} = useSearchQuery()
const {platformFilter} = usePlatformFilter()

watch(searchQuery, () => {
  filterDisplayedItems(searchQuery.value)
})

watch(platformFilter, () => {
  console.log("Filter changed : ",platformFilter.value)
  console.log("Current Search Query : ",searchQuery.value)

})
</script>

<template>
  <GameView />
</template>
