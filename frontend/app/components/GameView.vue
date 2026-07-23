<script setup lang="ts">
  import {platforms} from "~/data/platforms"

  const {displayedGames} = useGames();
  const {addGameName,platformSelect,statusSelect,modalOpen,loading_request,addGame} = addGameModal();
  const {showGrid} = mainLayout();

  const testConsoles = ['GameCube','XBOX','XBOXONE','NES','GameCube_2','WiiU_3','Switch2','XBOX360','Switch','GameBoy','GameBoyAdvance','PS1','N64','DS','3DS','Wii','SNES']
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
            <div class="relative hover:scale-105 transition-transform">
              <img
                :src="item.img"
                :alt="item.name"
                class="
                  rounded-2xl
                  object-cover
                  w-full
                  aspect-3/4
                  bg-gray-400
                "
              />
              <img
                src="/GameBorder.svg"
                class="absolute inset-0 w-full h-full rounded-2xl pointer-events-none"
              />
            </div>
          </div>
          <div class="flex flex-row flex-wrap ml-6">
            <div v-for="platform in item.platforms" class="mr-5">
              <ConsoleIcon :console="platforms.find((elem) => elem.label == platform)?.console_icon" size="50"/>
            </div>
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
