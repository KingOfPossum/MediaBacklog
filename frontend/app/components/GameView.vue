<script setup lang="ts">
  const {showGrid} = gridDisplayTypeButton();
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
        class="h-full overflow-y-auto overflow-x-visible grid grid-cols-3 md:grid-cols-4 lg:grid-cols-5 xl:grid-cols-6 2xl:grid-cols-7 gap-5 content-start"
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
            <img class="size-[20%]" :src="row.original.img"/>
          </template>
        </UTable>
      </div>
    </UContainer>
    <UButton
      class="absolute size-20 right-[10%] bottom-[20%] pl-7 shadow-2xl drop-shadow-2xl z-10 outline-2 outline-emerald-600"
      icon="i-lucide:plus"
      size="xl"
    />
  </div>
</template>
