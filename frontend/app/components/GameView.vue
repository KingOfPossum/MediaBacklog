<script setup lang="ts">
  import {platforms} from "~/data/platforms"

  const {displayedGames} = useGames();
  const {addGameName,platformSelect,statusSelect,modalOpen,loading_request,addGame} = addGameModal();
  const {showGrid} = mainLayout();
</script>

<template>
  <div class="relative h-screen flex flex-col mt-[3%]">
    <UContainer
      class="
              max-w-[90%]
              w-full
              h-[85%]
              p-4
              bg-accented
              rounded-lg
      "
    >
      <div v-if="showGrid" class="h-full scrollbar-none overflow-y-auto overflow-x-visible grid grid-cols-3 md:grid-cols-4 lg:grid-cols-5 xl:grid-cols-6 2xl:grid-cols-7 gap-4 content-start">
        <div class="flex flex-col" v-for="item in displayedGames" :key="item.id">
          <div class="p-[5%]">
            <img
              :src="item.img"
              :alt="item.name"
              class="
                rounded-2xl
                object-cover
                w-full
                aspect-3/4
                hover:scale-105
                transition-transform
                bg-gray-400
              "
            />
          </div>
          <div class="flex flex-row pl-1">
            <!--
              <img class="hover:scale-115 transition-transform size-8 object-contain bg-gray-500 rounded-lg ml-1 border border-black" v-for="platform in item.platforms" :src="`platformIcons/${platform.toLowerCase().replace(' ','')}.png`" :title="platform">
            -->
            <ConsoleIcon console="GameCube" size="64"/>
            <ConsoleIcon console="NES" size="64"/>
            <ConsoleIcon console="WiiU_2" size="64"/>
            <ConsoleIcon console="SNES" size="64"/>
            <ConsoleIcon console="PS1" size="64"/>
            <ConsoleIcon console="GameBoy" size="64"/>
            <ConsoleIcon console="GameBoyAdvance" size="64"/>
            <ConsoleIcon console="Switch" size="64"/>
            <ConsoleIcon console="Switch2" size="64"/>
          </div>
        </div>
      </div>

      <div v-if="!showGrid" class="h-full overflow-y-auto">
        <UTable :data="displayedGames" class="flex-1" >
          <template #img-cell="{row}">
            <img class="rounded-2xl size-[10%]" :src="row.original.img"/>
          </template>
        </UTable>
      </div>
    </UContainer>

    <UModal title="Add Game" :dismissible="false" :open="modalOpen">
      <UButton
        class="absolute size-20 right-[10%] bottom-[20%] pl-7 shadow-2xl drop-shadow-2xl z-10 outline-2 outline-emerald-600"
        icon="i-lucide:plus"
        size="xl"
        @click="modalOpen = true;"
      />

      <template #body>
        <div class="flex flex-col">
          <div class="flex flex-col">
            <span>Name:</span>
            <UInput v-model="addGameName" />
          </div>
          <div class="mt-3 flex flex-col">
            <span>Platform:</span>
            <USelect v-model="platformSelect" :items="platforms.map((platform) => platform.label)"/>
          </div>
          <div class="mt-3 flex flex-col">
            <span>Status:</span>
            <USelect v-model="statusSelect" :items="['Not Played','Started','Completed']" />
          </div>
          <UButton @click="addGame" :loading="loading_request" class="mt-10 w-30 h-10 pl-9">Submit</UButton>
        </div>
      </template>
    </UModal>
  </div>
</template>
