<script setup lang="ts">
const items = ref([
  {
    id: 1,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3ddc.jpg'
  },
  {
    id: 2,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co1r7l.jpg'
  },
  {
    id: 3,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co70qn.jpg'
  },
  {
    id: 4,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co5eju.jpg'
  },
  {
    id: 5,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co87hw.jpg'
  },
  {
    id: 6,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3jky.jpg'
  },
  {
    id: 7,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co4a3e.jpg'
  },
  {
    id: 8,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co2gzc.jpg'
  },
  {
    id: 9,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/cobw10.jpg'
  },
  {
    id: 10,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co55n7.jpg'
  },
  {
    id: 11,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3koh.jpg'
  },
  {
    id: 12,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/cob1t0.jpg'
  },
  {
    id: 13,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co4990.jpg'
  },
  {
    id: 14,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/cob9xh.jpg'
  },
  {
    id: 15,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3p5n.jpg'
  },
  {
    id: 16,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co20mh.jpg'
  },
  {
    id: 17,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co2m9q.jpg'
  },
  {
    id: 18,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co2str.jpg'
  },
  {
    id: 19,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co6ya8.jpg'
  },
  {
    id: 20,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co6lxr.jpg'
  },
  {
    id: 21,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/coakuz.jpg'
  },
  {
    id: 22,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co5uzk.jpg'
  }
])
</script>

<template>
  <div class="relative h-screen flex flex-col items-center justify-center">
    <UContainer
      class="
              max-w-[90%]
              w-full
              h-[80%]
              p-4
              bg-accented
              rounded-lg
      "
    >
      <div class="
        h-full overflow-y-auto grid grid-cols-3 md:grid-cols-4 lg:grid-cols-5 xl:grid-cols-6 2xl:grid-cols-7 gap-5 content-start">
        <img
          v-for="item in items"
          :key="item.id"
          :src="item.img"
          class="
            rounded-2xl
            object-cover
            w-full
            aspect-3/4
            hover:scale-105
            transition-transform
          "
        />
      </div>
    </UContainer>
    <UButton
      class="absolute size-20 right-[10%] bottom-[15%]"
      icon="i-lucide:plus"
      size="xl"
    />
  </div>
</template>
