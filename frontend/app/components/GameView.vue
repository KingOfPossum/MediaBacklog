<script setup lang="ts">
  let loading = ref(false);

  const selectValue = ref();
  const statusValue = ref();

  const addGame = () => {
    loading.value = true;
    setTimeout(() => {
      loading.value = false;
    }, 1000);
  }
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
      <div
        v-if="showGrid"
        class="h-full scrollbar-none overflow-y-auto overflow-x-visible grid grid-cols-3 md:grid-cols-4 lg:grid-cols-5 xl:grid-cols-6 2xl:grid-cols-7 gap-5 content-start"
      >
        <img
          v-for="item in displayedItems"
          :key="item.id"
          :src="item.img"
          class="
            rounded-2xl
            object-cover
            w-full
            aspect-3/4
            hover:scale-105
            transition-transform
            p-[3%]
          "
        />
      </div>

      <div
        v-if="!showGrid"
        class="h-full overflow-y-auto"
      >
        <UTable :data="displayedItems" class="flex-1" >
          <template #img-cell="{row}">
            <img class="rounded-2xl size-[10%]" :src="row.original.img"/>
          </template>
        </UTable>
      </div>
    </UContainer>

    <UModal title="Add Game">
      <UButton
        class="absolute size-20 right-[10%] bottom-[20%] pl-7 shadow-2xl drop-shadow-2xl z-10 outline-2 outline-emerald-600"
        icon="i-lucide:plus"
        size="xl"
      />

      <template #body>
        <div class="flex flex-col">
          <div class="flex flex-col">
            <span>Name:</span>
            <UInput />
          </div>
          <div class="mt-3 flex flex-col">
            <span>Platform:</span>
            <USelect :v-model="selectValue" :items="getPlatforms().map((platform) => platform.label)"/>
          </div>
          <div class="mt-3 flex flex-col">
            <span>Status:</span>
            <USelect :v-model="statusValue" :items="['Not played','Started','Completed']" />
          </div>
          <UButton @click="addGame" :loading="loading" class="mt-10 w-30 h-10 pl-9">Submit</UButton>
        </div>
      </template>
    </UModal>
  </div>
</template>
