<script setup lang="ts">
import MainSidebar from "~/components/MainSidebar.vue";
</script>

<template>
  <div class="h-screen flex overflow-hidden">
    <div class="flex flex-1">
      <MainSidebar />

      <div class="w-full">
        <UHeader title="">
          <template #left>
            <span>Filter</span>
            <UButton icon="i-lucide:filter" color="neutral" variant="outline" size="xl"/>
          </template>

          <template #default>
            <UInput class="w-30vw" size="xl" placeholder="Search..." color="neutral" variant="outline" icon="i-lucide-search"/>
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
