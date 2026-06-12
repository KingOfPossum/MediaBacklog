<script setup lang="ts">
import MainSidebar from "~/components/MainSidebar.vue";

const {searchQuery} = mainLayoutSearchbar()
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
            <UButton icon="i-lucide:list" color="neutral" variant="outline" size="xl" />
          </template>
        </UHeader>

        <slot />
      </div>
    </div>
  </div>
</template>
