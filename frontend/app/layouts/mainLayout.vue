<script setup lang="ts">
import MainSidebar from "~/components/MainSidebar.vue";

const {searchQuery} = mainLayoutSearchbar();
const {showGrid, changeDisplayType} = gridDisplayTypeButton();
</script>

<template>
  <div class="h-screen flex overflow-hidden">
    <div class="flex flex-1">
      <MainSidebar />

      <div class="w-full">
        <UHeader>
          <template #left>
            <span>Filter</span>
            <UButton icon="i-lucide:filter" color="neutral" variant="outline" size="xl"/>
          </template>

          <template #default>
            <UInput v-model="searchQuery" class="w-full" size="xl" placeholder="Search..." color="neutral" variant="outline" icon="i-lucide-search"/>
          </template>

          <template #right>
            <UButton @click="changeDisplayType" :icon="showGrid ? 'i-lucide:list' : 'i-lucide:layout-grid'" color="neutral" variant="outline" size="xl" />
          </template>
        </UHeader>

        <slot />
      </div>
    </div>
  </div>
</template>
