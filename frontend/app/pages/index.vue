<script setup lang="ts">
import GameView from "~/components/GameView.vue";
import MainSidebar from "~/components/MainSidebar.vue";
</script>

<template>
  <div class="h-screen flex">
    <div class="flex flex-1">
      <MainSidebar />

      <div>
        <div class="mt-5 ml-20 mr-20 flex">
          <span class="pt-1 mr-3">Filter</span>
          <UButton icon="i-lucide:filter" color="neutral" variant="outline" size="xl"/>
          <UInput class="w-[20%] ml-[30%]" placeholder="Search..." color="neutral" variant="outline" icon="i-lucide-search"/>
          <UButton class="ml-[32%]" icon="i-lucide:list" color="neutral" variant="outline" size="xl" />
          <UButton class="ml-[8%]" icon="i-lucide:plus" color="primary" variant="solid" size="xl"/>
        </div>

        <GameView />
      </div>
    </div>
  </div>
</template>
