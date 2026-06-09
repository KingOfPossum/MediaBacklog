<script setup lang="ts">
import GameView from "~/components/GameView.vue";
import MainSidebar from "~/components/MainSidebar.vue";
</script>

<template>
  <div class="h-screen flex overflow-hidden">
    <div class="flex flex-1">
      <MainSidebar />

      <div>
        <div class="mt-5 ml-20 mr-20 flex">
          <span class="pt-1 mr-3">Filter</span>
          <UButton icon="i-lucide:filter" color="neutral" variant="outline" size="xl"/>
          <UInput class="w-[50%] ml-[20%]" placeholder="Search..." color="neutral" variant="outline" icon="i-lucide-search"/>
          <UButton class="ml-[20%]" icon="i-lucide:list" color="neutral" variant="outline" size="xl" />
        </div>

        <GameView />
      </div>
    </div>
  </div>
</template>
