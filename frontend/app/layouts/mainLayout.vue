<script setup lang="ts">
import MainSidebar from "~/components/MainSidebar.vue";

const {searchQuery} = mainLayoutSearchbar();
const {showGrid, changeDisplayType} = gridDisplayTypeButton();

const filterValue = ref();
</script>

<template>
  <div class="h-screen flex overflow-hidden">
    <div class="flex flex-1">
      <MainSidebar />

      <div class="w-full">
        <UHeader>
          <template #left>
            <span>Filter</span>
            <UPopover arrow mode="click" :content="{align: 'center', side: 'bottom', sideOffset: 8}">
              <UButton icon="i-lucide:filter" color="neutral" variant="outline" size="xl"/>

              <template #content>
                <div class="flex flex-row">
                  <UCheckboxGroup legend="Platform" v-model="filterValue" :items="getPlatforms().map((platform) => platform.label)"/>
                </div>
              </template>
            </UPopover>
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
